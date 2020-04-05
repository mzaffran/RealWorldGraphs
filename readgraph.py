# -*- coding: utf-8 -*-
"""
Created on Wed Apr  1 13:41:34 2020

@author: walid
"""

import matplotlib.pyplot as plt

import networkx as nx
import numpy as np
import pandas as pd

number=6
p=0.02
q=0.001
file=str(number)+"p="+str(p)+"q="+str(q)+".txt"
file=str(number)+".txt"
n=400
df = pd.read_csv('labels'+str(number)+'.txt', delimiter=' ',header=None)
G = nx.read_edgelist(file, nodetype=int)
#G.nodes=[i for i in range(n)]


color_map=[]

colors=["red","orange","cyan","green"]


nodess=list(G)
for i in nodess:

    #color_map.append(colors[i//100])
    color_map.append(colors[df[1][i]])

           

positions=dict()

for i in range(100):   
     positions[i]=(i//10+np.random.rand()/3,i%10+np.random.rand()/3)
for i in range(100):

    positions[100+i]=(12+(i//10)+np.random.rand()/3,i%10+np.random.rand()/3)
for i in range(100):

    positions[200+i]=(i//10+np.random.rand()/3,12+(i%10)+np.random.rand()/3) 
for i in range(100):

    positions[300+i]=(12+(i//10)+np.random.rand()/3,12+(i%10)+np.random.rand()/3) 

nx.draw(G, node_size=10,node_color=color_map, pos=positions)

plt.savefig('LPG'+str(number)+'.png')

