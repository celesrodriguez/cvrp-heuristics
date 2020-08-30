import matplotlib.pyplot as plt
from sys import argv
import sys
from itertools import cycle
import numpy as np
import copy
import math
import pdb
from collections import defaultdict

class dotdict(dict):
    __getattr__ = dict.get
    __setattr__ = dict.__setitem__
    __delattr__ = dict.__delitem__

def main(solution, dataset_path, should_save_to_png, should_display_each_distance, should_display_total_distances, should_be_big_graph, should_display_demands):
    routes = load_routes(solution)
    dataset = load_dataset(dataset_path)

    do_plot(dataset.points, dataset.warehouse, routes, should_display_each_distance, should_be_big_graph, should_display_demands)
    total_distance = get_total_distance(dataset.warehouse, dataset.points, routes)

    if(should_display_each_distance or should_display_total_distances):
        legend(should_display_total_distances, total_distance)

    if should_save_to_png:
        plt.savefig(resolve_image_name(solution), dpi=320, bbox_inches='tight')

    plt.draw()
    plt.show()

def resolve_image_name(solution):
    script_path = sys.path[0]
    solution = solution.split('/')[-1]
    return script_path + "/images/" + solution.split('.sol')[0] + ".png"

def legend(toggle_total_distance, total_distance):
    if(toggle_total_distance):
        total_label = r'Total$\approx%.1f$' % total_distance
        plt.plot([], [], label=total_label, color="black")

    plt.legend()

def load_dataset(file):
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
        point.x = float(lines[i].strip().split(' ')[1])
        point.y = float(lines[i].strip().split(' ')[2])
        point.demand = float(lines[i + n + 1].strip().split(' ')[1])

        if i == 7:
            dataset.warehouse = point
        else:
            dataset.points.append(point)

    f.close()
    return dataset

def load_parsed_dataset(file):
    f = open(file)
    dataset = dotdict()

    lines =  f.read().splitlines()
    dataset.capacity = lines[0].strip()
    dataset.warehouse = dotdict()
    dataset.warehouse.x = float(lines[1].split(',')[0])
    dataset.warehouse.y = float(lines[1].split(',')[1])

    dataset.points = []
    for line in lines[2:]:
        point = dotdict()
        point.x = float(line.split(',')[0])
        point.y = float(line.split(',')[1])
        point.demand = float(line.split(',')[2])
        dataset.points.append(point)
    f.close();
    return dataset

def load_routes(file):
    f = open(file)
    next(f)
    routes = []

    for line in f.read().splitlines():
        if line.strip() == '\n' or line == '':
            continue

        route = []
        route.append(0)
        for point in line.strip().split(' '):
            route.append(int(point))
        route.append(0)
        routes.append(route)
    f.close();
    return routes

def do_plot(points, warehouse, routes, should_display_each_distance, should_be_big_graph, should_display_demands):
    setup(warehouse, should_be_big_graph)
    plot_routes(warehouse, points, routes, should_display_each_distance, should_display_demands)

def setup(warehouse, should_be_big_graph):
    plt.style.use('default')

    if should_be_big_graph:
        plt.rcParams["figure.figsize"] = (16, 4)

    plt.plot(warehouse.x, warehouse.y, marker='*', color='black', linestyle='', markersize=10.0)

#     plt.title('Puntos y depósito')
#     plt.xlabel('x')
#     plt.ylabel('y')

    plt.grid(True)
    plt.grid(b=True, which='major', color='black', linestyle='dotted', alpha=0.1)
    plt.grid(b=True, which='minor', color='black', linestyle='dotted', alpha=0.05)
    plt.minorticks_on()

def plot_routes(warehouse, points, routes, should_display_each_distance, should_display_demands):
    colour_codes = get_cycler()

    for route in routes:
        p_x = []
        p_y = []
        p_d = []

        for point in route:
            if point == 0:
                p_x.append(warehouse.x)
                p_y.append(warehouse.y)
                p_d.append(0)
            else:
                p_x.append(points[point - 1].x)
                p_y.append(points[point - 1].y)
                p_d.append(int(points[point - 1].demand))

        route_distance = get_route_distance(warehouse, points, route)
        route_label = r'Distancia$\approx%.1f$' % route_distance
        route_color = next(colour_codes)

        if(not should_display_each_distance):
            route_label = None

        p_d_multiplied = [i * distances_marker_multiplier for i in p_d]
        plt.plot(p_x, p_y, label=route_label, linewidth=1.0, linestyle='-', color=route_color)
        plt.scatter(p_x, p_y, marker='o', s=p_d_multiplied, color=route_color, zorder=10)

        if should_display_demands:
            p_d_str = parse_demands_to_str(p_d)
            for i in range (1, len(p_x) - 1):
                plt.text(p_x[i], p_y[i], text=p_d_str[i], s=0, color='white', fontsize=8, zorder=100, horizontalalignment='center', multialignment='center', verticalalignment='center')

def parse_demands_to_str(p_d):
    p_d_str = list(map(str, p_d))
    # del p_d_str[0]
    # del p_d_str[-1]
    return p_d_str

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

def get_cycler():
    colour_codes = map('C{}'.format, cycle(range(10)))
    next(colour_codes)
    next(colour_codes)

    return colour_codes

# pdb.set_trace()
def str2bool(v):
    if v.lower() in ('yes', 'true', 't', 'y', '1'):
        return True
    elif v.lower() in ('no', 'false', 'f', 'n', '0'):
        return False
    else:
        raise argparse.ArgumentTypeError('Boolean value expected.')

if(len(argv) >= 3):
    solution = argv[1]
    dataset = argv[2]
    should_save_to_png = False
    should_display_each_distance = False
    should_display_total_distances = False
    should_be_big_graph = False
    should_display_demands = False
    distances_marker_multiplier = 7

    if(len(argv) >= 4):
        should_save_to_png = argv[3].lower() == 'true'

    if(len(argv) >= 5):
        should_display_each_distance = argv[4].lower() == 'true'

    if(len(argv) >= 6):
        should_display_total_distances = argv[5].lower() == 'true'

    if(len(argv) >= 7):
        should_be_big_graph = argv[6].lower() == 'true'

    if(len(argv) >= 8):
        should_display_demands = argv[7].lower() == 'true'

    if(len(argv) >= 9):
        distances_marker_multiplier = int(argv[8])

    main(solution, dataset, should_save_to_png, should_display_each_distance, should_display_total_distances, should_be_big_graph, should_display_demands)
else:
    print("python3 plot.py {.sol} {.vrp} {should_save_to_png?} {display_each_distance?} {display_total_distances?} {big_graph?} {should_display_demands?} {int: distances_marker_multiplier}")
