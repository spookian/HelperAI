#include <HelperAI.h>
#include <RTDLMacros.h>

int fpointer = -1;
int spointer = -1;
int tpointer = -1;
//table of pointers point to 

// all returns will be replaced with branch statements in editing

noheader void helperDelete() // - hooks into 804f7fe8 - removeHeroIn__Q43
{
	register uint32_t *reg28 asm("r28");
	register uint32_t *reg04 asm("r4");
	register uint32_t *reg03 asm("r3");
	
	uint32_t* helperPointer = ((uint32_t*)RTDL_END_MEMORY + (uint32_t)reg28);
	if (helperPointer != -1)
	{
		RTDL_DELETEOP(*(void**)helperPointer);
		*helperPointer = -1;
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
		int numPlayer = -1;
		for (int i = 0; i < 3; i++)
		{
			if (*(temp + i) == *reg30) 
			{
				numPlayer = i;
				break;
			}
			
		}
		int* dataSection = RTDL_END_MEMORY;
		dataSection += (numPlayer - 1);
		
		if (numPlayer > 0 && *dataSection == -1) *dataSection = (uint32_t)helperConstructor(numPlayer);	// if helper is not created yet, create one
		helperLoop(*dataSection, temp);

		reg30[1] = (*(helperAI_t**)dataSection)->vpad_held;
		reg30[2] = (*(helperAI_t**)dataSection)->vpad_fp;
		reg30[3] = (*(helperAI_t**)dataSection)->vpad_sp;
		return;
		//THIS IS A FUCKING POINTER TO THE INJECTED DATA SECTION BECAUSE I CAN'T EVEN ACCESS IT PROPERLY
		//FUCK I HAD TO ANTICIPATE THE LOCATION OF DATA ANNDD DO ARITHMETIC TO REACH IT
		// I AM GOING TO KILL MY SELF 
		
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
	
	uint32_t magicword = 0x3fd9999a;	// 1.7f but im bypassing the compiler LMFAO
	uint32_t left_behind = 0x40900000;
	
	
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
				magicword = 0x3f8ccccd;
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
			magicword = 0x3f8ccccd;
		}
		else 
		{
			self->target = player;
			targetLoc = RTDL_HEROLOCATION(player);
		}
	}

	//chase
	if (targetLoc[0] - *(float*)(&magicword) > helperLoc[0])
	{
		held_button = HID_BUTTON_RIGHT;
		if (targetLoc[0] - *(float*)(&left_behind) > helperLoc[0]) spress_button = HID_BUTTON_RIGHT;
	}
	else if (targetLoc[0] + *(float*)(&magicword) < helperLoc[0])
	{
		held_button = HID_BUTTON_LEFT;
		if (targetLoc[0] + *(float*)(&left_behind) < helperLoc[0]) spress_button = HID_BUTTON_LEFT;
	}

	if (targetLoc[1] - *(float*)(&magicword) > helperLoc[1])
	{
		fpress_button |= HID_BUTTON_2;
		held_button |= HID_BUTTON_2;
	}
	else if ((targetLoc[1] + *(float*)(&magicword) < helperLoc[1]) && !enemyTarget) //add ifAir call later
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

		if (triHyp <= *(float*)(&magicword)) fpress_button |= HID_BUTTON_1;
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

void _start()
{
	helperDelete();
	helperInputHook();
	helperLoop(0, 0); // i haven't figured out how to get the compiler to compile these functions without adding them to my start function
	helperConstructor(0);
}