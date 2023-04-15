#include <HelperAI.h>
#include <piggybackAI.h>

// all returns will be replaced with branch statements in editing
extern void __start_RTDL();

#define ROCKY_BRANCHHOOK(start,end) (0x48000000 + (((uint32_t)end - (uint32_t)start) & 0x03FFFFFF));

noheader void __GOLEM_HOOK_START()
{
	*(uint32_t*)(0x804ee6bc) = ROCKY_BRANCHHOOK(0x804ee6bc, helperInputHook);

	uint32_t x = (uint32_t)helperInputHook;
	while(*(uint32_t*)x != 0x4e800020)
	{
		x += 4;
	}
	*(uint32_t*)x = ROCKY_BRANCHHOOK(x, hIH_Goto);

	*(uint32_t*)(0x805061bc) = ROCKY_BRANCHHOOK(0x805061bc, piggybackHook) | 1;
	*(uint32_t*)(0x805058f0) = ROCKY_BRANCHHOOK(0x805058f0, unlinkHook);
	return __start_RTDL();
}