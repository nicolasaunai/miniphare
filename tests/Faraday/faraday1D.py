#!/usr/bin/env python
#!coding: utf-8

import numpy as np
import math
#import matplotlib.pyplot as plt
#import scipy.integrate as scint
#import mpl_toolkits.mplot3d as mplt3d
#from math import factorial
#%matplotlib inline

import sys
sys.path.insert(0, '../')


import gridlayout
import os

# --------------------------------------------------------------------------

# We return 6 values Bx, By, Bz, Ex, Ey, Ez
#
def test03( field, x, x0, t, dt ):
    
    value = 0.
    
    if field == 'Bz':
        t_yee = t - 0.5*dt
        value = -2.*(x-x0)*math.exp(-pow(x-x0, 2.))*math.sin(t_yee)
    elif field == 'Ey':
        t_yee = t
        value = -math.exp(-pow(x-x0, 2.))*math.cos(t_yee)

    return value


faradayDict = {'test03': test03 }

# --------------------------------------------------------------------------

def field_list( test_name ):
    
    if test_name == 'test03':
        print(test_name)
        size = 2
        field_l = ["" for string in range(size)]
        field_l[0] = "Bz"
        field_l[1] = "Ey"   
    
    return field_l


# ---------------------- MAIN CODE -----------------------------------------
def main(path='./'):

    if len(sys.argv) == 2:
        path = sys.argv[1]
    
    # ---------------------- INITIALIZATION  -------------------------------
    interpOrder_l=[1]
    
    nbrCellX_l=[40]
    nbrCellY_l=[ 0]
    nbrCellZ_l=[ 0]
    
    dx_l=[0.1] 
    dy_l=[0. ]
    dz_l=[0. ]
    
    dim_l =[1] # 1 means 1D    
    
    origin = [0., 0., 0.]    

    gl = gridlayout.GridLayout()
    
    Direction_l = gl.Direction_l
    Qty_l = gl.Qty_l
    
    faraday_test_l=['test03']
    
    dt = 0.05
    
    t_start = 0.5
    t_end = 1.
    nstep = math.floor((t_end-t_start)/dt) +1
    
    print("nstep = %d" % nstep)
    
    time_l = np.linspace(t_start, t_end, nstep)
    print(time_l)    
    
    
    nbrTestCases = len(faraday_test_l)
    print( "Number of test cases : %d" % nbrTestCases )
    
    iord_l = np.arange( len(interpOrder_l) ) 
    print( iord_l )
    
    itime_l = np.arange( len(time_l) )
    
    iqty_l = np.arange( len(Qty_l) ) 
    print( iqty_l )
    
    icase_l = np.arange( len(faraday_test_l) )
    
   
    iqty = 0
    
    x0 = 2.
    
    f = open("faraday1D_summary.txt", "w")
    
    for icase in icase_l:    
        nbrCellsX = nbrCellX_l[icase]
        nbrCellsY = nbrCellY_l[icase]
        nbrCellsZ = nbrCellZ_l[icase]
        
        dx = dx_l[icase]
        dy = dy_l[icase]
        dz = dz_l[icase]
        
        centeringX = gl.qtyCentering(Qty_l[iqty][1], 'X')
        
        f.write(("%d %d %d %d %d %5.4f %5.4f %5.4f  ") % 
           (interpOrder_l[icase], dim_l[icase], 
            nbrCellsX, nbrCellsY, nbrCellsZ, 
            dx, dy, dz ) )                   
           
        iStart = gl.physicalStartPrimal(interpOrder_l[icase])
        iEnd   = gl.physicalEndPrimal  (interpOrder_l[icase], nbrCellsX) 
        
        f.write(("%10.4f %10.4f %10.4f ") % (dt, t_start, t_end))
           
    #    f.write(("%6.2f %6.2f %6.2f ") % (origin[0], origin[1], origin[2]))
        
        f.write("%s " % faraday_test_l[icase])
        
        field_l = field_list( faraday_test_l[icase] )    
        ifield_l = np.arange( len(field_l) )
        
        f.write("%d " % len(field_l) )        
        
        for ifield in ifield_l:
            f.write("%s " % field_l[ifield])
            
        f.write("%d " % len(time_l) )        
    
        f.write("\n")
    f.close()
    
    
    itime = 0
    
    for icase in icase_l:
        nbrCellsX = nbrCellX_l[icase]
        nbrCellsY = nbrCellY_l[icase]
        nbrCellsZ = nbrCellZ_l[icase]
        
        dx = dx_l[icase]
        dy = dy_l[icase]
        dz = dz_l[icase]
                    
        field_l = field_list( faraday_test_l[icase] )    
        ifield_l = np.arange( len(field_l) )    
        
        for itime in itime_l:    
        
            for ifield in ifield_l:
                f = open( ("faraday1D_%s_%s_t%d.txt") % (faraday_test_l[icase], field_l[ifield], itime), "w")
        
                print("field_l[ifield] = %s" % field_l[ifield])
                centeringX = gl.qtyCentering(field_l[ifield], 'X')
                print("%s along %s is %s" % (field_l[ifield], 'X', centeringX) )
                                
                iStart = gl.physicalStartIndex(interpOrder_l[icase], centeringX)
                iEnd   = gl.physicalEndIndex  (interpOrder_l[icase], centeringX, nbrCellsX)       
        
                print("iStart : %d" % iStart)
                print("iEnd   : %d" % iEnd)
        
                print("time = %7.3f" % time_l[itime])
                for iprimal in np.arange(iStart, iEnd+1):
                    x = gl.fieldCoords(iprimal, iStart, field_l[ifield], Direction_l[0], \
                                            dx, origin, 0)
                                           
                    fx = faradayDict[faraday_test_l[icase]](field_l[ifield], x, x0, time_l[itime], dt)
                                           
                    f.write(("%f   %f \n") % (x, fx) )
                
                f.close()
        
    # ------------------------------

if __name__ == "__main__":
    main()
    



