/* TDX Linker Script */

SECTIONS
{
  . = SIZEOF_HEADERS;
  .text : { *(.text) }
  .rodata : { *(.rodata) }
  .dynamic ALIGN(0x1000) : { *(.dynamic) }
  .data ALIGN(0x1000) : { *(.data) }
}
