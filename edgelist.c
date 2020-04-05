/*
Maximilien Danisch
September 2017
http://bit.ly/danisch
maximilien.danisch@gmail.com
Info:
Feel free to use these lines as you wish. This program loads a graph in main memory as a list of edges.
To compile:
"gcc edgelist.c -O9 -o edgelist".
To execute:
"./edgelist edgelist.txt".
"edgelist.txt" should contain the graph: one edge on each line (two unsigned long (nodes' ID)) separated by a space.
The prograph loads the graph in main memory and then it terminates.
Note:
If the graph is directed (and weighted) with selfloops and you want to make it undirected unweighted without selfloops, use the following linux command line.
awk '{if ($1<$2) print $1" "$2;else if ($2<$1) print $2" "$1}' net.txt | sort -n -k1,2 -u > net2.txt
Performance:
Up to 500 million edges on my laptop with 8G of RAM:
Takes more or less 1.6G of RAM and 25 seconds (I have an SSD hardrive) for 100M edges.
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>//to estimate the runing time
#include "utility.c"

#define NLINKS 100000000 //maximum number of edges for memory allocation, will increase if needed

typedef struct {
	unsigned long s;
	unsigned long t;
} edge;

//edge list structure:
typedef struct {
	unsigned long n;//number of nodes
	unsigned long e;//number of edges
	edge *edges;//list of edges
} edgelist;

//reading the edgelist from file
edgelist* readedgelist(char* input){
	unsigned long e1=NLINKS;
	FILE *file=fopen(input,"r");

	char ch[100];
  fscanf(file, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %s", ch)  ;
  // printf("%s\n",ch);
	edgelist *g=malloc(sizeof(edgelist));
	g->n=0;
	g->e=0;
	g->edges=malloc(e1*sizeof(edge));//allocate some RAM to store edges

	while (fscanf(file,"%lu %lu", &(g->edges[g->e].s), &(g->edges[g->e].t))==2) {
		g->n=max3(g->n,g->edges[g->e].s,g->edges[g->e].t);
		if (++(g->e)==e1) {//increase allocated RAM if needed
			e1+=NLINKS;
			g->edges=realloc(g->edges,e1*sizeof(edge));
		}
	}
	fclose(file);

	g->n++;

	g->edges=realloc(g->edges,g->e*sizeof(edge));

	return g;
}

void free_edgelist(edgelist *g){
	free(g->edges);
	free(g);
}

char** readpagelist(char* input, unsigned long n){
  //printf("=== Enter pagelist\n");
	FILE *file=fopen(input,"r");

	char ch[100];
	unsigned long i=0,k=0;
  fscanf(file, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %s", ch)  ;
  // printf("%s\n",ch);
	char** h=malloc(n*sizeof(char*));

	//fscanf(file,"%lu %s\n", &i, ch);
	//printf("ff %s\n", ch);

  char* word = malloc(100) ;

  while(fscanf( file, "%lu", &i)!=EOF){
                fscanf( file, " %[^\n]", word);
                h[i]=malloc(strlen(word) + 1);
                strcpy(h[i],word);
                k++;

                //if (k<=10){
                //printf("%lu %s ",i,g[i]);
                //}
  }
  printf("Number of page names found: %lu\n",k);

	fclose(file);
	//free(word);
	return h;
}

void BottomUpMerge2(unsigned long A[], unsigned long oldindices[], unsigned long iLeft, unsigned long iRight, unsigned long iEnd, unsigned long B[],unsigned long oldindices2[])
{
    unsigned long i,j,k;
    i = iLeft, j = iRight;
    // While there are elements in the left or right runs...
    for (k = iLeft; k < iEnd; k++) {
        // If left run head exists and is <= existing right run head.
        if (i < iRight && (j >= iEnd || A[i]>A[j]  )) {
            B[k] = A[i];
            oldindices2[k]=oldindices[i];
            i = i + 1;
        } else {
            B[k] = A[j];
            oldindices2[k]=oldindices[j];
            j = j + 1;
        }
    }
}

void CopyArray2(unsigned long B[], unsigned long A[], unsigned long n)
{
     unsigned long i;
    for(i = 0; i < n; i++)
        A[i] = B[i];
}
void BottomUpMergeSort2(unsigned long A[],unsigned long B[], unsigned long n , unsigned long oldindices[],unsigned long oldindices2[])
{
     unsigned long width,i;
    // Each 1-element run in A is already "sorted".
    // Make successively longer sorted runs of length 2, 4, 8, 16... until whole array is sorted.
    for (width = 1; width < n; width = 2 * width)
    {
        // Array A is full of runs of length width.
        for (i = 0; i < n; i = i + 2 * width)
        {
            // Merge two runs: A[i:i+width-1] and A[i+width:i+2*width-1] to B[]
            // or copy A[i:n-1] to B[] ( if(i+width >= n) )
            BottomUpMerge2(A, oldindices, i, min(i+width, n), min(i+2*width, n), B,oldindices2);
        }
        // Now work array B is full of runs of length 2*width.
        // Copy array B to array A for next iteration.
        // A more efficient implementation would swap the roles of A and B.
        CopyArray2(B, A, n);
        CopyArray2(oldindices2,oldindices,n);
        // Now array A is full of runs of length 2*width.
    }
}

unsigned long* renamevertices(edgelist* g){

	unsigned long i;
	unsigned long *d=calloc(g->n,sizeof(unsigned long));
  if (d==NULL)
     printf("failed to allocate d\n");
	for (i=0;i<g->e;i++)
	{
		d[g->edges[i].s]++;
		d[g->edges[i].t]++;
	}

  unsigned long *oldindices = malloc(g->n*sizeof(unsigned long));
  unsigned long *oldindices2 = malloc(g->n*sizeof(unsigned long));
  unsigned long *ordered_d =  malloc(g->n*sizeof(unsigned long));
  if (oldindices==NULL)
     printf("Failed to allocate old indices\n");
  if (oldindices2==NULL)
     printf("Failed to allocate old indices2\n");
  if (ordered_d==NULL)
     printf("Failed to allocate old ordered_d\n");
  for (i=0; i< g->n; i++)
	{
    oldindices[i]=i;
    oldindices2[i]=i;
  }
  BottomUpMergeSort2(d, ordered_d, g->n , oldindices, oldindices2);

  free(ordered_d);
  free(oldindices2);

  unsigned long isolated_nodes=0;
  i=g->n-1;

  while (d[i]==0)
	{
    isolated_nodes++;
    i--;
  }

  free(d);

  unsigned long *newindices = malloc(g->n*sizeof(unsigned long));
  if (newindices==NULL)
     printf("failed allocateion of new indices\n");

  for (i=0 ; i<g->n; i++)
	{
    newindices[oldindices[i]]=i;
  }

  //free(oldindices);

  for (i=0; i<g->e; i++)
	{
    g->edges[i].s=newindices[g->edges[i].s];
    g->edges[i].t=newindices[g->edges[i].t];
  }

  free(newindices);

  g->n -= isolated_nodes;
  return(oldindices);
}


edgelist* oldreadedgelist(char* input){
	unsigned long e1=NLINKS;

	FILE *file=fopen(input, "r");

	edgelist *g=malloc(sizeof(edgelist));
	g->n=0;
	g->e=0;

	char ch[100];
	char nedges [20];
  fscanf(file, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %s %*s %*s %s", nedges, ch)  ;

  char*eptr;
  e1 = strtoul(nedges,&eptr,10);
  g->edges = malloc((e1+1)*sizeof(edge));//allocate some RAM to store edges

 	if (g->edges==NULL)
	{
		printf("Failed allocation of memory to edges\n");
	}

	while (fscanf(file,"%lu %lu", &(g->edges[g->e].s), &(g->edges[g->e].t))==2)
	{
    g->n=max3(g->n,g->edges[g->e].s,g->edges[g->e].t);
		if ((g->e)++==e1)
		{//increase allocated RAM if needed
    	printf("realloc\n");
			//e1+= NLINKS;
			e1+=10000000;
      g->edges=realloc(g->edges,e1*sizeof(edge));
  		if (g->edges == NULL)
			{
				printf("edges became null");
			}
		}
  }

	fclose(file);
	g->n++;
	g->edges=realloc(g->edges,g->e*sizeof(edge));
	return g;
}
