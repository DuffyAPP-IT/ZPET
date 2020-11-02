#!/bin/bash

#echo "Dumping Database Blobs To HEX Values..."

echo "███████╗ ██████╗        ██████╗ ██████╗  ██████╗  ██████╗";
echo "██╔════╝██╔════╝        ██╔══██╗██╔══██╗██╔═══██╗██╔════╝";
echo "███████╗██║             ██████╔╝██████╔╝██║   ██║██║     ";
echo "╚════██║██║             ██╔═══╝ ██╔══██╗██║   ██║██║     ";
echo "███████║╚██████╗███████╗██║     ██║  ██║╚██████╔╝╚██████╗";
echo "╚══════╝ ╚═════╝╚══════╝╚═╝     ╚═╝  ╚═╝ ╚═════╝  ╚═════╝";
echo "v1 - SpiderKIT";

scdb=$(sudo find ./SENSITIVE -name "scdb-27.sqlite3")
arroyo=$(sudo find ./SENSITIVE -name "arroyo.db")

#echo $arroyo
#echo $scdb

echo -e "----------\nBasic Information:"
echo "User ID ->" | tr -d '\n' && sqlite3 $scdb -line "SELECT ZUSERID FROM ZGALLERYPROFILE" | cut -f2 -d'='
echo "User Sync Token ->" | tr -d '\n' && sqlite3 $scdb -line "SELECT ZSYNCTOKEN FROM ZGALLERYPROFILE" | cut -f2 -d'='
sleep 10

sqlline="select hex(message_content) from conversation_message"
sid="select sender_id from conversation_message"
senttime="SELECT DATETIME(ROUND(creation_timestamp / 1000), 'unixepoch') AS isodate FROM conversation_message"
readtime="SELECT DATETIME(ROUND(read_timestamp / 1000), 'unixepoch') AS isodate FROM conversation_message"

sqlite3 $arroyo -line "$sqlline" | cut -f2 -d'=' >> blobdata

sqlite3 $arroyo -line "$sid" | cut -f2 -d'=' >> siddatain

sqlite3 $arroyo -line "$senttime" | cut -f2 -d'=' >> senttimein

sqlite3 $arroyo -line "$readtime" | cut -f2 -d'=' >> readtimein

#Process Sender ID Column
if [ -f siddatain ]
then
sed '/^[[:space:]]*$/d' siddatain > siddataout
count=0
while read p; do
echo "$p" > $count.sid
count=$[$count +1]
done<siddataout
fi

#Process Sent DateTime Column
if [ -f senttimein ]
then
sed '/^[[:space:]]*$/d' senttimein > senttimeout
count=0
while read p; do
echo "$p" > $count.stime
count=$[$count +1]
done<senttimeout
fi

#Process Sent DateTime Column
if [ -f readtimein ]
then
sed '/^[[:space:]]*$/d' readtimein > readtimeout
count=0
while read p; do
echo "$p" > $count.rtime
count=$[$count +1]
done<readtimeout
fi

if [ -f blobdata ]
then
count=0
#    sidcount=0
while read p; do
length=${#p}
min=10 #testing

if [ "$length" -gt "$min" ]; then
echo "$p" | xxd -r -p > $count.outsc

#        Remove first 72 bytes
tail +60c $count.outsc > $count.outsc.truncated && mv $count.outsc.truncated $count.outsc

#        Remove last 75 bytes

newfsize=$(($(stat -f%z $count.outsc) - 72)) 2>/dev/null
truncate -s $newfsize $count.outsc 2>/dev/null

#        tr -d '\n\r' < $count.outsc

hexdump $count.outsc | grep '8b 01 0a 88'
isvid=$?
if [ $isvid -eq 0 ]
then

newfsize=$(($(stat -f%z $count.outsc) - 75)) 2>/dev/null
truncate -s $newfsize $count.outsc

fi

hexdump $count.outsc | grep '87 01 0a 84'
isvid=$?
if [ $isvid -eq 0 ]
then
tail +149c $count.outsc > $count.outsc.truncated && mv $count.outsc.truncated $count.outsc

newfsize=$(($(stat -f%z $count.outsc) - 162)) 2>/dev/null
truncate -s $newfsize $count.outsc

fi

hexdump $count.outsc | grep '8a 01 0a 87'
isvid=$?
if [ $isvid -eq 0 ]
then
tail +162c $count.outsc > $count.outsc.truncated && mv $count.outsc.truncated $count.outsc

newfsize=$(($(stat -f%z $count.outsc) - 188)) 2>/dev/null
truncate -s $newfsize $count.outsc

fi



echo -e '\n----------'
#            echo $count.sid
#            echo $count.outsc
#            echo -e '----------'

echo -n "Sender ID -> "
cat $count.sid

echo -n "Message Sent Date/Time -> "
cat $count.stime

echo -n "Message Read Date/Time -> "
cat $count.rtime

cat $count.outsc
rm $count.outsc
count=$[$count +1]

#            sidcount=$[$sidcount +1]
fi

done <blobdata

#    echo "Parsing Values..."
#    find . -name '*.outsc' -size +3c -exec cat {} 2>/dev/null \; >> vals
#    if [ -f vals ]
#        then
#            iconv -f utf-8 -t utf-8 -c vals
#            cat vals
#    else
#        echo Coud Not Find Values...
#    fi

echo "Cleaning Up..."
count=0
while read p; do
rm $count.sid
count=$[$count +1]
done <siddataout

echo "Cleaning Up..."
count=0
while read p; do
rm $count.stime
count=$[$count +1]
done <senttimeout

echo "Cleaning Up..."
count=0
while read p; do
rm $count.rtime
count=$[$count +1]
done <readtimeout

rm blobdata
rm siddatain
rm siddataout
rm senttimein
rm senttimeout
rm readtimein
rm readtimeout

else
echo Coud Not Find Blob Data...
fi

