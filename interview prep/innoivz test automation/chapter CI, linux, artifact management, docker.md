
### 3. Artifact Management

Once a binary is built, it becomes an **Artifact**.

- **Immutability:** Once an artifact is "Golden" (tested), it must never be changed. It is stored in a repository like **JFrog Artifactory** or **Nexus**.
    
- **Traceability:** Every artifact must be linked to a specific Git Commit Hash and a specific version of the compiler used to build it.


### 1. Containers as Reproducible Runtimes

In sensor software, the "Toolchain" (compilers, libraries, flashing tools) is complex.

- **The Benefit:** Instead of telling an engineer "Install these 50 libraries," you give them a Docker image. Everyone uses the **exact same version** of the compiler and headers, eliminating the "it works on my machine" excuse.

### Networking inside Containers

- **The Pitfall:** Docker’s default "Bridge" network adds latency.
    
- **The Sensor Reality:** Sensor data (LiDAR/Camera) is often massive (Gigabits/sec).
    
- **Solution:** Use `--net=host`. This bypasses Docker's network stack and lets the container talk directly to the sensor at wire speed.

**Real-Time Requirements:** If your software needs **Hard Real-Time** (microsecond precision), the tiny overhead of the containerized process can sometimes introduce "jitter" that breaks the system.



# jenkins in my task

You owned the **release-to-data pipeline** for LiDAR validation up to cloud ingestion.

Your flow was:

1. Receive a new **software release**
    
2. Develop an **ADTF filter** for that release
    
3. Package it with **Conan**
    
4. Upload artifacts to **JFrog Artifactory**
    
5. Hand over the filter to test engineers
    
6. After testing, process **MF4 / ADTFDAT logs**
    
7. Run a **standalone HDF5 converter**
    
8. Upload generated HDF5 data to cloud for KPI analysis
    

This is a **multi-stage pipeline** — perfect for Jenkins.



At TechHub, Jenkins was used to automate parts of our LiDAR software release and validation workflow. When a new sensor software release was available, Jenkins jobs helped orchestrate building and packaging ADTF filters, publishing artifacts to JFrog Artifactory, and triggering downstream validation steps.



I owned the pipeline up to generating and uploading validated HDF5 data. That included developing the ADTF filter, packaging it with Conan, publishing it to Artifactory, processing MF4 and ADTFDAT logs, and running my standalone HDF5 converter. The cloud-side KPI analysis was handled by another team.”
Yes, I worked with Jenkinsfiles written in Groovy. I didn’t design the entire CI system from scratch, but I modified and extended existing pipelines — for example to add build steps, artifact publishing to Artifactory, and parameterized jobs for different software releases.

“A typical Jenkins job would take a specific software release version as input, build the corresponding ADTF filter using Conan, publish the artifacts to JFrog Artifactory, and make them available for testing. After test execution, Jenkins jobs were also used to run log-processing steps where MF4 and ADTFDAT files were converted into HDF5 for downstream analysis.”

“The Groovy I used was mainly within declarative Jenkins pipelines — defining stages, parameters, environment variables, shell steps, artifact handling, and post-build actions. I also debugged Groovy issues when pipelines failed.”

**Your workflow is partially automated, not fully automated.**


### Step 1: New software release arrives

**Trigger:** Manual  
You receive a new Innoviz software release.

🟡 **Not automated** (human decision point)

---

### Step 2: Develop ADTF filter for the release

**Trigger:** Manual  
You write / update the ADTF filter.

🔴 **Not automated** (engineering work)

---

### Step 3: Build & package ADTF filter with Conan

**Trigger:** Can be automated  
If you run:

- `conan build`
    
- `conan create`
    

This step is **scriptable**.

🟢 **Automatable**  
🟡 **Likely semi-automated** (manual trigger, automated execution)

---

### Step 4: Upload ADTF filter to JFrog Artifactory

**Trigger:** Can be automated  
If done via:

- `conan upload`
    
- Artifactory REST API
    

🟢 **Automated** (once triggered)

---

### Step 5: Test engineers run vehicle / bench tests

**Trigger:** Manual  
Humans decide:

- which scenarios to run
    
- when tests are finished
    

🔴 **Not automated**  
(This is reality — hardware & vehicles)

---

### Step 6: MF4 / ADTFDAT logs are produced

**Trigger:** Automatic as a result of testing

🟢 **Automated output**

---

### Step 7: Run HDF5 converter on logs

**Trigger:** Scripted  
Your standalone converter is a program.

🟢 **Automated execution**  
🟡 **Manual trigger**

---

### Step 8: Upload HDF5 data to cloud

**Trigger:** Scriptable (CLI / API)

🟢 **Automated** (once triggered)

---

### Step 9: Cloud KPI analysis

**Trigger:** Automated (but not yours)

🟢 **Automated, owned by another team**


```
Git push / release tag
        ↓
Jenkins triggered automatically
        ↓
Build ADTF filter (Conan)
        ↓
Publish artifact to Artifactory
        ↓
Trigger test execution (HIL / SIL)
        ↓
Collect MF4 / ADTFDAT logs
        ↓
Run HDF5 converter
        ↓
Upload to cloud
        ↓
Trigger KPI jobs
        ↓
Report results
```


```
This sentence wins respect:

Not everything should be automated — only what is stable and repeatable.
```


**groovy = jenkinsfile**




In Jenkins, **Groovy is not used like application code**.  
You mostly write **pipeline glue**:

- define stages
    
- call shell commands
    
- pass parameters
    
- handle artifacts
    

That’s it.

---

I worked with declarative Jenkins pipelines written in Groovy, defining stages, parameters, environment variables, and shell steps to build ADTF filters with Conan, upload artifacts to Artifactory, and run log-processing tools.

---
# use of docker

You use Docker when:

- tools have messy dependencies
    
- environments differ across machines
    
- you want “works the same everywhere”


## where i did used dcoker:


Jenkins
  └── Docker container
        ├── Conan build
        ├── ADTF filter packaging
        └── HDF5 conversion

### 1. hdf5 converter
Dependency-heavy (HDF5, C++, libs)

### 2. adtf filter

**What’s in the container:**

- ADTF SDK
- compiler toolchain
- Conan
- dependencies

### **Offline playback / log replay**

If your ADTF filter
- consumes MF4 / ADTFDAT
- produces outputs
- does not talk to live hardware

Then you can run:
- ADTF runtime
- filter
- playback

**inside Docker**.

I worked with containerized build and validation environments for ADTF filters. On-vehicle execution stayed native due to real-time and certification constraints.

“I worked up to cloud ingestion. Kubernetes was downstream of my ownership.”

## Why Kubernetes exists (the real reason)

Imagine you have:

- Docker containers
- running on many machines
- in a data center or cloud

Think of Kubernetes as:

> **An operating system for a cluster of machines**

- starts processes
    
- restarts them if they crash
    
- manages resources (CPU, memory)
    
- provides networking
    

Kubernetes does the same — **but for containers across many machines**.


```
            (Jenkins)
                ↓
        HDF5 uploaded to cloud
                ↓
        Kubernetes Job starts
                ↓
        KPI containers process data

```
You were **before** Kubernetes, not inside it.

That’s why you should not claim hands-on K8s.


“Kubernetes orchestrates containerized workloads in the cloud; my work focused on producing reliable artifacts and data before that layer.”

Docker packages. Jenkins automates. Kubernetes operates at scale.

### Kubernetes is relevant for you **ONLY when**:

✅ You move into **cloud-side data processing**  
✅ You own **batch jobs or services** (not just artifacts)  
✅ You deploy **analysis pipelines at scale**


Sensors → C++ → ADTF → Jenkins → Artifacts → Cloud

Cloud → Kubernetes Jobs → Analysis Services → Dashboards

“I understand what Kubernetes does and where it fits in the pipeline, but I haven’t operated clusters myself. My hands-on work stopped at CI, Dockerized tools, and data preparation before cloud ingestion.”

“I didn’t manage Kubernetes clusters directly, but I worked with systems where Jenkins-produced artifacts and data were consumed by Kubernetes-based services downstream.”
- Docker → **how software is packaged**
    
- Jenkins → **how steps are automated**
    
- Kubernetes → **how workloads run at scale**
    
- You → **build reliable systems before the cloud layer**

---

, I worked entirely on Linux and used Docker for reproducible experiments and tooling. In my industry roles, development was primarily Windows-based due to ADTF and OEM tooling, with CI and automation handled separately.”

Docker was used during my master’s thesis work to isolate dependencies and ensure reproducibility of experiments. The thesis environment was fully Linux-based.”

“In the automotive ADTF environment, development is tightly coupled to Windows tooling and licensed middleware. Docker is more useful for build or offline processing stages, not day-to-day ADTF development.”



