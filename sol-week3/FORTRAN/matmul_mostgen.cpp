#include <iostream>
#include <iomanip>
#include <mpi.h>

using namespace std;

int main(int argc, char* argv[]){

  /* ========== Declaring Variables ========== */
  constexpr int N = 7;  // matrix dimension
  constexpr int nproc = 4;  // number of cores/processors
  // the number of rows in matrix A given to each process
  constexpr int row_size = N/nproc+(N%nproc!=0);
  // Augmneting a number of rows to A
  // (which will be junk but they will make our programming easier)
  constexpr int Nplus = row_size*nproc; 
  int A_rank[row_size][N];
  int C_rank[row_size][N];
  int B[N][N];
  int D[Nplus][N];
  int rank;
  int size;

// Initialize MPI and communicator
  MPI_Init(&argc, &argv);
  MPI_Comm comm = MPI_COMM_WORLD;

// Store index of process in variable "rank"
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

// Create matrix B in process 0
  if (rank == 0){
    cout<<"row_size =" << row_size << endl;	  
    cout<<"Matrix B: "<<endl;	
    for(int i=0; i<N; ++i){
      for(int j=0; j<N; ++j){
	B[i][j] = (j+i+2) * (N-j);
	cout << setw(7) << B[i][j];		
      }
      cout<<endl;	
    }
  }

// Broadcast the entire matrix B from process 0 to all ranks
  MPI_Bcast(B, N*N, MPI_INT, 0, comm); 

// Create the assigned rows of matrix A to each process
  for (int i=0; i<row_size; ++i){
    for (int j=0; j<N; ++j){
      A_rank[i][j] = (N-j+rank*row_size+i+1) * (rank*row_size+i+1);
    }
  } 


// Calculate the associated rows of each process for product A*B
  for (int k=0; k<row_size; ++k){
    for (int j=0; j<N; ++j){
      C_rank[k][j] = 0;	
      for (int i=0; i<N; ++i){
	C_rank[k][j] += A_rank[k][i] * B[i][j];
      }
    }
  }


// Collect the resulting rows in 2D array D in process 0
// Note: D has extra rows that needs to be discarded for final answer   
  MPI_Gather(C_rank, row_size*N, MPI_INT, D, row_size*N, MPI_INT, 0, comm);

// Print Matrix D up to Nth row (the rest of D is not wanted!)
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
