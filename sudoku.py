from ortools.sat.python import cp_model

def cross(A, B):
    "Cross product of elements in A and elements in B."
    return [(a,b) for a in A for b in B]

def var_from_domain(model, name, domain):
    domain = cp_model.Domain.FromIntervals([[i] for i in domain])
    val = model.NewIntVarFromDomain(domain, name)
    return val

def from_file(filename, sep='\n'):
    "Parse a file into a list of strings, separated by sep."
    with open(filename,"r") as f:
        s=f.read()
    return s.strip().split(sep)

def grid_values(grid):
    "Convert grid into a dict of {square: char} with '0' or '.' for empties."
    ints = [int(c) if c in digits else 0 for c in grid ]
    assert len(ints) == 81
    return {k:v for k,v in dict(list(zip(squares, ints))).items() if v!=0}

def solve(values):
    # initialize constraint programming model
    model = cp_model.CpModel()

    # initialize variable domains
    x=[[var_from_domain(model, 'x'+str(i+1)+str(j+1), range(1,10)) for j in range(9)] for i in range(9)]


    # shrink domain w.r.t. the assignment in the grid
    for k,v in values.items():
        x[k[0]][k[1]] = var_from_domain(model, 'x'+str(k[0]+1)+str(k[1]+1), [v])

    # initialize a list of units, every square in a unit are all diff
    unitlist = []
    # column units
    unitlist += [[x[i][j] for i in range(9)] for j in range(9)]
    # row units
    unitlist += [[x[i][j] for j in range(9)] for i in range(9)]
    # box units
    unitlist += [[x[i//3*3+j//3][i%3*3+j%3] for j in range(9)] for i in range(9)]

    # add all diff constraint to every unit
    for i in unitlist:
        model.AddAllDifferent(i)

    # initialize a solver
    solver=cp_model.CpSolver()

    # solve the CSP
    status = solver.Solve(model)

    # get the solution
    solution = [[solver.Value(x[i][j]) for j in range(9)] for i in range(9)]

    # print the solution
    print('\n'.join([''.join(['{:2}'.format(item) for item in row])
          for row in solution]))
    print('\n')


digits   = '123456789'
rows     = [i for i in range(9)]
cols     = [i for i in range(9)]
squares  = cross(rows, cols)

# read one grid
value_dics = [grid_values(grid) for grid in from_file("top95.txt")]
for i in value_dics:
    solve(i)
