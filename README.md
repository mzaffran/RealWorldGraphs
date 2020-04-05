# RealWorldGraphs

Project for the class "Operational Research and Massive Data", SOD322, [final year  at ENSTA Paris](https://perso.ensta-paris.fr/~pcarpent/SOD/), [M2 MPRO](https://uma.ensta-paris.fr/mpro/) and [M2 Data Science](https://datascience-x-master-paris-saclay.fr/). Part of the lecture given by Maximilien Danisch.

_Authors: Walid Makhlouf and Margaux Zaffran_

## References

The various programs loading graph as text file and putting them in appropriated structures (```adjarray.c``` and ```edgelist.c```) are widely inspired by Maximilien Danisch codes, available on [his github](https://github.com/maxdan94/LoadGraph).

## Scripts

For simplicity, we will assume in the following that your graph is stored in ```graph.txt```. This file can have two structures:
- each line is of the form: "u v" which indicates that a link exists between nodes u and v ;
- same, but containing at the very beginning a header of the form
```# Undirected graph: ../jouet.txt
# Jouet
# Nodes: 9 Edges: 13
# FromNodeId	ToNodeId
```

For every script listed below, all the results, as well as the execution times, will be printed out in the terminal.

- edgelist_main.c: once compiled (edgelist_main.out for example), execute in your terminal ```./edgelist_main.out graph.txt 1``` if your graph has an header, ```./edgelist_main.out graph.txt 0``` otherwise. The program will create the adjacency array of this graph and display the number of nodes and edges ;
- adjarray_main.c: once compiled (adjarray_main.out for example), execute in your terminal ```./adjarray_main.out graph.txt 1``` if your graph has an header, ```./adjarray_main.out graph.txt 0``` otherwise. The program will clean the graph (removing selfloops, removing isolated nodes, removing duplication and ordering the result), create the adjacency array of this graph and display the number of nodes and edges ;
- adjmatrix.c: once compiled (adjmatrix.out for example), execute in your terminal ```./adjmatrix.out graph.txt 1``` if your graph has an header, ```./adjmatrix.out graph.txt 0``` otherwise. The program will create the adjacency array of this graph and display the number of nodes and edges ;
- bfs.c: once compiled (bfs.out for example), execute in your terminal ```./bfs.out graph.txt 1``` if your graph has an header, ```./bfs.out graph.txt 0``` otherwise. The program will clean the graph (removing selfloops, removing isolated nodes, removing duplication and ordering the result, undirected), count the connected components and their number of elements, and look for a lower bound of the graph diameter ;
- triangles.c: once compiled (bfs.out for example), execute in your terminal ```./triangles.out graph.txt 1``` if your graph has an header, ```./triangles.out graph.txt 0``` otherwise. The program will clean the graph (removing selfloops, removing isolated nodes, removing duplication and ordering the result, directed), count the number of triangles in the resulting graph.
- pagerank.c
- kcore.c
- community.c

## Graphs

The graphs on which our algorithms were tested are the following:
- http://snap.stanford.edu/data/com-Amazon.html ;
- http://snap.stanford.edu/data/email-Eu-core.html ;
- http://snap.stanford.edu/data/com-LiveJournal.html ;
- http://snap.stanford.edu/data/com-Orkut.html ;
- http://cfinder.org/wiki/?n=Main.Data#toc1 (```dirLinks.zip``` and ```pageNum2Name.zip```) ;
- https://drive.google.com/file/d/0B6cGK503Ibt0dXA3Z2lJcHlLX28/view.
