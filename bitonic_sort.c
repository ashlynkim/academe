#include<stdio.h>					
#include<mpi.h>					
		
					
int compare_exchange_min(int pair, int num, int process_rank){					
	int toPass=0;				
	int temp;				
				
	MPI_Send(				
		&num, // Send values that are greater than min			
		1, // number of items sent			
		MPI_INT, // INT			
		process_rank ^ (1 << pair), // paired process calc by XOR with 1 shifted left j positions	
		0, 			
		MPI_COMM_WORLD 			
	);				
					
  	MPI_Recv(				
		&temp, // buffer the message			
		1, // one item received		
		MPI_INT, // INT			
		process_rank ^ (1 << pair), // paired process calc by XOR with 1 shifted left j positions	
		0, 			
		MPI_COMM_WORLD, 			
		MPI_STATUS_IGNORE 			
	);				
					
           if(num < temp){					
toPass= num; 	// toPass gets the smaller value			
  	}					
          else {	 				
              toPass= temp; 					
             }	  
      return toPass;				
 }					
					
int compare_exchange_max(int pair,int num, int process_rank){					
					
	int temp, toPass=0; 				
				
 	MPI_Recv(				
		&temp, 			
		1,  //one item received				
		MPI_INT, // INT			
		process_rank ^ (1 << pair), // paired process calc by XOR with 1 shifted left j positions	
		0, 			
		MPI_COMM_WORLD, 			
		MPI_STATUS_IGNORE 			
		);			
					
 	MPI_Send(				
		&num, 			
		1,  // number of items sent			
		MPI_INT, // INT			
		process_rank ^ (1 << pair), // paired process calc by XOR with 1 shifted left j positions	
		0, 			
		MPI_COMM_WORLD 			
		);			
					
           if(num > temp){					
                  toPass=num;		// toPass gets the higher value				
                }					
       else {			
            toPass=temp;					
            }					
        return toPass;					
}					
					
					
int main(int argc, char * argv[]) {					

int d=4; 					
int arrayNum[16] = {95, 90, 3,   8,       9, 23, 18, 40,       12,  0, 60,  5,    10, 14, 20, 35}; 	
int process_rank;					
int num_processes;					
int retrieve; 	 				

double temp_partner;
int partner; 
int test_bit1, bit1; 
int test_bit2, bit2;
				
   int i, j, k=4, l;					
  					
 // for bit identification					
   int bitArray[4];					
   int rem=0;					
  				
 //for MPI     					
    MPI_Init(&argc, &argv);					
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);					
    MPI_Comm_rank(MPI_COMM_WORLD, &process_rank);					
 					
      					
	for (i=0; i<d; i++){				
					
	   for(j=i; j>=0; j--){				
		
			//determine which bits to compare		
			
		 	partner= process_rank ^ (1 << j);
			test_bit1 = process_rank >> (i+1); // get the (i+1)th bit of process_rank 
			bit1= test_bit1 & 1; 
			
			test_bit2 = process_rank >> j; // get the jth bit of process_rank 
			bit2 = test_bit2 & 1; 			
		printf("Processing process %d and comparing with %d \n", process_rank, partner);
	     if(bit1 == bit2) {			
			retrieve=    compare_exchange_min(j, arrayNum[process_rank], process_rank);		
			 }						
		 else{									
			retrieve=   compare_exchange_max(j,arrayNum[process_rank], process_rank );		
			   }		
		//assign to array retrieved value			
		arrayNum[process_rank]= retrieve;			
	   }
	}	   
					
 MPI_Finalize();			
 
//printing the sorted result					
	printf("Sorted Array[%d]: %d\n", process_rank,arrayNum[process_rank]);			
				
				
return 0;					
}	
