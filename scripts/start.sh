#!/bin/bash

# Default values
nRange=(0 0 0 0 0 0 0 0 0 0 1 0 0) # Default matrix size 1024
tRange=(0 0 1 0 0 0 0) # Default number of threads 4
bsRange=(0 0 0 0 1 0 0 0 0 0 0 0 0) # Default block size 16
numRuns=1  # Default number of runs
tests="-sio" # Default tests to run
genSym=0  # Generate symmetric matrix (0 by default)
profiling="none"
group="MEM"

# Function to display help
show_help() {
    echo "Usage: $0 [OPTION]..."
    echo "Options:"
    echo "  --block-size-list <int,int,...> Run the test for the list of block sizes 2^<int> (default: 2^4)"
    echo "  -h --help              Display this information"
    echo " --group <string>     Set the performance group for likwid (default: CACHES, possible values: MEM, L2CACHE, L3CACHE, L2, L3, DATA, CACHES)"
    echo " --threads-list <int,int,...> Run the test for the list of threads 2^<int> (default: 2^2, max: 2^6)"
    echo " --profiling <string> Run the specified test with profiling (seq, imp, omp)"
    echo " --size-list <int,int,...> Run the test for the list of sizes 2^<int> (default: 2^10, max: 2^12)"
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
        --size-list)
            if [[ -n $2 ]]; then
                nRange=(0 0 0 0 0 0 0 0 0 0 0 0 0)
                for i in $(echo $2 | sed "s/,/ /g"); do
                    if [[ $i -le 0 || $i -gt 12 ]]; then
                        echo "Error: Invalid size"
                        exit 1
                    fi
                    nRange[$i]=1
                done
                shift
            else
                echo "Error: --size-range flag requires an argument"
                exit 1
            fi
            ;;
        --block-size-list)
            if [[ -n $2 ]]; then
                bsRange=(0 0 0 0 0 0 0 0 0 0 0 0 0)
                for i in $(echo $2 | sed "s/,/ /g"); do
                    if [[ $i -le 0 || $i -gt 12 ]]; then
                        echo "Error: Invalid block size"
                        exit 1
                    fi
                    bsRange[$i]=1
                done
                shift
            else
                echo "Error: --block-size-range flag requires an argument"
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
        --threads-list)
            if [[ -n $2 ]]; then
                tRange=(0 0 0 0 0 0 0)
                for i in $(echo $2 | sed "s/,/ /g"); do
                    if [[ $i -le 0 || $i -gt 6 ]]; then
                        echo "Error: Invalid number of threads"
                        exit 1
                    fi
                    tRange[$i]=1
                done
                shift
            else
                echo "Error: --threads-range flag requires an argument"
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

# Check if the block size range is in the selected matrix size range
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
    tRange=(0 1 0 0 0 0 0)
    bsRange=(1 0 0 0 0 0 0 0 0 0 0 0 0)
elif [[ $profiling == "imp" ]]; then
    tests="-i"
    tRange=(0 1 0 0 0 0 0)
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
if [[ $group == "CACHES" ]]; then
    metrics=("L1 load bandwidth" "L1 to/from L2 bandwidth" " L1 to L2 evict bandwidth") 
fi