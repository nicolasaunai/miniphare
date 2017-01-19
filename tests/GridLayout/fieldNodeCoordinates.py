#!/usr/bin/env python
#!coding: utf-8

import numpy as np
#import math

import sys

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

    nbrOrder = 4
    nbrTestCases = nbrOrder * len(Qty_l)
    
    dim_l =[0]*nbrTestCases

    interpOrder_l= [1]*len(Qty_l) + [2]*len(Qty_l) \
                 + [3]*len(Qty_l) + [4]*len(Qty_l)

    quantities_l = [0, 1, 2, 3, 4, 5, 6, 7, 8]*nbrOrder

    nbrCellX_l = [40]*nbrTestCases
    nbrCellY_l = [ 0]*nbrTestCases
    nbrCellZ_l = [ 0]*nbrTestCases
    
    dx_l=[0.1]*nbrTestCases
    dy_l=[0. ]*nbrTestCases
    dz_l=[0. ]*nbrTestCases
    
    nbrCells = {'X':nbrCellX_l, 'Y':nbrCellY_l, 'Z':nbrCellZ_l}    
    meshSize= {'X':dx_l, 'Y':dy_l, 'Z':dz_l}

    origin = [0., 0., 0.]

    print( nbrTestCases )

    icase_l = np.arange( nbrTestCases )
        
    # ------- Debug commands -------
    for icase in icase_l:
        iqty = quantities_l[icase]
        idim = dim_l[icase]
        centering = gl.qtyCentering(Qty_l[iqty][1], Direction_l[idim][1])
        nbcells = nbrCells[Direction_l[idim][1]][icase]
        order = interpOrder_l[icase]

        print( "field : %s, direction : %s, centering : %s" %
              (Qty_l[iqty][1], Direction_l[idim][1], centering ) )

        print( "Nbr of cells = %d" % nbcells )

        print( "Nbr of ghost cells = %d on each side" %
        gl.nbrGhosts(interpOrder_l[0], centering) )

        print( "Alloc size = %d" %
        gl.allocSize(interpOrder_l[0], centering, nbcells ) )

        print( gl.physicalStartIndex(order, centering),
               gl.physicalEndIndex(order, centering, nbcells),
               gl.ghostEndIndex(order, centering, nbcells)  )

    # ------------------------------

    f = open(os.path.join(path,"fieldCoords_summary.txt"), "w")
    
    # the number of test cases
    f.write("%d \n" % nbrTestCases )      
    
    for icase in icase_l:
        iqty = quantities_l[icase]
        idim = dim_l[icase]
        centering = gl.qtyCentering(Qty_l[iqty][1], Direction_l[idim][1])
        nbcells   = nbrCells[Direction_l[idim][1]][icase]
        stepSize = meshSize[Direction_l[idim][1]][icase]
        order = interpOrder_l[icase]


        f.write(("%03d %d %s %03d %6.4f ") %
           (order, dim_l[idim]+1, Qty_l[iqty][0],
            nbcells, stepSize ) )

        iStart = gl.physicalStartPrimal(order)
        iEnd   = gl.physicalEndPrimal  (order, nbcells)

        iEnd = iEnd - gl.isDual(centering)

        # f.write(("Primal start = %03d \n") % (iStart))
        # f.write(("Primal end   = %03d \n") % (iEnd))

        f.write(("%d %d ") % (iStart, iEnd))
        f.write(("%6.2f %6.2f %6.2f\n") % (origin[0], origin[1], origin[2]))

    f.close()


    for icase in icase_l:
        iqty = quantities_l[icase]
        idim = dim_l[icase]
        centering = gl.qtyCentering(Qty_l[iqty][1], Direction_l[idim][1])
        nbcells   = nbrCells[Direction_l[idim][1]][icase]
        stepSize = meshSize[Direction_l[idim][1]][icase]
        order = interpOrder_l[icase]

        # _case%d
        f = open((os.path.join(path,"fieldCoords_ord%d_dim%d_%s.txt") %
        (order, dim_l[idim]+1, Qty_l[iqty][1])), "w")

        iStart = gl.physicalStartPrimal(order)
        iEnd   = gl.physicalEndPrimal  (order, nbcells)

        iEnd = iEnd - gl.isDual(centering)

        for iprimal in np.arange(iStart, iEnd+1):
            x = fieldCoords(iprimal, iStart, Qty_l[iqty][1], Direction_l[idim], \
                            stepSize, origin)
            f.write(("%8.2f ") % (x))

        f.close()


#np.savetxt("gridlayouttest.txt", caseParamsTable, delimiter=" ", fmt="%4.1f")



if __name__ == "__main__":
    main()
