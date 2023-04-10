"__nw__FUI" = 0x801cc8bc;
"__dl__FPv" = 0x801cc8c0;

SECTIONS
{
	.data 0x808D120C : 
	{
		. = ALIGN(4);
		KEEP(*(.data .data.*)) 
		KEEP(*(.sdata .sdata.* ))
	}
	
	.text 0x808D120C + SIZEOF(.data) : 
	{
		. = ALIGN(4);
		KEEP(*(.text .text.*)) 
	}
}