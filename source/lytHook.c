#include <types.h>
#include <helperAI.h>
#include <lytHook.h>

const wchar_t cpuString[] = {'C', 'P', 0x00}; 
//wchar table to substitute in lyt

void PlayerNumHook(wchar_t* toSet, wchar_t* toCopy)
{
	uint32_t AINum = (toCopy[1] - 0x30) - 1; // really hacky but im using the string to grab player number
	wchar_t* replace = toCopy;
	
	if (AITable[AINum] == -1) AITable[AINum] = generateAI(AINum);
	if (!(AITable[AINum]->flags & AI_PLAYER)) 
	{
		*(uint32_t*)toCopy = *(uint32_t*)cpuString;
	}
	
	setText__Q23lyt12PaneAccessorCFPCw(toSet, replace);
	return;
}