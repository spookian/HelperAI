#include <Hook/Math.h>

extern void* vc_mutableArr_boss(void *bossList, int idx);
extern void* vc_mutableArr_enemy(void *enemyList, int idx);

extern vec2_t* location__Q43scn4step4boss4BossFv(void* boss);
extern vec2_t* location__Q43scn4step5enemy5EnemyCFv(void* enemy);
extern vec2_t* location__Q43scn4step4hero4HeroCFv(void* hero);

extern void* bossManager__Q33scn4step9ComponentFv(void* componentPtr);
extern void* enemyManager__Q33scn4step9ComponentFv(void* componentPtr);

vec2_t* checkEnemyList(helperAI_t* self, vec2_t *helperPos, vec2_t *leaderPos, void *enemyManager);
vec2_t* checkEnemyPos(helperAI_t* self, void* leader, vec2_t *helperPos, vec2_t *leaderPos, vec2_t *enemyPos);
vec2_t* getTargetPosition(helperAI_t* self);
void resetTarget(helperAI_t* self);
bool isEntityStillHere(helperAI_t* self, void* enemy, uint32_t* enemyManager);
