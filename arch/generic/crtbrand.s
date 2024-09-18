  .section .note.ohos.ident,"a",%note
  .type abitag, %object
abitag:
  .long 2f-1f                 // int32_t namesz
  .long 3f-2f                 // int32_t descsz
  .long 1                     // should be the "type" according to the elf spec
1:.ascii "OHOS\0"          // char name[]
.balign 8
2:.long 1  // int32_t ohos_api
3:
  .size abitag, .-abitag
