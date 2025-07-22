# cdtors

## Task

Ensure for constructors and destructors of shared libraries:

- they are called
- expected call effects are observable
- launched in the orinal order (TBD)

## Note

- Commonly `.preinit_array`/`.init`/`.init_array` sections are used for shared object initialization; and `.fini`/`.fini_array` are for finalization
- musl ARM/AArch64 supports only execution of `.init_array`/`.fini_array` sections
  - follow `NO_LEGACY_INITFINI` macro in https://gitee.com/openharmony/third_party_musl/commit/1da53dad278f98b7712ac002162afaa8815ba580
- only one of init_array sections gets executed, usually the one of last linked lib. This results in init_X_N constructors being ignored for all other linked libs thus giving some subtest failures. This can only be fixed in musl.
