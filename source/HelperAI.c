#include <HelperAI.h>
#include <RTDLMacros.h>

int* fpointer = -1;
int* spointer = -1;
int* tpointer = -1;
//table of pointers point to 

// all returns will be replaced with branch statements in editing

noheader void helperDelete() // - hooks into 804f7fe8 - removeHeroIn__Q43
{
	register uint32_t *reg28 asm("r28");
	register uint32_t *reg04 asm("r4");
	register uint32_t *reg03 asm("r3");
	
	uint32_t* helperPointer = *((uint32_t**)RTDL_END_MEMORY + (uint32_t)reg28);
	if (helperPointer != -1)
	{
		RTDL_DELETEOP(helperPointer);
	}
	return;
}

noheader void helperCreate() // - hooks into 804ef168
{
	register uint32_t *reg03 asm("r3");
	register uint8_t *reg31 asm("r31");
	register uint32_t *reg04 asm("r4");

	int heroNumber = *(uint32_t*)(reg31 + 0x3C);
	uint32_t* firstPlayer = reg03[40];

	helperAI_t* newBuddy = RTDL_NEWOP(sizeof(helperAI_t));
	newBuddy->charID = heroNumber;
	newBuddy->target = (void*)firstPlayer;
	
	return;
}

noheader void helperInputHook() //hooks into 804ee6c0 - update__Q43scn4step4hero3HidFv
{
	register uint32_t *reg28 asm("r28"); // holds input
	register uint32_t *reg30 asm("r30"); // holds pointer to hid, which holds pointer to current player? 81564328
	//0x0 of player gets used in GKI_getfirst, result can be used for heromanager, which adds 200 to the pointer and dereferences it
	
	int* firstPlayer = (**(int***)reg30)[50];
	int* temp = firstPlayer + 41;
	firstPlayer = firstPlayer[40];
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
		dataSection += numPlayer;
		if (dataSection != -1 && numPlayer != -1)
		{
			reg30[1] = (*(helperAI_t**)dataSection)->vpad;
			return;
			//THIS IS A FUCKING POINTER TO THE INJECTED DATA SECTION BECAUSE I CAN'T EVEN ACCESS IT PROPERLY
			//FUCK I HAD TO ANTICIPATE THE LOCATION OF DATA ANNDD DO ARITHMETIC TO REACH IT
			// I AM GOING TO KILL MY SELF 
			
			// note to self: try and see if i can patch instructions using golem and patch instructions reaching into data section
		}
		else
		{
			reg30[1] = 0;
			return;
		}
	}

	reg30[1] = reg28; 
	return;
}

noheader void helperLoopHook() // hooks into procBegin heromanager - 804f5588
{
	register uint32_t *reg03 asm("r3");	 //contains table to heroes
	register uint32_t *reg28 asm("r28"); //contains hero pointer, used after hook
	register uint32_t *reg29 asm("r29"); //contains counter
	if (reg29 > 0)
	{
		helperAI_t* helperPtr = *((uint32_t**)RTDL_END_MEMORY + (uint32_t)reg29); // pointer arithmetic don't fail me now
		if (helperPtr != -1) helperLoop(helperPtr, helperPtr->target, reg03);
	}
	
	reg03 = reg28; // going back to initial 
	return;
}


//don't edit return
void helperLoop(helperAI_t* self, void* target, uint32_t* heroTable) //has to be an entity with a move struct
{
	self->flags = 0;
	self->vpad &= !HID_BUTTON_1;

	register uint32_t *reg27 asm("r27");
	float* targetLoc = 0;
	void* player = *heroTable; //start of character table is first player
	void* charPtr = ((uint32_t*)heroTable + self->charID);
	float* helperLoc = RTDL_HEROLOCATION(charPtr);
	

	int* enemyManager = RTDL_ENEMYMANAGER(reg27);
	int* somethingEnemy = (int*)((uint32_t)enemyManager + 144);

	if (target == player)
	{
		targetLoc = RTDL_HEROLOCATION(target); //hero location function
		
		//809ed0e8 has enemy manager pointer????
		//r27 has all managers?
		for (int i = 0; i < enemyManager[36]; i++) //loop through enemy table to check which one's the closest
		{
			void* enemy = ((void*(*)(int*,int))0x803773C4)(somethingEnemy, i);
			// __vc__Q33hel6common41MutableArray<PQ43scn4step5enemy5Enemy,20>FUl. first arg is the aforementioned pointer and the second arg is the enemy you want to pick

			float* enemyLoc = ((float*(*)(void*))0x80375e8c)(enemy);
			//80375e8c - location__Q43scn4step5enemy5EnemyCFv 

			if (RTDL_ABS(enemyLoc[0] - helperLoc[0]) < RTDL_ABS(targetLoc[0] - helperLoc[0]))
			{
				self->target = enemy;
				targetLoc = enemyLoc;

				self->flags |= AI_TARGET_ENEMY;
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
			if (target == enemy)
			{
				stillHere = true;
				break;
			}
		}

		//check if enemy still exists and if not relock onto main character
		if (stillHere)
		{
			self->flags |= AI_TARGET_ENEMY;
		}
		else 
		{
			self->target = player;
			targetLoc = ((float*(*)(void*))0x804eda9c)(player);
		}
	}

	//chase
	if (targetLoc[0] - 0.8 > helperLoc[0])
	{
		self->vpad |= HID_BUTTON_RIGHT;
		self->vpad &= !HID_BUTTON_LEFT;
	}
	else if (targetLoc[0] + 0.8 < helperLoc[0])
	{
		self->vpad |= HID_BUTTON_LEFT;
		self->vpad &= !HID_BUTTON_RIGHT;
	}
	else
	{
		self->vpad &= !(HID_BUTTON_LEFT | HID_BUTTON_RIGHT);
	}

	if (targetLoc[1] - 0.8 > helperLoc[1]) self-> vpad |= HID_BUTTON_2;
	//check for states
	if (self->flags & AI_TARGET_ENEMY)
	{
		float triAdj = targetLoc[0] - helperLoc[0];
		float triOpp = targetLoc[1] - helperLoc[1];
		float triPytha = (triAdj * triAdj) + (triOpp * triOpp);
		float triHyp = RTDL_SQRT(triPytha); //dubious syntax
		//800fe170 - FrSqrt__Q24nw4r4mathFf

		if (triHyp <= 1.3) self->vpad |= HID_BUTTON_1;
		//what was i thinking hypotenuse IS length
	}
	return;
}
// later iterations of this code will call from a table of function pointers corresponding to the ability id the helper has to further define the behavior of the helper
 
void _start()
{
	helperDelete();
	helperCreate();
	helperInputHook();
	helperLoopHook();
	helperLoop(0, 0, 0); // i haven't figured out how to get the compiler to compile these functions without adding them to my start function
}