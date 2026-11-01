#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int numBusesToDestination(int** routes, int routesSize, int* routesColSize, int source, int target) {
    
    if (source == target) {
        return 0;
    }

    
    int max_stop = source > target ? source : target;
    int total_elements = 0;
    for (int i = 0; i < routesSize; i++) {
        total_elements += routesColSize[i];
        for (int j = 0; j < routesColSize[i]; j++) {
            if (routes[i][j] > max_stop) {
                max_stop = routes[i][j];
            }
        }
    }

    int* stop_bus_counts = (int*)calloc(max_stop + 1, sizeof(int));
    for (int i = 0; i < routesSize; i++) {
        for (int j = 0; j < routesColSize[i]; j++) {
            stop_bus_counts[routes[i][j]]++;
        }
    }

    int* flat_bus_list = (int*)malloc(total_elements * sizeof(int));
    int* stop_offsets = (int*)malloc((max_stop + 2) * sizeof(int));
    
    stop_offsets[0] = 0;
    for (int i = 0; i <= max_stop; i++) {
        stop_offsets[i + 1] = stop_offsets[i] + stop_bus_counts[i];
    }

    int* current_offset = (int*)calloc(max_stop + 1, sizeof(int));
    for (int i = 0; i < routesSize; i++) {
        for (int j = 0; j < routesColSize[i]; j++) {
            int stop = routes[i][j];
            int idx = stop_offsets[stop] + current_offset[stop];
            flat_bus_list[idx] = i;
            current_offset[stop]++;
        }
    }

    int* queue_bus = (int*)malloc(routesSize * sizeof(int));
    int* queue_cost = (int*)malloc(routesSize * sizeof(int));
    int head = 0, tail = 0;

    bool* visited_buses = (bool*)calloc(routesSize, sizeof(bool));
    bool* visited_stops = (bool*)calloc(max_stop + 1, sizeof(bool));

    int src_start = stop_offsets[source];
    int src_end = stop_offsets[source + 1];
    for (int i = src_start; i < src_end; i++) {
        int bus_id = flat_bus_list[i];
        visited_buses[bus_id] = true;
        queue_bus[tail] = bus_id;
        queue_cost[tail] = 1;
        tail++;
    }
    visited_stops[source] = true;

    int min_buses = -1;

    while (head < tail) {
        int curr_bus = queue_bus[head];
        int curr_cost = queue_cost[head];
        head++;

        for (int j = 0; j < routesColSize[curr_bus]; j++) {
            if (routes[curr_bus][j] == target) {
                min_buses = curr_cost;
                goto cleanup;
            }
        }

        for (int j = 0; j < routesColSize[curr_bus]; j++) {
            int stop = routes[curr_bus][j];
            if (!visited_stops[stop]) {
                visited_stops[stop] = true;
                
                int start_idx = stop_offsets[stop];
                int end_idx = stop_offsets[stop + 1];
                for (int k = start_idx; k < end_idx; k++) {
                    int next_bus = flat_bus_list[k];
                    if (!visited_buses[next_bus]) {
                        visited_buses[next_bus] = true;
                        queue_bus[tail] = next_bus;
                        queue_cost[tail] = curr_cost + 1;
                        tail++;
                    }
                }
            }
        }
    }

cleanup:
    free(stop_bus_counts);
    free(flat_bus_list);
    free(stop_offsets);
    free(current_offset);
    free(queue_bus);
    free(queue_cost);
    free(visited_buses);
    free(visited_stops);

    return min_buses;
}
