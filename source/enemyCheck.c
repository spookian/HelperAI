#include <helperAI.h>
#include <enemyCheck.h>
#include <Hook/Math.h>

extern float length__Q33hel4math7Vector2CFv(vec2_t* vec);
const float stayInRange = 8.0f;

void resetTarget(helperAI_t* self)
{
	self->target_dist = 99.f;
	self->target_type = TARGET_PLAYER;
}

bool isEntityStillHere(helperAI_t* self, void* enemy, uint32_t* enemyManager)
{
	uint32_t* componentPtr = *(void**)enemyManager;
	uint32_t* bossManager = bossManager__Q33scn4step9ComponentFv(componentPtr);
	
	switch(self->target_type)
	{
		case TARGET_ENEMY:
		for (int i = 0; i < enemyManager[36]; i++)
		{
			void* chk_enemy = *(void**)(vc_mutableArr_enemy(&enemyManager[36], i));
			if (enemy == chk_enemy)
			{
				return true;
			}
		}
		break;
		
		case TARGET_BOSS:
		for (int j = 0; j < bossManager[57]; j++)
		{
			void* chk_boss = (void*)(vc_mutableArr_boss(&bossManager[57], j));
			if (enemy == chk_boss)
			{
				return true;
			}
		}
		break;
	}
	return false;
}

vec2_t* getTargetPosition(helperAI_t* self)
{
	vec2_t* result_ptr = 0;
	switch(self->target_type)
	{
		default:
		case TARGET_PLAYER:
		result_ptr = location__Q43scn4step4hero4HeroCFv(self->target);
		break;
		
		case TARGET_ENEMY:
		result_ptr = location__Q43scn4step5enemy5EnemyCFv(self->target);
		break;
		
		case TARGET_BOSS:
		result_ptr = location__Q43scn4step4boss4BossFv(self->target);
		break;
	}
	return result_ptr;
}

int isInRadius(vec2_t* center, float radius, vec2_t* target, float* return_val)
{
	vec2_t newVec = {target->x - center->x, target->y - center->y};
	float hyp = length__Q33hel4math7Vector2CFv(&newVec);
	
	if (hyp <= radius) return 1;
	*return_val = hyp;
	return 0;
} // re

vec2_t* checkEnemyList(helperAI_t* self, vec2_t *helperPos, vec2_t *leaderPos, void *enemyManager)
{
	vec2_t* targetPos = 0;
	float length = 0.f;
	float prevLength = 99.f;
	
	if (isInRadius(helperPos, stayInRange, leaderPos, &length) == 0) // magic number
	{
		resetTarget(self);
		return leaderPos;
	}
	
	//
	uint32_t* componentPtr = *(void**)enemyManager;
	uint32_t* bossList = bossManager__Q33scn4step9ComponentFv(componentPtr);
	if (bossList[57] != 0) // bossmanager + 228
	{
		void* bossPtr = 0;
		int idx = 0;
		while (bossPtr == 0)
		{
			bossPtr = *(void**)vc_mutableArr_boss(&bossList[57], idx);
			idx++;
		}
		
		self->target_type = TARGET_BOSS;
		self->target = bossPtr;
		targetPos = location__Q43scn4step4boss4BossFv(bossPtr);
	}
	
	
	// check enemies after
	uint32_t* enemyList = (uint32_t*)((uint32_t)enemyManager + 144);
	for (int i = 0; i < enemyList[0]; i++)
	{
		void *enemy = *(void**)(vc_mutableArr_enemy(enemyList, i));
		vec2_t* enemyPos = location__Q43scn4step5enemy5EnemyCFv(enemy);
		
			// length from enemy
		if (isInRadius(helperPos, 6.f, enemyPos, &length) && (length < prevLength))
		{
				prevLength = length;
				self->target = enemy;
				self->target_type = TARGET_ENEMY;
				targetPos = enemyPos;
		}
	}
	if ((uint32_t)targetPos == 0) targetPos = leaderPos;
	return targetPos;
}

vec2_t* checkEnemyPos(helperAI_t* self, void* leader, vec2_t *helperPos, vec2_t *leaderPos, vec2_t *enemyPos)
{
	float length = 0;
	vec2_t* returnPos = enemyPos;
	/*if (isInRadius(helperPos, stayInRange, leader, &length) == 0)
	{
		self->target = leader;
		returnPos = leaderPos;
		resetTarget(self);
	}*/
	return returnPos;
}