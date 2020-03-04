#include "adjarray.c"

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
