
```cpp
constexpr void erase(const note& n) noexcept
{
 notes_.reset(n.value());
}
```

noexcept because:
The `std::bitset::reset(pos)` method **only** throws an exception (`std::out_of_range`) if the position provided is greater than or equal to the bitset size.In your specific domain, a `harmony::note` is mathematically constrained to a range of 0–11.