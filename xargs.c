#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

#define MAX_LINE_LENGTH 512

int main(int argc, char* argv[]) {
    if (argc < 2) {
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    char *args[MAXARG];

    for (int i = 1; i < argc; i++) {
        args[i - 1] = argv[i];
    }

    while (1) {
        int n = 0;
        char c;

        while (read(0, &c, 1) > 0) {
            if (c == '\n') {
                break;
            }

            if (n < MAX_LINE_LENGTH - 1) {
                line[n++] = c;
            }
        }

        if (n == 0) {
            break;
        }

        line[n] = '\0';
        args[argc - 1] = line;
        args[argc] = 0;

        if (fork() == 0) {
            exec(args[0], args);
            fprintf(2, "exec %s failed\n", args[0]);
            exit(1);
        }
        else {
            wait(0);
        }
    }

    exit(0);
}