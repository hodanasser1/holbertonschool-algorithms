#include "pathfinding.h"

static int *dists;
static int *fists;
static vertex_t **from;
static vertex_t **verts;

#define STRDUP(x) ((str = strdup(x)) ? str : (exit(1), NULL))
#define H(a) ((int)sqrt(pow((a->x - target->x), 2) + \
	pow((a->y - target->y), 2)))

#define ALLOCATE_ALL \
	do { \
		dists = calloc(graph->nb_vertices, sizeof(*dists)); \
		fists = calloc(graph->nb_vertices, sizeof(*fists)); \
		from = calloc(graph->nb_vertices, sizeof(*from)); \
		verts = calloc(graph->nb_vertices, sizeof(*verts)); \
	} while (0)

/**
 * a_star_graph - uses A* algo to find optimal path
 * @graph: pointer to graph struct
 * @start: pointer to starting vertex
 * @target: pointer to target vertex
 * Return: optimal path queue or NULL
 */
queue_t *a_star_graph(graph_t *graph, vertex_t const *start,
	vertex_t const *target)
{
	ssize_t i, d, j = -1;
	vertex_t *v;
	edge_t *e;
	queue_t *path = queue_create();
	char *str;

	if (!graph || !start || !target || !path)
		return (NULL);
	ALLOCATE_ALL;
	if (!dists || !fists || !from || !verts || !path)
		return (NULL);
	for (v = graph->vertices; v; v = v->next)
		verts[v->index] = v, dists[v->index] = fists[v->index] = INT_MAX;
	dists[start->index] = 0, from[start->index] = NULL;
	fists[start->index] = H(start);
	while (j != (ssize_t)target->index)
	{
		for (d = INT_MAX, j = -1, i = 0; i < (ssize_t)graph->nb_vertices; i++)
			if (fists[i] >= 0 && fists[i] < d)
				d = fists[i], j = i;
		if (j == -1)
			break;
		printf("Checking %s, distance to %s is %d\n",
			verts[j]->content, target->content, H(verts[j]));
		for (e = verts[j]->edges; e; e = e->next)
			if (dists[e->dest->index] >= 0 &&
				dists[j] + e->weight < dists[e->dest->index])
				dists[e->dest->index] = dists[j] + e->weight,
				from[e->dest->index] = verts[j],
				fists[e->dest->index] = dists[e->dest->index] + H(e->dest);
		fists[j] = -1;
	}
	if (j != -1)
		for (queue_push_front(path, STRDUP(verts[j]->content));
			j != (ssize_t)start->index; j = from[j]->index)
			queue_push_front(path, STRDUP(from[j]->content));
	else
		path = (free(path), NULL);
	free(dists), free(fists), free(from), free(verts);
	return (path);
}
