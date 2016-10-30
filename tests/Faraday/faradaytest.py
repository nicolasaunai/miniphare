#!/usr/bin/env python
#!coding: utf-8


import faraday1D


import sys


def main():

    if len(sys.argv) ==2:
        path = sys.argv[1]
    else:
        path = './'

    faraday1D.main(path)



if __name__ == "__main__":
    main()
