
### 1) Interface design


what a good interface look like:

```cpp
struct Logger
{
  virtual ~Logger() = default;
  virtual void log(std::string_view msg) = 0;
};
```

### Rules for good interfaces

✅ **Pure virtual functions only**  
✅ **No data members**  
✅ **Minimal responsibility**  
✅ **Virtual destructor**

Think of interfaces as **contracts**, not implementations.

data creates coupling

### 2) Liskov Substitution Principle (LSP)

objects of a superclass (parent) should be replaceable with objects of a subclass (child) without affecting the correctness of the program.

**The Core Concept: "Substitutability"**

In basic inheritance, we often use the **"is-a"** rule (e.g., a Penguin _is a_ Bird). However, LSP teaches us that "is-a" isn't enough. For a subclass to truly inherit from a parent, it must be able to stand in for that parent anywhere in the code without causing errors or unexpected behavior

### LSP rule of thumb

Derived classes:

- Must **accept at least what the base accepts**
- Must **promise at least what the base promises**
- Must **not surprise users**

If you need to say:

> “But in this derived case, it works differently…”

That’s a red flag 🚩

A Classic Violation: The Square-Rectangle Problem : If you pass a `Square` into a function that expects a `Rectangle` and it tries to set the width to 5 and height to 10, the `Square` will likely force both to be 10 to remain a square.

```cpp
struct Rectangle
{
  virtual void set_width(int w);
  virtual void set_height(int h);
};

struct Square : Rectangle
{
  void set_width(int w) override { /* sets both */ }
  void set_height(int h) override { /* sets both */ }
};

```


#### How to Fix LSP Violations

If a subclass cannot truly fulfill the parent's contract, you should:

1. **Refactor the Hierarchy:** Create a more specific interface (e.g., instead of `Bird`, use `FlyingBird` and `NonFlyingBird`).
    
2. **Use Composition over Inheritance:** Instead of having `Square` inherit from `Rectangle`, have them both implement a `Shape` interface or keep them entirely separate.

### 3) Ownership with polymorphism

common ownership models:

reference
```cpp
struct Bank
{
  Logger& logger;
};

```

pointer

```cpp
Logger* logger;

```

smart poitners

```cpp
std::unique_ptr<Logger> logger;
```


**Rule** : Never delete through a base pointer unless the destructor is virtual.


### 4)Multiple Inheritance : fine for interface, bad for implemenation inheritance

**safe multiple inhreitance - interface inheritance**

```cpp
struct Readable
{
  virtual std::string read() = 0;
};

struct Writable
{
  virtual void write(std::string_view) = 0;
};

struct File : Readable, Writable
{
  std::string read() override;
  void write(std::string_view) override;
};

```

**bad multiple inheritance: implemenation inheritance**

```cpp
struct A { int x; };
struct B { int x; };

struct C : A, B {}; // ❌ ambiguity

```


**diamond problem**
```cpp
      A
     / \
    B   C
     \ /
      D

```

```cpp
struct A {
  int x;
};

struct B : A {};
struct C : A {};

struct D : B, C {}; // 🚨 diamond

```

```cpp
D d;
d.x; // ❌ Which A::x? From B or from C?
```

The compiler can’t decide.


**rule**: Multiple inheritance is fine for interfaces, not implementations.

### 5) When inheritance is wrong

Inheritance is **overused**.

Ask this question:

> “Is this an _is-a_ relationship, or just code reuse?”

**wrong use**:
```cpp
struct Engine
{
  void start();
};

struct Car : Engine {}; // ❌ car is not an engine

```

it should be composition:

```cpp
struct Car
{
  Engine engine; // composition
};


```

### Prefer composition

> **Composition says “has-a”**  
> **Inheritance says “is-a”**


### where is inheriatnce a good idea in harmony lib

1.  output/formatting
```cpp
struct Formatter
{
  virtual ~Formatter() = default;
  virtual std::string format(const NoteSet&) const = 0;
};

```

Implementations:
- `JazzSymbolFormatter`
- `RomanNumeralFormatter`
- `ScientificPitchFormatter`

2. Algorithm/strategies
```cpp
struct ChordAnalyzer
{
  virtual ~ChordAnalyzer() = default;
  virtual AnalysisResult analyze(const NoteSet&) const = 0;
};

```

Implementations:

- Jazz harmony analyzer
- Classical harmony analyzer
- Modal analyzer

3. I/O adapter
```cpp
struct MidiSink
{
  virtual ~MidiSink() = default;
  virtual void send(const NoteSet&) = 0;
};

```

Implementations:

- ALSA
- CoreMIDI
- MIDI file writer

### What to use instead of inheritance for musical structure

**Composition + value semantics**

`struct Chord {   NoteSet notes;   Root root;   Quality quality; };`

**Enums, flags, strong types**

`enum class Quality { Major, Minor, Diminished, Augmented };`

**Templates / constexpr for theory rules**

`template<ScaleType S> 
constexpr IntervalSet intervals();`

This matches how **music theory actually works**.


**critical design rule:**

```
If it represents a musical object → value type
If it represents behavior → interface (inheritance allowed)
```



chatgpt said:

**The modern mental model (very important)**

Think like this:
- **Inheritance = polymorphic boundary**
- **Composition = structure**
- **Values = data**
- **Interfaces = behavior**



to learn :

**extension points**
strategy pattern
bridge pattern


**strategy pattern**

previous bank and logger example is strategy pattern
Behavior varies independently
Easy to add new loggers
Follows “composition over inheritance”


**What kind of Strategy this is**
- **Runtime strategy**
- **Injected via constructor**
- **Non-owning** (`Logger&`)


this is the Strategy Pattern: `Bank` is the context, `Logger` is the strategy interface, and `ConsoleLogger` / `FileLogger` are concrete strategies.”



design pattern is: - A **named solution** to a **recurring design problem**

problem your code solve: I want to change logging behavior without modifying the `Bank` class
this problem appears all the time. the apttern name for that solution - strategy pattern



**bridge pattern**

Bridge is used when you have **two dimensions of variation** that must evolve independently.

```cpp
struct Bank {
  virtual void make_transfer() = 0;
protected:
  Logger& logger;
};

struct RetailBank : Bank {};
struct CorporateBank : Bank {};

```

Now you have:
- Bank abstraction hierarchy
- Logger implementation hierarchy
THAT would be Bridge.

