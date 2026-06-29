import matplotlib.pyplot as plt
import random
import math

def coin_num():
    c = 0
    for _ in range(10):
        c += round(random.random())
    return c

def p(n):
    return math.comb(10,n)/ (2 ** 10)

if __name__ == "__main__":

    ls = [0 for _ in range(11)]
    for _ in range(10000):
        ls[coin_num()] += 1
    
    x = [i for i in range(11)]
    plt.bar(x, ls)
    plt.plot(x, [p(n)*10000 for n in x], c="r")
    plt.show()

