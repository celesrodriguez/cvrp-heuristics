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

plot_title = 'Algoritmos para un subconjunto del dataset A'
should_save_to_png = True
image_name = 'algorithms-distances-for-A-nogr.png'
algorithms = ['sweep', 'savings', 'kmeans', 'annealing']
datasets_directory = '../src/resources/correctedDatasets/%s.vrp'
vrps = ['a-n32-k5',
'a-n33-k5',
'a-n33-k6',
'a-n34-k5',
'a-n36-k5',
'a-n37-k5',
'a-n37-k6',
'a-n38-k5',
'a-n39-k5',
'a-n39-k6',
'a-n44-k6',
'a-n45-k6']

def measure():
    results = {}

    for algorithm in algorithms:
        results[algorithm] = {}

        for vrp in vrps:
            routes = execute_algorithm(algorithm, vrp)
            dataset = load_dataset(vrp)

            n = len(dataset.points)
            vrp_total_distance = get_total_distance(dataset.warehouse, dataset.points, routes)

            if n not in results[algorithm]: results[algorithm][n] = []
            results[algorithm][n].append(vrp_total_distance)

    plot(results)

def execute_algorithm(algorithm, vrp):
    dataset_path = datasets_directory % vrp
    flags = ['./../src/build/main', algorithm, dataset_path]

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
    file = datasets_directory % vrp

    f = open(file)
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
    # plt.rcParams["figure.figsize"] = (16, 4)

    plt.grid(True)
    plt.grid(b=True, which='major', color='black', linestyle='dotted', alpha=0.1)
    plt.grid(b=True, which='minor', color='black', linestyle='dotted', alpha=0.05)
    plt.minorticks_on()
    plt.xlabel('# vértices')
    plt.ylabel('Distancia total')
    plt.title(plot_title)

    colour_codes = get_cycler()

    # EACH ALGORITHM
    for algorithm, distances in results.items():
        xs = [n for n in distances.keys()]
        ys = [statistics.mean(xs) for xs in distances.values()]
        xs, ys = (list(t) for t in zip(*sorted(zip(xs, ys)))) # sorts xs, ys by xs

        plt.plot(xs, ys, label=algorithm, marker='o', color=next(colour_codes), linestyle='-', markersize=2)

    plt.legend()
    plt.draw()

    if should_save_to_png:
        plt.savefig(resolve_image_name(), dpi=320, bbox_inches='tight')

    plt.show()

def get_cycler():
    colour_codes = map('C{}'.format, cycle(range(10)))
    next(colour_codes)
    next(colour_codes)

    return colour_codes

def resolve_image_name():
    script_path = sys.path[0]
    return script_path + "/images/" + image_name

measure()
