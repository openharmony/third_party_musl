cd /data/tests/libc-test/src
rm REPORT 
touch REPORT 
for filelist in $(ls)
do
    cd /data/tests/libc-test/src/$filelist
    for file in $(ls)
    do
        if [ "$file" = "runtest" ] || [ "$file" = "libdlopen_dso.so" ] || [ "$file" = "libtls_get_new-dtv_dso.so" ] || [ "$file" = "src" ]; then 
            continue
        else
            /data/tests/libc-test/src/common/runtest -w '' $file >>../REPORT 
            echo $filelist/$file
        fi
    done
    cd ..
done
