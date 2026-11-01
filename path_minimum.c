#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>

typedef struct HeapNode {
    int row;
    int col;
    int effort;
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

    if (left < minHeap->size && minHeap->data[left].effort < minHeap->data[smallest].effort)
        smallest = left;

    if (right < minHeap->size && minHeap->data[right].effort < minHeap->data[smallest].effort)
        smallest = right;

    if (smallest != idx) {
        swapHeapNodes(&minHeap->data[smallest], &minHeap->data[idx]);
        minHeapify(minHeap, smallest);
    }
}

void push(MinHeap* minHeap, int row, int col, int effort) {
    if (minHeap->size == minHeap->capacity) return;

    int idx = minHeap->size;
    minHeap->data[idx].row = row;
    minHeap->data[idx].col = col;
    minHeap->data[idx].effort = effort;
    minHeap->size++;

    while (idx != 0 && minHeap->data[idx].effort < minHeap->data[(idx - 1) / 2].effort) {
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


int minimumEffortPath(int** heights, int heightsSize, int* heightsColSize) {
    int rows = heightsSize;
    int cols = heightsColSize[0];


    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    int** efforts = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        efforts[i] = (int*)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++) {
            efforts[i][j] = INT_MAX;
        }
    }


    MinHeap* minHeap = createMinHeap(rows * cols * 4);

    efforts[0][0] = 0;
    push(minHeap, 0, 0, 0);

    int final_effort = 0;

    while (minHeap->size > 0) {
        HeapNode curr = pop(minHeap);
        int r = curr.row;
        int c = curr.col;
        int curr_effort = curr.effort;


        if (curr_effort > efforts[r][c]) continue;

        if (r == rows - 1 && c == cols - 1) {
            final_effort = curr_effort;
            break;
        }

        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) {

                int step_cost = abs(heights[r][c] - heights[nr][nc]);
                int max_effort_to_neighbor = curr_effort > step_cost ? curr_effort : step_cost;

                if (max_effort_to_neighbor < efforts[nr][nc]) {
                    efforts[nr][nc] = max_effort_to_neighbor;
                    push(minHeap, nr, nc, max_effort_to_neighbor);
                }
            }
        }
    }

    for (int i = 0; i < rows; i++) {
        free(efforts[i]);
    }
    free(efforts);
    free(minHeap->data);
    free(minHeap);

    return final_effort;
}
