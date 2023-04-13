#include <helperAI.h>
#include <enemyCheck.h>
#include <Hook/Math.h>

vec2_t* checkEnemyList(helperAI_t* self, vec2_t *helperPos, vec2_t *leaderPos, void *enemyManager)
{
	vec2_t* targetPos = leaderPos;
	uint32_t* enemyList = (uint32_t*)((uint32_t)enemyManager + 144);
	for (int i = 0; i < enemyList[0]; i++)
	{
		void *enemy = *(void**)(vc_mutableArr_enemy(enemyList, i));
		vec2_t* enemyPos = location__Q43scn4step5enemy5EnemyCFv(enemy);

		float absDiffX_enemy = AbsF32__Q33hel4math4MathFf(enemyPos->x - helperPos->x);
		float absDiffX_target = AbsF32__Q33hel4math4MathFf(targetPos->x - helperPos->x);
		float absDiffY_enemy = AbsF32__Q33hel4math4MathFf(enemyPos->y - helperPos->y);
		float absDiffY_target = AbsF32__Q33hel4math4MathFf(targetPos->y - helperPos->y);

		if (absDiffX_enemy < absDiffX_target)
		{
			if (absDiffY_enemy < absDiffY_target)
			{
				self->target = enemy;
				targetPos = enemyPos;
			}
		}
	}
	return targetPos;
}

vec2_t* checkEnemyPos(helperAI_t* self, void* leader, vec2_t *helperPos, vec2_t *leaderPos, vec2_t *enemyPos)
{
	float absDiffY = AbsF32__Q33hel4math4MathFf(enemyPos->y - helperPos->y);
	if (absDiffY > 4.0)
	{
		self->target = leader;
		return leaderPos;
	}
	return enemyPos;
}