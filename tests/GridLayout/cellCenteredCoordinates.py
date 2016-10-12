#!/usr/bin/env python
#!coding: utf-8
"""
this script aims at producing expected values for cell center coordinates
it writes the following files:
    - centeredCoords_summary.txt : contains physical Start and End indexes
        as a function of the interpolation order, dimensionality

    - and centeredCoords_ord*.txt which contain the coordinates of all cell
    centers as a function of the interpolation order and dimensionality
"""

import numpy as np

import sys
sys.path.insert(0, '../')

import gridlayout
import os



# ---- Get coordinate methods -------------------------
# ds stands for dx or dy or dz
#
# This method returns a point
#
def centeredCoords(iprimal, iStart, direction, ds, origin):

    # a cell-centered coordinate is always dual
    halfCell = 0.5

    x = ( (iprimal - iStart) + halfCell )*ds + origin[direction[0]]

    return x




# ---------------------- MAIN CODE -----------------------------------------




def main(path='./'):

    if len(sys.argv) == 2:
        path = sys.argv[1]

    nbrCellX_l=[40, 40, 40]
    nbrCellY_l=[ 0, 12, 12]
    nbrCellZ_l=[ 0,  0, 12]
    nbrCells = {'X':nbrCellX_l, 'Y':nbrCellY_l, 'Z':nbrCellZ_l}

    dx_l=[0.1, 0.1, 0.1] # 1D, 2D, 3D cases
    dy_l=[0. , 0.1, 0.1]
    dz_l=[0. , 0. , 0.1]
    meshSize= {'X':dx_l, 'Y':dy_l, 'Z':dz_l}

    origin = [0., 0., 0.]

    gl = gridlayout.GridLayout()
    Direction_l = gl.Direction_l

    interpOrder_l=[1, 2, 3, 4]
    case_l=[0]
    #dim_l =[0, 1, 2]
    dim_l =[0]


    maxNbrDim = 3

    nbrTestCases = len(interpOrder_l)*len(dim_l)*len(case_l)
    print( nbrTestCases )

    iord_l=np.arange( len(interpOrder_l) )
    print( iord_l )

    # ------- Debug commands -------
    for icase in case_l:
        for iord in iord_l:
                print( "Interpolation order = %d" % interpOrder_l[iord] )

                idim = 0
                print( "Nbr of cells = %d" %  nbrCells[Direction_l[idim][1]][icase])

                print( "Nbr of ghost cells on the primal mesh = %d on each side" %
                gl.nbrGhostsPrimal(interpOrder_l[iord]) )

    # ------------------------------

    f = open(os.path.join(path,"centeredCoords_summary.txt"), "w")
    for iord in iord_l:
        for icase in case_l:
            for idim in dim_l:

                f.write(("%d %d %06d %4.1f ") %
                   (interpOrder_l[iord],
                    dim_l[idim]+1, nbrCells[Direction_l[idim][1]][icase],
                    meshSize[Direction_l[idim][1]][icase]))

                iStart = gl.physicalStartPrimal(interpOrder_l[iord])
                iEnd   = gl.physicalEndPrimal  (interpOrder_l[iord], nbrCells[Direction_l[idim][1]][icase])

                # a cell-centered coordinate is always dual
                iEnd = iEnd - 1

                f.write(("%d %d ") % (iStart, iEnd))
                f.write(("%6.2f %6.2f %6.2f\n") % (origin[0], origin[1], origin[2]))

    f.close()


    for iord in iord_l:
        for icase in case_l:
            for idim in dim_l:
                f = open((os.path.join(path,"centeredCoords_ord%d_dim%d_case%d.txt") %
                (interpOrder_l[iord], dim_l[idim]+1, icase)), "w")

                iStart = gl.physicalStartPrimal(interpOrder_l[iord])
                iEnd   = gl.physicalEndPrimal  (interpOrder_l[iord], nbrCells[Direction_l[idim][1]][icase])

                # a cell-centered coordinate is always dual
                iEnd = iEnd - 1

                for iprimal in np.arange(iStart, iEnd+1):
                    x = centeredCoords(iprimal, iStart, Direction_l[idim], \
                                       meshSize[Direction_l[idim][1]][icase], origin )
                    f.write(("%8.2f ") % (x))

                f.close()


#np.savetxt("gridlayouttest.txt", caseParamsTable, delimiter=" ", fmt="%4.1f")


if __name__ == "__main__":
    main()

