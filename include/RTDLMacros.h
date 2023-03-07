#define RTDL_NEWOP(A) (void(*)(int))(0x801cc8bc)(A) // newop is the c++ new operator 
#define RTDL_DELETEOP(A) (void(*)(void*))(0x801cc8c0)(A)
