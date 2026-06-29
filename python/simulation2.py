import matplotlib.pyplot as plt
import random
import math

def coin():
    c = 0
    for _ in range(10):
        c += round(random.random())
    return c

def p(n):
    return math.comb(10,n)/ (2 ** 10)

#1

a = [i for i in range(11)]
plt.bar(a, [p(n) for n in a])
plt.show()

#2

ls = [0 for _ in range(11)]
for _ in range(10000):
    ls[coin()] += 1
    
x = [i for i in range(11)]
plt.bar(x, ls)
plt.show()

#3

y = 0
y_ls = [0 for _ in range(11)]

for i in range(11000):
  
    if y == 0:
        y = 1   
    elif y == 10:
        y = 9
    elif random.random() < 0.5:
        if random.random() <= p(y+1) / p(y):
            y += 1
    else:
        if random.random() <= p(y-1) / p(y):
            y -= 1

    if i >= 1000:
        y_ls[y] += 1

y = [i for i in range(11)]
plt.bar(y, y_ls)
plt.show()

#3.5

z = 5
z_ls = [0 for _ in range(11)]

for i in range(11000):
  
    if z == 0:
        z = 1   
    elif z == 10:
        z = 9
    elif random.random() < 0.5:
        if random.random() <= p(z+1) / p(z):
            z += 1
    else:
        if random.random() <= p(z-1) / p(z):
            z -= 1

    if i >= 1000:
        z_ls[z] += 1

z = [i for i in range(11)]
plt.bar(z, z_ls)
plt.show()