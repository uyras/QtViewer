#!/bin/bash

iconsPath="/usr/share/icons/hicolor"
desktopPath="/usr/share/applications" 
installPath="/opt/QtViewer"
linkPath="/usr/bin/QtViewer"
mimePath="/usr/share/mime"

#Check user
if [[ $EUID -ne 0 ]]; then
    echo "This script must be run as root"
    exit 1
fi

#Remove old version
if [ -d $installPath ]; then
    $installPath'/uninstall.sh' > /dev/null   
fi
    
echo "QtViewer install..."
mkdir -v $installPath
cp -v QtViewer $installPath
cp -v uninstall.sh $installPath
cp -rv examples/ $installPath
cp -v QtViewer.desktop $desktopPath
ln -sv $installPath'/QtViewer' $linkPath
chmod a+x $linkPath
echo "---"
cp -v mfsys-mime.xml $mimePath'/packages'
update-mime-database $mimePath
echo "---"
cp -v icons/32x32/qtviewer.png $iconsPath'/32x32/apps'
cp -v icons/48x48/qtviewer.png $iconsPath'/48x48/apps'
cp -v icons/64x64/qtviewer.png $iconsPath'/64x64/apps'
cp -v icons/128x128/qtviewer.png $iconsPath'/128x128/apps'
cp -v icons/256x256/qtviewer.png $iconsPath'/256x256/apps'
gtk-update-icon-cache /usr/share/icons/hicolor
update-desktop-database

echo "...finished!" 
