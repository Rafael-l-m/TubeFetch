# TubeFetch

<br>

A multi-threaded **YouTube media downloader** with flexible format control. The application supports concurrent downloads and allows users to either:

-    Automatically fetch the highest‑quality audio or video stream
    
-    Select a specific **itag** for custom output formats
    
Ideal for users who want both convenience and fine‑grained control over YouTube media extraction

**Warning**: On the initial startup, the app may require two launches to function correctly. The first launch completes the download of all necessary components, and the second launch will operate as expected. A spinning loading indicator during the download is normal, particularly for macOS users

**Warning for macOS users**: If macOS displays a message stating that the application “cannot be opened,” users may enable it manually by navigating to **System Settings → Privacy & Security** and allowing the app. This warning appears because the application has not undergone Apple’s notarization process, which requires enrollment in the paid Apple Developer Program

**Warning for Windows users**: when downloading the installer from Github, Windows may show a **SmartScreen** such as: "Unknown publisher", "This app might put your PC at risk". This happens because the installer is **not code-signed**. Unsigned applications are common in open-source projects, and this warning does not mean the file is harmful

**Warning for Windows 11 users**: For users on the latest version of Windows 11, if Smart App Control is enabled by default, you will need to turn it off; otherwise, the uninstaller may fail to launch properly. The steps are: Windows Security → App & browser control → Smart App Control → Smart App Control settings → Off

**This application has undergone basic testing, but it has not yet been extensively tested on Windows or Linux. Feedback is highly appreciated — any issues you report will play an important role in improving and shaping the future of this project. Thank you very much for your support**


<br><br>

# Table of Contents
- [Download](#download)
- [Pre-requisites](#pre-requisites)
- [Platforms Supported](#platforms-supported)
- [Quick Start](#quick-start)
- [Usage](#usage)
- [FAQ](#faq)
- [Compile](#compile)
- [License](#license)


<br><br>

# Download

You can download the latest version from the Releases page: https://github.com/Rafael-l-m/TubeFetch/releases


<br><br>

# Pre-requisites

Before running the app, make sure you have the following:

-    [ffmpeg](https://ffmpeg.org/) installed
-    [node.js](https://nodejs.org/) installed
-    A stable internet connection

<br>

## For macOS Users (Apple Sillicon)

**Homebrew** is the recommended way to install it

-    Install Xcode Command Line Tools:

```bash
	xcode-select --install
```

-    Install Homebrew:

```bash
	/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

-    Install necessary tools:

```bash
	brew install ffmpeg
```
```bash
	brew install node
```

<br>

## For macOS Users (Intel Mac)

**MacPorts** is the recommended way to install it

-	Install Xcode Command Line Tools:

```bash
	xcode-select --install
```

-    Install MacPorts

-    Install necessary tools:

```bash
	sudo port install ffmpeg
```
```bash
	sudo port install node22
```
```bash
	sudo port install npm10
```

<br>

## For Windows Users (x86-64)

**Scoop** is the recommended way to install it

-    Install Scoop:

```Powershell
	Set-ExecutionPolicy RemoteSigned -Scope CurrentUser
```
```Powershell
	iwr -useb get.scoop.sh | iex
```

-    Install necessary tools:

```bash
	scoop install ffmpeg
```
```bash
	scoop install nodejs-lts
```

<br>

## For Linux (Ubuntu) Users (x86-64)

**Homebrew** is the recommended way to install it

-    Install dependencies:

```bash
	sudo apt install -y build-essential procps curl file git
```

-    Install Homebrew:

```bash
	/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```
```bash
	echo 'eval "$(/home/linuxbrew/.linuxbrew/bin/brew shellenv)" ' >> ~/.bashrc
```
```bash
	eval "$(/home/linuxbrew/.linuxbrew/bin/brew shellenv)"
```


<br><br>

# Platforms Supported

<br>

## macOS

- macOS 27 (Golden Gate) -- In Theory
- macOS 26 (Tahoe)  -- Tested
- macOS 15 (Sequoia)  -- In Theory
- macOS 14 (Sonoma)  -- In Theory
- macOS 13 (Ventura)  -- Tested

<br>

## Windows x86-64 

- Windows 11  -- Tested
- Windows 10  -- In Theory

<br>

## Linux x86-64 (glibc >= 2.35)

- Ubuntu 22.04+
- Debian 12+
- Linux Mint 21+
- Pop!_OS
- Arch Linux
- Manjaro
- EndeavourOS
- Garuda Linux
- Fedora 36+
- RHEL 10+

<br><br>

# Quick Start

1.  Launch the application
    
2.  Paste a YouTube URL into the input field
    
3.  Choose one of the following options: **Best Audio**, **Best Video**, or **Personalized**
    
4.  Click **Save As** and select the output location and filename
    
5.  Return to the main interface and press **Start** to begin downloading
    
This is the fastest way to download a video or audio file without configuring advanced options


<br><br>

# Usage

1. Launch the application

2. During the initial startup, the application will detect the installation paths of yt‑dlp, ffmpeg, and node.js. If they were installed as described earlier, they should be recognized automatically. Otherwise, you will need to manually locate them

3. Then choose one of the following actions:

	**Add New Download**
	
	-    Paste a YouTube URL into the input field

	-    Proceed as long as the title does not indicate "failed". If it does, you should check whether the link is valid

	-    Choose one of the following options:

			--    Best Audio
      
			--    Best Video
      
			--    Personalized

	-    When "Personalized" is selected, a button along with three input boxes will be shown. Clicking the button will bring up a new page listing all possible itags

			--    You can input either an audio itag alone, an audio itag combined with a video itag, or an audio itag with a *video itag. However, if you enter an audio itag, a video itag, and a *video itag simultaneously, the video itag will be discarded
			
			--    The *video itag refers to a special AI‑upscaled video format, such as 399‑sr

			-- At least one audio itag is required
	
			-- **Note**: Only itags whose acodec is audio‑only or whose vcodec is video‑only are supported.

	-    You may choose whether metadata is included

	-    After all configurations are finished, you can select "Save As". Please note that once "Save As"is chosen, the previous options can no longer be modified. To make changes, go back to the main interface and use "Edit" on the newly added entry

	-    Within the save menu, you may manually specify the save path, filename, and extension. Please note that even though extensions like `.mkv`  and  `.wav` are selectable, the underlying audio or video stream is still the highest‑quality stream offered by YouTube; it is merely wrapped inside the chosen container format

	-    Once you confirm, the interface will return to the main screen, and a download entry will be created

	**Edit/Remove**

	-    When download entries are present on the main interface, any entry that has not yet been downloaded or has already completed downloading (progress at 100%) can be edited or removed

	-    The "Edit" option opens the same interface as "Add New Download", but the URL is locked and cannot be changed, all other parameters remain editable

	-    The "Remove" option deletes the entry from the list

    **Remove All Downloads**

	-    Once you confirm the deletion, all download entries will be erased permanently and cannot be recovered

    **Start**

    -    Once the download begins, a maximum of three items can run concurrently. During the download process, the "Edit" and "Remove" buttons for that item are disabled. The progress bar displays real‑time updates, including both numerical progress and UI animation

    -    If a download entry has previously completed, or if the intended output filename already exists in the destination folder, the item will be skipped and will not be downloaded again

    -    As long as the download process has not finished, additional download entries may be added dynamically. Newly added entries will be incorporated into the active download tasks

    -    The maximum number of concurrent download processes is currently limited to 3. Dynamic adjustment of this limit and user‑defined concurrency settings are not yet supported. More features are coming soon

    -    Temporary files are generated in the destination folder during the download process. It can be resumed by clicking "start" again, provided the temporary files remain intact. The download will continue from the previous progress

    -    Download speed is throttled to avoid excessively rapid transfers that might be flagged by the server

    -    Download speed is partially determined by random factors. If the transfer appears slow, you may stop the download and start it again to obtain a different speed profile

    **Stop**

	-    If you confirm the stop action, all ongoing downloads will be terminated
	-    If no download items exist, the operation has no effect

    **ToolBar->File->Import Data/Export data**

	-    All download items displayed in the UI can be exported as a `.dat` file

	-    Previously exported `.dat` files may also be imported, provided that the current interface contains no active download entries

    **ToolBar->File->Clear Download Status**
 
    -    All completed download entries (those at 100% progress) can be cleared, enabling them to be downloaded again

    -    Tip: To re‑download an individual audio or video item, locate its entry and choose "Edit". Even without modifying any settings, saving the entry again will reset its progress to zero, enabling the item to be downloaded once more

	**ToolBar->File->Quit**

	-    This action is equivalent to clicking the red close icon. A confirmation dialog will be displayed, and upon confirming, the application will terminate immediately.

	**ToolBar->Language**

	-    The application currently supports the following languages:
 
			-- English US (en_US)
   
            -- English UK (en_GB)
      
			-- Spanish (es_ES)
      
			-- French (fr_FR)
      
			-- Italian (it_IT)
        
			-- Portuguese (pt_PT)
        
			-- Simplified Chinese (zh_CN)
      
			-- Traditional Chinese (zh_TW)
   
            -- Chinese LIT (zh_LIT)

	**ToolBar->Preferences**

	-    The paths for the yt‑dlp, ffmpeg, and node.js executables can be viewed in the settings panel, identical to the configuration screen shown during the application's initial launch

	**ToolBar->Help->Help Documentation**

	-    The action navigates directly to the current page

	-    Qt PDF integration will be added in upcoming releases

	**ToolBar->Help->Check for Updates**

	-    This feature checks for new updates on GitHub

	-    At present, updates can only be checked manually. When a new release is detected, a notification dialog will appear, remaining the user to download the latest version and remove the previous one

	-    Automatic update detection and user‑defined preferences for enabling or disabling it will be added in upcoming releases

	**ToolBar->Help->Report Issues**

	-    This action opens the GitHub page. Once you sign in to your GitHub account, you will be automatically redirected to the "Report Issues" page

	-    We welcome all issue reports


<br><br>

# FAQ

### **1. The application cannot be opened on macOS. What should I do?**

This happens because the app is not notarized by Apple. Go to **System Settings → Privacy & Security**, scroll down, and manually allow the application to run

### **2. The app cannot detect ffmpeg, or Node.js. How do I fix this?**

Make sure the tools are installed correctly and available in your system’s PATH. If auto‑detection fails, you can manually locate the executables in **Preferences**

### **3. Why is the download speed slow or inconsistent?**

Download speed is intentionally throttled to avoid triggering server-side rate limits. Speed also varies due to random factors. If the speed is unusually slow, stopping and restarting the download may help

### **4. Why are subtitles not available?**

Subtitle and auto‑generated subtitle downloads are not supported yet. Only “no subtitles” is currently available. Subtitle support will be added in future releases

### **5. Why do some videos not offer 1080p or higher resolutions?**

YouTube often provides high‑resolution video as **video‑only streams**. To download them, you must select a **video itag + audio itag** combination under **Personalized** mode

### 6. What is a “**video itag” (e.g., 399‑sr)?*

These are AI‑upscaled video formats provided by YouTube. They can be selected together with an audio itag to produce a higher‑quality output

### **7. Why does the app skip a download item?**

If the output file already exists or the item has previously completed (100%), it will be skipped automatically. You can reset its progress by opening **Edit** and saving again

### **8. Can I resume an interrupted download?**

Yes. As long as the temporary files remain in the destination folder, clicking **Start** again will resume the download

### **9. Why can’t I edit or remove an item during download?**

Items currently being downloaded are locked to prevent conflicts. You can edit or remove them once the download finishes or is stopped


<br><br>

# Compile

```bash
git clone https://github.com/Rafael-l-m/Youtube-Media-Downloader.git
```
```bash
cmake -S . -B build -G "Ninja"
```
```bash
cmake --build build --config Release
```
```bash
./build/MediaDownloader
```


<br><br>

# License

This project is licensed under the [MIT License](LICENSE)
