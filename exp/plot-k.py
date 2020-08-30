import matplotlib.pyplot as plt
import statistics
from sys import argv
import sys
from itertools import cycle
import numpy as np
import copy
import math
import pdb
import subprocess
from collections import defaultdict

class dotdict(dict):
    __getattr__ = dict.get
    __setattr__ = dict.__setitem__
    __delattr__ = dict.__delitem__

def measure():
    results = {}

    for k in range(from_k, to_k):
        routes = execute_algorithm(vrp, k)
        dataset = load_dataset(vrp)

        vrp_total_distance = get_total_distance(dataset.warehouse, dataset.points, routes)
        results[k] = vrp_total_distance

    plot(results)

def execute_algorithm(vrp, k):
    flags = ['./../src/build/main', 'greedy', vrp, str(k)]

    console_out = subprocess.run(flags, stdout = subprocess.PIPE)
    parsed_algorithm = parse_algorithm_output(console_out.stdout)
    routes = load_routes(parsed_algorithm)

    return routes

def parse_algorithm_output(console_out):
    array = str(console_out).split('\\n')
    del array[0]
    del array[-1]
    return array

def load_routes(lines):
    routes = []

    for line in lines:
        if line.strip() == '\n' or line == '':
            continue

        route = []
        route.append(0)
        for point in line.strip().split(' '):
            route.append(int(point))
        route.append(0)
        routes.append(route)
    return routes

def load_dataset(vrp):
    f = open(vrp)
    dataset = dotdict()
    lines =  f.read().splitlines()

    n = int(lines[3].strip().split(' ')[-1])
    dataset.capacity = int(lines[5].strip().split(' ')[-1])
    dataset.warehouse = dotdict()
    dataset.points = []
    points = []
    demands = []

    for i in range(7, n + 7):
        point = dotdict()
        splited = lines[i].strip().split(' ')

        if len(splited) > 3:
            safe = [e for e in splited if e != '']
            point.x = float(safe[1])
        else:
            point.x = float(lines[i].strip().split(' ')[1])

        point.y = float(lines[i].strip().split(' ')[-1])
        point.demand = float(lines[i + n + 1].strip().split(' ')[1])

        if i == 7:
            dataset.warehouse = point
        else:
            dataset.points.append(point)

    f.close()
    return dataset

def get_total_distance(warehouse, points, routes):
    total_distance = 0

    for route in routes:
        total_distance += get_route_distance(warehouse, points, route)

    return total_distance

def get_route_distance(warehouse, points, route):
    route = [0] + route + [0]
    points = [warehouse] + points

    d = 0
    for i in range(1, len(route)):
        current_index = route[i]
        prev_index = route[i - 1]
        a = points[prev_index]
        b = points[current_index]

        d += math.sqrt(math.pow(a.x - b.x, 2) + math.pow(a.y - b.y, 2))

    return d

def plot(results):
    # SETUP
    if should_be_big_graph:
        plt.rcParams["figure.figsize"] = (16, 4)

    plt.grid(True)
    plt.grid(b=True, which='major', color='black', linestyle='dotted', alpha=0.1)
    plt.grid(b=True, which='minor', color='black', linestyle='dotted', alpha=0.05)
    plt.minorticks_on()
    plt.xlabel('K')
    plt.ylabel('Distancia total')

    colour_codes = get_cycler()

    # EACH ALGORITHM
    xs = [n for n in results.keys()]
    ys = [xs for xs in results.values()]
    xs, ys = (list(t) for t in zip(*sorted(zip(xs, ys)))) # sorts xs, ys by xs

    xint = range(min(xs), math.ceil(max(xs))+1, 2)
    plt.xticks(xint)

    plt.plot(xs, ys, marker='o', color=next(colour_codes), linestyle='-', markersize=2)
    plt.draw()

    if should_save_to_png:
        plt.savefig(resolve_image_name(vrp), dpi=320, bbox_inches='tight')

    plt.show()

def get_cycler():
    colour_codes = map('C{}'.format, cycle(range(10)))
    next(colour_codes)
    next(colour_codes)

    return colour_codes

def resolve_image_name(solution):
    script_path = sys.path[0]
    solution = solution.split('/')[-1]
    name = script_path + "/images/gr-" + solution.split('.vrp')[0] + "-K-%s-to-%s.png" % (from_k, to_k)
    return name

vrp = ''
should_save_to_png = False
should_be_big_graph = False
from_k = 1
to_k = 20

if(len(argv) >= 2):
    vrp = argv[1]

    if(len(argv) >= 3):
        should_save_to_png = argv[2].lower() == 'true'

    if(len(argv) >= 4):
        should_be_big_graph = argv[3].lower() == 'true'

    if(len(argv) >= 5):
        from_k = int(argv[4])

    if(len(argv) >= 6):
        to_k = int(argv[5])

    measure()
else:
    print("python3 plot_k.py {.vrp} {should_save_to_png?} {big_graph?} {from_k} {to_k}")
