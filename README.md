<img src="OUT%20ZPET%20DESIGN%20FINAL-05.png">

# ZPETv2 - Open Source iOS Forensics
![ZPETv2 macOS](https://github.com/DuffyAPP-IT/ZPET/workflows/ZPETv2%20macOS/badge.svg)

---

# Project Overview

ZPET is an application dedicated to the forensic study of locked iOS Devices. It features a variety of different operation modes, ranging from 'blind analysis' of a connected device, to ingesting user-supplied data and identifying files of interest within the connected device.

ZPET also features 'module processing' abilities, where select user-data will be extracted from the connected device, using artifact information supplied in the ZPET Module Format. ZPET modules are placed in the 'modules' folder of the applications directory structure and will be imported automatically at runtime.

# Software/Hardware Requirements

With ZPET being written in C++, ZPET is, at it's core, fully cross platform. However, there are currently a few external dependencies i'll need to remove before that's possible...

### Hardware Requirements

- Mac running macOS 10.13 or later.
- Lightning to USB-A cable.
- An iOS device running iOS 12, 13 or 14.

There are no requirements for external dongles or proprietary cables.

### Software Requirements

[Untitled](https://www.notion.so/47da82edbcaa4e7b993c79964ef3461c)

ZPET can operate in a completely offline environment. There are some optional analytics you can enable - and I would really appreciate it if you did! Although there's absolutely no obligation. Absolutely zero PII will be submitted, and the source code is open for auditing via GitHub.

# Device Initialisation Process

When selecting an operation mode within ZPET, you'll have to complete a short 'device initialisation process' - This allows ZPET to interface with the connected device.

## Checkra1n/SSH

When initialising a connection to a device vulnerable to checkm8, it's suitable for us to to boot the device using checkra1n in order for the 'Secure SHell Daemon' to begin executing on the device.

We'll need to do this prior to using any of ZPETs functionality so that ZPET can establish a communication channel between the device and our host machine.

When selecting an operation that requires access to the live device, we'll be asked to input some basic information pertaining to our connected device - here's an example... 

![ZPET%20-%20The%20Zero%20Pin%20Extraction%20Toolkit%20-%20B18500%20d28c24283abe4a0d84cc551d79f0a8a7/Screenshot_2020-11-28_at_00.37.01.png](ZPET%20-%20The%20Zero%20Pin%20Extraction%20Toolkit%20-%20B18500%20d28c24283abe4a0d84cc551d79f0a8a7/Screenshot_2020-11-28_at_00.37.01.png)

A few moments following a successful connection, the new 'Device Info' view will appear, which is designed to display some basic information pertaining to the connected device.

The most significant value in the Device Info view is arguably the 'Encryption State'. The value is either 'Encrypted (BFU)' or 'Decrypted (AFU)', and represents the current state of the connected device.

![ZPET%20-%20The%20Zero%20Pin%20Extraction%20Toolkit%20-%20B18500%20d28c24283abe4a0d84cc551d79f0a8a7/Screenshot_2020-11-28_at_00.38.08.png](ZPET%20-%20The%20Zero%20Pin%20Extraction%20Toolkit%20-%20B18500%20d28c24283abe4a0d84cc551d79f0a8a7/Screenshot_2020-11-28_at_00.38.08.png)

Here's some further reading around the 'BFU' and 'AFU' states...

[Speed > Security - Apple's Approach To iOS Data Security](https://blog.elcomsoft.com/2020/08/speed-security-apples-approach-to-ios-data-security/)

## TAR / Acquired Full Filesystem

Build 18500 establishes the foundations for TAR support in ZPET. TAR support is not yet fully implemented, and ZPET Modules can **not** take advantage of a previously acquired filesystem.

However, other Operation Modes including Spider **are** compatible with the acquired filesystems, and can operate locally without  a connected device.

# Operation Modes

## SPIDER - Local/Live

![ZPET%20-%20The%20Zero%20Pin%20Extraction%20Toolkit%20-%20B18500%20d28c24283abe4a0d84cc551d79f0a8a7/Screenshot_2020-11-27_at_22.47.41.png](ZPET%20-%20The%20Zero%20Pin%20Extraction%20Toolkit%20-%20B18500%20d28c24283abe4a0d84cc551d79f0a8a7/Screenshot_2020-11-27_at_22.47.41.png)

Spider allows for both blind and user-data ingest based file identification, and can operate on both a **local filesystem dump** - and **directly on a live device**. Spider is research-centric, and designed to allow forensic enthusiasts of all skill levels to **discover new artifacts** (which we can later translate to ZPET modules).

### Database Schema Extraction w/Hidden Database Identification

This option within Spider will identify SQLite databases within the filesystem of your iDevice, and export the database path followed by a list of tables within each. This output is saved to **SENSITIVE/local** in the ZPET directory structure.

We can begin to make use of this exported data using **grep**, a pattern matching tool you'll find on almost all *nix systems including macOS - here's an example...

```bash
cat SENSITIVE/local | grep -A4 -B4 'CHAT'
```

In this example, we're using 'cat' to capture the contents of 'SENSITIVE/local', and passing it to 'grep' - which will match lines with the text 'CHAT' within. Using the '-A4 -B4' flag, we capture -+4 lines around the match, allowing us to identify the database path.

We can then further our research using **SQL** to query database contents and check for potentially useful contents. You'll find an introductory guide here:

[Taking The First Step - iOS Security & Forensics -P1 - James Duffy](https://www.duffy.app/blog/taking-the-first-step-p1)

### User-Data Ingest - Keyword Search

Spider also provides the ability to complete a 'keyword search' of the entire filesystem - this can be executed on both the connected device live, saving the time of completing a device filesystem dump - or on an already captured filesystem dump.

The user-data ingest option will take a single keyword, and identify databases in the filesystem where the specific keyword can be found. This allows for a much more targeted identification of data.

An example use case could be using a friends name as a keyword, a bank account number or account username as the keyword.

![ZPET%20-%20The%20Zero%20Pin%20Extraction%20Toolkit%20-%20B18500%20d28c24283abe4a0d84cc551d79f0a8a7/Screenshot_2020-11-27_at_23.12.11.png](ZPET%20-%20The%20Zero%20Pin%20Extraction%20Toolkit%20-%20B18500%20d28c24283abe4a0d84cc551d79f0a8a7/Screenshot_2020-11-27_at_23.12.11.png)

Using the standalone Spider distribution from my GitHub (DuffyAPP-IT), you can create a 'keyword-list' so as to automate a little more of your process.

Below you'll see an example result...

![ZPET%20-%20The%20Zero%20Pin%20Extraction%20Toolkit%20-%20B18500%20d28c24283abe4a0d84cc551d79f0a8a7/Screenshot_2020-11-27_at_23.19.10.png](ZPET%20-%20The%20Zero%20Pin%20Extraction%20Toolkit%20-%20B18500%20d28c24283abe4a0d84cc551d79f0a8a7/Screenshot_2020-11-27_at_23.19.10.png)

The clear benefit to using keyword searching is being able to quickly identify content in databases you may have not otherwise encountered.

### Apple Photos Connected Album Data

This option allows for the extraction of Apple Photos Shared Album titles, as well as the direct URLs to access the photos within those albums. These links will require authentication, although in theory albums set to public should be accessible without authentication... I have not verified this.

This option should ideally be translated to a ZPET module, although it was present in an older Spider build and so I left it in this release version. It'll soon be migrated to it's own ZPET module.

If you're interested in researching further in this area, here's an article of mine which may provide a starting point.

[Photo.sh - Analysing The Locked iPhone - Apple Photos Shared Albums 😼 - James Duffy](https://www.duffy.app/blog/photosh)

---

## ZPETv2 Modules

### Overview

ZPETv2 modules are small files, ~5-8 lines, that each define 'artifacts'. This involves defining a 'location to find X data', the format of said data, and 'how to approach the data'.

ZPET does not require user input to process user-supplied modules. To execute a specific module, the process would be as follows...

1. Place the module file in the '**modules/**' directory in the ZPET directory structure.
2. Open the '**moduleloader**' file within the '**modules/**' directory with nano, or your favourite text editor - simply add the filename of the module to the '**moduleloader**' file. It's case sensitive.
3. Launch ZPET and proceed with the '**Execute ZPETv2 Modules**' option.

### File Format Support

ZPET supports parsing the following formats:

- **PLIST**, via the use of '**plutil**' (macOS/*nix)
- **BPLIST**, via the use of '**plistbuddy**' (macOS)
- **JSON** (Python)
- **SQLITE/DB**, via the use of '**sqlite3**' (macOS/*nix)

Information pertaining to ZPET Module Development can also be found in this documentation.

### Example Module Output

```bash
[*] Module -> Last Device Update Timestamp
[*] Author -> DuffyAPP_IT
[*] Module Output -> ...
----------
Finished Copy
2020-11-25 13:14:08
----------
```

ZPETv2 modules will appear in the above format, exporting data to external files in the '**SENSITIVE/**' directory where appropriate. The '——————' denotes the start and end of the module's outputted data.

This process will repeat for all supplied modules in '**modules/moduleloader**'

Some modules, such as 'sc-proc' (designed to handle Snapchat chat data using custom formatting) will output data using it's own style. While it will still follow the 'module, author, module output' style - the module output style will be bespoke for the application/database being parsed.

![ZPET%20-%20The%20Zero%20Pin%20Extraction%20Toolkit%20-%20B18500%20d28c24283abe4a0d84cc551d79f0a8a7/Screenshot_2020-11-27_at_23.53.57.jpg](ZPET%20-%20The%20Zero%20Pin%20Extraction%20Toolkit%20-%20B18500%20d28c24283abe4a0d84cc551d79f0a8a7/Screenshot_2020-11-27_at_23.53.57.jpg)

SC-Proc Snapchat Data Processor

### ZPET Module Development Specification

[Untitled](https://www.notion.so/c7ec972408f94ee5a267ec38db5ead00)

It’s worth noting that if you require **Pipe-Init-Process**, but not **Exec End**, you are able to replace line **7** with a blank line.

![ZPET%20-%20The%20Zero%20Pin%20Extraction%20Toolkit%20-%20B18500%20d28c24283abe4a0d84cc551d79f0a8a7/Screenshot_2020-11-28_at_01.27.01.png](ZPET%20-%20The%20Zero%20Pin%20Extraction%20Toolkit%20-%20B18500%20d28c24283abe4a0d84cc551d79f0a8a7/Screenshot_2020-11-28_at_01.27.01.png)

Complete ZPETv2 Module Examples - Apple Wallet Pass Data Extraction & VPN Endpoint Identifier

## Device Root Filesystem Acquisition

It is possible to acquire the root filesystem of the connected device using ZPET.

Following the device initialisation process, ZPET will acquire the device filesystem contents, storing as a **TAR** in the '**SENSITIVE/**' directory within the ZPET directory structure.

It is important to ensure you have enough free space on your local machine prior to dumping the filesystem, as a single **TAR** can range from 5-100+GB depending on the Encryption State of the device as well as, of course, the amount of data stored on the device.

The Device Info view will appear before the acquisition begins, allowing you to identify the current encryption state of the device.

![ZPET%20-%20The%20Zero%20Pin%20Extraction%20Toolkit%20-%20B18500%20d28c24283abe4a0d84cc551d79f0a8a7/Screenshot_2020-11-28_at_01.26.19.png](ZPET%20-%20The%20Zero%20Pin%20Extraction%20Toolkit%20-%20B18500%20d28c24283abe4a0d84cc551d79f0a8a7/Screenshot_2020-11-28_at_01.26.19.png)

# Contact

If you have any questions around using, developing for, or diagnosing an issue with ZPET (and feedback too!), please do get in contact!

I'm available via Twitter **@J_Duffy01** and e-mail **public@duffy.app**.

Thank you! I hope you enjoy **ZPET**.
