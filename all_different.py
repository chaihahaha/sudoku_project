import networkx as nx
import matplotlib.pyplot as plt
from networkx.algorithms.bipartite.matching import hopcroft_karp_matching
def cross(A, B):
    "Cross product of elements in A and elements in B."
    return [(a,b) for a in A for b in B]

def from_file(filename, sep='\n'):
    "Parse a file into a list of strings, separated by sep."
    with open(filename,"r") as f:
        s=f.read()
    return s.strip().split(sep)

def grid_values(grid):
    "Convert grid into a dict of {square: char} with '0' or '.' for empties."
    ints = [c if c in digits else 0 for c in grid ]
    assert len(ints) == 81
    return {k:v for k,v in dict(list(zip(squares, ints))).items() if v!=0}

def filtering(GG, u):
    v = set()
    for i in unitlist[u]:
        if not set(GG[i]):
            return False
        v |= set(GG[i])
    G = GG.subgraph(unitlist[u]+list(v))
    max_matching=hopcroft_karp_matching(G, unitlist[u])
    max_matching=[(k,v) for k,v in max_matching.items()]
    v2x = [p if len(p[1])==2 else p[::-1] for p in list(map(tuple, set(map(frozenset,G.edges)) - set(map(frozenset,max_matching))))]
    x2v = [p if len(p[0])==2 else p[::-1] for p in max_matching]
    assert {i for p in G.edges for i in p}==set(G.nodes)
    GM=nx.DiGraph(v2x+x2v)
    used=set(map(frozenset,max_matching))
    for i in nx.strongly_connected_components(GM):
        used |= set(map(frozenset,GM.subgraph(i).edges))
    m_free = list(set(G.nodes)-{i for p in max_matching for i in p })
    if m_free:
        for i in nx.bfs_successors(GM,m_free[0]):
            for j in i[1]:
                used.add(frozenset({i[0],j}))
    unused = list(map(tuple, set(map(frozenset,G.edges)) - used))
    GG.remove_edges_from(unused)

    affected_units = set()
    for e in unused:
        affected_units |= units[e[0] if len(e[0])==2 else e[1]]
    for unit in list(affected_units - {u}):
        if not filtering(GG, unit):
            return False
    return True

def full_filtering(G):
    for i in range(len(unitlist)):
        if not filtering(G,i):
            return False
    return True


def solved(G):
    for i in range(9):
        for j in range(9):
            if len(G[(i,j)])!=1:
                return False
    return True

def assign(G, var, value):
    G.remove_edges_from([e for e in G.edges(var) if value not in e])

def min_domain_variable(G):
    min_domain=10
    for i in range(9):
        for j in range(9):
            if len(G[(i,j)])>1 and len(G[(i,j)])<min_domain:
                min_domain = len(G[(i,j)])
                min_var = (i,j)
    return min_var
def propagate_recursive(G):
    if not full_filtering(G):
        return False
    if solved(G):
        return G
    var = min_domain_variable(G)
    values = list(G[var])
    for v in values:
        G_cp = G.copy()
        assign(G_cp, var, v)
        sol = propagate_recursive(G_cp)
        if sol:
            return sol
    return False;

def solve(value):
    d = {}
    for i in range(9):
        for j in range(9):
            if (i,j) in value.keys():
                d[(i,j)] = value[(i,j)]
            else:
                d[(i,j)] = digits
    edges=[(k,v)  for k in d.keys() for v in d[k]]
    G=nx.Graph(edges)
    G= propagate_recursive(G)
    if G:
        print(solved(G))
        for i in range(9):
            for j in range(9):
                print(list(G[(i,j)])[0],end=" ")
            print()
    else:
        print("Fail")

digits   = '123456789'
rows     = [i for i in range(9)]
cols     = [i for i in range(9)]
# index pair(position) to the variable x with domain of the square
squares  = cross(rows, cols)
# initialize a list of units, every square in a unit are all diff
unitlist = []
# column units
unitlist += [[(i,j) for i in range(9)] for j in range(9)]
# row units
unitlist += [[(i,j) for j in range(9)] for i in range(9)]
# box units
unitlist += [[(i//3*3+j//3,i%3*3+j%3) for j in range(9)] for i in range(9)]

units = {(i,j):set() for i in range(9) for j in range(9)}
for u in range(len(unitlist)):
    for e in unitlist[u]:
        units[e].add(u)
# read the grids as dictionarys from index pair to assigned digit
value_dics = [grid_values(grid) for grid in from_file("top95.txt")]
for value in value_dics:
    for i in range(9):
        for j in range(9):
            if (i,j) in value.keys():
                print(value[(i,j)],end=" ")
            else:
                print(".", end=" ")
        print()
    solve(value)
