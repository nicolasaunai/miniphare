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

    nbrOrder = 4
    nbrTestCases = nbrOrder 

    dim_l =[0]*nbrTestCases

    interpOrder_l=[1, 2, 3, 4]

    nbrCellX_l=[40]*nbrTestCases
    nbrCellY_l=[ 0]*nbrTestCases
    nbrCellZ_l=[ 0]*nbrTestCases

    dx_l=[0.1]*nbrTestCases 
    dy_l=[0. ]*nbrTestCases
    dz_l=[0. ]*nbrTestCases
    
    nbrCells = {'X':nbrCellX_l, 'Y':nbrCellY_l, 'Z':nbrCellZ_l}
    meshSize= {'X':dx_l, 'Y':dy_l, 'Z':dz_l}

    origin = [0., 0., 0.]

    gl = gridlayout.GridLayout()
    Direction_l = gl.Direction_l

    
    icase_l = np.arange( nbrTestCases )

    # ------- Debug commands -------
    for icase in icase_l:
        idim = dim_l[icase]
        order = interpOrder_l[icase]
        print( "Interpolation order = %d" % order )
        print( "Nbr of cells = %d" %  nbrCells[Direction_l[idim][1]][icase])
        print( "Nbr of ghost cells on the primal mesh = %d on each side" %
                gl.nbrGhostsPrimal(order) )

    # ------------------------------

    f = open(os.path.join(path,"centeredCoords_summary.txt"), "w")
    
    # the number of test cases
    f.write("%d \n" % nbrTestCases )     
    
    for icase in icase_l:
        idim = dim_l[icase]
        order = interpOrder_l[icase]        
        
        f.write(("%d %d %06d %4.1f ") %
           (order, dim_l[idim]+1, 
            nbrCells[Direction_l[idim][1]][icase],
            meshSize[Direction_l[idim][1]][icase]))

        iStart = gl.physicalStartPrimal( order )
        iEnd   = gl.physicalEndPrimal  ( order, nbrCells[Direction_l[idim][1]][icase])

        # a cell-centered coordinate is always dual
        iEnd = iEnd - 1

        f.write(("%d %d ") % (iStart, iEnd))
        f.write(("%6.2f %6.2f %6.2f\n") % (origin[0], origin[1], origin[2]))

    f.close()

    for icase in icase_l:
        idim = dim_l[icase]
        order = interpOrder_l[icase]         
        
        f = open((os.path.join(path,"centeredCoords_ord%d_dim%d.txt") %
        (order, dim_l[idim]+1)), "w")

        iStart = gl.physicalStartPrimal(order)
        iEnd   = gl.physicalEndPrimal  (order, nbrCells[Direction_l[idim][1]][icase])

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

