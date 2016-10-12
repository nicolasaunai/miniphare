#!/usr/bin/env python
#!coding: utf-8

import numpy as np
#import math

import sys
sys.path.insert(0, '../')

import gridlayout
import os


# ---- Get coordinate methods -------------------------
# ds stands for dx or dy or dz
#
# This method returns a point
#
def fieldCoords(iprimal, iStart, qty, direction, ds, origin):
    halfCell = 0.
    gl  = gridlayout.GridLayout()

    if gl.qtyCentering(qty, direction[1]) == 'dual':
        halfCell = 0.5

    x = ( (iprimal - iStart) + halfCell )*ds + origin[direction[0]]

    return x


# ---------------------- MAIN CODE -----------------------------------------

def main(path='./'):

    if len(sys.argv) == 2:
        path = sys.argv[1]

    gl = gridlayout.GridLayout()
    Direction_l = gl.Direction_l
    Qty_l = gl.Qty_l

    interpOrder_l=[1, 2, 3, 4]
    case_l=[0]
    #dim_l =[0, 1, 2]
    dim_l =[0]

    nbrCellX_l=[40, 40, 40]
    nbrCellY_l=[ 0, 12, 12]
    nbrCellZ_l=[ 0,  0, 12]
    nbrCells = {'X':nbrCellX_l, 'Y':nbrCellY_l, 'Z':nbrCellZ_l}

    dx_l=[0.1, 0.1, 0.1] # 1D, 2D, 3D cases
    dy_l=[0. , 0.1, 0.1]
    dz_l=[0. , 0. , 0.1]
    meshSize= {'X':dx_l, 'Y':dy_l, 'Z':dz_l}

    origin = [0., 0., 0.]

    nbrTestCases = len(interpOrder_l)*len(dim_l)*len(Qty_l)*len(case_l)
    print( nbrTestCases )

    iord_l=np.arange( len(interpOrder_l) )
    print( iord_l )

    iqty_l=np.arange( len(Qty_l) )
    print( iqty_l )

    # ------- Debug commands -------
    for icase in case_l:
        for idim in dim_l:
            for iqty in iqty_l:
                centering = gl.qtyCentering(Qty_l[iqty][1], Direction_l[idim][1])
                nbcells = nbrCells[Direction_l[idim][1]][icase]

                print( "field : %s, direction : %s, centering : %s" %
                      (Qty_l[iqty][1], Direction_l[idim][1], centering ) )

                print( "Nbr of cells = %d" % nbcells )

                print( "Nbr of ghost cells = %d on each side" %
                gl.nbrGhosts(interpOrder_l[0], centering) )

                print( "Alloc size = %d" %
                gl.allocSize(interpOrder_l[0], centering, nbcells ) )

                print( gl.physicalStartIndex(interpOrder_l[0], centering),
                       gl.physicalEndIndex(interpOrder_l[0], centering, nbcells),
                       gl.ghostEndIndex(interpOrder_l[0], centering, nbcells)  )

    # ------------------------------

    f = open(os.path.join(path,"fieldCoords_summary.txt"), "w")
    for iord in iord_l:
        for icase in case_l:
            for idim in dim_l:
                for iqty in iqty_l:
                    centering = gl.qtyCentering(Qty_l[iqty][1], Direction_l[idim][1])
                    nbcells   = nbrCells[Direction_l[idim][1]][icase]
                    stepSize = meshSize[Direction_l[idim][1]][icase]

                    f.write(("%03d %d %s %03d %4.1f ") %
                       (interpOrder_l[iord],
                        dim_l[idim]+1, Qty_l[iqty][0],
                        nbcells, stepSize ) )

                    iStart = gl.physicalStartPrimal(interpOrder_l[iord])
                    iEnd   = gl.physicalEndPrimal  (interpOrder_l[iord], nbcells)

                    iEnd = iEnd - gl.isDual(centering)

                    # f.write(("Primal start = %03d \n") % (iStart))
                    # f.write(("Primal end   = %03d \n") % (iEnd))

                    f.write(("%d %d ") % (iStart, iEnd))
                    f.write(("%6.2f %6.2f %6.2f\n") % (origin[0], origin[1], origin[2]))

    f.close()


    for iord in iord_l:
        for icase in case_l:
            for idim in dim_l:
                for iqty in iqty_l:
                    centering = gl.qtyCentering(Qty_l[iqty][1], Direction_l[idim][1])
                    nbcells   = nbrCells[Direction_l[idim][1]][icase]
                    stepSize = meshSize[Direction_l[idim][1]][icase]

                    f = open((os.path.join(path,"fieldCoords_ord%d_dim%d_%s_case%d.txt") %
                    (interpOrder_l[iord], dim_l[idim]+1, Qty_l[iqty][1], icase)), "w")

                    iStart = gl.physicalStartPrimal(interpOrder_l[iord])
                    iEnd   = gl.physicalEndPrimal  (interpOrder_l[iord], nbcells)

                    iEnd = iEnd - gl.isDual(centering)

                    for iprimal in np.arange(iStart, iEnd+1):
                        x = fieldCoords(iprimal, iStart, Qty_l[iqty][1], Direction_l[idim], \
                                        stepSize, origin)
                        f.write(("%8.2f ") % (x))

                    f.close()


#np.savetxt("gridlayouttest.txt", caseParamsTable, delimiter=" ", fmt="%4.1f")



if __name__ == "__main__":
    main()
