#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define HISTORY_SIZE 5

float ans = 0;
// char history[HISTORY_SIZE][100]; 

// void displayHistory() {
//     for (int i = 0; i < HISTORY_SIZE; i++) {
//         if (strlen(history[i]) > 0) {
//             printf("%s\n", history[i]);
//         }
//     }
// }

int main() {
    char input[100];
    // int len = 0;

    while (1) {
        printf(">> ");

        fgets(input, sizeof(input), stdin);

        if (strcmp(input, "EXIT\n") == 0) {
            break;
        }

        // if (strcmp(input, "HIST\n") == 0) {
        //     displayHistory();
        //     continue;
        // }

        char op;
        char a[100], b[100];
        float num1 = 0, num2 = 0;

        int parsed = sscanf(input, "%s %c %s", a, &op, b);

        if (parsed != 3 || (op != '+' && op != '-' && op != '*' && op != '/' && op != '%')) {
            printf("SYNTAX ERROR\n");
            continue;
        }

        if (strcmp(a, "ANS") == 0) {
            num1 = ans;
        } else {
            num1 = atof(a);
        }

        if (strcmp(b, "ANS") == 0) {
            num2 = ans;
        } else {
            num2 = atof(b);
        }

        float result;

        switch (op) {
            case '+':
                result = num1 + num2;
                break;
            case '-':
                result = num1 - num2;
                break;
            case '*':
                result = num1 * num2;
                break;
            case '/':
                if (num2 == 0) {
                    printf("MATH ERROR\n");
                    continue;
                }
                result = (float)num1 / num2;
                break;
            case '%':
                if (num2 == 0) {
                    printf("MATH ERROR\n");
                    continue;
                }
                result = (div(num1, num2)).quot; 
                break;
        }

        // sprintf(history[len++], "%s %c %s = %.2f", a, op, b, result);

        ans = result;

        printf("%.2f\n", ans);
    }

    return 0;
}
