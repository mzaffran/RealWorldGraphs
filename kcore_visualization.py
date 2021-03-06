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
maxID
#Sa-kwang Song
#Sung-Pil Choi
#Chang-Hoo Jeong
#Yun-soo Choi
#Hong-Woo Chun
#Jinhyung Kim
#Hanmin Jung
#Do-Heon Jeong
#Myunggwon Hwang
#Won-Kyung Sung
#Hwamook Yoon
#Minho Lee
#Won-Goo Lee
#Jung Ho Um
#Dongmin Seo
#Mi-Nyeong Hwang
#Sung J. Jung
#Minhee Cho
#Sungho Shin
#Seungwoo Lee
#Heekwan Koo
#Jinhee Lee
#Taehong Kim
#Mikyoung Lee
#Ha-neul Yeom
#Seungkyun Hong
#Yun-ji Jang

df[(df.Coreness==6) & (df.Degree==df[df.Coreness==6].Degree.max())]
#Ali Khademhosseini


df[(df.Coreness==8) & (df.Degree==df[df.Coreness==8].Degree.max())]

df[(df.Coreness==9) & (df.Degree==df[df.Coreness==9].Degree.max())]

# listID = pd.read_csv('ID.txt', delimiter=' ')
