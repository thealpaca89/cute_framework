# cf_make_font | [text](https://github.com/RandyGaul/cute_framework/blob/master/docs/text_readme.md) | [cute_draw.h](https://github.com/RandyGaul/cute_framework/blob/master/include/cute_draw.h)

Constructs a font for rendering text.

```cpp
CF_Result cf_make_font(const char* path, const char* font_name);
```

Parameters | Description
--- | ---
path | A virtual path (TODO_LINK) to the font file.
font_name | A unique name for this font. Used by [cf_push_font](https://github.com/RandyGaul/cute_framework/blob/master/docs/text/cf_push_font.md) and friends.

## Return Value

Returns any errors as [CF_Result](https://github.com/RandyGaul/cute_framework/blob/master/docs/utility/cf_result.md).

## Remarks

Memory is only consumed when you draw a certain glyph (text character). Just loading up the font initially is
a low-cost operation. You may load up many fonts with low overhead. Please note that bold, italic, etc. are actually
_different fonts_ and each must be loaded up individually.

## Related Pages

[cf_draw_text](https://github.com/RandyGaul/cute_framework/blob/master/docs/text/cf_draw_text.md)  
[cf_make_font_mem](https://github.com/RandyGaul/cute_framework/blob/master/docs/text/cf_make_font_mem.md)  
[cf_destroy_font](https://github.com/RandyGaul/cute_framework/blob/master/docs/text/cf_destroy_font.md)  
[cf_push_font](https://github.com/RandyGaul/cute_framework/blob/master/docs/text/cf_push_font.md)  
[cf_push_font_size](https://github.com/RandyGaul/cute_framework/blob/master/docs/text/cf_push_font_size.md)  
[cf_push_font_blur](https://github.com/RandyGaul/cute_framework/blob/master/docs/text/cf_push_font_blur.md)  