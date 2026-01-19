
### Day 1 — Dynamic polymorphism (core)

**Master these fully:**

- What polymorphism means (static vs dynamic type) (done)
- Virtual functions (done)
- vtable / vptr (you already did this well)
- Object slicing (done)
- Virtual destructors (done)
- `override`, `final` (done)

---

### 🟡 Day 2 — Design correctness

**Master these:**

- Interface design in C++
    
- Liskov Substitution Principle (LSP)
    
- Ownership with polymorphism
    
- Multiple inheritance (interfaces only)
    
- When inheritance is wrong
    

**Exercises:**

- Design a bad hierarchy → explain why it violates LSP
    
- Convert it to composition
    
- Design an interface with correct ownership semantics
    

This is where most devs fail — doing it in 1 day puts you ahead.

---

### 🔵 Day 3 — Modern & advanced polymorphism

**Learn (not deeply master yet):**

- Static polymorphism (templates)
    
- CRTP (basic understanding)
    
- Type erasure (`std::function`)
    
- Concepts (idea + simple examples)
    

**Exercises:**

- Replace a virtual interface with a template
    
- Replace inheritance with `std::function`
    
- Write a simple concept
    

Goal: **know when these exist and why they matter**.