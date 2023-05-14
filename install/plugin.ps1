if (Test-Path "plugin") {Remove-Item -Recurse -Force "plugin"}
if (Test-Path "arduino-esp8266littlefs-plugin.zip") {Remove-Item -Force "arduino-esp8266littlefs-plugin.zip"}
$latestRelease = Invoke-RestMethod -Uri "https://api.github.com/repos/earlephilhower/arduino-esp8266littlefs-plugin/releases/latest"
$browserDownloadUrl = ($latestRelease.assets | Where-Object { $_.browser_download_url -like "*.zip" }).browser_download_url
Invoke-WebRequest -Uri $browserDownloadUrl -OutFile "plugin.zip"
New-Item -ItemType Directory -Force -Path "plugin"
Expand-Archive -Path "plugin.zip" -DestinationPath "plugin"
Remove-Item -Path "plugin.zip"

$toolsFolder = "$env:USERPROFILE\Documents\Arduino\tools"
if (-not (Test-Path $toolsFolder)) {New-Item -ItemType Directory -Force -Path $toolsFolder}
Copy-Item -Path ".\plugin\*" -Destination "$env:USERPROFILE\Documents\Arduino\tools" -Recurse -Force

