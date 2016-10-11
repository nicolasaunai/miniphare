import numpy as np
import math






nbPadding = [10,10,10]

Direction_l = [(0,'X'), (1,'Y'), (2,'Z')]

Qty_l=[(0,'Bx'), (1,'By'), (2,'Bz'), (3,'Ex'), (4,'Ey'), (5,'Ez'), (6,'rho'), (7,'V'), (8,'P')]

QtyDict = {'Bx':0, 'By':1, 'Bz':2, 'Ex':3, 'Ey':4, 'Ez':5, 'rho':6, 'V':7, 'P':8}

nbrCellX_l=[ 40, 40, 40]
nbrCellY_l=[ 0 , 12, 12]
nbrCellZ_l=[ 0 ,  0, 12]

dx_l=[0.1 , 0.1, 0.1] # 1D, 2D, 3D cases 
dy_l=[0.  , 0.1, 0.1]
dz_l=[0.  , 0. , 0.1]


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





