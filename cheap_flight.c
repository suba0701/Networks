#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define INF INT_MAX

int findCheapestPrice(int n, int** flights, int flightsSize, int* flightsColSize, int src, int dst, int k) {

    int* prices = (int*)malloc(n * sizeof(int));
    int* temp = (int*)malloc(n * sizeof(int));
    
    for (int i = 0; i < n; i++) {
        prices[i] = INF;
    }
    prices[src] = 0; 

    
    for (int step = 0; step <= k; step++) {
        
        memcpy(temp, prices, n * sizeof(int));

        for (int i = 0; i < flightsSize; i++) {
            int u = flights[i][0];     
            int v = flights[i][1];     
            int price = flights[i][2]; 

            
            if (prices[u] != INF && prices[u] + price < temp[v]) {
                temp[v] = prices[u] + price;
            }
        }
        

        memcpy(prices, temp, n * sizeof(int));
    }


    int final_cheapest_price = prices[dst] == INF ? -1 : prices[dst];

    free(prices);
    free(temp);

    return final_cheapest_price;
}
