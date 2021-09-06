OUTPUT_FORMAT("elf64-littleaarch64")
OUTPUT_ARCH(aarch64)
PHDRS {
 exec PT_LOAD FLAGS (5);
 rodata PT_LOAD FLAGS (4);
 rwdata PT_LOAD;
 dyn PT_DYNAMIC;
}
SECTIONS {
 .ta_head : {*(.ta_head)} :exec
 .text : {
  __text_start = .;
  *(.text .text.*)
  *(.stub)
  *(.glue_7)
  *(.glue_7t)
  *(.gnu.linkonce.t.*)
  *(.vfp11_veneer)
  PROVIDE(_mcount = __utee_mcount);
  __text_end = .;
 }
 .eh_frame : { *(.eh_frame) } :rodata
 .rodata : {
  *(.gnu.linkonce.r.*)
  *(.rodata .rodata.*)
 }
 .ARM.exidx : { *(.ARM.exidx* .gnu.linkonce.armexidx.*) }
        .ctors : { *(.ctors) }
        .dtors : { *(.dtors) }
        .plt : { *(.plt) }
 .got : { *(.got.plt) *(.got) }
 .rel.text : { *(.rel.text) *(.rel.gnu.linkonce.t*) }
 .rela.text : { *(.rela.text) *(.rela.gnu.linkonce.t*) }
 .rel.data : { *(.rel.data) *(.rel.gnu.linkonce.d*) }
 .rela.data : { *(.rela.data) *(.rela.gnu.linkonce.d*) }
 .rel.rodata : { *(.rel.rodata) *(.rel.gnu.linkonce.r*) }
 .rela.rodata : { *(.rela.rodata) *(.rela.gnu.linkonce.r*) }
 .rel.dyn : { *(.rel.dyn) }
 .rel.got : { *(.rel.got) }
 .rela.got : { *(.rela.got) }
 .rel.ctors : { *(.rel.ctors) }
 .rela.ctors : { *(.rela.ctors) }
 .rel.dtors : { *(.rel.dtors) }
 .rela.dtors : { *(.rela.dtors) }
 .rel.init : { *(.rel.init) }
 .rela.init : { *(.rela.init) }
 .rel.fini : { *(.rel.fini) }
 .rela.fini : { *(.rela.fini) }
 .rel.bss : { *(.rel.bss) }
 .rela.bss : { *(.rela.bss) }
 .rel.plt : { *(.rel.plt) }
 .rela.plt : { *(.rela.plt) }
 .dynamic : { *(.dynamic) } :dyn :rodata
 .dynsym : { *(.dynsym) } :rodata
 .dynstr : { *(.dynstr) }
 .hash : { *(.hash) }
 . = ALIGN(4096);
 .data : { *(.data .data.* .gnu.linkonce.d.*) } :rwdata
 .bss : {
  *(.bss .bss.* .gnu.linkonce.b.* COMMON)
  . = ALIGN(8);
  __gprof_buf_start = .;
  . += DEFINED(_mcount) ?
   ((__text_end - __text_start) * 177) / 100 : 0;
  __gprof_buf_end = .;
 }
 /DISCARD/ : { *(.interp) }
}
