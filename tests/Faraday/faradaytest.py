#!/usr/bin/env python
#!coding: utf-8

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(os.path.realpath(__file__)),'..'))

import faraday1D





def main():

    if len(sys.argv) ==2:
        path = sys.argv[1]
    else:
        path = './'

    faraday1D.main(path)



if __name__ == "__main__":
    main()
