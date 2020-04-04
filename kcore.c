#include "adjarray.c"
#include <string.h>
int kcore(adjlist* g, unsigned long *degree, unsigned long *coreness);

void saveKCore(char* input_name, adjlist* g, unsigned long *degree, unsigned long *coreness) ;

int main(int argc,char** argv){

  time_t t1,t2,t3,t4;

  t1=time(NULL);

  adjlist* g;
  g=specificreadedgelist(argv[1]);

  clean(g);

  edge *B = malloc (g->e*sizeof(edge));
  printf("Start sorting\n");

  BottomUpMergeSort(g->edges,B, g->e);

  free(B);
  printf("Finished sorting\n");
  duplicates(g);

      
  printf("Number of nodes: %lu\n",g->n);
  printf("Number of edges: %lu\n",g->e);
  mkadjlist(g);

  t2=time(NULL);
	printf("=== Graph preparation time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));

  unsigned long *degree = calloc(g->n,sizeof(unsigned long)) ;
  unsigned long *coreness = calloc(g->n,sizeof(unsigned long)) ;
  
  if (degree==NULL || coreness==NULL){
     printf("Memory failed at degree or corennes")         ;        
  }
  int c = kcore(g, degree, coreness);

  t3=time(NULL);
	printf("=== K-Core time = %ldh%ldm%lds\n",(t3-t2)/3600,((t3-t2)%3600)/60,((t3-t2)%60));
  printf("=== Overall k-core time = %ldh%ldm%lds\n",(t3-t1)/3600,((t3-t1)%3600)/60,((t3-t1)%60));

  printf("=== Core-value: %d\n", c) ;

  printf("=== Saving the results\n");

  saveKCore(argv[1], g, degree, coreness) ;

  t4=time(NULL);
	printf("Saving time = %ldh%ldm%lds\n",(t4-t3)/3600,((t4-t3)%3600)/60,((t4-t3)%60));

  printf("=== Overall time = %ldh%ldm%lds\n",(t4-t1)/3600,((t4-t1)%3600)/60,((t4-t1)%60));

  free(degree) ;
  free(coreness) ;

  return c ;
}

int kcore(adjlist* g, unsigned long *degree, unsigned long *coreness){
  // Création d'une liste contenant les degrés de chaque noeud
  int i;
  int max_degree = 0 ;
  unsigned long *degree_nb = calloc(g->e,sizeof(unsigned long)) ;
  for (i=0; i<g->n; i++) {
    degree[i] = g->cd[i+1]-g->cd[i];
    degree_nb[degree[i]]++ ;
    if (degree[i] > max_degree)
      {
        max_degree = degree[i] ;
      }
  }

  unsigned long *init = calloc(max_degree,sizeof(unsigned long)) ;
  int j = 0 ;
  for (j=1; j<=max_degree; j++) {
    init[j] = init[j-1]+degree_nb[j-1] ;
  }

  unsigned long *ordered_nodes = calloc(g->n,sizeof(unsigned long)) ;
  unsigned long *nodes_position = calloc(g->n,sizeof(unsigned long)) ;
  unsigned long *degree_compteur = calloc(g->e,sizeof(unsigned long)) ;

  int k = 0 ;
  for (k=0; k<g->n; k++) {
    ordered_nodes[init[degree[k]]+degree_compteur[degree[k]]] = k ;
    nodes_position[k] = init[degree[k]]+degree_compteur[degree[k]] ;
    degree_compteur[degree[k]]++ ;
  }

  int c = 0 ;
  unsigned long *num = calloc(g->n,sizeof(unsigned long)) ;
  int *visited = calloc(g->n,sizeof(unsigned long)) ; // visited[node] prendra la valeur 1 une fois que nous aurons visité node

  int l = 0 ;
  int node_number = g->n ;
  unsigned long node ;
  unsigned long neighbor ;
  unsigned long true_init ;
  unsigned long swapper ;
  unsigned long swapper_new_position ;
  int m ;
  for (l=0; l<g->n; l++){
    // Récupération du noeud de degré minimum
    node = ordered_nodes[l] ;
    // Numérotation du noeud courant
    num[node] = node_number ;
    // Mise à jour des noeuds visités
    visited[node] = 1 ;
    // Calcul de la coreness
    c = (c>degree[node])?c:degree[node] ;
    // Enregistrement de la coreness du noeud courant
    coreness[node] = c ;
    // Mise à jour des différents tableaux
    degree_compteur[degree[node]]-- ;
    init[degree[node]]++ ;
    // Mise à jour du degré des arrêtes (équivalent à supprimer les arrêtes du noeud courant)
    for (m=g->cd[node]; m<g->cd[node+1]; m++){
      neighbor = g->adj[m] ;
      if (visited[neighbor]!=1)
      {
        true_init = (init[degree[neighbor]])<(l+1)?(l+1):init[degree[neighbor]] ;
        swapper = ordered_nodes[true_init] ;
        swapper_new_position = nodes_position[neighbor] ;
        ordered_nodes[true_init] = neighbor ;
        nodes_position[neighbor] = true_init ;
        ordered_nodes[swapper_new_position] = swapper ;
        nodes_position[swapper] = swapper_new_position ;
        degree_compteur[degree[neighbor]]-- ;
        init[degree[neighbor]]++ ;
        degree[neighbor] = degree[neighbor] - 1 ;
        degree_compteur[degree[neighbor]]++ ;
      }
    }
    // Mise à jour de la numérotation
    node_number-- ;
  }

  free(ordered_nodes) ;
  free(init) ;
  free(degree_compteur) ;
  free(degree_nb) ;
  free(num) ;
  free(visited) ;
 

  return c ;
}

void saveKCore(char* input_name, adjlist* g, unsigned long *degree, unsigned long *coreness){
  char name[100] = "results/kcore/";

  char* file = input_name;
  
  int len = strlen(input_name)-4 ;

  char * filename = strndup(file, len );
  
  strcat(name, filename);
  strcat(name, "_kcore.txt") ;

  
  FILE *results = fopen(name, "w") ;

  fprintf(results,"ID;Degree;Coreness;\n");

  unsigned long i;
  for (i=0;i<g->n;i++)
  {
    fprintf(results, "%lu;%lu;%lu;\n", i, degree[i], coreness[i]);
  }

  fclose(results);
}
