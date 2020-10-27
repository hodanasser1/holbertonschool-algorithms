#include "graphs.h"
#include "pathfinding.h"
#include "queues.h"
#include <limits.h>
#include <string.h>

/**
 * get_min - get min distance vertex from @dist array:
 * @set: set where distances have already been calculated, hence min dist is
 * not considered to vertex inside this set (in this case boolean array)
 * @dist: array with distances to vertex from src vertex
 * @graph: a graph
 * Return: a min distance vertex or NULL if failed or
 * there are no more vertices
*/
vertex_t *get_min(int *set, size_t *dist, graph_t *graph)
{
	size_t min = ULONG_MAX;
	size_t idx = ULONG_MAX, i;
	vertex_t *v = graph->vertices;

	for (i = 0; i < graph->nb_vertices; ++i)
		if (!set[i] && min > dist[i])
			min = dist[i], idx = i;
	if (idx == ULONG_MAX)
		return (NULL);
	for (i = 0; i < idx; ++i)
		v = v->next;
	return (v);
}

/**
 * calculate_distance - main logic of Dijkstra's algorithm
 * @graph: a graph
 * @start: source vertex
 * @set: set where distances have already been calculated, hence min dist is
 * not considered to vertex inside this set (in this case boolean array)
 * @dist: array with distances to vertex from src vertex
 * @parent: parent array where path is stored, used to retrieve path
 * from src to destination
*/
void calculate_distance(graph_t *graph, vertex_t const *start,
			int *set, size_t *dist, vertex_t **parent)
{
	vertex_t *vertex;
	edge_t *edge;

	if (!graph || !set || !dist)
		return;
	while ((vertex = get_min(set, dist, graph)))
	{
		set[vertex->index] = 1;
		edge = vertex->edges;
		printf("Checking %s, distance from %s is %lu\n",
		       vertex->content, start->content, dist[vertex->index]);
		for (edge = vertex->edges; edge; edge = edge->next)
		{
			if (!set[edge->dest->index] &&
			    dist[edge->dest->index] > dist[vertex->index] + edge->weight)
			{
				parent[edge->dest->index] = vertex;
				dist[edge->dest->index] = dist[vertex->index] + edge->weight;
			}
		}
	}
}

/**
 * get_path - retrieve path from src to dest from @parent array
 * @parent: parent array where path is stored, used to retrieve path
 * @target: target vertex
 * Return: a queue with full path from src to @target
*/
queue_t *get_path(vertex_t **parent, vertex_t const *target)
{
	size_t i = target->index;
	queue_t *q;

	q = queue_create();
	if (!q)
	{
		free(parent);
		return (NULL);
	}
	if (!parent[i])
	{
		free(q);
		free(parent);
		return (NULL);
	}
	if (!queue_push_front(q, strdup(target->content)))
	{
		free(parent);
		queue_delete(q);
		return (NULL);
	}
	while (parent[i])
	{
		if (!queue_push_front(q, strdup(parent[i]->content)))
		{
			free(parent);
			queue_delete(q);
			return (NULL);
		}
		i = parent[i]->index;
	}
	free(parent);
	return (q);
}

/**
 * dijkstra_graph - Dijkstra's algorithm
 * @graph: a graph
 * @start: source vertex
 * @target: target vertex
 * Return: a queue with full path from src to @target
 */
queue_t *dijkstra_graph(graph_t *graph, vertex_t const *start,
			vertex_t const *target)
{
	int *set;
	size_t *dist;
	vertex_t **parent;

	if (!graph || !start || !target)
		return (NULL);
	set = calloc(graph->nb_vertices, sizeof(*set));
	if (!set)
	{
		return (NULL);
	}
	dist = malloc(graph->nb_vertices * sizeof(*dist));
	if (!dist)
	{
		free(set);
		return (NULL);
	}
	parent = calloc(graph->nb_vertices, sizeof(**parent));
	if (!parent)
	{
		free(set);
		free(dist);
		return (NULL);
	}
	/* Set all to UINT_MAX */
	memset(dist, 0xFF, graph->nb_vertices * sizeof(*dist));
	dist[start->index] = 0;
	calculate_distance(graph, start, set, dist, parent);
	free(set), free(dist);
	return (get_path(parent, target));
}
