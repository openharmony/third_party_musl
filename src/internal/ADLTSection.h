//===- ADLTSection.h - ADLT Section data types --------------------*- C -*-===//
//
// Copyright (C) 2024 Huawei Device Co., Ltd.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//       http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
//===----------------------------------------------------------------------===//
 
#ifndef LLVM_BINARYFORMAT_ADLTSECTION_H
#define LLVM_BINARYFORMAT_ADLTSECTION_H
 
#ifdef __cplusplus
#include <cstdint>
 
namespace llvm {
namespace adlt {
 
#else // __cplusplus
#include <stdint.h>
#endif // __cplusplus
 
#ifndef _ELF_H
// part of #include <elf.h>
typedef uint16_t Elf64_Half;
typedef uint64_t Elf64_Off;
typedef uint64_t Elf64_Addr;
typedef uint32_t Elf64_Word;
typedef uint64_t Elf64_Xword;
typedef uint8_t Elf64_Byte;
#endif // _ELF_H
 
typedef Elf64_Word adlt_secindex_t; // section index type
typedef Elf64_Off adlt_symindex_t;  // symbol index type
typedef uint32_t adlt_relindex_t;   // reloc index type
typedef Elf64_Half adlt_phindex_t;  // ph index type
 
typedef struct {
  adlt_secindex_t secIndex;
  Elf64_Off offset; // from section start
} adlt_cross_section_ref_t;
 
typedef struct {
  adlt_secindex_t secIndex;
  Elf64_Off offset; // from section start
  Elf64_Xword size; // size in bytes
} adlt_cross_section_array_t;
 
typedef struct {
  Elf64_Off offset; // relative to header.blobStart
  Elf64_Xword size; // size in bytes, make convertions for data type
} adlt_blob_array_t;
 
// plain-C has no strict typedefs, but aliases used to interpred underlying data
typedef adlt_blob_array_t adlt_blob_u8_array_t;      // uint8_t[]
typedef adlt_blob_array_t adlt_blob_u16_array_t;     // uint16_t[]
typedef adlt_blob_array_t adlt_blob_u32_array_t;     // uint32_t[]
typedef adlt_blob_array_t adlt_blob_u64_array_t;     // uint64_t[]
typedef adlt_blob_u32_array_t adlt_blob_rel_array_t; // uint32_t[] (adlt_relindex_t[])
 
typedef struct {
  Elf64_Half major : 6;
  Elf64_Half minor : 6;
  Elf64_Half patch : 4;
} adlt_semver_t;
 
// DT_NEEDED string index with embedded PSOD index if available
typedef struct {
  Elf64_Off hasInternalPSOD : 1; // true if soname
  Elf64_Off PSODindex : 16;      // PSOD in the current ADLT image
  Elf64_Off sonameOffset : 47;   // string start in bound .adlt.strtab
} adlt_dt_needed_index_t;
 
typedef enum {
  ADLT_HASH_TYPE_NONE = 0,
  ADLT_HASH_TYPE_GNU_HASH = 1,
  ADLT_HASH_TYPE_SYSV_HASH = 2,
  ADLT_HASH_TYPE_DEBUG_CONST = 0xfe,
  ADLT_HASH_TYPE_MAX = 0xff,
} adlt_hash_type_enum_t;
 
typedef uint8_t adlt_hash_type_t;
 
// Data chunk descriptor (e.g. for merged sections)
typedef struct {
  Elf64_Off offset;         // data offset within the segment
  Elf64_Xword size;         // data size
  adlt_secindex_t secIndex; // section index
  adlt_phindex_t phIndex;   // segment index
} adlt_chunk_t;
 
// Serializable representation per-shared-object-data in .adlt section
typedef struct {
  Elf64_Off soName;       // offset in .adlt.strtab
  Elf64_Off soFileName;
  Elf64_Xword soNameHash; // algorithm according to
                          // hdr.stringHashType value
  adlt_cross_section_array_t initArray;
  adlt_cross_section_array_t finiArray;
  adlt_blob_array_t dtNeeded; // array of adlt_dt_needed_index_t[] elems
  adlt_cross_section_ref_t sharedLocalSymbolIndex;
  adlt_cross_section_ref_t sharedGlobalSymbolIndex;
  adlt_blob_u16_array_t phIndexes;    // program header indexes, typeof(e_phnum)
  adlt_blob_rel_array_t relaDynIndx;  // .rela.dyn dependent indexes, raw list
  adlt_blob_rel_array_t relaPltIndx;  // .rela.plt dependent indexes, raw list
  adlt_blob_rel_array_t relrDynIndx;  // .relr.dyn dependent indexes, raw list
  adlt_secindex_t ehFrameHdrSecIndex; // .eh_frame_hdr index
  adlt_blob_array_t
      chunkArray; // chunk array (for merged sections); offset and size in blob
} adlt_psod_t;
 
typedef struct {
  adlt_semver_t schemaVersion;     // {major, minor, patch}
  Elf64_Half schemaHeaderSize;     // >= sizeof(adlt_section_header_t) if comp
  Elf64_Half schemaPSODSize;       // >= sizeof(adlt_psod_t) if compatible
  Elf64_Half sharedObjectsNum;     // number of PSOD entries
  adlt_hash_type_t stringHashType; // contains adlt_hash_type_enum_t value
  Elf64_Off blobStart;             // offset of binary blob start relative to .adlt
  Elf64_Xword blobSize;
  Elf64_Xword overallMappedSize;   // bytes, required to map the whole ADLT image
  adlt_blob_u16_array_t phIndexes; // program header indexes, typeof(e_phnum)
  adlt_blob_u8_array_t symIdxToSoIdx;
  adlt_cross_section_ref_t
      sharedEndLocalSymbolIndex;   // .symtab's end (after last) local symbol
                                   // index
  adlt_cross_section_ref_t
      sharedEndGlobalSymbolIndex;  // .symtab's end (after last) global symbol
                                   // index
  Elf64_Half schemaChunkSize;      // >= sizeof(adlt_chunk_t) if compatible
} adlt_section_header_t;
 
static const char adltBlobStartMark[4] = {0xA, 0xD, 0x1, 0x7};
 
static const adlt_semver_t adltSchemaVersion = {1, 5, 0};
 
#ifdef __cplusplus
} // namespace adlt
} // namespace llvm
#endif // __cplusplus
 
#endif // LLVM_BINARYFORMAT_ADLTSECTION_H