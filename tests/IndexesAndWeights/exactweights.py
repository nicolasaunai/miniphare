#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Created on Fri Apr 28 17:56:07 2017

@author: mdrouin
"""

import numpy as np
import matplotlib.pyplot as plt
import scipy.integrate as scint

import sys
import os
#sys.path.insert(0, "/home/mdrouin/codes/mini_phare/miniphare/tests")
sys.path.insert(0, os.path.join(os.path.dirname(os.path.realpath(__file__)),'..'))

import interpolator


#%%
# GENERAL SCRIPT FOR 
# ARBITRARY ORDER AND RF = 3

def main(path='./'):
    
    interp = interpolator.Interpolator()
    
    NbPoints = 10000
    xmin = -5.
    xmax =  5.
    Lmax = np.abs(xmax - xmin)
    dx = Lmax/NbPoints
    print("dx = %f" %dx)
    
    x = interp.Getx(NbPoints, xmin, xmax)
    
    x0 = 0.    # center of the spline function
    dx_L0 = 1.
    
    # Arbitrary order
    order=3
    
    print("order = %d"%order)
    
    shape = interp.getShape(order, x, x0, dx_L0)
    
    # Compute the area using the composite Simpson's rule.
    area_ref = scint.simps(shape, dx=dx)
    print("Area ref=", area_ref)
    
    nbpts = (order+1)
    print("nbpts = (order+1)")
    print("nbr of points = %d"%nbpts)
    
    ik_l = np.arange(nbpts)
    print(ik_l)
    
    bmin = x0 - (order+2)*0.5*dx_L0
    bmax = x0 + (order+2)*0.5*dx_L0
    plt.plot(x,shape)
    
    plt.xlim(bmin, bmax)
    
    plt.xlabel("x",size=20)  
    plt.ylabel("S(x)",size=20) 


if __name__ == "__main__":
    main()

#%%





