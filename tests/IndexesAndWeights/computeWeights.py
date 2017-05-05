#!/usr/bin/env python
#!coding: utf-8

import numpy as np
import math

import sys
import os
sys.path.insert(0, os.path.join(os.path.dirname(os.path.realpath(__file__)),'..'))

import gridlayout
import interpolator


#
#  Ponderations (or weights) are needed:
#  - to interpolate a field onto a point
#  - or to compute the contribution of a particle to charge density and fluxes
#
#  Those 2 operations use a list of points:
#  - Interpolation of a field to a particle requires a list of indexes
#  from the grid 
#  - Depositing the contribution of a particle to a grid requires the grid indexes
#  where the particle will deposit
# 
#  This python script computes those weights and writes them into files.
#
#  The weights are actually computed by interpolator.py
#  interpolator.py has been validated with exactweights.py, the latter 
#  script can be used to plot shape functions from 1st to 4th order
#
#
def main(path='./'):

    if len(sys.argv) == 2:
        path = sys.argv[1]
    # ---------------------- INITIALIZATION  -----------------------------------
    nbrParts = 2
    nbrOrder = 4
    nbrTestCases = nbrParts*nbrOrder

    nbrCellX_l=[40]*nbrTestCases
#    nbrCellY_l=[ 0]*nbrTestCases
#    nbrCellZ_l=[ 0]*nbrTestCases

    dx_l=[0.1]*nbrTestCases 
#    dy_l=[0. ]*nbrTestCases
#    dz_l=[0. ]*nbrTestCases

    interpOrder_l=[1, 2, 3, 4]*nbrParts

    # Hybrid quantities
    field_l=['rho']*nbrTestCases
    
    patchMinX_l = [0.]*nbrTestCases

    particlePosX_l = [1.01]*nbrOrder + [1.09]*nbrOrder
    
    gl  = gridlayout.GridLayout()
    
    interp = interpolator.Interpolator()

    print( nbrTestCases )

    icase_l = np.arange( nbrTestCases )
    print( icase_l )


    # -------- Let us define a function on Bx with Yee lattice --------
    f = open(os.path.join(path,"weights_summary.txt"), "w")

    # the number of test cases
    f.write("%d \n" % len(icase_l) )  

    for icase in icase_l:
        order = interpOrder_l[icase]
        nbrCellX = nbrCellX_l[icase]
        dx = dx_l[icase]
        field = field_l[icase]
        xmin = patchMinX_l[icase]
        xpart = particlePosX_l[icase]

        f.write(("%d %d %5.4f %s %f %f") %
           (order, nbrCellX, 
            dx, field, 
            xmin, xpart ) )

        f.write("\n")

    f.close()

    for icase in icase_l:
        f = open( os.path.join(path,("weights_testCase%d.txt") % \
        (icase_l[icase])), "w")
        
        order = interpOrder_l[icase]
        nbrCellX = nbrCellX_l[icase]
        dx = dx_l[icase]
        field = field_l[icase]
        xmin = patchMinX_l[icase]
        xpart = particlePosX_l[icase]
        
        reduced = interp.reducedCoord( field, xmin, xpart, dx, gl )
        
        indexes = interp.indexList(order, reduced)
#        print(indexes)
                    
#        print("xpart = ", xpart)            
        x_ticks = indexes * dx + xmin
#        print("x_ticks[] = ", x_ticks)        
        shape = interp.getShape(order, x_ticks, xpart, dx)
#        print("shape[] = ", shape)
#        print("Sum(shape[]) = %f" % sum(shape))
            
        for weight in shape:
            f.write("%.16f " % weight)
        
        f.close()
        

if __name__ == "__main__":
    main()


