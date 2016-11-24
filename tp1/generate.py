import networkx as nx
import random
import sys

def generate(vertices, chance):
    edges = int((vertices*vertices)*chance)
    print "vertices: {}, edges: {}".format(vertices, edges)
    G = nx.gnm_random_graph(vertices,edges)
    for (u,v,w) in G.edges_iter(data=True):
        w["weight"] = random.randint(1,20)/20.0
    out = open("{}_vertices_{}_edges".format(vertices, edges), "w")
    source = random.randint(0,vertices)
    dest = source
    while dest == source:
        dest = random.randint(0,vertices)
    out.write("1\n{} {} {} {} 20 100\n".format(vertices, edges, source, dest));
    for line in nx.generate_edgelist(G, data=["weight"]):
        out.write(line + "\n")
    B = set()
    while len(B) < 100:
        B.add(random.randint(0, vertices))
    for b in B:
        out.write(str(b) + "\n");
    out.close()

if __name__ == "__main__":
    generate(int(sys.argv[1]), float(sys.argv[2]))