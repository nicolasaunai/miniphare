#!/usr/bin/env python
#!coding: utf-8

import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.realpath(__file__)),'..'))

import allocSizes
import cellCenteredCoordinates
import fieldNodeCoordinates
import gridIndexing
import deriv1D


import sys
import os

def main():
    print (os.getcwd() ) 
    print ( os.path.dirname(os.path.realpath(__file__)) )

    if len(sys.argv) ==2:
        path = sys.argv[1]
    else:
        path = './'


    allocSizes.main(path)
    cellCenteredCoordinates.main(path)
    fieldNodeCoordinates.main(path)
    gridIndexing.main(path)
    deriv1D.main(path)



if __name__ == "__main__":
    main()
