`std::bitset` is a compact, efficient data structure .But it is surprisingly hard to use with modern C++ algorithms and ranges.  
This library provides **zero-overhead views** that make `std::bitset` iterable, composable, and algorithm-friendly.

## Design goals

- **No modification of `std::bitset`**
- **No allocations**
- **Header-only**
- **C++20 ranges-compatible**
- **Const-correct**
- **Minimal abstractions**

---

## Features

### Iterate over all bits (`bits_view`)

```cpp
std::bitset<8> b("10110010");

for (bool bit : bitset_view::bits(b)) {
    std::cout << bit << " ";
}

```

Works seamlessly with standard algorithms:

```cpp
auto ones = std::ranges::count(
    bitset_view::bits(b), true);

```

---

### Mutable iteration (proxy reference)

```cpp
for (auto bit : bitset_view::bits(b)) {
    bit.flip();   // modifies underlying bitset
}

```

This forwards the proxy reference returned by `std::bitset::operator[]`.

---

### Iterate over set bits only (`set_bits_view`)

```
for (std::size_t i : bitset_view::set_bits(b)) {
    std::cout << "bit set at " << i << "\n";
}
```
This enables **sparse iteration**, which is particularly useful for:

- DSP / audio engines
- compiler and systems code

---

## API Overview

```cpp
namespace bitset_view {

template <std::size_t N>
bits_view<N> bits(std::bitset<N>&);

template <std::size_t N>
bits_view<N> bits(const std::bitset<N>&);

template <std::size_t N>
set_bits_view<N> set_bits(const std::bitset<N>&);

}

```

All views:
- are lightweight
- store only a pointer to the underlying `std::bitset`
- do not allocate
- model `std::ranges::forward_range`

---

## Ranges & Concepts

The views integrate naturally with C++20 ranges:

```cpp
static_assert(
    std::ranges::forward_range<
        bitset_view::set_bits_view<128>>);
```

They are usable with:

- `std::ranges::count`
    
- `std::ranges::find`
    
- `std::ranges::for_each`
    
- range pipelines
    

---

## Why views instead of containers?

- avoids copying bits
    
- preserves bitset size at compile time
    
- follows the **ranges philosophy**
    
- allows composition with algorithms
    

This mirrors the design direction of the C++ standard library.

---

## Why not `std::vector<bool>`?

`std::vector<bool>`:

- is not a real container
    
- uses proxy references
    
- has surprising performance and semantic pitfalls
    

This library keeps `std::bitset` as the underlying representation and exposes **safe iteration semantics** on top.

---

## Limitations

- `std::bitset<N>` size must be known at compile time
    
- `set_bits_view` currently uses linear scanning
    
- not a drop-in replacement for dynamic bitsets
    

These limitations are deliberate and align with the design goals.

---

## Future work

- `constexpr` support
    
- optimized set-bit iteration (`find_first / find_next`)
    
- property-based testing
    
- micro-benchmarks
    
- `std::views` adaptor integration
    
- C++23 improvements