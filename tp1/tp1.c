#include "graph.h"
#include "queue.h"
#include "pqueue.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef BENCHMARK
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

typedef unsigned char byte;

#define FLAG_STATION   (1 << 1)
#define FLAG_DONTVISIT (1 << 2)

typedef struct 
{
  graph_t* graph;
  byte* vertex_flags;
  unsigned source;
  unsigned destination;
  unsigned max_distance;
#ifdef BENCHMARK
  unsigned edges;
#endif
} input_t;

// reads problem parameters from fin
input_t from_input(FILE* fin);

// deallocates input_t
void free_input(input_t* in);

// runs BFS from all stations and marks vertices that are farther than K edges 
// away. we start with all vertices marked as too far and then gradually mark
// them as reachable.
void mark_distances_from_stations(input_t in);

// runs Dijkstra on the graph, prints results to stdout.
void shortest_path(input_t in);

#ifdef BENCHMARK
double get_time() {
  static double timeConvert = 0.0;
  if (timeConvert == 0.0) {
    mach_timebase_info_data_t timeBase;
    (void)mach_timebase_info(&timeBase);
    timeConvert = (double)timeBase.numer / (double)timeBase.denom / 1000000000.0;
  }
  return (double)mach_absolute_time() * timeConvert;
}
#endif

int main(void)
{
  FILE* fin = stdin; // fopen("input", "r");

  int N;
  fscanf(fin, "%d\n", &N);

  int i;
  for (i = 0; i < N; ++i) {
    input_t in = from_input(fin);

#ifdef BENCHMARK
    double start = get_time();
#endif

    mark_distances_from_stations(in);
    // don't bother with any extra work if destination isn't reachable
    if (in.vertex_flags[in.source] & FLAG_DONTVISIT) {
      printf("-1\n");
    } else {
      shortest_path(in);
    }

#ifdef BENCHMARK
    double end = get_time();
    fprintf(stderr, "%d vertices. %d edges. v*e = %d. time: %lf seconds\n", graph_vertex_count(in.graph), in.edges, graph_vertex_count(in.graph)*in.edges, end-start);
#endif

    free_input(&in);
  }

  return 0;
}

input_t from_input(FILE* fin)
{
  unsigned vertices, edges, source, destination, max_distance, num_stations;
  fscanf(fin, "%d %d %d %d %d %d", &vertices, &edges, &source, &destination, &max_distance, &num_stations);
  graph_t* g = graph_new(vertices);

  unsigned i;
  for (i = 0; i < edges; ++i) {
    unsigned from, to;
    float weight;
    fscanf(fin, "%d %d %f\n", &from, &to, &weight);

    graph_append_edge(g, from, weight, graph_get_vertex(g, to));
    graph_append_edge(g, to, weight, graph_get_vertex(g, from));
  }

  byte* vertex_flags = (byte*)malloc(vertices * sizeof(byte));
  for (i = 0; i < vertices; ++i) {
    vertex_flags[i] = FLAG_DONTVISIT;
  }
  for (i = 0; i < num_stations; ++i) {
    unsigned vertex;
    fscanf(fin, "%d\n", &vertex);
    vertex_flags[vertex] = FLAG_STATION;
  }

  input_t in;
  in.graph = g;
  in.source = source;
  in.destination = destination;
  in.max_distance = max_distance;
  in.vertex_flags = vertex_flags;

#ifdef BENCHMARK
  in.edges = edges;
#endif

  return in;
}

void free_input(input_t* in)
{
  graph_free(in->graph);
  free(in->vertex_flags);
}

static void inner_bfs(graph_t* g,
                      unsigned max_distance,
                      byte* vertex_flags,
                      unsigned source)
{
  // 8 is a random estimate for average out-degree
  queue_t* queue = queue_new(8);
  unsigned* distances = (unsigned*)calloc(graph_vertex_count(g), sizeof(unsigned));
  bool* visited = (bool*)calloc(graph_vertex_count(g), sizeof(bool));

  distances[source] = 0;
  queue_enqueue(queue, source);

  while (queue_length(queue) > 0) {
    unsigned from = queue_dequeue(queue);
    edge_t* adj = graph_get_edges(g, from);

    while (adj) {
      unsigned to = vertex_id(edge_target(adj));
      if (!visited[to]) {
        visited[to] = true;
        distances[to] = distances[from] + 1;
        queue_enqueue(queue, to);
      }
      if (distances[to] <= max_distance) {
        vertex_flags[to] &= ~FLAG_DONTVISIT;
      }

      adj = edge_next(adj);
    }

    if (distances[from]+1 > max_distance) {
      break;
    }
  }

  queue_free(queue);
  free(distances);
  free(visited);
}

void mark_distances_from_stations(input_t in)
{
  unsigned i;
  for (i = 0; i < graph_vertex_count(in.graph); ++i) {
    if (in.vertex_flags[i] & FLAG_STATION) {
      inner_bfs(in.graph, in.max_distance, in.vertex_flags, i);
    }
  }
}

static float calc_distance(float from, float to)
{
  return 1.f - ((1.f - from) * (1.f - to));
}

#define CAN_VISIT(x) (!(in.vertex_flags[x] & FLAG_DONTVISIT))

static bool float_equal(float l, float r)
{
  return fabs(l - r) < 0.00001;
}

void shortest_path(input_t in)
{
  graph_t* g = in.graph;
  unsigned vertices = graph_vertex_count(g);

  pqueue_t* q = pq_new(vertices + 1);
  unsigned* predecessor = (unsigned*)malloc(vertices * sizeof(unsigned));

  vertex_set_distance(graph_get_vertex(g, in.destination), 0.f);

  unsigned i;
  for (i = 0; i < vertices; ++i) {
    predecessor[i] = i;
    vertex_t* v = graph_get_vertex(g, i);
    if (CAN_VISIT(i)) {
      pq_push(q, v);
    }
  }

  while (pq_size(q) > 0) {
    vertex_t* next = pq_pop(q);
    edge_t* adj = graph_get_edges(g, vertex_id(next));
    while (adj) {
      vertex_t* neighbor = edge_target(adj);
      float candidate_distance = calc_distance(vertex_distance(next),
                                               edge_weight(adj));
      if (CAN_VISIT(vertex_id(neighbor)) &&
          vertex_distance(neighbor) > candidate_distance) {
        vertex_set_distance(neighbor, candidate_distance);
        pq_decrease_key(q, neighbor);
        predecessor[vertex_id(neighbor)] = vertex_id(next);
      }

      adj = edge_next(adj);
    }
  }

  // at this point, we could end up with the following scenario:
  //
  //                    c <- f
  // source <- a <- b <        > destination
  //                    d <- e
  //
  // where both shortest paths have the same distance, and we need to choose
  // the one with the smaller lexicographic order. in order to break the tie, we
  // look at all edges from each vertex in the current shortest path, starting
  // from the source, until we find a tie and break it.
  bool tie_broken = false;
  i = in.source;
  while (!tie_broken && predecessor[i] != i) {
    vertex_t* current = graph_get_vertex(g, i);

    edge_t* adj = graph_get_edges(g, i);
    while (adj) {
      vertex_t* target = edge_target(adj);

      float current_distance = vertex_distance(current);
      float new_distance = calc_distance(vertex_distance(target),
                                         edge_weight(adj));

      // if we have an alternative path with equal distance but smaller lexico-
      // graphic order, choose that path instead of the current one
      if (vertex_id(target) != predecessor[i] &&         // not current path
          vertex_id(target) < vertex_id(current) &&      // lexicographic order
          float_equal(new_distance, current_distance)) { // same distance
        predecessor[i] = vertex_id(target);
        tie_broken = true;
        break;
      }
      adj = edge_next(adj);
    }

    i = predecessor[i];
  }

  // now all just print out the final probability and the path.
  if (predecessor[in.source] == in.source) {
    printf("-1\n");
  } else {
    printf("%.2f ", vertex_distance(graph_get_vertex(g, in.source)));

    unsigned j = in.source;
    while (predecessor[j] != j) {
      printf("%d ", j);
      j = predecessor[j];
    }
    printf("%d\n", in.destination);
  }

  pq_free(q);
  free(predecessor);
}
