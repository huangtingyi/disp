#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>

int main(int argc, char *argv[])
{
	int i,j,c,iGB=0;
	char *p;
	
	for (;(c = getopt(argc, argv, "g:?:")) != EOF;){

		switch (c){
		case 'g':
			iGB = atoi(optarg);
			break;
		case '?':
		default:
			printf("Usage: %s \n", argv[0]);
			printf("   [-g gB ]\n");
		}
	}
	
	for(j=0;j<iGB;j++){

		for(i=0;i<1024;i++){
			if((p=(char*)malloc(1024*1024))==NULL){
				printf("malloc error .\n");
				exit(1);
			}
			bzero((void*)p,1024*1024);
		}
		
		printf("had malloc %dGB memory press any key continue.\n",j);
		
		getchar();
	}
	
	return 0;
}