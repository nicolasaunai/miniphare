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
QtyDict = {'Bx':0, 'By':1, 'Bz':2, 'Ex':3, 'Ey':4, 'Ez':5, 'rho':6, 'V':7, 
            'P':8}
            


# ---------------------- X DIRECTION -----------------------------------------


def ghostStartIndexX(interpOrder, nbrCells, qty):
    return 0





def ghostEndIndexX(interpOrder, nbrCells, qty):

    if qty in (QtyDict['Ex'], QtyDict['By'], QtyDict['Bz']) :# dual qties
        return nbrCells + math.floor((interpOrder+1)/2) + math.floor((interpOrder-1)/2)

    else: # primal
        return nbrCells + 2 * math.floor(interpOrder/2)





def physicalStartIndexX(interpOrder,nbrCells, qty):

    if qty in (QtyDict['Ex'], QtyDict['By'], QtyDict['Bz']) :# dual qties
        return ghostStartIndexX(interpOrder, nbrCells, qty) + math.floor((interpOrder +1)/2)

    else: # primal
        return ghostStartIndexX(interpOrder, nbrCells, qty) + math.floor((interpOrder)/2)




def physicalEndIndexX(interpOrder, nbrCells, qty):
    if qty in (QtyDict['Ex'], QtyDict['By'], QtyDict['Bz']) :# dual qties
        return ghostEndIndexX(interpOrder, nbrCells, qty) - math.floor((interpOrder +1)/2)

    else: # primal
	        return ghostEndIndexX(interpOrder, nbrCells, qty) - math.floor((interpOrder)/2)





# ---------------------- Y DIRECTION -----------------------------------------

def ghostStartIndexY(interpOrder, nbrCells, qty):
    return 0





def ghostEndIndexY(interpOrder, nbrCells, qty):

    if qty in (QtyDict['Ey'], QtyDict['Bx'], QtyDict['Bz']) :# dual qties
        return nbrCells + math.floor((interpOrder+1)/2) + math.floor((interpOrder-1)/2)

    else: # primal
        return nbrCells + 2 * math.floor(interpOrder/2)





def physicalStartIndexY(interpOrder,nbrCells, qty):

    if qty in (QtyDict['Ey'], QtyDict['Bx'], QtyDict['Bz']) :# dual qties
        return ghostStartIndexY(interpOrder, nbrCells, qty) + math.floor((interpOrder +1)/2)

    else: # primal
        return ghostStartIndexY(interpOrder, nbrCells, qty) + math.floor((interpOrder)/2)




def physicalEndIndexY(interpOrder, nbrCells, qty):
    if qty in (QtyDict['Ey'], QtyDict['Bx'], QtyDict['Bz']) :# dual qties
        return ghostEndIndexY(interpOrder, nbrCells, qty) - math.floor((interpOrder +1)/2)

    else: # primal
        return ghostEndIndexY(interpOrder, nbrCells, qty) - math.floor((interpOrder)/2)



# ---------------------- Z DIRECTION -----------------------------------------



def ghostStartIndexZ(interpOrder, nbrCells, qty):
    return 0





def ghostEndIndexZ(interpOrder, nbrCells, qty):

    if qty in (QtyDict['Ez'], QtyDict['Bx'], QtyDict['By']) :# dual qties
        return nbrCells + math.floor((interpOrder+1)/2) + math.floor((interpOrder-1)/2)

    else: # primal
        return nbrCells + 2 * math.floor(interpOrder/2)





def physicalStartIndexZ(interpOrder,nbrCells, qty):

    if qty in (QtyDict['Ez'], QtyDict['Bx'], QtyDict['By']) :# dual qties
        return ghostStartIndexZ(interpOrder, nbrCells, qty) + math.floor((interpOrder +1)/2)

    else: # primal
        return ghostStartIndexZ(interpOrder, nbrCells, qty) + math.floor((interpOrder)/2)




def physicalEndIndexZ(interpOrder, nbrCells, qty):
    if qty in (QtyDict['Ez'], QtyDict['Bx'], QtyDict['By']) :# dual qties
        return ghostEndIndexZ(interpOrder, nbrCells, qty) - math.floor((interpOrder +1)/2)

    else: # primal
        return ghostEndIndexZ(interpOrder, nbrCells, qty) - math.floor((interpOrder)/2)













split_tab = np.zeros((100, 10))


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






# In[45]:

iord_l=np.arange( len(interpOrder_l) ) 
print( iord_l )
idim_l=np.arange( len(nbDims_l) ) 
print( idim_l )
iqty_l=np.arange( len(Qty_l) ) 
print( iqty_l )


# In[ ]:


f = open("result.txt", "w")

 



#  data_[iz + shape_[2]*iy + shape_[1]*shape_[2]*ix]; 
#icase = 0
for iord in iord_l:
    for idim in idim_l:
        for iqty in iqty_l:
        
        
        	f.write("%03d %03d %s %03d %03d %03d %4.1f %4.1f %4.1f %d %d %d %d %d %d %d %d %d %d %d %d\n" % (interpOrder_l[iord],
																								nbDims_l[idim],
																								Qty_l[iqty][1],
																								nbrCellX_l[idim],
																								nbrCellY_l[idim],
																								nbrCellZ_l[idim],
																								dx_l[idim],
																								dy_l[idim],
																								dz_l[idim],
																								physicalStartIndexX(interpOrder_l[iord],nbrCellX_l[idim], Qty_l[iqty]),
																								physicalEndIndexX  (interpOrder_l[iord],nbrCellX_l[idim], Qty_l[iqty]),
																								ghostStartIndexX   (interpOrder_l[iord],nbrCellX_l[idim], Qty_l[iqty]),
																								ghostEndIndexX     (interpOrder_l[iord],nbrCellX_l[idim], Qty_l[iqty]),
																								physicalStartIndexY(interpOrder_l[iord],nbrCellY_l[idim], Qty_l[iqty]),
																								physicalEndIndexY  (interpOrder_l[iord],nbrCellY_l[idim], Qty_l[iqty]),
																								ghostStartIndexY   (interpOrder_l[iord],nbrCellY_l[idim], Qty_l[iqty]),
																								ghostEndIndexY     (interpOrder_l[iord],nbrCellY_l[idim], Qty_l[iqty]),
																								physicalStartIndexZ(interpOrder_l[iord],nbrCellZ_l[idim], Qty_l[iqty]),
																								physicalEndIndexZ  (interpOrder_l[iord],nbrCellZ_l[idim], Qty_l[iqty]),
																								ghostStartIndexZ   (interpOrder_l[iord],nbrCellZ_l[idim], Qty_l[iqty]),
																								ghostEndIndexZ     (interpOrder_l[iord],nbrCellZ_l[idim], Qty_l[iqty])) )
																								 
																								 
        
        
        
            
# In[47]:

#print( caseParamsTable )

f.close()


#np.savetxt("gridlayouttest.txt", caseParamsTable, delimiter=" ", fmt="%4.1f")


# In[ ]:




# In[ ]:




# In[ ]:




# In[ ]:




# In[ ]:




# In[ ]:




# In[ ]:




# In[ ]:




# In[ ]:




# In[ ]:



