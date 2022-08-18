# File - graph_builder_grid.py
# Author - Alex Smith (SmithAlexLee30@gmail.com)
# Description - Creates and writes a grid graph (adjacency matrix) to a formatted file.
# Date - 2022-08-17

from dataclasses import dataclass
from sys import argv, exit, stderr
from os import read
from typing import List
from networkx import grid_2d_graph, convert_node_labels_to_integers, to_dict_of_dicts

@dataclass
class Arguments:
    """A class of expected arguments from the user."""
    rows: int
    columns: int
    coupling_strength: float

def get_args(args: List[str]) -> Arguments:
    """Attempts to parse the command-line arguments."""
    arguments = None

    if len(args) != 4:
        usage(args[0])

    try:
        args[1] = int(args[1])
        args[2] = int(args[2])
        args[3] = float(args[3])

        arguments = Arguments(*args[1:])
    except (TypeError, ValueError):
        usage(args[0])
    
    return arguments

def usage(prog_name: str) -> None:
    """Raises an error and prompts the user with the expected arguments."""
    raise SystemExit(
        f"\nUsage: {prog_name} [# rows] [# columns] [coupling strength]\n"
        f"\tCoupling strength should be <= 1"
    )

def main() -> None:
    """The entry point of this module."""
    # Verify command-line arguments.
    args = get_args(argv)

    filepath = f"Graph/{args.rows}x{args.columns}"
    nodeCount = args.rows * args.columns

    # Create 2D graph and relabel to integers instead of coordinates.
    grid_graph = convert_node_labels_to_integers(grid_2d_graph(args.rows, args.columns))

    # Convert the graph to a dict-of-dicts
    adj_mat = to_dict_of_dicts(grid_graph, edge_data=args.coupling_strength)

    # Write the adjacency matrix to the file.
    with open(filepath, "w") as file:
        # Write the number of nodes.
        file.write("# {0}\n".format(nodeCount))

        # Write the edges.
        for r in range(nodeCount):
            for c in range(nodeCount):
                if r in adj_mat and c in adj_mat[r]:
                    file.write(str(adj_mat[r][c]) + " ")
                else:
                    file.write("0 ")
            file.write("\n")
    
    # Output the save path of the generated graph file.
    print(f"Graph has been saved to: {filepath}")

if __name__ == "__main__":
    main()

# Three different ways of traversing a graph or adjacency matrix.
# for node1, adj in adj_mat.items():
#     for node2, weight in adj.items():
#         print("({0} {1}) = {2}".format(node1, node2, weight))
# 
# for n, nbrsdict in G.adjacency():
#     for nbr, eattr in nbrsdict.items():
# 
# for node1, node2, weight in grid_graph.edges.data("weight"):
#     print("({0} {1}) = {2}".format(node1, node2, weight))
#     # if weight is not None:
#     #     pass