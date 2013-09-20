#!/bin/bash
# Tests starting with x are expected to fail


export LANG=C

GREEN="$(echo -e '\033[1;32m')"
YELLOW="$(echo -e '\033[0;33m')"
RED="$(echo -e '\033[1;31m')"
NORMAL="$(echo -e '\033[0;39m')"

success=0
fail=0

[ "$#" -lt 1 ] && exit

for bin in $*; do
	[ ! -f "$bin" ] && continue
	[ ! -x "$bin" ] && continue

	test=$bin
	log=${test}.log

	ret=0
	case $test in x*) ret=1 ;; esac

	echo -n "Running test $test... "
	./$test > $log
	if [ $? -ne $ret ]; then
		fail=$((fail + 1))
		echo "${RED}Failed $NORMAL"
	else
		success=$((success + 1))
		echo
		rm -f $log
	fi

	# If empty, remove
	[ ! -s "$log" ] && rm -f $log
done

echo

total=$((fail + success))
percentage=$(awk "BEGIN{print $success/$total * 100}")
percentage=$(printf '%.2f' $percentage)

num=${percentage//.*/}

[ $fail -eq 0 ] && echo "$GREEN	All tests passed!"
[ $fail -ne 0 -a $num -ge 60 ] && echo "$YELLOW	$percentage% passed, $fail/$total fails"
[ $num -lt 60 ] && echo "$RED	$percentage% passed, $fail/$total fails"

echo $NORMAL
