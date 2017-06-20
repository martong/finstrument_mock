import argparse
from matplotlib import rc
import matplotlib

def init():
    parser = argparse.ArgumentParser()
    parser.add_argument("--save", help="save figures",
                        action='store_true', default=False)
    args = parser.parse_args()

    if args.save:
        rc('text', usetex=True)
        rc('font',**{'family':'serif','serif':['Computer Modern Roman']})
        matplotlib.rcParams.update({'font.size': 14})

    return args
