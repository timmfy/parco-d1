# Benchmarking the Performance of Implicit and Explicit Parallel Implementations of the Matrix Transpose


## Instructions to run the code

### 1. Clone the repository or download the code
### 2. Enable the scripts to run
```bash
chmod +x start.sh runner.sh
```
### 3. Run the code
```bash
./start.sh [OPTIONS]
```
where `[OPTIONS]` are among the following:
```
    --block-size-list <int,int,...>  Run the test for the list of block sizes 2^<int> (default: 2^4), a parameter
    of the block size for cache blocking/cache oblivious algorithms
    -h, --help                      Display this information
    --group <string>                Set the performance group for likwid (default: CACHES)
    --threads-list <int,int,...>    Run the test for the list of numbers of threads 2^<int> (default: 2^2, max: 2^6) (a parameter for OpenMP)
    --profiling <string>            Run the specified test with profiling (seq, imp, omp)
    --size-list <int,int,...>       Run the test for the list of sizes 2^<int> (default: 2^10, max: 2^12)
    --runs <int>                    Set the number of runs (default: 1)
    --symm <int>                    Generate a symmetric matrix (1 to generate the symmetric matrix, 0 to generate a random, default: 0)
```
With no options, the script runs the test for the block size 2<sup>4</sup> x 2<sup>4</sup>, the threads 2<sup>2</sup>, the matrix size 2<sup>10</sup> x 2<sup>10</sup>, and does 1 run.
```bash
./start.sh
```
Example that runs the test for the block sizes 2<sup>4</sup>, 2<sup>5</sup>, 2<sup>6</sup>, 2<sup>2</sup>, 2<sup>3</sup>, 2<sup>4</sup> threads , the matrix sizes 2<sup>10</sup>, 2<sup>11</sup>, 2<sup>12</sup>, and does 5 runs:
```bash
./start.sh --block-size-list 4,5,6 --threads-list 2,3,4 --size-list 10,11,12 --runs 5
```
Example that runs the test for the block size 2<sup>6</sup>, the matrix size 2<sup>12</sup>, and does 20 runs with likwid-perfctr profiling of the group CACHES for
the implicit parallel implementation:
```bash
./start.sh --profiling imp --runs 20 --block-size-list 6 --size-list 12 --group CACHES
```
### 4. The code generates the parco-d1-job.pbs file that can be submitted to the cluster
```bash
qsub parco-d1-job.pbs
```
### 5. The code generates the output files in the working directory
1. 'parco-d1-job.out' contains the output of the job
2. 'parco-d1-job.err' contains the compiler messages and errors
3. 'summary_comparison.txt' contains the average speedup for each configuration and input parameters
4. 'summary_profiling.txt' contains the performance metrics for the configuration that was run with profiling
   
The content of the summary files is also redirected to the 'parco-d1-job.out' file.
