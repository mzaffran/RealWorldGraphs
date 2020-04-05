# RealWorldGraphs

Project for the class "Operational Research and Massive Data", SOD322, [final year  at ENSTA Paris](https://perso.ensta-paris.fr/~pcarpent/SOD/), [M2 MPRO](https://uma.ensta-paris.fr/mpro/) and [M2 Data Science](https://datascience-x-master-paris-saclay.fr/). Part of the lecture given by Maximilien Danisch.

_Authors: Walid Makhlouf and Margaux Zaffran_

## References

The various programs loading graph as text file and putting them in appropriated structures (```adjarray.c``` and ```edgelist.c```) are widely inspired by Maximilien Danisch codes, available on [his github](https://github.com/maxdan94/LoadGraph).

## Scripts

For simplicity, we will assume in the following that your graph is stored in ```graph.txt```. This file can have two structures:
- each line is of the form: "u v" which indicates that a link exists between nodes u and v ;
- same, but containing at the very beginning a header of the form
```# Undirected graph: ../graph.txt
# Graph
# Nodes: nb_node Edges: nb_edges
# FromNodeId	ToNodeId
```

For every script listed below, all the results, as well as the execution times, will be printed out in the terminal.

- **edgelist_main.c**: once compiled (edgelist_main.out for example), execute in your terminal ```./edgelist_main.out graph.txt 1``` if your graph has a header, ```./edgelist_main.out graph.txt 0``` otherwise. The program will create the adjacency array of this graph and display the number of nodes and edges ;
- **adjarray_main.c**: once compiled (adjarray_main.out for example), execute in your terminal ```./adjarray_main.out graph.txt 1``` if your graph has a header, ```./adjarray_main.out graph.txt 0``` otherwise. The program will clean the graph (removing selfloops, removing isolated nodes, removing duplication and ordering the result), create the adjacency array of this graph and display the number of nodes and edges ;
- **adjmatrix.c**: once compiled (adjmatrix.out for example), execute in your terminal ```./adjmatrix.out graph.txt 1``` if your graph has a header, ```./adjmatrix.out graph.txt 0``` otherwise. The program will create the adjacency array of this graph and display the number of nodes and edges ;
- **bfs.c**: once compiled (bfs.out for example), execute in your terminal ```./bfs.out graph.txt 1 maxtime``` if your graph has a header, ```./bfs.out graph.txt 0 maxtime``` otherwise. The program will clean the graph (removing selfloops, removing isolated nodes, removing duplication and ordering the result, undirected), count the connected components and their number of elements, and look (within maxtime, expressed in seconds) for a lower bound of the graph diameter  ;
- **triangles.c**: once compiled (triangles.out for example), execute in your terminal ```./triangles.out graph.txt 1``` if your graph has a header, ```./triangles.out graph.txt 0``` otherwise. The program will clean the graph (removing selfloops, removing isolated nodes, removing duplication and ordering the result, directed), count the number of triangles in the resulting graph ;
- **pagerank.c**: once compiled (pagerank.out for example), execute in your terminal ```./pagerank.out graph.txt names_node.txt alpha 1``` [replacing alpha by the desired value of alpha for pagerank algorithm] if you do not wish to take into account isolated nodes, but if you wish to, then replace 1 by 0 (here, your graph shall have a header similar to the one of the ```dirLinks.txt``` file available [here](http://cfinder.org/wiki/?n=Main.Data#toc1 ), same for ```names_node.txt```). This program will run the pagerank algorithm for the value of alpha on your graph and display the 5 highest pages and 5 lowest with their names, and store the entire results in the ```results/pagerank``` folder (have it created if you did not clone the repository!) in a file named ```alpha_pagerank.txt``` ;
- **kcore.c**: once compiled (kcore.out for example), execute in your terminal ```./kcore.out graph.txt 1``` if your graph has a header, ```./kcore.out graph.txt 0``` otherwise. The program will calculate the coreness of each node and display the core-value of the graph. The corenesses and degrees of all the nodes will then be saved in the ```results/kcore``` folder (have it created if you did not clone the repository!) in a file named ```graph_kcore.txt``` ;
- **community.c**: once compiled (community.out for example), execute in your terminal ```./community.out graph.txt labels.txt 1``` if your graph has a header, ```./community.out graph.txt labels.txt 0``` otherwise. The program will load the graph then run a label propagation algorithm to detect communities within the graph. At the end, the file ```labels.txt``` will be created (or overwritten) and will contain for each node the label that it received. Labels are numbered from 0 up to the number of labels found minus one ;
- **compare.c**: once compiled (compare.out for example), execute in your terminal ```./compare.out labels1.txt labels2.txt```. The program will load the labels found in each file (```labels1.txt```, ```labels2.txt```), compute and display the MI, NMI (Normalized Mutual Information) and ARI (Adjusted Rand Index) of the two partitions. Note that for each file the labels must be comprised between 1 and the total number of labels in the considered partition.

The following files do not contain any main but are instead necessary for some of the previous files:
- **adjarray.c**, necessary for ```adjarray_main.c```, ```bfs.c```, ```triangles.c```, ```kcore.c``` and ```community.c```.
- **edgelist.c**, necessary for ```edgelist_main.c``` and ```pagerank.c```.
- **utility.c**, necessary for all of them except ```compare.c```

The following scripts are in python:
- **kcore_visualization.py** allows to draw a scatter plot of corenesses versus degrees, with intensity ;
- **generategraph.py** allows to generate graphs with known communities ;
- **readgraph.py** allows to display some graphs with known communities;
- **pagerank_visualization.py** allows to draw a scatter plot of PageRank algorithm with different alpha;

## Graphs

The graphs on which our algorithms were tested are the following:
- http://snap.stanford.edu/data/com-Amazon.html ;
- http://snap.stanford.edu/data/email-Eu-core.html ;
- http://snap.stanford.edu/data/com-LiveJournal.html ;
- http://snap.stanford.edu/data/com-Orkut.html ;
- http://cfinder.org/wiki/?n=Main.Data#toc1 (```dirLinks.zip``` and ```pageNum2Name.zip```) ;
- https://drive.google.com/file/d/0B6cGK503Ibt0dXA3Z2lJcHlLX28/view.
