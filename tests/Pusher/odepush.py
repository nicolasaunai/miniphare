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
    # number of Periods
    nPer = 1

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
    tend_l   = [nPer*Tgyro_l[ik] for ik in range(len(Bz0_l))]
    nstep_l  = [nPer*16, nPer*32, nPer*64, nPer*32]
    
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
                             

        sol = modifiedBorisV2( xv3_t0, t, q, m, \
                   t0, tf, nstep, \
                   Ex0, Exdef, Ey0, Eydef, Ez0, Ezdef, \
                   Bx0, Bxdef, By0, Bydef, Bz0, Bzdef )


#        sol_ref = integrate.odeint(dynamicsEB, xv3_t0, t, \
#                args=(q, m, Ex0, Exdef, Ey0, Eydef, Ez0, Ezdef, \
#                            Bx0, Bxdef, By0, Bydef, Bz0, Bzdef ) )

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
#        plt.plot(sol_ref[:, 0], sol_ref[:, 1], '-r', label='(x, y)')

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
        


def modifiedBoris( xv3, t, q, m, \
                   t0, tf, nstep, \
                   Ex0, Exdef, Ey0, Eydef, Ez0, Ezdef, \
                   Bx0, Bxdef, By0, Bydef, Bz0, Bzdef ):
                       
    x, y, z, vx, vy, vz = xv3
    
    # loop over the time steps
    dt = (tf - t0)/nstep
    
    dts2 = 0.5*dt    
    
    sol = np.zeros((nstep+1, 6))    
    
    sol[0,0] = x
    sol[0,1] = y
    sol[0,2] = z
    sol[0,3] = vx
    sol[0,4] = vy
    sol[0,5] = vz
    
    istep_l = np.arange(nstep)+1
    for istep in istep_l:
        x_tild = x + dts2*vx   
        y_tild = y + dts2*vy
        z_tild = z + dts2*vz
        
        # initializing fields
        Ex = Exdef(Ex0, t, x)
        Ey = Eydef(Ey0, t, x)
        Ez = Ezdef(Ez0, t, x)
        Bx = Bxdef(Bx0, t, x)
        By = Bydef(By0, t, x)
        Bz = Bzdef(Bz0, t, x)
        
        coef = q*dts2/m
        
        # 1st psuh of the electric field
        velx1 = vx + coef*Ex
        vely1 = vy + coef*Ey
        velz1 = vz + coef*Ez
        
        # preparing magnetic rotation
        rx = coef*Bx ;
        ry = coef*By ;
        rz = coef*Bz ;
    
        invDet = 2./(1. + rx*rx + ry*ry + rz*rz ) ;
    
        # preparing rotation matrix due to the magnetic field
        # m = invDet*(I + r*r - r x I) - I where x denotes the cross product
        mxx = invDet*( 1. + rx*rx      ) - 1. ;
        mxy = invDet*(      rx*ry + rz ) ;
        mxz = invDet*(      rx*rz - ry ) ;
    
        myx = invDet*(      ry*rx - rz ) ;
        myy = invDet*( 1. + ry*ry      ) - 1. ;
        myz = invDet*(      ry*rz + rx ) ;
    
        mzx = invDet*(      rz*rx + ry ) ;
        mzy = invDet*(      rz*ry - rx ) ;
        mzz = invDet*( 1. + rz*rz      ) - 1. ;
    
        # magnetic rotation
        velx2 = mxx*velx1 + mxy*vely1 + mxz*velz1 ;
        vely2 = myx*velx1 + myy*vely1 + myz*velz1 ;
        velz2 = mzx*velx1 + mzy*vely1 + mzz*velz1 ;

        # 2nd push of the electric field
        vx = velx2 + coef*Ex
        vy = vely2 + coef*Ey
        vz = velz2 + coef*Ez    
        
        # update the position
        x = x_tild + dts2*vx
        y = y_tild + dts2*vy
        z = z_tild + dts2*vz

        # Update the solution table        
        sol[istep,0] = x
        sol[istep,1] = y
        sol[istep,2] = z
        sol[istep,3] = vx
        sol[istep,4] = vy
        sol[istep,5] = vz

              
    return sol



def modifiedBorisV2( xv3, t, q, m, \
                   t0, tf, nstep, \
                   Ex0, Exdef, Ey0, Eydef, Ez0, Ezdef, \
                   Bx0, Bxdef, By0, Bydef, Bz0, Bzdef ):
                       
    x, y, z, vx, vy, vz = xv3
    
    # loop over the time steps
    dt = (tf - t0)/nstep
    
    dts2 = 0.5*dt    

    coef = q*dts2/m 
    
    sol = np.zeros((nstep+1, 6))    
    
    sol[0,0] = x
    sol[0,1] = y
    sol[0,2] = z
    sol[0,3] = vx
    sol[0,4] = vy
    sol[0,5] = vz
    
    istep_l = np.arange(nstep)+1
    for istep in istep_l:
        
        # initializing fields
        Ex = Exdef(Ex0, t, x)
        Ey = Eydef(Ey0, t, x)
        Ez = Ezdef(Ez0, t, x)
        Bx = Bxdef(Bx0, t, x)
        By = Bydef(By0, t, x)
        Bz = Bzdef(Bz0, t, x)

        # Preprush from x(tn) to prediction x(tn+1/2) 
        x_tild = x + dts2*vx        
        y_tild = y + dts2*vy
        z_tild = z + dts2*vz
        
        # 1st psuh of the electric field
        velx1 = vx + coef*Ex
        vely1 = vy + coef*Ey
        velz1 = vz + coef*Ez
 
        # preparing magnetic rotation
        rx = coef*Bx  
        ry = coef*By 
        rz = coef*Bz 
        
        rx2 = rx*rx
        ry2 = ry*ry
        rz2 = rz*rz        
        rxry = rx*ry
        rxrz = rx*rz
        ryrz = ry*rz       
    
        invDet = 1./(1. + rx2 + ry2 + rz2 ) ;
    
        # preparing rotation matrix due to the magnetic field
        # m = invDet*(I + r*r - r x I) - I where x denotes the cross product
        mxx = 1. + rx2 - ry2 - rz2 
        mxy = 2.*( rxry + rz )
        mxz = 2.*( rxrz - ry )
    
        myx = 2.*( rxry - rz )
        myy = 1. + ry2 - rx2 - rz2 
        myz = 2.*( ryrz + rx )
    
        mzx = 2.*( rxrz + ry )
        mzy = 2.*( ryrz - rx )
        mzz = 1. + rz2 - rx2 -ry2 
    
        # magnetic rotation
        velx2 = ( mxx*velx1 + mxy*vely1 + mxz*velz1 )*invDet
        vely2 = ( myx*velx1 + myy*vely1 + myz*velz1 )*invDet
        velz2 = ( mzx*velx1 + mzy*vely1 + mzz*velz1 )*invDet

        # 2nd push of the electric field
        vx = velx2 + coef*Ex
        vy = vely2 + coef*Ey
        vz = velz2 + coef*Ez    
        
        # update the position
        x = x_tild + dts2*vx
        y = y_tild + dts2*vy
        z = z_tild + dts2*vz

        # Update the solution table        
        sol[istep,0] = x
        sol[istep,1] = y
        sol[istep,2] = z
        sol[istep,3] = vx
        sol[istep,4] = vy
        sol[istep,5] = vz

              
    return sol
    
    
    
def modifiedBorisV3tan( xv3, t, q, m, \
                   t0, tf, nstep, \
                   Ex0, Exdef, Ey0, Eydef, Ez0, Ezdef, \
                   Bx0, Bxdef, By0, Bydef, Bz0, Bzdef ):
                       
    x, y, z, vx, vy, vz = xv3
    
    # loop over the time steps
    dt = (tf - t0)/nstep
    
    dts2 = 0.5*dt    
    
    sol = np.zeros((nstep+1, 6))    
    
    sol[0,0] = x
    sol[0,1] = y
    sol[0,2] = z
    sol[0,3] = vx
    sol[0,4] = vy
    sol[0,5] = vz
    
    istep_l = np.arange(nstep)+1
    for istep in istep_l:
        
        # initializing fields
        Ex = Exdef(Ex0, t, x)
        Ey = Eydef(Ey0, t, x)
        Ez = Ezdef(Ez0, t, x)
        Bx = Bxdef(Bx0, t, x)
        By = Bydef(By0, t, x)
        Bz = Bzdef(Bz0, t, x)

        # Estimate of the cyclotronic pulsation
        omega_c = q*np.sqrt( Bx**2 + By**2 + Bz**2 )/m
        
        # We find delta such that:
        # tan( 0.5*omega_c*dtNew ) = 0.5*omega_c*dt
        # dtNew = dt + delta
        #
        # a first order expansion gives
        # tan(omega_c*dts2) + delta*( 1+ tan(omega_c*dts2)**2 ) = omega_c*dts2
        #
        # We deduce delta:
        delta = -(omega_c*dts2 - np.tan(omega_c*dts2))/(1.+ np.tan(omega_c*dts2)**2)

        delta = 0.
        dts2 = 0.5*(dt+delta)
        
        if istep==1:
            print("delta = %f" % delta)  
        
        coef = q*dts2/m        

        # Prepush
        x_tild = x + dts2*vx        
        y_tild = y + dts2*vy
        z_tild = z + dts2*vz
        
        # 1st psuh of the electric field
        velx1 = vx + coef*Ex
        vely1 = vy + coef*Ey
        velz1 = vz + coef*Ez
 
        # preparing magnetic rotation
        # Use with delta != 0
#        rx = coef*Bx  
#        ry = coef*By 
#        rz = coef*Bz 
        
        # Use with delta == 0
        rx = np.tan( coef*Bx ) 
        ry = np.tan( coef*By )
        rz = np.tan( coef*Bz )
        
        rx2 = rx*rx
        ry2 = ry*ry
        rz2 = rz*rz        
        rxry = rx*ry
        rxrz = rx*rz
        ryrz = ry*rz       
    
        invDet = 1./(1. + rx2 + ry2 + rz2 ) ;
    
        # preparing rotation matrix due to the magnetic field
        # m = invDet*(I + r*r - r x I) - I where x denotes the cross product
        mxx = 1. + rx2 - ry2 - rz2 
        mxy = 2.*( rxry + rz )
        mxz = 2.*( rxrz - ry )
    
        myx = 2.*( rxry - rz )
        myy = 1. + ry2 - rx2 - rz2 
        myz = 2.*( ryrz + rx )
    
        mzx = 2.*( rxrz + ry )
        mzy = 2.*( ryrz - rx )
        mzz = 1. + rz2 - rx2 -ry2 
    
        # magnetic rotation
        velx2 = ( mxx*velx1 + mxy*vely1 + mxz*velz1 )*invDet
        vely2 = ( myx*velx1 + myy*vely1 + myz*velz1 )*invDet
        velz2 = ( mzx*velx1 + mzy*vely1 + mzz*velz1 )*invDet

        # 2nd push of the electric field
        vx = velx2 + coef*Ex
        vy = vely2 + coef*Ey
        vz = velz2 + coef*Ez    
        
        # update the position
        x = x_tild + dts2*vx
        y = y_tild + dts2*vy
        z = z_tild + dts2*vz

        # Update the solution table        
        sol[istep,0] = x
        sol[istep,1] = y
        sol[istep,2] = z
        sol[istep,3] = vx
        sol[istep,4] = vy
        sol[istep,5] = vz

              
    return sol    
    


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


