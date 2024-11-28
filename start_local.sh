#!/bin/bash

# Default values
#Ranges of values
nRange=(0 0 0 0 0 0 0 0 0 0 1 0 0)
tRange=(0 1 1 1 0 0 0)
bsRange=(0 0 0 1 1 1 0 0 0 0 0 0 0)
blockSize=16  # Default block size
numRuns=1  # Default number of runs
tests="-sio" # Default tests to run
genSym=0  # Generate symmetric matrix (0 by default)
profiling=""

summary_file="summary.txt"
> "$summary_file"

group="MEM"

# Function to display help
show_help() {
    echo "Usage: $0 [OPTION]..."
    echo "Options:"
    echo "  --block-size-range <int> <int> Run the test for the range of block sizes 2^<int> to 2^<int> (default: 2^4 2^4)"
    echo "  -h --help              Display this information"
    echo " --group <string>     Set the performance group for likwid (default: CACHES)"
    echo " --threads-range <int int> Run the test for the range of threads 2^<int> to 2^<int> (default: 2^1 2^3, max: 2^6)"
    echo " --profiling <string> Run the specified test with profiling (seq, imp, omp)"
    echo " --size-range <int int> Run the test for the range of sizes 2^<int> to 2^<int> (default: 2^10 2^10)"
    echo "  --runs <int>        Set the number of runs (default: 1)"
    echo "  --symm <int>        Generate a symmetric matrix (default: 0)"
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -h|--help)
            show_help
            exit 0
            ;;
        --size-range)
            if [[ -n $2 ]] && [[ -n $3 ]]; then
                if [[ $2 -le 0 || $2 -gt 12 || $3 -le 0 || $3 -gt 12 || $2 -gt $3 ]]; then
                    echo "Error: Invalid matrix size range"
                    exit 1
                fi
                nRange=(0 0 0 0 0 0 0 0 0 0 0 0 0)
                for (( i=$2; i<=$3; i++ )); do
                    nRange[$i]=1
                done
                shift
                shift
            else
                echo "Error: --size-range flag requires two arguments"
                exit 1
            fi
            ;;
        --block-size-range)
            if [[ -n $2 ]] && [[ -n $3 ]]; then
                if [[ $2 -le 0 || $2 -gt 12 || $3 -le 0 || $3 -gt 12 || $2 -gt $3 ]]; then
                    echo "Error: Invalid block size range"
                    exit 1
                fi
                bsRange=(0 0 0 0 0 0 0 0 0 0 0 0 0)
                for (( i=$2; i<=$3; i++ )); do
                    bsRange[$i]=1
                done
                shift
                shift
            else
                echo "Error: --block-size-range flag requires two arguments"
                exit 1
            fi
            ;;
        --group)
            if [[ -n $2 ]]; then
                group=$2
                shift
            else
                echo "Error: --group flag requires an argument"
                exit 1
            fi
            ;;
        --profiling)
            if [[ -n $2 ]]; then
                profiling=$2
                if [[ $profiling != "seq" && $profiling != "imp" && $profiling != "omp" ]]; then
                    echo "Error: Invalid profiling argument"
                    exit 1
                fi
                shift
            else
                echo "Error: --profiling flag requires an argument"
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
        --threads-range)
            if [[ -n $2 ]] && [[ -n $3 ]]; then
                if [[ $2 -le 0 || $2 -gt 6 || $3 -le 0 || $3 -gt 6 ]]; then
                    echo "Error: Invalid thread range"
                    exit 1
                fi
                tRange=(0 0 0 0 0 0 0)
                for (( i=$2; i<=$3; i++ )); do
                    tRange[$i]=1
                done
                shift
                shift
            else
                echo "Error: --threads-range flag requires two arguments"
                exit 1
            fi
            ;;
        *)
            echo "Error: Unknown argument '$1'"
            exit 1
            ;;
    esac
    shift
done

#Check if the block size range is in the selected matrix size range
maxBlockSize=0
for i in "${!bsRange[@]}"; do
    if [[ ${bsRange[$i]} -eq 1 ]]; then
        maxBlockSize=$((2**$i))
    fi
done
for i in "${!nRange[@]}"; do
    if [[ ${nRange[$i]} -eq 1 ]]; then
        N=$((2**$i))
        if [[ $maxBlockSize -gt $N ]]; then
            echo "Error: Block size must be less than or equal to matrix size"
            exit 1
        fi
    fi
done

if [[ $profiling == "seq" ]]; then
    tests="-s"
    tRange=(1 0 0 0 0 0 0)
    bsRange=(0 0 0 0 0 0 0 0 0 0 0 0 0)
elif [[ $profiling == "imp" ]]; then
    tests="-i"
    tRange=(1 0 0 0 0 0 0)
elif [[ $profiling == "omp" ]]; then
    tests="-o"
fi

metrics=(".*")
if [[ $group == "MEM" ]]; then
    metrics=("memory bandwidth")
fi
if [[ $group == "L2CACHE" ]] || [[ $group == "L3CACHE" ]]; then
    metrics=("miss rate" "miss ratio") 
fi
if [[ $group == "L2" ]] || [[ $group == "L3" ]]; then
    metrics=("bandwidth") 
fi
if [[ $group == "DATA" ]]; then
    metrics=("ratio") 
fi

pattern=$(IFS=\|; echo "${metrics[*]}")

for i in "${!nRange[@]}"; do
    if [[ ${nRange[$i]} -eq 1 ]]; then
        N=$((2**$i))
        if [[ $blockSize -gt $N ]]; then
            echo "Error: Block size must be less than or equal to matrix size"
            exit 1
        fi
    fi
done

if [[ $profiling == "" ]]; then
    #Compile and run for the nRange
    echo "Profiling: $profiling"
    echo "Tests to run: $tests"
    for i in "${!nRange[@]}"; do
        echo ${nRange[$i]}
        if [[ ${nRange[$i]} -eq 1 ]]; then
            for j in "${!bsRange[@]}"; do
                if [[ ${bsRange[$j]} -eq 1 ]]; then
                    for k in "${!tRange[@]}"; do
                        if [[ ${tRange[$k]} -eq 1 ]]; then
                            N=$((2**$i))
                            blockSize=$((2**$j))
                            threads=$((2**$k))
                            echo "Number of runs: $numRuns"
                            echo "-------------------------"
                            echo "Compiling..."
                            echo "-------------------------"
                            make all N=$N
                            echo "-------------------------"
                            echo "Running tests..."
                            echo "Matrix size: $N, Threads: $threads", Block size: $blockSize
                            echo "-------------------------"
                            echo "-------------------------" >> "$summary_file"
                            echo "Matrix size: $N, Threads: $threads", Block size: $blockSize >> "$summary_file"
                            ./bin/main --block-size $blockSize --runs $numRuns --symm $genSym $tests --threads $threads 2>&1 | tail -n 3 >> "$summary_file"
                            make clean
                        fi
                    done
                fi
            done
        fi
    done
else
    #Compile and profile with likwid the selected test
    #for threads and sizes
    echo ${nRange[@]}
    echo ${tRange[@]}
    echo "Tests to run: $tests"
    for i in "${!nRange[@]}"; do
        if [[ ${nRange[$i]} -eq 1 ]]; then
            N=$((2**$i))
            for j in "${!tRange[@]}"; do
                if [[ ${tRange[$j]} -eq 1 ]]; then
                    for k in "${!bsRange[@]}"; do
                        if [[ ${bsRange[$k]} -eq 1 ]]; then
                            blockSize=$((2**$k))
                            threads=$((2**$j))
                            echo "Number of runs: $numRuns"
                            echo "-------------------------"
                            echo "Compiling..."
                            echo "-------------------------"
                            make all N=$N
                            echo "-------------------------"
                            echo "Running tests..."
                            echo "Matrix size: $N, Threads: $threads", Block size: $blockSize
                            echo "-------------------------"
                            echo "-------------------------" >> "$summary_file"
                            echo "Matrix size: $N, Threads: $threads", Block size: $blockSize >> "$summary_file"
                            likwid-perfctr -V 0 -C 0-$(($threads-1)) -g $group ./bin/main --block-size $blockSize --runs $numRuns --symm $genSym $tests --threads $threads | grep "Sum\|STAT" | grep -v "Event\|_" | grep -Ei "$pattern|Sum" >> "$summary_file"
                            make clean
                        fi
                    done
                fi
            done
        fi
    done
fi
make clean
echo "Summary:"
cat "$summary_file"
