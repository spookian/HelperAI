INCLUDE symbol.ld

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