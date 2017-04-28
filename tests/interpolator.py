#!/usr/bin/env python
#!coding: utf-8

import numpy as np
import math


class Interpolator(object):
    """
    This class represents the python version of Gridlayout object
    its purpose is to define usual functions used for all tests using grid
    indexing etc.
    """


#    def __init__(self):

    
    
    def reducedCoord(self, field, xmin, xpart, dx, gl ):
    
        centeringX = gl.qtyCentering(field, 'X')    
#        print(centeringX)
        
        delta = 0.
        if centeringX == 'dual':
            delta = 0.5
            
        reduced = (xpart + delta*dx - xmin)/dx    
        return reduced
    

    def indexList(self, order, reduced ):
        imin = math.floor( reduced - 0.5*(order-1.) )
    
        ik_l = np.arange(order+1)   
        
        indexes = imin + ik_l
        return indexes  

    
    def Getx(self, nx, xmin, xmax):
        dx = np.abs(xmax - xmin)/float(nx)
        x  = xmin + np.arange(nx+1)*dx
        return x
    
    
    def GetOrd1(self, x, x0, dx):
        
        xi=(x-x0)/dx
    
        izeros = np.argwhere(abs(xi) > 1.)
        ileft  = np.argwhere(xi < 0.)
        iright = np.argwhere(xi >= 0.)
        
        shape = np.zeros(len(x))
        
        for ik in ileft:
            shape[ik] = xi[ik]+1.
        
        for ik in iright:
            shape[ik] = 1.-xi[ik]
            
        for ik in izeros:
            shape[ik] = 0.
        
        return (shape)
        
        
        
    
    def GetOrd2(self, x, x0, dx):
        
        xi=(x-x0)/dx
    
        iequ1  = np.argwhere( abs(xi) < 0.5  )
        iequ2  = np.argwhere( abs(xi) >= 0.5 ) 
        izeros = np.argwhere( abs(xi) > 1.5  )
        
        shape = np.zeros(len(x))
        
        for ik in iequ1:
            shape[ik] = 0.75 - abs(xi[ik])**2.
        
        for ik in iequ2:
            shape[ik] = 0.5*(1.5 - abs(xi[ik]))**2.
            
        for ik in izeros:
            shape[ik] = 0.
        
        return (shape)
    
    
    def GetOrd3(self, x, x0, dx):
        
        xi=(x-x0)/dx
    
        iequ1  = np.argwhere( abs(xi) < 1.  )
        iequ2  = np.argwhere( abs(xi) >= 1. ) 
        izeros = np.argwhere( abs(xi) > 2.  )
        
        shape = np.zeros(len(x))
        
        for ik in iequ1:
            shape[ik] = 0.5*abs(xi[ik])**3. - xi[ik]**2. + 2./3.
        
        for ik in iequ2:
            shape[ik] = 4./3.*(1. - 0.5*abs(xi[ik]))**3.
            
        for ik in izeros:
            shape[ik] = 0.
        
        return (shape)
    
    
    def GetOrd4(self, x, x0, dx):
        
        xi=(x-x0)/dx
    
        iequ1  = np.argwhere( abs(xi) <  0.5 )
        iequ2  = np.argwhere( abs(xi) >= 0.5 ) 
        iequ3  = np.argwhere( abs(xi) >= 1.5 ) 
        izeros = np.argwhere( abs(xi) >  2.5 )
        
        shape = np.zeros(len(x))
        
        for ik in iequ1:
            shape[ik] = 1./192.*( 115. - 120.*xi[ik]**2. + 48.*xi[ik]**4. )
        
        for ik in iequ2:
            shape[ik] = 1./96.*( 55. + 20.*abs(xi[ik]) - 120.*xi[ik]**2. + 80.*abs(xi[ik])**3. - 16.*xi[ik]**4.)
            
        for ik in iequ3:
            shape[ik] = 1./24.*(2.5 - abs(xi[ik]))**4.
            
        for ik in izeros:
            shape[ik] = 0.
        
        return (shape)
    
    
    
    
    def getShape(self, order, x, x0, dx_L0):
        return {
            1: self.GetOrd1(x, x0, dx_L0),
            2: self.GetOrd2(x, x0, dx_L0),
            3: self.GetOrd3(x, x0, dx_L0),
            4: self.GetOrd4(x, x0, dx_L0),
        }.get(order, self.GetOrd1(x, x0, dx_L0))
    
    
    
    
    
    
    
    
    
    