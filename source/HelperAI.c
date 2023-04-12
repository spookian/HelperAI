#include <HelperAI.h>
#include <RTDLMacros.h>
//#include <Hook/OperatorNewDelete.h>

int padding = 0xFFFFFFFF;
int fpointer = 0xFFFFFFFF;
int spointer = 0xFFFFFFFF;
int tpointer = 0xFFFFFFFF;

const char cpuString[] = {0x00, 'C', 0x00, 'P', 0x00, 'U', 0x00, 0x00}; 
//widechar string to substitute in lyt

const float helperDetectDistance = 1.7f;
const float helperRunDistance = 4.5f;

// all returns will be replaced with branch statements in editing
extern void __start_RTDL();

noheader void __GOLEM_HOOK_START()
{
	uint32_t hIH_branch = 0x48000000 + ((uint32_t)helperInputHook - 0x804ee6bc);
	*(uint32_t*)(0x804ee6bc) = hIH_branch;
	
	uint32_t hIH_ret = (uint32_t)helperLoop - 4;
	hIH_branch = (0x804ee6c0 - hIH_ret) & 0x03FFFFFF;
	hIH_branch += 0x48000000;
	*(uint32_t*)(hIH_ret) = hIH_branch;
	asm("b __start_RTDL");
	
	return;
}

noheader void helperDelete() // - hooks into 804f7fe8 - removeHeroIn__Q43
{
	register uint32_t *reg28 asm("r28");
	register uint32_t *reg04 asm("r4");
	register uint32_t *reg03 asm("r3");
	
	//uint32_t* helperPointer = ((uint32_t*)RTDL_END_MEMORY + (uint32_t)reg28);
	uint32_t* helperPointer = (int*)&padding[reg28];
	if (helperPointer != -1)
	{
		//__dl__FPv((void*)helperPointer);
		helperPointer = -1;
	}
	return;
}

noheader void helperInputHook() //hooks into 804ee6bc - update__Q43scn4step4hero3HidFv
{
	register uint32_t reg28 asm("r28"); // holds input
	register uint32_t reg29 asm("r29");
	register uint32_t *reg30 asm("r30"); // holds pointer to hid, which holds pointer to current player? 81564328
	register uint32_t reg31 asm("r31");

	//0x0 of player gets used in GKI_getfirst, which dereferences the dereferenced pointer
	//result can be used for heromanager, which adds 200 to the new pointer and dereferences it again
	
	uint32_t *firstPlayer = **(uint32_t***)reg30;
	firstPlayer = *(uint32_t**)((uint8_t*)firstPlayer + 200);
	uint32_t* temp = (uint32_t*)((uint32_t)firstPlayer + 0xA0);
	firstPlayer = *(uint32_t**)((uint8_t*)firstPlayer + 0xA0);
	
	// i got this horrific line of code from following the pointer trail in heroManager_Q33
	//40 is 0xA0 / 4
	//0x9C of heromanager struct is number of characters, 0xA0 of heroManager struct is start of character struct

	if (*reg30 != firstPlayer)
	{
		int numPlayer = 0;
		for (int i = 0; i < 3; i++)
		{
			if (temp[i] == *reg30) 
			{
				numPlayer = i;
				break;
			}
			
		}
		int* dataSection = &padding;
		if (numPlayer > 0 && dataSection[numPlayer]  == -1) dataSection[numPlayer] = (uint32_t)helperConstructor(numPlayer);	// if helper is not created yet, create one
		helperAI_t* aiObj = dataSection[numPlayer];

		helperLoop(aiObj, temp);

		reg30[1] = aiObj->vpad_held;
		reg30[2] = aiObj->vpad_fp;
		reg30[3] = aiObj->vpad_sp;
		return;
		// note to self: try and see if i can patch instructions using golem and patch instructions reaching into data section
	}
	reg30[1] = reg31; //held
	reg30[2] = reg28; //first frame
	reg30[3] = reg29; //switch frame
	return;
} // return to 804ee6c8

//don't edit return
void helperLoop(helperAI_t* self, uint32_t* heroTable) //has to be an entity with a move struct
{
	self->flags = 0;
	
	uint32_t held_button = 0;
	uint32_t fpress_button = 0;
	uint32_t spress_button = 0;
	bool enemyTarget = 0;

	float* targetLoc = 0;
	void* player = *heroTable; //start of character table is first player
	void* charPtr = *((uint32_t*)heroTable + self->charID);
	float* helperLoc = RTDL_HEROLOCATION(charPtr);
	
	int* enemyManager = *(int**)player; //0x0 of the player object leads to whatever gki_getfirst gives you. this can be passed into enemy manager
	enemyManager = (int*)RTDL_ENEMYMANAGER(enemyManager);
	int* somethingEnemy = (int*)((uint32_t)enemyManager + 144);

	if (self->target == player)
	{
		targetLoc = RTDL_HEROLOCATION(self->target); //hero location function
		
		//809ed0e8 has enemy manager pointer????
		//r27 has all managers?
		self->target = player;
		for (int i = 0; i < enemyManager[36]; i++) //loop through enemy table to check which one's the closest
		{
			void* enemy = ((void*(*)(int*,int))0x803773C4)(somethingEnemy, i);
			enemy = *(void**)enemy;
			// __vc__Q33hel6common41MutableArray<PQ43scn4step5enemy5Enemy,20>FUl. first arg is the aforementioned pointer and the second arg is the enemy you want to pick

			float* enemyLoc = RTDL_ENEMYLOCATION(enemy);
			//80375e8c - location__Q43scn4step5enemy5EnemyCFv 

			if (RTDL_ABS(enemyLoc[0] - helperLoc[0]) < RTDL_ABS(targetLoc[0] - helperLoc[0]))
			{
				self->target = enemy;
				targetLoc = enemyLoc;
				enemyTarget = 1;
			}
		}

		// 0x90 of enemy manager gives you a list of every enemy on screen
		// (enemy manager + 144) might be the mutable array
	}
	else
	{
		bool stillHere = false;
		for (int j = 0; j < enemyManager[36]; j++)
		{
			void* enemy = ((void*(*)(int*,int))0x803773C4)(somethingEnemy, j);
			enemy = *(void**)enemy;
			if (self->target == enemy)
			{
				stillHere = true;
				break;
			}
		}

		//check if enemy still exists and if not relock onto main character
		if (stillHere)
		{
			enemyTarget = 1;
			targetLoc = RTDL_ENEMYLOCATION(self->target);
		}
		else 
		{
			self->target = player;
			targetLoc = RTDL_HEROLOCATION(player);
		}
	}

	//chase
	if (targetLoc[0] - helperDetectDistance > helperLoc[0])
	{
		held_button = HID_BUTTON_RIGHT;
		if (targetLoc[0] - helperRunDistance > helperLoc[0]) fpress_button = HID_BUTTON_RIGHT;
	}
	else if (targetLoc[0] + helperDetectDistance < helperLoc[0])
	{
		held_button = HID_BUTTON_LEFT;
		if (targetLoc[0] + helperRunDistance < helperLoc[0]) fpress_button = HID_BUTTON_LEFT;
	}

	if (targetLoc[1] - helperDetectDistance > helperLoc[1])
	{
		fpress_button |= HID_BUTTON_2;
		held_button |= HID_BUTTON_2;
	}
	else if ((targetLoc[1] + helperDetectDistance < helperLoc[1]) && !enemyTarget) //add ifAir call later
	{
		held_button |= HID_BUTTON_DOWN;
	}
	//check for states
	if (enemyTarget)
	{
		float triAdj = targetLoc[0] - helperLoc[0];
		float triOpp = targetLoc[1] - helperLoc[1];
		float triPytha = (triAdj * triAdj) + (triOpp * triOpp);
		float triHyp = RTDL_SQRT(triPytha); //dubious syntax
		//800fe170 - FrSqrt__Q24nw4r4mathFf

		if (triHyp <= helperDetectDistance) fpress_button |= HID_BUTTON_1;
		//what was i thinking hypotenuse IS length
	}

	self->vpad_held = held_button;
	self->vpad_fp = fpress_button;
	self->vpad_sp = spress_button;
	return;
}
// later iterations of this code will call from a table of function pointers corresponding to the ability id the helper has to further define the behavior of the helper

helperAI_t* helperConstructor(int heroNumber)
{
	helperAI_t* result = RTDL_NEWOP(sizeof(helperAI_t));
	result->charID = heroNumber;
	result->target = 0;
	result->timer = 0;
	result->flags = 0;
	result->vpad_fp = 0;
	result->vpad_sp = 0;
	result->vpad_held = 0;
	return result;
}