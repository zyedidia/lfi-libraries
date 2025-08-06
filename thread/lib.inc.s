.section .rodata
.global lib_start
.global lib_end
lib_start:
    .incbin "lib.lfi"
lib_end:

.section .note.GNU-stack,"",@progbits
