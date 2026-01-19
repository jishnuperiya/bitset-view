**“What timing issues did you face?”**

The most common timing issues were timestamp misalignment between LiDAR, camera, and GNSS/INS, especially after sensor restarts or long runs. We also saw latency drift under load, where processing delays caused frames to arrive late or out of sync, even though individual components looked healthy.”

“I usually detected sync problems by correlating timestamps across sensors rather than looking at one stream in isolation. For example, I compared LiDAR frame timestamps with GNSS/INS time and checked whether spatial outputs stayed consistent over time. Sudden jumps, discontinuities, or gradual drift were good indicators of synchronization issues.”

“For me, validating timestamps meant checking monotonicity, continuity, and consistency across the pipeline. I verified that timestamps increased as expected, didn’t jump backward, and stayed aligned across LiDAR, perception outputs, and ground truth. I also looked at long recordings to make sure small errors didn’t accumulate into noticeable drift.”

“We initially tried software-based alignment, but the root cause was inconsistent clock domains. The system moved to a PTP-based setup with a grandmaster clock, which gave us a common time base. That significantly improved synchronization, although we still validated timestamp handling end-to-end.”




**ADTF is a time-driven dataflow framework.**

That means:

- Data samples **carry timestamps**
    
- Filters **process samples in time order**
    
- The framework tries to preserve **temporal consistency** across the graph
    

But ADTF itself does **not magically fix time**.  
It **propagates** time — _you_ must ensure it’s correct.


### Common issues:

- Sensors not synchronized (no PTP)
    
- Sensor restart resets timestamp domain
    
- Filters overwriting timestamps
    
- Mixed time bases (sensor vs system)
    
- Buffering hiding latency
    
- Replaying logs with wrong time interpretation

## regression testing

### 1️⃣ Functional regression

Does it still _work_?

- Does the filter still run?
    
- Does the pipeline still produce output?
    

---

### 2️⃣ Performance regression (very important for you)

Did it get _worse_?

- Latency increase? - lidar timestamp vs recording tiemstamp- timestamp delta analysis
    
- Dropped frames? -  frame IDs. we make plots . check expected frame rate vs calculated
    
- Worse timing alignment? . 
    It means:

- LiDAR, GNSS/INS, camera no longer agree in time
    
- Objects appear shifted
    
- Motion compensation looks wrong

---

### 3️⃣ Quality regression

Is output still _as good_?

- Geometry accuracy
    
- Detection stability
    
- Noise levels
    
- False positives
    

This is where your KPIs come in.


