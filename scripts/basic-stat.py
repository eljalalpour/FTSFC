import pandas as pd
import sys
import argparse
import os.path
import csv

def def_parser():
    parser = argparse.ArgumentParser(description="draw charts of the report")
    parser.add_argument('-i', '--input', dest='i', help='input file', type=str, required=True)

    return parser

def check_file_exists(p):
    if not os.path.isfile(p):
        raise argparse.ArgumentTypeError("File %s does not exist!" % p)


def parse_args(parser):
    opts = vars(parser.parse_args(sys.argv[1:]))
    check_file_exists(opts['i'])

    return opts


def read_file(file_path):
    return pd.read_csv(file_path, skiprows=1, header=None)


def stats(df):
    print("Mean: ")
    print(df.mean())

    print("Std: ")
    print(df.std())

def main():
    # Parse args
    opts = parse_args(def_parser())
    df = read_file(opts['i'])
    stats(df)

if __name__ == "__main__":
    main()


