#!/usr/bin/env python
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
    nbrParts = 2
    nbrOrder = 4
    nbrTestCases = nbrParts*nbrOrder

    nbrCellX_l=[40]*nbrTestCases
    nbrCellY_l=[ 0]*nbrTestCases
    nbrCellZ_l=[ 0]*nbrTestCases
    nbrCells = {'X':nbrCellX_l, 'Y':nbrCellY_l, 'Z':nbrCellZ_l}

    dx_l=[0.1]*nbrTestCases 
    dy_l=[0. ]*nbrTestCases
    dz_l=[0. ]*nbrTestCases
    meshSize= {'X':dx_l, 'Y':dy_l, 'Z':dz_l}

    interpOrder_l=[1, 2, 3, 4]*nbrParts

    # Hybrid quantities
    field_l=['rho']*nbrTestCases
    
    patchMinX_l = [0.]*nbrTestCases

    particlePosX_l = [1.01]*nbrOrder + [1.09]*nbrOrder
    
    gl  = gridlayout.GridLayout()

    Direction_l = gl.Direction_l
    Qty_l       = gl.Qty_l

    print( nbrTestCases )

    icase_l = np.arange( nbrTestCases )
    print( icase_l )


    # -------- Let us define a function on Bx with Yee lattice --------
    f = open(os.path.join(path,"indexes_summary.txt"), "w")

    # the number of test cases
    f.write("%d \n" % len(icase_l) )  

    for icase in icase_l:
        order = interpOrder_l[icase]
        nbrX = nbrCellX_l[icase]
        dx = dx_l[icase]
        field = field_l[icase]
        xmin = patchMinX_l[icase]
        xpart = particlePosX_l[icase]

        f.write(("%d %d %5.4f %s %f %f") %
           (order, nbrX, 
            dx, field, 
            xmin, xpart ) )

        f.write("\n")

    f.close()

    for icase in icase_l:
        f = open( os.path.join(path,("indexes_testCase%d.txt") % \
        (icase_l[icase])), "w")
        
        order = interpOrder_l[icase]
        nbrX = nbrCellX_l[icase]
        dx = dx_l[icase]
        field = field_l[icase]
        xmin = patchMinX_l[icase]
        xpart = particlePosX_l[icase]
        
        reduced = reducedCoord( field, xmin, xpart, dx, gl )
        
        indexes = indexList(order, reduced)
        print(indexes)
        size = len(indexes)
        print("len(indexes) : %d" % size ) 
        
        for index in indexes:
            f.write("%d " % index)
        
        
        f.close()



def reducedCoord( field, xmin, xpart, dx, gl ):
    
    centeringX = gl.qtyCentering(field, 'X')    
    print(centeringX)
    
    delta = 0.
    if centeringX == 'dual':
        delta = 0.5
        
    reduced = (xpart + delta*dx - xmin)/dx

    return reduced
    


def indexList( order, reduced ):
    imin = math.floor( reduced - 0.5*(order-1.) )

    ik_l = np.arange(order+1)   
    
    indexes = imin + ik_l
    
    return indexes
    
    

if __name__ == "__main__":
    main()





