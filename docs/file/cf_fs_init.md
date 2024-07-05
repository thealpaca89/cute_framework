[](../header.md ':include')

# cf_fs_init

Category: [file](/api_reference?id=file)  
GitHub: [cute_file_system.h](https://github.com/RandyGaul/cute_framework/blob/master/include/cute_file_system.h)  
---

Initializes the [Virtual File System](https://randygaul.github.io/cute_framework/#/topics/virtual_file_system).

```cpp
CF_Result cf_fs_init(const char* argv0);
```

Parameters | Description
--- | ---
argv0 | The first command-line argument passed into your `main` function.

## Remarks

This function is automatically called by `cf_app_make`; for most use cases you do not
need to call this function. However, sometimes it's convenient to make tools that crawl
over files without the need for a full application window. In this case simply call this
function to enable all the `cf_fs_` functions.

## Related Pages

[cf_fs_destroy](/file/cf_fs_destroy.md)  