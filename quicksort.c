#include<stdio.h>
#include<stdlib.h>
#include"mpi.h"
#include<time.h>
#include <sys/time.h>

void swap(int* a, int* b);
int partition (int arr[], int low, int high);
void quickSort(int arr[], int low, int high);
void divide(int arr[],int low,int high, int p, int small[],int* big);
int binarySearch(int arr[], int l, int r, int x);


int main (int argc ,char **argv )
{
        int i,j,l, myid ,numprocs,size ;
		int* gpivot=(int*)malloc(sizeof(int));
        int n=atoi(argv[1]);
        int* data = malloc(n*sizeof(int));
		MPI_Status status ;

        MPI_Init(&argc , &argv ) ;
        MPI_Comm_size(MPI_COMM_WORLD, & numprocs ) ;
        MPI_Comm_rank(MPI_COMM_WORLD, & myid ) ;

		struct timeval tb;
		struct timeval ta;
		
    
        srand(23*(myid+5));
		
        for( j = 0 ; j < n ; j++ ) {
                data[j] = rand()%5000;
        }
        //call quicksort
		gettimeofday(&tb, NULL); 
        quickSort(data,0,n-1);
		
		
		
		
		MPI_Comm newcomm;
		MPI_Comm_split(MPI_COMM_WORLD, 1, myid, &newcomm);
        int num = numprocs;
		while(num>1){

			MPI_Comm_size(newcomm, &numprocs);
			MPI_Comm_rank(newcomm, &myid);
			if(myid ==0){
					*gpivot = data[n/2];
			}
			//broadcast pivot to all;
			MPI_Bcast(gpivot, 1, MPI_INT,0, newcomm);
			int mid = binarySearch(data,0,n,*gpivot);
			
			int *small;
			int *their;
			int *big;
			small =  malloc((mid+1)*sizeof(int));
			their =  malloc((mid+1)*sizeof(int));
			big = malloc((n-(mid+1))*sizeof(int));
			j=0;
			
			for (i=0;i<=mid;i++){
					small[i]=data[i];
			}

			for(i=mid+1;i<n;i++){
					big[j]=data[i];
					j++;
			}
			
			int* midt=(int*)malloc(sizeof(int));
			int* t =(int*)malloc(sizeof(int));
			if(myid<numprocs/2){
				*t = n-(mid+1);
				MPI_Send ( t , 1 ,MPI_INT, myid+(numprocs/2), 11 ,newcomm) ;
				MPI_Send ( big , *t ,MPI_INT, myid+(numprocs/2) , 11 ,newcomm) ;
				
						
				MPI_Recv ( midt , 1 ,MPI_INT, myid+(numprocs/2) , 11 ,newcomm,&status ) ;
				big = realloc(big,(*midt)*sizeof(int));
				MPI_Recv ( big , *midt ,MPI_INT, myid+(numprocs/2) , 11 ,newcomm,&status ) ;
				
			}
			else{
				MPI_Recv ( midt , 1 ,MPI_INT, myid-(numprocs/2) , 11 ,newcomm,&status ) ;
				their = realloc(their,(*midt)*sizeof(int));
				MPI_Recv ( their , *midt ,MPI_INT, myid-(numprocs/2) , 11 ,newcomm,&status ) ;		
				
				*t = mid +1;				
				MPI_Send ( t , 1 ,MPI_INT, myid-(numprocs/2) , 11 ,newcomm) ;
				MPI_Send ( small , *t ,MPI_INT, myid-(numprocs/2), 11 ,newcomm) ;
	
			}
			if(myid<numprocs/2){
				
				
				size = *midt+mid+1;
				data  =  realloc(data, size*sizeof(int));
				i=0,j=0,l=0;
				while(i<mid+1 && j<*midt){
					if(small[i]<=big[j]){
						data[l] = small[i];
						i++;
					}else{
						data[l] = big[j];
						j++;
					}
					
					l++;
									
				}
				while (i<mid+1){
					data[l] = small[i];
					l++;
					i++;
				}
				while(j<*midt){
					data[l] = big[j];
					l++;
					j++;				
				}
				
				

			}
			else{		
				size = *midt+n-(mid+1);
				data  =  realloc(data, size*sizeof(int));
				i=0,j=0,l=0;
				while(i<*midt && j< n-(mid+1)){
					if(their[i]<=big[j]){
						data[l] = their[i];
						i++;
					}else{
						data[l] = big[j];
						j++;
					}
					l++;
				}
				while (i<*midt){
					data[l] = their[i];
					l++;
					i++;
				}
				while(j<n-(mid+1)){
					data[l] = big[j];
					l++;
					j++;				
				}
				
			
				
			}
			n=size;
			
			MPI_Comm_split(newcomm, myid < numprocs/2 , myid, &newcomm);
			num/=2;

		}
		
			

		MPI_Comm_rank(MPI_COMM_WORLD, &myid);
		
		gettimeofday(&ta, NULL); 
		printf("\n Time taken for execution on processor = %d is : %ld microseconds\n",myid,
				((ta.tv_sec*1000000+ta.tv_usec)-(tb.tv_sec*1000000+tb.tv_usec)));
		
		//test if data is sorted correctly
		printf("Process %d: ", myid);
		for (j=0;j<n-1;j++){
			if(data[j]>data[j+1]){
				printf("~~~prev=%d, curr=%d, next=%d ",data[i-1],data[i],data[i+1]);
			}
		}
		printf("Size: %d, first: %d and last: %d ", n,data[0],data[n-1]);	
		printf("\n");
			
		 
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
  int m;

	while( r - l > 1 )
	{
		m = l + (r - l)/2;

		if( arr[m] <= x )
			l = m;
		else
			r = m;
	}

	return l;
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

void divide(int arr[],int low,int high,int p,int small[],int* big){
        int i;
        int j=0;
        for (i=low;i<=p;i++){
                small[i]=arr[i];
        }

        for(i=p+1;i<=high;i++){
                big[j]=arr[i];
                j++;
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