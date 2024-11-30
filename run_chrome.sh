#!/bin/bash

# Check if the argument is provided
if [ -z "$1" ]; then
	echo "Usage: ./tab_chrome.sh <config_number>"
	echo "Example : ./tab_chrome.sh 012 ==> For config012.ini"
	exit 1
fi

SCRIPT_NUMBER=$1

# Launch the web server
./webserv Requests_Tester/configs/config${SCRIPT_NUMBER}.ini > /dev/null 2>&1 &

webserv_pid=$!

# Launch the corresponding Python script
python3 Requests_Tester/manual_chrome_test/launch_chrome.py ${SCRIPT_NUMBER}

while true; do
	read -p "Do you want to stop webserv? (y/n) " answer
	if [ "$answer" != "${answer#[Yy]}" ]; then
		kill "$webserv_pid"
		wait "$webserv_pid" 2>/dev/null || {
			kill -9 "$webserv_pid" 2>/dev/null
		}
		break
	else
		echo "webserv is still running with PID $webserv_pid"
	fi
done