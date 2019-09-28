# $env:GITHUB_WORKSPACE = 'C:\Users\qbgee\Documents\Code\Aurora\Prototyping'

Set-Location $env:GITHUB_WORKSPACE
# $installer_url = 'https://download.qt.io/archive/online_installers/3.1/qt-unified-windows-x86-3.1.1-online.exe'
$installer_url = 'https://download.qt.io/official_releases/qt/5.13/5.13.1/qt-opensource-windows-x86-5.13.1.exe'
Invoke-WebRequest -Uri $installer_url -OutFile "qt-install.exe"
.\qt-install.exe --script .\actions\installer-noninteractive-windows.qs --platform minimal --verbose