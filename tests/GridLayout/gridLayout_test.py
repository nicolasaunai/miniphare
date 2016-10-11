#!/usr/bin/env python
#!coding: utf-8

# In[1]:

import numpy as np
import math


nbPadding = [10,10,10]
QtyDict = {'Bx':0, 'By':1, 'Bz':2, 'Ex':3, 'Ey':4, 'Ez':5, 'rho':6, 'V':7, 'P':8}
            


# ---------------------- X DIRECTION -----------------------------------------

# ---- usefull methods ------
def nbrGhostsX(interpOrder, qty):
    if qty[0] in (QtyDict['Ex'], QtyDict['By'], QtyDict['Bz']) :# dual qties
        return math.floor( (interpOrder +1)/2 )
    else: # primal
        return math.floor( interpOrder/2 )
        
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


# ---------------------- Y DIRECTION -----------------------------------------
    
# ---- usefull methods ------
def nbrGhostsY(interpOrder, qty):
    if qty[0] in (QtyDict['Ey'], QtyDict['Bx'], QtyDict['Bz']) :# dual qties
        return math.floor( (interpOrder +1)/2 )
    else: # primal
        return math.floor( interpOrder/2 )
        
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


# ---------------------- Z DIRECTION -----------------------------------------

# ---- usefull methods ------
def nbrGhostsZ(interpOrder, qty):
    if qty[0] in (QtyDict['Ez'], QtyDict['Bx'], QtyDict['By']) :# dual qties
        return math.floor( (interpOrder +1)/2 )
    else: # primal
        return math.floor( interpOrder/2 )
        
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


# ---------------------- MAIN CODE -----------------------------------------
if __name__ == "__main__":
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
    
    dx_l=[0.1, 0.1, 0.1] # 1D, 2D and 3D cases 
    dy_l=[0. , 0.1, 0.1]
    dz_l=[0. , 0. , 0.1]
    
    nbrTestCases = len(interpOrder_l)*len(nbDims_l)*len(Qty_l)
    print( nbrTestCases )
    
    caseParamsTable = np.zeros((nbrTestCases, len(paramList) + 4*3))
    
    col_l = np.arange( len(paramList) )
    print( col_l )
    
    
    iord_l=np.arange( len(interpOrder_l) ) 
    print( iord_l )
    idim_l=np.arange( len(nbDims_l) ) 
    print( idim_l )
    iqty_l=np.arange( len(Qty_l) ) 
    print( iqty_l )
    
    
    f = open("result.txt", "w")
    
    for iord in iord_l:
        for idim in idim_l:
            for iqty in iqty_l:
            
            	f.write(("%03d %03d %s %03d %03d %03d %4.1f %4.1f %4.1f"+" %d"*12 +"\n") % 
                   (interpOrder_l[iord],
                    nbDims_l[idim],
                    Qty_l[iqty][0],
                    nbrCellX_l[idim],
                    nbrCellY_l[idim],
                    nbrCellZ_l[idim],
                    dx_l[idim],
                    dy_l[idim],
                    dz_l[idim],
                    physicalStartIndexX(interpOrder_l[iord],nbrCellX_l[idim], Qty_l[iqty]),
                    physicalEndIndexX  (interpOrder_l[iord],nbrCellX_l[idim], Qty_l[iqty]),
                    ghostStartIndexX   (),
                    ghostEndIndexX     (interpOrder_l[iord],nbrCellX_l[idim], Qty_l[iqty]),
                    physicalStartIndexY(interpOrder_l[iord],nbrCellY_l[idim], Qty_l[iqty]),
                    physicalEndIndexY  (interpOrder_l[iord],nbrCellY_l[idim], Qty_l[iqty]),
                    ghostStartIndexY   (),
                    ghostEndIndexY     (interpOrder_l[iord],nbrCellY_l[idim], Qty_l[iqty]),
                    physicalStartIndexZ(interpOrder_l[iord],nbrCellZ_l[idim], Qty_l[iqty]),
                    physicalEndIndexZ  (interpOrder_l[iord],nbrCellZ_l[idim], Qty_l[iqty]),
                    ghostStartIndexZ   (),
                    ghostEndIndexZ     (interpOrder_l[iord],nbrCellZ_l[idim], Qty_l[iqty])) )
    																								 
    
    f.close()


