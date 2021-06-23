# 关键路径
# 先拓扑排序, 即 DFS 的完成序的反向
# 按拓扑排序访问每个顶点, 并以负值松弛相邻的边
# 找到负值最小的路径, 即为最长路径

from math import inf

read = lambda: map(int, input().split(' '))

V, E = read()
graph = [[] for v in range(V+1)] # 下标 0 空出不用
visited = [False for v in range(V+1)]
dist = [-inf for v in range(V+1)] # 求最长路, 初值应为 -inf
dist[1] = 0
topo = []

while E:
    E -= 1
    u, v, w = read()
    graph[u].append((v, w))

#print(graph)

def relax(u, v, w):
    if dist[v] < dist[u] + w:
        #print(dist)
        dist[v] = dist[u] + w

def dfs(u):
    visited[u] = True
    for v in graph[u]:
        if not visited[v[0]]:
            dfs(v[0])
    topo.append(u) # DFS 完成序是拓扑序反向

dfs(1)
for u in reversed(topo):
    for v in graph[u]:
        relax(u, v[0], v[1])
print(dist[V] if dist[V] > -inf else -1)
