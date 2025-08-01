.section .rodata
.global opusfile_start
.global opusfile_end
opusfile_start:
    .incbin "libopusfile.lfi"
opusfile_end:

.section .note.GNU-stack,"",@progbits
