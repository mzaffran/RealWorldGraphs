#include "adjarray.c"

int main(int argc,char** argv){

  time_t t1,t2 ;

  t1=time(NULL);

  adjlist* g;
  g=readedgelist(argv[1]);
  mkadjlist(g);

  // Création d'une liste contenant les degrés de chaque noeud
  int i;
  unsigned long *d = calloc(g->n,sizeof(unsigned long)) ;
  unsigned long *names = calloc(g->n,sizeof(unsigned long)) ;
  for (i=0; i<g->n; i++) {
    d[i] = g->cd[i+1]-g->cd[i];
    names[i] = i;
  }

  int j = g->n ;
  int c = 0 ;
  unsigned long *num = calloc(g->n,sizeof(unsigned long)) ;
  unsigned long *coreness = calloc(g->n,sizeof(unsigned long)) ;
  int *visited = calloc(g->n,sizeof(unsigned long)) ; // visited[node] prendra la valeur 1 une fois que nous aurons visité node
  // Instanciation de variables
  unsigned long t ;
  unsigned long k ;
  unsigned long dmin ;
  unsigned long node ;
  unsigned long neighbor ;
  unsigned long l ;
  unsigned long last_visited = g->n+1;
  unsigned long max_iter ;

  while (j != 0){
    // Recherche du noeud de degré minimum
    dmin = g->e ;
    max_iter = (j==g->n)?(g->n):(j+1) ;
    for (t=0; t < max_iter ; t++){
      k = names[t] ;
      if ( (d[k]<dmin) && (visited[k]!=1)){
         node = k ;
         dmin = d[k] ;
      }
      if (k >= last_visited){
        names[t] = names[t+1];
      }
    }
    // Mise à jour des noeuds visités
    visited[node] = 1 ;
    // Calcul de la coreness
    c = (c>d[node])?c:d[node] ;
    // Mise à jour du degré des arrêtes (équivalent à supprimer les arrêtes du noeud courant)
    for (l=g->cd[node]; l<g->cd[node+1]; l++){
      neighbor = g->adj[l] ;
      if (visited[neighbor]!=1)
      {
        d[neighbor] = d[neighbor] - 1 ;
      }
    }
    // Numérotation du noeud courant
    num[node] = j ;
    // Enregistrement de la coreness du noeud courant
    coreness[node] = c ;
    // Mise à jour de la numérotation
    j-- ;
    last_visited = node ;
  }

  t2=time(NULL);

  printf("=== Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60)) ;

  printf("=== Core-value: %d\n", c) ;

  free(d) ;
  free(g) ;
  free(num) ;
  free(coreness) ;
  free(visited) ;
  return c ;
}
