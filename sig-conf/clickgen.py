import os.path
import sys
import argparse
import os.path
from defs import *
import ftcgen
import nfgen

NF = "nf"
FTC = "ftc"

APPROACHES = [
    NF,
    FTC,
]

DEFAULT_FAILS = 1
DEFAULT_THREADS = 1
DEFAULT_BATCH = 5

DEFAULT_MB = COUNTER_MB


def def_parser():
    parser = argparse.ArgumentParser(description="Generating click configurations")
    parser.add_argument('-c',
                        '--chain',
                        dest='c',
                        help='chain size',
                        type=int,
                        required=True)

    parser.add_argument('-a',
                        '--approach',
                        dest='a',
                        help='Approach',
                        type=str,
                        required=True)

    parser.add_argument('-m',
                        '--middlebox',
                        dest='m',
                        help='Middlebox (Select among "counter", "lb")',
                        type=str,
                        default=True)

    parser.add_argument('-f',
                        '--fail',
                        dest='f',
                        help='Number of failures',
                        type=int,
                        default=DEFAULT_FAILS)

    parser.add_argument('-t',
                        '--thread',
                        dest='t',
                        help='Number of threads',
                        type=int,
                        default=DEFAULT_THREADS)

    parser.add_argument('-b',
                        '--batch',
                        dest='b',
                        help='Buffer batch size',
                        type=int,
                        default=DEFAULT_BATCH)

    parser.add_argument('-o',
                        '--output',
                        dest='o',
                        help='output folder',
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
    if apr == FTC:
        print("Generating for {}".format(FTC))
        clicks = ftcgen.generate(opts['c'], opts['f'], opts['t'], opts['b'])
        ftcgen.store(opts['o'], clicks)

    elif apr == NF:
        print("Generating for {}".format(NF))
        clicks = nfgen.generate(opts['c'], opts['t'], opts['m'])
        nfgen.store(opts['o'], clicks)

    else:
        print("Invalid approach '{}'!".format(apr))


def main():
    opts = parse_args(def_parser())
    gen_store(opts)


if __name__ == "__main__":
    main()
