# Benchmarking the Performance of Implicit and Explicit Parallel Implementations of the Matrix Transpose
The repository contains the code for the benchmarking of the performance of the matrix transpose operation comparing the sequential, implicit parallel using the SIMD instructions and explicit parallel using the OpenMP library. The cache performance and the memory bandwidth are measured on the HPC cluster using the Likwid performance analysis tool.
## Toolchain Used
- `GCC 9.1.0` (module `gcc91` on the HPC Cluster)
- `OpenMP 4.5`
- `Likwid 4.3.4`
- `GNU Make 3.8.2`
- PBS job scheduler for the HPC cluster
## Running Locally Using the Scripts
### 1. Clone the repository or download the code and navigate to the directory
```bash
git clone https://github.com/timmfy/parco-d1
```
### 2. Enable the scripts to run
```bash
chmod +x scripts/*
```
### 3. Run the code
```bash
./scripts/run_locally.sh [OPTIONS]
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
    --symm <int>                    Generate a symmetric matrix (1 to generate the symmetric matrix, 0 to generate a random one, default: 0)
```
With no options, the script runs the test for the block size 2<sup>4</sup> x 2<sup>4</sup>, the threads 2<sup>2</sup>, the matrix size 2<sup>10</sup> x 2<sup>10</sup>, and does 1 run.
```bash
./scripts/run_locally.sh
```
Example that runs the test for the block sizes 2<sup>4</sup>, 2<sup>5</sup>, 2<sup>6</sup> and for 2<sup>2</sup>, 2<sup>3</sup>, 2<sup>4</sup> threads , the matrix sizes 2<sup>10</sup>, 2<sup>11</sup>, 2<sup>12</sup>, and does 5 runs:
```bash
./scripts/run_locally.sh --block-size-list 4,5,6 --threads-list 2,3,4 --size-list 10,11,12 --runs 5
```
Example that runs the test for the block size 2<sup>6</sup>, the matrix size 2<sup>12</sup>, and does 20 runs with likwid-perfctr profiling of the group CACHES for
the implicit parallel implementation:
```bash
./scripts/run_locally.sh --profiling imp --runs 20 --block-size-list 6 --size-list 12 --group CACHES
```
Note that the profiling option requires the likwid to be installed (see the section Running on the HPC Cluster).
## Running Locally Using the Manual Compilation
It is recommended to use the Makefile to compile the code:
```bash
make all N=<size>
```
where `<size>` is the size of the square matrix (a power of 2) that is used for the benchmarking.
Without the Makefile, the following sequence of commands can be used:
```bash
gcc -fopenmp -DN=<size> -O2 -Iinclude -c src/seq.c -o seq.o
gcc -fopenmp -DN=<size> -O2 -ftree-vectorize -funroll-loops -fopt-info -Iinclude -c src/imp_par.c -o imp_par.o
gcc -fopenmp -DN=<size> -Iinclude -c src/omp_par.c -o omp_par.o
gcc -fopenmp -DN=<size> -Iinclude -c src/seq_test.c -o seq_test.o
gcc -fopenmp -DN=<size> -Iinclude -c src/imp_par_test.c -o imp_par_test.o
gcc -fopenmp -DN=<size> -Iinclude -c src/omp_par_test.c -o omp_par_test.o
gcc -fopenmp -DN=<size> -Iinclude -c src/main.c -o main.o
gcc -fopenmp main.o seq.o imp_par.o omp_par.o seq_test.o imp_par_test.o omp_par_test.o -o main
```
Then, run with the following command:
```bash
./main --block-size <blockSize> --runs <runs> --symm <symm> <tests> --threads <threads>
```
where `<blockSize>` is the block size for the cache blocking/cache oblivious algorithms, `<runs>` is the number of runs, `<threads>` is the number of threads for OpenMP, and `<symm>` is a flag that generates a symmetric matrix.
<tests> is a flag that specifies which tests to run (seq (`-s`) for the sequential test, imp (`-i`) for the implicit parallel test, omp (`-o`) for the OpenMP test). The tests can be combined (e.g., `-si` for the sequential and implicit parallel tests or `-sio` for all tests).
For example, the following command runs the sequential and OMP parallel tests with the block size 16, 5 runs, and 4 threads:
```bash
./main --block-size 4 --runs 5 --symm 0 -si --threads 2
```
The standard output is the time in seconds that it took to transpose the matrix.
## Running on the HPC Cluster
Running the code on the HPC cluster requires either an interactive session or the use of a PBS scheduler.
### Running with an interactive session
The following command requests an interactive session with 64 cores (which is the maximum number of cores that is used for the benchmarks in the project)
```bash
qsub -I -q short_cpuQ -l select=1:ncpus=64:mem=1gb
```
Download the necessary modules and create an alias for the gcc command
```bash
module load gcc91
module load likwid-4.3.4
gcc() { gcc-9.1.0 "$@"; }
```
Set the number of threads for OpenMP
```bash
export OMP_NUM_THREADS=64
```
Then, it is possible to proceed the same way as running locally either with the scripts or the manual compilation.

### Running with a PBS scheduler
The following command creates a pbs script `parco-d1-job.pbs` that runs the code with the specified options
```bash
./scripts/generate_pbs.sh [OPTIONS]
```
where `[OPTIONS]` are the same as for the local execution.

Then, submit the job to the scheduler
```bash
qsub parco-d1-job.pbs
```

## Output and Results
The output is stored in the following files:
1. `parco-d1-job.out` contains the output of the job (if run on the HPC cluster)
2. `parco-d1-job.err` contains the compiler messages and errors (if run on the HPC cluster)
3. `summary_comparison.txt` contains the average speedup for each configuration and input parameters (if run with multiple runs using a script)
4. `summary_profiling.txt` contains the performance metrics for the configuration that was run with profiling (if run with profiling using a script)
The `parco-d1-job.out` file (if exists) contains also the redirected output of the code execution (for example, the gcc optimization information).
