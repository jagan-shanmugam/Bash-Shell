
//Header files
#include "headerfile.h"


//Function definition
void matrix(void)
{
	//Declarations and initialization	
	int m=5,k=5; 
	
	int MatA[m][k],MatB[m][k],Max[m][k],C[k];

	//Generating a random matrix A and B	
	generateRandMatrix(m,k,MatA);
	generateRandMatrix(m,k,MatB);

	//Compute the maximum matrix and sum of columns of maximum matrix
	max_matrix(m,k,MatA,MatB,Max);
	column_matrix(m,k,Max,C);

}

//Function to generate a random matrix
void generateRandMatrix(int m,int k,int MatB[m][k])
{	
	static int count=1;
	printf("Matrix %d :\n",count);
	
	//Assigning a random value to the matrix element by traversing the matrix	
	for(int i=0;i<m;i++)
	{ 
		for(int j=0;j<k;j++)
		{
			MatB[i][j]=rand()%36+1;
			printf("%d\t",MatB[i][j]);
			
		}
	printf("\n");
	}
	count++;
}

//Function to compute the maximum of two matrices
void max_matrix(int m, int k, int MatA[m][k], int MatB[m][k] , int Max[m][k])
{
	printf("Maximum matrix: \n"); 	

	//Finding the maximum value of each corresponding element of two matrices
	for(int i=0;i<m;i++)
	{ 		
		for(int j=0;j<k;j++)
		{
			Max[i][j]=MAX(MatA[i][j],MatB[i][j]);
			printf("%d\t",Max[i][j]);
			
		}
	printf("\n");
	}
}

//Function to compute the sum of columns in a matrix
void column_matrix(int m, int k, int Max[m][k], int C[k])
{

	printf("Sum of columns - Matrix:\n");

	//Computing the sum of columns in a matrix by traversing row-wise
	for(int i=0;i<m;i++)
	{		
		C[i]=0;
		for(int j=0;j<k;j++)
		{
			C[i]=C[i]+Max[j][i];
			
		}
		printf("%d\t",C[i]);
	}
	printf("\n");
}










