import sys
import argparse
import os.path

def def_parser():
    parser = argparse.ArgumentParser(description="draw charts of the report")
    parser.add_argument('-i', '--ft', dest='i', help='first file', type=str, required=True)
    parser.add_argument('-j', '--nft', dest='j', help='second file', type=str, default='')
    parser.add_argument('-o', '--output', dest='o', help='output file', type=str, required=True)
    parser.add_argument('-f', '--ftlabel', dest='f', help='first label', type=str, required=True)
    parser.add_argument('-s', '--nftlabel', dest='s', help='second label', type=str, default='')

    return parser


def parse_args(parser):
    opts = vars(parser.parse_args(sys.argv[1:]))

    if not os.path.isfile(opts['i']):
        raise argparse.ArgumentTypeError("File %s does not exist!" % opts['i'])

    if opts['j'] != '' and not os.path.isfile(opts['j']):
        raise argparse.ArgumentTypeError("File %s does not exist!" % opts['j'])
    return opts


def main():
    opts = parse_args(def_parser())


if __name__ == "__main__":
    main()