#include "adjarray.c"

unsigned long connected(adjlist* g){
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
     visitednodes=1; // nombre de noeods atteint par BFS
     marked[0]=1;
     first_not_marked=1;
     
     while (visitednodes!=g->n){
           
         if (end < start && visitednodes < g->n){
                 
             contains[k]=start-contains[k-1];    
             //printf("%lu %lu k\n",k, contains[k]);
             
             
             k++;
             for (i=first_not_marked; i<g->n;i++){
                 if (marked[i]==0){
                     break;
                 }    
             }
             first_not_marked=i;
             end++;
             FIFO[end]= i;
             marked[i]=1; 
             //printf("next %lu\n",i);
             //if (k==100) break;
         }  
           
         u=FIFO[start];
         start++;
         for (i=0 ; i<g->cd[u+1]-g->cd[u] ;i++ ){
             v=g->adj[g->cd[u] + i];
             if (marked[v]==0) {
                marked[v]=1;
                end++;
                visitednodes++;
                
                FIFO[end]=v  ;          
             }
         }
         if (visitednodes == g->n){
                 
             contains[k]=g->n-contains[k-1]; 
         }    
     }

         
           
        free (FIFO);
        free (marked);
        
        printf("Nombdre de composante connexes %lu\n",k);
        for (i=1;i<k+1;i++){
            printf("Nombre d element de la %lu composante: %lu\n",i,contains[i]);
        }
        //contains = realloc((k+1)*sizeof(unsigned long));
        free(contains);
}

int main(int argc,char** argv){


    
	adjlist* g;
	time_t t1,t2,t3;

	t1=time(NULL);

	printf("Reading edgelist from file %s\n",argv[1]);
	g=readedgelist(argv[1]);
    
    
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

	printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));
 
    
    printf("Start counting connected parts\n");
    connected(g);
    t3=time(NULL);
    printf("BFS finished\n");
    
    printf("- BFS time = %ldh%ldm%lds\n",(t3-t2)/3600,((t3-t2)%3600)/60,((t3-t2)%60));
    free_adjlist(g);
	return 0;
}
