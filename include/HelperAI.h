//Esoteric things
#ifndef GOL_HELPER_AI
#define GOL_HELPER_AI

#define noheader __attribute__((naked))

//Input things
#define HID_BUTTON_LEFT		0x0001
#define HID_BUTTON_RIGHT	0x0002
#define HID_BUTTON_DOWN		0x0004
#define HID_BUTTON_UP		0x0008
#define HID_BUTTON_2		0x0010
#define HID_BUTTON_1		0x0020

//AI flag things
#define AI_PIGGYBACK		0x0001
#define AI_ACTIVE		0x0002

//Compiler things
#define false 0
#define true 1
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef char bool;

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
void helperLoop(helperAI_t* self, uint32_t* heroTable);

extern helperAI_t* AITable[4];
#endif