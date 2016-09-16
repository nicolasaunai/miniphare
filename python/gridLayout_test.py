
# coding: utf-8

# In[1]:

import numpy as np
import matplotlib.pyplot as plt
import scipy.integrate as scint
import mpl_toolkits.mplot3d as mplt3d
import math
#from math import factorial
#%matplotlib inline


# In[ ]:




# In[8]:

split_tab = np.zeros((100, 10))


# In[ ]:




# In[27]:

paramList = ['interpOrder', 'nbDims', 'Quantity', 'nbrCellX', 'nbrCellY', 'nbrCellZ', 'dx', 'dy', 'dz'];


# In[28]:

len( paramList )


# In[ ]:




# In[35]:

print( paramList[:] )


# In[34]:

print( paramList[2] )


# In[ ]:




# In[ ]:




# In[43]:

interpOrder_l=[1, 2, 3, 4]

nbDims_l=[1, 2, 3] 

#Qty_l=['Bx', 'By', 'Bz', 'Ex', 'Ey', 'Ez', 'rho', 'V', 'P']
Qty_l=[0, 1, 2, 3, 4, 5, 6, 7, 8]

nbrCellX_l=[40, 40, 40]
nbrCellY_l=[ 0, 10, 10]
nbrCellZ_l=[ 0,  0, 10]

dx_l=[0.1, 0.1, 0.1] # 1D, 2D and 3D cases 
dy_l=[0. , 0.1, 0.1]
dz_l=[0. , 0. , 0.1]

nbrTestCases = len(interpOrder_l)*len(nbDims_l)*len(Qty_l)
print( nbrTestCases )


# In[44]:

caseParamsTable = np.zeros((nbrTestCases, len(paramList)))


# In[ ]:




# In[33]:

col_l = np.arange( len(paramList) )
print( col_l )


# In[ ]:




# In[45]:

iord_l=np.arange( len(interpOrder_l) ) 
print( iord_l )
idim_l=np.arange( len(nbDims_l) ) 
print( idim_l )
iqty_l=np.arange( len(Qty_l) ) 
print( iqty_l )


# In[ ]:




# In[46]:

#  data_[iz + shape_[2]*iy + shape_[1]*shape_[2]*ix]; 
#icase = 0
for iord in iord_l:
    for idim in idim_l:
        for iqty in iqty_l:
            caseParamsTable[iqty + len(Qty_l)*idim + len(nbDims_l)*len(Qty_l)*iord, 0] = interpOrder_l[iord]
            caseParamsTable[iqty + len(Qty_l)*idim + len(nbDims_l)*len(Qty_l)*iord, 1] = nbDims_l[idim]
            caseParamsTable[iqty + len(Qty_l)*idim + len(nbDims_l)*len(Qty_l)*iord, 2] = Qty_l[iqty]
            caseParamsTable[iqty + len(Qty_l)*idim + len(nbDims_l)*len(Qty_l)*iord, 3] = nbrCellX_l[idim]
            caseParamsTable[iqty + len(Qty_l)*idim + len(nbDims_l)*len(Qty_l)*iord, 4] = nbrCellY_l[idim]
            caseParamsTable[iqty + len(Qty_l)*idim + len(nbDims_l)*len(Qty_l)*iord, 5] = nbrCellZ_l[idim]
            caseParamsTable[iqty + len(Qty_l)*idim + len(nbDims_l)*len(Qty_l)*iord, 6] = dx_l[idim]
            caseParamsTable[iqty + len(Qty_l)*idim + len(nbDims_l)*len(Qty_l)*iord, 7] = dy_l[idim]
            caseParamsTable[iqty + len(Qty_l)*idim + len(nbDims_l)*len(Qty_l)*iord, 8] = dz_l[idim]


# In[47]:

print( caseParamsTable )


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



