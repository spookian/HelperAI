#define WPAD_BUTTON_2							0x0001
#define WPAD_BUTTON_1							0x0002
#define WPAD_BUTTON_B							0x0004
#define WPAD_BUTTON_A							0x0008
#define WPAD_BUTTON_MINUS						0x0010
#define WPAD_BUTTON_HOME						0x0080
#define WPAD_BUTTON_LEFT						0x0100
#define WPAD_BUTTON_RIGHT						0x0200
#define WPAD_BUTTON_DOWN						0x0400
#define WPAD_BUTTON_UP							0x0800
#define WPAD_BUTTON_PLUS						0x1000

#define AI_TARGET_ENEMY 0x0001
#define 

#define ABS(A) ((A) & 0x7FFFFFFF)

typedef struct helperAI_s
{
	uint32_t charID;
	voidptr charTable;
	voidptr enemyTable;

	voidptr target;
	uint32_t flags;
	uint32_t vpad;
} helperAI_t;

noheader void helperLHook();
void helperLoop(helperAI_t* self, void* target);