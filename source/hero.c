#include <hero.h>
#include <helperAI.h>
#include <enemyCheck.h>
#include <Hook/Math.h>
#include <types.h>

void setupPanel(uint32_t* componentPtr, uint32_t panelNum, uint32_t charId)
{
	uint32_t *infoManager = infoManager__Q33scn4step9ComponentFv(componentPtr);
	infoManager = enemyCtrl__Q53scn4step4boss14mahoroabattle26CustomFv(infoManager);
	infoManager = heroPanel__Q43scn4step4info14InfoGameStatusFUl(infoManager, panelNum);
	
	uint32_t abilityId = 0;
	switch (charId)
	{
		default:
		case 4:
		abilityId = 32;
		break;
		
		case 5:
		abilityId = 33;
		break;
		
		case 6:
		abilityId = 34;
		break;
	}
	
	SetupInfo__Q43scn4step4hero7UtilityFRQ43scn4step4info9HeroPanelQ43scn4step4hero4KindQ43scn4step4hero11AbilityKindbbb(infoManager, charId, abilityId, 0, 0, 0);
	//last one is IsExtraMode so keep an eye out later
}

void checkMainAndCreateHero(uint32_t *hidPtr) //function assumes player 1
{
	uint32_t *heroPtr = (uint32_t*)*hidPtr;
	uint32_t *heroManager = heroManager__Q33scn4step9ComponentFv(*(uint32_t**)heroPtr);
	uint32_t curNum = heroManager[39];
	uint32_t individHero[20]; // research seems to imply individHero is 64 bytes but im not taking any chances
	
	vec2_t firstV = {0.f, 0.f};
	vec2_t secV = {0.f, 0.f};
	if ((hidPtr[1] & HID_BUTTON_B) && (curNum < 4))
	{
		uint32_t character = 0x03 + curNum;
		setupPanel((uint32_t*)*heroPtr, curNum, character);
		
		vec2_t* location = location__Q43scn4step4hero4HeroCFv(heroPtr);
		
		firstV.x = location->x;
		firstV.y = location->y;
		
		secV.x = location->x;
		secV.y = location->y;
		
		CreateDefault__Q33scn4step17ContextHeroIndiviFv(individHero);
		individHero[1] = character;
		createHeroAndStart__Q43scn4step4hero7ManagerFUlRCQ33hel4math7Vector2RCQ33hel4math7Vector2bRCQ33scn4step17ContextHeroIndiviQ43scn4step4hero10StepInKind(heroManager, curNum, &firstV, &secV, 1, individHero, 4);
		// that last argument is very strange, it's only set to 3 for the main player and 4 for any other players who join in
	}
	
	return;
}