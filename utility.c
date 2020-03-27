//compute the maximum of three unsigned long
unsigned long max3(unsigned long a,unsigned long b,unsigned long c){
	a=(a>b) ? a : b;
	return (a>c) ? a : c;
}

unsigned long min (unsigned long a,unsigned long b){
       return (a<b) ? a : b;
}
