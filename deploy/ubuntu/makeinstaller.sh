 #!/bin/bash

 if [[ -z "$1" ]]; then
    echo "Please, provide path to current QtViewer executable file"
    exit 1
fi

 if [[ ! -x "$1" ]]; then
    echo "File $1 is not exists or not executable"
    exit 1
fi

echo "Making installer..."

installerPath="../../installer/QtViewer"
iconPath="../../icons"
examplePath="../../examples"

#Remove old folder
if [ -d $installerPath ]; then
    rm -r -v $installerPath   
fi

mkdir -v $installerPath
cp -v install.sh $installerPath
cp -v uninstall.sh $installerPath
cp -v QtViewer.desktop $installerPath
cp -v mfsys-mime.xml $installerPath
cp -rv $examplePath $installerPath
cp -v $1 $installerPath
mkdir -v $installerPath'/icons'
mkdir -v $installerPath'/icons/32x32'
mkdir -v $installerPath'/icons/48x48'
mkdir -v $installerPath'/icons/64x64'
mkdir -v $installerPath'/icons/128x128'
mkdir -v $installerPath'/icons/256x256'

cp -v $iconPath'/icon_32.png' $installerPath'/icons/32x32/qtviewer.png'
cp -v $iconPath'/icon_48.png' $installerPath'/icons/48x48/qtviewer.png'
cp -v $iconPath'/icon_64.png' $installerPath'/icons/64x64/qtviewer.png'
cp -v $iconPath'/icon_128.png' $installerPath'/icons/128x128/qtviewer.png'
cp -v $iconPath'/icon_256.png' $installerPath'/icons/256x256/qtviewer.png'

tar -cvzf $installerPath'/../QtViewer.tar.gz' $installerPath

echo "...finished!" 