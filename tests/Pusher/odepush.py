import scipy.integrate as integrate
import matplotlib.pyplot as plt
import numpy as np



import sys
sys.path.insert(0, '../')

import gridlayout
import os



def main(path='./'):
    q = 1
    m = 1
    Ex_l = [0.]
    Ey_l = [0.]
    Ez_l = [0.]
    Bx_l = [0.]
    By_l = [0.]
    Bz_l = [5.]

    x0_l  = [5.]
    y0_l  = [0.]
    z0_l  = [0.]
    vx0_l = [0.]
    vy0_l = [2.]
    vz0_l = [0.]

    nbrTestCases = len( x0_l )

    icase_l = np.arange( nbrTestCases )
    print( icase_l )

    # -------- Let us define a function on Bx with Yee lattice --------
#    f = open(os.path.join(path,"indexes_summary.txt"), "w")

    # the number of test cases
#    f.write("%d \n" % len(icase_l) )  
#
#    for icase in icase_l:
#        order = interpOrder_l[icase]
#        nbrX = nbrCellX_l[icase]
#        dx = dx_l[icase]
#        field = field_l[icase]
#        xmin = patchMinX_l[icase]
#        xpart = particlePosX_l[icase]
#
#        f.write(("%d %d %5.4f %s %f %f") %
#           (order, nbrX, 
#            dx, field, 
#            xmin, xpart ) )
#
#        f.write("\n")
#
#    f.close() 
    

    for icase in icase_l:
        x0  = x0_l[icase]
        y0  = y0_l[icase]
        z0  = z0_l[icase]
        vx0 = vx0_l[icase]
        vy0 = vy0_l[icase]
        vz0 = vz0_l[icase]
        xv3_t0 = [x0, y0, z0, vx0, vy0, vz0]
    
        t = np.linspace(0, 1, 10001)
    
        Ex = Ex_l[icase]
        Ey = Ey_l[icase]
        Ez = Ez_l[icase]
        Bx = Bx_l[icase]
        By = By_l[icase]
        Bz = Bz_l[icase]
        sol = integrate.odeint(dynamicsEB, xv3_t0, t, args=(q, m, Ex, Ey, Ez, Bx, By, Bz))
 
    
        plt.figure()
        plt.xlabel('x')
        plt.ylabel('y')
        plt.grid()
        plt.plot(sol[:, 0], sol[:, 1], '-b', label='(x, y)')
        
        #plt.figure()
        #plt.xlabel('y')
        #plt.ylabel('vy')
        #plt.grid()
        #plt.plot(sol[:, 1], sol[:, 4], '-b', label='(y, vy)')
        
#        plt.figure()
#        plt.xlabel('x')
#        plt.ylabel('vx, vy')
#        plt.grid()
#        
#        plt.plot(sol[:, 0], sol[:, 3], '-b', label='(x, vx)')
#        plt.plot(sol[:, 0], sol[:, 4], '-k', label='(x, vy)')
#        
#        plt.legend(loc='best')
#        
#        plt.show()




def dynamicsEB(xv3, t, q, m, Ex, Ey, Ez, Bx, By, Bz):
    x, y, z, vx, vy, vz = xv3
    dxv3dt = [vx, vy, vz, (q/m)*(vy*Bz-vz*By), (q/m)*(vz*Bx-vx*Bz), (q/m)*(vx*By-vy*Bx)]
    return dxv3dt


if __name__ == "__main__":
    main()


