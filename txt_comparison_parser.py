import matplotlib.pyplot as plt
from collections import defaultdict

def parse_data(file_path):
    with open(file_path, 'r') as file:
        data = file.read().strip().split('\n-------------------------\n')
    parsed_data = []

    for section in data:
        lines = section.split('\n')
        matrix_size = int(lines[1].split(': ')[1].split(',')[0])
        threads = int(lines[1].split(': ')[2].split(',')[0])
        block_size = int(lines[1].split(': ')[3].split(',')[0])
        speedup_implicit = float(lines[2].split(': ')[1])
        speedup_explicit = float(lines[3].split(': ')[1])

        parsed_data.append({
            'matrix_size': matrix_size,
            'threads': threads,
            'block_size': block_size,
            'speedup_implicit': speedup_implicit,
            'speedup_explicit': speedup_explicit
        })

    return parsed_data

def plot_speedup(parsed_data):
    fig, ax = plt.subplots()

    # Define colors and markers
    color_map = plt.cm.get_cmap('tab10')
    markers = ['o', 's', 'D', '^', 'v', '<', '>', 'p', '*', 'h']

    # Organize data for implicit speedup
    implicit_data = defaultdict(lambda: defaultdict(list))
    explicit_data = []

    matrix_sizes = sorted(set(entry['matrix_size'] for entry in parsed_data))
    matrix_size_map = {size: idx for idx, size in enumerate(matrix_sizes)}

    for entry in parsed_data:
        matrix_size = entry['matrix_size']
        block_size = entry['block_size']
        threads = entry['threads']
        speedup_implicit = entry['speedup_implicit']
        speedup_explicit = entry['speedup_explicit']

        # Collect implicit data for averaging
        implicit_data[matrix_size][block_size].append(speedup_implicit)

        # Collect explicit data
        explicit_data.append((matrix_size, block_size, threads, speedup_explicit))

    # Calculate average implicit speedup and plot
    plotted_implicit = set()
    for matrix_size, block_data in implicit_data.items():
        for block_size, speedups in block_data.items():
            avg_speedup = sum(speedups) / len(speedups)
            color_index = block_size % len(markers)  # Use block size for color differentiation
            if (matrix_size, block_size) not in plotted_implicit:
                ax.scatter(matrix_size_map[matrix_size], avg_speedup, color=color_map(color_index), marker='o',
                           label=f'Implicit Avg (Block size: {block_size})')
                plotted_implicit.add((matrix_size, block_size))

    # Plot OMP speedup with unique colors and markers
    plotted_omp = set()
    for i, (matrix_size, block_size, threads, speedup_explicit) in enumerate(explicit_data):
        color_index = i % len(color_map.colors)  # Ensure varied color selection
        marker_index = i % len(markers)
        if (matrix_size, threads, block_size) not in plotted_omp:
            ax.scatter(matrix_size_map[matrix_size], speedup_explicit, color=color_map(color_index), marker=markers[marker_index],
                       label=f'OMP (Threads: {threads}, Block size: {block_size})')
            plotted_omp.add((matrix_size, threads, block_size))

    # Set labels and title
    ax.set_xlabel('Matrix Size')
    ax.set_ylabel('Speedup')
    ax.set_title('Speedup vs Matrix Size')

    # Set x-ticks to display matrix sizes with equal spacing
    ax.set_xticks(list(matrix_size_map.values()))
    ax.set_xticklabels(matrix_sizes)

    # Create a legend
    handles, labels = ax.get_legend_handles_labels()
    by_label = dict(zip(labels, handles))
    ax.legend(by_label.values(), by_label.keys(), bbox_to_anchor=(1.05, 1), loc='upper left')

    # IEEE Style
    plt.grid(True, linestyle='--', linewidth=0.5)
    plt.tight_layout()

    # Save plot as a vector image
    plt.savefig('speedup_plot.svg', format='svg', bbox_inches='tight')

# Example usage
file_path = 'summary_comparison.txt'  # Replace with your actual file path
parsed_data = parse_data(file_path)
plot_speedup(parsed_data)
#export QT_QPA_PLATFORM=offscreen Fucking WSL doesn't support GUI so we need to disable it