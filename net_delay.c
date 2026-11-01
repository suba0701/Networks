#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define INF INT_MAX

typedef struct Edge {
    int to;
    int weight;
    struct Edge* next;
} Edge;

typedef struct HeapNode {
    int vertex;
    int dist;
} HeapNode;


typedef struct MinHeap {
    HeapNode* data;
    int size;
    int capacity;
} MinHeap;

MinHeap* createMinHeap(int capacity) {
    MinHeap* minHeap = (MinHeap*)malloc(sizeof(MinHeap));
    minHeap->data = (HeapNode*)malloc(capacity * sizeof(HeapNode));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    return minHeap;
}

void swapHeapNodes(HeapNode* a, HeapNode* b) {
    HeapNode temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(MinHeap* minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->data[left].dist < minHeap->data[smallest].dist)
        smallest = left;

    if (right < minHeap->size && minHeap->data[right].dist < minHeap->data[smallest].dist)
        smallest = right;

    if (smallest != idx) {
        swapHeapNodes(&minHeap->data[smallest], &minHeap->data[idx]);
        minHeapify(minHeap, smallest);
    }
}

void push(MinHeap* minHeap, int vertex, int dist) {
    if (minHeap->size == minHeap->capacity) return;

    int idx = minHeap->size;
    minHeap->data[idx].vertex = vertex;
    minHeap->data[idx].dist = dist;
    minHeap->size++;

    while (idx != 0 && minHeap->data[idx].dist < minHeap->data[(idx - 1) / 2].dist) {
        swapHeapNodes(&minHeap->data[idx], &minHeap->data[(idx - 1) / 2]);
        idx = (idx - 1) / 2;
    }
}

HeapNode pop(MinHeap* minHeap) {
    HeapNode root = minHeap->data[0];
    minHeap->data[0] = minHeap->data[minHeap->size - 1];
    minHeap->size--;
    minHeapify(minHeap, 0);
    return root;
}


int networkDelayTime(int** times, int timesSize, int* timesColSize, int n, int k) {

    Edge** adj = (Edge**)calloc(n + 1, sizeof(Edge*));
    for (int i = 0; i < timesSize; i++) {
        int u = times[i][0];
        int v = times[i][1];
        int w = times[i][2];
        
        Edge* edge = (Edge*)malloc(sizeof(Edge));
        edge->to = v;
        edge->weight = w;
        edge->next = adj[u];
        adj[u] = edge;
    }

    int* dist = (int*)malloc((n + 1) * sizeof(int));
    for (int i = 1; i <= n; i++) {
        dist[i] = INF;
    }

    MinHeap* minHeap = createMinHeap(timesSize + 1);

    dist[k] = 0;
    push(minHeap, k, 0);

    while (minHeap->size > 0) {
        HeapNode curr = pop(minHeap);
        int u = curr.vertex;
        int d = curr.dist;

        if (d > dist[u]) continue;

        Edge* edge = adj[u];
        while (edge != NULL) {
            int v = edge->to;
            int weight = edge->weight;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                push(minHeap, v, dist[v]);
            }
            edge = edge->next;
        }
    }

    int max_time = 0;
    for (int i = 1; i <= n; i++) {
        if (dist[i] == INF) {
            max_time = -1; 
            break;
        }
        if (dist[i] > max_time) {
            max_time = dist[i];
        }
    }

    for (int i = 1; i <= n; i++) {
        Edge* edge = adj[i];
        while (edge != NULL) {
            Edge* tmp = edge;
            edge = edge->next;
            free(tmp);
        }
    }
    free(adj);
    free(dist);
    free(minHeap->data);
    free(minHeap);

    return max_time;
}
