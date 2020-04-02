# -*- coding: utf-8 -*-
"""
Created on Wed Apr  1 13:41:34 2020

@author: walid
"""

import matplotlib.pyplot as plt

import networkx as nx

number=6
p=0.1
q=0.001
file=str(number)+"p="+str(p)+"q="+str(q)+".txt"
n=400

G = nx.read_edgelist(file, nodetype=int)
#G.nodes=[i for i in range(n)]


color_map=[]

colors=["red","orange","cyan","green"]
shapes=['o','s','v','p']
nodeshapes=[]

nodess=list(G)
for i in nodess:
    
    #nodeshapes.append(shapes[nodess[i]//100])
    color_map.append(colors[i//100])


           

positions=dict()
#for i in nodess:
#    if i<100:
#        positions[i]=(i//10+np.random.rand()/3,i%10+np.random.rand()/3)
#    elif i<200:
#        positions[i]=(12+((i-100)//10)+np.random.rand()/3,(i-100)%10+np.random.rand()/3)
#    elif i<300:
#        positions[i]=((i-200)//10+np.random.rand()/3,12+((i-200)%10)+np.random.rand()/3) 
#    elif i<400:
#        positions[i]=(12+((i-300)//10)+np.random.rand()/3,12+((i-300)%10)+np.random.rand()/3) 

for i in range(100):   
     positions[i]=(i//10+np.random.rand()/3,i%10+np.random.rand()/3)
for i in range(100):

    positions[100+i]=(12+(i//10)+np.random.rand()/3,i%10+np.random.rand()/3)
for i in range(100):

    positions[200+i]=(i//10+np.random.rand()/3,12+(i%10)+np.random.rand()/3) 
for i in range(100):

    positions[300+i]=(12+(i//10)+np.random.rand()/3,12+(i%10)+np.random.rand()/3) 

nx.draw(G, node_size=10,node_color=color_map, pos=positions)



