# TubeFetch

<br>

A multi-threaded **YouTube media downloader** with flexible format control. The application supports concurrent downloads and allows users to either:

-    Automatically fetch the highest‑quality audio or video stream.
    
-    Select a specific **itag** for custom output formats.
    
Ideal for users who want both convenience and fine‑grained control over YouTube media extraction.

Warning: On the initial startup, the app may require two launches to function correctly. The first launch completes the download of all necessary components, and the second launch will operate as expected. A spinning loading indicator during the download is normal, particularly for macOS users.

**Warning for macOS users**: If macOS displays a message stating that the application “cannot be opened,” users may enable it manually by navigating to **System Settings → Privacy & Security** and allowing the app. This warning appears because the application has not undergone Apple’s notarization process, which requires enrollment in the paid Apple Developer Program.

**Warning for Windows users**: when downloading the installer from Github, Windows may show a **SmartScreen** such as: "Unknown publisher", "This app might put your PC at risk". This happens because the installer is **not code-signed**. Unsigned applications are common in open-source projects, and this warning does not mean the file is harmful.

**Warning for Windows 11 users**: For users on the latest version of Windows 11, if Smart App Control is enabled by default, you will need to turn it off; otherwise, the uninstaller may fail to launch properly. The steps are: Windows Security → App & browser control → Smart App Control → Smart App Control settings → Off.

**This application has undergone basic testing, but it has not yet been extensively tested on Windows or Linux. Feedback is highly appreciated — any issues you report will play an important role in improving and shaping the future of this project. Thank you very much for your support.**

<br>

## For more details, please refer to [HelpDoc TubeFetch](https://rafael-l-m.github.io/TubeFetch/)


<br><br>


# Download

You can download the latest version from the Releases page: https://github.com/Rafael-l-m/TubeFetch/releases

