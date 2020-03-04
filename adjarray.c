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
	unsigned long *cd;//cumulative degree cd[0]=0 length=n+1
	unsigned long *adj;//concatenated lists of neighbors of all nodes
} adjlist;

//compute the maximum of three unsigned long
inline unsigned long max3(unsigned long a,unsigned long b,unsigned long c){
	a=(a>b) ? a : b;
	return (a>c) ? a : c;
}

inline unsigned long min (unsigned long a,unsigned long b){
       return (a<b) ? a : b;
}
//reading the edgelist from file
adjlist* readedgelist(char* input){
	unsigned long e1=NLINKS;
	FILE *file=fopen(input, "r");

	adjlist *g=malloc(sizeof(adjlist));
	g->n=0;
	g->e=0;
	g->edges=malloc(e1*sizeof(edge));//allocate some RAM to store edges
	
	char* ch[100];
    fscanf(file, "%*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %*s %s", ch)  ;
    
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

//building the adjacency matrix
void mkadjlist(adjlist* g){
	unsigned long i,u,v;
	unsigned long *d=calloc(g->n,sizeof(unsigned long));

	for (i=0;i<g->e;i++) {
		d[g->edges[i].s]++;
		d[g->edges[i].t]++;
	}

	g->cd=malloc((g->n+1)*sizeof(unsigned long));
	g->cd[0]=0;
	for (i=1;i<g->n+1;i++) {
		g->cd[i]=g->cd[i-1]+d[i-1];
		d[i-1]=0;
	}

	g->adj=malloc(2*g->e*sizeof(unsigned long));

	for (i=0;i<g->e;i++) {
		u=g->edges[i].s;
		v=g->edges[i].t;
		g->adj[ g->cd[u] + d[u]++ ]=v;
		g->adj[ g->cd[v] + d[v]++ ]=u;
	}

	free(d);
	//free(g->edges);
}

// Merges two subarrays of arr[]. 
// First subarray is arr[l..m] 
// Second subarray is arr[m+1..r] 
void merge(edge arr[], unsigned long l, unsigned long m, unsigned long r)
{
	unsigned long i, j, k;
	unsigned long n1 = m - l + 1;
	unsigned long n2 = r - m;

	/* create temp arrays */
	edge L[n1], R[n2];

	/* Copy data to temp arrays L[] and R[] */
	for (i = 0; i < n1; i++)
	
		L[i] = arr[l + i];
	for (j = 0; j < n2; j++)
		R[j] = arr[m + 1 + j];

	/* Merge the temp arrays back into arr[l..r]*/
	i = 0; // Initial index of first subarray 
	j = 0; // Initial index of second subarray 
	k = l; // Initial index of merged subarray 
	while (i < n1 && j < n2)
	{
		if (L[i].s < R[j].s || (L[i].s==R[j].s && L[i].t < R[j].t) )
		{
			arr[k] = L[i];
			i++;
		}
		else
		{
			arr[k] = R[j];
			j++;
		}
		k++;
	}

	/* Copy the remaining elements of L[], if there
	are any */
	while (i < n1)
	{
		arr[k] = L[i];
		i++;
		k++;
	}

	/* Copy the remaining elements of R[], if there
	are any */
	while (j < n2)
	{
		arr[k] = R[j];
		j++;
		k++;
	}
}

/* l is for left index and r is right index of the
sub-array of arr to be sorted */
void mergeSort(edge arr[], unsigned long l, unsigned long r)
{
     
	if (l < r)
	{
		// Same as (l+r)/2, but avoids overflow for 
		// large l and h 
		unsigned long m = l + (r - l) / 2;

		// Sort first and second halves 
		mergeSort(arr, l, m);
		mergeSort(arr, m + 1, r);

		merge(arr, l, m, r);
	}
}



void clean(adjlist* g){
     //FILE * fp;
   
     //fp = fopen ("cleanamazon.txt","w"); 

     unsigned long repetitions=0,i,k=0;
    
     for (i=0;i< g->e; i++){
         if (g->edges[i].s == g->edges[i].t){
            repetitions++;
         }
     }
     edge* newedges = malloc((g->e - repetitions)*sizeof(edge));
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
     g->edges= newedges;
     g->e= g->e - repetitions;
}

void duplicates(adjlist* g){
     FILE * fp;
   
     fp = fopen ("cleanlivejournal.txt","w"); 

     unsigned long repetitions=0,i,k=0;
    
     for (i=0;i< g->e-1; i++){
         if (g->edges[i].s == g->edges[i+1].s && g->edges[i].t == g->edges[i+1].t ){
            repetitions++;
         }
     }
     edge* newedges = malloc((g->e - repetitions)*sizeof(edge));
     for (i=0 ; i< g->e-1 ; i++){
         if (g->edges[i].s != g->edges[i+1].s || g->edges[i].t != g->edges[i+1].t ){
            
            newedges[k].s = g->edges[i].s;
            newedges[k].t = g->edges[i].t;
            fprintf (fp, "%lu %lu\n",newedges[k].s, newedges[k].t );
            k++;
              
         }
     }
      newedges[k].s = g->edges[g->e-1].s;
      newedges[k].t = g->edges[g->e-1].t; 
      fprintf (fp, "%lu %lu\n",newedges[k].s, newedges[k].t );  
   
     
     fclose(fp);
     g->edges= newedges;
     g->e = g->e - repetitions;
}
     
long int triangles(adjlist* g){
     unsigned long nbtriangles=0;
     unsigned long u,v,w1,w2,ku,kv,i=0;
     for(i=0;i< g->e ;i++){
         //printf("%lu %",i);
         u = g->edges[i].s;
         //printf("%lu %",i);
         v = g->edges[i].t;
         ku=0; kv=0;
         //printf("%lu %lu %",u ,v);
         while (ku < g->cd[u+1]-g->cd[u] && kv < g->cd[v+1]-g->cd[v]){
               //printf("%lu lu %",ku ,kv);
               w1=g->adj[g->cd[u]+ku]  ; w2= g->adj[g->cd[v]+kv];
               
               while (w1 <w2 && ku < g->cd[u+1]-g->cd[u]){
                     ku++;
                     w1=g->adj[g->cd[u]+ku]  ;
               }
               if (w1==w2 && ku<g->cd[u+1]-g->cd[u]){
                    nbtriangles++;
                    kv++; 
                    //printf("%lu %lu %lu \n" , u,v,w1)   ;   
               }
               while (w2 <w1 && kv < g->cd[v+1]-g->cd[v]){
                     kv++;
                     w2=g->adj[g->cd[v]+kv]  ;
               }
               
         }
     
     }
     return(nbtriangles);
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



int main(int argc,char** argv){
	adjlist* g;
	time_t t1,t2,t3;

	t1=time(NULL);

	printf("Reading edgelist from file %s\n",argv[1]);
	g=readedgelist(argv[1]);
    
    
    clean(g);
    //mergeSort(g->edges,0, g->e-1);
    edge *B =malloc (g->e*sizeof(edge));
    
    BottomUpMergeSort(g->edges,B, g->e);
    duplicates(g);
    
    
	printf("Number of nodes: %lu\n",g->n);
	printf("Number of edges: %lu\n",g->e);
    
	printf("Building the adjacency list\n");
	mkadjlist(g);
	
	

	t2=time(NULL);

	printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));
 
    
    
    unsigned long triang =  triangles(g);
    t3=time(NULL);
    printf("Triangles %lu\n",triang);
    
    printf("- Trinagles time = %ldh%ldm%lds\n",(t3-t2)/3600,((t3-t2)%3600)/60,((t3-t2)%60));
    free_adjlist(g);
	return 0;
}


