
a dependent name is any name that depends on a template parameter

**example:**

- `C::value_type`
- `C::iterator`
- `C::const_iterator`
- `C::mapped_type`

## The ambiguity problem

inside a template, 

C::const_iterator;

here the compiler doeskt know wheter the const_iterator is 
- a type
- or a static data member
c++ parsing rule require the compiler to assume it is **not a type** unless explicitly told

if its a **type**, then use the **typename** keyword

## Example: generic lookup function that insert the value if not found


```cpp
template <typename C>
typename C::mapped_type lookup(C& c, const typename C::key_type& k, const typename C::mapped_type& v)
{
	typename C::const_iterator i = c.find(k);
	
	if(i == c.end())
	{
		c.insert(std::pair<typename C::key_type, typename C::mapped_type>(k,v));
		return v;
	}
	return i->second;
}
```













basics:

int* const iptr; - means iptr is const. and it points to int

const int* iptr; - means iptr is mutable(it can point to something else later) and it points to const int