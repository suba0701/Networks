#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* addBinary(char* a, char* b) {
    int lenA = strlen(a);
    int lenB = strlen(b);
    
    int maxLen = (lenA > lenB ? lenA : lenB) + 1;
    
    char* result = (char*)malloc((maxLen + 1) * sizeof(char));
    result[maxLen] = '\0'; 
    
    int i = lenA - 1;
    int j = lenB - 1;
    int k = maxLen - 1;
    int carry = 0;
    
   
    while (i >= 0 || j >= 0 || carry > 0) {
        int sum = carry;
        
        if (i >= 0) {
            sum += a[i] - '0'; 
            i--;
        }
        if (j >= 0) {
            sum += b[j] - '0'; 
            j--;
        }
        
        result[k] = (sum % 2) + '0'; 
        carry = sum / 2;            
        k--;
    }
    
    
    return &result[k + 1];
}
