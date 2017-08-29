"""
Quick functions to read fields exported with the PHARE ASCII exporter
"""

import numpy as np

class Field:
    def __init__(self, nbdims, origin, spacing, nbrNodes, centering, data):
        self.nbdims      = nbdims
        self.origin      = origin
        self.spacing     = spacing
        self.nbrNodes    = nbrNodes
        self.centering   = centering
        self.data        = data



def read_block(lines, cursor):
    """
    read a component in a list of lines from a Field File
    """
    nbdims = int(lines[cursor + 1])
    origin_s = lines[cursor + 3]
    origin = [float(o) for o in origin_s.split()]
    spacing = [float(n) for n in lines[cursor + 5].split()]
    nbrNodes = [int(n) for n in lines[cursor + 7].split()]
    centering = [float(c) for c in lines[cursor+ 9].split()]
    data = np.zeros(nbrNodes[0])
    for i in np.arange(nbrNodes[0]):
        data[i] = float(lines[cursor+12+i])

    return Field(nbdims, origin, spacing, nbrNodes, centering, data)





def readEMFile(filename):
    """
    read the field File 'filename'
    """
    f = open(filename, "r")
    lines = f.readlines()
    cursor = 0
    Ex = read_block(lines, cursor)
    cursor = int(12 + Ex.nbrNodes[0]+3)
    Ey = read_block(lines, cursor)
    cursor = int(12*2 + Ey.nbrNodes[0] + Ex.nbrNodes[0] + 3*2)
    Ez = read_block(lines, cursor)
    f.close()
    return Ex, Ey, Ez




def EM_filenameFromTime(fieldname, patchID, time):
    """
    return the name of a EM file for a given time
    """
    
    return fieldname + '_' + '%06d' % (patchID) + '_' + '%7.6f.txt' % (time)








#
