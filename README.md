# Qt Timeline Editor (scaffold)

Minimal scaffold for a timeline-based video editor with:
- HTML/CSS/JS UI (Qt WebEngine + QWebChannel)
- C++ core (Qt) with plugin hot-reload via `.dll`
- FFmpeg CLI export (tools/export.ps1)
- Sample `grayscale` plugin

Requirements:
- Qt 6 with WebEngine, WebChannel, Multimedia (MinGW build if using g++)
- CMake 3.24+
- g++ (MinGW) or MSVC
- ffmpeg on PATH

Important:
- Copy `qwebchannel.js` from your Qt installation into `ui/qwebchannel.js`
  (on Windows with Qt installed via online installer, look under Qt/<version>/<compiler>/qml/QtWebChannel/qwebchannel.js)
  Tip: run the helper `tools/install_qwebchannel.ps1` to locate and copy it automatically, e.g.:
  ```powershell
  PS> .\tools\install_qwebchannel.ps1 -qtBase "C:\\Qt"
  ```

Build (PowerShell):
PS> .\tools\build.ps1

Run:
- After build, run the `timeline_editor.exe` from the build folder.
- Drop a media file using the file chooser in the UI and press Play.

To load plugins:
- Put plugin DLLs (built into build/plugins by CMake) into the build plugins folder.
- Call `bridge.loadPlugin("path-to-dll")` from JS or extend UI to call it.
  Plugins placed in `plugins/` next to the executable will be auto-loaded at startup.

