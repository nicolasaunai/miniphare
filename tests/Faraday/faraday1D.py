# -*- coding: utf-8 -*-
#"""
#Created on Fri Oct  7 16:54:36 2016
#
#@author: mdrouin
#"""

# In[1]:

import numpy as np
import math
#import matplotlib.pyplot as plt
#import scipy.integrate as scint
#import mpl_toolkits.mplot3d as mplt3d
#from math import factorial
#%matplotlib inline

import sys
sys.path.insert(0, '../')

import commons 



# ---------------------- INITIALIZATION  -----------------------------------

interpOrder_l=[1, 2, 3, 4]

Direction_l = commons.Direction_l
Qty_l = commons.Qty_l
#function_l = commons.function_l

faraday_test_l=['test03']

funcDict = commons.funcDict
der_funcDict = commons.der_funcDict



dt = 0.05

t_start = 0.5
t_end = 1.
nstep = math.floor((t_end-t_start)/dt) +1

print("nstep = %d" % nstep)

time_l = np.linspace(t_start, t_end, nstep)
print(time_l)


origin = [0., 0., 0.]

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
    
    if test_name == faraday_test_l[0]:
        print(test_name)
        size = 2
        field_l = ["" for string in range(size)]
        field_l[0] = "Bz"
        field_l[1] = "Ey"   
    
    return field_l


# ---------------------- MAIN CODE -----------------------------------------

nbrTestCases = len(faraday_test_l)
print( "Number of test cases : %d" % nbrTestCases )

iord_l = np.arange( len(interpOrder_l) ) 
print( iord_l )

itime_l = np.arange( len(time_l) )

iqty_l = np.arange( len(Qty_l) ) 
print( iqty_l )

itest_l = np.arange( len(faraday_test_l) )
print( itest_l )


dim_l =[0] 
icase = 0

iqty = 0
iord = 0
idim = 0

x0 = 2.

f = open("faraday1D_summary.txt", "w")


for itest in itest_l:
    #for iqty in iqty_l:   
    f.write(("%03d %d %s %03d %5.4f ") % 
       (interpOrder_l[iord],
        dim_l[idim]+1,
        Qty_l[iqty][0],
        commons.nbrCells(Direction_l[idim][1], icase),
        commons.spatialStep(Direction_l[idim][1], icase) ) )                   
       
    iStart = commons.physicalStartPrimal(interpOrder_l[iord])
    iEnd   = commons.physicalEndPrimal  (interpOrder_l[iord], Direction_l[idim][1], icase)                
    iEnd = iEnd - commons.isDual( commons.qtyCentering(Qty_l[iqty][1], Direction_l[idim][1]) )         
    
    f.write(("%d %d ") % (iStart, iEnd))
    
    f.write(("%10.4f %10.4f %10.4f ") % (dt, t_start, t_end))
       
#    f.write(("%6.2f %6.2f %6.2f ") % (origin[0], origin[1], origin[2]))
    
    f.write("%s \n" % faraday_test_l[itest])

f.close()


itime = 0

for itest in itest_l:
    field_l = field_list( faraday_test_l[itest] )    
    
    ifield_l = np.arange( len(field_l) )    
    
    for itime in itime_l:    
    
        for ifield in ifield_l:
            f = open( ("faraday1D_%s_%s_t%d.txt") % (faraday_test_l[itest], field_l[ifield], itime), "w")
    
            print("field_l[ifield] = %s" % field_l[ifield])
            qtyCentering = commons.qtyCentering(field_l[ifield], Direction_l[idim][1])
            print("%s along %s is %s" % (field_l[ifield],  Direction_l[idim][1], qtyCentering) )
            
            
            iStart = commons.physicalStartIndex(interpOrder_l[iord], qtyCentering)
            iEnd   = commons.physicalEndIndex  (interpOrder_l[iord], qtyCentering, \
                                                Direction_l[idim][1], icase)       
    
            print("iStart : %d" % iStart)
            print("iEnd   : %d" % iEnd)
    
            print(time_l[itime])
            for iprimal in range(iStart, iEnd+1):
                x = commons.fieldCoords(iprimal, iStart, field_l[ifield], Direction_l[idim], \
                                       commons.spatialStep(Direction_l[idim][1], icase), origin, 0)
                                       
                fx = faradayDict[faraday_test_l[itest]](field_l[ifield], x, x0, time_l[itime], dt)
                                       
                f.write(("%f   %f \n") % (x, fx) )
            
            f.close()
    
# ------------------------------




