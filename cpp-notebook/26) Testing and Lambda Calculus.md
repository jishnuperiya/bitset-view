**monomorphic** : a piece of code that works with exactly one concrete type

**mutable iterator vs const iterator** :  difference is about whether the element you get from the iterator can be modified

**mutable iterator** - can modfiy the element it points to
**const iterator** - can only read the element itpoints to 


**Associative containers**: locate elements using a key  
**`std::set<T>`**: keys only, key == value  
**`std::map<K, V>`**: key → mapped value  
**`map::value_type`** = `std::pair<const K, V>`

A `std::map` iterator gives a `pair<const Key, Value>`:  
`.first` is the key (read-only), `.second` is the value (modifiable).

`std::set` is node-based: one heap allocation per element -> heavy computation


# Test Driven Development (TDD)

software develoment practice where you write the tests before you write the code

Write a failing test → write the minimum code to pass it → refactor → repeat.

TDD is a development style where tests define behavior first, and implementation follows.

## Doctest

**doctest** is a c++ unit testing framework (single header, lightweight)

A **TEST_CASE** is basically a function that doctest registers and runs

example:
```cpp
TEST_CASE("addition works") {
    CHECK(1 + 1 == 2);
}

```

```cpp
void test_addition() {
    CHECK(1 + 1 == 2);
}

```

- Each `TEST_CASE` expands to a **generated function**
- doctest stores a pointer to that function
- The test runner calls all registered test functions

Each doctest `TEST_CASE` expands into a test function

#### A note on exceptions:
doctest catches exceptions thrown inside test cases so the test runner doesn’t crash.

Under the hood:
- doctest wraps each test case in `try / catch`
- If an exception escapes:
    - doctest reports it as a test failure
    - continues running other tests
Without this:
- One thrown exception would terminate the entire test program


#### useful :
-ltc
-lts
-tc ="basic*"

we can also debug a test program - **later**
test suits - **later**

## Rapidcheck

property based testing library in c++

instead of handwriting indvidual test cases rapid check generates many test cases automatically and check that a property always holds


## Lambda calculus

why are they called lambda?

- the term comes from alonso church
- he invented lambda calculus in 1930
- lambda calculus is a mathematical system where :
   - functions are values
   - everything is expressed as fucntion application

core idea
	A fucntion is  not a statement - its an expression

that idea is directly influenced:
- functional programming
- modern c++
- lambda expressions


### Lambda expressions

**lambda expression** is the syntax you write in your code, but at runtime, it evaluates to an **unnamed objec** which is **callalble**


When you write a lambda, the compiler secretly generates a class that looks something like this:


```cpp

int offset = 10;

auto add_offset = [offset](int x) 
				   {
				     return x + offset
				   };				   
```

the compiler tranasform it into:

```cpp

class --lambda_unnmaed_type 
{
private:
	int offset; // captured variable becomes a member
public:
	__lambda_unnamed_type(int o) : offset(0){}
	
	int operator()(int x) const{
		return x + offset;
	}
};

```

### Key Properties of the Lambda Object

- **Unique Type:** Every single lambda you write has a unique, anonymous type. Even if two lambdas look identical, the compiler treats them as different types.
    
- **Stateful:** If you capture variables, they are stored as data members inside this unnamed object.
    
- **Temporary by Default:** If you don't assign a lambda to a variable (like when passing it directly to `std::sort`), it exists only as a temporary object for that specific line of code.
    
- **Small & Efficient:** Because it's an object with a defined `operator()`, the compiler can easily **inline** the code, making lambdas just as fast as (and often faster than) traditional function pointers.
### The "Closure" vs. "Lambda" Distinction

- **Lambda Expression:** The actual text you write: `[](int x){ return x * x; }`.
    
- **Closure Type:** The unique, secret class the compiler creates.
    
- **Closure Object:** The actual instance (the "unnamed object") that exists at runtime.

|**Capture**|**Meaning**|
|---|---|
|**`[]`**|Capture nothing.|
|**`[x]`**|Capture `x` by **value** (a copy).|
|**`[&x]`**|Capture `x` by **reference**.|
|**`[=]`**|Capture all used variables by **value**.|
|**`[&]`**|Capture all used variables by **reference**.|
|**`[this]`**|Capture the current class instance pointer.|



example of using a lambda:
```cpp
std::vector<int> nums = { 3 , 5 , 12 , 6 };    
std::sort(nums.begin() , nums.end(), [](int a, int b){ return a > b; });
```

## Advanced Features

### Mutable Lambdas

By default, variables captured by value are **read-only** inside the lambda. If you need to modify them, you must use the `mutable` keyword.

C++

```cpp
int count = 0;
auto incrementer = [count]() mutable {
    return ++count; // Works because of 'mutable'
};
```
here increment is the instance of the secret classs, callable object
### Generic Lambdas (C++14)

You can use `auto` in the parameter list to create a lambda that works with any type, similar to a template.

C++

```cpp
auto sum = [](auto a, auto b) {
    return a + b;
};
```

here sum is the callable object

---
**note**: function object is different from lambda

object that can be called like a function is a functor aka function object

```cpp
struct Adder
{
   int operator()(int a, int b)
   {
	   return a + b;
   }
};

Adder add;
int result = add(2,3); // looks like a function call. but add is a functor
```


