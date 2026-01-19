# Dynamic Dispatch in C++: vtable, vptr, and What _Actually_ Happens

One of the most common questions in modern C++ interviews and code reviews is:

> _“What happens when you call a virtual function?”_

We throw around terms like **dynamic dispatch**, **vtable**, and **vptr**, but many developers only know them at a surface level. Let’s walk through the _exact process_ using a realistic example.

---

## The motivating example

Consider a banking system that logs transfers, but doesn’t want to care _how_ logging is done.


```cpp
struct Logger {
virtual ~Logger() = default;   
virtual void log_transfer() = 0; 
};
```

Two concrete implementations:

```cpp
struct ConsoleLogger : Logger 
{   
void log_transfer() override   
{ std::cout << "[console] x money transferred\n";} 
};  

struct FileLogger : Logger 
{   
void log_transfer() override   {     std::cout << "[file] x money transferred\n";} 
};
```

And a consumer:
```cpp
struct Bank 
{   
void make_transfer()  
{     // banking logic     
logger.log_transfer();   
}  
private:   Logger& logger; };
```
``

Usage:

```cpp
int main() 
{   
ConsoleLogger logger;
Bank bank{logger};
bank.make_transfer(); 
}
```

The key question is:

**How does C++ know which `log_transfer()` to call at runtime?**

---
## Static type vs dynamic type

Inside `Bank`, the logger is stored as:

`Logger& logger;`

That’s the **static type**.

But at runtime, the object is actually:

`ConsoleLogger`

That’s the **dynamic type**.

Dynamic dispatch means:

> The function implementation is chosen based on the _dynamic type_ of the object, not the static type of the reference or pointer.

---

## Enter the vtable

For every class that has at least one `virtual` function, the compiler generates a **vtable** (virtual table).

Conceptually, it looks like this:

```
ConsoleLogger vtable
-------------------
~ConsoleLogger
log_transfer -> ConsoleLogger::log_transfer

```

```
FileLogger vtable
----------------
~FileLogger
log_transfer -> FileLogger::log_transfer

```

A vtable is simply a table of **function pointers**.

---

## Enter the vptr

Every object of a polymorphic class contains a hidden pointer called the **vptr** (virtual pointer).

So in memory, a `ConsoleLogger` object looks roughly like:

`[ vptr ] ---> ConsoleLogger vtable`

This pointer is automatically initialized by the constructor.

- One vptr **per object**
    
- Points to the vtable of the object’s dynamic type
    
- Typically costs one pointer (8 bytes on 64-bit systems)
    

---

## What actually happens on a virtual call

When this line executes:

`logger.log_transfer();`

The compiler translates it conceptually into something like:

`logger.vptr->log_transfer(logger);`

Step by step:

1. Read the `vptr` from the object
    
2. Follow it to the vtable
    
3. Look up the `log_transfer` slot
    
4. Call the function stored there
    

No `if` statements  
No `switch` on type  
No RTTI checks

Just **one pointer dereference and one indirect call**.

That’s dynamic dispatch.

---

## Why `virtual` matters

If `log_transfer()` were not declared `virtual`:

```cpp
struct Logger
{
  void log_transfer(); // not virtual
};

```

Then the call would be resolved at **compile time** based on the static type (`Logger&`).

- No vtable
    
- No vptr
    
- No runtime polymorphism
    

Virtual is the _opt-in switch_ for dynamic dispatch.

---

## Why references or pointers are required

This breaks polymorphism:

`Logger logger = ConsoleLogger{}; // object slicing`

Only the `Logger` part survives. The derived part is sliced away.

Dynamic dispatch works only when you use:

`Logger& Logger* std::unique_ptr<Logger>`

Never by value.

---

## Performance reality

A virtual call costs:

- One extra memory load (vptr)
    
- One indirect call
    
- Usually prevents inlining
    

In practice:

- Negligible for logging, I/O, networking
    
- Avoidable in tight inner loops
    

That’s why virtual functions are best used at **architectural boundaries**, not in hot paths.

---

## Mental model to remember

If you remember nothing else, remember this chain:

> **Object → vptr → vtable → function pointer → derived function**

That’s dynamic dispatch in C++.

---

## Final thought

Dynamic dispatch is not “slow magic.”  
It’s a **simple, deterministic mechanism** built on pointers and tables.

Once you understand that, virtual functions stop being mysterious — and you can use them _intentionally_, not defensively.




