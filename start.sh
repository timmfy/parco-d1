#!/bin/bash

# Default values
N=1024  # Default matrix size (2^10)
blockSize=4  # Default block size
numRuns=1  # Default number of runs
tests="-" 
genSym=0  # Generate symmetric matrix (0 by default)
threads=4 # Default number of threads

# Function to display help
show_help() {
    echo "Usage: $0 [OPTION]..."
    echo "Options:"
    echo "  -a                  Run all tests"
    echo "  --block-size <int>  Set the block size to <int> (default: 4)"
    echo "  -e                  Run explicit parallel test"
    echo "  -h --help              Display this information"
    echo "  -i                  Run implicit parallel test"
    echo " --threads <int>     Set the number of threads (default: 4)"
    echo " --profiling <string> Run the specified test with profiling"
    echo "  --runs <int>        Set the number of runs (default: 1)"
    echo "  --size <int>        Set the matrix size to 2^<int> (default: 2^10)"
    echo "  --symm <int>        Generate a symmetric matrix (default: 0)"
    echo "  -s                  Run sequential test"
   
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
        --threads)
            if [[ -n $2 ]]; then
                threads=$2
                if [[ $threads -le 0 ]]; then
                    echo "Error: Number of threads must be greater than 0"
                    exit 1
                fi
                shift
            else
                echo "Error: --threads flag requires an argument"
                exit 1
            fi
            ;;
        -?*)
            for (( i=1; i<${#1}; i++ )); do
                case ${1:i:1} in
                    a)
                        test_seq=1
                        test_imp=1
                        test_exp=1
                        ;;
                    s)
                        test_seq=1
                        ;;
                    i)
                        test_imp=1
                        ;;
                    e)
                        test_exp=1
                        ;;
                    h)
                        show_help
                        exit 0
                        ;;
                    *)
                        echo "Error: Invalid test option '${1:i:1}'"
                        exit 1
                        ;;
                esac
            done
            ;;
        *)
            echo "Error: Unknown argument '$1'"
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

if [[ $test_seq -ne 0 ]]; then
    tests+="s"
fi
if [[ $test_imp -ne 0 ]]; then
    tests+="i"
fi
if [[ $test_exp -ne 0 ]]; then
    tests+="e"
fi
if [[ $tests == "-" ]]; then
    tests="-sie"
fi

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
echo "Number of runs: $numRuns"
echo "-------------------------"
echo "Compiling..."
echo "-------------------------"
make all N=$N
echo "-------------------------"
echo "Running tests..."
echo "-------------------------"
./bin/main --block-size $blockSize --runs $numRuns --symm $genSym $tests --threads $threads
make clean

EOL