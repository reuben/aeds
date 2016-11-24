#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "graph.h"
#include "queue.h"
#include "pqueue.h"

const int FLAG_STATION   = 1 << 1;
const int FLAG_DONTVISIT = 1 << 2;

typedef struct {
    graph_t* graph;
    int source;
    int destination;
    int max_distance;
    int* vertex_flags;
} input_t;

input_t from_input(FILE* fin) {
    int vertices, edges, source, destination, max_distance, num_stations;
    fscanf(fin, "%d %d %d %d %d %d", &vertices, &edges, &source, &destination, &max_distance, &num_stations);
    graph_t* g = graph_new(vertices);

    int i;
    for (i = 0; i < edges; ++i) {
        int from, to;
        float weight;
        fscanf(fin, "%d %d %f\n", &from, &to, &weight);

        graph_append_edge(g, from, weight, graph_get_vertex(g, to));
        graph_append_edge(g, to, weight, graph_get_vertex(g, from));
    }

    int* vertex_flags = (int*)malloc(sizeof(int) * vertices);
    for (i = 0; i < vertices; ++i) {
        vertex_flags[i] = FLAG_DONTVISIT;
    }
    for (i = 0; i < num_stations; ++i) {
        int vertex;
        fscanf(fin, "%d\n", &vertex);
        vertex_flags[vertex] = FLAG_STATION;
    }

    input_t in;
    in.graph = g;
    in.source = source;
    in.destination = destination;
    in.max_distance = max_distance;
    in.vertex_flags = vertex_flags;

    return in;
}

void free_input(input_t* in) {
    graph_free(in->graph);
    free(in->vertex_flags);
}

void inner_bfs(graph_t* g, int max_distance, int* vertex_flags, int source) {
    // 8 is a random estimate for average out-degree
    queue_t* queue = queue_new(8);
    int* distances = (int*)calloc(graph_vertex_count(g), sizeof(int));
    bool* visited = (bool*)calloc(graph_vertex_count(g), sizeof(bool));

    distances[source] = 0;
    queue_enqueue(queue, source);

    while (queue_length(queue) > 0) {
        int from = queue_dequeue(queue);
        edge_t* adj = graph_get_edges(g, from);

        while (adj) {
            int to = adj->target->id;
            if (!visited[to]) {
                visited[to] = true;
                distances[to] = distances[from] + 1;
                queue_enqueue(queue, to);
            }
            if (distances[to] <= max_distance) {
                vertex_flags[to] &= ~FLAG_DONTVISIT;
            }

            adj = adj->next;
        }
    }

    queue_free(queue);
    free(distances);
}

// runs BFS from all stations and marks vertices that are farther than K edges 
// away. we start with all vertices marked as too far and then gradually mark
// them as reachable. after we're done we remove all edges to vertices that
// can't be reached.
void mark_distances_from_stations(input_t in) {
    int i;
    for (i = 0; i < graph_vertex_count(in.graph); ++i) {
        if (in.vertex_flags[i] & FLAG_STATION) {
            inner_bfs(in.graph, in.max_distance, in.vertex_flags, i);
        }
    }

    for (i = 0; i < graph_vertex_count(in.graph); ++i) {
        if (in.vertex_flags[i] & FLAG_DONTVISIT) {
            graph_remove_vertex(in.graph, i);
        }
    }
}

char* HEADER = "<!DOCTYPE html>\n\n\
<html><head>\n\
<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">\n\
  <title>Network | Basic usage</title>\n\
\n\
  <script type=\"text/javascript\" src=\"vis.js\"></script>\n\
  <link href=\"vis.css\" rel=\"stylesheet\" type=\"text/css\">\n\
\n\
  <style type=\"text/css\">\n\
    #mynetwork {\n\
      width: 1000px;\n\
      height: 800px;\n\
    }\n\
  </style>\n\
</head>\n\
<body>\n\
\n\
<div id=\"mynetwork\"><div tabindex=\"900\" style=\"position: relative; overflow: hidden; width: 100%; height: 100%;\" class=\"vis-network\"><canvas height=\"800\" width=\"1200\" style=\"position: relative; width: 100%; height: 100%;\"></canvas></div></div>\n\
\n\
<script type=\"text/javascript\">\n";

char* FOOTER = "  var container = document.getElementById(\"mynetwork\");\n\
  var data = {\n\
    nodes: nodes,\n\
    edges: edges\n\
  };\n\
  var options = {physics: {enabled: false}};\n\
  var network = new vis.Network(container, data, options);\n\
</script>\n\
\n\
</body></html>\n";

void print_html(input_t in, int n) {
    char name[100];
    snprintf(name, 100, "graph_%d.html", n);
    FILE* fout = fopen(name, "w");
    fprintf(fout, "%s", HEADER);
    fprintf(fout, "  var nodes = new vis.DataSet([\n");
    for (int i = 0; i < graph_vertex_count(in.graph); ++i) {
        fprintf(fout, "    {id: %d, label: %d%s},\n", i, i, (in.vertex_flags[i] & FLAG_DONTVISIT) ? ", color: 'red'" : (in.vertex_flags[i] & FLAG_STATION) ? ", color: 'green'" : "");
    }
    fprintf(fout, "]);\n\n");
    fprintf(fout, "  var edges = new vis.DataSet([\n");
    for (int i = 0; i < graph_vertex_count(in.graph); ++i) {
        edge_t* e = graph_get_edges(in.graph, i);
        while (e) {
            if (i < e->target->id) {
                fprintf(fout, "    {from: %d, to: %d, label: %.2f, font: {align: 'horizontal'}},\n", i, e->target->id, e->weight);
            }
            // fprintf(fout, "    {from: %d, to: %d},\n", e->target->id, i);
            e = e->next;
        }
    }
    fprintf(fout, "]);\n\n");
    fprintf(fout, "%s", FOOTER);
    fclose(fout);
}

int main(void) {
    FILE* fin = stdin; // fopen("input", "r");

    int N;
    fscanf(fin, "%d\n", &N);

    int i;
    for (i = 0; i < N; ++i) {
        input_t in = from_input(fin);
        mark_distances_from_stations(in);
        print_html(in, i);
    }

    return 0;
}
