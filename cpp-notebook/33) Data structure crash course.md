
# Big 3

They are the "workhorses" of C++.

| **Container**            | **Type**      | **Why it's a "Top 3"**                                                                                                     |
| ------------------------ | ------------- | -------------------------------------------------------------------------------------------------------------------------- |
| **`std::vector`**        | Dynamic Array | **Default choice.** Fastest for nearly everything because it stores data in a continuous block of memory (Cache Friendly). |
| **`std::unordered_map`** | Hash Table    | Essential for **Key $\to$ Value** lookups (like a dictionary). Constant time search ($O(1)$) on average.                   |
| **`std::unordered_set`** | Hash Set      | Best for **Unique Presence**. Use it to quickly check if an item has been "seen" before.                                   |
### Comparison Cheat Sheet

|**Feature**|**vector**|**unordered_map**|**unordered_set**|
|---|---|---|---|
|**Search by Value**|Slow ($O(n)$)|**Instant ($O(1)$)**|**Instant ($O(1)$)**|
|**Search by Index**|**Instant ($O(1)$)**|N/A|N/A|
|**Adding to End**|Fast ($O(1)$)|Fast ($O(1)$)|Fast ($O(1)$)|
|**Memory Usage**|Very Low|High|High|
|**Ordering**|Keeps input order|Random/Jumbled|Random/Jumbled|


### Summary: The "Fast" Rule of Thumb

If you are stuck during a coding interview or a project, follow this hierarchy:

1. **Do I need to search by a key?** * Yes $\to$ `unordered_map`.
    
2. **Does order matter?**
    
    - Yes $\to$ `std::map` or `std::set`.
        
3. **Do I just need to store a bunch of stuff?**
    
    - Use `std::vector`. **90% of the time, this is the right answer in C++.**

there is also bitset -> which i know now


