#include <hero.h>
#include <helperAI.h>
#include <enemyCheck.h>
#include <Hook/Math.h>
#include <types.h>

void setFlags(helperAI_t* self, uint32_t* heroPtr)
{
	self->flags &= 0xF;
	
	uint8_t* stateChecker = (uint8_t*)stateChecker__Q43scn4step4hero4HeroFv(heroPtr);
	if (stateChecker[8]) self->flags |= AI_RUNNING;
	
	uint8_t* footState = (uint8_t*)footState__Q43scn4step4hero4HeroFv(heroPtr);
	if (*footState == 0) self->flags |= AI_INAIR; 
	
	uint8_t* waterState = (uint8_t*)water__Q43scn4step4hero4HeroFv(heroPtr);
	if (waterState[4]) self->flags |= AI_INWATER;
		// 801e6a78 - isEnable__Q25pause11ButtonPanelCFv; used by IsInWater__Q43scn4step4hero11MintUtilityFv
	
	uint8_t* passThru = (uint8_t*)mapColl__Q43scn4step4hero4HeroFv(heroPtr);
	if (passThru[0x86]) self->flags |= AI_PASSTHRU;
	return;
}

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

	uint32_t inactiveNum = heroManager[44];
	uint8_t* inactiveHero = (uint8_t*)(*mutableArray_InactiveHero(&heroManager[44], 0));
	uint32_t curNum = ((uint32_t*)inactiveHero)[15];
	uint32_t individHero[20]; // research seems to imply individHero is 64 bytes but im not taking any chances
	
	vec2_t firstV = {0.f, 0.f};
	vec2_t secV = {0.f, 0.f};
	if ((hidPtr[1] & HID_BUTTON_B) && (inactiveNum != 0))
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
		
		// would you believe that the inactivehero mutable array gets reorganized once a hero is created? the latest inactivehero gets deleted once the 'active' byte is set to 1 and the other ones in the array get their indexes shifted up
		((uint32_t*)inactiveHero)[16] = character;
		inactiveHero[0x46] = 1;
		
		if (AITable[curNum] == -1)
		{
			AITable[curNum] = generateAI(curNum);
		}
		else
		{
			helperConstructor(AITable[curNum], curNum);
		}
	}
	
	return;
}