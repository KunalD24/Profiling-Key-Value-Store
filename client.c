//Implement a mini key-value store server using Unix domain sockets and a client that can set/get values.
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/keyvaluestore.sock"
#define BUF_SIZE 1024

int main(void) 
{
    int fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (fd == -1) 
    { 
        perror("socket"); exit(1); 
    }

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path)-1);

    if (connect(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) 
    {
        perror("connect"); exit(1);
    }

    char line[BUF_SIZE];
    char buf[BUF_SIZE];

    while (1) 
    {
        printf("kv> ");
        if (!fgets(line, sizeof(line), stdin)) break;

        write(fd, line, strlen(line));

        ssize_t n = read(fd, buf, sizeof(buf)-1);
        if (n <= 0) break;
        buf[n] = '\0';
        printf("%s", buf);
    }

    close(fd);
    return 0;
}

// TM/Profiling/key-value-store# perf stat ./client
// kv> SET name Kunal
// OK  
// kv> GET name 
// Kunal
// kv> ^C./client: Interrupt

//  Performance counter stats for './client':

//               2.67 msec task-clock
//        #    0.000 CPUs utilized
//                  9      context-switches
//        #    3.365 K/sec
//                  1      cpu-migrations
//        #  373.846 /sec
//                 55      page-faults
//        #   20.562 K/sec
//            3602260      cycles
//        #    1.347 GHz
//            1274745      instructions
//        #    0.35  insn per cycle
//             250127      branches
//        #   93.509 M/sec
//              13368      branch-misses
//        #    5.34% of all branches

//       11.259439636 seconds time elapsed

//        0.000000000 seconds user
//        0.019205000 seconds sys