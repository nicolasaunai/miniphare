# -*- coding: utf-8 -*-
"""
Created on Fri Sep 30 11:56:02 2016

@author: mdrouin
"""

# In[1]:

import numpy as np
import matplotlib.pyplot as plt
import scipy.integrate as scint
import mpl_toolkits.mplot3d as mplt3d
import math
#from math import factorial
#%matplotlib inline

nbPadding = [10,10,10]

Direction_l = [(0,'X'), (1,'Y'), (2,'Z')]

Qty_l=[(0,'Bx'), (1,'By'), (2,'Bz'), (3,'Ex'), (4,'Ey'), (5,'Ez'), (6,'rho'), (7,'V'), (8,'P')]

QtyDict = {'Bx':0, 'By':1, 'Bz':2, 'Ex':3, 'Ey':4, 'Ez':5, 'rho':6, 'V':7, 'P':8}


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

CenterX = {'Bx':'primal' , 'By':'dual'  , 'Bz':'dual'  , 
           'Ex':'dual'   , 'Ey':'primal', 'Ez':'primal', 
           'rho':'primal', 'V':'primal' , 'P':'primal' }

CenterY = {'Bx':'dual'   , 'By':'primal', 'Bz':'dual'  , 
           'Ex':'primal' , 'Ey':'dual'  , 'Ez':'primal', 
           'rho':'primal', 'V':'primal' , 'P':'primal' }
           
CenterZ = {'Bx':'dual'   , 'By':'dual'  , 'Bz':'primal', 
           'Ex':'primal' , 'Ey':'primal', 'Ez':'dual'  , 
           'rho':'primal', 'V':'primal' , 'P':'primal' }


nbrCellX_l=[ 40, 40, 40]
nbrCellY_l=[ 0 , 12, 12]
nbrCellZ_l=[ 0 ,  0, 12]

dx_l=[0.1 , 0.1, 0.1] # 1D, 2D, 3D cases 
dy_l=[0.  , 0.1, 0.1]
dz_l=[0.  , 0. , 0.1]

origin = [0., 0., 0.]
           

# ---------------------- ANY DIRECTION -----------------------------------------
def qtyCentering(qty, direct):
    if direct == 'X':
        return CenterX[qty]
    elif direct == 'Y':
        return CenterY[qty]
    elif direct == 'Z':
        return CenterZ[qty]

def nbrGhosts(interpOrder, centering):
    if centering == 'primal':
        return math.floor( interpOrder/2 )
    else:
        return math.floor( (interpOrder +1)/2 )

def nbrGhostsPrimal(interpOrder):
    return math.floor( interpOrder/2 )

def isDual(centering):
    if centering == 'dual':
        return 1
    else:
        return 0
        
def nbrCells(direction, icas):
    if direction == 'X':
        return nbrCellX_l[icas]
    elif direction == 'Y':
        return nbrCellY_l[icas]
    elif direction == 'Z':
        return nbrCellZ_l[icas]

def spatialStep(direction, icas):
    if direction == 'X':
        return dx_l[icas]
    elif direction == 'Y':
        return dy_l[icas]
    elif direction == 'Z':
        return dz_l[icas]

# ---- Start / End index methods -----------
def ghostStartIndex():
    return 0

def physicalStartIndex(interpOrder, centering):
    index = ghostStartIndex() + nbrGhosts(interpOrder, centering) 
    return index

def physicalEndIndex(interpOrder, centering, direction, caseNbr):
    index = physicalStartIndex(interpOrder, centering) \
    + nbrCells(direction, caseNbr) - isDual(centering)    
    return index

def ghostEndIndex(interpOrder, centering, direction, caseNbr):
    index = physicalEndIndex(interpOrder, centering, direction, caseNbr) \
    + nbrGhosts(interpOrder, centering)       
    return index
    
# ---- Start / End   primal methods ------
def physicalStartPrimal(interpOrder):
    index = ghostStartIndex() + nbrGhostsPrimal(interpOrder)     
    return index

def physicalEndPrimal(interpOrder, direction, caseNbr):
    index = physicalStartPrimal(interpOrder) + nbrCells(direction, caseNbr)    
    return index

# ---- Alloc methods -------------------------

def allocSize(interpOrder, centering, direction, caseNbr):
    size = nbrCells(direction, caseNbr) + 1 \
    + 2*nbrGhosts(interpOrder, centering)    
    
    return size


# ---- Get coordinate methods -------------------------
# ds stands for dx or dy or dz
#
# This method returns a point
#
def fieldCoords(iprimal, iStart, qty, direction, ds, origin, derivOrder):
    halfCell = 0.    
        
    newCentering = changeCentering( qtyCentering(qty, direction[1]), derivOrder )                
        
    if newCentering == 'dual':
        halfCell = 0.5 
        
    x = ( (iprimal - iStart) + halfCell )*ds + origin[direction[0]]

    return x

# ---- Change centering method -------------------------
#
# Use case:
#   changeCentering( qtyCentering(qty, direct), 1 )
#
def changeCentering( centering, derivOrder ):

#    print( "Inputs\n   centering : %s\n   derivOrder : %d" % (centering, derivOrder) )

    newCentering = centering

    # if derivOrder is odd the centering 
    # is changed    
    if derivOrder%2 != 0:
        newCentering = swapCentering( centering )
        
#    print( "   output : %s" % newCentering )

    return newCentering
    
# -------------------------------------------------------
def swapCentering( centering ):
    
    newCentering = 'primal'    
    
    if centering == 'primal':
        newCentering = 'dual'
        
    return newCentering


# ---------------------- MAIN CODE -----------------------------------------

paramList = ['interpOrder', 'nbDims', 'Quantity', 'nbrCellX', 'nbrCellY', 'nbrCellZ', 'dx', 'dy', 'dz'];
len( paramList )
print( paramList[:] )
print( paramList[2] )

interpOrder_l=[1, 2, 3, 4]

# functions to be derivated
case_l=[0] 

#dim_l =[0, 1, 2] 
dim_l =[0] 


maxNbrDim = 3

nbrTestCases = len(interpOrder_l)*len(dim_l)*len(Qty_l)*len(case_l)
print( nbrTestCases )

caseParamsTable = np.zeros((nbrTestCases, len(paramList) + 2*maxNbrDim))

col_l = np.arange( len(paramList) )
print( col_l )

iord_l = np.arange( len(interpOrder_l) ) 
print( iord_l )

iqty_l = np.arange( len(Qty_l) ) 
print( iqty_l )

ifunc_l = np.arange( len(funcDict) )
print( ifunc_l )


# ------- Debug commands -------
#centering = 'primal'
#derivOrder = 1
#changeCentering( centering, derivOrder )

#for icase in case_l:
#    for idim in dim_l:
#        for iqty in iqty_l:
#            print( "field : %s, direction : %s, centering : %s" % 
#                  (Qty_l[iqty][1], Direction_l[idim][1], 
#                   qtyCentering(Qty_l[iqty][1], Direction_l[idim][1]) ) )  
#                   
#            print( "Nbr of cells = %d" % nbrCells(Direction_l[idim][1], icase) )
#                   
#            print( "Nbr of ghost cells = %d on each side" % 
#            nbrGhosts(interpOrder_l[0], \
#                      qtyCentering( Qty_l[iqty][1], Direction_l[idim][1]) ) )
#            
#            print( "Alloc size = %d" % allocSize(interpOrder_l[0], \
#                   qtyCentering( Qty_l[iqty][1], Direction_l[idim][1] ), \
#                   Direction_l[idim][1], icase) )
#            
#            print( physicalStartIndex(interpOrder_l[0], \
#                       qtyCentering( Qty_l[iqty][1], Direction_l[idim][1] ) ),
#                   physicalEndIndex(interpOrder_l[0], \
#                       qtyCentering( Qty_l[iqty][1], Direction_l[idim][1] ), \
#                       Direction_l[idim][1], icase), 
#                   ghostEndIndex(interpOrder_l[0], \
#                       qtyCentering( Qty_l[iqty][1], Direction_l[idim][1] ), \
#                       Direction_l[idim][1], icase)  )

# -------- Let us define a function on Bx with Yee lattice --------
icase = 0

iqty = 0
iord = 0
idim = 0


f = open("deriv1D_summary.txt", "w")


for ifunc in ifunc_l:
    #for iqty in iqty_l:   
    f.write(("%03d %d %s %03d %5.4f ") % 
       (interpOrder_l[iord],
        dim_l[idim]+1,
        Qty_l[iqty][0],
        nbrCells(Direction_l[idim][1], icase),
        spatialStep(Direction_l[idim][1], icase) ) )                   
       
    iStart = physicalStartPrimal(interpOrder_l[iord])
    iEnd   = physicalEndPrimal  (interpOrder_l[iord], Direction_l[idim][1], icase)                
    iEnd = iEnd - isDual( qtyCentering(Qty_l[iqty][1], Direction_l[idim][1]) )         
    
    f.write(("%d %d ") % (iStart, iEnd))
       
    f.write(("%6.2f %6.2f %6.2f ") % (origin[0], origin[1], origin[2]))
    
    f.write("%s \n" % function_l[ifunc])

f.close()



for ifunc in ifunc_l:
    f = open( ("deriv1D_%s_%s.txt") % (Qty_l[iqty][1], function_l[ifunc]), "w")
    
    iStart = physicalStartPrimal(interpOrder_l[iord])
    iEnd   = physicalEndPrimal  (interpOrder_l[iord], Direction_l[idim][1], icase)            
    
    iEnd = iEnd - isDual( qtyCentering(Qty_l[iqty][1], Direction_l[idim][1]) )                      
        
    # qtyCentering(Qty_l[iqty][1], Direction_l[idim][1])    
        
    for iprimal in range(iStart, iEnd+1):
        x = fieldCoords(iprimal, iStart, Qty_l[iqty][1], Direction_l[idim], \
                        spatialStep(Direction_l[idim][1], icase), origin, 0)
        x_der = fieldCoords(iprimal, iStart, Qty_l[iqty][1], Direction_l[idim], \
                            spatialStep(Direction_l[idim][1], icase), origin, 1)                    
                        
        f.write(("%10.4f %14.10f   ") % (x    , funcDict[function_l[ifunc]](x)         ) )
        f.write(("%10.4f %14.10f \n") % (x_der, der_funcDict[function_l[ifunc]](x_der) ) )
    																								
    f.close()



# ------------------------------

#for iord in iord_l:
#    for icase in case_l:
#        for idim in dim_l:
#            for iqty in iqty_l:
#                f = open(("deriv1D_ord%d_dim%d_%s_case%d.txt") % 
#                (interpOrder_l[iord], dim_l[idim]+1, Qty_l[iqty][1], icase), "w")
#                    
#                iStart = physicalStartPrimal(interpOrder_l[iord])
#                iEnd   = physicalEndPrimal  (interpOrder_l[iord], Direction_l[idim][1], icase)            
#                
#                iEnd = iEnd - isDual(Qty_l[iqty][1], Direction_l[idim][1])                      
#                    
#                for iprimal in range(iStart, iEnd+1):
#                    x = fieldCoords(iprimal, iStart, Qty_l[iqty][1], Direction_l[idim], \
#                                    spatialStep(Direction_l[idim][1], icase), origin)
#                    f.write(("%8.2f ") % (x))
#    																								
#                f.close()
                
                
                
                
                