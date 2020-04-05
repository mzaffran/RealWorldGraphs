# -*- coding: utf-8 -*-
"""
Created on Wed Apr  1 10:11:00 2020

@author: walid
"""
import matplotlib.pyplot as plt
import numpy as np
import networkx as nx

number=1
p=0.1
q=0.01

G = nx.Graph()


color_map=[]
#color_map2=[]
colors=["red","orange","cyan","green"]
n=400
for i in range(n):
    
    G.add_node(i)
    color_map.append(colors[i//100])

for i in range(n-1):
    for j in range(i+1,n):
        x=np.random.rand()
        if i//100==j//100 and x<p:
            G.add_edge(i,j)
        #color_map2.append("green")
        elif i//100!=j//100 and x<q:
            G.add_edge(i,j)
            #color_map2.append("red")

positions=dict()
for i in range(100):
    positions[i]=(i//10+np.random.rand()/3,i%10+np.random.rand()/3)

for i in range(100):

    positions[100+i]=(12+(i//10)+np.random.rand()/3,i%10+np.random.rand()/3)
for i in range(100):

    positions[200+i]=(i//10+np.random.rand()/3,12+(i%10)+np.random.rand()/3) 
for i in range(100):

    positions[300+i]=(12+(i//10)+np.random.rand()/3,12+(i%10)+np.random.rand()/3) 
    

plt.figure()
nx.draw(G, node_size=10,node_color=color_map, pos=positions)


#nx.write_edgelist(G, str(number)+"p="+str(p)+"q="+str(q)+".txt",data=False)

