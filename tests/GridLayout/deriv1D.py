#!/usr/bin/env python
#!coding: utf-8

import numpy as np
import math

import sys
sys.path.insert(0, '../')

import gridlayout


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


function_l=['sin', 'linear', 'square', 'compo01']

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
    nbrCellX_l=[40, 40, 40]
    nbrCellY_l=[ 0, 12, 12]
    nbrCellZ_l=[ 0,  0, 12]
    nbrCells = {'X':nbrCellX_l, 'Y':nbrCellY_l, 'Z':nbrCellZ_l}

    dx_l=[0.1, 0.1, 0.1] # 1D, 2D, 3D cases
    dy_l=[0. , 0.1, 0.1]
    dz_l=[0. , 0. , 0.1]
    meshSize= {'X':dx_l, 'Y':dy_l, 'Z':dz_l}

    origin = [0., 0., 0.]

    interpOrder_l=[1, 2, 3, 4]

    gl  = gridlayout.GridLayout()

    Direction_l = gl.Direction_l
    Qty_l       = gl.Qty_l

    origin = [0., 0., 0.]

    # functions to be derivated
    case_l=[0]

    #dim_l =[0, 1, 2]
    dim_l =[0]


    maxNbrDim = 3

    nbrTestCases = len(interpOrder_l)*len(dim_l)*len(Qty_l)*len(case_l)
    print( nbrTestCases )

    iord_l = np.arange( len(interpOrder_l) )
    print( iord_l )

    iqty_l = np.arange( len(Qty_l) )
    print( iqty_l )

    ifunc_l = np.arange( len(funcDict) )
    print( ifunc_l )


    # ------- Debug commands -------
    centering = 'primal'
    derivOrder = 1
    gl.changeCentering( centering, derivOrder )

    for icase in case_l:
        for idim in dim_l:
            nbcells = nbrCells[Direction_l[idim][1]][icase]

            for iqty in iqty_l:
                centering = gl.qtyCentering(Qty_l[iqty][1], Direction_l[idim][1])

                print( "field : %s, direction : %s, centering : %s" %
                      (Qty_l[iqty][1], Direction_l[idim][1], centering ) )

                print( "Nbr of cells = %d" % nbcells )

                print( "Nbr of ghost cells = %d on each side" %
                gl.nbrGhosts(interpOrder_l[0], centering ) )

                print( "Alloc size = %d" % \
                  gl.allocSize(interpOrder_l[0], centering, nbcells) )

                print( gl.physicalStartIndex(interpOrder_l[0], centering ),
                       gl.physicalEndIndex(interpOrder_l[0], centering, nbcells),
                       gl.ghostEndIndex(interpOrder_l[0], centering, nbcells)   )

    # -------- Let us define a function on Bx with Yee lattice --------
    icase = 0

    iqty = 0
    iord = 0
    idim = 0

    nbcells = nbrCells[Direction_l[idim][1]][icase]
    stepSize = meshSize[Direction_l[idim][1]][icase]

    f = open("deriv1D_summary.txt", "w")

    for ifunc in ifunc_l:
        f.write(("%03d %d %s %03d %5.4f ") %
           (interpOrder_l[iord],
            dim_l[idim]+1, Qty_l[iqty][0], nbcells, stepSize ) )

        iStart = gl.physicalStartPrimal(interpOrder_l[iord])
        iEnd   = gl.physicalEndPrimal  (interpOrder_l[iord], nbcells)
        iEnd = iEnd - gl.isDual( gl.qtyCentering(Qty_l[iqty][1], Direction_l[idim][1]) )

        f.write(("%d %d ") % (iStart, iEnd))

        f.write(("%6.2f %6.2f %6.2f ") % (origin[0], origin[1], origin[2]))

        f.write("%s \n" % function_l[ifunc])

    f.close()



    for ifunc in ifunc_l:
        f = open( ("deriv1D_%s_%s.txt") % (Qty_l[iqty][1], function_l[ifunc]), "w")

        iStart = gl.physicalStartPrimal(interpOrder_l[iord])
        iEnd   = gl.physicalEndPrimal  (interpOrder_l[iord], nbcells)

        iEnd = iEnd - gl.isDual( gl.qtyCentering(Qty_l[iqty][1], Direction_l[idim][1]) )

        # qtyCentering(Qty_l[iqty][1], Direction_l[idim][1])

        for iprimal in np.arange(iStart, iEnd+1):
            x = gl.fieldCoords(iprimal, iStart, Qty_l[iqty][1], Direction_l[idim], \
                                    stepSize, origin, 0)
            x_der = gl.fieldCoords(iprimal, iStart, Qty_l[iqty][1], Direction_l[idim], \
                                        stepSize, origin, 1)

            f.write(("%10.4f %14.10f   ") % (x    , funcDict[function_l[ifunc]](x)         ) )
            f.write(("%10.4f %14.10f \n") % (x_der, der_funcDict[function_l[ifunc]](x_der) ) )

        f.close()



if __name__ == "__main__":
    main()
# ------------------------------







