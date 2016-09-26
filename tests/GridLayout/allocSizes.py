# -*- coding: utf-8 -*-
"""
Created on Mon Sep 26 14:15:49 2016

@author: mdrouin
"""

#!/usr/bin/env python
#!coding: utf-8

# In[1]:

import numpy as np
import matplotlib.pyplot as plt
import scipy.integrate as scint
import mpl_toolkits.mplot3d as mplt3d
import math
#from math import factorial
#%matplotlib inline


nbPadding = [10,10,10]
QtyDict = {'Bx':0, 'By':1, 'Bz':2, 'Ex':3, 'Ey':4, 'Ez':5, 'rho':6, 'V':7, 'P':8}


# ---------------------- X DIRECTION -----------------------------------------

# ---- usefull methods ------
def nbrGhostsX(interpOrder, qty):
    if qty[0] in (QtyDict['Ex'], QtyDict['By'], QtyDict['Bz']) :# dual qties
        return math.floor( (interpOrder +1)/2 )
    else: # primal
        return math.floor( interpOrder/2 )

def nbrGhostsDerivedX(interpOrder, qty):
    # once derived, they become primal qties
    if qty[0] in (QtyDict['Ex'], QtyDict['By'], QtyDict['Bz']) :
        return math.floor( interpOrder/2 )
    else: # once derived, they become dual qties
        return math.floor( (interpOrder +1)/2 )

        
def isDualX(qty):
    if qty[0] in (QtyDict['Ex'], QtyDict['By'], QtyDict['Bz']) :# dual qties
        return 1
    else:
        return 0
    
# ---- Start / End index methods ------
def ghostStartIndexX():
    return 0

def physicalStartIndexX(interpOrder, nbrCells, qty):
    return ghostStartIndexX() + nbrGhostsX(interpOrder, qty)

def physicalEndIndexX(interpOrder, nbrCells, qty):
    return physicalStartIndexX(interpOrder, nbrCells, qty) + nbrCells - isDualX(qty)

def ghostEndIndexX(interpOrder, nbrCells, qty):
    return physicalEndIndexX(interpOrder, nbrCells, qty) + nbrGhostsX(interpOrder, qty)

#    uint32 nx =  2*nbrPaddingCells( Direction::X ) + nbrPhysicalCells( Direction::X ) + 1
#               + 2*nbrGhosts( qtyCenterings[data.idirX] ) ;
def allocSizeX(interpOrder, nbrCells, qty):
    return nbrCells + 1 + 2*nbrGhostsX(interpOrder, qty)

def allocSizeDerivedX(interpOrder, nbrCells, qty):
    return nbrCells + 1 + 2*nbrGhostsDerivedX(interpOrder, qty)


# ---------------------- Y DIRECTION -----------------------------------------
    
# ---- usefull methods ------
def nbrGhostsY(interpOrder, qty):
    if qty[0] in (QtyDict['Ey'], QtyDict['Bx'], QtyDict['Bz']) :# dual qties
        return math.floor( (interpOrder +1)/2 )
    else: # primal
        return math.floor( interpOrder/2 )
        
def nbrGhostsDerivedY(interpOrder, qty):
    # once derived, they become primal qties
    if qty[0] in (QtyDict['Ey'], QtyDict['Bx'], QtyDict['Bz']) :
        return math.floor( interpOrder/2 )
    else: # once derived, they become dual qties
        return math.floor( (interpOrder +1)/2 )        
        
def isDualY(qty):
    if qty[0] in (QtyDict['Ey'], QtyDict['Bx'], QtyDict['Bz']) :# dual qties
        return 1
    else:
        return 0
    
# ---- Start / End index methods ------
def ghostStartIndexY():
    return 0

def physicalStartIndexY(interpOrder, nbrCells, qty):
    return ghostStartIndexY() + nbrGhostsY(interpOrder, qty)

def physicalEndIndexY(interpOrder, nbrCells, qty):
    return physicalStartIndexY(interpOrder, nbrCells, qty) + nbrCells - isDualY(qty)

def ghostEndIndexY(interpOrder, nbrCells, qty):
    return physicalEndIndexY(interpOrder, nbrCells, qty) + nbrGhostsY(interpOrder, qty)


def allocSizeY(interpOrder, nbrCells, qty):
    return nbrCells + 1 + 2*nbrGhostsY(interpOrder, qty)

def allocSizeDerivedY(interpOrder, nbrCells, qty):
    return nbrCells + 1 + 2*nbrGhostsDerivedY(interpOrder, qty)

# ---------------------- Z DIRECTION -----------------------------------------

# ---- usefull methods ------
def nbrGhostsZ(interpOrder, qty):
    if qty[0] in (QtyDict['Ez'], QtyDict['Bx'], QtyDict['By']) :# dual qties
        return math.floor( (interpOrder +1)/2 )
    else: # primal
        return math.floor( interpOrder/2 )

        
def nbrGhostsDerivedZ(interpOrder, qty):
    # once derived, they become primal qties
    if qty[0] in (QtyDict['Ez'], QtyDict['Bx'], QtyDict['By']) :
        return math.floor( interpOrder/2 )
    else: # once derived, they become dual qties
        return math.floor( (interpOrder +1)/2 )                   
        
def isDualZ(qty):
    if qty[0] in (QtyDict['Ez'], QtyDict['Bx'], QtyDict['By']) :# dual qties
        return 1
    else:
        return 0
    
# ---- Start / End index methods ------
def ghostStartIndexZ():
    return 0

def physicalStartIndexZ(interpOrder, nbrCells, qty):
    return ghostStartIndexZ() + nbrGhostsZ(interpOrder, qty)

def physicalEndIndexZ(interpOrder, nbrCells, qty):
    return physicalStartIndexZ(interpOrder, nbrCells, qty) + nbrCells - isDualZ(qty)

def ghostEndIndexZ(interpOrder, nbrCells, qty):
    return physicalEndIndexZ(interpOrder, nbrCells, qty) + nbrGhostsZ(interpOrder, qty)


def allocSizeZ(interpOrder, nbrCells, qty):
    return nbrCells + 1 + 2*nbrGhostsZ(interpOrder, qty)

def allocSizeDerivedZ(interpOrder, nbrCells, qty):
    return nbrCells + 1 + 2*nbrGhostsDerivedZ(interpOrder, qty)

# ---------------------- MAIN CODE -----------------------------------------

paramList = ['interpOrder', 'nbDims', 'Quantity', 'nbrCellX', 'nbrCellY', 'nbrCellZ', 'dx', 'dy', 'dz'];
len( paramList )
print( paramList[:] )
print( paramList[2] )

interpOrder_l=[1, 2, 3, 4]
nbDims_l=[1, 2, 3] 

#Qty_l=['Bx', 'By', 'Bz', 'Ex', 'Ey', 'Ez', 'rho', 'V', 'P']
Qty_l=[0, 1, 2, 3, 4, 5, 6, 7, 8]
Qty_l=[(0,'Bx'), (1,'By'), (2,'Bz'), (3,'Ex'), (4,'Ey'), (5,'Ez'), (6,'rho'), (7,'V'), (8,'P')]

nbrCellX_l=[40, 40, 40]
nbrCellY_l=[ 0, 12, 12]
nbrCellZ_l=[ 0,  0, 12]

dx_l=[0.1, 0.1, 0.1] # 1D, 2D, 3D cases 
dy_l=[0. , 0.1, 0.1]
dz_l=[0. , 0. , 0.1]

maxNbrDim = 3

nbrTestCases = len(interpOrder_l)*len(nbDims_l)*len(Qty_l)
print( nbrTestCases )

caseParamsTable = np.zeros((nbrTestCases, len(paramList) + 2*maxNbrDim))

col_l = np.arange( len(paramList) )
print( col_l )


iord_l=np.arange( len(interpOrder_l) ) 
print( iord_l )
idim_l=np.arange( len(nbDims_l) ) 
print( idim_l )
iqty_l=np.arange( len(Qty_l) ) 
print( iqty_l )


f = open("allocSizes.txt", "w")

for iord in iord_l:
    for idim in idim_l:
        for iqty in iqty_l:
        
        	f.write(("%03d %03d %s %03d %03d %03d %4.1f %4.1f %4.1f"+" %d"*2*maxNbrDim +"\n") % 
               (interpOrder_l[iord],
                nbDims_l[idim],
                Qty_l[iqty][0],
                nbrCellX_l[idim],
                nbrCellY_l[idim],
                nbrCellZ_l[idim],
                dx_l[idim],
                dy_l[idim],
                dz_l[idim],
                allocSizeX(interpOrder_l[iord], nbrCellX_l[idim], Qty_l[iqty]),
                allocSizeY(interpOrder_l[iord], nbrCellY_l[idim], Qty_l[iqty]),
                allocSizeZ(interpOrder_l[iord], nbrCellZ_l[idim], Qty_l[iqty]),
                allocSizeDerivedX(interpOrder_l[iord], nbrCellX_l[idim], Qty_l[iqty]),
                allocSizeDerivedY(interpOrder_l[iord], nbrCellY_l[idim], Qty_l[iqty]),
                allocSizeDerivedZ(interpOrder_l[iord], nbrCellZ_l[idim], Qty_l[iqty])) )
																								 

f.close()


#np.savetxt("gridlayouttest.txt", caseParamsTable, delimiter=" ", fmt="%4.1f")



