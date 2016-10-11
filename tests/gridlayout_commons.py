# -*- coding: utf-8 -*-
#"""
#Created on Mon Oct 10 11:24:42 2016
#
#@author: mdrouin
#"""

#import numpy as np
import math



Direction_l = [(0,'X'), (1,'Y'), (2,'Z')]

Qty_l=[(0,'Bx'), (1,'By'), (2,'Bz'), (3,'Ex'), (4,'Ey'), (5,'Ez'), (6,'rho'), (7,'V'), (8,'P')]

QtyDict = {'Bx':0, 'By':1, 'Bz':2, 'Ex':3, 'Ey':4, 'Ez':5, 'rho':6, 'V':7, 'P':8}


CenterX = {'Bx':'primal' , 'By':'dual'  , 'Bz':'dual'  , 
           'Ex':'dual'   , 'Ey':'primal', 'Ez':'primal', 
           'rho':'primal', 'V':'primal' , 'P':'primal' }

CenterY = {'Bx':'dual'   , 'By':'primal', 'Bz':'dual'  , 
           'Ex':'primal' , 'Ey':'dual'  , 'Ez':'primal', 
           'rho':'primal', 'V':'primal' , 'P':'primal' }
           
CenterZ = {'Bx':'dual'   , 'By':'dual'  , 'Bz':'primal', 
           'Ex':'primal' , 'Ey':'primal', 'Ez':'dual'  , 
           'rho':'primal', 'V':'primal' , 'P':'primal' }
           

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

def nbrGhostsDerived(interpOrder, centering):
    if centering == 'primal':
        # 1st order derivative becomes dual
        return math.floor( (interpOrder +1)/2 )
    else:
        # 1st order derivative becomes primal
        return math.floor( interpOrder/2 )


def nbrGhostsPrimal(interpOrder):
    return math.floor( interpOrder/2 )

def isDual(centering):
    if centering == 'dual':
        return 1
    else:
        return 0
        
# ---- Start / End index methods -----------
def ghostStartIndex():
    return 0

def physicalStartIndex(interpOrder, centering):
    index = ghostStartIndex() + nbrGhosts(interpOrder, centering) 
    return index

def physicalEndIndex(interpOrder, centering, nbrCells):
    index = physicalStartIndex(interpOrder, centering) \
    + nbrCells - isDual(centering)    
    return index

def ghostEndIndex(interpOrder, centering, nbrCells):
    index = physicalEndIndex(interpOrder, centering, nbrCells) \
    + nbrGhosts(interpOrder, centering)       
    return index
    
# ---- Start / End   primal methods ------
def physicalStartPrimal(interpOrder):
    index = ghostStartIndex() + nbrGhostsPrimal(interpOrder)     
    return index

def physicalEndPrimal(interpOrder, nbrCells):
    index = physicalStartPrimal(interpOrder) + nbrCells    
    return index

# ---- Alloc methods -------------------------

def allocSize(interpOrder, centering, nbrCells):
    size = nbrCells + 1 + 2*nbrGhosts(interpOrder, centering)    
    return size

def allocSizeDerived(interpOrder, centering, nbrCells):
    size = nbrCells + 1 + 2*nbrGhostsDerived(interpOrder, centering)
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





