#include <stdlib.h>
#include <stdio.h>

void normmi (unsigned long* labels1, unsigned long* labels2, unsigned long n, double* mi, double* nmi, double* ari);

int main(int argc,char** argv){

    double mi,nmi,ari;
	unsigned long n,u,v,i,j;
    n=0;
    
	FILE *file=fopen(argv[1], "r");
	while (fscanf(file,"%lu %lu", &u, &v)==2){
      n++;
    }
    
    fclose(file);
    printf("n= %lu\n",n);
    unsigned long* labels1=calloc(n,sizeof(unsigned long));
    unsigned long* labels2=calloc(n,sizeof(unsigned long));
    
    if (labels1==NULL || labels2 == NULL)
       printf("memory failure compare.c");
  	file=fopen(argv[1], "r");
	for (j=0; j<n ; j++){ 
      fscanf(file,"%lu %lu", &u, &labels1[j]);
      labels1[j]--;                  
    }  
    
    fclose(file);
    
                 
                 
    file=fopen(argv[2], "r");
	for (j=0; j<n ; j++){ 
      fscanf(file,"%lu %lu", &u, &labels2[j]);
      
      labels2[j]--;                    
    }  
 
    fclose(file);
    

    normmi(labels1, labels2, n, &mi, &nmi, &ari);
    printf("mi = %lf nmi = %lf ari %lf\n",mi ,nmi,ari);                       
    
}

//normalized mututal information
void normmi (unsigned long* labels1, unsigned long* labels2, unsigned long n, double* mi, double* nmi, double* ari){
    unsigned long i,j,nl1,nl2;
    double h1,h2;
    *mi=0;
    
    nl1=0; //numbre of labels for 1st classification
    nl2=0; //numbre of labels for 2nd classification
    for (i=0; i<n; i++){
        if (labels1[i]>nl1) {
           nl1= labels1[i] ;
        }   
        if (labels2[i]>nl2){
           nl2= labels2[i]; 
        }
    }
    nl1++;
    nl2++;
    printf("nl1 %lu nl2 %lu\n",nl1 ,nl2);
    
    unsigned long* c= calloc(nl1*nl2,sizeof(unsigned long));
    if (c==NULL){
       printf("failed malloc at nmi");
    }
    unsigned long* labelusage1= calloc(nl1,sizeof(unsigned long));
    unsigned long* labelusage2= calloc(nl2,sizeof(unsigned long));
    
    for (i=0; i<n; i++){
        c[nl2*labels1[i]+labels2[i]]++;   
        labelusage1[labels1[i]]++;
        labelusage2[labels2[i]]++; 
    }
    
    
    double y=0;
    for (i=0; i<nl1; i++){
        for (j=0; j<nl2; j++){
            if (c[nl2*i+j]>0){
               y+= (double) c[nl2*i+j]*log ((double) n*c[nl2*i+j] /( labelusage1[i]*labelusage2[j] ) )/log(2) ;  
            } 
        }
    }
    
    *mi = y/n;
    
    h1=0; h2=0;
    for (i=0;i<nl1;i++){
        h1+= (double) labelusage1[i] * log ((double) labelusage1[i]/n)/log(2);
    }
    for (i=0;i<nl2;i++){
        
        h2+=(double) labelusage2[i] * log ((double) labelusage2[i]/n)/log(2);
    }

    *nmi= -2* y/(h1+h2);
    
    
    
    *ari=0;
    double x=0,z=0,a=0,b=0;
    
    for (i=0; i<nl1; i++){
        
        if (labelusage1[i]>1){
           a+=   (labelusage1[i]*labelusage1[i]-1)/2    ;               
        }
    }
    for (j=0; j<nl2; j++){
        if (labelusage2[j]>1){
           b+=   (labelusage2[j]*labelusage2[j]-1)/2  ;                 
        }
    }
    for (i=0; i<nl1; i++){
        for (j=0; j<nl2; j++){
            if (c[nl2*i+j]>1){
               x+= c[nl2*i+j]*(c[nl2*i+j]-1)/2;
            
            }                 
        }
    }
    z=(a+b)/2-2*a*b/n*(n-1);
    *ari= (x-2*a*b/n*(n-1)) / z;                           
}   



