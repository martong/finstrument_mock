#!/usr/bin/env python

import os
from matplotlib import rc
import matplotlib
import numpy as np
import matplotlib.pyplot as plt
import argparse


def plot(results):
    penalty_names = results[0][1].keys()
    print penalty_names
    ind = np.arange(len(penalty_names)) + .5
    colors = ['r', 'g', 'b', 'y', 'c']
    #colors = ['w', 'w', 'w', 'w', 'w']
    hatches = ['////', '\\\\\\\\', '+++', 'xxx', 'ooo', 'O']
    i = 0
    fi = 0.0
    height = 0.8 / len(results)  # keep 0.2 for place between each penalty
    # with align='edge' -0.5 offset would put to the bottom of the penalty
    offset = -0.4
    for penalty_name, result in results:
        plt.barh(ind + offset + fi, result.values(), align='edge',
                 height=height, color=colors[i], hatch=hatches[i],
                 label=penalty_name)
        fi = fi + height
        i = i + 1

    plt.yticks(ind, penalty_names)

    plt.legend(loc='upper right', shadow=True, fontsize='medium')

    #plt.title(name)
    #plt.xlabel('seconds')
    #plt.ylabel('compiler setup')

    plt.tight_layout(pad=0.4, w_pad=0.5, h_pad=1.0)

    plt.show()


# Returns a dict for a result file
#  key: penalty name, e.g. 'Penalty'
#  value: penalty value
def parse_result_file(filename):
    result = {}
    with open(filename) as f:
        for line in f:
            key = ""
            value = -1.0
            if "Penalty" in line:
                parts = line.split(":")
                key = parts[0]
                value = float(parts[1].strip().split(" ")[0])
            else:
                continue
            assert len(key) > 0
            assert value >= 0.0
            result[key] = value
    return result


def plot_all_results():
    results = []
    for dir in [
        'O2',
        'O0',
        'O2__fsanitize_mock',
        'O2__fsanitize_mock__fno-inline-functions',
        'O2__finstrument-functions'
    ]:
        result_file = os.path.join(dir, "report.txt")
        print result_file
        result = parse_result_file(result_file)
        compiler_setup = "-" + result_file.split(
            "/")[0].replace("__", " -").replace("_mock", "=mock")
        results.append((compiler_setup, result))
    plot(results)

plot_all_results()
