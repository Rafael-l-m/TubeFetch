# TubeFetch

<br>

**TubeFetch** is a lightweight download tool for batch downloading YouTube audio and video. Users can add multiple
download tasks at once and independently select the content, format, and quality for each task. The application
interface is designed to be lightweight and intuitive, helping users quickly complete batch download operations.


<br><br>


# Table of Contents
- [Important Notes](#important-notes)
- [Installation](#installation)
- [Running](#running)
- [Pre-requisites](#pre-requisites)
- [Port Check](#port-check)
- [Platforms Supported](#platforms-supported)
- [Quick Start](#quick-start)
- [Usage](#usage)
- [FAQ](#frequently-asked-questions)
- [Compile](#compile)
- [License](#license)


<br><br>


# Important Notes

- This application depends on several external tools, including yt-dlp, ffmpeg, Node.js, and bgutil-pot-
ytdlp-provider. Some of these tools do not support 32-bit environments, so the application cannot run
properly on 32-bit systems. Due to this limitation, the application supports 64-bit platforms only.

- Except for Node.js, all other tools that this application depends on are open-source software. Their source
code and related resources are available on GitHub.

- Because this application does not bundle the dependency tools mentioned above or directly integrate their
code into the application, the application itself is distributed under the MIT open-source license. Users
may freely redistribute or use the application in closed-source projects. If this application or any part of
its content is used in another project, the original author must be credited as required by the MIT License.

- None of the systems labeled "Theoretically supported" have been tested in practice. When running the
application on these systems, you may encounter functional issues or instability. Use the application with
caution. If the application crashes or exhibits other unexpected behavior, you can report it by submitting
an issue in the project repository.

- Due to the application’s operating model and the limitations of mobile platforms, a mobile version is not
currently provided. Mobile operating systems impose numerous restrictions on permission management,
background execution, file system access, and the use of external tools, making it impossible to meet the
application’s core functional requirements.

- Because the author has not obtained code-signing certificates for any platform, additional steps may be
required during installation or uninstallation when running this application on platforms other than Linux.
Follow the system prompts for the platform you are using.

- On Windows, a shortcut to the corresponding uninstaller is automatically created after the application
is installed. At present, this is the only platform that provides an automated uninstallation method. On
other systems, such as macOS and Linux, the application must be uninstalled manually according to each
platform’s mechanisms.

- After the initial installation is complete, the application must be launched twice before it can run normally.
During the first launch, the system will automatically download and configure the required external tools.
Once the required dependencies have been installed, the second launch will enter the normal workflow.

- After installing a new version (v3.0.0 and earlier), user data from previous versions will be automatically
removed on the first launch. To prevent the loss of important data, back up the relevant data before
upgrading.

- After navigating to the second page in the same window, swipe left (the application supports simulated
touch input) or press the `ESC` key to return to the main interface.

- Some ways of using this application may violate YouTube’s Terms of Service. Users should therefore
manage their download frequency appropriately to reduce the risk of their IP address being temporarily
or permanently restricted. If errors such as 403, 404, 429, or "Too Many Requests" appear frequently in
the terminal-style area, the corresponding requests may have been restricted by YouTube’s servers. In
this case, it is recommended to stop downloading and refrain from using the application for a period of 
time. Avoid making a large number of requests within a short period. Please note that specific restrictions
depend on YouTube’s service policies, and this application cannot guarantee that such restrictions will
be avoided.

- Starting with v4.0.0, the author is committed to maintaining both forward and backward compatibility.
If you are still using an earlier alpha or beta version, manually delete the user data and user configuration
from the old version to avoid compatibility issues. For detailed cleanup instructions, see the“Uninstal-
lation and Residual Files”section at the end of the documentation.


<br><br>


# Installation

The latest version can be installed from https://github.com/Rafael-l-m/TubeFetch/releases

Except for Linux, this application provides two installation methods: an installer and a compressed archive.
The installation methods for each platform are as follows:

### macOS:

- A `.dmg` installer is provided. Double-click the installer package to open it, then drag the application to the "Applications" folder to use it.

- An `.app.zip` archive is also provided. After extracting it, you can run the application directly without going through an installation process.


### Windows:

- An `.exe` installer is provided. After completing the installation through the setup wizard, the system will automatically create shortcuts to the application and uninstaller.

- A `.zip` archive is also provided. After extracting it, open the corresponding folder and double-click `TubeFetch.exe` to run the application. This method does not include an uninstaller. To uninstall the application, simply delete the entire folder.


### Linux:

- Only a single-file executable `.AppImage` version is provided. Because there are many Linux distributions and significant differences between them, providing a universal installer could result in numerous compatibility issues. Therefore, no traditional installer is provided. Users can run the `.AppImage` file directly.


<br><br>


# Running

Cause this application is not code-signed, different platforms may display security warnings or prevent the
application from running the first time it is launched. Follow the steps below according to the platform you
are using:

### macOS:

- Open "Settings".

- Go to "Privacy & Security".

- Locate the blocked application in the "Security" section. 

- Click "Allow to Run".

- Click "Run Anyway" again to complete the launch.

### Windows 11 (older versions), Windows 10, and earlier versions:

- Click "More Options". 

- Select "Run Anyway" to continue using the application.

### Windows 11 (recent versions):

- Open "Windows Security".

- Go to "App & browser control".

- Locate "Smart App Control".

- Open "Smart App Control settings".

- Set it to "Off".

Linux users only need to grant execute permission to the `.AppImage` file to run it directly:

```bash
chmod +x TubeFetch.AppImage
```


<br><br>


# Pre-requisites

Before running the app, make sure you have the following:

-    [ffmpeg](https://ffmpeg.org/) installed
-    [node.js](https://nodejs.org/) installed
-    A stable internet connection

You can download these tools manually and add them to the system’s PATH environment variable so that the system can locate them correctly. If they have not been configured in advance, the application will provide an option to manually select the tool paths during the first launch to complete the dependency configuration.

<br>

### For macOS Users (Apple Sillicon)

**Homebrew** is the recommended way to install it

-    Install Xcode Command Line Tools:

```bash
        xcode-select --install
```

-    Install Homebrew:

```bash
        /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

-    Install required tools:

```bash
        brew update
```
```bash
        brew upgrade
```
```bash
        brew install ffmpeg
```
```bash
        brew install node
```

<br>

### For macOS Users (Intel Mac)

**MacPorts** is the recommended way to install it

-	Install Xcode Command Line Tools:

```bash
        xcode-select --install
```

-    Install [MacPorts](https://www.macports.org/)

-    Install required tools:

```bash
        sudo port selfupdate
```
```bash
        sudo port upgrade outdated
```
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

### For Windows Users (x86-64)

**Scoop** is the recommended way to install it

-    Install Scoop:

```Powershell
        Set-ExecutionPolicy RemoteSigned -Scope CurrentUser
```
```Powershell
        iwr -useb get.scoop.sh | iex
```

-    Install required tools:

```bash
        scoop update
```
```bash
        scoop update *
```
```bash
        scoop install ffmpeg
```
```bash
        scoop install nodejs-lts
```

<br>

### For Linux Users (x86-64)

**Homebrew** is the recommended way to install it

- Install dependencies:

- - Ubuntu / Debian:
```bash
        sudo apt install -y build-essential procps curl file git
```


- - Fedora / CentOS / RHEL:
```bash
        sudo dnf groupinstall "Development Tools" && sudo dnf install procps-ng curl file git
```


- - Arch:
```bash
        sudo pacman -S base-devel procps-ng curl file git
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

-    Install required tools:

```bash
        brew update
```
```bash
        brew upgrade
```
```bash
        brew install ffmpeg
```
```bash
        brew install node
```


<br><br>


# Port Check

Before running the application, make sure that local port 4416 is available. This port is reserved for bgutil-pot-ytdlp-provider and is used to support yt-dlp token parsing and authentication. If the port is already in use by another process, token-related functions will not work properly. To ensure that the port is available, users can perform the appropriate port checks and process termination operations for their platform.

### macOS:

- Check whether a port is in use using lsof
```bash
        lsof -i :4416
```

- Force-Kill the process with specified PID (e.g., 1234)
```bash
        kill -9 1234
```

### Windows:

- Check whether a port is in use using Get-NetTCPConnection (PowerShell)
```Powershell
        Get-NetTCPConnection -LocalPort 4416
```

- Force-Kill the process with the specified PID (e.g., 1234) (PowerShell)
```Powershell
        Stop-Process -Id 1234 -Force
```

### Linux:

- Check whether a port is in use using ss
```bash
        ss -tulnp | grep 4416
```

- Force-Kill the process with the specified PID (e.g., 1234)
```bash
        kill -9 1234
```


<br><br>


# Platforms Supported

<br>

### macOS

- macOS 27 (Golden Gate) -- Tested (aarch64 & x86-64)
- macOS 26 (Tahoe)  -- Tested (aarch64 & x86-64)
- macOS 15 (Sequoia)  -- Theoretically Supported
- macOS 14 (Sonoma)  -- Theoretically Supported
- macOS 13 (Ventura)  -- Tested (x86-64)

<br>

### Windows x86-64 

- Windows 11  -- Tested (x86-64)
- Windows 10  -- Tested (x86-64)

<br>

### Linux x86-64 (glibc >= 2.35)

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

- Click `Add New Download` on the main interface.

- Enter or paste a valid URL.

- Select the download type: `Best Audio` / `Best Video` / `Personalized`. 

- After selecting the save path, click `Continue` to return to the main page.

- Click `Start` button on the main interface, then wait for the download to complete.


<br><br>


# Usage

1. First Launch Interface:
    - On the first launch, the system will open the external tools detection page first. For users
who downloaded the tools according to the tutorial, the system can automatically detect their
installation locations and configure the paths. Users who downloaded the tools manually must
select or enter the actual installation paths of the external tools on this page to ensure that
the system can invoke the corresponding functions correctly. (Please note that tools installed
through other package managers may work normally when invoked from the terminal, while
their actual paths may be symbolic links pointing to the actual executable files. These symbolic
links may cause the program to incorrectly identify the file. Even if the system reports that the
selected file is not executable, the path in the input field will not be cleared, so you can simply
click "Confirm").
    -  After the user confirms the relevant settings, the system will load the main interface. If the two
required components, yt-dlp and bgutil-pot-ytdlp-provider, are not detected during startup, the
system will automatically display a download dialog prompting the user to install them. These
components are required dependencies for the application to function properly. If the user
declines the installation or the download cannot be completed, the system will automatically
exit the application to ensure the integrity and stability of the runtime environment.
    - Once the required components have been downloaded, the system will automatically open the
main interface.

2. Main Interface:
    - The main interface uses a top-to-bottom sectioned layout. The top section contains two function
buttons for common operations (start download / end download). Below it is the download list,
which displays the current download tasks and their status in real time. The middle section
provides a terminal-like interface for displaying system output and execution logs. The bottom section 
contains two main action buttons for quick access to core functions (add download /
delete all downloads).
    - The bottom of the interface provides two action buttons: `Add New Download` and `Remove All Downloads`. 
The `Remove All Downloads` button is enabled only when download tasks exist. When the
download list is empty, the button is automatically disabled to prevent ineffective operations.
The `Add New Download` button remains enabled at all times, allowing users to open the New
Download interface and create a new download task at any time.
    - The terminal-style area displays debugging information, standard output, error messages, warnings, 
and general execution logs while the system is running. A green arrow button at the bottom of the 
area allows users to enter commands for a terminal-like experience. This feature
has been available since v2 and is currently used primarily to display system output; full command 
interaction is not yet supported. This module will be further improved in v4.1.0 and will
gradually support full command interaction. 
    - The display area shows all current download items in a list. Each download item provides four
buttons for editing, deleting, starting, and ending the task, respectively. Operations such as
editing and deleting remain available while a download is in progress. However, once a download 
has started, any subsequent changes to the task configuration will not affect the download
currently in progress. In other words, the new configuration displayed in the interface may
differ from the configuration actually being used for the download. Please keep this in mind.
    - Of the two buttons at the top of the interface, the `Stop` button is enabled only when the system
is in download mode and is used to terminate download tasks currently in progress. Download
mode is activated when the user clicks `Start` and is divided into two mutually exclusive modes:
        - Single Download mode: the user must click the download button corresponding to a
specific download item, and the system will download only that item.
        - Download All mode: after clicking the download button at the top, the system downloads all download items sequentially.
    - In Single Download mode, the user can click the `Stop` button for a specific download item.
The system will determine whether the task is currently downloading and, if so, stop it immediately. 
The user can also use the `Stop` button at the top to stop the current download.
In Download All mode, the system does not support stopping an individual download item. The
user must use the `Stop` button to stop all download tasks currently in progress.

3. New Download Interface: consists of three main sections: the Link section, the Configuration section, and the Output section.
    - The Link section is divided into two rows. The first row contains an editable input field
for entering the resource URL, supporting both manual entry and pasting, as well as a
button for parsing the link information. After approximately three seconds without input,
the field loses focus. Once focus is lost, the system automatically removes unnecessary
portions of the resource URL. After the field loses focus, you can click the `Search Info` button,
and the system will automatically attempt to load and parse the URL. During the first
parsing attempt, the retrieved information is saved to the database and remains valid for
31 days. During this 31-day period, the resource information associated with the URL will be retrieved from the database first. If you believe the link information is outdated,
select `Clear Cache` from the menu bar to remove the corresponding information stored
in the database. (This does not affect saved download items; it only removes the cached
information associated with the URL). You can also click the button in the second row to
view real-time resource information in a separate window. Please note that this feature
sends real-time requests to YouTube servers. Manage the access frequency and operation
order appropriately to reduce the risk of your IP address being restricted. The second row
also contains a non-editable input field that displays the title associated with the resource
URL. You can proceed with the remaining configuration only when the title is not `Error`.
If `Error` is displayed, check whether the URL is correct. (Downloading YouTube videos
that require payment to watch is currently not supported.)
    - After the link configuration is complete, the system will enter the download configuration
section. There are three configuration modes: `Best Video`, `Best Audio`, and `Personalized`. When
the user switches to `Personalized` mode, additional dropdown options will be displayed.
    - The dropdown options in the configuration area include `audio codecs`, `video codecs`, and
`non-DASH stream codecs`. `Audio codecs`, `video codecs`, and `non-DASH stream codecs` are
mutually exclusive: users must choose between `Non-DASH` and `Audio / Video Codec`,
although they may also select only an audio codec. On the current YouTube platform,
non-DASH streams are gradually becoming less common and are relatively old stream
formats. The `metadata` option can be toggled freely, allowing users to choose whether to
include metadata in downloaded content. If the parsing results contain manually uploaded
subtitles, a `subtitle` option will be displayed; otherwise, the subtitle option will not be
provided by default. (Support for machine-translated subtitles provided by the platform
will be gradually added in v4.1.0.)
    - In the output section, users can set the output path for downloaded files. Click the path
input field to select the destination directory. Linux users should pay particular attention
to the following: the system does not automatically append the container extension to the
output file, so the complete filename must be specified manually. For example, if you
want the output to be in MP3 format, the filename must explicitly be entered as test.mp3
rather than simply test. Although the file filter in the interface displays MP3, omitting the
file extension may cause errors during the download or cause the task to fail.
    - After clicking `Continue`, the system will attempt to write the current configuration to
the database. If the data is written successfully, the interface will automatically return to
the main page. If the operation fails, the system will display a message explaining the
specific reason. Common causes include a duplicate output path or a conflict with an
existing record.

4. Menu Bar:
    - File:
        - The `Export Data` feature is used to export all current download items, including task
configuration, progress status, and other related information, to a file. Users can
choose `.dat` or `.data` as the output file extension.
        - The `Import Data` feature is used to restore the download task list and its associated
status information from an external file. To ensure data consistency and prevent data
conflicts, the system requires all existing download items to be cleared before importing. 
When the download list is empty, the user can perform the import operation
to load task records from `.dat` or `.data` files into the system. If download items still
exist, the import feature will remain disabled, and the user must clear the download
list before proceeding.
        - The `Export Outputs` allows all information displayed in the terminal-style area to be
exported as a plain text file `.txt` or another supported file format for saving, viewing,
or further analysis.
        - The `Clear Download Status` feature resets the status indicators of download items.
After this operation is performed, all tasks currently marked as `Downloaded` will
be reset to `Not Downloaded`, allowing users to download or manage them again.
This operation only changes the task status and does not affect any output files that
have already been generated. All downloaded files will remain in their original output
paths and will not be deleted or overwritten.
        - The `Clear Cache` feature deletes all stored resource link information.
	- Click `Settings` to open the Settings page directly. (On macOS, the location of this
option may vary depending on the selected language. If you cannot find `Settings` in
the `File` menu, click `TubeFetch` in the menu bar to locate the corresponding option).
        - The `Exit` feature safely closes the application. After this operation is performed, the
system will immediately terminate the current process and return to the operating
system. Exiting does not affect existing download items, configuration data, or files
that have already been generated; all data remains unchanged. (On macOS, the location 
of this option may vary depending on the selected language. If you cannot find 
`Settings` in the `File` menu, click `TubeFetch` in the menu bar to locate the corresponding option).
        - Language
	
            - The system supports multiple languages, allowing users to dynamically switch the interface language while the application is running without restarting the application.
	    - The following languages are supported:
	        - British English.
		- American English.
		- Simplified Chinese.
		- Spanish.
	    - Starting with v4.1.0, support for the following languages will be introduced gradually:
	        - Portuguese.
		- French.
		- Italian.
		- German.
		- Russian.
		- Ukrainian.
		- Korean.
		- Japanese.
		- Thai.
		- Traditional Chinese.
		- Classical Chinese.
		- Arabic.
		- Norwegian.
		- Swedish.
		- Finnish.
	- Help:
	    - Visit the project repository.
	    - Help Documentation.
	    - Check for Updates.
	    - Report Issues.


<br><br>


# Frequently Asked Questions

### 1. Why doesn’t the application start properly on macOS?

Because the application is not code-signed or notarized, macOS may prevent it from launching the first
time it is run. Go to `Settings → Privacy & Security → Security`, manually allow the application to run,
and select `Open Anyway` to complete the initial authorization.

### 2. Why can’t the uninstaller run on Windows?

Because the application is not code-signed or notarized, and newer versions of Windows 11 include the
`Smart App Control` feature, Windows may prevent the application from running. Go to
`Windows Security → App & browser control → Smart App Control`, turn off this feature, and then try launching
the application again.

### 3. Why does the loading animation remain visible while the application is starting?

If the `Startup Check` feature is enabled, the application will automatically update yt-dlp every time it
starts, so a loading animation may be displayed during startup. In particular, on macOS, the system may
take a considerable amount of time to perform security and integrity checks when invoking yt-dlp, which
can cause the loading animation to remain visible for an extended period. This is normal behavior; please
wait patiently. If the wait is excessively long, try closing and restarting the application.

### 4. Why do errors occur during downloading after selecting manually specified audio codecs, video codecs, or non-DASH streams?

Since 2026, YouTube has continued to modify its platform structure and data interfaces, requiring frequent 
yt-dlp updates to maintain parsing capabilities. Therefore, starting with v2.1.0, this application has
used the faster-updating yt-dlp nightly version. This version belongs to an experimental branch, allowing
it to receive the latest updates earlier and parse more formats.
However, because this version is experimental, although it provides greater adaptability when parsing
YouTube content, it cannot guarantee that all formats will be parsed correctly every time, especially new
or unstable formats that are not yet supported by the stable release.

### 5. Why does the same link return different encoding formats in different download attempts?

The previous question has already been explained fully and accurately in the preceding section.

### 6. Why do manually specified audio, video codecs, or non-DASH streams differ from the information shown after clicking "Show Codecs"?

The information displayed under `Show Codecs` is retrieved from YouTube in real time. If fewer
formats are displayed than are actually available, it may be because the YouTube server did not return
complete information this time. If more formats are displayed than in previous attempts, it may be because
yt-dlp’s ability to parse YouTube formats has improved through subsequent updates. To avoid sending
repeated requests to YouTube’s servers, resource URLs are saved to the database when they are first
retrieved and remain valid for one month. Therefore, the resource may have been updated sometime
within the past month. To obtain the latest information, select `Menu Bar → File → Clear Cache`, then
return to the New Download interface and click `Search Information` again.

### 7. Why can’t download, stop, or clear operations be performed?

If the current interface does not display any download items, download, stop, clear, and related operations cannot be performed. These buttons are enabled only when there are manageable download tasks.

### 8. Why is the download speed slow?

The speed of each download task is randomly selected within a predefined range. This mechanism is
intended to make download speed patterns less identifiable and reduce the possibility of being classified
as abnormal behavior by YouTube’s servers. In general, the system selects a relatively low speed range
to improve the stability and success rate of the download process.

### 9. Can incomplete download tasks be resumed?

During the execution of a download task, the system generates the corresponding temporary files for that
task. If the temporary files have not been deleted and the configuration of the download item has not
changed, the next time the download is started, the program will automatically resume from the existing
temporary files instead of starting over. This mechanism reduces redundant downloads and improves the
efficiency of resuming download tasks.

### 10. Why do numerous .vtt files appear after downloading?
If subtitles are downloaded along with the current download, the system will generate `.vtt` files. These
files are not automatically deleted and must be removed manually by the user. Starting with v4.1.0, the
storage location of these download-related files will be adjusted, and the system will gradually manage
these files automatically.


<br><br>


# Compile

This application is compiled using Qt Creator, the official integrated development environment provided by Qt. The Qt version used must be 6.10 or later.


<br><br>


# License

This project is licensed under the [MIT License](LICENSE)


<br><br><br>


# More details please refer to [HelpDoc](https://rafael-l-m.github.io/TubeFetch/).
