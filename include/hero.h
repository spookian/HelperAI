#ifndef HELPER_HERO_H
#define HELPER_HERO_H

#include <types.h>
#include <Hook/Math.h>

extern void CreateDefault__Q33scn4step17ContextHeroIndiviFv(uint32_t* obj);
extern uint32_t* createHeroAndStart__Q43scn4step4hero7ManagerFUlRCQ33hel4math7Vector2RCQ33hel4math7Vector2bRCQ33scn4step17ContextHeroIndiviQ43scn4step4hero10StepInKind(uint32_t* heroManager, uint32_t heroNumber, vec2_t* unk_00, vec2_t* unk_04, uint32_t unk_08, uint32_t* individHero, uint32_t unk_0c);
extern uint32_t* heroManager__Q33scn4step9ComponentFv(uint32_t* component);

extern uint32_t* infoManager__Q33scn4step9ComponentFv(uint32_t* component);
extern uint32_t* enemyCtrl__Q53scn4step4boss14mahoroabattle26CustomFv(uint32_t* infoManager);
extern uint32_t* heroPanel__Q43scn4step4info14InfoGameStatusFUl(uint32_t* infoManager, uint32_t panelNum);
extern void SetupInfo__Q43scn4step4hero7UtilityFRQ43scn4step4info9HeroPanelQ43scn4step4hero4KindQ43scn4step4hero11AbilityKindbbb(uint32_t* infoManager, uint32_t unk_04, uint32_t unk_08, uint32_t unk_0c, uint32_t unk_10, uint32_t unk_14);

void checkMainAndCreateHero(uint32_t *hidPtr);
void setupPanel(uint32_t* componentPtr, uint32_t panelNum, uint32_t charId);
#endif