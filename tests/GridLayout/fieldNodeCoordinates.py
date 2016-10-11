#!/usr/bin/env python
#!coding: utf-8

# In[1]:

import numpy as np
import math


nbPadding = [10,10,10]

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


nbrCellX_l=[40, 40, 40]
nbrCellY_l=[ 0, 12, 12]
nbrCellZ_l=[ 0,  0, 12]

dx_l=[0.1, 0.1, 0.1] # 1D, 2D, 3D cases 
dy_l=[0. , 0.1, 0.1]
dz_l=[0. , 0. , 0.1]

origin = [0., 0., 0.]
           

# ---------------------- ANY DIRECTION -----------------------------------------
def qtyCentering(qty, direct):
    if direct == 'X':
        return CenterX[qty]
    elif direct == 'Y':
        return CenterY[qty]
    elif direct == 'Z':
        return CenterZ[qty]

def nbrGhosts(interpOrder, qty, direct):
    if qtyCentering(qty, direct) == 'primal':
        return math.floor( interpOrder/2 )
    else:
        return math.floor( (interpOrder +1)/2 )

def nbrGhostsPrimal(interpOrder):
    return math.floor( interpOrder/2 )

def isDual(qty, direct):
    if qtyCentering(qty, direct) == 'dual':
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

# ---- Start / End index methods ------
def ghostStartIndex():
    return 0

def physicalStartIndex(interpOrder, qty, direction):
    index = ghostStartIndex() + nbrGhosts(interpOrder, qty, direction) 
    return index

def physicalEndIndex(interpOrder, qty, direction, caseNbr):
    index = physicalStartIndex(interpOrder, qty, direction) \
    + nbrCells(direction, caseNbr) - isDual(qty, direction)    
    return index

def ghostEndIndex(interpOrder, qty, direction, caseNbr):
    index = physicalEndIndex(interpOrder, qty, direction, caseNbr) \
    + nbrGhosts(interpOrder, qty, direction)       
    return index
    
def physicalStartPrimal(interpOrder):
    index = ghostStartIndex() + nbrGhostsPrimal(interpOrder)     
    return index

def physicalEndPrimal(interpOrder, direction, caseNbr):
    index = physicalStartPrimal(interpOrder) + nbrCells(direction, caseNbr)    
    return index

# ---- Alloc methods -------------------------

def allocSize(interpOrder, qty, direction, caseNbr):
    size = nbrCells(direction, caseNbr) + 1 \
    + 2*nbrGhosts(interpOrder, qty, direction)    
    
    return size


# ---- Get coordinate methods -------------------------
# ds stands for dx or dy or dz
#
# This method returns a point
#
def fieldCoords(iprimal, iStart, qty, direction, ds, origin):
    halfCell = 0.    
        
    if qtyCentering(qty, direction[1]) == 'dual':
        halfCell = 0.5 
        
    x = ( (iprimal - iStart) + halfCell )*ds + origin[direction[0]]

    return x


# ---------------------- MAIN CODE -----------------------------------------

if __name__ == "__main__":
    paramList = ['interpOrder', 'nbDims', 'Quantity', 'nbrCellX', 'nbrCellY', 'nbrCellZ', 'dx', 'dy', 'dz'];
    len( paramList )
    print( paramList[:] )
    print( paramList[2] )
    
    interpOrder_l=[1, 2, 3, 4]
    case_l=[0] 
    #dim_l =[0, 1, 2] 
    dim_l =[0] 
    
    
    maxNbrDim = 3
    
    nbrTestCases = len(interpOrder_l)*len(dim_l)*len(Qty_l)*len(case_l)
    print( nbrTestCases )
    
    caseParamsTable = np.zeros((nbrTestCases, len(paramList) + 2*maxNbrDim))
    
    col_l = np.arange( len(paramList) )
    print( col_l )
    
    
    iord_l=np.arange( len(interpOrder_l) ) 
    print( iord_l )
    
    iqty_l=np.arange( len(Qty_l) ) 
    print( iqty_l )
    
    # ------- Debug commands -------
    for icase in case_l:
        for idim in dim_l:
            for iqty in iqty_l:
                print( "field : %s, direction : %s, centering : %s" % 
                      (Qty_l[iqty][1], Direction_l[idim][1], 
                       qtyCentering(Qty_l[iqty][1], Direction_l[idim][1]) ) )  
                       
                print( "Nbr of cells = %d" % nbrCells(Direction_l[idim][1], icase) )
                       
                print( "Nbr of ghost cells = %d on each side" % 
                nbrGhosts(interpOrder_l[0], Qty_l[iqty][1], Direction_l[idim][1]) )
                
                print( "Alloc size = %d" %
                allocSize(interpOrder_l[0], Qty_l[iqty][1], Direction_l[idim][1], icase) )
                
                print( physicalStartIndex(interpOrder_l[0], Qty_l[iqty][1], Direction_l[idim][1]),
                       physicalEndIndex(interpOrder_l[0], Qty_l[iqty][1], Direction_l[idim][1], icase), 
                       ghostEndIndex(interpOrder_l[0], Qty_l[iqty][1], Direction_l[idim][1], icase)  )
    # ------------------------------
    
    f = open("fieldCoords_summary.txt", "w")
    for iord in iord_l:
        for icase in case_l:
            for idim in dim_l:
                for iqty in iqty_l:   
                    f.write(("%03d %d %s %03d %4.1f ") % 
                       (interpOrder_l[iord],
                        dim_l[idim]+1,
                        Qty_l[iqty][0],
                        nbrCells(Direction_l[idim][1], icase),
                        spatialStep(Direction_l[idim][1], icase) ) )                   
                       
                    iStart = physicalStartPrimal(interpOrder_l[iord])
                    iEnd   = physicalEndPrimal  (interpOrder_l[iord], Direction_l[idim][1], icase)            
                    
                    iEnd = iEnd - isDual(Qty_l[iqty][1], Direction_l[idim][1])                
    
                    # f.write(("Primal start = %03d \n") % (iStart))
                    # f.write(("Primal end   = %03d \n") % (iEnd))
    
                    f.write(("%d %d ") % (iStart, iEnd))
                    f.write(("%6.2f %6.2f %6.2f\n") % (origin[0], origin[1], origin[2]))
    
    f.close()
    
    
    for iord in iord_l:
        for icase in case_l:
            for idim in dim_l:
                for iqty in iqty_l:
                    f = open(("fieldCoords_ord%d_dim%d_%s_case%d.txt") % 
                    (interpOrder_l[iord], dim_l[idim]+1, Qty_l[iqty][1], icase), "w")
                        
                    iStart = physicalStartPrimal(interpOrder_l[iord])
                    iEnd   = physicalEndPrimal  (interpOrder_l[iord], Direction_l[idim][1], icase)            
                    
                    iEnd = iEnd - isDual(Qty_l[iqty][1], Direction_l[idim][1])                      
                        
                    for iprimal in range(iStart, iEnd+1):
                        x = fieldCoords(iprimal, iStart, Qty_l[iqty][1], Direction_l[idim], \
                                        spatialStep(Direction_l[idim][1], icase), origin)
                        f.write(("%8.2f ") % (x))
        																								
                    f.close()


#np.savetxt("gridlayouttest.txt", caseParamsTable, delimiter=" ", fmt="%4.1f")



