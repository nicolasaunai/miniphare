#!/usr/bin/env python
#!coding: utf-8



import computeIndexes


import sys


def main():

    if len(sys.argv) ==2:
        path = sys.argv[1]
    else:
        path = './'

    computeIndexes.main(path)




if __name__ == "__main__":
    main()
