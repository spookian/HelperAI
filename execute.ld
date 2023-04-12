"__nw__FUI" = 0x801cc8bc;
"__dl__FPv" = 0x801cc8c0;
"__start_RTDL" = 0x80006310;
"AbsF32__Q33hel4math4MathFf" = 0x80387a58;

ENTRY(__start_RTDL)

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