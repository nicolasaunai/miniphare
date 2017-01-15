#!/usr/bin/env python
#!coding: utf-8


import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(os.path.realpath(__file__)),'..'))

import odepush




def main():

    if len(sys.argv) ==2:
        path = sys.argv[1]
    else:
        path = './'

    odepush.main(path)




if __name__ == "__main__":
    main()
