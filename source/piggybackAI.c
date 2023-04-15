#include <helperAI.h>
#include <piggybackAI.h>

//function that something something hooks into piggyback loop to disconnect ais from each other

void unlinkHook(uint32_t *ptr) //805058f0
{
	uint32_t *hero = **(uint32_t***)ptr;
	uint32_t piggyID = hero[23];	
	AITable[piggyID]->flags |= !(AI_PIGGYBACK);

	*ptr = 0;
	return;
}


//check for 

void piggybackHook(void* piggybacked, void* piggybacker)
{
	bool okPiggy = 0;
	uint32_t* receivingObj = *(uint32_t**)(piggybacked);
	uint32_t* piggyObj = *(uint32_t**)(piggybacker);

	helperAI_t* receivingAI = AITable[receivingObj[23]];
	helperAI_t* piggyAI = AITable[piggyObj[23]];

	if (isMainPlayer__Q43scn4step4hero4HeroCFv(receivingObj))
	{
		if ((!piggyAI->flags) & AI_ACTIVE) okPiggy = 1; 
		// check if ai is deactivated on piggyObj and set okPiggy to true if so
	}
	else if (isMainPlayer__Q43scn4step4hero4HeroCFv(piggyObj))
	{
		okPiggy = 1;
		piggyAI->flags |= AI_PIGGYBACK;
		piggyAI->ctrlID = receivingAI->charID;
	}
	// have to be mutually exclusive or else helpers would be hopping all over each other 

	if (okPiggy)
	{
		setChild__Q43scn4step4hero9PiggybackFRQ43scn4step4hero9Piggyback(piggybacked, piggybacker);
	}
	return;
}