#include <helperAI.h>
#include <piggybackAI.h>

void unlinkHook(void *ptr) //80505930
{
	*ptr = 0;
	
	return;
}

void piggybackHook(void* piggybacked, void* piggybacker)
{
	uint32_t* receivingObj = *(uint32_t**)(piggybacked);
	uint32_t* piggyObj = *(uint32_t**)(piggybacker);

	bool okPiggy = 0;

	if (isMainPlayer__Q43scn4step4hero4HeroCFv(receivingObj))
	{
		okPiggy = 0;
		// check if ai is deactivated on piggyObj and set okPiggy to true if so
	}
	else if (isMainPlayer__Q43scn4step4hero4HeroCFv(piggyObj))
	{
		okPiggy = 1;
		// for now keep it like this until i find a way to check if a piggybacker has multiple entities
		// also check if a piggybacker has a certain player on it so i can collapse the player tower instantly
	}
	// have to be mutually exclusive or else helpers would be hopping all over each other 

	if (okPiggy)
	{
		setChild__Q43scn4step4hero9PiggybackFRQ43scn4step4hero9Piggyback(piggybacked, piggybacker);
	}
	return;
}