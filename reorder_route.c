#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct Edge {
    int to;
    int is_original;
    struct Edge* next;
} Edge;

int total_changes;

void dfs(int node, Edge** adj, bool* visited) {
    visited[node] = true;
    
    Edge* curr = adj[node];
    while (curr != NULL) {
        int neighbor = curr->to;
        if (!visited[neighbor]) {
            if (curr->is_original == 1) {
                total_changes++;
            }
            dfs(neighbor, adj, visited);
        }
        curr = curr->next;
    }
}

int minReorder(int n, int** connections, int connectionsSize, int* connectionsColSize) {
    total_changes = 0;

    Edge** adj = (Edge**)calloc(n, sizeof(Edge*));
    
    for (int i = 0; i < connectionsSize; i++) {
        int u = connections[i][0];
        int v = connections[i][1];
        

        Edge* e1 = (Edge*)malloc(sizeof(Edge));
        e1->to = v;
        e1->is_original = 1;
        e1->next = adj[u];
        adj[u] = e1;
        
        Edge* e2 = (Edge*)malloc(sizeof(Edge));
        e2->to = u;
        e2->is_original = 0;
        e2->next = adj[v];
        adj[v] = e2;
    }
    
    bool* visited = (bool*)calloc(n, sizeof(bool));
    
    dfs(0, adj, visited);
    
    for (int i = 0; i < n; i++) {
        Edge* curr = adj[i];
        while (curr != NULL) {
            Edge* tmp = curr;
            curr = curr->next;
            free(tmp);
        }
    }
    free(adj);
    free(visited);
    
    return total_changes;
}
