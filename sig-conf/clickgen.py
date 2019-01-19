import os.path
import sys
import argparse
import os.path
from defs import *
import ftcgen
import nfgen
import ftmbgen
import tfgen

NF = "nf"
FTC = "ftc"
FTMB = "ftmb"
TF = "tf"

APPROACHES = [
    NF,
    FTC,
    FTMB,
    TF,
]

DEFAULT_FAILS = 1
DEFAULT_THREADS = 1
DEFAULT_BATCH = 5
DEFAULT_SHARING_LEVEL = 1  # no sharing

PERFORMANCE_METRICS = [
    THROUGHPUT_METRIC,
    LATENCY_METRIC
]
DEFAULT_PERF_METRIC = THROUGHPUT_METRIC
DEFAULT_DEBUG = False
DEFAULT_MB = COUNTER


def def_parser():
    parser = argparse.ArgumentParser(description="Generating click configurations")
    parser.add_argument('-c',
                        '--chain',
                        dest='c',
                        help='<Required> Chain size',
                        type=int,
                        required=True)

    parser.add_argument('-a',
                        '--approach',
                        dest='a',
                        help='<Required> Approach -- select among ' + ', '.join(APPROACHES),
                        type=str,
                        required=True)

    parser.add_argument('-m',
                        '--middlebox',
                        dest='m',
                        help='Middlebox -- select among counter, lb, nat -- default {}'.format(COUNTER),
                        nargs='+',
                        type=str,
                        default=[COUNTER])

    parser.add_argument('-f',
                        '--fail',
                        dest='f',
                        help='Number of failures -- default {}'.format(DEFAULT_FAILS),
                        type=int,
                        default=DEFAULT_FAILS)

    parser.add_argument('-t',
                        '--thread',
                        dest='t',
                        help='Number of threads -- default {}'.format(DEFAULT_THREADS),
                        type=int,
                        default=DEFAULT_THREADS)

    parser.add_argument('-b',
                        '--batch',
                        dest='b',
                        help='Buffer batch size -- default {}'.format(DEFAULT_BATCH),
                        type=int,
                        default=DEFAULT_BATCH)

    parser.add_argument('-l',
                        '--level',
                        dest='l',
                        help='Sharing level -- select among 1, 2, 4, 8 -- default {}'.format(DEFAULT_SHARING_LEVEL),
                        type=int,
                        default=DEFAULT_SHARING_LEVEL)

    parser.add_argument('-p',
                        '--perf',
                        dest='p',
                        help='What performance metric to measure -- select among ' + ', '.join(PERFORMANCE_METRICS) +
                             ' -- default {}'.format(DEFAULT_PERF_METRIC),
                        type=str,
                        default=DEFAULT_PERF_METRIC)

    parser.add_argument('-d',
                        '--debug',
                        dest='d',
                        help='Debug mode -- default False',
                        type=bool,
                        default=DEFAULT_DEBUG)

    parser.add_argument('-o',
                        '--output',
                        dest='o',
                        help='<Required> output folder',
                        type=str,
                        required=True)

    return parser


def parse_args(parser):
    opts = vars(parser.parse_args(sys.argv[1:]))

    if not os.path.isdir(opts['o']):
        raise ValueError("Output folder '{}' does not exist!".format(opts['i']))

    return opts


def gen_store(opts):
    apr = opts['a'].lower()
    print("Generating for approach '{}', metric '{}', chain size {}, and MB(s) '{}'".format(
        apr, opts['p'], opts['c'], opts['m']))

    if apr == FTC:
        clicks = ftcgen.generate(opts['c'], opts['t'], opts['m'], opts['l'], opts['f'], opts['b'], opts['p'], opts['d'])
        ftcgen.store(opts['o'], clicks)

    elif apr == NF:
        clicks = nfgen.generate(opts['c'], opts['t'], opts['m'], opts['l'], opts['p'], opts['d'])
        nfgen.store(opts['o'], clicks)

    elif apr == FTMB:
        clicks = ftmbgen.generate(opts['c'], opts['t'], opts['m'], opts['l'], opts['b'], opts['p'], opts['d'])
        ftmbgen.store(opts['o'], clicks)

    elif apr == TF:
        clicks = tfgen.generate(opts['c'], opts['t'], opts['m'], opts['l'], opts['f'], opts['b'], opts['p'], opts['d'])
        tfgen.store(opts['o'], clicks)

    else:
        clicks = None
        print("Invalid approach '{}'!".format(apr))

    print("{} click scripts were generated!".format(len(clicks)))


def main():
    opts = parse_args(def_parser())
    gen_store(opts)


if __name__ == "__main__":
    main()
