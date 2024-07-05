[](../header.md ':include')

# cf_text_get_markup_info

Category: [text](/api_reference?id=text)  
GitHub: [cute_draw.h](https://github.com/RandyGaul/cute_framework/blob/master/include/cute_draw.h)  
---

Reports markup information for a text effect.

```cpp
void cf_text_get_markup_info(cf_text_markup_info_fn* fn, const char* text, CF_V2 position, int num_chars_to_draw /*= -1*/);
```

Parameters | Description
--- | ---
fn | The callback to invoke once per text effect.
text | The renderable text.
position | The top-left corner of the text.
num_chars_to_draw | The number of characters to draw `text`. Use -1 to draw the whole string.

## Remarks

The callback `fn` is invoked once per markup within the renderable `text`. If you wish to fetch any of the markup metadata
you may use [cf_text_effect_get_number](/text/cf_text_effect_get_number.md), [cf_text_effect_get_color](/text/cf_text_effect_get_color.md), or [cf_text_effect_get_string](/text/cf_text_effect_get_string.md) by passing in the `fx` pointer to each.

## Related Pages

[CF_TextEffect](/text/cf_texteffect.md)  
[CF_MarkupInfo](/text/cf_markupinfo.md)  
[cf_text_markup_info_fn](/text/cf_text_markup_info_fn.md)  