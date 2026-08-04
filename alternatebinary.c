#include <stdbool.h>

bool hasAlternatingBits(int n) {
    // Get the value of the very last bit (0 or 1)
    int prev_bit = n % 2;
    n = n / 2; // Move to the next bit
    
    // Loop through the rest of the bits
    while (n > 0) {
        int current_bit = n % 2;
        
        // If two adjacent bits are the same, return false
        if (current_bit == prev_bit) {
            return false;
        }
        
        // Update the previous bit and move to the next one
        prev_bit = current_bit;
        n = n / 2;
    }
    
    // If we checked all bits and found no duplicates, return true
    return true;
}
