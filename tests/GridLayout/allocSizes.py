#!/usr/bin/env python
#!coding: utf-8

# In[1]:

import numpy as np
#import math

import sys
sys.path.insert(0, '../')

import gridlayout_commons as commons




# ---------------------- MAIN CODE -----------------------------------------

if __name__ == "__main__":
    paramList = ['interpOrder', 'nbDims', 'Quantity', 'nbrCellX', 'nbrCellY', 'nbrCellZ', 'dx', 'dy', 'dz'];
    len( paramList )
    print( paramList[:] )
    print( paramList[2] )
    
    interpOrder_l=[1, 2, 3, 4]
    
    icase_l=[0, 1, 2] 
    
    Direction_l = commons.Direction_l
    Qty_l = commons.Qty_l
    
    
    nbrCellX_l=[40, 40, 40]
    nbrCellY_l=[ 0, 12, 12]
    nbrCellZ_l=[ 0,  0, 12]
    
    nbDims_l  = [1, 2, 3]
    
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

    iqty_l=np.arange( len(Qty_l) ) 
    print( iqty_l )
    
    
    f = open("allocSizes.txt", "w")
    
    for iord in iord_l:
        for icase in icase_l:
            for iqty in iqty_l:
                centeringX = commons.qtyCentering(Qty_l[iqty][1], 'X')
                centeringY = commons.qtyCentering(Qty_l[iqty][1], 'Y')
                centeringZ = commons.qtyCentering(Qty_l[iqty][1], 'Z')
            
                f.write(("%03d %03d %s %03d %03d %03d %4.1f %4.1f %4.1f"+" %d"*2*maxNbrDim +"\n") % 
                   (interpOrder_l[iord],
                    nbDims_l[icase],
                    Qty_l[iqty][0],
                    nbrCellX_l[icase],
                    nbrCellY_l[icase],
                    nbrCellZ_l[icase],
                    dx_l[icase],
                    dy_l[icase],
                    dz_l[icase],
                    commons.allocSize(interpOrder_l[iord], centeringX, nbrCellX_l[icase]),
                    commons.allocSize(interpOrder_l[iord], centeringY, nbrCellY_l[icase]),
                    commons.allocSize(interpOrder_l[iord], centeringZ, nbrCellZ_l[icase]),
                    commons.allocSizeDerived(interpOrder_l[iord], centeringX, nbrCellX_l[icase]),
                    commons.allocSizeDerived(interpOrder_l[iord], centeringY, nbrCellY_l[icase]),
                    commons.allocSizeDerived(interpOrder_l[iord], centeringZ, nbrCellZ_l[icase])) )
    
    f.close()

#np.savetxt("gridlayouttest.txt", caseParamsTable, delimiter=" ", fmt="%4.1f")																								 




