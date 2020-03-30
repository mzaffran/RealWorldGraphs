#include "adjarray.c"

int main(int argc,char** argv){
	adjlist* g;
	time_t t1,t2;

	t1=time(NULL);

	printf("Reading edgelist from file %s\n",argv[1]);
	g=readedgelist(argv[1]);
	printf("Number of edges: %lu\n",g->e);

    //clean(g);

    //mergeSort(g->edges,0, g->e-1);
    //edge *B =malloc (g->e*sizeof(edge));
    printf("Start sorting");
    //BottomUpMergeSort(g->edges, B, g->e);
    //free(B);
    printf("Finished sorting\n");
    //duplicates(g);


	printf("Number of nodes: %lu\n",g->n);
	printf("Number of edges: %lu\n",g->e);

	printf("Building the adjacency list\n");
	mkadjlist(g);
	printf("finished Building the adjacency list\n");


	t2=time(NULL);

	printf("- Overall time = %ldh%ldm%lds\n",(t2-t1)/3600,((t2-t1)%3600)/60,((t2-t1)%60));



	free_adjlist(g);
	return 0;
}
