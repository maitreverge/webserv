#!/bin/bash

EXECUTABLE="./tests/webtest"

if [ -z "$1" ]; then
	$EXECUTABLE
	exit
else
	FILTER=""
	for CLASS_NAME in "$@"; do
		FILTER+="${CLASS_NAME}.*:"
	done
	FILTER=${FILTER%:}
fi

$EXECUTABLE --gtest_filter=$FILTER