import fileinput
import re
from enum import Enum
from collections import deque
from collections import namedtuple

# input coming in from component

class Pulse(Enum):
    LOW = 0
    HIGH = 1

class State(Enum):
    OFF = 0
    ON = 1

Event = namedtuple('Event', ['name', 'input', 'module'])
    
BROADCASTER = re.compile('broadcaster -> (.*)\n')
CONJUNCTION = re.compile('&(\w+) -> (.*)\n')
FLIPFLOP = re.compile('%(\w+) -> (.*)\n')

class PulseCounter:
    def __init__(self):
        self.num_high = 0
        self.num_low = 0
        
    def count_pulse(self, pulse):
        if pulse == Pulse.HIGH:
            self.num_high += 1
        else:
            self.num_low += 1

class Broadcaster:
    def __init__(self, name, outputs, counter, modules):
        self.name = name
        self.outputs = outputs
        self.counter = counter
        self.modules = modules

    def add_connection(self, name):
        pass
    
    def input(self, name, pulse):
        counter.count_pulse(pulse)
        for output in self.outputs:
            if output in self.modules:
                queue.append(Event(self.name, Pulse.LOW, self.modules[output]))
            else:
                counter.count_pulse(Pulse.LOW)

class Conjunction:
    def __init__(self, name, outputs, counter, modules):
        self.name = name
        self.outputs = outputs
        self.counter = counter
        self.modules = modules
        self.connections = []
        self.inputs = []

    def add_connection(self, name):
        self.connections.append(name)
        self.inputs.append(Pulse.LOW)

    def input(self, name, pulse):
        counter.count_pulse(pulse)
        # all high send low, else high
        output_pulse = Pulse.LOW
        for i, connection in enumerate(self.connections):
            if connection == name:
                self.inputs[i] = pulse
            if self.inputs[i] == Pulse.LOW:
                output_pulse = Pulse.HIGH

        for output in self.outputs:
            if output in self.modules:
                queue.append(Event(self.name, output_pulse, self.modules[output]))
            else:
                counter.count_pulse(output_pulse)

class Flipflop:
    def __init__(self, name, outputs, counter, modules):
        self.name = name
        self.outputs = outputs
        self.counter = counter
        self.modules = modules
        self.state = State.OFF

    def add_connection(self, name):
        pass

    def input(self, name, pulse):
        counter.count_pulse(pulse)
        if pulse == Pulse.LOW:
            self.state = State.ON if self.state == State.OFF else State.OFF
            output_pulse = Pulse.LOW if self.state == State.OFF else Pulse.HIGH
            for output in self.outputs:
                if output in self.modules:
                    queue.append(Event(self.name, output_pulse, self.modules[output]))
                else:
                    counter.count_pulse(output_pulse)

modules = {}
counter = PulseCounter()
for line in fileinput.input():
    match = BROADCASTER.match(line)
    if match:
        modules['broadcaster'] = Broadcaster('broadcaster', match.group(1).split(', '), counter, modules)

    match = CONJUNCTION.match(line)
    if match:
        modules[match.group(1)] = Conjunction(match.group(1), match.group(2).split(', '), counter, modules)

    match = FLIPFLOP.match(line)
    if match:
        modules[match.group(1)] = Flipflop(match.group(1), match.group(2).split(', '), counter, modules)

for i, (k, v) in enumerate(modules.items()):
    for output in v.outputs:
        if output in modules:
            modules[output].add_connection(k)

queue = deque()
for x in range(1000):
    queue.append(Event('button', Pulse.LOW, modules['broadcaster']))
    while queue:
        event = queue.popleft()
        event.module.input(event.name, event.input)
print(counter.num_low, counter.num_high, counter.num_high * counter.num_low)    
