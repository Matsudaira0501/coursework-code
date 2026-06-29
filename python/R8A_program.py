def display(A, T):
    print("A=")
    for j in range(len(A)):
        print(A[j])
    print("T={}".format(T))

def proc(A, T):
    m = 1
    J = len(A)
    I = len(A[0])
    for j in range(J):
        for i in range(I):
            a1 = A[j][i-1] if i > 0 else 0
            a2 = A[j-1][i-1] if j > 0 and i > 0 else 0
            a3 = A[j-1][i] if j > 0 else 0
            a4 = A[j-1][i+1] if j > 0 and i < I - 1 else 0
            if A[j][i] != 0:
                if a3 != 0:
                    A[j][i] = a3
                elif a1 != 0:
                    A[j][i] = a1
                    if a4 != 0:
                        update(T, a1, a4)
                elif a2 != 0:              # 問5 ここから
                    A[j][i] = a2
                    if a4 != 0:
                        update(T, a2, a4)
                elif a4 != 0:
                    A[j][i] = a4           # ここまでコメントアウト
                else:
                    A[j][i] = m
                    T[m] = [m, -1, m]
                    m = m + 1

        display(A, T)

    if False: # 問4,5 FalseをTrueに
        for j in range(J):
            for i in range(I):
                A[j][i] = T[A[j][i]][0]

    display(A, T)

def update(T, u, v):
    if T[u][0] != T[v][0]:                    # 問１
        if T[u][0] < T[v][0]:                 # ア: (1) T[u][0] < T[v][0]
            pu, pv = T[u][0], T[v][0]         # イ: (6) pu, ウ: (9) T[u][0]
        else:
            pu, pv = T[v][0], T[u][0]         # エ: (6) pu, オ: (10) T[v][0]
            
        T[T[pu][2]][1] = pv
        T[pu][2] = T[pv][2]
        while pv != -1:
            T[pv][0] = T[pu][0]
            pv = T[pv][1]

A = [[3, 0, 1, 0],
     [2, 5, 1, 0],
     [0, 0, 0, 9],
     [2, 0, 4, 0],
     [6, 1, 8, 3]]
T = {0:[0,-1,0]}

proc(A,T)

# 問2
#
#A=
#[1, 0, 2, 0]
#[1, 1, 2, 0]
#[0, 0, 0, 9]
#[2, 0, 4, 0]
#[6, 1, 8, 3]
#T={0: [0, -1, 0], 1: [1, 2, 2], 2: [1, -1, 2]}

# 問3
#
#A=
#[1, 0, 2, 0]
#[1, 1, 2, 0]
#[0, 0, 0, 2]
#[3, 0, 2, 0]
#[3, 3, 2, 2]
#T={0: [0, -1, 0], 1: [1, 2, 3], 2: [1, 3, 2], 3: [1, -1, 3]}

# 問4
#
#A=
#[1, 0, 1, 0]
#[1, 1, 1, 0]
#[0, 0, 0, 1]
#[1, 0, 1, 0]
#[1, 1, 1, 1]

# 問5
#
#A=
#[1, 0, 1, 0]
#[1, 1, 1, 0]
#[0, 0, 0, 3]
#[4, 0, 4, 0]
#[4, 4, 4, 4]
#T={0: [0, -1, 0], 1: [1, 2, 2], 2: [1, -1, 2], 3: [3, -1, 3], 4: [4, 5, 5], 5: [4, -1, 5]}