# -*- coding: utf-8 -*-
#"""
#Created on Fri Oct  7 16:54:36 2016
#
#@author: mdrouin
#"""

# In[1]:

import numpy as np

import sys
sys.path.insert(0, '../')

import commons 



# ---------------------- INITIALIZATION  -----------------------------------

paramList = ['interpOrder', 'nbDims', 'Quantity', 'nbrCellX', 'nbrCellY', 'nbrCellZ', 'dx', 'dy', 'dz'];
len( paramList )
print( paramList[:] )
print( paramList[2] )

interpOrder_l=[1, 2, 3, 4]

Direction_l = commons.Direction_l
Qty_l = commons.Qty_l
function_l = commons.function_l

funcDict = commons.funcDict
der_funcDict = commons.der_funcDict


origin = [0., 0., 0.]

# ---------------------- MAIN CODE -----------------------------------------

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
        commons.nbrCells(Direction_l[idim][1], icase),
        commons.spatialStep(Direction_l[idim][1], icase) ) )                   
       
    iStart = commons.physicalStartPrimal(interpOrder_l[iord])
    iEnd   = commons.physicalEndPrimal  (interpOrder_l[iord], Direction_l[idim][1], icase)                
    iEnd = iEnd - commons.isDual( commons.qtyCentering(Qty_l[iqty][1], Direction_l[idim][1]) )         
    
    f.write(("%d %d ") % (iStart, iEnd))
       
    f.write(("%6.2f %6.2f %6.2f ") % (origin[0], origin[1], origin[2]))
    
    f.write("%s \n" % function_l[ifunc])

f.close()



for ifunc in ifunc_l:
    f = open( ("deriv1D_%s_%s.txt") % (Qty_l[iqty][1], function_l[ifunc]), "w")
    
    iStart = commons.physicalStartPrimal(interpOrder_l[iord])
    iEnd   = commons.physicalEndPrimal  (interpOrder_l[iord], Direction_l[idim][1], icase)            
    
    iEnd = iEnd - commons.isDual( commons.qtyCentering(Qty_l[iqty][1], Direction_l[idim][1]) )                      
        
    # qtyCentering(Qty_l[iqty][1], Direction_l[idim][1])    
        
    for iprimal in range(iStart, iEnd+1):
        x = commons.fieldCoords(iprimal, iStart, Qty_l[iqty][1], Direction_l[idim], \
                               commons.spatialStep(Direction_l[idim][1], icase), origin, 0)
        x_der = commons.fieldCoords(iprimal, iStart, Qty_l[iqty][1], Direction_l[idim], \
                            commons.spatialStep(Direction_l[idim][1], icase), origin, 1)                    
                        
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
                
                
                
                
                