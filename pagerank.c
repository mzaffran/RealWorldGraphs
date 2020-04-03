#include "edgelist.c"

double* Pagerank(edgelist* g, double alpha){

     double *pagerank = malloc(g->n*sizeof(double));
     double *pageranktemp = malloc(g->n*sizeof(double));

     unsigned long *d=calloc(g->n,sizeof(unsigned long));

     double somme=0;
     int t = 40, j;

     unsigned long i;

     for (i=0; i<g->e;i++)
     {
         d[g->edges[i].s]++ ;
     }
     for (i=0; i<g->n;i++)
     {
         pagerank[i] = 1/g->n;
         pageranktemp[i] = 0;
     }
     for (j=0; j<t ; j++)
     {
         // MatVecctProd (T,P)
         for(i=0;i< g->e ;i++)
         {
             //printf("s t %lu %lu\n",g->edges[i].s,g->edges[i].t);
             pageranktemp[g->edges[i].t]+=pagerank[g->edges[i].s]/d[g->edges[i].s];
         }

         somme=0;
         for(i=0; i<g->n; i++)
         {
             pageranktemp[i]=(1-alpha)*pageranktemp[i]+alpha/g->n;
             somme+=pageranktemp[i];
         }
         //normalize

         for(i=0; i<g->n; i++)
         {
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
    unsigned long i;
    for (i=0;i<n;i++)
    {
        if ((l[i]>maxvalue) && (l[i] <= 1) && (0 <= l[i]))
        {
            maxvalue=l[i];
            maxindex=i;
        }
    }
    return(maxindex);
}

int SearchMin(double* l, unsigned long n){
    int minindex=0;
    double minvalue=1;
    unsigned long i;
    for (i=0;i<n;i++)
    {
        if ((l[i]<minvalue) && (l[i] <= 1) && (0 < l[i]))
        {
            minvalue=l[i];
            minindex=i;
        }
    }
    return(minindex);
}

void getPages (edgelist* g, int number, char** pagenames, double* pagerank){

    unsigned long index;
    int i;
    printf("=== Highest pagerank obtained, first %d ones\n", number);
    for (i=0;i<number;i++)
    {
        index = SearchMax(pagerank, g->n);
        printf("Page with rank %d has ID: %lu, probability %lf and name %s\n",i+1, index, pagerank[index], pagenames[index]);
        pagerank[index]=-1;
    }
    printf("=== Lowest pagerank obtained, last %d ones\n", number);
    for (i=0;i<number;i++)
    {
        index = SearchMin(pagerank, g->n);
        printf("Page with rank %lu has ID: %lu, probability %lf and name %s\n",g->n-i, index, pagerank[index], pagenames[index]);
        pagerank[index]=2;
    }

}

void savePages (edgelist* g, double* pagerank, double alpha){

  char name[100] = "results/pagerank/";
  char alpha_str[10] ;
  sprintf(alpha_str, "%.2f", alpha) ;
  strcat(name, alpha_str) ;
  strcat(name, "_PageRank.txt") ;

  FILE *results = fopen(name, "w") ;
  fprintf(results, "ID;PageRank;\n");

  unsigned long i;
  for (i=0;i<g->n;i++)
  {
    fprintf(results, "%lu;%lf;\n", i, pagerank[i]);
  }

  fclose(results);
}

int main(int argc,char** argv){
	edgelist* g;
	time_t t1,t2,t3,t4,t5;

	t1=time(NULL);

	printf("=== Reading edgelist from file %s\n",argv[1]);
	g=readedgelist(argv[1]);

  printf("Number of nodes: %lu\n",g->n);
	printf("Number of edges: %lu\n",g->e);

  t2=time(NULL);
	printf("=== Graph preparation time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));

  char* alpha_value = argv[3] ;
  double alpha ;
  alpha = strtod(alpha_value, NULL);

  char** pagenames = readpagelist(argv[2], g->n);

  double* pagerank = Pagerank(g, alpha);

  t3=time(NULL);
	printf("=== Pagerank time = %ldh%ldm%lds\n",(t3-t2)/3600,((t3-t2)%3600)/60,((t3-t2)%60));
  printf("=== Overall pagerank time = %ldh%ldm%lds\n",(t3-t1)/3600,((t3-t1)%3600)/60,((t3-t1)%60));

  printf("=== Saving the results. ");

  savePages (g, pagerank, alpha) ;

  t4=time(NULL);
	printf("Saving time = %ldh%ldm%lds\n",(t4-t3)/3600,((t4-t3)%3600)/60,((t4-t3)%60));

  getPages (g, 5, pagenames, pagerank);

	t5=time(NULL);
	printf("=== Overall time = %ldh%ldm%lds\n",(t5-t1)/3600,((t5-t1)%3600)/60,((t5-t1)%60));

  free_edgelist(g);
  free(pagenames);
  free(pagerank);

	return 0;
}
