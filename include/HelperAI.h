//Esoteric things
#ifndef GOL_HELPER_AI
#define GOL_HELPER_AI
#include <types.h>

#define noheader __attribute__((naked))

//Input things
#define HID_BUTTON_LEFT		0x0001
#define HID_BUTTON_RIGHT	0x0002
#define HID_BUTTON_DOWN		0x0004
#define HID_BUTTON_UP		0x0008
#define HID_BUTTON_2		0x0010
#define HID_BUTTON_1		0x0020

#define HID_BUTTON_B		0x0200

//AI flag things
#define AI_PIGGYBACK		0x0001
#define AI_PLAYER			0x0002 // for reading purposes
#define AI_SINGLESTOP		0x0004 // stop moving for a single frame
#define AI_RUNNING			0x0008 // if ai is already running
#define AI_FLYING			0x0010
#define AI_INAIR			0x0020

typedef struct helperAI_s
{
	uint32_t charID;
	uint32_t ctrlID; // used for piggybacking
	void* target;

	uint16_t timer;
	uint16_t flags;

	uint32_t vpad_fp;
	uint32_t vpad_sp;
	uint32_t vpad_held;
} helperAI_t;

extern void* hitStop__Q43scn4step4hero4HeroFv(void* something);

void* helperInputHook(uint32_t* HIDptr);
void helperConstructor(helperAI_t* result, uint32_t heroNumber);
void helperLoop(helperAI_t* self, uint32_t* heroTable, uint32_t* heroPtr);
helperAI_t* generateAI(uint32_t heroNumber);

extern helperAI_t* AITable[4];
#endif