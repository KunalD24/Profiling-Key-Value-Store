## Performance Analysis

To evaluate the performance of our key-value store, we used `perf stat` to collect system-level metrics for both the server and multiple clients. We monitored CPU and I/O statistics, as well as specific system call activity using events like `syscalls:sys_enter_read`, `syscalls:sys_enter_write`, and `syscalls:sys_enter_accept`. This approach helped us understand how the server manages multiple client connections and the frequency of key system calls during read, write, and accept operations.

## Commands Used

### 1. General performance statistics for the server
```bash
sudo perf stat ./server
```
### 2. General performance statistics for the client
```bash
sudo perf stat ./client1
sudo perf stat ./client2
```
### 3. Syscall-specific statistics for the server
```bash
sudo perf stat -e syscalls:sys_enter_read,syscalls:sys_enter_write,syscalls:sys_enter_accept ./server
```

### 4. Syscall-specific statistics for the client
```bash
sudo perf stat -e syscalls:sys_enter_read,syscalls:sys_enter_write,syscalls:sys_enter_accept ./client1
sudo perf stat -e syscalls:sys_enter_read,syscalls:sys_enter_write,syscalls:sys_enter_accept ./client2
```
