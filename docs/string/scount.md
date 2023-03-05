# scount | [string](https://github.com/RandyGaul/cute_framework/blob/master/docs/string_readme.md) | [cute_string.h](https://github.com/RandyGaul/cute_framework/blob/master/include/cute_string.h)

Returns the number of characters in the string.

```cpp
#define scount(s) cf_string_count(s)
```

Parameters | Description
--- | ---
s | The string. Must not be `NULL`.

## Code Example

> Demonstrating decrement on [ssize](https://github.com/RandyGaul/cute_framework/blob/master/docs/string/ssize.md).

```cpp
char s = NULL;
spush(s, 'a');
CUTE_ASSERT(ssize(s) == 1);
ssize(s)--;
CUTE_ASSERT(ssize(a) == 0);
sfree(ssize);
```

## Remarks

Both "" and NULL count as empty. Returns a proper l-value, so you can assign or increment it.

## Related Pages

[slen](https://github.com/RandyGaul/cute_framework/blob/master/docs/string/slen.md)  
[ssize](https://github.com/RandyGaul/cute_framework/blob/master/docs/string/ssize.md)  
[sempty](https://github.com/RandyGaul/cute_framework/blob/master/docs/string/sempty.md)  
[scap](https://github.com/RandyGaul/cute_framework/blob/master/docs/string/scap.md)  