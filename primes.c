#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void prime(int p[2]) __attribute__((noreturn));

void prime(int p[2]) {
    close(p[1]); // close write end of input pipe

    int prime_number;
    if (read(p[0], &prime_number, sizeof(int)) == 0) {
        close(p[0]); // No more primes to read, close read end of pipe
        exit(0);
    }
    fprintf(2, "prime %d\n", prime_number);

    int next_pipe[2];
    pipe(next_pipe);

    if(fork() == 0) {
        close(p[0]); // close read end of child
        close(next_pipe[1]); // close write end of child
        prime(next_pipe);
    } else {
        close(next_pipe[0]); // close read end of parent
        int n;
        while(read(p[0], &n, sizeof(int)) != 0) {
            if(n % prime_number != 0) { // check if the current number is a multipole of the prime number
                write(next_pipe[1], &n, sizeof(int));
            }
        }
        close(p[0]); // close input pipe
        close(next_pipe[1]); // close next_pipe write end
        wait(0); // wait for child to finish
        exit(0);
    }
}

int main() {
    int p[2];
    pipe(p);

    if(fork() == 0) {
        prime(p);
    } else {
        close(p[0]); // close read end of parent
        for(int i = 2; i <= 280; i++) {
            write(p[1], &i, sizeof(int));
        }
        close(p[1]); // close write end
        wait(0); // wait for child to finish
    }
    fprintf(2, "done yay\n");
}