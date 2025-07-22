## Legend
***added*** - test for this reloc added.
***skipped*** - non-trivial reloc, test didn't wrote and skipped.
***unknown*** - support's status of this reloc is unknown.
***unknown-skipped*** - unknown test which, skipped.
***todo*** - test will be written.

## Used relocs
### Dynamic relocations
| Status | Reloc
| -	| -
| unknown | R_AARCH64_COPY
| added | R_AARCH64_RELATIVE
| added | R_AARCH64_GLOB_DAT
| added | R_AARCH64_JUMP_SLOT

### ABS relocations
The most of these relocs in this group can't be reproduced for our test scenarios.
| Status | Reloc
| -	| -
| skipped | R_AARCH64_ABS16
| skipped | R_AARCH64_ABS32
| added | R_AARCH64_ABS64
| skipped | R_AARCH64_PREL16
| skipped | R_AARCH64_PREL32
| skipped | R_AARCH64_PREL64
| skipped | R_AARCH64_PLT32
| skipped | R_AARCH64_LDST8_ABS_LO12_NC
| skipped | R_AARCH64_LDST16_ABS_LO12_NC
| skipped | R_AARCH64_LDST32_ABS_LO12_NC
| skipped | R_AARCH64_LDST64_ABS_LO12_NC
| skipped | R_AARCH64_LDST128_ABS_LO12_NC

### PLT relocations
| Status | Reloc
| -	| -
| added | R_AARCH64_JUMP26
| added | R_AARCH64_CALL26
| added | R_AARCH64_CONDBR19
| added | R_AARCH64_TSTBR14

### GOT relocations
| Status | Reloc
| -	| -
| unknown | R_AARCH64_GOT_LD_PREL19
| unknown | R_AARCH64_LD64_GOTOFF_LO15
| added | R_AARCH64_ADR_GOT_PAGE
| added | R_AARCH64_LD64_GOT_LO12_NC
| added | R_AARCH64_LD64_GOTPAGE_LO15
| unknown | R_AARCH64_GOTREL64
| unknown | R_AARCH64_GOTREL32

### Relocations to create unsigned data value or address inline
| Status | Reloc
| -	| -
| skipped | R_AARCH64_MOVW_UABS_G0
| skipped | R_AARCH64_MOVW_UABS_G0_NC
| skipped | R_AARCH64_MOVW_UABS_G1
| skipped | R_AARCH64_MOVW_UABS_G1_NC
| skipped | R_AARCH64_MOVW_UABS_G2
| skipped | R_AARCH64_MOVW_UABS_G2_NC
| skipped | R_AARCH64_MOVW_UABS_G3

### Relocations to create signed data or offset value inline
| Status | Reloc
| -	| -
| skipped | R_AARCH64_MOVW_SABS_G0
| skipped | R_AARCH64_MOVW_SABS_G1
| skipped | R_AARCH64_MOVW_SABS_G2

### Relocations to create PC-relative offset inline
| Status | Reloc
| -	| -
| skipped | R_AARCH64_MOVW_PREL_G0
| skipped | R_AARCH64_MOVW_PREL_G0_NC
| skipped | R_AARCH64_MOVW_PREL_G1
| skipped | R_AARCH64_MOVW_PREL_G1_NC
| skipped | R_AARCH64_MOVW_PREL_G2
| skipped | R_AARCH64_MOVW_PREL_G2_NC
| skipped | R_AARCH64_MOVW_PREL_G3

### Relocations to create GOT-relative offsets inline
| Status | Reloc
| -	| -
| unknown | R_AARCH64_MOVW_GOTOFF_G0
| unknown | R_AARCH64_MOVW_GOTOFF_G0_NC
| unknown | R_AARCH64_MOVW_GOTOFF_G1
| unknown | R_AARCH64_MOVW_GOTOFF_G1_NC
| unknown | R_AARCH64_MOVW_GOTOFF_G2
| unknown | R_AARCH64_MOVW_GOTOFF_G2_NC
| unknown | R_AARCH64_MOVW_GOTOFF_G3

### Relocations to generate 19, 21 and 33 bit PC-relative addresses
| Status | Reloc
| -	| -
| added | R_AARCH64_LD_PREL_LO19
| added | R_AARCH64_ADR_PREL_PG_HI21
| added | R_AARCH64_ADR_PREL_PG_HI21_NC
| added | R_AARCH64_ADD_ABS_LO12_NC
| added | R_AARCH64_ADR_PREL_LO21

### Relocation sections
| Status | Reloc
| -	| -
| skipped | REL
| skipped | RELA
| skipped | RELR

### Null relocations
| Status | Reloc
| -	| -
| added | R_AARCH64_NONE

### TLS descriptor relocations
| Status | Reloc
| -	| -
| added | R_AARCH64_TLSDESC
| added | R_AARCH64_TLSDESC_ADR_PAGE21
| added | R_AARCH64_TLSDESC_LD64_LO12
| added | R_AARCH64_TLSDESC_ADD_LO12
| added | R_AARCH64_TLSDESC_CALL
| unknown | R_AARCH64_TLSDESC_LD_PREL19
| unknown | R_AARCH64_TLSDESC_ADR_PREL21
| unknown | R_AARCH64_TLSDESC_OFF_G1
| unknown | R_AARCH64_TLSDESC_OFF_G0_NC
| unknown | R_AARCH64_TLSDESC_LDR
| unknown | R_AARCH64_TLSDESC_ADD

### TLS dynamic relocations
| Status | Reloc
| -	| -
| unknown | R_AARCH64_TLS_DTPMOD
| unknown | R_AARCH64_TLS_DTPMOD64
| unknown | R_AARCH64_TLS_DTPREL
| unknown | R_AARCH64_TLS_DTPREL64
| unknown | R_AARCH64_TLS_TPREL
| added | R_AARCH64_TLS_TPREL64

### TLS relocations local dyn TLSLD
| Status | Reloc
| -	| -
| unknown | R_AARCH64_TLSLD_ADR_PREL21
| unknown | R_AARCH64_TLSLD_ADR_PAGE21
| unknown | R_AARCH64_TLSLD_ADD_LO12_NC
| unknown | R_AARCH64_TLSLD_MOVW_G1
| unknown | R_AARCH64_TLSLD_MOVW_G0_NC
| unknown | R_AARCH64_TLSLD_LD_PREL19
| unknown | R_AARCH64_TLSLD_MOVW_DTPREL_G2
| unknown | R_AARCH64_TLSLD_MOVW_DTPREL_G1
| unknown | R_AARCH64_TLSLD_MOVW_DTPREL_G1_NC
| unknown | R_AARCH64_TLSLD_MOVW_DTPREL_G0
| unknown | R_AARCH64_TLSLD_MOVW_DTPREL_G0_NC
| unknown | R_AARCH64_TLSLD_ADD_DTPREL_HI12
| unknown | R_AARCH64_TLSLD_ADD_DTPREL_LO12
| unknown | R_AARCH64_TLSLD_ADD_DTPREL_LO12_NC
| unknown | R_AARCH64_TLSLD_LDST8_DTPREL_LO12
| unknown | R_AARCH64_TLSLD_LDST8_DTPREL_LO12_NC
| unknown | R_AARCH64_TLSLD_LDST16_DTPREL_LO12
| unknown | R_AARCH64_TLSLD_LDST16_DTPREL_LO12_NC
| unknown | R_AARCH64_TLSLD_LDST32_DTPREL_LO12
| unknown | R_AARCH64_TLSLD_LDST32_DTPREL_LO12_NC
| unknown | R_AARCH64_TLSLD_LDST64_DTPREL_LO12
| unknown | R_AARCH64_TLSLD_LDST64_DTPREL_LO12_NC
| unknown | R_AARCH64_TLSLD_LDST128_DTPREL_LO12
| unknown | R_AARCH64_TLSLD_LDST128_DTPREL_LO12_NC

### TLS relocations glob dyn TLSGD
| Status | Reloc
| -	| -
| unknown | R_AARCH64_TLSGD_ADR_PREL21
| unknown | R_AARCH64_TLSGD_ADR_PAGE21
| unknown | R_AARCH64_TLSGD_ADD_LO12_NC
| unknown | R_AARCH64_TLSGD_MOVW_G1
| unknown | R_AARCH64_TLSGD_MOVW_G0_NC

### TLS relocations init exec TLSIE
| Status | Reloc
| -	| -
| unknown | R_AARCH64_TLSIE_MOVW_GOTTPREL_G1
| unknown | R_AARCH64_TLSIE_MOVW_GOTTPREL_G0_NC
| added | R_AARCH64_TLSIE_ADR_GOTTPREL_PAGE21
| added | R_AARCH64_TLSIE_LD64_GOTTPREL_LO12_NC
| unknown | R_AARCH64_TLSIE_LD_GOTTPREL_PREL19


## Unused relocs
### TLS relocations local exec TLSLE
| Reloc
| -
| R_AARCH64_TLSLE_ADD_TPREL_HI12
| R_AARCH64_TLSLE_ADD_TPREL_LO12_NC
| R_AARCH64_TLSLE_MOVW_TPREL_G0
| R_AARCH64_TLSLE_MOVW_TPREL_G0_NC
| R_AARCH64_TLSLE_MOVW_TPREL_G1
| R_AARCH64_TLSLE_MOVW_TPREL_G1_NC
| R_AARCH64_TLSLE_MOVW_TPREL_G2
| R_AARCH64_TLSLE_ADD_TPREL_LO12
| R_AARCH64_TLSLE_LDST128_TPREL_LO12
| R_AARCH64_TLSLE_LDST128_TPREL_LO12_NC
| R_AARCH64_TLSLE_LDST8_TPREL_LO12
| R_AARCH64_TLSLE_LDST8_TPREL_LO12_NC
| R_AARCH64_TLSLE_LDST16_TPREL_LO12
| R_AARCH64_TLSLE_LDST16_TPREL_LO12_NC
| R_AARCH64_TLSLE_LDST32_TPREL_LO12
| R_AARCH64_TLSLE_LDST32_TPREL_LO12_NC
| R_AARCH64_TLSLE_LDST64_TPREL_LO12
| R_AARCH64_TLSLE_LDST64_TPREL_LO12_NC

### Arm32 relocations
| Reloc
| -
| R_AARCH64_P32_ABS32
| R_AARCH64_P32_COPY
| R_AARCH64_P32_GLOB_DAT
| R_AARCH64_P32_JUMP_SLOT
| R_AARCH64_P32_RELATIVE
| R_AARCH64_P32_TLS_DTPMOD
| R_AARCH64_P32_TLS_DTPREL
| R_AARCH64_P32_TLS_TPREL
| R_AARCH64_P32_TLSDESC
| R_AARCH64_P32_IRELATIVE
