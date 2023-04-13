#include <HelperAI.h>
#include <RTDLMacros.h>
#include <enemyCheck.h>
#include <Hook/OperatorNewDelete.h>

uint32_t AITable[4] = {-1, -1, -1, -1};
const char cpuString[] = {0x00, 'C', 0x00, 'P', 0x00, 'U', 0x00, 0x00}; 
//wchar table to substitute in lyt

const float helperDetectDistance = 1.7f;
const float helperDetectEnemy 	 = 1.5f;
const float helperRunDistance 	 = 2.5f;

noheader void helperInputHook() //hooks into 804ee6bc - update__Q43scn4step4hero3HidFv
{
	register uint32_t reg28 asm("r28"); // holds input
	register uint32_t reg29 asm("r29");
	register uint32_t *reg30 asm("r30"); // holds pointer to hid, which holds pointer to current player? 81564328
	register uint32_t reg31 asm("r31");

	//0x0 of player gets used in GKI_getfirst, which dereferences the dereferenced pointer
	//result can be used for heromanager, which adds 200 to the new pointer and dereferences it again
	
	uint32_t *firstPlayer = **(uint32_t***)reg30;
	firstPlayer = *(uint32_t**)((uint32_t)firstPlayer + 200);
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
		int* dataSection = AITable;
		if (numPlayer > 0 && dataSection[numPlayer]  == -1) 
		{
			dataSection[numPlayer] = __nw__FUI(sizeof(helperAI_t));
			helperConstructor((helperAI_t*)dataSection[numPlayer], numPlayer);
		}
		helperAI_t* aiObj = dataSection[numPlayer];
		helperLoop(aiObj, temp);

		reg31 = aiObj->vpad_held;
		reg28 = aiObj->vpad_fp;
		reg29 = aiObj->vpad_sp;
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

	//start of character table is first player
	void* player = (void*)(heroTable[0]); 
	void* charPtr = (void*)(heroTable[self->charID]);
	vec2_t* helperPos = (vec2_t*)RTDL_HEROLOCATION(charPtr);
	vec2_t* leaderPos = (vec2_t*)RTDL_HEROLOCATION(self->target);

	int* enemyManager = *(int**)player; //0x0 of the player object leads to component ptr. this can be passed into enemy manager
	enemyManager = (int*)enemyManager__Q33scn4step9ComponentFv(enemyManager);
	
	vec2_t* targetPos = leaderPos;
	if (self->target == player)
	{	
		self->target = player;
		if (checkEnemyList(self, helperPos, leaderPos, enemyManager) != leaderPos) enemyTarget = 1;
	}
	else
	{
		bool stillHere = false;
		uint32_t *enemyList = (uint32_t*)((uint32_t)enemyManager + 144);
		void* enemy = 0;

		for (int j = 0; j < enemyList[0]; j++)
		{
			enemy = *(void**)(vc_mutableArr_enemy(enemyList, j));
			if (self->target == enemy)
			{
				stillHere = true;
				break;
			}
		}

		//check if enemy still exists and if not relock onto main character
		if (stillHere)
		{
			targetPos = RTDL_ENEMYLOCATION(self->target);
			targetPos = checkEnemyPos(self, player, helperPos, leaderPos, targetPos);
			if (self->target == enemy) enemyTarget = 1;
		}
		else 
		{
			self->target = player;
			targetPos = leaderPos;
		}
	}

	//chase
	float diffX = targetPos->x - helperPos->x;
	float diffY = targetPos->y - helperPos->y;
	
	float absDiffX = AbsF32__Q33hel4math4MathFf(diffX);
	float absDiffY = AbsF32__Q33hel4math4MathFf(diffY);
	if ((absDiffX >= helperDetectDistance) | enemyTarget)
	{
		if (diffX > 0.0f) { held_button = HID_BUTTON_RIGHT; }
		else if (diffX < 0.0f) { held_button = HID_BUTTON_LEFT; }

		if (absDiffX >= helperRunDistance) { spress_button = held_button; fpress_button = held_button };
	}

	if (diffY > helperDetectDistance)
	{
		fpress_button |= HID_BUTTON_2;
		held_button |= HID_BUTTON_2;
	}
	else if ((targetPos->y + helperDetectDistance < helperPos->y) && !enemyTarget) //add ifAir call later
	{
		held_button |= HID_BUTTON_DOWN;
	}

	if (enemyTarget)
	{

		if (absDiffX <= helperDetectEnemy)
		{
			if (absDiffY <= helperDetectEnemy)
			{
				fpress_button |= HID_BUTTON_1;
			}
		}
	}

	self->vpad_held = held_button;
	self->vpad_fp = fpress_button;
	self->vpad_sp = spress_button;
	return;
}
// later iterations of this code will call from a table of function pointers corresponding to the ability id the helper has to further define the behavior of the helper

void helperConstructor(helperAI_t* result, uint32_t heroNumber)
{
	result->charID = heroNumber;
	result->target = 0;
	result->timer = 0;
	result->flags = 0;
	result->vpad_fp = 0;
	result->vpad_sp = 0;
	result->vpad_held = 0;
	return;
}