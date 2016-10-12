# -*- coding: utf-8 -*-
#"""
#Created on Mon Oct 10 11:24:42 2016
#
#@author: mdrouin
#"""

#import numpy as np
import math


class GridLayout(object):
    """
    This class represents the python version of Gridlayout object
    its purpose is to define usual functions used for all tests using grid
    indexing etc.
    """


    def __init__(self):
        self.Direction_l = [(0,'X'), (1,'Y'), (2,'Z')]

        self.Qty_l=[(0,'Bx'), (1,'By'), (2,'Bz'), (3,'Ex'), (4,'Ey'), (5,'Ez'), (6,'rho'), (7,'V'), (8,'P')]

        self.QtyDict = {'Bx':0, 'By':1, 'Bz':2, 'Ex':3, 'Ey':4, 'Ez':5, 'rho':6, 'V':7, 'P':8}


        self.CenterX = {'Bx':'primal' , 'By':'dual'  , 'Bz':'dual'  ,
                        'Ex':'dual'   , 'Ey':'primal', 'Ez':'primal',
                        'rho':'primal', 'V':'primal' , 'P':'primal' }

        self.CenterY = {'Bx':'dual'   , 'By':'primal', 'Bz':'dual'  ,
                        'Ex':'primal' , 'Ey':'dual'  , 'Ez':'primal',
                        'rho':'primal', 'V':'primal' , 'P':'primal' }

        self.CenterZ = {'Bx':'dual'   , 'By':'dual'  , 'Bz':'primal',
                        'Ex':'primal' , 'Ey':'primal', 'Ez':'dual'  ,
                        'rho':'primal', 'V':'primal' , 'P':'primal' }


        self.centering = {'X':self.CenterX, 'Y':self.CenterY, 'Z':self.CenterZ}


    # ---------------------- ANY DIRECTION -----------------------------------------
    def qtyCentering(self,qty, direct):
        return self.centering[direct][qty]



    def nbrGhosts(self,interpOrder, centering):
        if centering == 'primal':
            return math.floor( interpOrder/2 )
        else:
            return math.floor( (interpOrder +1)/2 )



    def nbrGhostsDerived(self,interpOrder, centering):
        if centering == 'primal':
            # 1st order derivative becomes dual
            return math.floor( (interpOrder +1)/2 )
        else:
            # 1st order derivative becomes primal
            return math.floor( interpOrder/2 )




    def nbrGhostsPrimal(self,interpOrder):
        return math.floor( interpOrder/2 )



    def isDual(self,centering):
        if centering == 'dual':
            return 1
        else:
            return 0



    # ---- Start / End index methods -----------
    def ghostStartIndex(self):
        return 0



    def physicalStartIndex(self,interpOrder, centering):
        index = self.ghostStartIndex() + self.nbrGhosts(interpOrder, centering)
        return index



    def physicalEndIndex(self, interpOrder, centering, nbrCells):
        index = self.physicalStartIndex(interpOrder, centering) \
        + nbrCells - self.isDual(centering)
        return index



    def ghostEndIndex(self, interpOrder, centering, nbrCells):
        index = self.physicalEndIndex(interpOrder, centering, nbrCells) \
        + self.nbrGhosts(interpOrder, centering)
        return index



    # ---- Start / End   primal methods ------
    def physicalStartPrimal(self,interpOrder):
        index = self.ghostStartIndex() + self.nbrGhostsPrimal(interpOrder)
        return index



    def physicalEndPrimal(self,interpOrder, nbrCells):
        index = self.physicalStartPrimal(interpOrder) + nbrCells
        return index



    # ---- Alloc methods -------------------------

    def allocSize(self, interpOrder, centering, nbrCells):
        size = nbrCells + 1 + 2*self.nbrGhosts(interpOrder, centering)
        return size



    def allocSizeDerived(self, interpOrder, centering, nbrCells):
        size = nbrCells + 1 + 2*self.nbrGhostsDerived(interpOrder, centering)
        return size



    # ---- Get coordinate methods -------------------------
    # ds stands for dx or dy or dz
    #
    # This method returns a point
    #
    def fieldCoords(self, iprimal, iStart, qty, direction, ds, origin, derivOrder):
        halfCell = 0.

        newCentering = self.changeCentering( self.qtyCentering(qty, direction[1]), derivOrder )

        if newCentering == 'dual':
            halfCell = 0.5

        x = ( (iprimal - iStart) + halfCell )*ds + origin[direction[0]]

        return x



    # ---- Change centering method -------------------------
    #
    # Use case:
    #   changeCentering( qtyCentering(qty, direct), 1 )
    #
    def changeCentering(self, centering, derivOrder ):

    #    print( "Inputs\n   centering : %s\n   derivOrder : %d" % (centering, derivOrder) )

        newCentering = centering

        # if derivOrder is odd the centering
        # is changed
        if derivOrder%2 != 0:
            newCentering = self.swapCentering( centering )

    #    print( "   output : %s" % newCentering )

        return newCentering



    # -------------------------------------------------------
    def swapCentering(self, centering ):

        newCentering = 'primal'

        if centering == 'primal':
            newCentering = 'dual'

        return newCentering





