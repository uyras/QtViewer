#!/bin/bash

iconsPath="/usr/share/icons/hicolor"
desktopPath="/usr/share/applications" 
installPath="/opt/QtViewer"
linkPath="/usr/bin/QtViewer"
mimePath="/usr/share/mime"
echo "QtViewer uninstall"

#Check user
if [[ $EUID -ne 0 ]]; then
    echo "This script must be run as root"
    exit 1
fi

rm -v $iconsPath'/32x32/apps/qtviewer.png'
rm -v $iconsPath'/48x48/apps/qtviewer.png'
rm -v $iconsPath'/64x64/apps/qtviewer.png'
rm -v $iconsPath'/128x128/apps/qtviewer.png'
rm -v $iconsPath'/256x256/apps/qtviewer.png'
rm -v $desktopPath'/QtViewer.desktop'
rm -v $mimePath'/packages/mfsys-mime.xml'
update-desktop-database
update-mime-database $mimePath

#Remove app folder
echo "---"
rm -rfv $installPath
rm -v  $linkPath

echo "Uninstall finished!" 
