#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* addBinary(char* a, char* b) {
    int lenA = strlen(a);
    int lenB = strlen(b);
    
    // The max length of the result can be max(lenA, lenB) + 1 (for carry)
    int maxLen = (lenA > lenB ? lenA : lenB) + 1;
    
    // Allocate memory for the result string + 1 for the null terminator
    char* result = (char*)malloc((maxLen + 1) * sizeof(char));
    result[maxLen] = '\0'; // Set null terminator at the end
    
    int i = lenA - 1;
    int j = lenB - 1;
    int k = maxLen - 1;
    int carry = 0;
    
    // Loop from right to left through both strings
    while (i >= 0 || j >= 0 || carry > 0) {
        int sum = carry;
        
        if (i >= 0) {
            sum += a[i] - '0'; // Convert char to int
            i--;
        }
        if (j >= 0) {
            sum += b[j] - '0'; // Convert char to int
            j--;
        }
        
        result[k] = (sum % 2) + '0'; // Get binary bit character
        carry = sum / 2;             // Compute new carry
        k--;
    }
    
    // Shift the pointer if the first index was skipped (no extra carry)
    return &result[k + 1];
}
