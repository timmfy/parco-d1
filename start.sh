#!/bin/bash

# Default values
N=10  # Default matrix size (2^10)
blockSize=4  # Default block size
numRuns=1  # Default number of runs
test="all"  # Default test to run
genSym=0  # Generate symmetric matrix (0 by default)

# Function to display help
show_help() {
    echo "Usage: $0 [OPTION]..."
    echo "Options:"
    echo "  --block-size <int>  Set the block size to <int> (default: 4)"
    echo "  --help              Display this information"
    echo "  --runs <int>        Set the number of runs (default: 1)"
    echo "  --size <int>        Set the matrix size to 2^<int> (default: 2^10)"
    echo "  --symm <int>        Generate a symmetric matrix (default: 0)"
    echo "  --test <string>     Select the test to run (default: 'all') ('seq' for sequential implementation, 'imp' for implicit parallel implementation, 'exp' for explicit parallel implementation)"
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        --help)
            show_help
            exit 0
            ;;
        --size)
            if [[ -n $2 ]]; then
                N=$((2**$2))
                shift
            else
                echo "Error: --size flag requires an argument"
                exit 1
            fi
            ;;
        --block-size)
            if [[ -n $2 ]]; then
                blockSize=$2
                shift
            else
                echo "Error: --block-size flag requires an argument"
                exit 1
            fi
            ;;
        --test)
            if [[ -n $2 ]]; then
                test=$2
                shift
            else
                echo "Error: --test flag requires an argument"
                exit 1
            fi
            ;;
        --runs)
            if [[ -n $2 ]]; then
                numRuns=$2
                if [[ $numRuns -le 0 ]]; then
                    echo "Error: Number of runs must be greater than 0"
                    exit 1
                fi
                shift
            else
                echo "Error: --runs flag requires an argument"
                exit 1
            fi
            ;;
        --symm)
            if [[ -n $2 ]]; then
                genSym=$2
                if [[ $genSym -ne 0 && $genSym -ne 1 ]]; then
                    echo "Error: Invalid argument value"
                    exit 1
                fi
                shift
            else
                echo "Error: --symm flag requires an argument"
                exit 1
            fi
            ;;
        *)
            echo "Error: Invalid flag"
            exit 1
            ;;
    esac
    shift
done


if [[ $N -le 1 || $N -gt 4096 ]]; then
    echo "Error: Invalid matrix size"
    exit 1
fi

if [[ $blockSize -le 1 || $blockSize -gt 4096 ]]; then
    echo "Error: Invalid block size"
    exit 1
fi

echo "Number of runs: $numRuns"
echo "Matrix size: 2^$N"
cat <<EOL > parco-d1-job.pbs
#!/bin/bash
#PBS -N parco-d1-job
#PBS -o ./parco-d1-job.out
#PBS -e ./parco-d1-job.err
#PBS -q short_cpuQ
#PBS -l walltime=0:03:00
#PBS -l select=1:ncpus=1:mem=1mb
module load gcc91
cd ${PWD}
make all
echo "Running tests..."
./bin/main --size $N --runs $numRuns --block-size $blockSize --symm $genSym --test $test
make clean

EOL