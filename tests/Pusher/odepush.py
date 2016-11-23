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
        f.write(("%f %f %d \n%d %d \n%f %f %f \n%f %f %f \n" ) %
           (tbegin_l[icase], tend_l[icase], nstep_l[icase], 
            q_l[icase], m_l[icase], 
            x0_l[icase], y0_l[icase], z0_l[icase], 
            vx0_l[icase], vy0_l[icase], vz0_l[icase] ) )

    f.close() 
    

    for icase in icase_l:
        fsol = open( os.path.join(path,("odepush_testCase%d.txt") % \
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

        Ex0 = Ex0_l[icase] 
        Ey0 = Ey0_l[icase]
        Ez0 = Ez0_l[icase]
        Bx0 = Bx0_l[icase]
        By0 = By0_l[icase]
        Bz0 = Bz0_l[icase]
        
        Exdef = funcDict[ExShape_l[icase]]
        Eydef = funcDict[EyShape_l[icase]]
        Ezdef = funcDict[EzShape_l[icase]]
        Bxdef = funcDict[BxShape_l[icase]]
        Bydef = funcDict[ByShape_l[icase]] 
        Bzdef = funcDict[BzShape_l[icase]]
                             

        sol = integrate.odeint(dynamicsEB, xv3_t0, t, \
              args=(q, m, Ex0, Exdef, Ey0, Eydef, Ez0, Ezdef, \
                          Bx0, Bxdef, By0, Bydef, Bz0, Bzdef ) )

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
 
        Ex_p = [ Exdef(Ex0, t[ik], sol[ik,0]) \
                 for ik in range(t.shape[0])]

        Ey_p = [ Exdef(Ey0, t[ik], sol[ik,0]) \
                 for ik in range(t.shape[0])]

        Ez_p = [ Exdef(Ez0, t[ik], sol[ik,0]) \
                 for ik in range(t.shape[0])]
        
        Bx_p = [ Bzdef(Bx0, t[ik], sol[ik,0]) \
                 for ik in range(t.shape[0])]
        
        By_p = [ Bzdef(By0, t[ik], sol[ik,0]) \
                 for ik in range(t.shape[0])]

        Bz_p = [ Bzdef(Bz0, t[ik], sol[ik,0]) \
                 for ik in range(t.shape[0])]

        np.savetxt(os.path.join(path,("odepush_fields_testCase%d.txt") % (icase_l[icase])), \
        (Ex_p, Ey_p, Ez_p, Bx_p, By_p, Bz_p), delimiter=' ') 

#        plt.figure()
#        plt.xlabel('x')
#        plt.ylabel('y')
#        plt.grid()
#        plt.plot(sol[:, 0], sol[:, 1], '-b', label='(x, y)')

#        print(Ex_p)
#        print(Ey_p)
#        print(Ez_p)
#        print(Bx_p)
#        print(By_p)
#        print(Bz_p)
        


def dynamicsEB(xv3, t, q, m, Ex0, Exdef, Ey0, Eydef, Ez0, Ezdef, \
                             Bx0, Bxdef, By0, Bydef, Bz0, Bzdef ):
    x, y, z, vx, vy, vz = xv3
    
#    Ex = Exdef(Ex0, t, x)
#    Ey = Eydef(Ey0, t, x)
#    Ez = Ezdef(Ez0, t, x)
    Bx = Bxdef(Bx0, t, x)
    By = Bydef(By0, t, x)
    Bz = Bzdef(Bz0, t, x)
    dxv3dt = [vx, vy, vz, (q/m)*(vy*Bz-vz*By), \
              (q/m)*(vz*Bx-vx*Bz), \
              (q/m)*(vx*By-vy*Bx)]
              
    return dxv3dt


if __name__ == "__main__":
    main()


