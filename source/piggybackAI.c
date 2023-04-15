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
	uint32_t* receivingObj = *(uint32_t**)(piggybacked);
	uint32_t* piggyObj = *(uint32_t**)(piggybacker);

	helperAI_t* receivingAI = AITable[receivingObj[23]];
	helperAI_t* piggyAI = AITable[piggyObj[23]];

	if ((piggyAI->flags & AI_ACTIVE) == 0)
	{
		receivingAI->flags |= AI_PIGGYBACK;
		receivingAI->ctrlID = piggyAI->charID;

		setChild__Q43scn4step4hero9PiggybackFRQ43scn4step4hero9Piggyback(piggybacked, piggybacker);
	}
	return;
}