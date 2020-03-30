#include "adjarray.c"

// Find number of elements in each connected component
// !! not tested on graphs with many components !!

void connected(adjlist* g){
     int* marked = calloc(g->n,sizeof(int)) ;

     unsigned long* FIFO = malloc(g->n*sizeof(unsigned long)) ;
     unsigned long* contains = malloc(g->n*sizeof(unsigned long));

     if (contains==NULL) printf("null c\n");
     if (marked==NULL) printf("null m\n");
     if (FIFO==NULL) printf("null f\n");

     unsigned long u, v, i, k, start, end, visitednodes, first_not_marked;

     contains[0]=0;  // contains[k] nombre d'element de la compasate connexe k pour k > 0
     k=1; // nombre de composantes connexes
     start=0;
     FIFO[0]=0;
     end=0;
     visitednodes=1; // nombre de noeuds atteint par BFS
     marked[0]=1;
     first_not_marked=1;

     while (visitednodes!=g->n)
     {
         if (end < start && visitednodes < g->n)
         {
             contains[k]=start-contains[k-1];
             //printf("%lu %lu k\n",k, contains[k]);
             k++;
             for (i=first_not_marked; i<g->n;i++)
             {
                 if (marked[i]==0)
                 {
                     break;
                 }
             }
             first_not_marked=i;
             end++;
             FIFO[end]=i;
             marked[i]=1;
             //printf("next %lu\n",i);
             //if (k==100) break;
         }
         u=FIFO[start];
         start++;
         for (i=0 ; i<g->cd[u+1]-g->cd[u] ;i++ )
         {
             v=g->adj[g->cd[u] + i];
             if (marked[v]==0)
             {
                marked[v]=1;
                end++;
                visitednodes++;
                FIFO[end]=v  ;
             }
         }
         if (visitednodes == g->n)
         {
             contains[k]=g->n-contains[k-1];
         }
     }

     free (FIFO);
     free (marked);

     printf("Nombre de composantes connexes : %lu\n",k);
     for (i=1;i<k+1;i++)
     {
         printf("Nombre d'éléments de la composante %lu : %lu\n",i,contains[i]);
     }
     //contains = realloc((k+1)*sizeof(unsigned long));
     free(contains);
}

//Find a lower bound to tthe diameter
// !! not tested on graphs with many components !!

unsigned long diameter(adjlist* g){
     int* marked = calloc(g->n,sizeof(int)) ;
     int* depth = calloc(g->n,sizeof(int)) ;
     unsigned long* FIFO = malloc(g->n*sizeof(unsigned long)) ;
     time_t t1;

     if (marked==NULL) printf("null m\n");
     if (FIFO==NULL) printf("null f\n");

     unsigned long u, v, i, k, start, end,  diameter, last_marked, newsource, best_source, best_destination;

     diameter=0;

     k=1; // nombre de composantes connexes
     start=0;
     FIFO[0]=0;
     end=0;
     marked[0]=1;

     depth[0]=0;

     t1=time(NULL);
     int tmax=60, stagnate=0;
     while (time(NULL) -t1 <=tmax)
     {
         stagnate=0;
         while (stagnate <5)
         {
             while(end>=start)
             {
                 u=FIFO[start];
                 start++;
                 for (i=0 ; i<g->cd[u+1]-g->cd[u] ;i++ )
                 {
                     v=g->adj[g->cd[u] + i];
                     if (marked[v]==0)
                     {
                        last_marked=v;
                        marked[v]=1;
                        depth[v]=1+depth[u];
                        end++;
                        FIFO[end]=v  ;
                     }
                 }
             }
             if (diameter < depth[last_marked])
             {
                best_source=FIFO[0];
                best_destination= last_marked;
                diameter = depth[last_marked];
                for (i=0;i<g->n;i++)
                {
                    if (marked[i]>0)
                       marked[i]=0;
                }
                start=0;
                FIFO[start]=last_marked;
                end=0;
                marked[last_marked]=1;
                depth[last_marked]=0;
             }
             else
             {
                  stagnate++;
             }
         }
         newsource=genrand64_int64()%g->n;

         start=0;
         FIFO[0]= newsource;
         end=0;
         marked[newsource]=1;
         depth[newsource]=0;
     }

    free (FIFO);
    free (marked);
    free(depth);

    printf("Longest shortest path found is between nodes %lu and %lu\n", best_source, best_destination);
    return(diameter);
}


void shortestpath(adjlist* g, unsigned long source, unsigned long destination){
     int* marked = calloc(g->n,sizeof(int)) ;
     int* depth = calloc(g->n,sizeof(int)) ;
     unsigned long* FIFO = malloc(g->n*sizeof(unsigned long)) ;

     if (marked==NULL) printf("null m\n");
     if (FIFO==NULL) printf("null f\n");

     unsigned long u, v, i, start, end,  diameter, last_marked;

     diameter=0;

     start=0;
     FIFO[0]=source;
     end=0;
     marked[source]=1;

     depth[source]=0;

     while(end>=start)
     {
         u=FIFO[start];
         start++;
         for (i=0 ; i<g->cd[u+1]-g->cd[u] ;i++ )
         {
             v=g->adj[g->cd[u] + i];
             if (destination==v)
             {
                printf("found shortest path with length %d of arcs\n", depth[u]+1);
                break;
             }
             if (marked[v]==0)
             {
                last_marked=v;
                marked[v]=1;
                depth[v]=1+depth[u];
                end++;

                FIFO[end]=v  ;
             }
         }
     }

    free (FIFO);
    free (marked);
    free(depth);
}

int main(int argc,char** argv){

	adjlist* g;
	time_t t1,t2,t3,t4;

	t1=time(NULL);

	printf("Reading edgelist from file %s\n",argv[1]);
	g=readedgelist(argv[1]);

  printf("Finished Reading, start cleaning\n");
  clean(g);
  renamevertices(g);
  clean(g);

  //mergeSort(g->edges,0, g->e-1);
  edge *B =malloc (g->e*sizeof(edge));
  printf("Start sorting\n");

  BottomUpMergeSort(g->edges,B, g->e);

  free(B);
  printf("Finished sorting\n");

  duplicates(g);

	printf("Number of nodes: %lu\n",g->n);
	printf("Number of edges: %lu\n",g->e);

	printf("Building the adjacency list\n");
	//mkadjlist(g);

	mkadjlist(g);

	t2=time(NULL);

	printf("=== Graph preparation time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));

  printf("Start counting connected parts\n");
  connected(g);
  t3=time(NULL);
  printf("BFS finished\n");

  printf("=== BFS time = %ldh%ldm%lds\n",(t3-t2)/3600,((t3-t2)%3600)/60,((t3-t2)%60));

  printf("Start finding lower bound to diameter\n");
  unsigned long d = diameter(g);

  printf("Lower bound for graph diameter: %lu", d );

  t4=time(NULL);

  printf("=== BFSs for lower bound time = %ldh%ldm%lds\n",(t4-t3)/3600,((t4-t3)%3600)/60,((t4-t3)%60));

  printf("=== Overall time = %ldh%ldm%lds\n",(t4-t1)/3600,((t4-t1)%3600)/60,((t4-t1)%60));

  free_adjlist(g);
	return 0;
}
