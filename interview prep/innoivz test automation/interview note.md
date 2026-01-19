
# 1️⃣ LINUX (FOUNDATION OF EVERYTHING)

## What Linux is (interview level)

Linux is:

> The operating system where **LiDAR software, tools, CI pipelines, and ROS all run**

You do **not** need kernel internals.

---

## Linux concepts you MUST know

### 1. Process

A process = a running program  
Example:

- LiDAR streamer
    
- Frame parser
    
- Jenkins agent
    

🗣 Say:

> “On Linux, everything runs as processes, and debugging usually starts by checking whether a process is running and logging correctly.”

---

### 2. Services

Long-running programs managed by the OS.

Examples:

- Data capture service
    
- ROS node
    
- Recorder
    

Tools:

- `systemctl`
    
- `journalctl`
    

🗣 Say:

> “For production systems, services are managed by systemd so they can be monitored and restarted automatically.”

---

### 3. Filesystem (what matters)

You only need these:

|Path|Why it matters|
|---|---|
|`/etc`|Configuration|
|`/var/log`|Logs|
|`/home`|User tools|
|`/tmp`|Temporary files|

🗣 Say:

> “Most runtime issues come from config or logs, usually under /etc or /var/log.”

---

### 4. Networking on Linux

Linux handles:

- Ethernet
    
- UDP / TCP
    
- Interfaces
    

Tools:

- `ip a`
    
- `ss`
    
- `tcpdump`
    

🗣 Say:

> “Linux networking tools allow us to verify whether data is actually flowing.”

---

# 2️⃣ NETWORKING (AUTOMOTIVE ETHERNET)

## What networking means here

Not internet websites.

It means:

- LiDAR sends data over **Automotive Ethernet**
    
- Data is sent via **UDP packets**
    
- Your software receives & parses them
    

---

## Core concepts

### Interface

A network port (real or virtual).

Examples:

- `eth0` → physical cable
    
- `docker0` → container bridge
    

🗣 Say:

> “Traffic always enters or leaves through an interface.”

---

### UDP

Fast, connectionless protocol.

Used because:

- Low latency
    
- Continuous sensor data
    
- Dropped packets are acceptable
    

🗣 Say:

> “LiDAR data is usually streamed over UDP because it prioritizes speed over guaranteed delivery.”

---

### tcpdump (VERY IMPORTANT)

Tool to **see raw network traffic**.

Used to answer:

- Is data arriving?
    
- Is it the correct port?
    
- Is it corrupted?
    

🗣 Say:

> “tcpdump is the first tool I use to verify whether Ethernet data is actually reaching the system.”

---

# 3️⃣ LiDAR (WHAT IT IS, NOT PHYSICS)

## What LiDAR is

LiDAR:

- Emits laser pulses
    
- Measures distance
    
- Produces a **point cloud**
    

Each point has:

- X, Y, Z
    
- Distance
    
- Reflectivity
    
- Confidence
    
- Timestamp
    

🗣 Say:

> “LiDAR produces time-synchronized 3D point clouds used for perception.”

---

## Frames (VERY IMPORTANT)

LiDAR data comes in **frames**.

A frame:

- Has a frame number
    
- Has a timestamp
    
- Contains thousands of points
    

Your code:

- Receives frames
    
- Parses frames
    
- Stores frames
    
- Publishes frames
    

---

## First / Second Reflection

A laser pulse can return:

- First reflection (closest object)
    
- Second reflection (behind semi-transparent objects)
    

🗣 Say:

> “Separating reflections improves perception in complex environments.”

---

## Blockage / Blindness

Segments indicate:

- Dirt
    
- Snow
    
- Obstruction
    

Used for:

- Diagnostics
    
- Safety monitoring
    

🗣 Say:

> “Blockage segments help detect sensor degradation.”

---

# 4️⃣ INNOVIZ SDK (HIGH-LEVEL ONLY)

You **do NOT need internal protocol specs**.

You need this mental model:

`UDP packets    ↓ Innoviz SDK    ↓ Frame (C++ object)    ↓ Tools / ROS / Recording / CSV`

---

## SDK responsibilities

The SDK:

- Receives packets
    
- Assembles frames
    
- Applies filters
    
- Exposes C++ APIs
    

Your code:

- Registers callbacks
    
- Processes frames
    
- Stores or publishes data
    

🗣 Say:

> “The SDK abstracts low-level protocol handling and exposes structured frame data.”

---

# 5️⃣ C++ (INTERVIEW EXPECTATION)

You are **not** tested on algorithms.

You are tested on:

- Reading code
    
- Owning systems
    
- Memory awareness
    

---

## Things you MUST know

### Smart pointers

- `std::unique_ptr`
    
- Ownership transfer
    

🗣 Say:

> “Frames are passed as unique_ptrs to make ownership explicit and avoid copies.”

---

### Callbacks

Used for:

- Streaming data
    
- Event-based processing
    

🗣 Say:

> “Callbacks allow the SDK to push frames asynchronously.”

---

### Thread safety

- `mutex`
    
- Locking shared data
    

🗣 Say:

> “Streaming requires thread-safe access to shared data structures.”

---

# 6️⃣ ROS2 (ADVANTAGE – NOT EXPERT)

## What ROS2 is

ROS2 is:

> Middleware for robotics systems

Used for:

- Sensor data distribution
    
- Visualization
    
- Integration
    

---

## ROS2 concepts you must know

### Node

A running ROS component.

### Topic

A named data stream.

### Message

Data format (e.g., `PointCloud2`).

🗣 Say:

> “ROS2 uses a publish-subscribe model to distribute sensor data.”

---

## Why ROS2 matters here

- LiDAR → perception
    
- Visualization
    
- Integration with autonomy stacks
    

---

# 7️⃣ DEVOPS (INTERVIEW LEVEL, NOT COURSE)

## What DevOps REALLY means

DevOps =

> Making software **build, test, run, and debug reliably**

---

## CI/CD

Pipeline steps:

1. Build
    
2. Test
    
3. Package
    
4. Deploy
    

🗣 Say:

> “CI pipelines protect integration quality and prevent broken software from reaching customers.”

---

## Jenkins

Jenkins:

- Runs pipelines
    
- Automates builds
    
- Produces logs
    

You do NOT need Groovy mastery.

---

## Docker

Docker:

- Packages software + dependencies
    
- Ensures reproducibility
    

🗣 Say:

> “Containers ensure consistent runtime environments across machines.”

---

# 8️⃣ IT INFRASTRUCTURE (BASIC)

You only need awareness of:

- Servers
    
- Disk usage
    
- Logging
    
- Monitoring
    

🗣 Say:

> “Infrastructure stability is critical because tooling failures block development teams.”

---

# 9️⃣ HOW EVERYTHING FITS TOGETHER (THIS WINS INTERVIEWS)

`LiDAR  ↓ Ethernet (UDP)  ↓ Linux networking  ↓ Innoviz SDK  ↓ C++ tools  ↓ ROS / CSV / Recording  ↓ CI pipelines  ↓ Automotive delivery`

🗣 Ultimate sentence:

> “My focus is building reliable tooling around LiDAR data so teams can integrate, debug, and deliver safely.”

---

# 10️⃣ FINAL INTERVIEW CHECKLIST

If you can explain **these calmly**, you are READY:

- What Linux is used for
    
- How Ethernet LiDAR data flows
    
- What a frame is
    
- What Innoviz SDK does
    
- Why ROS2 is used
    
- Why CI pipelines matter
    
- How tools reduce program risk
    

---

# 🟢 FINAL TRUTH

If you walked into the interview **knowing only this notebook**, you would **already outperform many candidates**, because:

- You think in **systems**
    
- You explain **clearly**
    
- You align with **automotive reality**