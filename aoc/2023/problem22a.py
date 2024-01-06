import fileinput
import re
from dataclasses import dataclass
from itertools import groupby

@dataclass 
class Point:
    x: int
    y: int
    z: int

@dataclass 
class Line:
    points: list[Point]
    num_supports: int

def is_vertical(line):
    return len(line.points) > 1 and line.points[0].z != line.points[1].z

POINTS = re.compile('(\d+),(\d+),(\d+)~(\d+),(\d+),(\d+)')

# lowest z first
def make_line(p1, p2):
    points = []
    if p1.x != p2.x:
        for i in range(min(p1.x, p2.x), max(p1.x, p2.x) + 1):
            points.append(Point(i, p1.y, p2.z))
    elif p1.y != p2.y:
        for i in range(min(p1.y, p2.y), max(p1.y, p2.y) + 1):
            points.append(Point(p1.x, i, p1.z))
    else:
        for i in range(min(p1.z, p2.z), max(p1.z, p2.z) + 1):
            points.append(Point(p1.x, p2.y, i))
    return Line(points, 0)

lines = []
height_map = {}
support_map = {}
for line in fileinput.input():
    match = POINTS.match(line)
    lines.append(make_line(Point(int(match.group(1)), int(match.group(2)), int(match.group(3))),
                           Point(int(match.group(4)), int(match.group(5)), int(match.group(6)))))
lines.sort(key=lambda l: l.points[0].z)
#print(lines)
for line in lines:
    max_heights = []
    for point in line.points:
        # Highest z value stops drop
        if (point.x, point.y) in height_map:
            max_heights.append(height_map[(point.x, point.y)] + 1)
        else:
            max_heights.append(1)
    max_heights.sort(reverse=True)
    
    if is_vertical(line):
        height = max_heights[0]
        for point in line.points:
            point.z = height
            height += 1
        line.num_supports = 1
    else:
        for point in line.points:
            point.z = max_heights[0]
        for k, g in groupby(max_heights):
            line.num_supports = len(list(g))
            break

    if is_vertical(line):
        for point in line.points:
            height_map[(point.x, point.y)] = point.z
        support_map[(line.points[0].x, line.points[0].y, line.points[0].z)] = line
    else:
        for point in line.points:
            height_map[(point.x, point.y)] = point.z
            support_map[(point.x, point.y, point.z)] = line
#print(lines)

num_no_disintegrate = 0
for line in lines:
    no_disintegrate = 0
    for point in line.points:
        if (point.x, point.y, point.z + 1) in support_map:
            supported_line = support_map[(point.x, point.y, point.z + 1)]
            supported_line.num_supports -= 1
            if supported_line.num_supports == 0:
                no_disintegrate = 1
                
#    print(f'{no_disintegrate} {line}')
    num_no_disintegrate += no_disintegrate
    for point in line.points:
        if (point.x, point.y, point.z + 1) in support_map:
            support_map[(point.x, point.y, point.z + 1)].num_supports += 1
print(len(lines) - num_no_disintegrate)


        

