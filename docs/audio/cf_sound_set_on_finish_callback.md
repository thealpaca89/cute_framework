[//]: # (This file is automatically generated by Cute Framework's docs parser.)
[//]: # (Do not edit this file by hand!)
[//]: # (See: https://github.com/RandyGaul/cute_framework/blob/master/samples/docs_parser.cpp)
[](../header.md ':include')

# cf_sound_set_on_finish_callback

Category: [audio](/api_reference?id=audio)  
GitHub: [cute_audio.h](https://github.com/RandyGaul/cute_framework/blob/master/include/cute_audio.h)  
---

Sets the callback for notifications of when a sound finishes playing.

```cpp
void cf_sound_set_on_finish_callback(void (*on_finished)(CF_Sound, void*), void* udata, bool single_threaded);
```

Parameters | Description
--- | ---
on_finished | Called whenever a [CF_Sound](/audio/cf_sound.md) finishes playing, including music.
udata | An optional pointer handed back to you within the `on_finished` callback.
single_threaded | Set to true to queue up callbacks and invoke them on the main thread. Otherwise this callback is called from the mixing thread directly.

## Related Pages

[CF_Audio](/audio/cf_audio.md)  
[cf_audio_sample_rate](/audio/cf_audio_sample_rate.md)  
[cf_audio_sample_count](/audio/cf_audio_sample_count.md)  
[cf_audio_channel_count](/audio/cf_audio_channel_count.md)  
