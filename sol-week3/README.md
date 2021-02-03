There are three versions of solution:

matmul.*: which is the most basic version where the matrix dimension is equal to the number of hired processes

matmul_gen.*: it can handle the case where the matrix dimension is larger than the number of hired processes but it has to be divisible by the number of processes

matmul_mostgen.*: it can handle the case where the matrix dimension is larger than the number of hired processes (both divisible and not divisible)

I suggest you to investigate the solutions in the order above.
