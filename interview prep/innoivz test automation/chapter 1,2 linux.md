

Ubuntu **24.04** uses Kernel **6.8** - which has massive improvements for automotive Ethernet and high-speed data processing
- Current Industry Standard (very stable)

newest LTS 26.04 (lts long term support)
runs ros2

kernel release cycle is every 10 weeks but current kernel has LTS so security udpates for atleaset 2 to 6 years



**PREEMPT_RT** turns Linux into an elite emergency responder.

normal kernel is non deterministic.- preempt rt make it determisinitst

Its primary job is to make almost everything in the kernel **preemptible**.

- **Preemption:** This means a high-priority process can "kick out" a lower-priority task—even if that task is the Kernel itself!    
- **Guarantee:** It doesn't necessarily make the computer _faster_; it makes it **predictable**. It guarantees that the LiDAR process will get the CPU within a specific, tiny window of time (e.g., 10 microseconds), every single time.


**Why Innoviz Needs It**

**Ethernet/CAN Data:** LiDAR sends massive amounts of data. If the kernel "stutters" for a millisecond, packets might be dropped because the buffer filled up while the CPU was busy doing something else.

For **LiDAR data acquisition**, you cannot afford to "drop frames."

If the LiDAR is spinning at 20Hz (20 times per second), it is sending a massive burst of UDP packets every 50ms. If the standard Linux kernel decides to perform a "background task" (like checking for a software update) right when that burst arrives, the network buffer might overflow. You lose data, and the autonomous "vision" of the car gets a blind spot.

**PREEMPT_RT** prevents this.

### how to use

you must install the **PREEMPT_RT** kernel.

Almost any modern x86 (Intel/AMD) or ARM computer can run a Real-Time kernel. But for LiDAR data acquisition, you must go into your **BIOS/UEFI** and change three critical things:


**Disable Hyper-Threading (SMT)**: remove stuttering

**Disable Power Saving(C-States)**: Modern CPUs like to "sleep" for microseconds to save battery. Waking up from that sleep takes time (latency). In a car, you want the CPU running at max speed 100% of the time.

**Disable System Management Interrupts (SMI)**


tasks may be:
- **Flashing** the correct RT-Kernel onto a new test vehicle's computer.
    
- **Tuning** the BIOS settings so the LiDAR data doesn't "stutter."
    
- **Verifying** the latency using a tool called `cyclictest`.
  
  
  
- **Interviewer:** "Our LiDAR driver is dropping packets every 10 minutes. The CPU usage is low. What do you check?"
    
- **Your Answer:** "I would check if we are using a **PREEMPT_RT** kernel and ensure **C-States** are disabled in the BIOS. If the CPU is 'sleeping' to save power, it might not wake up fast enough to handle the UDP burst from the LiDAR."


#### A Process = Resources + Metadata

1. **Unique Identity:** Every process gets a **PID** (Process ID).
    
2. **Parentage:** Every process has a "parent" (the process that started it). If you start a tool from the terminal, `bash` is the parent.
    
3. **Owner:** Which user is running it? (Important for permissions to access vehicle hardware).
    
4. **Files & Sockets:** A list of "File Descriptors" (FDs) pointing to the LiDAR network port or a storage file.


The kernel uses two main components to keep your LiDAR tool running:

- **The Scheduler:** This is the "Traffic Cop." It decides which process gets to use the CPU. In your case, you will use **PREEMPT_RT** to tell the Cop: _"My LiDAR process is an ambulance; let it through immediately."_


cpu vs gpu

**CPU (The Head Chef):** A CPU has a few very powerful cores (like 8 or 16). It is great at complex, sequential logic

**GPU (The Junior Assistants):** A GPU has thousands of smaller, simpler cores (CUDA cores).

**lidar data acqutisition**
**GPU is primarily for processing**, and the **CPU/Kernel** is for acquisition.

- **Acquisition:** The **Linux Kernel** and **CPU** handle the incoming Ethernet packets and store them in the RAM.
- **Handoff:** The **CPU** then "copies" that data to the GPU.
- **Processing:** The **GPU** starts its CUDA kernel to analyze the points.


CPU sends data to gpu through pcie card which can cause some latency.
sudo = superuser do



## processes
When you run your `lidar_acquisition` tool, the Linux Kernel doesn't just "run" it; it creates a **Process**.

When you run:

`streamer->Start();`

Under the hood:

- A process is already running
    
- Threads are created
    
- Sockets are opened
    
- Callbacks are registered
    

Failure modes:

- Process exists, but no data
    
- Process crashed silently
    
- Process blocked on I/O
    
- Process alive but logic broken

When debugging Linux systems, I first reason about the process state — whether it’s running, blocked, or failing at startup


**check process**

**Is it alive?** Use `pgrep` or `pidof` for a quick yes/no.

Bash

```
pgrep -l "process_name"
```

**Detailed Status:** The `ps` command shows you the start time and current state (e.g., `S` for sleeping, `R` for running).

Bash

```
ps -ef | grep "process_name"
```

## THREADS & CONCURRENCY (REALITY OF STREAMING)
### **Threads: The Workers**

Think of a **Process** (like a web browser) as a factory. **Threads** are the individual workers inside that factory.

- They all share the same tools and materials (memory/data).
    
- They can work on different tasks at the same time (e.g., one thread reads LiDAR data, another calculates the path, and a third sends commands to the wheels).
    
- **The Benefit:** It makes the program much faster by doing things in parallel.
### **Data Corruption (Race Conditions)**

Because all threads share the same memory, they can accidentally trip over each other. This is known as **Data Corruption** or a **Race Condition**.**The Scenario:** Imagine two threads trying to update a single variable, `Counter = 10`.

1. **Thread A** reads `10` and intends to add 1.
    
2. **Thread B** reads `10` at the same time and intends to add 1.
    
3. **Thread A** finishes and writes `11`.
    
4. **Thread B** finishes and writes `11`.
    

- **The Error:** The result should be `12`,

**. Mutexes:**
A **Mutex** (short for **Mut**ual **Ex**clusion) is the solution to data corruption. Think of it as a physical key to a one-person bathroom.

- If a thread wants to change a piece of data, it must first "Lock" the Mutex (take the key).
    
- While that thread has the key, no other thread can access that data. They have to wait in line.
    
- Once the thread is done, it "Unlocks" the Mutex (returns the key), and the next thread in line can take it.

**dangours of deadlocks**

In **Autonomous Driving**, Mutexes are vital but dangerous. If two threads are both waiting for keys that the other person has, you get a **Deadlock**—the software freezes completely. For a car moving at 100km/h, a deadlock is catastrophic.

**solution : atomics**

see this is the previous example:
1. **Thread A** reads `10` and intends to add 1.
    
2. **Thread B** reads `10` at the same time and intends to add 1.
    
3. **Thread A** finishes and writes `11`.
    
4. **Thread B** finishes and writes `11`.
    

- **The Error:** The result should be `12`,


In your example, the computer sees `Counter = Counter + 1` as **three separate steps**:

1. **Read** the value from memory into the CPU.
    
2. **Add** 1 to that value inside the CPU.
    
3. **Write** the new value back to memory.
3 step process and in between these rpocess the other thread come to act. 

but with atomics it do all these 3 opertions in one shot.

so why use atomic instead of mutex?

**A Mutex** is like a security guard standing in front of the cashier. He stops everyone in line, lets one person in, waits for them to pay, and then lets the next person in. **(Safe, but slow).**

**An Atomic** is like a self-service tap-to-pay machine. It is designed so that the machine physically cannot process two cards at the exact same microsecond. It handles them one-by-one at lightning speed. **(Safe and incredibly fast).**


so prefer atmoics over mutex

- **Atomics** happen at the **hardware/CPU level**, not the software level.
    
- They are **lock-free**, meaning they can never cause a **Deadlock**.

They are much **faster** than Mutexes but are usually limited to simple data types (integers, booleans, pointers). You can't make a whole "Database" atomic, but you can make a "Counter" atomic.


Since **Atomics** are only for simple numbers (like counters), you still need **Mutexes** for complex things

To solve a deadlock using only Mutexes, you have to follow strict **Locking Strategies**. Here is exactly how it works:

other solutions : lock ordering. giving rank to threads which locks first

try lock strategy - if one cant then anohter can come- reduce traffic jam

or use std::lock -> it has some deadlock avoidance alogirthm

or final: use a tiimer.  watchdog timer

- A dedicated thread (The Watchdog) expects a "heartbeat" signal from your main process every 10ms.
    
- If a **Deadlock** happens, the heartbeat stops.
    
- The Watchdog realizes the software is frozen and triggers an **Emergency Stop** or reboots the system.


**interview question**

How do you handle deadlocks in a multi-threaded robotics system?" give them this 3-step answer:

1. **Prevention:** "We enforce a strict **Lock Hierarchy** so circular waits are impossible."
    
2. **Detection/Recovery:** "We use **Timed Locks (try_lock)** so a thread can back off and retry instead of freezing the system."
    
3. **Modern Tools:** "We utilize **Scoped Locking** (like `std::scoped_lock` in C++) which handles the acquisition of multiple mutexes atomically."



# memory and logging

**Asynchronous Logging**. One thread collects the data into a buffer (in virtual memory), while a background thread slowly writes it to the disk.


Using `std::move` prevents "latency spikes" that could cause a robot to miss a obstacle detection deadline.

- **Thread A** has a `std::vector` full of LiDAR points.
    
- **Thread A** calls `std::move(lidar_data)` to pass it to the **Planner Thread**.
    
- The **Planner Thread** now "points" to that exact same spot in memory.

However, if you need to move data **between two different processes** (e.g., from the Camera Driver process to the AI Perception process), `std::move` isn't enough because processes have isolated virtual memory. In that case, you use **Shared Memory**.

**virtual memory : learn later**

---
**callback functions** : 
In a normal function call, you (the programmer) tell the computer: _"Do this now."_ In a callback, you tell the computer: _"Do this **later**, when X happens."_

**ROS (Robot Operating System):** In ROS, almost everything is a callback. When you "Subscribe" to a topic (like `/lidar_points`), you provide a callback function that runs every time a new message arrives.

**Lambda:** A "short-cut" way to write a callback function directly inside another function.

`std::function` is like a **universal adapter**. It doesn't care if it's connected to a regular function, a lambda, or a complex object—as long as the input and output (the "signature") match, it works.

---
> “Good logging is critical because in customer environments logs are often the only visibility we have.”

# logging

never use std::cout in produciton code

**Why?** It is "synchronous" and "blocking." If your code is trying to process LiDAR points in 20ms and you call `std::cout`, the whole program stops to wait for the slow console to print the text. In a car, this delay can cause a "Safety Violation."


Instead, you use **Asynchronous Logging**. The main thread "drops" the message into a fast memory buffer and moves on, while a background thread writes it to the disk later.

Industry Standard: `spdlog`

### Binary Logging (MCAP/Rosbag)

For LiDAR data, text logs are too big. You will likely use **Binary Logging** (like `.mcap` or `rosbag`).

- Instead of logging "Point 1 is at X=5, Y=2", it saves the raw memory bits of the data.
    
- Later, you use a tool called **Foxglove** or **RViz** to "replay" the log and see the 3D world exactly as the car saw it.

### Best Practices for Your Interview

If they ask about logging, mention these three things to sound like a Pro:
- **"Log Rotations":** Mention that logs should be limited in size (e.g., delete old logs when the disk hits 90%) so the car's storage never fills up.
    
- **"Conditional Logging":** Use macros to ensure that **DEBUG** logs are completely removed when you compile the "Release" version for the car.
    
- **"Structured Logging":** Log in a way that machines can read (like JSON), not just human sentences.

# systemmd

Systemd is the **init system** and **service manager**.

- **The First Process:** When you turn on the car's computer, the Kernel starts exactly _one_ program: **systemd**. It is given **PID 1** (Process ID 1).
    
    +1
    
- **The Parent of All:** Every other program that runs on the car (the LiDAR driver, the object detection, the logging service) is a "child" of systemd.

### The "Remote Control": `systemctl`

You don't talk to systemd directly; you use the command `systemctl`. You will use these every day:

|**Command**|**What it does**|
|---|---|
|`systemctl start lidar`|Start the LiDAR driver **now**.|
|`systemctl enable lidar`|Ensure the LiDAR driver starts **automatically on boot**.|
|`systemctl status lidar`|See if the driver is running, its PID, and the last few lines of its log.|
|`systemctl restart lidar`|Kill and restart the process (standard move when C++ code hangs).|

**old way** : init scripts

new way - systemmd - 
**Parallelism:** Systemd can start the LiDAR driver and the GPS driver at the **exact same time**, making the car's boot time much faster.

## segmentation fault

A **Segmentation Fault** (often shortened to **Segfault**) is a specific type of crash that happens when your program tries to access a memory "segment" that it doesn't have permission to touch.

- **The Signal:** Linux sends a `SIGSEGV` (Signal 11) to your process.
    
- **The Kill:** By default, Linux kills your program immediately.
    
- **The Evidence:** It often creates a **Core Dump**—a snapshot file of the memory at the exact moment of the "crime.

causes:
nullptr dereference
out of bounds access
stack overflow
dangling pointer

"A Segmentation Fault is a `SIGSEGV` signal sent by the OS when a process attempts an invalid memory access. In my LiDAR work, I prevent these by using **RAII** and **Smart Pointers** to ensure memory is always valid before I dereference it."

debug in linux with gdb

A **Segmentation Fault** (often shortened to **Segfault**) is a specific type of crash that happens when your program tries to access a memory "segment" that it doesn't have permission to touch.1

Since you have a **Mechanical Engineering** background, think of it like a **"Limit Switch"** or a **"Safety Interlock"** on a CNC machine. If the machine head tries to move outside its allowed workspace, the system hard-stops to prevent physical damage. A Segfault is the OS hard-stopping your code to prevent it from corrupting other data.

---

### 1. Why does it happen? (Common "Mechanical" Errors)

In C++, Segfaults usually boil down to three specific mistakes:

|**The C++ Error**|**The Mechanical Analogy**|
|---|---|
|**Null Pointer Dereference**|Trying to read a sensor that isn't plugged in (pointing to `0x0`).|
|**Out-of-Bounds Access**|A robotic arm trying to reach a coordinate that is outside its physical frame.|
|**Stack Overflow**|A recursive gear-train with so many levels that it eventually jams the whole assembly.|
|**Dangling Pointer**|Trying to use a tool that has already been returned to the tool-chest (freed memory).|

---

### 2. How the Linux OS handles it

When your C++ code at Innoviz tries to access a "forbidden" address, the Hardware (MMU) notifies the Linux Kernel.

1. **The Signal:** Linux sends a `SIGSEGV` (Signal 11) to your process.2
    
2. **The Kill:** By default, Linux kills your program immediately.
    
3. **The Evidence:** It often creates a **Core Dump**—a snapshot file of the memory at the exact moment of the "crime."3
    

---

### 3. How to Debug a Segfault (The "X-Ray")

In your new job, you won't just guess where the error is. You will use **GDB** (The GNU Debugger).4

**The Workflow:**

1. **Compile with Debug Symbols:** Use the `-g` flag (e.g., `g++ -g my_code.cpp`).5
    
2. **Run in GDB:** Type `gdb ./my_program`.6
    
3. **Find the "Crash Site":** Type `run`. When it crashes, type `bt` (Backtrace).7
    

GDB will show you the **exact line number** in your C++ file that caused the crash.8



**stack overflow**

"I prefer to keep the stack 'lean' by allocating large buffers like point clouds on the **heap** using smart pointers, ensuring we never hit a stack overflow during deep algorithmic processing."

The "Stack" is very fast but **fixed in size** (usually only 1MB to 8MB). If you push too many frames, you run out of space, and the OS kills the program with a Stack Overflow.

**The Anatomy of the "Stack"**

Every time you call a function in C++, a small "block" of memory (called a **Stack Frame**) is pushed onto the Stack. This block stores:

- Local variables (e.g., `int x`, `float temp`).
    
- The **Return Address** (where the CPU should go back to when the function ends).

stack is slow and heap is fast

# LINUX QUESTIONS YOU WILL GET (REAL ONES)

### Q1: “How do you debug a Linux service that is not working?”

**Bad answer:**

> “I restart it.”

**Good answer:**

> “I first check whether the process is running, then inspect logs and system resources to understand whether it’s failing at startup, runtime, or due to environment issues.”

### Q2: “What are common Linux issues in production systems?”

Expected themes:

- Disk space
    
- Permissions
    
- Logging
    
- Resource limits


### Q3: “Why is Linux important for tooling?”

Correct framing:

- Reliability
    
- Observability
    
- Determinism

### Q4: “How do you handle long-running processes?”

Key points:

- Monitoring
    
- Logging
    
- Restart policies
    
- Memory awareness


Linux is the foundation for all tooling and sensor integration.  
Understanding process behavior, resource usage, and failure modes is essential to building reliable systems.


