//
//AHMET SALTAN --MARUF EMRE KARATAY --CANER KAYA
//CSE-232 
//Systems Programming 
//Term Project
//

#include <stdio.h>
#include <string.h>


struct symbol_table {
int type;
// 0-integer variable 1-vector 2-matrix
char name[10];
// array/variable name
char dim1;
// size of dimension1 (may also be a char array)
char dim2;
// size of dimension2 (may also be a char array)
};
// maximum 20 arrays/variables

int stct=-1;
struct symbol_table ST[20];


int searchST(char* name){
	int x;
	for(x = 0;x < stct + 1; x++){
		if(strcmp(ST[x].name,name) == 0)
			return x;
	}
	return -1;
}




int main(){
	
	FILE *file = fopen ( "file.txt", "r" );
	FILE *exp = fopen ( "expanded.c", "a" );
	
	char *token; 
	char tokenarr[7][128];
	int tokenct = 0, filect = 1;
	
  	if ( file != NULL )
   	{
      	char line [ 128 ]; /* or other suitable maximum line size */

      	while ( fgets ( line, sizeof line, file ) != NULL ){ /* read a line */
		
			if(line[0] == '@'){
				
				stct++;
				token = strtok(line," ,\n");

				int i=0;
				while( token != NULL ) {
					 
					strcpy(tokenarr[i],token);
					tokenct++;
					printf( " %s ", tokenarr[i] );
					token = strtok(NULL, " ,\n");
					i++;
					
					if(token == NULL){	
						
						printf("%d", tokenct);
						i=0;
							
						if(strcmp(tokenarr[0],"@def") == 0){
							
							fprintf(exp,"#define %s %s\n",tokenarr[1],tokenarr[2]);
							strcpy(ST[stct].name,tokenarr[1]);
							ST[stct].dim1=tokenarr[2][0];
							
						}
						
						else if(strcmp(tokenarr[0],"@mat") == 0){
							
							fprintf(exp,"int %s[%s][%s];\n",tokenarr[1],tokenarr[2],tokenarr[3]);
							ST[stct].type=2;
							strcpy(ST[stct].name,tokenarr[1]);
							ST[stct].dim1=tokenarr[2][0];
							ST[stct].dim2=tokenarr[3][0];
			
						}
						
						else if(strcmp(tokenarr[0],"@vec") == 0){
							
							fprintf(exp,"int %s[%s];\n",tokenarr[1],tokenarr[2]);
							ST[stct].type=1;
							strcpy(ST[stct].name,tokenarr[1]);
							ST[stct].dim1=tokenarr[2][0];
							
						}
						
						else if(strcmp(tokenarr[0],"@int") == 0){
							
							fprintf(exp,"int %s;\n",tokenarr[1]);
							ST[stct].type=0;
							strcpy(ST[stct].name,tokenarr[1]);
							ST[stct].dim1=tokenarr[2][0];
							
						}
						
						else{
							
							int length = strlen(tokenarr[0]);
							strncpy(tokenarr[0],tokenarr[0]+1,length);
							
							if(ST[searchST(tokenarr[0])].type == 2){			// RESULTS ARE MATRIX
					
								if(ST[searchST(tokenarr[2])].type == 0){
									
									fprintf(exp,"for (int i=0; i < %c;i++)\n\tfor(int j=0; j < %c;j++)\n\t\t %s[i][j]=%s*%s[i][j];\n",ST[searchST(tokenarr[0])].dim1,ST[searchST(tokenarr[0])].dim2,ST[searchST(tokenarr[0])].name,ST[searchST(tokenarr[2])].name,ST[searchST(tokenarr[4])].name);
									
								}
								
								else if(ST[searchST(tokenarr[2])].type == 2){
									
									if(strcmp(tokenarr[3],"*") == 0){
										
										fprintf(exp,"for(int i=0;i<%c; i++)\n\tfor(int j=0;j<%c;j++){\n\t\t%s[i][j]=0;\n\t\tfor(int k=0;k<%c;k++)\n\t\t\t%s[i][j]+= %s[i][k] * %s[k][j];\n\t}\n",ST[searchST(tokenarr[2])].dim1,ST[searchST(tokenarr[4])].dim2,ST[searchST(tokenarr[0])].name,ST[searchST(tokenarr[2])].dim2,ST[searchST(tokenarr[0])].name,ST[searchST(tokenarr[2])].name,ST[searchST(tokenarr[4])].name);
									
									}
									else{
										
										fprintf(exp, "for(int i = 0; i < %c; i++)\n\tfor(int j = 0; j < %c; j++)\n\t\t%s[i][j] = %s[i][j] %s %s[i][j];\n", ST[searchST(tokenarr[0])].dim1, ST[searchST(tokenarr[0])].dim2, ST[searchST(tokenarr[0])].name, ST[searchST(tokenarr[2])].name, tokenarr[3], ST[searchST(tokenarr[4])].name);
									
									}
									
								}
								
							}
							
							else if(ST[searchST(tokenarr[0])].type == 1){		//RESULTS ARE VECTOR
								
								if(ST[searchST(tokenarr[2])].type == 0){
									
									fprintf(exp, "for(int i = 0; i < %c; i++)\n\t%s[i] = %s * %s[i];\n",ST[searchST(tokenarr[4])].dim1, ST[searchST(tokenarr[0])].name, ST[searchST(tokenarr[2])].name, ST[searchST(tokenarr[4])].name);
								
								}
								
								else if(ST[searchST(tokenarr[2])].type == 1){
									
									fprintf(exp, "for(int i = 0; i < %c; i++)\n\t%s[i] = %s[i] %s %s[i];\n", ST[searchST(tokenarr[0])].dim1,  ST[searchST(tokenarr[0])].name, ST[searchST(tokenarr[2])].name, tokenarr[3], ST[searchST(tokenarr[4])].name);
								
								}
								
								else{
									
									if(tokenct > 5){
										
										
										fprintf(exp,"for(int i=0;i<%c; i++)\n\tfor(int j=0;j<1;j++){\n\t\t%s[i]=0;\n\t\tfor(int k=0;k<%c;k++)\n\t\t\t%s[i]+= %s[i][k] * %s[k];\n\t\t%s[i] += %s[i];\n\t}\n", ST[searchST(tokenarr[2])].dim1,ST[searchST(tokenarr[0])].name,ST[searchST(tokenarr[2])].dim2,ST[searchST(tokenarr[0])].name,ST[searchST(tokenarr[2])].name,ST[searchST(tokenarr[4])].name, ST[searchST(tokenarr[0])].name, ST[searchST(tokenarr[6])].name);
									}
									
									else{
										
										fprintf(exp,"for(int i=0;i<%c; i++)\n\tfor(int j=0;j<1;j++){\n\t\t%s[i]=0;\n\t\tfor(int k=0;k<%c;k++)\n\t\t\t%s[i]+= %s[i][k] * %s[k];\n\t}\n", ST[searchST(tokenarr[2])].dim1,ST[searchST(tokenarr[0])].name,ST[searchST(tokenarr[2])].dim2,ST[searchST(tokenarr[0])].name,ST[searchST(tokenarr[2])].name,ST[searchST(tokenarr[4])].name);
									
									}
								
								}
							
							}
							
							else if(strcmp(tokenarr[0], "read") == 0){
								
								if(ST[searchST(tokenarr[1])].type == 1){  // VECTOR READ
								
									fprintf(exp, "FILE *f%d;\nf%d = fopen(\"%s\",\"r\");\nfor(int i = 0; i < %c; i++)\n\tfscanf(f%d,\"%%d\", &%s[i]);\n", filect, filect, tokenarr[3], ST[searchST(tokenarr[1])].dim1, filect, ST[searchST(tokenarr[1])].name);
									filect++;
									
								}
								else{					// MATRIX READ

									fprintf(exp, "FILE *f%d;\nf%d = fopen(\"%s\", \"r\");\nfor(int i = 0; i < %c; i++)\n\tfor(int j = 0; j < %c; j++)\n\t\tfscanf(f%d, \"%%d\", &%s[i][j]);\n", filect, filect, tokenarr[3], ST[searchST(tokenarr[1])].dim1, ST[searchST(tokenarr[1])].dim2, filect, ST[searchST(tokenarr[1])].name);
									
									filect++;
								}
								
							}
							
							else if(strcmp(tokenarr[0], "print") == 0){
								
								if(ST[searchST(tokenarr[1])].type == 1){ // VECTOR PRINT
									
									fprintf(exp, "for(int i = 0; i < %c; i++)\n\tprintf(\"%%d\\n\", %s[i]);\n", ST[searchST(tokenarr[1])].dim1, ST[searchST(tokenarr[1])].name);
									
								}
								
								else {								// MATRIX PRINT
									
									fprintf(exp, "for(int i = 0; i < %c; i++)\n\tfor(int j = 0; j < %c; j++)\n\t\tprintf(\"%%d\\n\", %s[i][j]);\n", ST[searchST(tokenarr[1])].dim1, ST[searchST(tokenarr[1])].dim2, ST[searchST(tokenarr[1])].name);
								
								}
							
							}
						
						}
						tokenct = 0;
						
					}
				
				}

			}
			
			else{
				
					fprintf(exp, "%s", line);
					
			}
		
		}
		
		fclose ( file );
		fclose ( exp );
   	}
   	else
   	{
      		perror ( "file.txt" ); /* why didn't the file open? */
   	}






	return 0;	

}
