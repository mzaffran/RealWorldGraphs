import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.cm import rainbow
from matplotlib.colors import LogNorm

from matplotlib import rc

path = 'results/kcore/scholar/'
df = pd.read_csv(path+'net_kcore.txt', delimiter=';')

df.shape
df = df.iloc[:,:3]
df.tail(10)
df.head()
# df.sort_values(['Degree','Coreness'], inplace=True)
df['Color'] = df.groupby(['Degree','Coreness'])['ID'].transform('count').values


fig, ax = plt.subplots(figsize=(10,8))
cm = plt.cm.get_cmap('jet')
# sc = ax.scatter(x=df.Degree, y=df.Coreness, c=df.Color, vmin=0, vmax=20, s=35, cmap=cm, norm=LogNorm())
sc = ax.scatter(x=df.Degree, y=df.Coreness, c=df.Color, s=35, cmap=cm, norm=LogNorm())
clb = plt.colorbar(sc)
clb.ax.set_xlabel('Occurencies')
plt.xscale('log')
plt.yscale('log')
plt.xlim((0.9,110))
plt.ylim((0.9,110))
ax.set(xlabel='Degree', ylabel='Coreness', title='Google Scholar citations')
plt.plot(range(101), range(101), 'k-')
plt.savefig(path+'visu.png')
plt.show()


maxID = df[df.Coreness==df.Coreness.max()].ID.values
len(maxID)
# listID = pd.read_csv('ID.txt', delimiter=' ')
