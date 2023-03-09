#define RTDL_NEWOP 		((void*(*)(int))0x801cc8bc) // newop is the c++ new operator 
#define RTDL_DELETEOP		((void(*)(void*))0x801cc8c0) //deleteop is the c++ delete operator

#define RTDL_HEROLOCATION 	((float*(*)(void*))0x804eda9c)
////location_q43scn4step4hero function - takes hero struct as an argument 
#define RTDL_ENEMYMANAGER 	((void*(*)(int*))0x8023f330)
#define RTDL_SQRT		((float(*)(float))0x800fe170)
#define RTDL_ABS		((float(*)(float))0x80387a58)
#define RTDL_ENEMYLOCATION	((float*(*)(void*))0x80375e8c)