"""
  Copyright 2005 Allen B. Downey

    This file contains an example program from The Little Book of
    Semaphores, available from Green Tea Press, greenteapress.com

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, see http://www.gnu.org/licenses/gpl.html
    or write to the Free Software Foundation, Inc., 51 Franklin St, 
    Fifth Floor, Boston, MA  02110-1301  USA
"""

from threading_cleanup import *
import random

class Shared:
    def __init__(self, start=5):
        self.num_consumers = 0
        self.num_producers = 0
        self.cokes = start
        self.mutex = Semaphore(1)
        self.can_consume = Semaphore(0)
        self.can_produce = Semaphore(0)

def consume(shared):
    shared.mutex.wait()
    if shared.cokes == 0:
        shared.num_consumers += 1
        shared.mutex.signal()
        shared.can_consume.wait()               
    else:
        if shared.cokes == 10 and shared.num_producers > 0:
            shared.can_produce.signal(shared.num_producers)
            shared.num_producers = 0
        shared.cokes -= 1
        print(shared.cokes)
        shared.mutex.signal()
        
def produce(shared):
    shared.mutex.wait()
    if shared.cokes == 10:
        shared.num_producers += 1
        shared.mutex.signal()
        shared.can_produce.wait()               
    else:
        if shared.cokes == 0 and shared.num_consumers > 0:
            shared.can_consume.signal(shared.num_consumers)
            shared.num_consumers = 0
        shared.cokes += 1
        print(shared.cokes)
        shared.mutex.signal()

def loop(shared, f, mu=1):
    while True:
        t = random.expovariate(1.0/mu)
        time.sleep(t)
        f(shared)

shared = Shared()
fs = [consume]*2 + [produce]*2 
threads = [Thread(loop, shared, f) for f in fs]
for thread in threads: thread.join()
