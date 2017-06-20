#!/usr/bin/env python

import os
import numpy as np
import matplotlib.pyplot as plt
import charts_common as cs

def plot(normalizedResults, save):
    plt.figure(figsize=(9,8))
    csetups = normalizedResults[0][2]
    ind = np.arange(len(csetups)) + .5
    i = 0
    fi = 0.0
    height = 0.8 / len(normalizedResults)  # keep 0.2 for place between each csetup
    # with align='edge' -0.5 offset would put to the bottom of the csetup
    offset = -0.4
    for (test_name, values, csetup) in normalizedResults:
        plt.barh(ind + offset + fi,values, align='edge',
                 height=height,
                 label=test_name)
        fi = fi + height
        i = i + 1

    plt.yticks(ind, csetups)

    #plt.legend(loc='upper right', shadow=True, fontsize='medium')
    plt.tight_layout(pad=2.4, w_pad=0.5, h_pad=1.0)

    # Either show or save, save after show results in an empty pdf
    if save:
        plt.savefig('normalized_total_absolute_times.pdf', format='pdf', dpi=400)
        plt.clf()
    else:
        plt.show()


# Returns a dict for a result file
#  key: e.g. 'Total absolute time for Vector accumulate'
#  value: a pair (time, compiler setup deduced from the dir name)
#    e.g (12.21, '-O0 -finstrument-functions')
def parse_result_file(filename):
    result = {}
    with open(filename) as f:
        compiler_setup = filename.split(
            "/")[0].replace("__", " -").replace("_mock", "=mock")
        for line in f:
            key = ""
            value = -1.0
            if "Total absolute time" in line:
                parts = line.split(":")
                key = parts[0]
                value = float(parts[1].strip().split(" ")[0])
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
        for dir in [
            '__O0__finstrument-functions__fno-inline-functions',
            '__O0__finstrument-functions',
            '__O0__fsanitize_mock__fno-inline-functions__DSUB',
            '__O0__fsanitize_mock__fno-inline-functions',
            '__O0__fsanitize_mock__DSUB',
            '__O0__fsanitize_mock',
            '__O0',
            '__O2__finstrument-functions__fno-inline-functions',
            '__O2__finstrument-functions',
            '__O2__fsanitize_mock__fno-inline-functions__DSUB',
            '__O2__fsanitize_mock__fno-inline-functions',
            '__O2__fsanitize_mock__DSUB',
            '__O2__fsanitize_mock',
            '__O2',
        ]:
            result_file = os.path.join(dir, "report.txt")
            print result_file
            result = parse_result_file(result_file)
            for key, Value in result.iteritems():
                if key in results:
                    results[key].append(Value)  # (value, compiler_setup)
                else:
                    results[key] = [Value]
        return results

args = cs.init()
results = get_all_results()

# Normalize results into a list of
#  (test_name: string, list of float values: [0.0, 1.0], list of compiler sets)
normalizedResults = []
for test_name, Value in results.iteritems():
    #sortedValue = sorted(Value, reverse=True, key=lambda x: x[0])
    values, csetup = zip(*Value)
    max_value = max(values)
    values = [x/max_value for x in values]
    normalizedResults.append((test_name, values, csetup))

print(normalizedResults)
plot(normalizedResults, args.save)
