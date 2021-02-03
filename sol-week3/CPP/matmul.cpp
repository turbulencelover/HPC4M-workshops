#include <iostream>
#include <iomanip>
#include <mpi.h>

using namespace std;

int main(int argc, char* argv[]){

  /* ========== Declaring Variables ========== */
  constexpr int N = 4;  // matrix dimension
  constexpr int nproc = N;  // number of cores/processors 
  
  int A_rank[N];
  int C_rank[N];
  int B[N][N];
  int D[N][N];
  int rank;
  int size;


// Initialize MPI and communicator
  MPI_Init(&argc, &argv);
  MPI_Comm comm = MPI_COMM_WORLD;

// store index of process in variable "rank"
  MPI_Comm_rank(comm, &rank);

// Store number of processes in variable "size"
  MPI_Comm_size(comm, &size);

// If number of process hired by the user (size) does not match 'nproc' -> abort!
  if(size != nproc) {
    cout<<"nproc = "<<nproc<<" does not fit number of processes requested which is "
	<<size<<". Restart with matching number!"<<endl;
    MPI_Abort(comm, 911);
    MPI_Finalize();
    return 0;
  }

// If 'nproc' not equal to N (this code cannot handle it) -> abort!
  if(N != nproc) {
    cout<<"number of processes = "<<nproc<<" , which is not equal to matrix dimension";
    cout<<"Sorry cannot handle this case!";
    MPI_Abort(comm, 911);
    MPI_Finalize();
    return 0;
  }
  

// Create matrix B in process 0
  if (rank == 0){
    cout<<"Matrix B: "<<endl;	
    for(int i=0; i<N; ++i){
      for(int j=0; j<N; ++j){
	B[i][j] = (j+i+2) * (N-j);
	cout << setw(7) << B[i][j];		
      }
      cout<<endl;	
    }
  }

// Create the assigned rows of matrix A to each process
  for (int j=0; j<N; ++j){
      A_rank[j] = (N-j+rank+1) * (rank+1);
  } 

// Broadcast the entire matrix B from process 0 to all ranks
  MPI_Bcast(B, N*N, MPI_INT, 0, comm); 
 
// Calculate the associated rows of each process for product A*B
  for (int j=0; j<N; ++j){
    C_rank[j] = 0;	
    for (int i=0; i<N; ++i){
      C_rank[j] += A_rank[i] * B[i][j];
    }
  }

// collect the resulting rows in 2D array D on process 0
  MPI_Gather(C_rank, N, MPI_INT, D, N, MPI_INT, 0, comm);

// Print the final answer : Matrix D 
  if (rank == 0){
    cout<<"\nThe resulting Matrix D is: "<<endl;
    for (int i=0; i<N; ++i){
      for (int j=0; j<N; ++j){
	cout << setw(7) << D[i][j];		
      }
      cout << endl;
    }
  }
  
MPI_Finalize();
 
}
