from sys import argv, exit, stderr
from os import read
from networkx import grid_2d_graph, convert_node_labels_to_integers, to_dict_of_dicts

if len(argv) == 5:
    rows = int(argv[1])
    columns = int(argv[2])
    coupling_strength = float(argv[3])
    filename = "Graph/" + argv[4]
    nodeCount = rows * columns

    # Create 2D graph and relabel to integers instead of coordinates.
    grid_graph = convert_node_labels_to_integers(grid_2d_graph(rows, columns))

    # Convert the graph to a dict-of-dicts
    adj_mat = to_dict_of_dicts(grid_graph, edge_data=coupling_strength)

    with open(filename, "w") as file:
        file.write("# {0}\n".format(nodeCount))
        for r in range(nodeCount):
            for c in range(nodeCount):
                if r in adj_mat and c in adj_mat[r]:
                    file.write(str(adj_mat[r][c]) + " ")
                else:
                    file.write("0 ")
            file.write("\n")
else:
    print(f"\nUsage: {argv[0]} [# rows] [# columns] [coupling strength] [file name]", file=stderr)
    print(f"\tCoupling strength should be <= 1", file=stderr)
    print(f"\tThe generated file will be saved in the Graph directory", file=stderr)
    exit(1)



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