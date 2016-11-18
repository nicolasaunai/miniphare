import scipy.integrate as integrate
import matplotlib.pyplot as plt
import numpy as np


pi = np.pi
sqrt = np.sqrt
cos = np.cos
sin = np.sin



def dynamicsEB(xv3, t, q, m, Ex, Ey, Ez, Bx, By, Bz):
    x, y, z, vx, vy, vz = xv3
    dxv3dt = [vx, vy, vz, (q/m)*(vy*Bz-vz*By), (q/m)*(vz*Bx-vx*Bz), (q/m)*(vx*By-vy*Bx)]
    return dxv3dt

def main():
    q = 1
    m = 1
    Ex = 0.
    Ey = 0.
    Ez = 0.
    Bx = 0.
    By = 0.
    Bz = 5.

    xv3_t0 = [5., 0., 0., 0., 2., 0.]
    
    t = np.linspace(0, 100, 10001)
    
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
    
    plt.figure()
    plt.xlabel('x')
    plt.ylabel('vx, vy')
    plt.grid()
    
    plt.plot(sol[:, 0], sol[:, 3], '-b', label='(x, vx)')
    plt.plot(sol[:, 0], sol[:, 4], '-k', label='(x, vy)')
    
    plt.legend(loc='best')
    
    plt.show()


if __name__ == "__main__":
    main()


