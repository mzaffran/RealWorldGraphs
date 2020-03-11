/*
Maximilien Danisch
September 2017
http://bit.ly/danisch
maximilien.danisch@gmail.com

Info:
Feel free to use these lines as you wish. This program loads a graph in main memory.

To compile:
"gcc adjlist.c -O9 -o adjlist".

To execute:
"./adjlist edgelist.txt".
"edgelist.txt" should contain the graph: one edge on each line (two unsigned long (nodes' ID)) separated by a space.
The prograph will load the graph in main memory and then terminate.

Note:
If the graph is directed (and weighted) with selfloops and you want to make it undirected unweighted without selfloops, use the following linux command line.
awk '{if ($1<$2) print $1" "$2;else if ($2<$1) print $2" "$1}' net.txt | sort -n -k1,2 -u > net2.txt

Performance:
Up to 200 million edges on my laptop with 8G of RAM: takes more or less 4G of RAM and 30 seconds (I have an SSD hardrive) for 100M edges.
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>//to estimate the runing time
#include <string.h>
#include "utility.c"



#define NLINKS 120000000 //maximum number of edges for memory allocation, will increase if needed


typedef struct {
	unsigned long s;
	unsigned long t;
} edge;

typedef struct {
	unsigned long n;//number of nodes
	unsigned long e;//number of edges
	edge *edges;//list of edges
	unsigned long *cd;//cumulative degree cd[0]=0 length=n+1
	unsigned long *adj;//concatenated lists of neighbors of all nodes
} adjlist;






//reading the edgelist from file
adjlist* readedgelist(char* input){
	unsigned long e1=NLINKS;

	FILE *file=fopen(input, "r");

	adjlist *g=malloc(sizeof(adjlist));
	g->n=0;
	g->e=0;
	
	

	char* ch[100];
	char nedges [20];
    fscanf(file, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %s %*s %*s %s", nedges, ch)  ;
    
    char*eptr;
    e1=strtoul(nedges,&eptr,10);
    g->edges=malloc((e1+1)*sizeof(edge));//allocate some RAM to store edges
    
   	if (g->edges==NULL) printf("Failed allocation of memory to edges\n");
    else printf("Succesful allocation of memory to edges\n");
    

	while (fscanf(file,"%lu %lu", &(g->edges[g->e].s), &(g->edges[g->e].t))==2) {
        g->n=max3(g->n,g->edges[g->e].s,g->edges[g->e].t);
		if ((g->e)++==e1) {//increase allocated RAM if needed
  		     printf("realloc\n");           
			//e1+= NLINKS;
			e1+=10000000;
            g->edges=realloc(g->edges,e1*sizeof(edge));
 
		    if (g->edges ==NULL)
		       printf("edges became null");
		}

    }

	fclose(file);

	g->n++;

	g->edges=realloc(g->edges,g->e*sizeof(edge));

	return g;
}

//building the adjacency matrix
void mkadjlist(adjlist* g){
	unsigned long i,u,v;

	unsigned long *d=calloc(g->n,sizeof(unsigned long));
    if (d==NULL)
    printf("d null");
	for (i=0;i<g->e;i++) {
		d[g->edges[i].s]++;
		d[g->edges[i].t]++;
	}

	g->cd=malloc((g->n+1)*sizeof(unsigned long));
	 if (g->cd==NULL)
    printf("cd null");
	g->cd[0]=0;
	for (i=1;i<g->n+1;i++) {
		g->cd[i]=g->cd[i-1]+d[i-1];
		d[i-1]=0;
	}

	g->adj=malloc(2*g->e*sizeof(unsigned long));
if (g->adj==NULL)
    printf("adj null");
	for (i=0;i<g->e;i++) {
		u=g->edges[i].s;
		v=g->edges[i].t;
		g->adj[ g->cd[u] + d[u]++ ]=v;
		g->adj[ g->cd[v] + d[v]++ ]=u;
	}

	free(d);
	//free(g->edges);
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

//  Left run is A[iLeft :iRight-1].
// Right run is A[iRight:iEnd-1  ].


//rename vertices such that the vertices are in decreasing order of degree and there are no isolated nodes.
void renamevertices(adjlist* g){
    
	unsigned long i,j;
	unsigned long *d=calloc(g->n,sizeof(unsigned long));
    if (d==NULL)
       printf("failed to allocate d\n");
	for (i=0;i<g->e;i++) {
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
    for (i=0; i< g->n; i++){
        oldindices[i]=i; 
        oldindices2[i]=i;   
    }
    BottomUpMergeSort2(d, ordered_d, g->n , oldindices, oldindices2);
    
    free(ordered_d);
    free(oldindices2);
    

    unsigned long isolated_nodes=0;
    i=g->n-1;
    
    while (d[i]==0){
          
          isolated_nodes++;      
          i--;
          
    }
   
    free(d); 
    

    
    
    unsigned long *newindices = malloc(g->n*sizeof(unsigned long));
    if (newindices==NULL)
       printf("failed allocateion of new indices\n");

    for (i=0 ; i<g->n; i++){
        newindices[oldindices[i]]=i;
    }


    free(oldindices);
    

    //edge *newedges = malloc(g->e*sizeof(edge));
    //if (newedges==NULL)
      //printf("Failed to allocate ew edges in rename\n");
    for (i=0; i<g->e; i++){
        g->edges[i].s=newindices[g->edges[i].s];
        g->edges[i].t=newindices[g->edges[i].t];
        //newedges[i].s=newindices[g->edges[i].s];
        //newedges[i].t=newindices[g->edges[i].t];

    }
    //free(g->edges);
    //g->edges=newedges;
    //free(newindices);


    g->n -= isolated_nodes; 

   

}


void mkdirectedadjlist(adjlist* g){
	unsigned long i,u,v;
	unsigned long *d=calloc(g->n,sizeof(unsigned long));

	for (i=0;i<g->e;i++) {
		d[g->edges[i].s]++;
		
	}

	g->cd=malloc((g->n+1)*sizeof(unsigned long));
	g->cd[0]=0;
	for (i=1;i<g->n+1;i++) {
		g->cd[i]=g->cd[i-1]+d[i-1];
		d[i-1]=0;
	}

	g->adj=malloc(g->e*sizeof(unsigned long));

	for (i=0;i<g->e;i++) {
		u=g->edges[i].s;
		v=g->edges[i].t;
		g->adj[ g->cd[u] + d[u]++ ]=v;
		
	}

	free(d);
	//free(g->edges);
}

//freeing memory
void free_adjlist(adjlist *g){
	free(g->edges);
	free(g->cd);
	free(g->adj);
	free(g);
}



void BottomUpMerge(edge A[], unsigned long iLeft, unsigned long iRight, unsigned long iEnd, edge B[])
{
    unsigned long i,j,k; 
    i = iLeft, j = iRight;
    // While there are elements in the left or right runs...
    for (k = iLeft; k < iEnd; k++) {
        // If left run head exists and is <= existing right run head.
        if (i < iRight && (j >= iEnd || A[i].s < A[j].s || (A[i].s == A[j].s && A[i].t < A[j].t  )  )) {
            B[k] = A[i];
            i = i + 1;
        } else {
            B[k] = A[j];
            j = j + 1;    
        }
    } 
}

void CopyArray(edge B[], edge A[], unsigned long n)
{
     unsigned long i;
    for(i = 0; i < n; i++)
        A[i] = B[i];
}
void BottomUpMergeSort(edge A[],edge B[], unsigned long n)
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
            BottomUpMerge(A, i, min(i+width, n), min(i+2*width, n), B);
        }
        // Now work array B is full of runs of length 2*width.
        // Copy array B to array A for next iteration.
        // A more efficient implementation would swap the roles of A and B.
        CopyArray(B, A, n);
    
        // Now array A is full of runs of length 2*width.
    }
}

//  Left run is A[iLeft :iRight-1].
// Right run is A[iRight:iEnd-1  ].





//Eliminer les boucles et mettre toutes les arretes sous la forme s->t avec s<t
void clean(adjlist* g){
     //FILE * fp;
   
     //fp = fopen ("cleanamazon.txt","w"); 

     unsigned long repetitions=0,i,k=0;
    
     for (i=0;i< g->e; i++){
         if (g->edges[i].s == g->edges[i].t){
            repetitions++;
         }
     }

     if (repetitions !=0){
         printf("boucle %lu\n",repetitions);
         edge* newedges = malloc((g->e - repetitions)*sizeof(edge));
         if (newedges == NULL)
            printf("Failed memory allocation of newedges in function clean.c\n");
         for (i=0 ; i< g->e ; i++){
             if (g->edges[i].s > g->edges[i].t){
                
                newedges[k].s = g->edges[i].t;
                newedges[k].t = g->edges[i].s;
                //fprintf (fp, "%lu %lu\n",newedges[k].s, newedges[k].t );
                k++;
                  
             }
             else if (g->edges[i].s < g->edges[i].t){
                newedges[k].s = g->edges[i].s;
                newedges[k].t = g->edges[i].t;  
                //fprintf (fp, "%lu %lu\n",newedges[k].s, newedges[k].t );            
                k++;
             }        
         }
         //fclose(fp);
         free(g->edges);
         g->edges= newedges;
         g->e= g->e - repetitions;
     }
}

void duplicates(adjlist* g){
     //FILE * fp;
   
     //fp = fopen ("cleanlivejournal.txt","w"); 

     unsigned long repetitions=0,i,k=0;
    
     for (i=0;i< g->e-1; i++){
         
         if (g->edges[i].s == g->edges[i+1].s && g->edges[i].t == g->edges[i+1].t ){
            repetitions++;
         }
     }

     if (repetitions!=0){
        edge* newedges =  malloc((g->e - repetitions-1)*sizeof(edge));
        if (newedges==NULL)
        printf("null err\n");
        for (i=0 ; i< g->e-1 ; i++){
         
            if (g->edges[i].s != g->edges[i+1].s || g->edges[i].t != g->edges[i+1].t ){
           
               newedges[k].s = g->edges[i].s;
               newedges[k].t = g->edges[i].t;
               //fprintf (fp, "%lu %lu\n",newedges[k].s, newedges[k].t );
               k++;
            
               
            }
        }
     
        newedges[k].s = g->edges[g->e-1].s;
        newedges[k].t = g->edges[g->e-1].t; 
        //fprintf (fp, "%lu %lu\n",newedges[k].s, newedges[k].t );  
   
     
        //fclose(fp);
        free(g->edges);
        g->edges= newedges;
        g->e = g->e - repetitions;
     }
}
