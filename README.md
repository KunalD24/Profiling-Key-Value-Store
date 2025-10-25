## Performance Analysis

To evaluate the performance of our key-value store, we used `perf stat` to collect system-level metrics for both the server and multiple clients. We monitored CPU and I/O statistics, as well as specific system call activity using events like `syscalls:sys_enter_read`, `syscalls:sys_enter_write`, and `syscalls:sys_enter_accept`. This approach helped us understand how the server manages multiple client connections and the frequency of key system calls during read, write, and accept operations.

## Commands Used

## 1. Syscall-specific statistics for the server
```bash
sudo perf stat -e syscalls:sys_enter_read,syscalls:sys_enter_write,syscalls:sys_enter_accept,syscalls:sys_enter_close ./server
```
- ## Output (server)
```bash
Performance counter stats for './server':

                50      syscalls:sys_enter_read
                 5      syscalls:sys_enter_write
                 5      syscalls:sys_enter_accept
                11      syscalls:sys_enter_close

      68.067048897 seconds time elapsed

       0.000000000 seconds user
       0.017860000 seconds sys
```

## Server-Side Interpretation
| Observation | Meaning | Inference |
| --- | --- | --- |
| read = 50 | The server performed 50 read operations from connected clients. | Low load; likely few requests received. |
| write = 5 | Only 5 responses sent to clients. | Indicates limited interaction or mostly idle server. |
| accept = 5| Five client connections accepted. | Few clients connected during profiling. |
| close = 11| One connection closed. | Graceful shutdown or one completed session. |
| Overall | Low syscall counts and high elapsed time (68s) | 	Server was mostly waiting for client requests or idle. |


## 2. Syscall-specific statistics for the client
## For Client1 :
```bash
sudo perf stat -e syscalls:sys_enter_read,syscalls:sys_enter_write,syscalls:sys_enter_connect,syscalls:sys_enter_close ./client1
```
- ## Output (client1)
```bash
Performance counter stats for './client1':

                 6      syscalls:sys_enter_read
                 7      syscalls:sys_enter_write
                 1      syscalls:sys_enter_connect
                 2      syscalls:sys_enter_close

      47.406747103 seconds time elapsed

       0.005024000 seconds user
       0.005024000 seconds sys
```
## Client-Side Interpretation :

| Observation | Meaning | Inference |
| --- | --- | --- |
| read = 6 | Client received responses fewer times than it sent data. | May be waiting for replies or batching requests. |
| write = 7 | 	Client issued many write calls (sending commands/requests). | Indicates client actively communicating with the server. |
| connect = 1 | One connection attempts made. | Could be reconnects or multiple sessions. |
| close = 2 | 	Two connection closed. | 	Session ended gracefully. |
| Overall | More writes than reads | 	Client is request-heavy; may be generating load. |

## For Client2 :
```bash
sudo perf stat -e syscalls:sys_enter_read,syscalls:sys_enter_write,syscalls:sys_enter_connect,syscalls:sys_enter_close ./client2
```
- ## Output (client2)
```bash
Performance counter stats for './client2':

                 6      syscalls:sys_enter_read
                 7      syscalls:sys_enter_write
                 1      syscalls:sys_enter_connect
                 2      syscalls:sys_enter_close

      45.701622591 seconds time elapsed

       0.004097000 seconds user
       0.000000000 seconds sys
```

## Client-Side Interpretation :

| Observation | Meaning | Inference |
| --- | --- | --- |
| read = 6 | Client received responses fewer times than it sent data. | May be waiting for replies or batching requests. |
| write = 7 | 	Client issued many write calls (sending commands/requests). | Indicates client actively communicating with the server. |
| connect = 1 | One connection attempts made. | Could be reconnects or multiple sessions. |
| close = 2 | 	Two connection closed. | 	Session ended gracefully. |
| Overall | More writes than reads | 	Client is request-heavy; may be generating load. |

## Syscall Bottleneck Interpretation Summary
| Syscall | When Count is High | Likely System State | Possible Fix / Optimization |
| --- | --- | --- | --- |
| read() | Frequent small reads | I/O-bound, waiting for data | Use larger buffers, batch reads, or non-blocking I/O |
| write() | Many small writes | Kernel overhead, syscall transitions | Buffer or batch writes, use async I/O |
| accept()  | Many short-lived connections | Connection churn, context switches | Use connection pooling, reuse sockets |
| close() | 	Frequent open/close cycles | Descriptor churn, possible exhaustion | Keep connections alive longer |
| connect() | Many connection attempts | Handshake overhead | 	Use persistent connections or keep-alive |
| send() / recv() | High message exchange frequency | Heavy socket I/O | Batch messages, reduce fragmentation |
| fsync() | Frequent disk flushes | 	I/O wait for durability | Group commits, delayed fsync, async WAL flush |


