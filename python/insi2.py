import numpy as np

def search(A, s):
    R = np.abs(A[s:, s:])
    print(R)
    print(R.shape[0])
    print(list(range(R.shape[0])))

    idx, rmin = (-1, -1), 1000000000
    for r in range(R.shape[0]):
        for c in range(R.shape[1]):
            if R[r, c] > 0 and R[r, c] < rmin:
                idx, rmin = (s + r, s + c), R[r, c] #0より大きい最小の数のインデックスと値を探す。
                print(idx, rmin)
    return idx

def check(A, s):
    tmp = A[s+1:, s+1:]
    for r in range(tmp.shape[0]):
        for c in range(tmp.shape[1]):
            if tmp[r, c] % A[s, s] != 0:
                return (s + r + 1, s + c + 1, tmp[r, c] // A[s, s])
    return (-1, 0, 0)

def proc(A, s):
    print(A)

    i, j = search(A, s)
    print(i,j)

    if i < 0:
        return A

    A[ [s, i] ] = A[ [i, s] ]  #行列Aに対してi行とs行の要素を全て入れ替える処理。
    print(A)

    A[ :, [s, j] ] = A[ :, [j, s] ]  #行列Aに対してj列とs列の要素を全て入れ替える処理。
    
    print(A)
    #0列と1列が入れ替わる。

    
    for r in range(s + 1, A.shape[0]):
        print(r)
        k = A[r, s] // A[s, s]
        print(k)
        A[r, :] -= k * A[s, :]

    print(A)

    
    for c in range(s + 1, A.shape[1]):
        print(c)
        k = A[s, c] // A[s, s]
        print(k)
        print(A[:,c])
        print(k * A[s, c])
        #A[:, c] -= k * A[s, c]
        A[:, c] -= k * A[:, s]
    
    print(A)


    if True:
        rr, cc, qq = check(A, s)
        if rr >= 0:
            A[rr, :] += A[s, :]
            A[:, cc] -= qq * A[:, s]
        else:
            if A[s, s] < 0:
                A[s, :] *= -1
            print("Aは",A)
            return proc(A, s + 1)
    return proc(A,s)

    

A = np.array([3, 1, 3, 1, -1, 3, 3, 3, 0], dtype=int).reshape(3, 3)
A = proc(A, 0)