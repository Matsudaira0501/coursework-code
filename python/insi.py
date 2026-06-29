import numpy as np

a = np.array([1, 2, 3, 4, 5])
print(a+2) 
#[3 4 5 6 7] ok

a_1 = np.array([1, 2, 3])
b_1 = np.array([4, 5, 6])
print(a_1.dot(b_1))
#[4, 10, 18] ng
#dot関数はドット積（内積）を表す。
#32 re ok

a_2 = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]])
#a_2 = np.array([[[1,1], [2,2], [3,3]], [[4,4], [5,5], [6, 6]], [[7,7], [8,8], [9,9]]])
print(a_2[1:, 1:])
#[[5, 6], [8, 9]] ng
#[[5 6]
# [8 9]] 出力結果に","は無く多次元の場合は複数行にわたる

a_3 = np.array([[1, 2, 3], [4, 5, 6], [7, 8, 9]])
#sum = np.sum(a_3, axis=0) #列ごと　基本的に列→行の順
su = np.sum(a_3, axis=1) #行ごと
su_1 = np.sum(a_3)
print(a_3)
#print(sum[1])
print(su)
print(su_1)
#15 ok?

a_4 = np.arange(6).reshape(2, 3)
print(a_4)
#[[0 1 2]
# [3 4 5]] ok(,は不要)

def func(m, n):
    m[0] = m[0] * 3
    n = n * 3
    print(m[0], n)
a = [1]
b = 1
func(a, b)
print("a =", a[0], "b =", b)
#a (リスト): 関数にオブジェクトそのものが渡され、中身が直接変更された。
#ミュータブルなオブジェクト　リスト、辞書、集合　同じid
#b (整数): 関数には値のコピーが渡され、関数内での変更は元の変数に影響しなかった。
#イミュータブルなオブジェクト　整数、浮動小数点数、文字列、タプル　新しいid

sum = 0.0
while True:
    if sum >= 10:
        break
    else:
        sum = sum + 0.1
print(sum)
#sumが増加し10になっても、sumに格納されているのはfloat型の10.0なので、
#int型の10とは異なると判断されてしまうことがあるから。

#コンピュータが浮動小数点数（0.0や0.1のような小数のこと）を扱う際の誤差
#0.1と書いた値は、コンピュータの中では0.10000000000000001のような、ごくわずかにずれた値として扱われます。
#10.0に非常に近い値にはなりますが、決してぴったり10.0にはなりません。

x = "Hello"
print(x[0])
#x[0] = "h"
x = "h" + x[1:] #とする
print(x[0])
 

#エラーは発生しない。ng
#エラーが発生する。
#文字列はイミュータブルなオブジェクトなので、中身を変更しようとするとエラーが発生する。

x = 10
def fc():
    global x #とする
    x += 1
    print(x)

fc()

#関数に変数xが引数として渡されていないためエラーが発生する。
#関数の中で変数に値の代入（=, += など）を行うと、その変数は**関数内だけで使える「ローカル変数」**だとみなされます。
#未定義のローカル変数を参照
#関数内でグローバル変数として宣言する必要がある


for i in range(10):
    print(i)
    i = "Hello"
#発生しない　ループごとにiが更新されるから
#ループ変数に値を代入しても問題なく、型の異なるものを代入しても問題ない

def prime_numbers(n):
    primes = []
    for num in range(n, 1, -1):
        is_prime = True
        for i in range(2, num):
            if (num % i == 0): #a
                is_prime = False
                break
        if is_prime: #b
            primes.append(num)
    return primes
print(prime_numbers(20))
print(list(range(20, 1, -1)))
print(list(range(2)))
#[2, 3, 5, 7, 11, 13, 17, 19] ng
#[19, 17, 13, 11, 7, 5, 3, 2] ok
#range(start, stop, step)start ≦ i < stop（stepずつ）
#等差数列、startは含むがstopは含まない、float×(np.arangeなら可能)
#reversedによって逆順に

def g(x, y, a, b):
    m, n = x.shape #5*5
    p, q = y.shape #2*2
    for i in range(m-p+1): #0~3
        for j in range(n-q+1): #0~3
            #flag = True
            #for k in range(p): #0,1
                #for l in range(q): #0,1
                    #if abs(x[i+k, j+l] - y[k, l]) > a:
#x[0, 0]-y[0, 0]=-1 1>1f
#x[0, 1]-y[0, 1]=-1 1>1f
#x[1, 0]-y[1, 0]=-1 1>1f
#x[1, 1]-y[1, 1]=-1 1>1f
                        #flag = False
                        #break
            #if flag:
            if all(abs(x[i+k, j+l] - y[k, l]) <= a for k in range(p) for l in range(q)):
#all()は、カッコの中の全ての要素がTrueのときに限りTrueを返す関数
#ジェネレータ式と呼ばれるもので、forループを1行で表現する方法です。kとlの全ての組み合わせについて、左側にある式の評価を次々と行います。
                x[i:i+p, j:j+p] = b
#x[0:2, 0:2] = 0

    return x

x1 = np.array([[1, 2, 3, 4, 5],
               [6, 7, 8, 9, 10],
               [1, 2, 3, 4, 5],
               [6, 7, 8, 9, 10],
               [1, 2, 3, 4, 5]])

y1 = np.array([[2, 3],
               [7, 8]])

y2 = np.array([[0, 0],
               [0, 0]])
print(g(x1, y1, 1, 0))
#[[0, 0, 0, 0, 5]
# [0, 0, 0, 0, 10]
# [0, 0, 0, 0, 5]
# [0, 0, 0, 0, 10]
# [1, 2, 3, 4, 5]] ok
#変更後のx1について適用
print(g(x1, y2, 0, 1))
#[1, 1, 1, 1, 5],
# [1, 1, 1, 1, 10],
# [1, 1, 1, 1, 5],
# [1, 1, 1, 1, 10],
# [1, 2, 3, 4, 5]] reok
#print(x1[:3, 0:2])
#スライスもrangeと同様に[start:stop:step]
#マイナスで指定すると末尾からの位置

#(4)この関数は、画像（行列 x）の中から特定のパターン（テンプレート y）を見つけ出し、その部分を別のパターン（行列 b）に置き換えた画像を返します。

print(-6 // 4)
print(6 // -4)