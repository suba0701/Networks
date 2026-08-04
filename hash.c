#include <stdio.h>
#include <stdlib.h>

// Simple hash set implementation to store overall unique values
#define HASH_SIZE 1000003

typedef struct Node {
    int val;
    struct Node* next;
} Node;

typedef struct {
    Node* bucket[HASH_SIZE];
} HashSet;

void initSet(HashSet* set) {
    for (int i = 0; i < HASH_SIZE; i++) {
        set->bucket[i] = NULL;
    }
}

int insertSet(HashSet* set, int val) {
    int index = abs(val) % HASH_SIZE;
    Node* curr = set->bucket[index];
    while (curr != NULL) {
        if (curr->val == val) return 0; // Already exists
        curr = curr->next;
    }
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->val = val;
    newNode->next = set->bucket[index];
    set->bucket[index] = newNode;
    return 1; // Successfully inserted new item
}

void freeSet(HashSet* set) {
    for (int i = 0; i < HASH_SIZE; i++) {
        Node* curr = set->bucket[i];
        while (curr != NULL) {
            Node* temp = curr;
            curr = curr->next;
            free(temp);
        }
    }
}

int subarrayBitwiseORs(int* arr, int arrSize) {
    HashSet globalSet;
    initSet(&globalSet);
    
    // Arrays to maintain unique OR values ending at the previous and current index
    // Size 32 is sufficient because a 32-bit int can change bits at most 32 times
    int prev_ors[32];
    int prev_size = 0;
    
    int result_count = 0;

    for (int i = 0; i < arrSize; i++) {
        int curr_ors[32];
        int curr_size = 0;
        
        // 1. Every element by itself forms a valid single-element subarray
        curr_ors[curr_size++] = arr[i];
        if (insertSet(&globalSet, arr[i])) {
            result_count++;
        }
        
        // 2. Compute ORs with all unique results ending at the previous element
        for (int j = 0; j < prev_size; j++) {
            int new_or = prev_ors[j] | arr[i];
            
            // Avoid adding duplicates to the local array to keep it small
            if (curr_ors[curr_size - 1] != new_or) {
                curr_ors[curr_size++] = new_or;
                if (insertSet(&globalSet, new_or)) {
                    result_count++;
                }
            }
        }
        
        // 3. Move current results to previous tracking buffers for the next step
        prev_size = curr_size;
        for (int j = 0; j < curr_size; j++) {
            prev_ors[j] = curr_ors[j];
        }
    }
    
    freeSet(&globalSet);
    return result_count;
}
