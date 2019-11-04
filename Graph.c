#include "Graph.h"
#include "List.h"
#include<stdio.h>
#include<stdlib.h>
#include<assert.h>

typedef struct GraphObj {
	int order;
	int size;
	int label;
	List* adj;
	int* color;
	int* parent;
	int* distance;
} GraphObj;

// Graph
typedef GraphObj* Graph;

Graph newGraph(int n) {
	Graph M = malloc(sizeof(GraphObj));
	assert(M != NULL);
	M->order = n;
	M->size = 0;
	M->label = NIL;
	M->adj = malloc((n + 1) * sizeof(List));
	M->color = malloc((n + 1) * sizeof(int));
	M->parent = malloc((n + 1) * sizeof(int));
	M->distance = malloc((n + 1) * sizeof(int));
	for (int i = 1; i <= M->order; i++) {
		M->adj[i] = newList();
		M->color[i] = -1;
		M->parent[i] = NIL;
		M->distance[i] = INF;
	}
	return M;
}

void freeGraph(Graph* pG) {
	if (pG != NULL && *pG != NULL) {
		if (getOrder(*pG) != 0) makeNull(*pG);
		for (int i = 1; i <= (*pG)->order; i++) {
			freeList(&((*pG)->adj[i]));
		}
		free((*pG)->adj);
		free((*pG)->parent);
		free((*pG)->color);
		free((*pG)->distance);
		free(*pG);
		*pG = NULL;
	}
}

int getOrder(Graph G) {
	return G->order;
}

int getSize(Graph G) {
	return G->size;
}

int getSource(Graph G) {
	return G->label;
}

int getParent(Graph G, int u) {
	if (u >= 1 && u <= getOrder(G)) {
		return G->parent[u];
	}
	return NIL;
}

int getDist(Graph G, int u) {
	if (u >= 1 && u <= getOrder(G)) {
		return G->distance[u];
	}
	return INF;
}



void getPath(List L, Graph G, int u) {
	if (u >= 1 && u <= getOrder(G) && G->label != NIL) {
		if (u != G->label) {
			if (G->distance[u] != INF) {
				int x = u;
				prepend(L, u);
				while (G->parent[x] != G->label) {
					prepend(L, G->parent[x]);
					x = G->parent[x];
				}
				prepend(L, G->label);
			}
			else {
				append(L, NIL);
			}
		}
		else {
			append(L, u);
		}
	}
}

void makeNull(Graph G) {
	for (int i = 1; i <= G->order; i++) {
		clear(G->adj[i]);
		G->color[i] = -1;
		G->parent[i] = NIL;
		G->distance[i] = INF;
	}
	G->size = 0;
	G->label = NIL;
}

void addEdge(Graph G, int u, int v) {
	if (u >= 1 && u <= G->order && v >= 1 && v <= G->order) {
		int find = -1;
		if (length(G->adj[u]) != 0 && length(G->adj[v]) != 0) {
			moveFront(G->adj[u]);
			for (int i = 0; i < length(G->adj[u]); i++) {
				if (get(G->adj[u]) > v) {
					insertBefore(G->adj[u], v);
					find = 1;
					break;
				}
				moveNext(G->adj[u]);
			}
			if (find != 1) {
				append(G->adj[u], v);
			}
			find = -1;
			moveFront(G->adj[v]);
			for (int j = 0; j < length(G->adj[v]); j++) {
				if (get(G->adj[v]) > u) {
					insertBefore(G->adj[v], u);
					find = 1;
					break;
				}
				moveNext(G->adj[v]);
			}
			if (find != 1) {
				append(G->adj[v], u);
			}
		}
		else if (length(G->adj[u]) == 0 && length(G->adj[v]) == 0) {
			append(G->adj[u], v);
			append(G->adj[v], u);
		}
		else if (length(G->adj[u]) == 0) {
			append(G->adj[u], v);
			moveFront(G->adj[v]);
			for (int j = 0; j < length(G->adj[v]); j++) {
				if (get(G->adj[v]) > u) {
					insertBefore(G->adj[v], u);
					find = 1;
					break;
				}
				moveNext(G->adj[v]);
			}
			if (find != 1) {
				append(G->adj[v], u);
			}
		}
		else {
			append(G->adj[v], u);
			moveFront(G->adj[u]);
			for (int i = 0; i < length(G->adj[u]); i++) {
				if (get(G->adj[u]) > v) {
					insertBefore(G->adj[u], v);
					find = 1;
					break;
				}
				moveNext(G->adj[u]);
			}
			if (find != 1) {
				append(G->adj[u], v);
			}
		}
		G->size++;
	}
}

void addArc(Graph G, int u, int v) {
	if (u >= 1 && u <= G->order && v >= 1 && v <= G->order) {
		if (length(G->adj[u]) != 0) {
			int find = -1;
			moveFront(G->adj[u]);
			for (int i = 1; i <= length(G->adj[u]); i++) {
				if (get(G->adj[u]) > v) {
					insertBefore(G->adj[u], v);
					find = 1;
					break;
				}
				moveNext(G->adj[u]);
			}
			if (find == -1) {
				append(G->adj[u], v);
			}
		}
		else {
			append(G->adj[u], v);
		}
		G->size++;
	}
}

void BFS(Graph G, int s) {
	for (int s = 1; s <= G->order; s++) {
		G->color[s] = -1;
		G->parent[s] = NIL;
		G->distance[s] = INF;
	}
	G->label = s;
	G->distance[s] = 0;
	G->color[s] = 0;
	List Q = newList();
	append(Q, s);
	moveFront(Q);
	int x;
	int y;
	while (index(Q) != -1) {
		x = get(Q);
		moveFront(G->adj[x]);
		for (int i = 1; i <= length(G->adj[x]); i++) {
			y = get(G->adj[x]);
			if (G->color[y] == -1) {
				G->color[y] = 0;
				G->distance[y] = G->distance[x] + 1;
				G->parent[y] = x;
				append(Q, y);
			}
			moveNext(G->adj[x]);
		}
		G->color[x] = 1;
		moveNext(Q);
	}
	freeList(&Q);
}

void printGraph(FILE* out, Graph G) {
	for (int i = 1; i <= G->order; i++) {
		if (G->adj[i] != NULL && length(G->adj[i]) != 0) {
			fprintf(out, "%d: ", i);
			moveFront(G->adj[i]);
			for (int j = 1; j <= length(G->adj[i]); j++) {
				fprintf(out, "%d ", get(G->adj[i]));
				moveNext(G->adj[i]);
			}
		}
		else {
			fprintf(out, "%d: ", i);
		}
		fprintf(out, "\n");
	}
}