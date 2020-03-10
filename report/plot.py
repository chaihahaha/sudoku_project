import matplotlib.pyplot as plt
with open("time_c.txt","r") as f:
    s=f.read()
d_c=[float(i) for i in s.split("\n") if i]
with open("time_solver.txt","r") as f:
    s=f.read()
d_s=[float(i) for i in s.split("\n") if i]
with open("time_pyalldiff.txt","r") as f:
    s=f.read()
d_p=[float(i) for i in s.split("\n") if i]
with open("time_c_without_OI2.txt","r") as f:
    s=f.read()
d_w=[float(i) for i in s.split("\n") if i]

fig,ax = plt.subplots()
ax.scatter(d_c,d_s, marker='.',c='k',s=10)
ax.set_title('C version to solver')
ax.set_xlabel('time by second (C version)')
ax.set_ylabel('time by second (solver)')
ax.set_ylim(ymin=0)
ax.set_xlim(xmin=0)
fig.savefig("img/c2solver.png")

fig,ax = plt.subplots()
ax.scatter(d_c,d_p, marker='.',c='k',s=10)
ax.set_title('C version to Python version')
ax.set_xlabel('time by second (C version)')
ax.set_ylabel('time by second (Python version)')
ax.set_ylim(ymin=0)
ax.set_xlim(xmin=0)
fig.savefig("img/c2p.png")

fig,ax = plt.subplots()
ax.scatter(d_s,d_p, marker='.',c='k',s=10)
ax.set_title('Solver to Python version')
ax.set_xlabel('time by second (solver)')
ax.set_ylabel('time by second (Python version)')
ax.set_ylim(ymin=0)
ax.set_xlim(xmin=0)
fig.savefig("img/solver2p.png")

fig,ax = plt.subplots()
ax.scatter(d_c,d_w, marker='.',c='k',s=10)
ax.set_title('With and without OI2')
ax.set_xlabel('time by second (with OI2)')
ax.set_ylabel('time by second (without OI2)')
ax.set_ylim(ymin=0)
ax.set_xlim(xmin=0)
fig.savefig("img/c2w.png")
