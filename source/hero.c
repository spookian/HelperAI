#include <hero.h>
#include <helperAI.h>
#include <enemyCheck.h>
#include <Hook/Math.h>
#include <types.h>

void checkMainAndCreateHero(uint32_t *hidPtr) //function assumes player 1
{
	uint32_t* heroPtr = (uint32_t*)*hidPtr;
	uint32_t* heroManager = heroManager__Q33scn4step9ComponentFv(*(uint32_t**)heroPtr);
	uint32_t curNum = heroManager[39];
	uint32_t individHero[20]; // research seems to imply individHero is 64 bytes but im not taking any chances
	if ((hidPtr[1] & HID_BUTTON_B) && (curNum > 4))
	{
		vec2_t* location = location__Q43scn4step4hero4HeroCFv(heroPtr);
		CreateDefault__Q33scn4step17ContextHeroIndiviFv(individHero);
		individHero[1] = 0x03 + curNum;
		createHero__Q43scn4step4hero7ManagerFUlRCQ33hel4math7Vector2RCQ33hel4math7Vector2bRCQ33scn4step17ContextHeroIndiviQ43scn4step4hero10StepInKind(heroManager, curNum, location, location, 1, individHero, 4);
		// that last argument is very strange, it's only set to 3 for the main player and 4 for any other players who join in
	}
	
	return;
}