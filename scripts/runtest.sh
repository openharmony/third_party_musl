cd /data/tests/libc-test/src
rm REPORT 
touch REPORT 
for filelist in $(ls)
do
    cd /data/tests/libc-test/src/$filelist
    for file in $(ls)
    do
        /data/tests/libc-test/src/common/runtest -w '' $file >>../REPORT 
        echo $filelist/$file
    done
    cd ..
done
