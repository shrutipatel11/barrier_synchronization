from mpl_toolkits import mplot3d
import numpy as np
import matplotlib.pyplot as plt
import numpy as np

fig = plt.figure()
ax = plt.axes(projection='3d')
X = [2,
4,
6,
8,
10,
12,
2,
4,
6,
8,
2,
4,
6,
2,
3,
4,
5,
2,
3,
4,
2,
3,
4,
2,
3]
Y = [2,
2,
2,
2,
2,
2,
3,
3,
3,
3,
4,
4,
4,
5,
5,
5,
5,
6,
6,
6,
7,
7,
7,
8,
8]
Z = [1,
2,
2.5,
4,
2.5,
3,
5.33,
5,
7.33,
5,
5.5,
4.5,
5.75,
8,
8.2,
9,
7.2,
8,
7.67,
8,
8.85,
9,
8.42,
8,
7.75]
# ax.plot_surface(np.array(X), np.array(Y), np.array(Z), rstride=1, cstride=1,cmap='viridis', edgecolor='none')
ax.plot_trisurf(X, Y, Z, linewidth=0, antialiased=False)
ax.set_title('surface')
plt.show()
