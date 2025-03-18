 @section intro_sec Introduction

 * This project is a **multi-threaded simulation** of the classic **Dining Philosophers Problem**.
 * The goal is to **prevent deadlocks and starvation** while ensuring philosophers can eat, think,
 and sleep concurrently in a well-coordinated manner.
 *
 @section problem_sec The Problem

 The **Dining Philosophers Problem** consists of `N` philosophers sitting at a circular table  each  requiring two forks (shared with neighbors) to eat.
 The main challenges include:
 * - **Avoiding deadlocks** (e.g., all philosophers picking up one fork and waiting forever).
 * - **Preventing starvation** (ensuring every philosopher gets a chance to eat).
 * - **Ensuring synchronization** (coordinating shared resources correctly).
 *
 @section solution_sec Solution Approach

 The solution uses **mutex locks** to properly manage forks
 and ensure fair access for all philosophers.

 **Key Features:**
 * **Thread Management:** Each philosopher runs in a separate pthread thread.
 * **Mutex Synchronization:** Forks are protected with mutexes to avoid race conditions.
 * **Logging System:** Uses a buffered logging mechanism for efficient output handling.
 * **Precise Timing:** Implements `gettimeofday()` and `usleep()` for accurate time management.
 * **State Machine Logic:** Philosophers follow an optimized eating/thinking/sleeping sequence.
 * **Monitor Thread:** A separate thread monitors if any philosopher dies from starvation.
 * **Optimized Performance Techniques** (see below).

 @section optimizations_sec Performance Optimizations

 Several techniques are used to **reduce the computational load** and improve performance:

 **1. Buffered Logging (Batch Processing)**
 - Instead of calling `printf()` multiple times (which is slow), a **log buffer** stores messages.
 - The **log_flusher thread** periodically flushes stored messages in bulk, reducing I/O overhead.
 *
 **2. Bitwise Operations for Fast Computation**
 - The program uses **bitwise AND (`&`)** and **bitwise shift (`<<`)** operations to optimize:
 - Checking if a philosopher’s ID is **odd or even**: `p->id & 1` instead of `p->id % 2`.
 - Multiplying by 2 using `p->eat_time << 1` instead of `p->eat_time * 2`.
 - These reduce **CPU cycles** compared to traditional modulus and multiplication.

 **3. Reduced System Calls**
 * **Minimized use of `usleep()`**: Instead of calling `usleep(ms * 1000)`, I use a **precise sleeping loop**
 *   that continuously checks elapsed time. This ensures better timing precision with lower overhead.
 *
 **4. Efficient Memory Allocation**
 * **All philosopher structures and mutexes are allocated in a single contiguous block** instead of multiple allocations.
 * **Memory reuse:** Freed memory is explicitly reset to `NULL` to avoid accidental access.
 *
 **5. Deadlock Prevention via Smart Fork Pickup Strategy**
 * Even-indexed philosophers pick up the **left fork first**, then the **right fork**.
 * Odd-indexed philosophers pick up the **right fork first**, then the **left fork**.
 * This ensures that at least **one philosopher always eats**, preventing circular waits.
 *
 @section usage_sec Usage

 * To compile and run the program:
 ```bash
  git clone https://github.com/iliamunaev/Threads-Linux-Philosophers.git
  cd philo
  make
```
 *
 * Example:
```bash
./philo 5 800 200 200 5
```
```bash
./philo `num_philos` `time_to_die` `time_to_eat` `time_to_sleep` `[num_meals]`
```
 * `num_philos` = Number of philosophers (and forks).
 * `time_to_die` = Time a philosopher can survive without eating (ms).
 * `time_to_eat` = Time a philosopher takes to eat (ms).
 * `time_to_sleep` = Time a philosopher sleeps before thinking again (ms).
 * `[num_meals]` (optional) = If provided, simulation stops after all philosophers eat this many times.
 *
 @section implementation_sec Implementation Details

 **Thread Lifecycle:**
 * Each philosopher thread executes a **routine** of taking forks, eating, and sleeping.
 * The **monitor thread** continuously checks for starvation or termination conditions.
 * A **logger thread** efficiently manages console output.
 *
 **Fork Handling:**
 * Even-indexed philosophers pick up the **left fork first**, then the **right fork**.
 * Odd-indexed philosophers pick up the **right fork first**, then the **left fork**.
 * This **reduces deadlocks** by ensuring at least one philosopher eats at a time.
 *
 **Synchronization with Mutexes:**
 * **Fork mutexes** prevent multiple philosophers from grabbing the same fork.
 * A **meal tracking mutex** ensures consistent updates to philosopher state.
 *  A **printing mutex** prevents log messages from overlapping.
 *
 **Logging System:**
 *   - Log messages are **buffered** to avoid excessive `printf()` calls.
 *   - The **log_flusher thread** prints stored logs at periodic intervals.
 *
 @section files_sec File Structure

 ```bash
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
 @section authors Author
 **Ilya Munaev** ([**LinkedIn**](https://www.linkedin.com/in/iliamunaev/)) ([**GitHub**](https://github.com/iliamunaev))

 @section license_sec License

This project is licensed under the **MIT License**.

