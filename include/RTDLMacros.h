#define RTDL_NEWOP(A) (void(*)(int))(0x801cc8bc)(A) // newop is the c++ new operator 
#define RTDL_DELETEOP(A) (void(*)(void*))(0x801cc8c0)(A) //deleteop is the c++ delete operator

#define RTDL_HEROLOCATION(A) (void(*)(void*))(0x804eda9c)(A)
////location_q43scn4step4hero function - takes hero struct as an argument 
#define RTDL_ENEMYMANAGER(A) (void(*)(int*))(0x8023f330)(A)
