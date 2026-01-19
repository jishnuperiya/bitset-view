
```cpp
 note_set(const std::initializer_list<note>& notes) noexcept
: notes_{0}
 {
for (auto n : notes)
{
     insert(n);
}
 }
```

to make this constexpr, the varialbes has to be literals. so notes has to be also constexpr.

but note is defined in header file and cpp file. and constexpr ctors should be in header files only right? how to deal wit this

---

```cpp
 constexpr note_set& remove(const note& n)
 {
  notes_.reset(n.value());
  return *this;
 }
```

in this code i cant do 

` return notes_reset(n.value())`

since notes_reset returns the bitset& (raw bitsaet reference) - so is there any way to return in a single line?

---


i can do 2 ways:

version 1:

```cpp
  [[nodiscard]] constexpr note_set operator|(const note_set& other) const noexcept
  {
	  note_set result;
	  result.notes_ = this->notes_ | other.notes_;
	  return result;
  }
```


version 2:

```cpp
  [[nodiscard]] constexpr note_set operator|(const note_set& other) const noexcept
  {
	  note_set result = *this;
	  result.notes_ |= other.notes_;
	  return result;
  }
```


- version 1: 1 Default Construction + 1 Bitwise OR + 1 Assignment.
    
- version 2 : 1 Copy Construction + 1 Bitwise OR.

---
### constexpr

i cant use constexpr in iteraotor constructo because it works with runtime logic in advance() method

i cant use oeprator* and operator-> const expr because it construct a note object - which doesnt have a constexpr ctor

also post and prefix cant be constexpr because advance()


### member vs non member functions

i currently have this 2 methods in the class:

```cpp
[[nodiscard]] constexpr note_set operator|(const note_set& other) const noexcept
{
  note_set result = *this;
  result.notes_ |= other.notes_;
  return result;
}

[[nodiscard ]] constexpr note_set& operator|=(const note_set& other) noexcept
{
  this->notes_ |= other.notes_;
  return *this;
	 
```

but the oprator| can be non member

