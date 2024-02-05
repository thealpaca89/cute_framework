[](../header.md ':include')

# cf_draw_box

Category: [draw](/api_reference?id=draw)  
GitHub: [cute_draw.h](https://github.com/RandyGaul/cute_framework/blob/master/include/cute_draw.h)  
---

Draws a quad wireframe.

```cpp
void cf_draw_box(CF_Aabb bb, float thickness, float chubbiness)
```

Parameters | Description
--- | ---
bb | The AABB (Axis-Aligned Bounding Box) to draw a quad over.
thickness | The thickness of each line to draw.
chubbiness | Inflates the shape, similar to corner-rounding. Makes the shape chubbier.

## Remarks

This is an alias for [cf_draw_quad](/draw/cf_draw_quad.md)

## Related Pages

[cf_draw_quad](/draw/cf_draw_quad.md)  
[cf_draw_quad2](/draw/cf_draw_quad2.md)  
[cf_draw_quad_fill](/draw/cf_draw_quad_fill.md)  
[cf_draw_quad_fill2](/draw/cf_draw_quad_fill2.md)  