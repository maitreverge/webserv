#!/bin/bash

echo "
*************************
!!! adresse IP de ta machine doit etre a jour dans le fichier my_ip.txt !!! 
*************************
"

# update IP address in siege_urls.txt
python3 tests/scripts/update_ip.py

ip=$(cat tests/config/my_ip.txt)
run tests
echo "
*************************
***** Test de charge ****
*************************
"
docker run --rm -t jstarcher/siege -c10 -r20  "$ip:1024"  | grep -v "HTTP"

echo "
*************************
***Tests de Spike *******
*************************
"
docker run --rm -t jstarcher/siege -c100 -t10s  "$ip:1024"  | grep -v "HTTP"

docker run --rm -t -v $(pwd)/tests/siege/siege_urls.txt:/siege_urls.txt jstarcher/siege -f /siege_urls.txt -c2 -r5

echo "
************************************
***** Test de charge BRUTEFORCE ****
************************************
"
docker run --rm -t jstarcher/siege -b -c10 -r20  "$ip:1024"  | grep -v "HTTP"

echo "
*********************************
***Tests de Spike BRUTEFORCE ****
*********************************
"
docker run --rm -t jstarcher/siege -b -c100 -t10s  "$ip:1024"  | grep -v "HTTP"

docker run --rm -t -v $(pwd)/tests/siege/siege_urls.txt:/siege_urls.txt jstarcher/siege -f /siege_urls.txt -c2 -r5
