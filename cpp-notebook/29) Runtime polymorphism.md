
**polymorphic code**: Polymorphism allows code to work with different types through a common interface,
with behavior selected either at compile time or runtime.


**polymorphism:**
- **compile time polymorphism** : behavior is selected at compile time (templates, overloads)
- **runtime polymorphism**: behavior is selected at runtime via dynamic dispatch (virtual functions)

motivating example

```cpp

struct logger
{
  log_transfer();
}
struct Bank
{
  make_transfer();
  logger.log_transfer();
}
```

**seperate concerns** : bank -> banking, logger -> logging

what if you want several loggers?
- remote server logger
- logger to print
- local console logger
what can we do?

consider this approach:

bank holds the pointer to the logger. bank doesnt need to know the implementation details of the logger reference it holds to. it just need to know how to invoke it's methods. the loggers will have the same function prototypes.


**Interface**

An interface in C++ is typically an abstract base class that defines a contract
through virtual functions, usually without owning data.

consumer doesnt need to know about implementation -> just the contract


**modern c++ guidance**
- prefer composition 
- use inheritance only for polymorphism
- dont inherit from stl containers
- dont inherit to just reuse code
Prefer value semantics; introduce inheritance only when runtime variation is required.

**2 kinds of inheritance**

1. Interface inheritance (good)
abstract base classes / pure virtual interfaces

2. implementation inheritance (problematic?)
reuse parent's code
child inherit data + implementation
Implementation inheritance is usually problematic and should be avoided unless the base class is explicitly designed for extension


**implementation iniheritance** is an anti-pattern 
- if base class changes, derived classes break
- derived classes expose :
   - unwanted APIs
   - violate encapsulation
-> composition is better


**object composition**
a class contains another class

has - a : composition
is - a : inheritance


---

```cpp
struct BaseClass{};

struct DerivedClass : BaseClass{};

void are_belong_to_us (BaseClass& base){};


int main()
{
 DerivedClass derived;
 are_belong_to_us (derived); // you can treat derived class reference as if they were of base class reference type
```
```
This works because derived types must be substitutable for base types (LSP)
Liskov Substitution Principle
```


Derived classes inherit base class members, which can unintentionally expose APIs
and break encapsulation. Composition avoids this problem.


**virtual and override keyword** : to allow method to be overridden

---
**virtual dispatch / dynamic dispatch**
Dynamic dispatch selects the implementation based on the dynamic type of the object, not the static type of the pointer or reference.

---
### pure virtual classes

virtual fn() = 0;

the derived classes **must** implement them

**can't instantiate a base class containing any pure virtual method - only inherit**

also called **abstract class**

Virtual function calls add a level of indirection (vtable lookup),
which may inhibit inlining and has a small runtime cost depending on context.


compiler generate a vtable (virtual function table) -> contains function pointers 

consumer of interface doesnt need to know the underlying type.

### pure virtual classes and virtual destructors

**interface inheritance** - inheriting from the pure virtual classes

**pure virtual class - has virtual destructor**


why you need a virtual destructor in base class?

```cpp
struct BaseClass{}; // -> no dtor

struct DerivedClass : BaseClass
{
  DerivedClass() {};
  ~DerivedClass(){};
}

int main()
{
  BaseClass* derived { new DerivedClass{}};
  
  delete derived; //!!!!!
}
```

in the above example, you delete the dervied class -> which calls the BaseClass destructor, not derived class dtor. -> **Leak resources!!**

```
add virtual ~BaseClass(){};
so that derived class dtor is properly invoked.
```


### using interfaces

when we use interfaces, the compiler doesnt know the underlying type right? (underlying type is the implmeneted class - but only known at runtime)
Because the static type is the interface, objects are accessed through
pointers or references. The concrete object is created separately.


so the compiler doesnt know how much memory to allocate at compiler time (if it knows, then better use templates) - thats why you can only deal with the interface with **reference or pointer to the interface**



### constructor injection and property injection

**constructor injection** : you typically use an interface reference. because reference cannot be reseated, they wont change during the lifetime of the object

**property injection** : you use interface pointer. you use a setter method to set the pointer member. allows you to change the object which the member points during its lifetime.


**tip**: use constructor injection when the injected field wont change throughout the lifetime of the object. if you need flexibility - property injection

- References → cannot be null
    
- Pointers → can be null (must be checked)


logger class:

```cpp
struct Logger
{
 virtual ~Logger() = default;
 virtual void log_transfer() =  0;
};

struct ConsoleLogger : Logger
{
 void log_transfer() override
 {
   std::cout<< "[console] x money transferred" << std::endl;
 }
};

struct FileLogger : Logger
{
  void log_transfer() override
  {
    std::cout << "[File] x money transferred" << std::endl;
  }
};

```

logger is a pure virtual class with a viruatl destructor and a single method


**Constructor Injection**

```cpp
struct Bank
{
  void make_transfer()
  {
    // do something
    logger.log_transfer();
  }
private:
logger& logger;
}

int main()
{
  ConsoleLogger logger;
  Bank bank{logger};
  bank.make_transfer();
}
```


you pass logger reference to Bank class's constructor

**object that logger points will not change during the lifetime of the bank** 

you fix logger choice upon bank construction. (references cant be reseated)

**property injection**

```cpp
struct Bank
{
  void set_logger(logger* new_logger)
  { 
    logger = new_logger;
  }
  void make_transfer()
  {
    if(logger) logger->make_transfer();
  }

private:
logger* logger;
}


int main()
{
ConsoleLogger console_logger;
FileLogger file_logger;

Bank bank;
bank.set_logger(console_logger);
bank.make_transfer();

bank.set_logger(file_logger);
bank.make_transfer();

}
```

**the set_logger method enable you to inject a new logger into bank object at any point during the lifecycle of bank object**

### choosing constructor or propery injection

which one to choose? -> based on design requirements

if you need to change the underlying type of object's member -> property injection 

but what is the initial value of logger?

you can provide a constructor also which takes a pointer -> that make it kind of consturor injection + property injection with added flexibility

```cpp
struct Bank
{
  Bank(logger* logger) : logger{logger}{};
  void set_logger(logger* new_logger)
  { 
    logger = new_logger;
  }
  void make_transfer()
  {
    if(logger) logger->make_transfer();
  }

private:
logger* logger;
}
```

note: ostream uses both property and construtor injection


