
# Object Slicing in C++: The Silent Polymorphism Killer

**Object slicing** happens when a derived object is copied **by value** into a base object.  
The derived part is **cut off**, leaving only the base subobject.

```cpp
ConsoleLogger d;
Logger b = d;   // slicing
```
Result:
- `b` is a `Logger`
- `ConsoleLogger` data and behavior are lost
- Virtual functions now behave like `Logger`

**Rule to remember:**  
👉 _Never pass or store polymorphic objects by value — use references or pointers._

when you pass by value -> copy ctor is invoked and copy happens





**(Optional) Make slicing impossible**

If a base class is meant to be used polymorphically, you can disable copying:

```cpp
struct Logger  
{  
Logger(const Logger&) = delete;  
Logger& operator=(const Logger&) = delete;  
virtual ~Logger() = default;  
};

```

Now slicing fails at compile time instead of runtime.

---

**When object slicing is not a bug**

Slicing is only a problem when you expect runtime polymorphism.

Example where slicing is fine:

```cpp
struct Point { int x, y; };  
struct ColoredPoint : Point { int color; };

Point p = ColoredPoint{1, 2, 3};
```

Here:
- No virtual functions
- No polymorphic intent
- You only care about x and y

If a base class is not designed for polymorphism, slicing is harmless.



## final keyword in inheritance

**prevent further inheritance or overriding**

this has 2 uses:

1. prevent overriding

```cpp
struct Base {
  virtual void f();
};

struct Derived : Base {
  void f() final;   // cannot be overridden further
};

struct MoreDerived : Derived {
  void f(); // ❌ compile-time error
};

```



2. prevent inheritance
```cpp
struct Logger final {
  void log();
};

struct MyLogger : Logger { }; // ❌ compile-time error

```

