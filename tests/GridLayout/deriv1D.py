#!/usr/bin/env python
#!coding: utf-8

import numpy as np
import math

import sys
sys.path.insert(0, '../')

import gridlayout
import os


# --------- mathematical functions ----------------------
def unity( x ):
    return 1.

def linear( x ):
    return x

def square( x ):
    return pow(x, 2.)

def d_square( x ):
    return 2.*linear(x)

# compo01 = x2_plus_sin
def compo01( x ):
    val = square(x) + math.sin(x)
    return val

def d_compo01( x ):
    val = 2.*linear(x) + math.cos(x)
    return val
# ---------------------------------------------------------

funcDict = {'sin'    : math.sin,
            'linear' : linear,
            'square' : square,
            'compo01': compo01 }

# dictionnary for first order derivatives
der_funcDict = {'sin'    : math.cos,
                'linear' : unity,
                'square' : d_square,
                'compo01': d_compo01 }


def main(path='./'):

    if len(sys.argv) == 2:
        path = sys.argv[1]
    # ---------------------- INITIALIZATION  -----------------------------------
    nbrCellX_l=[40, 40, 40, 40, 40, 40, 40, 40]
    nbrCellY_l=[ 0,  0,  0,  0,  0,  0,  0,  0]
    nbrCellZ_l=[ 0,  0,  0,  0,  0,  0,  0,  0]
    nbrCells = {'X':nbrCellX_l, 'Y':nbrCellY_l, 'Z':nbrCellZ_l}

    dx_l=[0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1] # 1D, 2D, 3D cases
    dy_l=[0. , 0. , 0. , 0. , 0. , 0. , 0. , 0. ]
    dz_l=[0. , 0. , 0. , 0. , 0. , 0. , 0. , 0. ]
    meshSize= {'X':dx_l, 'Y':dy_l, 'Z':dz_l}

    function_l=['sin', 'linear', 'square', 'compo01', 'sin', 'linear', 'square', 'compo01']

    interpOrder_l=[1, 1, 1, 1, 1, 1, 1, 1]

    # functions to be derivated : iqty
    qty_l=[0, 0, 0, 0, 1, 1, 1, 1]

    #dim_l =[0, 1, 2]
    dim_l =[0, 0, 0, 0, 0, 0, 0, 0]

    gl  = gridlayout.GridLayout()

    Direction_l = gl.Direction_l
    Qty_l       = gl.Qty_l

    origin = [0., 0., 0.]


    nbrTestCases = len(qty_l)
    print( nbrTestCases )

    print( interpOrder_l )
    print( qty_l )
    print( dim_l )
    print( function_l )

    icase_l = np.arange( len(qty_l) )
    print( icase_l )


    # -------- Let us define a function on Bx with Yee lattice --------
    f = open(os.path.join(path,"deriv1D_summary.txt"), "w")

    # the number of test cases
    f.write("%d \n" % len(icase_l) )  

    for icase in icase_l:
        idim  = dim_l[icase]
        order = interpOrder_l[icase]
        iqty = Qty_l[qty_l[icase]][0]  # 0 ,  1,  2,  3, ... 
        qty  = Qty_l[qty_l[icase]][1]  # Bx, By, Bz, Ex, ...
        
        nbcells = nbrCells[Direction_l[idim][1]][icase]
        stepSize = meshSize[Direction_l[idim][1]][icase]

        f.write(("%03d %d %s %03d %5.4f ") %
           (order, idim+1, 
            iqty, nbcells, stepSize ) )

        centering = gl.qtyCentering(qty, Direction_l[idim][1])

        iStart = gl.physicalStartIndex(order, centering)
        
        iGhostStart = gl.ghostStartIndex()
        iGhostEnd   = gl.ghostEndIndex  (order, centering, nbcells)

        # last argument means 1st derivative
        newCentering = gl.changeCentering( centering, 1 )

        iDerStart = gl.physicalStartIndex(order, newCentering)
        iDerEnd   = gl.physicalEndIndex  (order, newCentering, nbcells)

        f.write(("%d %d %d %d ") % (iGhostStart, iGhostEnd, iDerStart, iDerEnd))

        f.write(("%6.2f %6.2f %6.2f ") % (origin[0], origin[1], origin[2]))

        f.write("%s \n" % function_l[icase])

    f.close()



    for icase in icase_l:
        idim  = dim_l[icase]
        order = interpOrder_l[icase]
        iqty = Qty_l[qty_l[icase]][0]  # 0 ,  1,  2,  3, ... 
        qty  = Qty_l[qty_l[icase]][1]  # Bx, By, Bz, Ex, ...
        
        nbcells = nbrCells[Direction_l[idim][1]][icase]
        stepSize = meshSize[Direction_l[idim][1]][icase]
        
        f = open( os.path.join(path,"deriv1D_%s_%s.txt" % (qty, function_l[icase]) ), "w")

        centering = gl.qtyCentering(qty, Direction_l[idim][1])

        iStart = gl.physicalStartIndex(order, centering)

        iGhostStart = gl.ghostStartIndex()
        iGhostEnd   = gl.ghostEndIndex  (order, centering, nbcells)

        for knode in np.arange(iGhostStart, iGhostEnd+1):
            x = gl.yeeCoords(knode, iStart, centering, Direction_l[idim], \
                                    stepSize, origin, 0)
            f.write(("%10.4f %14.10f \n") % (x    , funcDict[function_l[icase]](x)         ) )

        f.write("\n \n")
        
        # we take the centering of the 1st derivative
        newCentering = gl.changeCentering( centering, 1 )

        iDerStart = gl.physicalStartIndex(order, newCentering)
        iDerEnd   = gl.physicalEndIndex  (order, newCentering, nbcells)
            
        for knode in np.arange(iDerStart, iDerEnd+1):
            x_der = gl.yeeCoords(knode, iDerStart, newCentering, Direction_l[idim], \
                                        stepSize, origin, 0)
            f.write(("%10.4f %14.10f \n") % (x_der, der_funcDict[function_l[icase]](x_der) ) )            
            

        f.close()



if __name__ == "__main__":
    main()
# ------------------------------


    # ------- Debug commands -------
#    centering = 'primal'
#    derivOrder = 1
#    gl.changeCentering( centering, derivOrder )
#
#    for icase in case_l:
#            nbcells = nbrCells[Direction_l[dim_l[icase]][1]][icase]
#
#            for iqty in iqty_l:
#                centering = gl.qtyCentering(Qty_l[iqty][1], Direction_l[dim_l[icase]][1])
#
#                print( "field : %s, direction : %s, centering : %s" %
#                      (Qty_l[iqty][1], Direction_l[dim_l[icase]][1], centering ) )
#
#                print( "Nbr of cells = %d" % nbcells )
#
#                print( "Nbr of ghost cells = %d on each side" %
#                gl.nbrGhosts(interpOrder_l[0], centering ) )
#
#                print( "Alloc size = %d" % \
#                  gl.allocSize(interpOrder_l[0], centering, nbcells) )
#
#                print( gl.physicalStartIndex(interpOrder_l[0], centering ),
#                       gl.physicalEndIndex(interpOrder_l[0], centering, nbcells),
#                       gl.ghostEndIndex(interpOrder_l[0], centering, nbcells)   )




