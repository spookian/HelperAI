//Esoteric things
#define RTDL_END_MEMORY 0x808D120C //end of vanilla memory
#define noheader __attribute__((naked))

//Input things
#define HID_BUTTON_LEFT		0x0001
#define HID_BUTTON_RIGHT	0x0002
#define HID_BUTTON_DOWN		0x0004
#define HID_BUTTON_UP		0x0008
#define HID_BUTTON_2		0x0010
#define HID_BUTTON_1		0x0020

//AI flag things
#define AI_TARGET_ENEMY 	0x0001

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
	void* target;

	uint16_t timer;
	uint16_t flags;
	uint32_t vpad_fp;
	uint32_t vpad_sp;
	uint32_t vpad_held;
} helperAI_t;

noheader void helperInputHook();
noheader void helperDelete();
helperAI_t* helperConstructor(int heroNumber);
void helperLoop(helperAI_t* self, uint32_t* heroTable);