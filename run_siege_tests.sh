echo "
*************************
!!! adresse IP de ta machine doit etre a jour dans le fichier my_ip.txt !!! 
*************************
"

# update IP address in siege_urls.txt
python3 tests/scripts/update_ip.py

#run tests
docker run --rm -t -v $(pwd)/tests/siege/siege_urls.txt:/siege_urls.txt jstarcher/siege -f /siege_urls.txt -c1 -r5