"__nw__FUI" = 0x801cc8bc;
"__dl__FPv" = 0x801cc8c0;
"__start_RTDL" = 0x80006310;
"AbsF32__Q33hel4math4MathFf" = 0x80387a58;
"vc_mutableArr_enemy" = 0x803773C4;
"location__Q43scn4step5enemy5EnemyCFv" = 0x80375e8c;
"location__Q43scn4step4hero4HeroCFv" = 0x804eda9c;
"enemyManager__Q33scn4step9ComponentFv" = 0x8023f330;
"isMainPlayer__Q43scn4step4hero4HeroCFv" = 0x804ebafc;
"setChild__Q43scn4step4hero9PiggybackFRQ43scn4step4hero9Piggyback" = 0x80505858;
"hitStop__Q43scn4step4hero4HeroFv" = 0x804edafc;
"setText__Q23lyt12PaneAccessorCFPCw" = 0x801b9b04;

ENTRY(__GOLEM_HOOK_START)

OUTPUT_FORMAT("elf32-powerpc", "elf32-powerpc",
	      "elf32-powerpc")
OUTPUT_ARCH(powerpc:common)

MEMORY
{
	mem (rwx) : ORIGIN = 0x808D120C, LENGTH = 1M
}

PHDRS
{
	headers PT_LOAD PHDRS ;
	text PT_LOAD ;
}

SECTIONS
{
	.text : 
	{
		KEEP(*(.text .text.*)) 

	} >mem :text
	
	.data : 
	{
		KEEP(*(.rodata .rodata.*))
		KEEP(*(.data .data.*)) 
		KEEP(*(.sdata .sdata.* ))

	} >mem :text

}