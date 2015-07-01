#!/bin/bash

function test {
	"$@"
	local status=$?
	if [ $status -ne 0 ]; then
		echo "error with $1" >&2
	fi
	return $status
}

echo "check_pm_support.sh"
test pm-is-supported --suspend
test pm-is-supported --hibernate
test pm-is-supported --suspend-hybrid
