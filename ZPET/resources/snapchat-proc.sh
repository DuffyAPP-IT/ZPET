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
geofilter=$(sudo find . -name "com.pinterest.PINDiskCache.GeoFilterRenderedImages")
docobj=$(sudo find . -name "primary.docobjects");

#echo $arroyo
#echo $scdb

echo -e "----------\nBasic Information:"
echo "User ID ->" | tr -d '\n' && sqlite3 $scdb -line "SELECT ZUSERID FROM ZGALLERYPROFILE" | cut -f2 -d'='
echo "User Sync Token ->" | tr -d '\n' && sqlite3 $scdb -line "SELECT ZSYNCTOKEN FROM ZGALLERYPROFILE" | cut -f2 -d'='

sleep 5

echo -e "----------\nFinding & Reading GeoFilters...\n---------"
find $geofilter -size +5k -exec tesseract {} stdout --dpi 72 2>/dev/null \;

sleep 5


echo -e "----------\nFetching Friends List...\n---------"
sqlite3 $docobj -line "select legacyConversationId from index_arroyomigration__oneononemetadata_draftlegacyConversationId" | cut -f2 -d'='
echo "---------"
sleep 5

echo -e "----------\nFetching Account Information...\n---------"
rm adatain
sqlite3 $docobj -line "select hex(p) from userinfo__coreuserdata" | cut -f2 -d'=' >> adatain

if [ -f adatain ]
then
sed '/^[[:space:]]*$/d' adatain > adataout
count=0
while read p; do
echo "$p" | xxd -r -p > $count.adata
#rm $count.adata
#        Remove first 76 bytes
tail +76c $count.adata > $count.adata.truncated && mv $count.adata.truncated $count.adata

if [ -f $count.adata ]
then
sleep 1
echo 'PRINTING DATA...'
sleep 1
cat $count.adata
fi
rm $count.adata
count=$[$count +1]
done<adataout
fi

echo "---------"

sleep 1
echo -n "."
sleep 1
echo -n "."
sleep 1
echo -n "."
sleep 1
echo -n "."
`
sqlline="select hex(message_content) from conversation_message"
sid="select sender_id from conversation_message"
senttime="SELECT DATETIME(ROUND(creation_timestamp / 1000), 'unixepoch') AS isodate FROM conversation_message"
readtime="SELECT DATETIME(ROUND(read_timestamp / 1000), 'unixepoch') AS isodate FROM conversation_message"

friends="SELECT hex(p) FROM arroyomigration__oneononemetadata_draft";

#sqlite3 $docobj -line "$friends" | cut -f2 -d'=' >> fdatain

sqlite3 $arroyo -line "$sqlline" | cut -f2 -d'=' >> blobdata

sqlite3 $arroyo -line "$sid" | cut -f2 -d'=' >> siddatain

sqlite3 $arroyo -line "$senttime" | cut -f2 -d'=' >> senttimein

sqlite3 $arroyo -line "$readtime" | cut -f2 -d'=' >> readtimein

##Process Friend Data Column
#if [ -f fdatain ]
#then
#sed '/^[[:space:]]*$/d' fdatain > fdataout
#count=0
#while read p; do
##echo "$p" | xxd -r -p > $count.fdata
#protoc --decode_raw < $count.fdata
#count=$[$count +1]
#done<fdataout
#rm fdataout
#fi

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

echo -n "Message Data -> "
if protoc --decode_raw < $count.outsc | grep "351m" >/dev/null; then
protoc --decode_raw < $count.outsc | grep 11 -A2 | grep ' 1:'
fi
if protoc --decode_raw < $count.outsc | grep "307f" >/dev/null; then
protoc --decode_raw < $count.outsc | grep '4 {' -A2 | grep '1:'
fi
if protoc --decode_raw < $count.outsc | grep "010cl" >/dev/null; then
protoc --decode_raw < $count.outsc | grep '4 {' -A2 | grep '1:'
fi



#cat $count.outsc
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

rm adatain
rm adataout
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

