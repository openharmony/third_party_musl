# Other FS

## Purpose
Customer claims that moving library and making symlink to new location causes problems sometimes. Below is the customer's observation:
```
- Prabhdeep Singh Soni 2024-07-31 21:27
The errors were like this in hilog.

`08-22 19:41:32.184 33032 33046 E C03F00/MUSL-LDSO:  [invalidDomain]dlopen_impl load library header failed for libace_compatible.z.so`

...
After running the following commands that copy a library to a different location and
symlink the new location to the original location, the phone no longer boots

# cd /system/lib64/platformsdk/
# mv libace_compatible.z.so /data/local/tmp
# ln -s /data/local/tmp/libace_compatible.z.so libace_compatible.z.so
# reboot

I have investigated further and it seems that if you make a symlink in the same
filesystem, then it still works.
It only causes problems when you move the file into a different file system

```


The test is an attempt to mimic that situation

## Special notes

1. That test uses not a qemu binary but a shell script running on host. The qemu binary is launched inside that script.
2. The qemu binary runs twice - before and after moving library.
3. To get dir on other FS, shell script uses RAM disk so it requires sudo privileges.
