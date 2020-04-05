import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.cm import rainbow
from matplotlib.colors import LogNorm
from math import *
from matplotlib import rc

path = 'results/pagerank/'
df1 = pd.read_csv(path+'0.15_PageRank.txt', delimiter=';')

df3 = pd.read_csv(path+'indegree_PageRank.txt', delimiter=';')

df1 = df1.iloc[:,:2]
df3 = df3.iloc[:,:2]
df3=df3[df3.OutDegree==0]
df1=df1.iloc[df3.ID,:]

df= pd.DataFrame()

df['x']=df1.PageRank
df['y']=df3.OutDegree

fig, ax = plt.subplots(figsize=(10,8))

plt.scatter(x=np.log10(df.x), y=np.log10(df.y), s=10,  norm=LogNorm())

ax.set( xlabel='PageRank α = 0.15', ylabel='PageRank α = 0.10', title='Wikipedia PageRank')

#plt.savefig(path+'pr1.png')
plt.show()
