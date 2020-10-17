<img src="OUT%20ZPET%20DESIGN%20FINAL-05.png">

# ZPETv2 - Open Source iOS Forensics
![ZPET BUILD](https://github.com/DuffyAPP-IT/ZPET/workflows/ZPET%20BUILD/badge.svg?branch=master)

**ZPET** is designed to allow for the analysis of iDevices in **Before First Unlock (BFU)** Mode. ZPET will allow for the extraction of various pieces of information which may be considered sensitive.
Examples include **Installed Application Identifiers**, **WiFi Access Point Information**, **Apple ID** & **Other Accounts on-device**.
Other than the built-in Modules, the community has the option to create their own Modules for ZPET.
This will allow for the functionality of ZPET to be expanded and support extra third party applications such as Twitter (WIP), WhatsAPP, Facebook, Signal and more.

The data is extracted live from the device and processed immediately on the host machine - in contrast to other solutions which require a rootFS dump to have already been acquired, **saving valuable time during the device triage process**.

The software has been compiled to run on macOS High Sierra and above, although should be compatible with 10.12 as there are not any APIs in use for ZPET that are specific to newer macOS versions. In a future release, I will recompile for a lower minimum operating system version in order to maximise the amount of compatible machines.

Linux + Windows Support is actively being worked on and will be available in future updates.
