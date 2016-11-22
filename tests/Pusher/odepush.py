import scipy.integrate as integrate
import matplotlib.pyplot as plt
import numpy as np
import math


import sys
sys.path.insert(0, '../')

import gridlayout
import os

# --------- mathematical functions ----------------------
def uniform( E0, t, x ):
    return E0

def wave( E0, t, x ):
    return E0*math.cos(t-x)
    
# ---------------------------------------------------------

funcDict = {'unif' : uniform,
            'wave' : wave }
            

def main(path='./'):

    x0_l  = [5.]
    y0_l  = [0.]
    z0_l  = [0.]
    vx0_l = [0.]
    vy0_l = [2.]
    vz0_l = [0.]

    q_l = [1]
    m_l = [1]
    Ex0_l = [0.] ; ExShape_l = ['unif'] 
    Ey0_l = [0.] ; EyShape_l = ['unif'] 
    Ez0_l = [0.] ; EzShape_l = ['unif'] 
    Bx0_l = [0.] ; BxShape_l = ['unif'] 
    By0_l = [0.] ; ByShape_l = ['unif'] 
    Bz0_l = [5.] ; BzShape_l = ['unif'] 

    tbegin_l = [0.]
    tend_l   = [1.57]
    nstep_l = [32]


    nbrTestCases = len( x0_l )

    icase_l = np.arange( nbrTestCases )
    print( icase_l )

    # -------- Let us define a function on Bx with Yee lattice --------
    f = open(os.path.join(path,"odepush_summary.txt"), "w")

    # the number of test cases
    f.write("%d \n" % len(icase_l) )  

    for icase in icase_l:
        f.write(("%f %f %d \n%d %d \n%f %f %f \n%f %f %f \n" \
                 "%f %s \n%f %s \n%f %s \n" \
                 "%f %s \n%f %s \n%f %s ") %
           (tbegin_l[icase], tend_l[icase], nstep_l[icase], 
            q_l[icase], m_l[icase], 
            x0_l[icase], y0_l[icase], z0_l[icase], 
            vx0_l[icase], vy0_l[icase], vz0_l[icase],
            Ex0_l[icase], ExShape_l[icase],
            Ey0_l[icase], EyShape_l[icase],
            Ez0_l[icase], EzShape_l[icase],
            Bx0_l[icase], BxShape_l[icase],
            By0_l[icase], ByShape_l[icase],
            Bz0_l[icase], BzShape_l[icase] ) )

        f.write("\n")

    f.close() 
    

    for icase in icase_l:
        fsol = open( os.path.join(path,("odepush_solutionCase%d.txt") % \
        (icase_l[icase])), "w")        
        
        x0  = x0_l[icase]
        y0  = y0_l[icase]
        z0  = z0_l[icase]
        vx0 = vx0_l[icase]
        vy0 = vy0_l[icase]
        vz0 = vz0_l[icase]
        xv3_t0 = [x0, y0, z0, vx0, vy0, vz0]
    
        t0 = tbegin_l[icase]
        tf = tend_l[icase]
        nstep = nstep_l[icase]
    
        t = np.linspace(t0, tf, nstep)

        q = q_l[icase] 
        m = m_l[icase]    

        sol = integrate.odeint(dynamicsEB, xv3_t0, t, \
              args=(q, m, \
              Ex0_l[icase], funcDict[ExShape_l[icase]], \
              Ey0_l[icase], funcDict[EyShape_l[icase]], \
              Ez0_l[icase], funcDict[EzShape_l[icase]], \
              Bx0_l[icase], funcDict[BxShape_l[icase]], \
              By0_l[icase], funcDict[ByShape_l[icase]], \
              Bz0_l[icase], funcDict[BzShape_l[icase]] ) )

#        print( np.size(sol[:,0]) )

        for xsol in sol[:,0]:
            fsol.write("%f " % xsol)
        fsol.write("\n")

        for ysol in sol[:,1]:
            fsol.write("%f " % ysol)
        fsol.write("\n")        
        
        for zsol in sol[:,2]:
            fsol.write("%f " % zsol)
        fsol.write("\n")    

        for vxsol in sol[:,3]:
            fsol.write("%f " % vxsol)
        fsol.write("\n")    

        for vysol in sol[:,4]:
            fsol.write("%f " % vysol)
        fsol.write("\n")    

        for vzsol in sol[:,5]:
            fsol.write("%f " % vzsol)
        fsol.write("\n")            
        
        fsol.close()
    


def dynamicsEB(xv3, t, q, m, Ex0, Exdef, Ey0, Eydef, Ez0, Ezdef, \
                             Bx0, Bxdef, By0, Bydef, Bz0, Bzdef ):
    x, y, z, vx, vy, vz = xv3
    
#    Ex = Exdef(Ex0, t, x)
#    Ey = Eydef(Ey0, t, x)
#    Ez = Ezdef(Ez0, t, x)
    Bx = Bxdef(Bx0, t, x)
    By = Bydef(By0, t, x)
    Bz = Bzdef(Bz0, t, x)
    dxv3dt = [vx, vy, vz, (q/m)*(vy*Bz-vz*By), (q/m)*(vz*Bx-vx*Bz), (q/m)*(vx*By-vy*Bx)]
    return dxv3dt


if __name__ == "__main__":
    main()


