#!/bin/bash
echo "===| Testing Cases Summary |==="
echo "Test 0: Compiling"
echo "Test 1: Simple GET"
echo "Test 2: GET with content-type, headers, and content"
echo "Test 3: 400"
echo "Test 4: 403"
echo "Test 5: 404"
echo "Test 6: 405"
echo "Test 7: 415"
echo "Test 8: HEAD"
echo "Test 9: Client Test"
echo -e "Test 10: Code style and readability\n"

PORT="8089"
SERVERDOCROOT="./docs/"
LOCALDOCROOT="./docs/"

TMPDIR=`mktemp --tmpdir=/dev/shm -dt "hw3Test.XXXXXXXXXX"`
touch ${SERVERDOCROOT}/restricted
chmod -r ${SERVERDOCROOT}/restricted

### Test 0: Compiling Test
echo "*** Test 0: Compiling ***"
tar xzf *-homework2.tar.gz &> /dev/null
warningcount=`make 2>&1 | grep -i "warning:" | wc -l`
if [ $warningcount -ge 1 ]; then
	echo -e "\nCompiler warnings found.\n"
fi

if test -e "./webserver" && test -e "./getfile"; then
  echo -e "\tPASS: Successful compilation \n"
  echo -e "===> Compiling score (10 points max): 10.0 <===\n\n\n"
  P0=10
else
  echo -e "\t__ERROR__: Compilation Failed\n"
  echo -e "(Cannot find the execution file ./webserver or ./getfile)\n"
  echo -e "===> Test 0 score (10 points max): 0.0 <===\n\n\n"
  P0=0
fi
  
###echo "Killing zombies"
killall -q -9 webserver &> /dev/null
killall -q -9 printf &> /dev/null
sleep 1

### Test 1-9: Ruby unit tests
echo "*** Test 1-9: Functionality ***"
stdbuf -o0 ./webserver -t $PORT $SERVERDOCROOT | while read line; do echo "[SERVER] $line"; done &
P1=$(ruby hw2Test.rb -t $PORT -d $LOCALDOCROOT)
echo -e "\n===> Test 1-9 score (150 points max): ${P1} <===\n\n\n"

### cleanup
killall -q -9 webserver &> /dev/null
sleep 1

### Test 10: Code style and readability
echo "*** Test 10: Code style and readability ***"
echo -e "\n===> Test 10 score (10 points max): 10.0 <==="
P2=10


### Calculate total score 
echo -e "\n\n\n---------------------------------"
echo -e "Total Score (170 points max): $[$P0+$P1+$P2]"
echo -e "---------------------------------"

rm -rf ${SERVERDOCROOT}/restricted
