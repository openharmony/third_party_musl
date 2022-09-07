
cd /data/tests/libc-test
rm src/REPORT
touch src/REPORT

function FileSuffix() {
    local filename="$1"
    if [ -n "$filename" ]; then
        echo "${filename##*.}"
    fi
}

#Test cases that need to be shielded
ShieldedList=("trace_stresstest" "fatal_message" "tgkill" "exittest01" "exittest02"
"syslog" "vsyslog" "ldso_randomization_manual" "ldso_randomization_test" "dlopen_ext_relro_test")

function ShieldedCases() {
	for filename in ${ShieldedList[*]}
	do
		if [ "$1" = "$filename" ];
		then
			echo "ShieldedCases"
		fi
	done
}

for dir in src/*
do
	if [ -d $dir ]; then
		if [ "$dir" = "src/functionalext" ]
		then
			# continue
			for subdir in $dir/*
			do
				if [ "$subdir" = "src/functionalext/supplement" ]
				then
					for supplementsubdir in $subdir/*
					do
						for subfile in `ls $supplementsubdir`
						do
							if [ "$(FileSuffix ${subfile})" = "so" ] \
							|| [ "$(ShieldedCases ${subfile})" = "ShieldedCases" ] \
							|| [ -d ./$supplementsubdir/$subfile ]
							then
								continue
							else
								src/common/runtest -w '' ./$supplementsubdir/$subfile >> src/REPORT
							fi
						done
					done
				else
					for file in `ls $subdir`
					do
						if [ "$(FileSuffix ${file})" = "so" ] \
						|| [ "$(ShieldedCases ${file})" = "ShieldedCases" ] \
						|| [ -d ./$subdir/$file ]
						then
							continue
						else
							# echo file=$subdir/$file
							src/common/runtest -w '' ./$subdir/$file >> src/REPORT
						fi
					done
				fi
			done
		else
			# continue
			for file in `ls $dir`
			do
				if [ "$file" = "runtest" ] \
				|| [ "$(FileSuffix ${file})" = "so" ] \
				|| [ -d $file ]
				then
					continue
				elif [ "$file" = "tgkill" ]
				then
				    src/common/runtest -w '' $dir/$file 12345 34567 >> src/REPORT
				else
					# echo file=$dir/$file
					src/common/runtest -w '' $dir/$file >> src/REPORT
				fi
			done
		fi
	fi
done