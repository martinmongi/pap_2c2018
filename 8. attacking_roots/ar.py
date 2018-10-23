import copy
from collections import deque, defaultdict

def bfs(g, s, t, rc):
    visited = [False for i in rc]
    prev = [None for i in rc]
    q = deque([s])
    while len(q) > 0:
        v = q.popleft()
        # print(v)
        visited[v] = True
        for v2 in rc[v]:
            if (not visited[v2]):
                # print(v,"->",v2)
                q.append(v2)
                prev[v2] = v
    
    # print(prev)
    if prev[t] == None:
        return None
    v = t
    path = []
    while prev[v] != None:
        # print(v, prev[v])
        path.append(v)
        v = prev[v]
        # print(v, prev[v])
    path.append(s)
    path.reverse()
    return path
             



def max_flow(graph, source, sink):
    rc = copy.deepcopy(graph)
    flow = 0
    while True:
        path = bfs(graph, source, sink, rc)
        # print("Path:",path)
        if not path:
            return flow
        flow += 1
        for i in range(0, len(path)-1):
            rc[path[i]].remove(path[i+1])
            rc[path[i+1]].add(path[i])
    return flow
    

while True:
    try:
        n = input()
        n = int(n)
        board = []

        for i in range(n):
            board.append(input().rstrip())
        # print(board)
        rows = []
        cols = []
        for i in range(n):
            row = []
            for j in range(n):
                if board[i][j] == 'X':
                    if row != []:
                        rows.append(row)
                        row = []
                else:
                    row.append((i,j))
            if row != []:
                rows.append(row)

        for j in range(n):
            col = []
            for i in range(n):
                if board[i][j] == 'X':
                    if col != []:
                        cols.append(col)
                        col = []
                else:
                    col.append((i,j))
            if col != []:
                cols.append(col)


        row_vertices = {i:rows[i] for i in range(len(rows))}
        col_vertices = {i+len(row_vertices):cols[i] for i in range(len(cols))}

        # print(row_vertices)
        # print(col_vertices)
        vcount = len(row_vertices) + len(col_vertices)
        
        sq_to_row = {i: k for k, v in row_vertices.items() for i in v}
        sq_to_col = {i: k for k, v in col_vertices.items() for i in v}
        # print(sq_to_row)
        # print(sq_to_col)

        graph = {i:set() for i in range(vcount)}
        for i in range(n):
            for j in range(n):
                if board[i][j] != 'X':
                    graph[sq_to_row[(i,j)]].add(sq_to_col[(i,j)])
        graph[vcount] = set(i for i in row_vertices.keys())    #source
        graph[vcount+1] = set()
        for i in col_vertices.keys():
            graph[i].add(vcount+1)
        # print(graph)
        print(max_flow(graph, vcount, vcount+1))

    
        # print("==========================================================")

    except EOFError:
        break
