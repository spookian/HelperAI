#include <HelperAI.h>

int* fpointer = -1;
int* spointer = -1;
int* tpointer = -1;
//table of pointers point to 

noheader void helperInputHook() //hooks into 804ee6c0 - update__Q43scn4step4hero3HidFv
{
	register unsigned int *reg28 asm("r28"); // holds input
	register unsigned int *reg30 asm("r30"); // holds pointer to hid, which holds pointer to current player? 81564328
	//0x0 of player gets used in GKI_getfirst, result can be used for heromanager
	
	int* firstPlayer = (**(int**)reg30)[50];
	int* temp = firstPlayer + 41;
	firstPlayer = firstPlayer[40];
	// i got this horrific line of code from following the pointer trail in heroManager_Q33
	//40 is 0xA0 / 4
	//0x9C of heromanager struct is number of characters, 0xA0 of heroManager struct is start of character struct

	if (*reg30 != firstPlayer)
	{
		int numPlayer = 0;
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
		if (dataSection != -1)
		{
			reg30[1] = *(helperAI_t**)dataSection->vpad;
		}
	}
	else
	{
		reg30[1] = reg28; 
	}
	return;
}

noheader void helperLHook()
{
	return;
}

//this will play at the start of 
void helperLoop(helperAI_t* self, void* target) //has to be an entity with a move struct
{
	self->flags = 0;
	self->vpad &= !HID_BUTTON_1;

	register unsigned int *reg27 asm("r27");
	float* targetLoc = NULL;
	void* player = self->charTable; //start of character table is first player
	void* charPtr = ((uint32_t*)self->char + self->charID);
	float* helperLoc = (void(*)(void*))(0x804eda9c)(charPtr);
	//location_q43scn4step4hero function

	int* enemyManager = (void(*)(int*))(0x8023f330)(reg27);
	int* somethingEnemy = (int*)((uint32_t)enemyManager + 144);

	if (target == player)
	{
		targetLoc = (void(*)(void*))(0x804eda9c)(target); //hero location function
		
		//809ed0e8 has enemy manager pointer????
		//r27 has all managers?
		for (int i = 0; i < enemyManager[36]; i++) //loop through enemy table to check which one's the closest
		{
			void* enemy = (void(*)(int*,int))(0x803773C4)(somethingEnemy, i);
			// __vc__Q33hel6common41MutableArray<PQ43scn4step5enemy5Enemy,20>FUl. first arg is the aforementioned pointer and the second arg is the enemy you want to pick

			float* enemyLoc = (void(*)(void*))(0x80375e8c)(enemy);
			//80375e8c - location__Q43scn4step5enemy5EnemyCFv 

			if (ABS(enemyLoc[0] - helperLoc[0]) < ABS(targetLoc[0] - helperLoc[0]))
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
			void* enemy = (void(*)(int*,int))(0x803773C4)(somethingEnemy, j);
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
			targetLoc = (void(*)(void*))(0x804eda9c)(player);
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
		float triAdj = ABS(targetLoc[0] - helperLoc[0]);
		float triOpp = ABS(targetLoc[1] - helperLoc[1]);
		float triPytha = (triAdj * triAdj) + (triOpp * triOpp);
		float triHyp = (float()(float))(0x800fe170)(triPytha); //dubious syntax
		//800fe170 - FrSqrt__Q24nw4r4mathFf

		float length = triHyp / triOpp;
		if (length <= 1.3) self->vpad |= HID_BUTTON_1;
	}
	
}
// later iterations of this code will call from a table of function pointers corresponding to the ability id the helper has to further define the behavior of the helper
 
void _start()
{
	helperLHook();
	helperFollow(null, null); // i haven't figured out how to get the compiler to compile these functions without adding them to my start function
}