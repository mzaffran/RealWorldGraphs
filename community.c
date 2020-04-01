#include "adjarray.c"

void labelPropagation(adjlist* g);

int main(int argc,char** argv){

  time_t t1,t2 ;

  t1=time(NULL);

  adjlist* g;
  g=readedgelist(argv[1]);

  clean(g);
  edge *B = malloc (g->e*sizeof(edge));
  printf("Start sorting\n");

  BottomUpMergeSort(g->edges,B, g->e);

  free(B);
  printf("Finished sorting\n");
  duplicates(g);

  mkadjlist(g);

  printf("Number of nodes: %lu\n",g->n);
  printf("Number of edges: %lu\n",g->e);

  unsigned long i,k=0;
  for (i=0;i<g->n;i++){
      if (g->cd[i+1]-g->cd[i]==0){
         k++;
      }
  }
  printf("Number of isolated nodes = %lu \n",k)    ;

  labelPropagation(g);

  printf("Finished label propagation \n");

  t2=time(NULL);

  printf("=== Overall time label propagation = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60)) ;

  free(g) ;

}

void labelPropagation(adjlist* g){ // version n^2

    unsigned long* labels = malloc(g->n*sizeof(unsigned long)); // label of each node
    unsigned long* order = malloc(g->n*sizeof(unsigned long)); // shuffle order
    unsigned long* labelusage = malloc(g->n*sizeof(unsigned long)); // how many times the label is used
    unsigned long* freq = calloc(g->n,sizeof(unsigned long)); // frequency of neighbouring labels (for a fixed node)
    if (labels==NULL || order ==NULL || labelusage == NULL || freq == NULL){
       printf("failed memory at labelPropgation2");
    }

    unsigned long i, j, aux, maxfreq, freqlabel, node, nblabels;

    nblabels = g->n;
    //printf("intital %lu ", nblabels);
    for (i=0; i<g->n; i++)
    {
      labels[i]=i;
      labelusage[i]=1;
      order[i]=i;
    }
    int finish=0;

    while (!finish){
        //printf("nb labels= %lu\n",nblabels);
        finish=1;
        //shuffle
        for (i=g->n-1; i>=1; i--)
        {
          j = genrand64_int64()%(i+1);
          aux = order[i];
          order[i] = order[j];
          order[j] = aux;
        }
        //printf("f");
        //label propagation
        for (i=0;i<g->n; i++) {
            node = order[i];
            if (g->cd[node+1]-g->cd[node]==0)
            {
             continue;
            }
            //calcul des frequences de chaque label parmi les voisins
            for (j=g->cd[node]; j < g->cd[node+1];j++)
            {
             freq[labels[g->adj[j]]]++;
            }
            freq[labels[node]]++;
            // Recherche du label le plus frequent
            maxfreq=freq[labels[node]];
            freqlabel=labels[node];
            for (j=g->cd[node]; j< g->cd[node+1];j++)
            {
              if (maxfreq < freq[labels[g->adj[j]]])
              {
               maxfreq=freq[labels[g->adj[j]]];
               freqlabel=labels[g->adj[j]];
              }
              freq[labels[g->adj[j]]]=0;
            }
            if ((maxfreq==1 || freq[labels[node]]<maxfreq) && labels[node]!=freqlabel)
            {
             if (--labelusage[labels[node]] == 0)
             {
              nblabels--;
             }
             labels[node]=freqlabel;
             labelusage[freqlabel]++;
             finish=0;
            }
        }
    }
    free(order);
    free(freq);
    printf("Number of labels used %lu\n", nblabels);
    for (j=0; j<g->n;j++)
    {
      if (labelusage[j]>0)
        printf("Label %lu is used %lu times\n",j,labelusage[j]);
    }
    for (j=0; j<g->n;j++)
    {
      printf("node %lu has label  %lu\n",j,labels[j]);
    }

    free(labelusage);

    free(labels);
}
