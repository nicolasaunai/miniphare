#!/usr/bin/env python3
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
# The vector potential A(Ax, Ay, Az)
# is defined by:
# Ay = exp( -(x-x0)**2 ) * sin(t)
#
# E(r, t) = -d A(r, t)/ dt    (partial derivative)
# B(r, t) = Rot( A(r, t) )
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


#
# The vector potential A(Ax, Ay, Az) of 
# a circularly polarized plane wave is defined by:
# 
# Ay = a0/sqrt(2) * cos(t-x)
# Az = a0/sqrt(2) * sin(t-x)
#
#
def circPolarizedWave( field, x, a0, t, dt ):
    
    value = 0.
    coef = a0/math.sqrt(2.)
    
    if field == 'By':
        t_yee = t - 0.5*dt
        value = coef*math.cos(t_yee - x)
    elif field == 'Bz':
        t_yee = t - 0.5*dt
        value = coef*math.sin(t_yee - x)
    elif field == 'Ey':
        t_yee = t
        value = coef*math.sin(t_yee - x)
    elif field == 'Ez':
        t_yee = t
        value = -coef*math.cos(t_yee - x)

    return value
    

faradayDict = {'test03': test03, 'circPolarizedWave': circPolarizedWave }

# --------------------------------------------------------------------------

def field_list( test_name ):
    
    if test_name == 'test03':
        print(test_name)
        size = 2
        field_l = ["" for string in range(size)]
        field_l[0] = "Bz"
        field_l[1] = "Ey"   
    
    if test_name == 'circPolarizedWave':
        print(test_name)
        size = 4
        field_l = ["" for string in range(size)]
        field_l[0] = "By"
        field_l[1] = "Bz"
        field_l[2] = "Ey"
        field_l[3] = "Ez"
    
    return field_l


# ---------------------- MAIN CODE -----------------------------------------
def main(path='./'):

    if len(sys.argv) == 2:
        path = sys.argv[1]
    
    # ---------------------- INITIALIZATION  -------------------------------
    x0 = 2.
    a0 = 3.    
    
    interpOrder_l=[1, 1, 1]
    
    nbrCellX_l=[40, 40, 40]
    nbrCellY_l=[ 0,  0,  0]
    nbrCellZ_l=[ 0,  0,  0]
    
    dx_l=[0.1, 0.1, 0.1] 
    dy_l=[0. , 0. , 0. ]
    dz_l=[0. , 0. , 0. ]
    
    dim_l =[1, 1, 1] # 1 means 1D    
    
    param_l = [x0, a0, 2*a0]    

    faraday_test_l=['test03', 'circPolarizedWave', 'circPolarizedWave']
    
    origin = [0., 0., 0.]    

    gl = gridlayout.GridLayout()
    
    Direction_l = gl.Direction_l
    Qty_l = gl.Qty_l
    
    
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
    

    f = open(os.path.join(path,"faraday1D_summary.txt"), "w")


    # the number of test cases
    f.write("%d \n" % len(icase_l) )     
    
    for icase in icase_l:    
        nbrCellsX = nbrCellX_l[icase]
        nbrCellsY = nbrCellY_l[icase]
        nbrCellsZ = nbrCellZ_l[icase]
        
        dx = dx_l[icase]
        dy = dy_l[icase]
        dz = dz_l[icase]
        
        f.write(("%d %d %d %d %d %5.4f %5.4f %5.4f  ") % 
           (interpOrder_l[icase], dim_l[icase], 
            nbrCellsX, nbrCellsY, nbrCellsZ, 
            dx, dy, dz ) )                   
           
        iStart = gl.physicalStartPrimal(interpOrder_l[icase])
        iEnd   = gl.physicalEndPrimal  (interpOrder_l[icase], nbrCellsX) 
        
        f.write(("%10.4f %10.4f %10.4f ") % (dt, t_start, t_end))
        f.write("%d " % len(time_l) ) 
        
        f.write("%s " % faraday_test_l[icase])
        
        field_l = field_list( faraday_test_l[icase] )    
        ifield_l = np.arange( len(field_l) )
        
        f.write("%d " % len(field_l) )        
        
        for ifield in ifield_l:
            f.write("%s " % field_l[ifield])       
    
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
                f = open( os.path.join(path,("faraday1D_%s_%s_t%d.txt") % \
                (faraday_test_l[icase], field_l[ifield], itime)), "w")
        
                print("field_l[ifield] = %s" % field_l[ifield])
                centeringX = gl.qtyCentering(field_l[ifield], 'X')
                print("%s along %s is %s" % (field_l[ifield], 'X', centeringX) )
                                
                iStart = gl.physicalStartIndex(interpOrder_l[icase], centeringX)
                iEnd   = gl.physicalEndIndex  (interpOrder_l[icase], centeringX, nbrCellsX)       
        
                print("iStart : %d" % iStart)
                print("iEnd   : %d" % iEnd)
        
                print("time = %7.3f" % time_l[itime])
                for knode in np.arange(iStart, iEnd+1):
                    x = gl.fieldCoords(knode, iStart, field_l[ifield], Direction_l[0], \
                                            dx, origin, 0)
                                           
                    fx = faradayDict[faraday_test_l[icase]](field_l[ifield], x, param_l[icase], time_l[itime], dt)
                                           
                    f.write(("%f   %f \n") % (x, fx) )
                
                f.close()
        
    # ------------------------------

if __name__ == "__main__":
    main()
    



