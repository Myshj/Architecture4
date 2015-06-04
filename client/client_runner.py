
import argparse
import sys
import os
import time

args_parser = argparse.ArgumentParser(version='1.0',
                                      add_help=True,
                                      prog='client runner',
                                      description='Runs TCP clients.')

args_parser.add_argument('frequency',
                         metavar='frequency',
                         type=int,
                         help='Clients per second.')

args_parser.add_argument('request_length',
                         metavar='length',
                         type=int,
                         help='Length of data which generate every client.')

args_parser.add_argument('-sec',
                         metavar='seconds',
                         type=int,
                         default=1,
                         help='Number of seconds script will work.')


args = args_parser.parse_args(sys.argv[1:])

for _ in xrange(0, args.sec):
    start = time.time()
    for __ in xrange(0, args.frequency):
        os.system('./client -l %d' % args.request_length)
    time.sleep(1.0 - (time.time() - start))
