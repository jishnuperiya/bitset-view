# q1 A service is not working on a Linux system

Let’s say a service running on Linux is not working as expected. How would you debug it?

### Step 1: Is the service running?

`systemctl status myservice`

You are checking:

- Active / inactive
- Exit code    
- Restart loops
### Step 2: Logs (this is HUGE for him)

`journalctl -u myservice journalctl -u myservice -f`
You’re looking for:
- Missing config
- Permission errors
- Missing libraries
- Network failur
### Step 3: Resource checks

`top free -h df -h`
Common real failures:
- Disk full → service won’t start
- Memory pressure → random crashes
- CPU starvation → timeouts
### Step 4: Ports & networking (VERY relevant to you)

`ss -tulpn lsof -i`
Used when:
- Ethernet data not flowing
- SOME/IP / TCP service unreachable
If the service depends on Ethernet communication, I verify that it’s listening on the expected interface and port.”

### Step 5: strace (bonus but very strong)
`strace -p <pid>`
You use it when:
- Process is “running” but frozen
- Blocking on I/O

Say this calmly:
> “strace helps me understand whether the process is blocked on file, network, or IPC operations.”

---

# SCENARIO 2: “The service worked yesterday, today it doesn’t”

This is a **program manager’s favorite**.

### Strong answer:

> “If it worked before, I first check what changed — deployment, configuration, environment variables, or system updates. **I compare logs from the working and failing versions.”**

Mention:

- Config drift
- Depedency changes
- CI/CD changes

# Permissions issue (very common
### Typical failure
- Service can’t access a file or socket
Tools:
`ls -l id myuser getenforce   # SELinux (if applicable)`
Say:
> “Many production issues come down to permissions, especially after deployments or container changes.”


# Shared library problem (C++ relevance)
Failure:
`error while loading shared libraries: libxyz.so`
Your answer:
`ldd mybinary`
Say:
> “I verify that all shared libraries are present and the correct versions are loaded.”
This connects **Linux + C++**, which he likes.

---
**On Linux systems, I focus on observability first — service status, logs, and resources — before going deeper into runtime debugging.”**

---


# Capturing Ethernet traffic (YOU mentioned this!)

If he asks:

> “How would you debug missing Ethernet data?”

Answer:

> “I would start by checking interface status, then capture traffic using tcpdump at the interface level. I usually apply filters and rotate files to avoid disk issues.”

Command (you don’t need to type it, just know it):

`tcpdump -i eth0`


---
> What do you mean by interface or bridge?”

You say calmly:

> “A network interface is where traffic enters or leaves a system, like a physical Ethernet port.  
> A bridge is a virtual switch used to connect virtual interfaces, for example when applications run in containers.  
> Depending on where the application runs, I capture traffic either on the physical interface or on the bridge.”

---
## Interfaces & Bridges (Linux, explained simply)

An interface is a software representation of a network connection point. It can be physical (like your Wi-Fi card) or virtual (created by the software).

### Types of Interfaces:

- **Physical:** `eth0` or `enp3s0` (Ethernet), `wlan0` (Wi-Fi).
- **Loopback:** `lo` (127.0.0.1), used for the machine to talk to itself.    
- **Virtual:** `veth` (Virtual Ethernet) pairs, often used to connect Linux Containers (LXC) or Docker to the main system.

**Common Commands:**
- `ip link show`: Lists all interfaces and their hardware status.
- `ip addr show`: Shows the IP addresses assigned to those interfaces.

## Linux Bridges

A **Bridge** is a virtual version of a physical network switch. It lives inside the Linux kernel and its job is to forward data packets between different interfaces based on MAC addresses.
### How it works:

When you create a bridge (often named `br0`), you "enslave" other interfaces to it. Once an interface is part of a bridge, it stops acting as an independent entity and starts acting like a port on a switch.

### Why use a Bridge?

1. **Virtualization:** To give Virtual Machines (VMs) access to the physical network. You bridge the physical `eth0` with the VM's virtual interface.
2. **Container Networking:** Connecting multiple Docker containers so they can talk to each other.

to configure network : netplan
netplan uses a yaml file

its important devops, sysadmin roles
understanding bridges and interfaces is crucial for managing virutal amchiens and containers


testing on both **bench** and **vehicle** environments
**The Scenario:** You have a LiDAR sensor sending huge amounts of point-cloud data via **Automotive Ethernet**.

**The Linux Task:** You will likely need to create a **Bridge** on a Linux-based test bench to capture traffic between the LiDAR and the Perception software without interfering with the data (a "Transparent Bridge").

**Latency:** Bridges add a tiny bit of latency. In autonomous driving, milliseconds matter.- 

**SocketCAN:** In Linux, CAN buses are treated as **network interfaces** (e.g., `can0`).
**VLANs (802.1Q):** Automotive Ethernet often uses VLANs to separate sensor data from control data.
    
    - _Command to know:_ `ip link add link eth0 name eth0.10 type vlan id 10`. This creates a virtual interface for a specific VLAN tag.


###  The Bridge (The "Virtual Switch")

Now, imagine you have **five** Containers. You have five virtual cables (veth pairs), but your Host only has **one** physical internet connection. How do you connect all five containers to that one connection?

You need a **Bridge**.

A Bridge (`br0`) acts exactly like a physical **Network Switch** sitting inside your RAM.

1. You take all the "End B" cables from your containers.
    
2. You "plug" them into the Bridge.
    
3. You "plug" your physical internet card (`eth0`) into the Bridge as well.
    

Now, all the containers can talk to each other through the bridge, and they can all share the physical internet connection.


# basics

### 1. The Network Interface Card (NIC)
This is the physical hardware (the "Controller") inside your computer or the LiDAR sensor.
- **Function:** It converts electrical signals from a cable into digital bits that a CPU can understand.
- **Linux Representation:** Linux sees this hardware and creates a **Network Interface** (e.g., `eth0` or `enp3s0`).

### 2. Ethernet (The Protocol)
Ethernet is the "language" used to send data over the wire. In a car, we use **Automotive Ethernet** (100Base-T1).
- **The Frame:** Data is sent in "frames." A frame contains the Destination MAC, Source MAC, and the Data (Payload).
- **The Collision Domain:** In the old days, if two devices talked at once, they crashed. Modern switches and Linux Bridges prevent this by creating isolated paths for each device.

### 3. Channels (Physical vs. Logical)

In your interview, "channels" will likely refer to how we split up that one Ethernet connection so it doesn't get "clogged."

#### Physical Channels

In Automotive Ethernet, you have a **Point-to-Point** channel. Unlike your Wi-Fi at home where 10 devices share one signal, the LiDAR has its own dedicated physical wire "channel" connected directly to the computer.

#### Logical Channels (VLANs)

This is where the **Virtual Interfaces** we discussed come back in. On one physical wire, we create "Logical Channels" using **VLAN Tags**.

- **Channel 1 (VLAN 10):** The "Control Channel." Used for low-speed commands (e.g., "Start Scanning").
    
- **Channel 2 (VLAN 20):** The "Data Channel." Used for the massive LiDAR Point Cloud.
    
- **Channel 3 (VLAN 30):** The "PTP Channel." Used for time synchronization (so the sensor and car agree on exactly _when_ an object was seen).
### Putting it all together: The Flow

Here is how it looks in a real Innoviz testing environment:

1. **Hardware:** The LiDAR sends an Ethernet frame.
    
2. **The Wire:** The frame travels over a single-pair copper "Physical Channel."
    
3. **The NIC:** Your Linux test bench receives the frame on its physical **Interface** (`eth0`).
    
4. **The Bridge:** If you have a **Bridge** (`br0`) configured, it looks at the MAC address. It says, "This is for the Perception Software running in Docker," and forwards it to the correct **Virtual Interface** (`veth`).
    
5. **The Software:** Your Python/C++ code reads the data from that virtual interface and processes the LiDAR image.


In your test environment, you won't just see one messy stream of data. You will use **Virtual Interfaces** to separate them so your automation tools can "listen" to only what they need.

- **Interface `eth0.10` (Point Cloud Channel):** Your Python script for "Object Detection" listens here. It processes millions of points per second.
    
- **Interface `eth0.20` (Blockage/Diagnostic Channel):** Your "System Health" script listens here. It waits for a specific message (like a `SOME/IP` notification) that says "Window Blocked."


### The "Bridge" Role in Testing

When you are testing these systems, you often use a **Bridge** to perform "Packet Sniffing."

If you want to verify that the LiDAR is correctly reporting a blockage (e.g., you put a piece of tape over the lens), you would:

1. **Bridge** the LiDAR's physical port to your PC.
2. Create a **Virtual Interface** for the blockage channel.
3. Use a tool like `tcpdump` or a Python script to verify the message appears on that virtual interface.


### Why Innoviz asks for "Protocol Analysis"

In the job description, they mention **SOME/IP** and **UDS**. These are the "languages" spoken on these channels:

- **Point Cloud:** Usually sent via **UDP raw** or a specialized high-speed protocol.
    
- **Blockage/Diagnostics:** Usually sent via **SOME/IP** (Service Oriented MiddlewarE over IP) or **UDS** (Unified Diagnostic Services).


### Study Tip for your Interview:

If the interviewer asks: _"How would you verify that the sensor correctly detects a blockage while also transmitting a full point cloud?"_

**A Great Answer:**

> "I would configure a Linux bridge to capture the traffic. I'd then create two virtual VLAN interfaces: one for the point cloud and one for diagnostics. Using a Python script with Scapy or a tool like Wireshark, I would monitor the diagnostic channel for the specific SOME/IP 'Blockage Status' flag while ensuring the point cloud throughput remains steady on the other channel."


