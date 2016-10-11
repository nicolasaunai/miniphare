# -*- coding: utf-8 -*-
#"""
#Created on Fri Oct  7 16:54:36 2016
#
#@author: mdrouin
#"""

# In[1]:

import numpy as np
import math

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
                


if __name__ == "__main__":
    # ---------------------- INITIALIZATION  -----------------------------------    
    nbrCellX_l=[40, 40, 40]
    nbrCellY_l=[ 0, 12, 12]
    nbrCellZ_l=[ 0,  0, 12]
    
    dx_l=[0.1, 0.1, 0.1] # 1D, 2D, 3D cases 
    dy_l=[0. , 0.1, 0.1]
    dz_l=[0. , 0. , 0.1]
    
    origin = [0., 0., 0.]
    
    interpOrder_l=[1, 2, 3, 4]
    
    Direction_l = commons.Direction_l
    Qty_l = commons.Qty_l
    
    
    origin = [0., 0., 0.]
    
    # ---------------------- MAIN CODE -----------------------------------------
    
    # functions to be derivated
    case_l=[0] 
    
    #dim_l =[0, 1, 2] 
    dim_l =[0] 
    
    
    maxNbrDim = 3
    
    nbrTestCases = len(interpOrder_l)*len(dim_l)*len(Qty_l)*len(case_l)
    print( nbrTestCases )
    
    iord_l = np.arange( len(interpOrder_l) ) 
    print( iord_l )
    
    iqty_l = np.arange( len(Qty_l) ) 
    print( iqty_l )
    
    ifunc_l = np.arange( len(funcDict) )
    print( ifunc_l )
    
    
    # ------- Debug commands -------
    centering = 'primal'
    derivOrder = 1
    commons.changeCentering( centering, derivOrder )
    
    for icase in case_l:
        for idim in dim_l:
            nbrCells = nbrCellsM(Direction_l[idim][1], icase)
            
            for iqty in iqty_l:
                centering = commons.qtyCentering(Qty_l[iqty][1], Direction_l[idim][1])
                
                print( "field : %s, direction : %s, centering : %s" % 
                      (Qty_l[iqty][1], Direction_l[idim][1], centering ) )  
                       
                print( "Nbr of cells = %d" % nbrCells )
                       
                print( "Nbr of ghost cells = %d on each side" % 
                commons.nbrGhosts(interpOrder_l[0], centering ) )
                
                print( "Alloc size = %d" % \
                  commons.allocSize(interpOrder_l[0], centering, nbrCells) )
                
                print( commons.physicalStartIndex(interpOrder_l[0], centering ),
                       commons.physicalEndIndex(interpOrder_l[0], centering, nbrCells), 
                       commons.ghostEndIndex(interpOrder_l[0], centering, nbrCells)   )
    
    # -------- Let us define a function on Bx with Yee lattice --------
    icase = 0
    
    iqty = 0
    iord = 0
    idim = 0
    
    nbrCells = nbrCellsM(Direction_l[idim][1], icase)
    stepSize = spatialStep(Direction_l[idim][1], icase)    
    
    f = open("deriv1D_summary.txt", "w")   
    
    for ifunc in ifunc_l:
        f.write(("%03d %d %s %03d %5.4f ") % 
           (interpOrder_l[iord],
            dim_l[idim]+1, Qty_l[iqty][0],
            nbrCells, stepSize ) )                   
           
        iStart = commons.physicalStartPrimal(interpOrder_l[iord])
        iEnd   = commons.physicalEndPrimal  (interpOrder_l[iord], nbrCells)                
        iEnd = iEnd - commons.isDual( commons.qtyCentering(Qty_l[iqty][1], Direction_l[idim][1]) )         
        
        f.write(("%d %d ") % (iStart, iEnd))
           
        f.write(("%6.2f %6.2f %6.2f ") % (origin[0], origin[1], origin[2]))
        
        f.write("%s \n" % function_l[ifunc])
    
    f.close()
    
    
    
    for ifunc in ifunc_l:
        f = open( ("deriv1D_%s_%s.txt") % (Qty_l[iqty][1], function_l[ifunc]), "w")
        
        iStart = commons.physicalStartPrimal(interpOrder_l[iord])
        iEnd   = commons.physicalEndPrimal  (interpOrder_l[iord], nbrCells)            
        
        iEnd = iEnd - commons.isDual( commons.qtyCentering(Qty_l[iqty][1], Direction_l[idim][1]) )                      
            
        # qtyCentering(Qty_l[iqty][1], Direction_l[idim][1])    
            
        for iprimal in range(iStart, iEnd+1):
            x = commons.fieldCoords(iprimal, iStart, Qty_l[iqty][1], Direction_l[idim], \
                                    stepSize, origin, 0)
            x_der = commons.fieldCoords(iprimal, iStart, Qty_l[iqty][1], Direction_l[idim], \
                                        stepSize, origin, 1)                    
                            
            f.write(("%10.4f %14.10f   ") % (x    , funcDict[function_l[ifunc]](x)         ) )
            f.write(("%10.4f %14.10f \n") % (x_der, der_funcDict[function_l[ifunc]](x_der) ) )
        																								
        f.close()



# ------------------------------


                
                
                
                
                