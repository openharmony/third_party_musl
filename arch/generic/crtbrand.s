  .section .note.ohos.ident,"a",%note
  .balign 4
  .type abitag, %object
abitag:
  .long 2f-1f                 // int32_t namesz
  .long 3f-2f                 // int32_t descsz
  .long 1                     // should be the "type" according to the elf spec
1:.ascii "OHOS\0"          // char name[]
.balign 4
2:.long 1  // int32_t ohos_api
3:
  .size abitag, .-abitag
