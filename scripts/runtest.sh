
cd /data/tests/libc-test/src
rm /data/tests/libc-test/REPORT
touch /data/tests/libc-test/REPORT

function FileSuffix() {
    local filename="$1"
    if [ -n "$filename" ]; then
        echo "${filename##*.}"
    fi
}

#Test cases that need to be shielded
ShieldedList=("trace_stresstest" "fatal_message" "tgkill" "exittest01" "exittest02"
"syslog" "vsyslog" "ldso_randomization_manual" "ldso_randomization_test" "dlopen_ext_relro_test"
"runtest")

function ShieldedCases() {
	for filename in ${ShieldedList[*]}
	do
		if [ "$1" = "$filename" ];
		then
			echo "ShieldedCases"
		fi
	done
}

for file in `ls *`
do
	if [ "$(FileSuffix ${file})" = "so" ] \
	|| [ "$(ShieldedCases ${file})" = "ShieldedCases" ] \
	|| [ -d $file ]
	then
		continue
	elif [ "$file" = "tgkill" ]
	then
		./runtest -w '' $file 12345 34567 >> /data/tests/libc-test/REPORT
	else
		echo $subdir/$file >> FileList
		./runtest -w '' $file >> /data/tests/libc-test/REPORT
	fi
done