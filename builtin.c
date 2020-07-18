//
// Created by 010010 on 28/05/2020.
//

#include "res.h"
#include "processor.h"

struct module wifi(){
    struct module wifi;
    wifi.name="Wifi Module";
    wifi.parsetype=3; //plist
    wifi.foi="/private/var/preferences/SystemConfiguration/com.apple.wifi.plist";
    wifi.description="WiFi Data Extraction";
    wifi.sval="SSID_STR";
    wifi.version=1;
    wifi.dict="List of known networks";
    wifi.usecust=1;
    wifi.cust="echo '\<h2\>WiFi Module - ZPET Output:\</h2\>' \>\> report.html";
    return wifi;
}

struct module wifiloc(){
    struct module wifiloc;
    wifiloc.name="Accounts Fetch Module";
    wifiloc.parsetype=2; //db
    wifiloc.foi="/private/var/root/Library/Caches/com.apple.wifid/ThreeBars.sqlite";
    wifiloc.description="Extract Account Data From Connected Device";
    wifiloc.sql="select ZLAT, ZLNG, ZBSSID from ZACCESSPOINT";
    wifiloc.sval="";
    wifiloc.usecust=1;
    wifiloc.cust="echo '\<h2\>WiFi Locations - ZPET Output:\</h2\>' \>\> report.html";
    wifiloc.version=1;
    return wifiloc;
}

struct module accounts(){
    struct module accounts;
    accounts.name="Accounts Fetch Module";
    accounts.parsetype=2; //db
    accounts.foi="/private/var/mobile/Library/Accounts/Accounts3.sqlite";
    accounts.description="Extract Account Data From Connected Device";
    accounts.sql="SELECT DISTINCT zusername FROM ZACCOUNT";
    accounts.sval="";
    accounts.usecust=1;
    accounts.cust="echo '\<h2\>Account Fetch Module - ZPET Output:\</h2\>' \>\> report.html";
    accounts.version=1;
    return accounts;
}

struct module bsn(){
    struct module bsn;
    bsn.name="Serial Fetcher";
    bsn.parsetype=3; //plist
    bsn.foi="/private/var/preferences/com.apple.batterydata.cyclecount.plist";
    bsn.description="Battery Serial Finder";
    bsn.sval="Serial";
    bsn.usecust=1;
    bsn.cust="echo '\<h2\>Battery Serial Module - ZPET Output:\</h2\>' \>\> report.html";
    bsn.dict="";
    return bsn;
}

struct module vpnd(){
    struct module vpnd;
    vpnd.name="VPN Detail Fetcher";
    vpnd.parsetype=3; //plist
    vpnd.foi="/private/var/preferences/com.apple.networkextension.plist";
    vpnd.description="VPN Endpoint Fetch Module";
    vpnd.sval="-a @ -B 1";
    vpnd.usecust=1;
    vpnd.cust="echo '\<h2\>VPN Endpoint Fetch Module - ZPET Output:\</h2\>' \>\> report.html";
    vpnd.dict="";
    return vpnd;
}

struct module appids(){
    struct module appids;
    appids.name="Application Bundle ID Fetch Module";
    appids.parsetype=2; //db
    appids.sval="";
    appids.foi="/var/mobile/Library/FrontBoard/applicationState.db";
    appids.description="Extract Bundle Identifiers From Installed/Removed Applications.";
    appids.sql="SELECT DISTINCT Application_identifier FROM kvs_debug";
    appids.version=1;
    appids.usecust=1;
    appids.cust="echo '\<h2\>Bundle ID Fetch Module - ZPET Output:\</h2\>' \>\> report.html";
    return appids;
}

struct module wallet(){
    struct module wallet;
    wallet.name="AppleWallet-Processor";
    wallet.parsetype=1;
    wallet.version=2;
    wallet.foi="/var/mobile/Library/Passes/Cards/";
    wallet.description="Extract Apple Wallet Passes";
    wallet.sval="";
    wallet.useendcust=1;
    wallet.endcust="mkdir Data/WalletCardImages Data/WalletCardJSON >/dev/null 2>/dev/null \; find . -name '*.png' -exec mv {} Data/WalletCardImages >/dev/null 2>/dev/null \; echo '\<h2\> Pass Descriptions \<\/h2\> \<ul\>' \>\> report.html \; find . -name 'pass.json' -exec python -m json.tool {} \\; \| grep description \| cut -f2- -d':' | sort --unique | awk '{print \"\<li\>\"$0\"\<\/li\>\"}' \>\> report.html ; echo '\<\/ul\>' \>\> report.html \| tee Data/WalletCardJSON/passdesc.txt \; echo '\<h2\> Pass Flight IDs \<\/h2\> \<ul\>' \>\> report.html \; find . -name 'pass.json' -exec python -m json.tool {} \\; \| grep altText \| cut -f2- -d':' | sort --unique | awk '{print \"\<li\>\"$0\"\<\/li\>\"}' \>\> report.html ; echo '\<\/ul\>' \>\> report.html \| tee Data/WalletCardJSON/passflightid.txt \; echo //////////PASS BARCODES////////// \; find . -name 'pass.json' -exec python -m json.tool {} \\; \| grep PKBarcodeFormat -A 1 \| tee Data/WalletCardJSON/passbarcodes.txt \; echo //////////PASS SYNC AUTH KEYS////////// \; find . -name 'pass.json' -exec python -m json.tool {} \\; \| grep authenticationToken \| tee Data/WalletCardJSON/passauth-keys.txt \; echo '\<h2\> Apple Pay 4 Digits \<\/h2\> \<ul\>' \>\> report.html \; find . -name 'pass.json' -exec python -m json.tool {} \\; \| grep CARD -A 1 \| grep -o \"[0-9][0-9][0-9][0-9]\" \| sort --unique | awk '{print \"\<li\>\"$0\"\<\/li\>\"}' \>\> report.html ; echo '\<\/ul\>' \>\> report.html \|  tee Data/WalletCardJSON/applepay.txt \;";
    return wallet;
}

struct module mailextract(){
    struct module mailextract;
    mailextract.parsetype=4;
    mailextract.name="PDF Finder - CREDITS FOR HELP -> digital-forensics.it";
    mailextract.description="Mail Data Extraction";
    mailextract.foi="/private/var/mobile/Library/Mail";
    mailextract.sval="";
    mailextract.usecust=1;
    mailextract.cust="mkdir Data/MailPDF Data/MailDOC Data/MailXLS >/dev/null 2>/dev/null \; find Data/OUT/ -type f -name \"*.pdf\" -exec mv {} Data/MailPDF >/dev/null 2>/dev/null \; find Data/OUT/ -type f -name \"*.doc*\" -exec mv {} Data/MailDOC >/dev/null 2>/dev/null \;  find Data/OUT/ -type f -name \"*.pdf\" -exec mv {} Data/MailPDF >/dev/null 2>/dev/null \; find /private/var/mobile/Library/Mail -type f -name \"*.xls*\" -exec mv {} Data/MailXLS >/dev/null 2>/dev/null \; echo '\<h3\> Check Data/Mail* for Mail Extraction \<\/h3\> \<ul\>' \>\> report.html";
    return mailextract;
}

struct module wapp(){
    struct module wapp;
    wapp.parsetype=4;
    wapp.name="WhatsAPP Contact ID Extract";
    wapp.description="Extract WhatsAPP Contact IDs From History";
    wapp.foi="/private/var/mobile/Containers/Data/Application/F3A03AA5-0041-4B55-AB29-B8FE1AAA1111/";
    wapp.usefoi2=1;
    wapp.foi2="/private/var/mobile/Containers/Shared/AppGroup/2A1045A5-7EF1-4571-833C-029B0E19E7AC/";
    wapp.sval="";
    wapp.usecust=1;
    //find /private/var/mobile/Containers -type d -name "*@s.whatsapp.net*" | grep -o '[0-9][0-9][0-9][0-9][0-9][0-9][0-9]*'
    wapp.cust="echo '\<\/ul\> \<h2\> WhatsAPP Contact IDs \<\/h2\> \<ul\>' \>\> report.html \; find . -type d -name \'*@s.whatsapp.net*\' \| grep -o '[0-9][0-9][0-9][0-9][0-9][0-9][0-9]*' \| sort --unique | awk '{print \"\<li\>\"$0\"\<\/li\>\"}' \>\> report.html ; echo '\<\/ul\>' \>\> report.html";
    return wapp;
}

struct module ktxproc(){
    struct module ktxproc;

    //find all ktx files in rootfs,
    //return to mac
    //remove the first 8 bytes of file
    //rename all to .png/.jpg or attempt file identification

    return ktxproc;
}

struct module mailvip(){
    struct module mailvip;
    mailvip.name="Mail VIP Extraction Module";
    mailvip.parsetype=4; //plist
    mailvip.foi="/private/var/mobile/Library/Mail/VIPs.plist";
    mailvip.description="VIP List Extractor";
    mailvip.sval="";
    mailvip.version=1;
    mailvip.dict="List of known networks";
    mailvip.usecust=1;
    mailvip.cust="echo '\<\/ul\>  \<h2\>Mail VIP List - ZPET Output:\</h2\>' \>\> report.html \; echo '<ul>' \>\> report.html ; /usr/libexec/PlistBuddy -c 'print \":\"' Data/OUT/VIPs.plist | grep -v Dict | grep -v EmailAddresses | grep -v '}' | awk '{print \"\<li\>\"$0\"\<\/li\>\"}' \>\> report.html ; echo '\<\/ul\>' \>\> report.html";
    return mailvip;
}


//fix sqlite3 unixepoch column not found issue.
struct module PCInfo(){
    struct module pcinfo;
    pcinfo.name="Passcode Information Fetch Module";
    pcinfo.parsetype=2; //db
    pcinfo.foi="/private/var/mobile/Library/AggregateDictionary/ADDataStore.sqlitedb";
    pcinfo.description="Extract Vague Passcode Data From Connected Device";
    pcinfo.sql="SELECT KEY AS 'KEY', VALUE AS 'VALUE' FROM SCALARS where key like \"com.apple.passcode.NumPasscode%\" ";
    pcinfo.sval="";
    pcinfo.usecust=1;
    pcinfo.cust="echo '\<h2\>Passcode Info Fetch - ZPET Output:\</h2\>' \>\> report.html";
    pcinfo.version=1;
    return pcinfo;
}

struct module callhist(){
    struct module callhist;
    callhist.parsetype=2;
    callhist.name="Call History";
    callhist.description="Call-Related Data Extraction";
    callhist.foi="/private/var/mobile/Library/CallHistoryDB/CallHistory.storedata";
    callhist.sval="";
    callhist.sql="SELECT ZADDRESS AS 'ADDRESS', ZANSWERED AS 'WAS ANSWERED', ZCALLTYPE AS 'CALL TYPE', ZORIGINATED AS 'ORIGINATED', ZDURATION AS 'DURATION', ZISO_COUNTRY_CODE as 'ISO COUNTY CODE', ZLOCATION AS 'LOCATION', ZSERVICE_PROVIDER AS 'SERVICE PROVIDER' FROM ZCALLRECORD";
    callhist.usecust=1;
    callhist.cust="echo '\<h2\>Call Hist - ZPET Output:\</h2\>' \>\> report.html";
    return callhist;
}