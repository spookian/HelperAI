#include <HelperAI.h>
#include <types.h>
#include <enemyCheck.h>
#include <hero.h>
#include <Hook/OperatorNewDelete.h>

helperAI_t* AITable[4] = {-1, -1, -1, -1};

const float helperDetectDistance = 1.7f;
const float helperDetectEnemy 	 = 1.5f;
const float helperRunDistance 	 = 2.5f;

void* helperInputHook(uint32_t *HIDptr) // retool for 0x804ee6e4, 
{
	uint32_t* heroPtr = *(uint32_t**)HIDptr;
	uint32_t*** componentPtr = (uint32_t***)(*heroPtr);
	uint32_t* heroTable = &((componentPtr[50])[40]);

	int numPlayer = heroPtr[23]; // 0x5C of hero obj is hero number
	if (AITable[numPlayer]  == -1) 
	{
		AITable[numPlayer] = generateAI(numPlayer);
	}
	
	helperAI_t* aiObj = AITable[numPlayer];

	if (!(aiObj->flags & AI_PLAYER))
	{
		if (aiObj->flags & AI_PIGGYBACK)
		{
			helperAI_t* ctrlObj = AITable[aiObj->ctrlID]; // uh depending on the order you might get a frame or three of input lag oopsie my bad
			aiObj->vpad_held = ctrlObj->vpad_held;
			aiObj->vpad_fp = ctrlObj->vpad_fp;
			aiObj->vpad_sp = ctrlObj->vpad_sp;
		}
		else { helperLoop(aiObj, heroTable); }
		
		HIDptr[1] = aiObj->vpad_held; //held
		HIDptr[2] = aiObj->vpad_fp; //first frame
		HIDptr[3] = aiObj->vpad_sp; //switch frame
	}
	else
	{
		aiObj->vpad_held = HIDptr[1];
		aiObj->vpad_fp = HIDptr[2];
		aiObj->vpad_sp = HIDptr[3];
	}
	// Reason for this refactor was so Player 1's inputs could get recorded without any nonsense happening
	if (heroPtr[23] == 0)
	{
		checkMainAndCreateHero(HIDptr);
	}

	return hitStop__Q43scn4step4hero4HeroFv(heroPtr);
}

//don't edit return
void helperLoop(helperAI_t* self, uint32_t* heroTable) //has to be an entity with a move struct
{
	uint32_t held_button = 0;
	uint32_t fpress_button = 0;
	uint32_t spress_button = 0;
	bool enemyTarget = 0;

	//start of character table is first player
	void* player = (void*)(heroTable[0]); 
	void* charPtr = (void*)(heroTable[self->charID]);
	vec2_t* helperPos = location__Q43scn4step4hero4HeroCFv(charPtr);
	vec2_t* leaderPos = location__Q43scn4step4hero4HeroCFv(player);

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
			targetPos = location__Q43scn4step5enemy5EnemyCFv(self->target);
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

		if (absDiffX >= helperRunDistance) { spress_button = held_button; fpress_button = held_button; };
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
	result->ctrlID = 0;
	result->charID = heroNumber;
	result->target = 0;
	result->timer = 0;
	result->flags = 0;
	result->vpad_fp = 0;
	result->vpad_sp = 0;
	result->vpad_held = 0;
	
	if (heroNumber == 0) result->flags = AI_PLAYER;
	return;
}

helperAI_t* generateAI(uint32_t heroNumber)
{
	helperAI_t* ret = (helperAI_t*)__nw__FUI(sizeof(helperAI_t));
	helperConstructor(ret, heroNumber);
	return ret;
}