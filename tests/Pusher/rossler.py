import scipy.integrate as integrate
import matplotlib.pyplot as plt
import numpy as np


pi = np.pi
sqrt = np.sqrt
cos = np.cos
sin = np.sin



def rossler(pos, t, a, b, c):
    x, y, z = pos
    dposdt = [-y-z, x+a*y, b+z*(x-c)]
    return dposdt


def main():
    a = 0.2
    b = 0.4
    c = 5.7
    
    pos0 = [0.1, 0.2, 0.05]
    
    t = np.linspace(0, 200, 2001)
    
    sol = integrate.odeint(rossler, pos0, t, args=(a, b, c))
    
    plt.plot(sol[:, 0], sol[:, 1], 'b', label='Rossler')
    
    plt.legend(loc='best')
    plt.xlabel('x')
    plt.ylabel('y')
    plt.grid()
    plt.show()



if __name__ == "__main__":
    main()


