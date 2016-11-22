import scipy.integrate as integrate
import matplotlib.pyplot as plt
import numpy as np



import sys
sys.path.insert(0, '../')

import gridlayout
import os



def main(path='./'):

    x0_l  = [5.]
    y0_l  = [0.]
    z0_l  = [0.]
    vx0_l = [0.]
    vy0_l = [2.]
    vz0_l = [0.]

    q_l = [1]
    m_l = [1]
    Ex_l = [0.]
    Ey_l = [0.]
    Ez_l = [0.]
    Bx_l = [0.]
    By_l = [0.]
    Bz_l = [5.]

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
        f.write(("%f %f %d \n%d %d \n%f %f %f \n%f %f %f \n%f %f %f \n%f %f %f") %
           (tbegin_l[icase], tend_l[icase], nstep_l[icase], 
            q_l[icase], m_l[icase], 
            x0_l[icase], y0_l[icase], z0_l[icase], 
            vx0_l[icase], vy0_l[icase], vz0_l[icase],
            Ex_l[icase], Ey_l[icase], Ez_l[icase], 
            Bx_l[icase], By_l[icase], Bz_l[icase] ) )

        f.write("\n")

    f.close() 
    

    for icase in icase_l:
        f = open( os.path.join(path,("odepush_testCase%d.txt") % \
        (icase_l[icase])), "w")        
        
        x0  = x0_l[icase]
        y0  = y0_l[icase]
        z0  = z0_l[icase]
        vx0 = vx0_l[icase]
        vy0 = vy0_l[icase]
        vz0 = vz0_l[icase]
        xv3_t0 = [x0, y0, z0, vx0, vy0, vz0]
    
        t1 = tbegin_l[icase]
        t2 = tend_l[icase]
        nstep = nstep_l[icase]
    
        t = np.linspace(t1, t2, nstep)

        q = q_l[icase] 
        m = m_l[icase]    
        Ex = Ex_l[icase]
        Ey = Ey_l[icase]
        Ez = Ez_l[icase]
        Bx = Bx_l[icase]
        By = By_l[icase]
        Bz = Bz_l[icase]
        sol = integrate.odeint(dynamicsEB, xv3_t0, t, args=(q, m, Ex, Ey, Ez, Bx, By, Bz))

#        print( np.size(sol[:,0]) )

        for xsol in sol[:,0]:
            f.write("%f " % xsol)
        f.write("\n")

        for ysol in sol[:,1]:
            f.write("%f " % ysol)
        f.write("\n")        
        
        f.close()
    


def dynamicsEB(xv3, t, q, m, Ex, Ey, Ez, Bx, By, Bz):
    x, y, z, vx, vy, vz = xv3
    dxv3dt = [vx, vy, vz, (q/m)*(vy*Bz-vz*By), (q/m)*(vz*Bx-vx*Bz), (q/m)*(vx*By-vy*Bx)]
    return dxv3dt


if __name__ == "__main__":
    main()


