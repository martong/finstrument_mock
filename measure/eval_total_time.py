#!/usr/bin/env python

import os
from matplotlib import rc
import matplotlib
import numpy as np
import matplotlib.pyplot as plt
import argparse


def plot(csetup, value, name):
    ind = np.arange(len(csetup)) + .5
    plt.barh(ind, value, align='center', height=0.6)
    plt.yticks(ind, csetup)

    plt.title(name)
    plt.xlabel('seconds')
    #plt.ylabel('compiler setup')

    plt.tight_layout(pad=0.4, w_pad=0.5, h_pad=1.0)

    plt.show()


# Returns a dict for a result file
#  key: e.g. 'Total absolute time for Vector accumulate'
#  value: a pair (time, compiler setup deduced from the dir name)
#    e.g (12.21, '-O0 -finstrument-functions')
def parse_result_file(filename):
    result = {}
    with open(filename) as f:
        compiler_setup = "-" + filename.split(
            "/")[0].replace("__", " -").replace("_mock", "=mock")
        for line in f:
            key = ""
            value = -1.0
            if "Total absolute time" in line:
                parts = line.split(":")
                key = parts[0]
                value = float(parts[1].strip().split(" ")[0])
            #elif "Penalty" in line:
                #parts = line.split(":")
                #key = parts[0]
                #value = float(parts[1].strip().split(" ")[0])
            else:
                continue
            assert len(key) > 0
            assert value >= 0.0
            result[key] = (value, compiler_setup)
    return result


# Returns a dict for all measurement results
#  key: e.g. 'Total absolute time for Vector accumulate'
#  value: a list of tuples of (time, compiler setup)
#    e.g. [(3.03, '-O0'), (12.21, '-O0 -finstrument-functions')]
def get_all_results():
    for dirpath, dirs, files in os.walk("."):
        results = {}
        for dir in dirs:
            if dir.startswith('.'):
                continue
            if dir.startswith('_'):
                continue
            result_file = os.path.join(dir, "report.txt")
            print result_file
            result = parse_result_file(result_file)
            for key, Value in result.iteritems():
                if key in results:
                    results[key].append(Value)  # (value, compiler_setup)
                else:
                    results[key] = [Value]
        return results

results = get_all_results()
for key, Value in results.iteritems():
    print Value
    sortedValue = sorted(Value, reverse=True, key=lambda x: x[0])
    print sortedValue
    values, csetup = zip(*sortedValue)
    print "values:"
    print values
    print "csetup:"
    print csetup
    print "TITLE: ", key
    plot(csetup, values, key)
    #exit(0)