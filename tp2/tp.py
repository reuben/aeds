#!/usr/bin/env python3
from functools import reduce, lru_cache

text = "1 2 3 1"
words = text.split(" ")
n = len(words)
H = 2

Infinity = float('inf')

def badness(i, j):
    cost = reduce(lambda x, y: x + int(y), words[i:j], 0)
    print("badness({}, {}) = {}".format(i, j, cost))
    return cost

parent = {n: None}
# @lru_cache(maxsize=None)
def DP(i, lines):
    if i == n:
        return 0 if lines == H else Infinity
    cost, argmin = min((badness(i, j) + DP(j, lines+1), j) for j in range(i+1, n+1))
    parent[i] = argmin
    return cost

DP_data = {}
def DP_bottomup():
    for lines in range(0, H+1):
        DP_data[(n, lines)] = (k * pow(H - lines, X), None)

    for i in range(0, n+1):
        DP_data[(i, H)] = (0, None)

    for i in range(n-1, -1, -1):
        for lines in range(H-1, -1, -1):
            DP_data[(i, lines)] = min((badness(i, j) + DP_data[(j, lines+1)][0], j) for j in range(i+1, n+1))

def greedy():
    copy = words[::-1]
    text = []
    cost = 0
    while len(copy) > 0:
        line = ""
        while len(copy) > 0 and len(line)+len(copy[-1]) < L:
            line += copy.pop() + " "
        line = line[:-1]
        text.append(line)
        cost += k * pow(L - len(line), X)
    cost += k * pow(H - len(text), X)
    print(cost)
    print("\n".join(text))


DP(0, 0)
i = 0
while i is not None:
    print(" ".join(words[i:parent[i]]))
    i = parent[i]

# greedy()

# DP_bottomup()
# print(DP_data[(0, 0)][0])
# i = 0
# line = 0
# while i is not None:
#     print(i)
#     # print(" ".join(words[i:DP_data[(i, line)][1]]))
#     i = DP_data[(i, line)][1]
#     line = line+1
