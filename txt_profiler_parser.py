# Print parsed data to verify
import matplotlib.pyplot as plt
import itertools
import numpy as np
import pprint

def parse_txt_data(file_path):
    data = {}
    current_matrix_size = None
    current_threads = None
    current_block_size = None

    with open(file_path, 'r') as file:
        for line in file:
            # Strip whitespace from the beginning and end of the line
            line = line.strip()

            # Check for matrix size line
            if line.startswith("Matrix size:"):
                parts = line.split(", ")
                current_matrix_size = int(parts[0].split(": ")[1])
                current_threads = int(parts[1].split(": ")[1])
                current_block_size = int(parts[2].split(": ")[1])

                # Initialize the dictionary structure
                if current_matrix_size not in data:
                    data[current_matrix_size] = {}
                if current_threads not in data[current_matrix_size]:
                    data[current_matrix_size][current_threads] = {}
                data[current_matrix_size][current_threads][current_block_size] = {}

            # Check for metric lines
            elif "|" in line:
                # Split line into parts
                parts = line.split("|")
                metric_name = parts[1].strip()
                if metric_name == "Metric":
                    continue
                sum_value = float(parts[2].strip())
                min_value = float(parts[3].strip())
                max_value = float(parts[4].strip())
                avg_value = float(parts[5].strip())

                # Store the parsed values in the data dictionary
                data[current_matrix_size][current_threads][current_block_size][metric_name] = {
                    'sum': sum_value,
                    'min': min_value,
                    'max': max_value,
                    'avg': avg_value
                }

    return data

def plot_metrics(data, x_param, y_metric, metric_type, free_params, output_file):
    fig, ax = plt.subplots()

    # Define colors and markers
    color_map = plt.cm.get_cmap('tab10')
    markers = ['o', 's', 'D', '^', 'v', '<', '>', 'p', '*', 'h']

    # Prepare for dynamic combination of free parameters
    unique_combinations = set()

    # Collect unique matrix sizes for equal spacing
    matrix_sizes = sorted(set(data.keys()))
    matrix_size_map = {size: idx for idx, size in enumerate(matrix_sizes)}

    # Loop over the data
    for matrix_size, threads_data in data.items():
        for threads, block_sizes in threads_data.items():
            for block_size, metrics in block_sizes.items():
                # Create a dictionary for the current scope
                current_context = {
                    'matrix_size': matrix_size,
                    'threads': threads,
                    'block_size': block_size
                }

                # Gather the values for the free parameters
                free_param_values = tuple(current_context[param] for param in free_params if param in current_context)

                # Map matrix size to its corresponding integer for equal spacing
                x_value = matrix_size_map[matrix_size]
                y_value = metrics[y_metric][metric_type]  # Use the specified type of the metric

                # Add the combination to the set
                unique_combinations.add(free_param_values)

                # Determine the index for color and marker
                combination_index = list(unique_combinations).index(free_param_values)
                color_index = combination_index % 10
                marker_index = combination_index % len(markers)

                # Plot each point
                ax.scatter(x_value, y_value, 
                           color=color_map(color_index), 
                           marker=markers[marker_index],
                           label=', '.join(f'{param}: {value}' for param, value in zip(free_params, free_param_values)))

    # Set labels and title
    ax.set_xlabel(x_param.capitalize())
    ax.set_ylabel(f'{y_metric} ({metric_type})')
    ax.set_title(f'{y_metric} vs {x_param}')

    # Set x-ticks to display matrix sizes with equal spacing
    ax.set_xticks(list(matrix_size_map.values()))
    ax.set_xticklabels(matrix_sizes)

    # Create a legend with unique combinations
    handles, labels = ax.get_legend_handles_labels()
    by_label = dict(zip(labels, handles))
    ax.legend(by_label.values(), by_label.keys(), bbox_to_anchor=(1.05, 1), loc='upper left')

    # IEEE Style
    plt.grid(True, linestyle='--', linewidth=0.5)
    plt.tight_layout()

    # Save plot as a vector image
    plt.savefig(output_file, format='svg', bbox_inches='tight')  # Change `format` to 'pdf' if needed
def plot_metrics_with_baseline(data, x_param, y_metric, metric_type, free_params, output_file, baseline_x, baseline_y):
    fig, ax = plt.subplots()

    # Define colors and markers
    color_map = plt.cm.get_cmap('tab10')
    markers = ['o', 's', 'D', '^', 'v', '<', '>', 'p', '*', 'h']

    # Prepare for dynamic combination of free parameters
    unique_combinations = set()

    # Collect unique matrix sizes for equal spacing
    matrix_sizes = sorted(set(data.keys()))
    matrix_size_map = {size: idx for idx, size in enumerate(matrix_sizes)}

    # Plot the baseline
    baseline_x_mapped = [matrix_size_map[str(x)] for x in baseline_x]  # Map baseline x values to the categorical axis
    ax.plot(baseline_x_mapped, baseline_y, color='black', linestyle='--', marker='x', label='Baseline')

    # Loop over the data
    for matrix_size, threads_data in data.items():
        for threads, block_sizes in threads_data.items():
            for block_size, metrics in block_sizes.items():
                # Create a dictionary for the current scope
                current_context = {
                    'matrix_size': matrix_size,
                    'threads': threads,
                    'block_size': block_size
                }

                # Gather the values for the free parameters
                free_param_values = tuple(current_context[param] for param in free_params if param in current_context)

                # Map matrix size to its corresponding integer for equal spacing
                x_value = matrix_size_map[matrix_size]
                y_value = metrics[y_metric][metric_type]  # Use the specified type of the metric

                # Add the combination to the set
                unique_combinations.add(free_param_values)

                # Determine the index for color and marker
                combination_index = list(unique_combinations).index(free_param_values)
                color_index = combination_index % 10
                marker_index = combination_index % len(markers)

                # Plot each point
                ax.scatter(x_value, y_value, 
                           color=color_map(color_index), 
                           marker=markers[marker_index],
                           label=', '.join(f'{param}: {value}' for param, value in zip(free_params, free_param_values)))

    # Set labels and title
    ax.set_xlabel(x_param.capitalize())
    ax.set_ylabel(f'{y_metric} ({metric_type})')
    ax.set_title(f'{y_metric} vs {x_param}')

    # Set x-ticks to display matrix sizes with equal spacing
    ax.set_xticks(list(matrix_size_map.values()))
    ax.set_xticklabels(matrix_sizes)

    # Create a legend with unique combinations
    handles, labels = ax.get_legend_handles_labels()
    by_label = dict(zip(labels, handles))
    ax.legend(by_label.values(), by_label.keys(), bbox_to_anchor=(1.05, 1), loc='upper left')

    # IEEE Style
    plt.grid(True, linestyle='--', linewidth=0.5)
    plt.tight_layout()

    # Save plot as a vector image
    plt.savefig(output_file, format='svg', bbox_inches='tight')  # Change `format` to 'pdf' if needed
# Example usage
file_path = 'summary_profiler.txt'  # Path to your text file
parsed_data = parse_txt_data(file_path)

plot_metrics(parsed_data, x_param='matrix_size', 
    y_metric='L2 miss rate STAT', 
    metric_type='avg',
    free_params=['threads', 'block_size'],
    output_file='plot_profiler.svg')
#export QT_QPA_PLATFORM=offscreen Fucking WSL doesn't support GUI so we need to disable it