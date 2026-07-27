#include <stdio.h>
#include <string.h>
#include <stdbool.h>

void check_password() {
    char s[21]; // Fits strings up to length 20 + null terminator
    scanf("%s", s);
    int len = strlen(s);

    // Condition 5: Minimum length 10
    if (len < 10) {
        printf("NO\n");
        return;
    }

    bool has_lower = false;
    bool has_upper_inside = false;
    bool has_digit_inside = false;
    bool has_special_inside = false;

    for (int i = 0; i < len; i++) {
        // Lowercase check anywhere
        if (s[i] >= 'a' && s[i] <= 'z') {
            has_lower = true;
        }

        // Inside checks (excluding first and last character)
        if (i > 0 && i < len - 1) {
            if (s[i] >= 'A' && s[i] <= 'Z') {
                has_upper_inside = true;
            }
            else if (s[i] >= '0' && s[i] <= '9') {
                has_digit_inside = true;
            }
            else if (s[i] == '@' || s[i] == '#' || s[i] == '%' || s[i] == '&' || s[i] == '?') {
                has_special_inside = true;
            }
        }
    }

    if (has_lower && has_upper_inside && has_digit_inside && has_special_inside) {
        printf("YES\n");
    } else {
        printf("NO\n");
    }
}

int main() {
    int t;
    if (scanf("%d", &t) == 1) {
        while (t--) {
            check_password();
        }
    }
    return 0;
}
