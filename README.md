<img src="OUT%20ZPET%20DESIGN%20FINAL-05.png">


# B100 Release Features:
 - Welcome Text On First Launch
 - Call-Related Data Extraction
 - Enhanced Error-Handling
 - Increased Overall Stability
 - A few hidden features for all to explore ❤️

# Build B91 RC1 Release Features
- Apple ID & Connected Device Accounts
- WiFi Network Detailed Extraction
- Flight Information (FROM/TO)
- Barcode Generator Info f/each pass - Wallet Pass Descriptions
- Apple Pay 4 Digits f/each Apple Pay Card
- Battery Serial Number Retrieval
- VPN Endpoint Detection/Fetcher
- Installed/Removed Applications/Bundle-Identifiers
- Extended Testing Enabled
- WhatsAPP Contact ID Fetching
- Mail VIP Contact List Fetching
- Passcode Attempt Information

# An Overview

![ZPET BUILD](https://github.com/DuffyAPP-IT/ZPET/workflows/ZPET%20BUILD/badge.svg?branch=master)

In a situation where an iOS Device is BFU, it’s generally speaking not possible to retrieve data from the iOS Device. The release of the checkm8 exploit made the challenge of extracting data much easier for the likes of Cellebrite and other forensics organisations. 

ZPET is designed to enable the public to understand and visualise the data that their device is holding in an unencrypted form. By executing ZPET while the device is locked and BFU, a wealth of information can be extracted and visualised by the end-user.

The information is extracted live from the device - in contrast to other solutions which require a rootFS dump to have already been acquired, saving valuable time in an investigation.

ZPET is also designed to work in a forensically-sound fashion, without modifying any files on the device and only retrieving the specific files of interest.

The software has been compiled to run on macOS Catalina and above, although should be compatible with 10.12 as there are not any APIs in use for ZPET that are specific to newer macOS versions. In a future release, I will recompile for a lower minimum operating system version in order to maximise the amount of compatible machines.

ZPET has its core functionality written in C which is compatible with other platforms and operating system versions, so there is a potential for porting to Linux and/or Windows.

The only other requirements are that you have SSHPASS installed via Brew Package Manager and iPhone Tunnel running on device port 44... and a checkm8 vulnerable device.
