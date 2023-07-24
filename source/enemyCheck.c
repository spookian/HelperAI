#include <helperAI.h>
#include <enemyCheck.h>
#include <Hook/Math.h>

extern float length__Q33hel4math7Vector2CFv(vec2_t* vec);

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
	float length = 0.f;
	if (isInRadius(helperPos, 6.5f, leaderPos, &length) == 0) return leaderPos; // magic number
	vec2_t* targetPos = (vec2_t*)0;
	
	uint32_t* enemyList = (uint32_t*)((uint32_t)enemyManager + 144);
	for (int i = 0; i < enemyList[0]; i++)
	{
		
		void *enemy = *(void**)(vc_mutableArr_enemy(enemyList, i));
		vec2_t* enemyPos = location__Q43scn4step5enemy5EnemyCFv(enemy);
		
		if (targetPos == 0) targetPos = enemyPos;
		
			// length from enemy
		if (isInRadius(helperPos, 3.5f, enemyPos, &length) && (length < self->target_dist))
		{
				self->target_dist = length;
				self->target = enemy;
				targetPos = enemyPos;
		}
	}
	if (enemyList[0] == 0) targetPos = leaderPos;
	return targetPos;
}

vec2_t* checkEnemyPos(helperAI_t* self, void* leader, vec2_t *helperPos, vec2_t *leaderPos, vec2_t *enemyPos)
{
	float length = 0;
	vec2_t* returnPos = enemyPos;
	if (isInRadius(helperPos, 5.f, leader, &length) == 0)
	{
		self->target = leader;
		returnPos = leaderPos;
	}
	return returnPos;
}