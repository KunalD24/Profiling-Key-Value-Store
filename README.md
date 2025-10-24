## Performance Analysis

To evaluate the performance of our key-value store, we used `perf stat` to collect system-level metrics for both the server and multiple clients. We monitored CPU and I/O statistics, as well as specific system call activity using events like `syscalls:sys_enter_read`, `syscalls:sys_enter_write`, and `syscalls:sys_enter_accept`. This approach helped us understand how the server manages multiple client connections and the frequency of key system calls during read, write, and accept operations.

## Commands Used

### 1. Syscall-specific statistics for the server
```bash
sudo perf stat -e syscalls:sys_enter_read,syscalls:sys_enter_write,syscalls:sys_enter_accept ./server
```
- ## Output (server)
```bash
Performance counter stats for './server':

                45      syscalls:sys_enter_read
                 5      syscalls:sys_enter_write
                 5      syscalls:sys_enter_accept

      59.926353160 seconds time elapsed

       0.006932000 seconds user
       0.001713000 seconds sys
```

### Server-Side Interpretation
| Observation | Meaning | Inference |
| --- | --- | --- |
| read = 45 | The server performed 45 read operations from connected clients. | Low load; likely few requests received. |
| write = 5 | Only 5 responses sent to clients. | Indicates limited interaction or mostly idle server. |
| accept = 5| Five client connections accepted. | Few clients connected during profiling. |
| Overall | Low syscall counts and high elapsed time (59s) | 	Server was mostly waiting for client requests or idle. |


### 2. Syscall-specific statistics for the client
### For Client1 :
```bash
sudo perf stat -e syscalls:sys_enter_read,syscalls:sys_enter_write,syscalls:sys_enter_accept ./client1
```
- ## Output (client1)
```bash
Performance counter stats for './client1':

                 6      syscalls:sys_enter_read
                 7      syscalls:sys_enter_write
                 0      syscalls:sys_enter_accept

      39.675607740 seconds time elapsed

       0.002989000 seconds user
       0.000000000 seconds sys
```

### For Client2 :
```bash
sudo perf stat -e syscalls:sys_enter_read,syscalls:sys_enter_write,syscalls:sys_enter_accept ./client2
```
- ## Output (client2)
```bash
Performance counter stats for './client2':

                 6      syscalls:sys_enter_read
                 7      syscalls:sys_enter_write
                 0      syscalls:sys_enter_accept

      19.822051080 seconds time elapsed

       0.002459000 seconds user
       0.000000000 seconds sys
```


