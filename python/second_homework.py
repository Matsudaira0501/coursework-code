import math
import random
import matplotlib.pyplot as plt

def p(n):
    return math.comb(10,n)/ (2 ** 10)

x = 0
x_ls = [0 for _ in range(11)]

for i in range(11000):
  
    if x == 0:
        x = 1   
    elif x == 10:
        x = 9
    else:
        if random.random() <= (p(x-1) / (p(x-1) + p(x+1))):
            x -= 1
        else:
            x += 1  

    if i >= 1000:
        x_ls[x] += 1

x = [i for i in range(11)]
plt.bar(x, x_ls)
plt.plot(x, [p(n)*10000 for n in x], c="r")
plt.show()