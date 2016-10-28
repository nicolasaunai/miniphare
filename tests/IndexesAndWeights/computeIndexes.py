#!/usr/bin/env python2
#!coding: utf-8

import numpy as np
import math

import sys
sys.path.insert(0, '../')

import gridlayout
import os




def main(path='./'):

    if len(sys.argv) == 2:
        path = sys.argv[1]
    # ---------------------- INITIALIZATION  -----------------------------------
    nbrCellX_l=[40]*8
    nbrCellY_l=[ 0]*8
    nbrCellZ_l=[ 0]*8
    nbrCells = {'X':nbrCellX_l, 'Y':nbrCellY_l, 'Z':nbrCellZ_l}

    dx_l=[0.1]*8 
    dy_l=[0. ]*8
    dz_l=[0. ]*8
    meshSize= {'X':dx_l, 'Y':dy_l, 'Z':dz_l}

    interpOrder_l=[1, 2, 3, 4]*2

    # functions to be derivated : iqty
    # 'Bx':0, 'By':1, 'Bz':2, 'Ex':3, 'Ey':4, 'Ez':5, 'rho':6, 'V':7, 'P':8
    qty_l=[6]*8
    
    dim_l =[0]*8





    gl  = gridlayout.GridLayout()

    Direction_l = gl.Direction_l
    Qty_l       = gl.Qty_l

    origin = [0., 0., 0.]


    nbrTestCases = len(qty_l)
    print( nbrTestCases )

    print( interpOrder_l )
    print( qty_l )
    print( dim_l )

    icase_l = np.arange( len(qty_l) )
    print( icase_l )


    # -------- Let us define a function on Bx with Yee lattice --------
    f = open(os.path.join(path,"indexes_summary.txt"), "w")

    # the number of test cases
    f.write("%d \n" % len(icase_l) )  

    for icase in icase_l:
        idim  = dim_l[icase]
        order = interpOrder_l[icase]
        iqty = Qty_l[qty_l[icase]][0]  # 0 ,  1,  2,  3, ... 
        qty  = Qty_l[qty_l[icase]][1]  # Bx, By, Bz, Ex, ...

        nbcells = nbrCells[Direction_l[idim][1]][icase]
        stepSize = meshSize[Direction_l[idim][1]][icase]

        f.write(("%03d %d %d %d %5.4f ") %
           (order, idim+1, 
            iqty, nbcells, 
            stepSize ) )

        centering = gl.qtyCentering(qty, Direction_l[idim][1])

#        iStart = gl.physicalStartIndex(order, centering)
        
        iGhostStart = gl.ghostStartIndex()
        iGhostEnd   = gl.ghostEndIndex  (order, centering, nbcells)

        # last argument means 1st derivative
        newCentering = gl.changeCentering( centering, 1 )

        iDerStart = gl.physicalStartIndex(order, newCentering)
        iDerEnd   = gl.physicalEndIndex  (order, newCentering, nbcells)

        f.write(("%d %d %d %d ") % (iGhostStart, iGhostEnd, iDerStart, iDerEnd))

        f.write(("%6.2f %6.2f %6.2f ") % (origin[0], origin[1], origin[2]))

        f.write("\n")

    f.close()






if __name__ == "__main__":
    main()





