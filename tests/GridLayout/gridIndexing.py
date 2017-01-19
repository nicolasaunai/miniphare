#!/usr/bin/env python
#!coding: utf-8
"""
This script prepares 'gridIndexing.txt'. The file contains expected values
for physical/ghostStart/EndIndexes in directions X, Y and Z for all hybrid
quantities and for all interpolation orders (1,2,3,4), in 1D, 2D and 3D.
"""

import numpy as np

import sys

import gridlayout
import os



# ---------------------- MAIN CODE -----------------------------------------
def main(path='./'):

    if len(sys.argv) == 2:
        path = sys.argv[1]

    paramList = ['interpOrder', 'nbDims', 'Quantity', 'nbrCellX', 'nbrCellY', 'nbrCellZ', 'dx', 'dy', 'dz'];
    len( paramList )
    print( paramList[:] )
    print( paramList[2] )

    interpOrder_l=[1, 2, 3, 4]
    nbDims_l=[1, 2, 3]

    Qty_l=[0, 1, 2, 3, 4, 5, 6, 7, 8]
    Qty_l=[(0,'Bx'), (1,'By'), (2,'Bz'), (3,'Ex'), (4,'Ey'), (5,'Ez'), (6,'rho'), (7,'V'), (8,'P')]

    nbrCellX_l=[40, 40, 40]
    nbrCellY_l=[ 0, 12, 12]
    nbrCellZ_l=[ 0,  0, 12]

    nbDims_l  = [1, 2, 3]    # 1D, 2D, 3D

    dx_l=[0.1, 0.1, 0.1] # 1D, 2D and 3D cases
    dy_l=[0. , 0.1, 0.1]
    dz_l=[0. , 0. , 0.1]

    icase_l = [0, 1, 2]

    nbrTestCases = len(interpOrder_l)*len(nbDims_l)*len(Qty_l)
    print( nbrTestCases )

    caseParamsTable = np.zeros((nbrTestCases, len(paramList) + 4*3))

    col_l = np.arange( len(paramList) )
    print( col_l )


    iord_l=np.arange( len(interpOrder_l) )
    print( iord_l )

    iqty_l=np.arange( len(Qty_l) )
    print( iqty_l )


    gl = gridlayout.GridLayout()


    f = open(os.path.join(path,"gridIndexing.txt"), "w")

    for iord in iord_l:
        for icase in icase_l:
            for iqty in iqty_l:
                centeringX = gl.qtyCentering(Qty_l[iqty][1], 'X')
                centeringY = gl.qtyCentering(Qty_l[iqty][1], 'Y')
                centeringZ = gl.qtyCentering(Qty_l[iqty][1], 'Z')

                f.write(("%03d %03d %s %03d %03d %03d %4.1f %4.1f %4.1f"+" %d"*12 +"\n") %
                   (interpOrder_l[iord],
                    nbDims_l[icase],
                    Qty_l[iqty][0],
                    nbrCellX_l[icase],
                    nbrCellY_l[icase],
                    nbrCellZ_l[icase],
                    dx_l[icase],
                    dy_l[icase],
                    dz_l[icase],
                    gl.physicalStartIndex(interpOrder_l[iord], centeringX),
                    gl.physicalEndIndex  (interpOrder_l[iord], centeringX, nbrCellX_l[icase]),
                    gl.ghostStartIndex   (),
                    gl.ghostEndIndex     (interpOrder_l[iord], centeringX, nbrCellX_l[icase]),
                    gl.physicalStartIndex(interpOrder_l[iord], centeringY),
                    gl.physicalEndIndex  (interpOrder_l[iord], centeringY, nbrCellY_l[icase]),
                    gl.ghostStartIndex   (),
                    gl.ghostEndIndex     (interpOrder_l[iord], centeringY, nbrCellY_l[icase]),
                    gl.physicalStartIndex(interpOrder_l[iord], centeringZ),
                    gl.physicalEndIndex  (interpOrder_l[iord], centeringZ, nbrCellZ_l[icase]),
                    gl.ghostStartIndex   (),
                    gl.ghostEndIndex     (interpOrder_l[iord], centeringZ, nbrCellZ_l[icase])) )


    f.close()


if __name__ == "__main__":
    main()
