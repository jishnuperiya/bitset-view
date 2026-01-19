**ogx** is **not an existing library** I’m pointing you to.  
It’s a **project idea / working name** for a **learning + showcase C++ library** you could build.

Think of **logx** as:

> **A small, modern C++ logging framework whose _main goal_ is to demonstrate the correct use of**
> 
> - runtime polymorphism (interfaces)
>     
> - templates
>     
> - concepts
>     
> - and the boundary between them
>     

Logging is just the _vehicle_ — the real subject is **C++ design**.



## The one-sentence definition

> **logx is a hybrid logging framework where the logging _interface_ is runtime-polymorphic, but the logging _implementation_ (sinks, formatters, policies) is compile-time checked using templates and concepts.**


## Why logging?

Because logging naturally needs **both** forms of polymorphism:

### Runtime polymorphism (interface)

- You want to swap loggers at runtime
    
- You want ABI-stable boundaries
    
- You want plugin-style usage


```cpp
std::unique_ptr<ILogger> logger = make_logger_from_config();
```


### Compile-time polymorphism (templates)

- Sinks should be fast
    
- Formatters should be type-safe
    
- Policies should be customizable
    
- Errors should be caught at compile time

```cpp
Logger<ConsoleSink, JsonFormatter, SyncPolicy>

```
This makes logging an **ideal teaching example**.

## Core idea in one diagram

```
        Runtime boundary (virtual)
┌──────────────────────────────────┐
│            ILogger               │
│   virtual void log(LogMsg&) = 0   │
└───────────────▲──────────────────┘
                │
        Bridge pattern
                │
┌──────────────────────────────────┐
│    Logger<Sink, Formatter>        │  ← templates + concepts
│  - Sink must satisfy LogSink     │
│  - Formatter must satisfy Format │
└──────────────────────────────────┘

```


This is the **Bridge pattern** done _properly_ in modern C++.

---

## Minimal mental model

### 1️⃣ The interface (runtime)

```cpp
struct ILogger {
    virtual ~ILogger() = default;
    virtual void log(LogMsg const&) = 0;
};
```

This is what application code depends on.


2️⃣ The implementation (compile-time)

```cpp
template <LogSink Sink, Formatter Fmt>
class Logger final : public ILogger {
    Sink sink_;
    Fmt  fmt_;
};
```

This is where performance, policies, and flexibility live.



it’s useful as a lightweight, dependency-free logging abstraction at library boundaries. It’s not a replacement for production loggers like spdlog, but a way to _decouple_ logging from infrastructure while keeping performance.”


