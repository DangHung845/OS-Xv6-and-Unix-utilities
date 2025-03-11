#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int p1[2], p2[2];
    char buf[1];

    pipe(p1); // Pipe từ parent -> child
    pipe(p2); // Pipe từ child -> parent

    if (fork() == 0) { // Child process
        read(p1[0], buf, 1); // Nhận tín hiệu từ parent
        printf("%d: received ping\n", getpid());
        write(p2[1], "x", 1); // Gửi tín hiệu lại cho parent
        exit(0);
    } else { // Parent process
        write(p1[1], "x", 1); // Gửi tín hiệu cho child
        read(p2[0], buf, 1); // Nhận tín hiệu từ child
        printf("%d: received pong\n", getpid());
        wait(0);
    }

    exit(0);
}
