

# Threads-Linux-Philosophers

## Introduction
This project is a multi-threaded simulation of the classic **Dining Philosophers Problem**. The goal is to prevent **deadlocks** and **starvation** while ensuring philosophers can **eat, think, and sleep** concurrently in a well-coordinated manner.

## The Problem
The **Dining Philosophers Problem** consists of `N` philosophers sitting at a circular table, each requiring **two forks** (shared with neighbors) to eat. The main challenges include:

- **Avoiding deadlocks** (e.g., all philosophers picking up one fork and waiting forever).
- **Preventing starvation** (ensuring every philosopher gets a chance to eat).
- **Ensuring synchronization** (coordinating shared resources correctly).

## Solution Approach
The solution uses **mutex locks** to properly manage forks and ensure fair access for all philosophers.

### Key Features

| Feature                  | Description |
|--------------------------|-------------|
| **Thread Management**    | Each philosopher runs in a separate `pthread` thread. |
| **Mutex Synchronization** | Forks are protected with **mutexes** to avoid race conditions. |
| **Logging System**       | Uses a **buffered logging** mechanism for efficient output handling. |
| **Precise Timing**       | Implements `gettimeofday()` and `usleep()` for accurate time management. |
| **State Machine Logic**  | Philosophers follow an optimized **eating/thinking/sleeping** sequence. |
| **Monitor Thread**       | A separate thread monitors if any philosopher dies from starvation. |
| **Optimized Performance** | Several techniques are used to reduce computational load (see below). |

## Performance Optimizations
Several techniques are used to reduce the **computational load** and improve performance:

### 1. **Buffered Logging (Batch Processing)**
- Instead of calling `printf()` multiple times (which is slow), a **log buffer** stores messages.
- The **log_flusher thread** periodically flushes stored messages in bulk, reducing I/O overhead.

### 2. **Bitwise Operations for Fast Computation**
The program uses **bitwise AND (&)** and **bitwise shift (<<)** operations to optimize calculations:
- Checking if a philosopher’s ID is **odd or even**: `p->id & 1` instead of `p->id % 2`.
- Multiplying by 2 using `p->eat_time << 1` instead of `p->eat_time * 2`.
- These **reduce CPU cycles** compared to traditional modulus and multiplication operations.

### 3. **Reduced System Calls**
- **Minimized use of `usleep()`**: Instead of calling `usleep(ms * 1000)`, a **precise sleeping loop** continuously checks elapsed time.

### 4. **Efficient Memory Allocation**
- All **philosopher structures** and **mutexes** are allocated in a **single contiguous block** instead of multiple allocations.
- **Memory reuse**: Freed memory is explicitly reset to `NULL` to avoid accidental access.

### 5. **Deadlock Prevention via Smart Fork Pickup Strategy**
- **Even-indexed philosophers** pick up the **left fork first**, then the **right fork**.
- **Odd-indexed philosophers** pick up the **right fork first**, then the **left fork**.
- This ensures that at least **one philosopher always eats**, preventing circular waits.

## Usage
To **compile and run** the program:
```sh
# Clone the repository
git clone https://github.com/iliamunaev/Threads-Linux-Philosophers.git
cd philo

# Build the project
make
```
### Example Usage:
```sh
./philo 5 800 200 200 5
```
```sh
./philo <num_philos> <time_to_die> <time_to_eat> <time_to_sleep> [num_meals]
```
#### Arguments:
| Parameter       | Description |
|----------------|-------------|
| `num_philos`   | Number of philosophers (and forks). |
| `time_to_die`  | Time a philosopher can survive without eating (ms). |
| `time_to_eat`  | Time a philosopher takes to eat (ms). |
| `time_to_sleep`| Time a philosopher sleeps before thinking again (ms). |
| `[num_meals]`  | *(Optional)* Simulation stops after all philosophers eat this many times. |

## Implementation Details

### **Thread Lifecycle**
- Each philosopher thread executes a **routine** of **taking forks, eating, and sleeping**.
- The **monitor thread** continuously checks for **starvation or termination** conditions.
- A **logger thread** efficiently manages console output.

### **Fork Handling**
- **Even-indexed philosophers** pick up the **left fork first**, then the **right fork**.
- **Odd-indexed philosophers** pick up the **right fork first**, then the **left fork**.
- This reduces deadlocks by ensuring **at least one philosopher eats at a time**.

### **Synchronization with Mutexes**
- **Fork mutexes** prevent multiple philosophers from grabbing the same fork.
- A **meal tracking mutex** ensures consistent updates to philosopher states.
- A **printing mutex** prevents log messages from overlapping.

### **Logging System**
| Feature | Description |
|---------|-------------|
| **Buffered Logging** | Log messages are stored and printed in batches. |
| **Efficient Flushing** | The `log_flusher` thread prints stored logs at periodic intervals. |

## File Structure
```
├── philo/
│   ├── philo.h
│   ├── init_env.c
│   ├── init_mutexes_1.c
│   ├── init_mutexes_2.c
│   ├── init_program.c
│   ├── join_threads.c
│   ├── memory_managment.c
│   ├── philo.c
│   ├── philo_routin.c
│   ├── start_threads.c
│   ├── utils.c
│   ├── validate_args.c
│   ├── Makefile
├── docs/
│   ├── mainpage.md
├── .gitignore
├── Doxyfile
├── README.md
```

## Tests

### Test 1:
Compile with
```makefile
 -fsanitize=thread -g
```
Run:
```bash
./philo <num_philos> <time_to_die> <time_to_eat> <time_to_sleep> [num_meals]
```

### Test 2:
Compile with
```makefile
- g
```
Run:
```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./philo <num_philos> <time_to_die> <time_to_eat> <time_to_sleep> [num_meals]
```
Run:
```bash
valgrind --tool=helgrind ./philo <num_philos> <time_to_die> <time_to_eat> <time_to_sleep> [num_meals]
```
Run:
```bash
valgrind --tool=drd ./philo <num_philos> <time_to_die> <time_to_eat> <time_to_sleep> [num_meals]
```
## Philosophers visualizer
https://nafuka11.github.io/philosophers-visualizer/

## **Author**
- **Ilya Munaev**
  - [GitHub](https://github.com/iliamunaev)
  - [LinkedIn](https://www.linkedin.com/in/iliamunaev/)

## **License**
This project is licensed under the **MIT License**.
