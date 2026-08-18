#include <stdio.h>
#include <string.h>

/* Function to print 8-bit binary array */
void printBinary(int bits[]) {
    int i; /* Declared at the top for old compilers */
    for (i = 7; i >= 0; i--) {
        printf("%d", bits[i]);
    }
    printf(" ");
}

int main() {
    char text[100];
    int total_chars;
    int binary_data[100][8];
    int num_changes;
    int error_detected = 0;

    /* Declare all loop and temporary variables at the top */
    int i, j, k;
    int char_index, bit_pos, new_val;

    /* 1. Get text input from user */
    printf("Enter the text: ");
    fgets(text, sizeof(text), stdin);

    /* Remove newline character if present */
    text[strcspn(text, "\n")] = '\0';
    total_chars = strlen(text);

    printf("\n--- Binary Conversion & Parity Generation ---\n");

    for (i = 0; i < total_chars; i++) {
        int ascii = (int)text[i];
        int count_ones = 0;

        printf("\nCharacter: '%c' (ASCII: %d)\n", text[i], ascii);

        /* 2. Convert to 7-bit binary and display */
        printf("7-bit Binary: ");
        for (j = 0; j < 7; j++) {
            binary_data[i][j] = (ascii >> j) & 1;
            if (binary_data[i][j] == 1) {
                count_ones++;
            }
        }
        /* Display 7 bits (from MSB to LSB) */
        for (j = 6; j >= 0; j--) {
            printf("%d", binary_data[i][j]);
        }
        printf("\n");

        /* 3. Add Odd Parity Bit at 8th position (Index 7) */
        if (count_ones % 2 != 0) {
            binary_data[i][7] = 0; /* Already odd, so add 0 */
        } else {
            binary_data[i][7] = 1; /* Even, so add 1 to make it odd */
        }

        /* 4. Display the final 8-bit stream */
        printf("8-bit Stream (with Odd Parity): ");
        printBinary(binary_data[i]);
        printf("\n");
    }

    /* 5. Ask user for Error Injection */
    printf("\n--- Error Injection ---\n");
    printf("Enter how many bit positions you want to change: ");
    scanf("%d", &num_changes);

    for (k = 0; k < num_changes; k++) {
        printf("\nFor change %d:\n", k + 1);
        printf("Enter the block character index (0 to %d): ", total_chars - 1);
        scanf("%d", &char_index);
        printf("Enter bit position to change in the selected block: ");
        scanf("%d", &bit_pos);
        printf("Enter new bit value (0 or 1): ");
        scanf("%d", &new_val);

        if (char_index >= 0 && char_index < total_chars && bit_pos >= 0 && bit_pos <= 7) {
            binary_data[char_index][bit_pos] = new_val;
            printf("Bit changed successfully!\n");
        } else {
            printf("Invalid index or bit position!\n");
        }
    }

    /* Display data after changes */
    printf("\nData after modifications:\n");
    for (i = 0; i < total_chars; i++) {
        printf("Char %d: ", i);
        printBinary(binary_data[i]);
        printf("\n");
    }

    /* 6. Check for Odd Parity and validate errors */
    printf("\n--- Receiver / Parity Checking ---\n");

    for (i = 0; i < total_chars; i++) {
        int total_ones = 0;
        for (j = 0; j < 8; j++) {
            if (binary_data[i][j] == 1) {
                total_ones++;
            }
        }

        /* If total 1s is even, then it violates Odd Parity rule (Error!) */
        if (total_ones % 2 == 0) {
            printf("Error Occurred at character index %d ('%c')! Total 1s is %d (Even number of 1s).\n", i, text[i], total_ones);
            error_detected = 1;
        }
    }

    if (!error_detected) {
        printf("Odd parity done successful! No errors found.\n");
    }

    return 0;
}
