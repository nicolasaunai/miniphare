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

    nbCase = 4

    x0_l  = [5.]*nbCase
    y0_l  = [0.]*nbCase
    z0_l  = [0.]*nbCase
    vx0_l = [0., 0., 0., 2.]
    vy0_l = [2., 2., 2., 0.]
    vz0_l = [0.]*nbCase

    q_l = [1]*nbCase
    m_l = [1]*nbCase
    
    Ex0_l = [0.]*nbCase ; ExShape_l = ['unif']*nbCase 
    Ey0_l = [0.]*nbCase ; EyShape_l = ['unif']*nbCase 
    Ez0_l = [0.]*nbCase ; EzShape_l = ['unif']*nbCase 
    Bx0_l = [0.]*nbCase ; BxShape_l = ['unif']*nbCase 
    By0_l = [0.]*nbCase ; ByShape_l = ['unif']*nbCase 
    Bz0_l = [5.]*nbCase ; BzShape_l = ['unif']*nbCase 

    # Tgyro = 2*pi/omega with omega = q*B/m    
    print(len(Bz0_l))
    omega_l = [q_l[ik]*Bz0_l[ik]/m_l[ik] for ik in range(len(Bz0_l))]    
    Tgyro_l = [2*np.pi/omega_l[ik] for ik in range(len(Bz0_l))]

    tbegin_l = [0.]*nbCase
    tend_l   = [Tgyro_l[ik] for ik in range(len(Bz0_l))]
    nstep_l  = [16, 32, 64, 32]
    
    print("T_gyro = ", tend_l)


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
        # Time step
        dt = (tend_l[icase] - tbegin_l[icase])/nstep_l[icase]
        print("dt = %f" % dt)        

        file_x = open( os.path.join(path,("odepush_x_testCase%d.txt") % \
        (icase_l[icase])), "w")

        file_y = open( os.path.join(path,("odepush_y_testCase%d.txt") % \
        (icase_l[icase])), "w")
        
        file_z = open( os.path.join(path,("odepush_z_testCase%d.txt") % \
        (icase_l[icase])), "w")
        
        file_vx = open( os.path.join(path,("odepush_vx_testCase%d.txt") % \
        (icase_l[icase])), "w")
        
        file_vy = open( os.path.join(path,("odepush_vy_testCase%d.txt") % \
        (icase_l[icase])), "w")
        
        file_vz = open( os.path.join(path,("odepush_vz_testCase%d.txt") % \
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
    
        t = np.linspace(t0, tf, nstep+1)

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
            file_x.write("%f \n" % xsol)
        file_x.write("\n")

        for ysol in sol[:,1]:
            file_y.write("%f \n" % ysol)
        file_y.write("\n")        
        
        for zsol in sol[:,2]:
            file_z.write("%f \n" % zsol)
        file_z.write("\n")    

        for vxsol in sol[:,3]:
            file_vx.write("%f \n" % vxsol)
        file_vx.write("\n")    

        for vysol in sol[:,4]:
            file_vy.write("%f \n" % vysol)
        file_vy.write("\n")    

        for vzsol in sol[:,5]:
            file_vz.write("%f \n" % vzsol)
        file_vz.write("\n")            
        
        file_x.close()
        file_y.close()
        file_z.close()
        file_vx.close()
        file_vy.close()
        file_vz.close()
 
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

#        plt.figure()
#        plt.xlabel('t')
#        plt.ylabel('vx')
#        plt.grid()
#        plt.plot(t, sol[:, 3], '-b', label='(t, vx)')
#
#        plt.figure()
#        plt.xlabel('t')
#        plt.ylabel('vy')
#        plt.grid()
#        plt.plot(t, sol[:, 4], '-b', label='(t, vy)')

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


