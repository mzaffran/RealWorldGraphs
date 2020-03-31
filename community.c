#include "adjarray.c"


void labelPropagation2(adjlist* g);
void labelPropagation(adjlist* g){
    unsigned long* labels = malloc(g->n*sizeof(unsigned long));
    unsigned long* nodelabels = malloc(g->n*sizeof(unsigned long));
    unsigned long* labelusage = malloc(g->n*sizeof(unsigned long));
    unsigned long* order = malloc(g->n*sizeof(unsigned long));
    unsigned long* freq = calloc(g->n,sizeof(unsigned long));
    unsigned long i,j,aux,maxfreq, numberlabels,freqlabel,currentfreq, node;
    
    if (labels==NULL || order ==NULL || labelusage == NULL || freq == NULL || nodelabels==NULL){
       printf("failed memory at first");                 
    }
    numberlabels=g->n;
    for (i=0;i <g->n; i++){
        labels[i]=i;
        nodelabels[i]=i;
        labelusage[i]=1;
        order[i]=i;
    }           
    int finish=0;
    while (!finish){
          //printf("%lu ", numberlabels);

        finish=1;
        //shuffle
        for (i=g->n-1; i>=1; i--){
            
            j= genrand64_int64()%(i+1);
            aux=order[i];
            order[i]=order[j];
            order[j]=aux;
            
            }
        printf("fin")    ;
        //label propagation    
        for (i=0;i<g->n; i++) {
            node= order[i];
            //calcul des frequence de chaque label
            for (j=g->cd[node]; j< g->cd[node+1];j++){
                freq[labels[nodelabels[g->adj[j]]]]++;
            }
            freq[labels[nodelabels[node]]]++;
            // Recher du label le plus frequent
            maxfreq=0;
            freqlabel=nodelabels[node];
            for (j=0; j< numberlabels ; j++){
                if (maxfreq< freq[labels[j]]){
                   maxfreq=freq[labels[j]];
                   freqlabel=j;
                   }
                if (labels[j]== labels[nodelabels[node]] ){
                   currentfreq = freq[labels[nodelabels[node]]];
                }
                freq[labels[j]]=0;
                   
            }
            if ((maxfreq==1 || currentfreq < maxfreq) && labels[nodelabels[node]]!=labels[freqlabel] ){
              if (-- labelusage[labels[nodelabels[node]]] ==0 ){ // decrease then compare
                
                 labels[nodelabels[node]]= labels[numberlabels-1];
                 //labelusage[nodelabels[node]]= labelusage[numberlabels-1];
                 numberlabels--;     
              }  
                         
              nodelabels[node]=freqlabel;   
              labelusage[labels[freqlabel]]++;
              finish=0;
               
                            
            }                
            
            
            
        }       
    }          
    free(freq);
    free(order);
    printf("There are %lu labels\n",numberlabels);
    for (j=0; j<g->n;j++){
        if (labelusage[labels[j]]>0)
        printf("Label %lu is used %lu times\n",labels[j],labelusage[labels[j]]);    
    }      
           
     
     
    free(labelusage);
    free(nodelabels);
    free(labels); 
}

int main(int argc,char** argv){

  time_t t1,t2 ;

  t1=time(NULL);

  adjlist* g;
  g=readedgelist(argv[1]);
  mkadjlist(g);


  printf("Number of nodes: %lu\n",g->n);
  printf("Number of edges: %lu\n",g->e);
	
  
  unsigned long i,k=0;
  for (i=0;i<g->n;i++){
      if (g->cd[i+1]-g->cd[i]==0){
         k++;
      }	
  }
  printf("k= %lu \n",k)    ;
  //labelPropagation(g);
labelPropagation2(g);
  t2=time(NULL);

  printf("=== Overall time label prop = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60)) ;

 

  free(g) ;

}


















void labelPropagation2(adjlist* g){ // version n^2
    unsigned long* labels = malloc(g->n*sizeof(unsigned long));
    unsigned long* order = malloc(g->n*sizeof(unsigned long));
    unsigned long* labelusage = malloc(g->n*sizeof(unsigned long));
    unsigned long* freq = calloc(g->n,sizeof(unsigned long));
    if (labels==NULL || order ==NULL || labelusage == NULL || freq == NULL){
       printf("failed memory at first");                 
    }
    unsigned long i,j,aux,maxfreq, freqlabel, node, nblabels;
    
    nblabels=g->n;
    printf("intital %lu ", nblabels); 
    for (i=0;i <g->n; i++){
        labels[i]=i;
        labelusage[i]=1;
        order[i]=i;
    }           
    int finish=0;
    int p=0;
    //printf("here");
    while (!finish){

        finish=1;
        //shuffle
        for (i=g->n-1; i>=1; i--){
            j= genrand64_int64()%(i+1);
            aux=order[i];
            order[i]=order[j];
            order[j]=aux;
            
            }
        //printf("f");    
        //label propagation    
        for (i=0;i<g->n; i++) {
            node= order[i];
            //calcul des frequence de chaque label
            for (j=g->cd[node]; j< g->cd[node+1];j++){
                freq[labels[g->adj[j]]]++;
            }
            freq[labels[node]]++;
            // Recher du label le plus frequent
            maxfreq=0;
            freqlabel=labels[node];
            for (j=0; j< g->n; j++){
                if (maxfreq< freq[j]){
                   maxfreq=freq[j];
                   freqlabel=j;
                   }
                //freq[j]=0;
                   
            }
            if ((maxfreq==1 || freq[labels[node]]<maxfreq) && labels[node]!=freqlabel){
               if (--labelusage[labels[node]] ==0){
                  nblabels--;                                                     
               }            
               labels[node]=freqlabel;
               labelusage[freqlabel]++;   
               finish=0;             
            }                
            
            for (j=0; j< g->n; j++){
                freq[j]=0;
            }
            
        }       
    }          
    printf("Number of labels used %lu", nblabels);      
    for (j=0; j<g->n;j++){
        if (labelusage[j]>0)
        printf("Label %lu is used %lu times\n",j,labelusage[j]);    
    }            
    for (j=0; j<g->n;j++){
          printf("node %lu has label  %lu\n",j,labels[j]);
    }
     
}
