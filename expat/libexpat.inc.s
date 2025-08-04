.section .rodata
.global expat_start
.global expat_end
expat_start:
    .incbin "libexpat.lfi"
expat_end:

.section .note.GNU-stack,"",@progbits
