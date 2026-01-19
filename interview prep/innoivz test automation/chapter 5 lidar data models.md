
# time synchronization

#### **PTP (Precision Time Protocol - IEEE 1588)**

**The Hardware Layer:** The Grandmaster sends PTP packets (Sync, Follow_Up) over Automotive Ethernet. The LiDAR and your Linux PC act as **Slave Clocks**.

On your Linux machine, you use the `linuxptp` toolchain to manage this synchronization. It consists of two main "workers":

You must explicitly tell the LiDAR to stop using its internal clock and "listen" to the PTP Master. For Innoviz or similar sensors, this is done via a web portal or a **REST API / UDS command**.

**Switching:** If you use a switch, it **must** be a **PTP-Aware Switch** (Transparent Clock or Boundary Clock)

You must explicitly tell the LiDAR to stop using its internal clock and "listen" to the PTP Master. For Innoviz or similar sensors, this is done via a web portal or a **REST API / UDS command**.

**Set Mode:** Change Time Sync Mode from `Internal` or `GPS/PPS` to `PTP` (or `gPTP/802.1AS`).

# Noise, blooming, ghosting

They are **artifacts born from the physics of light,** but they must be solved in your C++ code to prevent the car's perception system from making dangerous mistakes.

---

### 1. Blooming (The "Halo" Effect)

**What it is:** Blooming occurs when the LiDAR hits a **highly reflective target** (like a retro-reflective stop sign, a license plate, or a high-vis vest). The returned energy is so intense that it "oversaturates" the sensor's receiver.

- **The Symptom:** In your point cloud, a small stop sign will look like a giant, glowing 3D blob or a "halo." The points "bleed" outward, making the object appear much larger than it is.
    
- **The Risk:** The car might think a stop sign is actually a large truck or a wall blocking the road.

**Adaptive Gain Control:** High-end sensors (like Innoviz) automatically lower the laser power or receiver sensitivity when they detect a highly reflective object.


### Ghosting (The "Hallucination")

**What it is:** Ghosting is the appearance of objects where nothing physically exists. This is usually caused by **Multi-path Reflections** or **Internal Reflections**.

**Probability Mapping:** Modern SLAM (Simultaneous Localization and Mapping) tracks objects over time. If a cluster of points appears suddenly in the "air" and doesn't follow a logical path, the software labels it a "ghost" and ignores it.

- **The Symptom:** You see a "ghost" car floating in the sky or a second "ghost" wall behind a real one.
**The Cause:** The laser hits a shiny surface (like a glass building or a mirror-finish tanker truck), bounces to a second object, and then returns. The sensor thinks the light traveled in a straight line, so it places the object at the total distance traveled.

## Noise (The "Salt and Pepper")

**The Cause:** * **Ambient Noise:** Sunlight (photons from the sun hitting the sensor at the same frequency as the laser).

- **Atmospheric Noise:** Rain, snow, fog, or dust scattering the laser beam.
    
- **Electronic Noise:** "Dark current" or heat inside the sensor's own electronics.


|**Artifact**|**Primary Cause**|**Best C++ Solution**|
|---|---|---|
|**Blooming**|High Reflectivity|Intensity-based "cropping" & Geometry fitting.|
|**Ghosting**|Multi-path bounces|Temporal tracking & Probability-based pruning.|
|**Noise6**|Sun/Rain/Electronics7|Radius (ROR) or Statistical (SOR) Outlier Removal.8|

