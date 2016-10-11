#!/usr/bin/env python
#!coding: utf-8

# In[1]:

import numpy as np
#import math

import sys
sys.path.insert(0, '../')

import gridlayout_commons as commons

      
      
# --------- getters for global tables  ----------------------
def nbrCellsM(direction, icas):
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


# ---- Get coordinate methods -------------------------
# ds stands for dx or dy or dz
#
# This method returns a point
#
def fieldCoords(iprimal, iStart, qty, direction, ds, origin):
    halfCell = 0.    
        
    if commons.qtyCentering(qty, direction[1]) == 'dual':
        halfCell = 0.5 
        
    x = ( (iprimal - iStart) + halfCell )*ds + origin[direction[0]]

    return x


# ---------------------- MAIN CODE -----------------------------------------

if __name__ == "__main__":
    
    Direction_l = commons.Direction_l
    Qty_l = commons.Qty_l    
    
    interpOrder_l=[1, 2, 3, 4]
    case_l=[0] 
    #dim_l =[0, 1, 2] 
    dim_l =[0] 
    
    nbrCellX_l=[40, 40, 40]
    nbrCellY_l=[ 0, 12, 12]
    nbrCellZ_l=[ 0,  0, 12]
    
    dx_l=[0.1, 0.1, 0.1] # 1D, 2D, 3D cases 
    dy_l=[0. , 0.1, 0.1]
    dz_l=[0. , 0. , 0.1]
    
    origin = [0., 0., 0.]
    
    nbrTestCases = len(interpOrder_l)*len(dim_l)*len(Qty_l)*len(case_l)
    print( nbrTestCases )
    
    iord_l=np.arange( len(interpOrder_l) ) 
    print( iord_l )
    
    iqty_l=np.arange( len(Qty_l) ) 
    print( iqty_l )
    
    # ------- Debug commands -------
    for icase in case_l:
        for idim in dim_l:
            for iqty in iqty_l:
                centering = commons.qtyCentering(Qty_l[iqty][1], Direction_l[idim][1])
                nbrCells = nbrCellsM(Direction_l[idim][1], icase)
                
                print( "field : %s, direction : %s, centering : %s" % 
                      (Qty_l[iqty][1], Direction_l[idim][1], centering ) )  
                       
                print( "Nbr of cells = %d" % nbrCells )
                       
                print( "Nbr of ghost cells = %d on each side" % 
                commons.nbrGhosts(interpOrder_l[0], centering) )
                
                print( "Alloc size = %d" %
                commons.allocSize(interpOrder_l[0], centering, nbrCells) )
                
                print( commons.physicalStartIndex(interpOrder_l[0], centering),
                       commons.physicalEndIndex(interpOrder_l[0], centering, nbrCells), 
                       commons.ghostEndIndex(interpOrder_l[0], centering, nbrCells)  )
    # ------------------------------
    
    f = open("fieldCoords_summary.txt", "w")
    for iord in iord_l:
        for icase in case_l:
            for idim in dim_l:
                for iqty in iqty_l: 
                    centering = commons.qtyCentering(Qty_l[iqty][1], Direction_l[idim][1])
                    nbrCells = nbrCellsM(Direction_l[idim][1], icase) 
                    
                    stepSize = spatialStep(Direction_l[idim][1], icase)
                    
                    f.write(("%03d %d %s %03d %4.1f ") % 
                       (interpOrder_l[iord],
                        dim_l[idim]+1, Qty_l[iqty][0],
                        nbrCells, stepSize ) )                   
                       
                    iStart = commons.physicalStartPrimal(interpOrder_l[iord])
                    iEnd   = commons.physicalEndPrimal  (interpOrder_l[iord], nbrCells)            
                    
                    iEnd = iEnd - commons.isDual(centering)                
    
                    # f.write(("Primal start = %03d \n") % (iStart))
                    # f.write(("Primal end   = %03d \n") % (iEnd))
    
                    f.write(("%d %d ") % (iStart, iEnd))
                    f.write(("%6.2f %6.2f %6.2f\n") % (origin[0], origin[1], origin[2]))
    
    f.close()
    
    
    for iord in iord_l:
        for icase in case_l:
            for idim in dim_l:
                for iqty in iqty_l:
                    centering = commons.qtyCentering(Qty_l[iqty][1], Direction_l[idim][1])
                    nbrCells = nbrCellsM(Direction_l[idim][1], icase) 
                    
                    stepSize = spatialStep(Direction_l[idim][1], icase)                    
                    
                    f = open(("fieldCoords_ord%d_dim%d_%s_case%d.txt") % 
                    (interpOrder_l[iord], dim_l[idim]+1, Qty_l[iqty][1], icase), "w")
                        
                    iStart = commons.physicalStartPrimal(interpOrder_l[iord])
                    iEnd   = commons.physicalEndPrimal  (interpOrder_l[iord], nbrCells)            
                    
                    iEnd = iEnd - commons.isDual(centering)                      
                        
                    for iprimal in range(iStart, iEnd+1):
                        x = fieldCoords(iprimal, iStart, Qty_l[iqty][1], Direction_l[idim], \
                                        stepSize, origin)
                        f.write(("%8.2f ") % (x))
        																								
                    f.close()


#np.savetxt("gridlayouttest.txt", caseParamsTable, delimiter=" ", fmt="%4.1f")



