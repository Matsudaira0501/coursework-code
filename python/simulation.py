import random
import math 
from matplotlib import pyplot
from numpy import mean

def f(x):
    y = 4/(1+(x**2))
    return y


I_n_avelist = []
sigma_nlist = []
nlist = [10, 10**3, 10**5, 10**7]
for n in nlist:
    I_n = [0 for _ in range(10)]
    for i in range(10):
        for j in range(int(n)):
            x = random.random()
            I_n[i] += f(x)
        I_n[i] /= n
    I_n_ave = mean(I_n)
    sigma_n = math.sqrt((1/9)*mean([(I - I_n_ave)**2 for I in I_n]))
    I_n_avelist.append(I_n_ave)
    sigma_nlist.append(sigma_n)


pyplot.plot(nlist, I_n_avelist)
pyplot.xscale("log")
pyplot.show()

pyplot.plot(nlist, sigma_nlist)
pyplot.xscale("log")
pyplot.show()