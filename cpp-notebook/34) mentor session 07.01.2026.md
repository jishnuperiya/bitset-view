

he highly recommended me to watch this video:
[https://lexfridman.com/joel-david-hamkins/?utm_source=rss&utm_medium=rss&utm_campaign=joel-david-hamkins](https://www.codementor.io/redirect-notice?url=https%3A%2F%2Flexfridman.com%2Fjoel-david-hamkins%2F%3Futm_source%3Drss%26utm_medium%3Drss%26utm_campaign%3Djoel-david-hamkins)

at 1 hr mark- important stuff- mathematica set theor, turing computablitily


### use of nodiscard keyword

**RAII** resource acquistion is initialization

RAII object is a type where 
- the ctor acquire a resource
- the dtor releases that resource
the lifetime of the object is lifetime of the resource

destroying such object is not free- it does something importnant

**`[[nodiscard]]` is particularly useful when the returned object has a constructor and destructor with side effects**

Because:
- constructor **does something**
- destructor **undoes something**
- ignoring the object makes both run immediately
- logic breaks silently

unless you assign this objetc into something else, the dtor will also be immediately called after ctor and what is the use of it then? -> so an object whose ctor and dtor has a side effect like acquiring a file handle and then later releasing it, its important to assign this to some ojbect to make meaning -> so nodiscard is particularly useful in that regard


## todo

implement all modifiers in the STL bitset

const iterator

### generator in rapid check

