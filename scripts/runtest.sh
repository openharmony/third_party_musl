
cd /data/tests/libc-test
rm src/REPORT 
touch src/REPORT

function FileSuffix() {
    local filename="$1"
    if [ -n "$filename" ]; then
        echo "${filename##*.}"
    fi
}

for dir in src/*
do
	if [ -d $dir ]; then
		if [ "$dir" = "src/functionalext" ] 
		then
			# continue
			for subdir in $dir/*
			do
				for file in `ls $subdir`
				do
					if [ "$(FileSuffix ${file})" = "so" ] \
					|| [ "$file" = "trace_stresstest" ] \
					|| [ "$file" = "fatal_message" ] \
					|| [ "$file" = "tgkill" ] \
					|| [ -d ./$subdir/$file ] 
					then
						continue
					else
						# echo file=$subdir/$file
						src/common/runtest -w '' ./$subdir/$file >> src/REPORT
					fi
				done
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
				else
					# echo file=$dir/$file
					src/common/runtest -w '' $dir/$file >> src/REPORT
				fi
			done
		fi
	fi
done