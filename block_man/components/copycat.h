/*
	Cute Framework
	Copyright (C) 2020 Randy Gaul https://randygaul.net

	This software is provided 'as-is', without any express or implied
	warranty.  In no event will the authors be held liable for any damages
	arising from the use of this software.

	Permission is granted to anyone to use this software for any purpose,
	including commercial applications, and to alter it and redistribute it
	freely, subject to the following restrictions:

	1. The origin of this software must not be misrepresented; you must not
	   claim that you wrote the original software. If you use this software
	   in a product, an acknowledgment in the product documentation would be
	   appreciated but is not required.
	2. Altered source versions must be plainly marked as such, and must not be
	   misrepresented as being the original software.
	3. This notice may not be removed or altered from any source distribution.
*/

#ifndef COPYCAT_H
#define COPYCAT_H

#include <cute.h>
using namespace cute;

#include <components/animator.h>

#include <cute/cute_coroutine.h>

struct CopyCat
{
	bool busy = false;
	bool awake = false;
	float t = 0;
	static constexpr float awake_delay = 5.0f;
	static constexpr float hop_delay = 0.35f;
};

CUTE_INLINE cute::error_t CopyCat_serialize(app_t* app, kv_t* kv, entity_t entity, void* component, void* udata)
{
	CopyCat* copycat = (CopyCat*)component;
	if (kv_get_state(kv) == KV_STATE_READ) {
		CUTE_PLACEMENT_NEW(copycat) CopyCat;
		Animator* animator = (Animator*)app_get_component(app, entity, "Animator");
		animator->sprite.play("sleeping");
	}
	return kv_error_state(kv);
}

#endif // COPYCAT_H