#!/usr/bin/env python
#!coding: utf-8


import allocSizes
import cellCenteredCoordinates
import fieldNodeCoordinates
import gridIndexing
import deriv1D


import sys


def main():

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
