#!/usr/bin/env python
#!coding: utf-8

# In[1]:

import numpy as np

import sys
sys.path.insert(0, '../')

import commons 

nbPadding = [10,10,10]

Direction_l = [(0,'X'), (1,'Y'), (2,'Z')]


nbrCellX_l=[40, 40, 40]
nbrCellY_l=[ 0, 12, 12]
nbrCellZ_l=[ 0,  0, 12]

dx_l=[0.1, 0.1, 0.1] # 1D, 2D, 3D cases 
dy_l=[0. , 0.1, 0.1]
dz_l=[0. , 0. , 0.1]

origin = [0., 0., 0.]
           

# ---- Get coordinate methods -------------------------
# ds stands for dx or dy or dz
#
# This method returns a point
#
def centeredCoords(iprimal, iStart, direction, ds, origin):
        
    # a cell-centered coordinate is always dual
    halfCell = 0.5 
        
    x = ( (iprimal - iStart) + halfCell )*ds + origin[direction[0]]

    return x


# ---------------------- MAIN CODE -----------------------------------------

paramList = ['interpOrder', 'nbDims', 'Quantity', 'nbrCellX', 'nbrCellY', 'nbrCellZ', 'dx', 'dy', 'dz'];
len( paramList )
print( paramList[:] )
print( paramList[2] )

interpOrder_l=[1, 2, 3, 4]
case_l=[0] 
#dim_l =[0, 1, 2] 
dim_l =[0] 


maxNbrDim = 3

nbrTestCases = len(interpOrder_l)*len(dim_l)*len(case_l)
print( nbrTestCases )

caseParamsTable = np.zeros((nbrTestCases, len(paramList) + 2*maxNbrDim))

col_l = np.arange( len(paramList) )
print( col_l )


iord_l=np.arange( len(interpOrder_l) ) 
print( iord_l )

# ------- Debug commands -------
for icase in case_l:
    for iord in iord_l:
            print( "Interpolation order = %d" % interpOrder_l[iord] )        
        
            idim = 0
            print( "Nbr of cells = %d" % commons.nbrCells(Direction_l[idim][1], icase) )
                   
            print( "Nbr of ghost cells on the primal mesh = %d on each side" % 
            commons.nbrGhostsPrimal(interpOrder_l[iord]) )
            
# ------------------------------

f = open("centeredCoords_summary.txt", "w")
for iord in iord_l:
    for icase in case_l:
        for idim in dim_l:
            f.write(("%d %d %06d %4.1f ") % 
               (interpOrder_l[iord],
                dim_l[idim]+1,
                commons.nbrCells(Direction_l[idim][1], icase),
                commons.spatialStep(Direction_l[idim][1], icase) ) )                   
               
            iStart = commons.physicalStartPrimal(interpOrder_l[iord])
            iEnd   = commons.physicalEndPrimal  (interpOrder_l[iord], Direction_l[idim][1], icase)            
            
            # a cell-centered coordinate is always dual
            iEnd = iEnd - 1

            f.write(("%d %d ") % (iStart, iEnd))
            f.write(("%6.2f %6.2f %6.2f\n") % (origin[0], origin[1], origin[2]))

f.close()


for iord in iord_l:
    for icase in case_l:
        for idim in dim_l:
            f = open(("centeredCoords_ord%d_dim%d_case%d.txt") % 
            (interpOrder_l[iord], dim_l[idim]+1, icase), "w")
                
            iStart = commons.physicalStartPrimal(interpOrder_l[iord])
            iEnd   = commons.physicalEndPrimal  (interpOrder_l[iord], Direction_l[idim][1], icase)            
            
            # a cell-centered coordinate is always dual
            iEnd = iEnd - 1                       
                
            for iprimal in range(iStart, iEnd+1):
                x = centeredCoords(iprimal, iStart, Direction_l[idim], \
                                   commons.spatialStep(Direction_l[idim][1], icase), \
                                   origin )
                f.write(("%8.2f ") % (x))
																								
            f.close()


#np.savetxt("gridlayouttest.txt", caseParamsTable, delimiter=" ", fmt="%4.1f")



