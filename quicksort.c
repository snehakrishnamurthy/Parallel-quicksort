#include<stdio.h>
#include<stdlib.h>
#include"mpi.h"

void swap(int* a, int* b);
int partition (int arr[], int low, int high);
void quickSort(int arr[], int low, int high);
void divide(int arr[],int low,int high, int p, int small[],int big[]);
int binarySearch(int arr[], int l, int r, int x);

int cs = 0;
int cb = 0;

int main (int argc ,char **argv )
{
	int i,j,l, myid ,gpivot,numprocs ;
	int n=10;
	int data[n];

	MPI_Init(&argc , &argv ) ;
	MPI_Comm_size(MPI_COMM_WORLD, & numprocs ) ;
	MPI_Comm_rank(MPI_COMM_WORLD, & myid ) ;

	
	srand(23*(myid+5));
	
	for( j = 0 ; j < n ; j++ ) {
		data[j] = rand()%50;
	}
	
	
	printf("\n");
			


	//call quicksort
	
	quickSort(data,0,n-1);
	printf("Process %d: ", myid);
	for(j =0 ;j<n;j++){
		printf("%d ", data[j]);
	}
	printf("\n");
		
	
	if(myid ==0){
		gpivot = data[n/2];
	}
	//broadcast pivot to all;
	MPI_Bcast(&gpivot, 1, MPI_INT,0, MPI_COMM_WORLD);
	
	int mid = binarySearch(data,0,n-1,gpivot);
	printf("mid: %d",res);
	int small[mid+1];
	int big[n-(mid+1)]
	divide(data,0,n-1,mid,small,big);
	printf("Process %d: ", myid);
	for(j =0 ;j<cs;j++){
		printf("%d ", small[j]);
	}
	printf("\n");
	printf("Process %d: ", myid);
	for(j =0 ;j<cb;j++){
		printf("%d ", big[j]);
	}
	printf("\n");
	free(data);
	
	MPI_Finalize();
}

void swap(int* a, int* b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

int binarySearch(int arr[], int l, int r, int x)
{
  while (l <= r)
  {
    int m = (l + r)/2;
 
    // Check if x is present at mid
    if (arr[m] <= x && arr[m+1]>x) 
        return m;  
 
    // If x greater, ignore left half  
    if (arr[m] < x) 
        l = m + 1; 
 
    // If x is smaller, ignore right half 
    else
         r = m - 1; 
  }
 
  // if we reach here, then element was not present
  return -1; 
}

int partition (int arr[], int low, int high)
{
	int k = (low - 1);  // Index of smaller element
	int pivot = arr[high];
	int l;
	for (l = low; l <= high- 1; l++)
	{
		// If current element is smaller than or
		// equal to pivot
		if (arr[l] <= pivot)
		{
			k++;    // increment index of smaller element
			swap(&arr[k], &arr[l]);
		}
	}
	swap(&arr[k + 1], &arr[high]);
	return (k + 1);
}

void divide(int arr[],int low,int high,int p,int small[],int big[]){
	int i;
	int j=0;
	for (i=low;i<=p;i++){
		small[i]=arr[i];
	}
	for(i=p+1;i<=high;i++){
		big[j++]=arr[i];
	}
}

void quickSort(int arr[], int low, int high)
{
	if (low < high)
	{
		int pi = partition(arr, low, high);
 
		quickSort(arr, low, pi - 1);
		quickSort(arr, pi + 1, high);
	}
}
