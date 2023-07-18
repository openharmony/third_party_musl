
cd /data/tests/libc-test/src
rm /data/tests/libc-test/REPORT
rm /data/tests/libc-test/FileList.txt
rm /data/tests/libc-test/SkipList.txt
touch /data/tests/libc-test/REPORT
touch /data/tests/libc-test/FileList.txt
touch /data/tests/libc-test/SkipList.txt
echo 'root:This.is.a.test:18997:0:99999:7:::'>/etc/shadow
param set debug.hitrace.tags.enableflags 1

ARCH=arm
ABILIST=$(param get const.product.cpu.abilist)
if [ $ABILIST == "arm64-v8a" ]; then
    ARCH=aarch64
fi

function FileSuffix() {
    local filename="$1"
    if [ -n "$filename" ]; then
        echo "${filename##*.}"
    fi
}

#Test cases that need to be shielded
ShieldedList=("trace_stresstest" "syslog" "vsyslog" "runtest"
#Failed when running by shell, need to be run manually
"tgkill_ext" "exittest02" "stat" "isatty" "ttyname" "a_stack_chk_fail"
"ldso_randomization_manual" "ldso_randomization_test"
"tcgetattr" "tcgetpgrp" "tcgetsid" "tcsendbreak" "tcsetattr" "tcsetpgrp"
#These need run with special condiction
"pthread_atfork-errno-clobber" "flockfile-list" 
#Symbol hidden
"dlns_set_fun_test" "dlns_inherit_test" "dlns_separated_test"
"unittest_ldso_ns_config" "unittest_ldso_dynlink"
#Some math test cases need to skip.
"acoshl" "asinhl" "erfcl" "fenv" "fma" "fmaf" "fmal" "lgammal" "nearbyint" "nearbyintf"
"nearbyintl" "rint" "rintf" "rintl" "sqrt" "sqrtf" "sqrtl" "tgammal"
#TODO-arm32
"malloc-brk-fail" "pthread_cancel" "res_send"
)

#TODO-aarch64
if [ $ARCH == "aarch64" ]; then
  ShieldedList+=("faccessat" "signal" "unittest_hilog_vsnprint" "yn")
fi

for skiped in ${ShieldedList[*]};do
    echo $skiped >> /data/tests/libc-test/SkipList.txt
done

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
	elif [ -x $file ] && [ -s $file ]
	then
		echo $file >> /data/tests/libc-test/FileList.txt
		if [ "$file" = "tgkill" ]
		then
			./runtest -w '' $file 12345 34567 >> /data/tests/libc-test/REPORT
		else
			./runtest -w '' -t 30 $file >> /data/tests/libc-test/REPORT
		fi
	fi
done
