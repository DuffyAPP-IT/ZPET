#!/binpack/bin/bash

echo [*] ///ZPETv2 SPIDER Local Integration///
echo [*] Build 3

if [ $# -eq 0 ]
then
    echo -e "[!] Spider (for ZPETv2) should not be directly executed... Head to DuffyAPP-IT on GitHub to download the standalone distribution!"
    exit 1
#	echo -e "Usage: ./spider\n-f\t\t\t\tFind Databases & Dump Schemas\n-p\t\t\t\tExtract Apple Photos Shared Album Information\n-i yourpersonalinfo\t\tDetect Personal Information Within Databases\n-a\t\t\t\tExtended Scan"
fi

echo Background Init...
#Initialise Local Counters
ljpgCount=0
lpngCount=0

# Check if rootFS is present
if [ ! -d /private ]
then
echo [!] iOS Root Filesystem Not Detected
echo [!] Exiting For Safety Reasons...
exit 1
fi

#Clean Old Directories & Files
if [ -d /SpiderOUT ]
then
rm -rf /SpiderOUT
fi

mkdir /SpiderOUT


# Database Schema Extraction
# - Last Modified -> 25-11-2020
# - Notes:
#        * Extracts Table Formats/'schemas' from databases with or without the correct extention appended...
if [[ $1 = "-f" ]]
then

#         **********
#         Lots of extra exec time... is this necessary?
#         **********

#		echo Finding SQLITE DBs
#		dbCount=$(find . -name '*.sqlite' 2>/dev/null | tee SpiderOUT/DB.txt | wc -l >/dev/null 2>/dev/null )
#		echo Found $dbCount DBs
#
#		echo Finding DBs
#		dbCount=$(find . -name '*.db' 2>/dev/null | tee SpiderOUT/DB2.txt | wc -l >/dev/null 2>/dev/null )
#		echo Found $dbCount DBs


		echo "[+] Extracting Database Schemas (sqlite)"
		find / -name '*.sqlite' -print -exec ls -lh {} \; -exec /usr/bin/sqlite3 {} '.tables' 2>/dev/null \; | tee /SpiderOUT/TableData.txt

		echo "[+] Extracting Database Schemas (db)"
		find / -name '*.db' -print -exec ls -lh {} \; -exec /usr/bin/sqlite3 {} '.tables' 2>/dev/null \; | tee /SpiderOUT/TableData.txt
  
        echo "[+] Extracting Database Schemas (db)"
        find / -name '*.db' -print -exec ls -lh {} \; -exec /usr/bin/sqlite3 {} '.tables' 2>/dev/null \; | tee /SpiderOUT/TableData.txt
        
        echo "[+] Hidden Database Identification...(this may take a while)"
        find / -type f -size +10k -size -800k -not -name "*.db" -not -name "*.sqlite*" -exec file '{}' \; 2>/dev/null | grep -H SQLite | cut -f2 -d':' | tee /SpiderOUT/HiddenDBs.txt
        
        echo "[+] Extracting Hidden Database Schemas (db)"
        count=0
        while read p; do
        sqlite3 "$p" '.tables' > /SpiderOUT/HiddenTableData.txt
        count=$[$count +1]
        done</SpiderOUT/HiddenDBs.txt
        
        exit 0;
fi

if [[ $1 = "-p" ]]
then
		echo [+] Extracting ConnectedAlbum Details
		find /private/var/mobile/Library/MediaStream/albumshare/ -name 'Model.sqlite' -exec /usr/bin/sqlite3 {} 'SELECT email FROM 'AccessControls'' \; 2>/dev/null | tee /SpiderOUT/ConnectedAlbum.txt

		echo [+] Extracting SharedAlbum URLs
		find /private/var/mobile/Library/MediaStream/albumshare/ -name 'Model.sqlite' -exec /usr/bin/sqlite3 {} 'SELECT name,url FROM 'Albums'' \; 2>/dev/null | tee /SpiderOUT/SharedAlbum.txt
        exit 0;
fi

if [[ $1 = "-i" ]]
then
	if [ $2 = 0 ]
	then
		echo "[!] No Information Supplied... Exiting For Safety"
		exit 1
	fi
	echo "[+] Searching..."
	find / -name '*.sqlite' -exec grep -H $2 {} \; 2>/dev/null | tee /SpiderOUT/MATCHSQLITE.txt 2>/dev/null
	find / -name '*.db' -exec grep -H $2 {} \; 2>/dev/null | tee /SpiderOUT/MATCHSQLITE.txt 2>/dev/null
    exit 0;
fi


