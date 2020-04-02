# -*- coding: utf-8 -*-
"""
Created on Wed Apr  1 10:11:00 2020

@author: walid
"""
import matplotlib.pyplot as plt
import numpy as np
import networkx as nx

number=10
p=0.9
q=0.5

# networkx graph
G = nx.Graph()
# ad edges with red color
#G.add_edge(1, 2, color='red')
#G.add_edge(2, 3, color='red')
# add nodes 3 and 4
#G.add_node(3)
#G.add_node(4)

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
#edge_colors=range(len(G.edges))     
#print("edges")
#print(list(G.edges(data=True)))
#print("default graph attributes")
#print(G.graph)
#print("node node attributes")
#print(G.node)
positions=dict()
for i in range(100):
    positions[i]=(i//10+np.random.rand()/3,i%10+np.random.rand()/3)

for i in range(100):

    positions[100+i]=(12+(i//10)+np.random.rand()/3,i%10+np.random.rand()/3)
for i in range(100):

    positions[200+i]=(i//10+np.random.rand()/3,12+(i%10)+np.random.rand()/3) 
for i in range(100):

    positions[300+i]=(12+(i//10)+np.random.rand()/3,12+(i%10)+np.random.rand()/3) 
    
#for i in range(len(positions)):
#    positions[i+1]=(positions[i+1][0]/10,positions[i+1][1]/10)

biparti=[i for i in range(200)]
shells=[[i+100*j for i in range(100)] for j in range(4)]
nodesize=[[10 for i in range(100)]+[20 for i in range(100)]+[30 for i in range(100)]+[40 for i in range(100)]]
#nx.draw(G, node_size=nodesize,node_color=color_map, pos=nx.shell_layout(G,nlist=shells))

plt.figure()
nx.draw(G, node_size=10,node_color=color_map, pos=positions)


nx.write_edgelist(G, str(number)+"p="+str(p)+"q="+str(q)+".txt",data=False)

#rect=[0,0,20,20]
#fig=matplotlib.figure.Figure(figsize=(20,20))
#fig.add_axes(rect)
#
#matplotlib.pyplot.plot(fig)
#matplotlib.pyplot.show()
#axe= matplotlib.axes.Axes(fig,rect,frameon=True)
#nx.draw(G, node_size=10,node_color=color_map, pos=positions,ax=axe)

#nx.draw(G, node_size=nodesize,node_color=color_map, pos=nx.bipartite_layout(G,nodes=biparti))