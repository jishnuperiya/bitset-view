
“The most common issues were timestamp misalignment between LiDAR, camera, and GNSS/INS, especially after restarts or warm-up. We also saw latency drift under load. I usually validated this by correlating sensor timestamps with vehicle time and checking consistency over long sequences.”

### Q: _What is an ADTF filter?_

**Answer:**

> “An ADTF filter is a modular processing component in a dataflow graph. It receives typed input samples, performs computation or transformation, and publishes outputs to downstream filters.”

### _How did you debug filter-graph issues?_

**Answer:**

> “I checked dataflow ordering, pin connections, timestamps, and sample rates. Many issues turned out to be configuration mismatches rather than code bugs.”### Q: _What exactly was automated?_

**Answer:**

> “Repeatable steps like building and packaging ADTF filters, publishing artifacts to Artifactory, running log-processing tools, and generating validation outputs. Hardware testing itself stayed manual.”


### Q: _Why HDF5?_

**Answer:**

> “HDF5 handles large, structured datasets efficiently and allows hierarchical organization. It’s well-suited for sensor data where you need both performance and clear schema.”

### Q: _Why HDF5?_

**Answer:**

> “HDF5 handles large, structured datasets efficiently and allows hierarchical organization. It’s well-suited for sensor data where you need both performance and clear schema.”

### : _What are common mistakes in sensor serialization?_

**Answer:**

> “Mixing coordinate frames, losing timestamp precision, and making schemas too rigid to evolve. I tried to keep schemas explicit but extensible.”


## challenge faced?

binary serializtion

unplausibile data

was because of strucut alignment issues in ddl

### Q: _What KPIs did you compute?_

**Answer:**

> “Examples include positional error, temporal consistency, detection stability, and geometry accuracy relative to ground truth over time.”

say i focussed mostly on object and geometry



### _How do you approach system-level failures?_

**Answer:**

> “I start by narrowing scope — is it data, timing, configuration, or code? Then I reduce the system to the smallest reproducible case before going deeper.”

# 8️⃣ Architecture & ownership

### Q: _What part did you own?_

**Answer:**

> “I owned the validation pipeline up to generating structured HDF5 data and uploading it for downstream analysis. Cloud-side analytics were handled by another team.”

