#include "edgelist.c"

double* Pagerank(edgelist* g){
     
     double *pagerank = malloc(g->n*sizeof(double));
     double *pageranktemp = malloc(g->n*sizeof(double));
     
     unsigned long *d=calloc(g->n,sizeof(unsigned long));
     
     double alpha=0.15,somme=0;
     int t =40,j;
     
     unsigned long i;
     
     for (i=0; i<g->e;i++){
         d[g->edges[i].s]++ ;
     }
     for (i=0; i<g->n;i++){
         pagerank[i] = 1/g->n;
         pageranktemp[i] = 0;
     }
     for (j=0; j<t ; j++){
         // MatVecctProd (T,P) 
         for(i=0;i< g->e ;i++){
             //printf("s t %lu %lu\n",g->edges[i].s,g->edges[i].t);       
             pageranktemp[g->edges[i].t]+=pagerank[g->edges[i].s]/d[g->edges[i].s];  
         }
          
         somme=0;
         for(i=0;i< g->n ;i++){
                    
             pageranktemp[i]=(1-alpha)*pageranktemp[i]+alpha/g->n;  
             somme+=pageranktemp[i];
         }
         //normalize

         for(i=0;i< g->n ;i++){
                    
             pageranktemp[i]+=(1-somme)/g->n;  

             pagerank[i]=pageranktemp[i]; // update
             pageranktemp[i]=0; //reinitialize
         }
                      
     
     }
     
     free(pageranktemp);
     free(d);
     return(pagerank);
         
}

int SearchMax(double* l, unsigned long n){
    int maxindex=0;
    double maxvalue=0;
    unsigned long  i;
    for (i=0;i<n;i++){
        if (l[i]>maxvalue)    {
            maxvalue=l[i];
            maxindex=i;                           
        }
    }    
    return(maxindex);
}

void getPages (edgelist *g, int number, char** pagenames){
    double* pagerank= Pagerank(g);
    unsigned long index;
    int i;
    printf("ranks\n");
    for (i=0;i<number;i++){
        index = SearchMax(pagerank, g->n);
        
        printf("Page with rank %d has ID: %lu ,probability %lf and name %s\n",i+1, index, pagerank[index], pagenames[index]);   
        pagerank[index]=0;
    }
          
     
}

int main(int argc,char** argv){
	edgelist* g;
	time_t t1,t2,t3;

	t1=time(NULL);

	printf("Reading edgelist from file %s\n",argv[1]);
	g=readedgelist(argv[1]);

    char** pagenames = readpagelist(argv[2], g->n);
    
  
    
	printf("Number of nodes: %lu\n",g->n);
	printf("Number of edges: %lu\n",g->e);


    getPages (g, 5 , pagenames);

	

	t2=time(NULL);

	printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));
 
    

    free_edgelist(g);
	return 0;
}
