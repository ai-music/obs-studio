# Exit if something fails
set -e

rm -rf ./OSSIALIVE.app

mkdir OSSIALIVE.app
mkdir OSSIALIVE.app/Contents
mkdir OSSIALIVE.app/Contents/MacOS
mkdir OSSIALIVE.app/Contents/PlugIns
mkdir OSSIALIVE.app/Contents/Resources

cp -R rundir/RelWithDebInfo/bin/ ./OSSIALIVE.app/Contents/MacOS
cp -R rundir/RelWithDebInfo/data ./OSSIALIVE.app/Contents/Resources
cp ../CI/install/osx/OSSIALIVE.icns ./OSSIALIVE.app/Contents/Resources
cp -R rundir/RelWithDebInfo/obs-plugins/ ./OSSIALIVE.app/Contents/PlugIns
cp ../CI/install/osx/Info.plist ./OSSIALIVE.app/Contents

../CI/install/osx/dylibBundler -b -cd -d ./OSSIALIVE.app/Contents/Frameworks -p @executable_path/../Frameworks/ \
-s ./OSSIALIVE.app/Contents/MacOS \
-s /usr/local/opt/mbedtls/lib/ \
-x ./OSSIALIVE.app/Contents/PlugIns/coreaudio-encoder.so \
-x ./OSSIALIVE.app/Contents/PlugIns/decklink-ouput-ui.so \
-x ./OSSIALIVE.app/Contents/PlugIns/frontend-tools.so \
-x ./OSSIALIVE.app/Contents/PlugIns/image-source.so \
-x ./OSSIALIVE.app/Contents/PlugIns/linux-jack.so \
-x ./OSSIALIVE.app/Contents/PlugIns/mac-avcapture.so \
-x ./OSSIALIVE.app/Contents/PlugIns/mac-capture.so \
-x ./OSSIALIVE.app/Contents/PlugIns/mac-decklink.so \
-x ./OSSIALIVE.app/Contents/PlugIns/mac-syphon.so \
-x ./OSSIALIVE.app/Contents/PlugIns/mac-vth264.so \
-x ./OSSIALIVE.app/Contents/PlugIns/obs-ffmpeg.so \
-x ./OSSIALIVE.app/Contents/PlugIns/obs-filters.so \
-x ./OSSIALIVE.app/Contents/PlugIns/obs-transitions.so \
-x ./OSSIALIVE.app/Contents/PlugIns/obs-vst.so \
-x ./OSSIALIVE.app/Contents/PlugIns/rtmp-services.so \
-x ./OSSIALIVE.app/Contents/MacOS/OSSIALIVE \
-x ./OSSIALIVE.app/Contents/MacOS/obs-ffmpeg-mux \
-x ./OSSIALIVE.app/Contents/PlugIns/obs-x264.so \
-x ./OSSIALIVE.app/Contents/PlugIns/text-freetype2.so \
-x ./OSSIALIVE.app/Contents/PlugIns/obs-libfdk.so
# -x ./OSSIALIVE.app/Contents/PlugIns/obs-outputs.so \
#- -x ./OSSIALIVE.app/Contents/PlugIns/obs-browser.so \
#- -x ./OSSIALIVE.app/Contents/PlugIns/obs-browser-page \
#- -x ./OSSIALIVE.app/Contents/MacOS/obslua.so \
#- -x ./OSSIALIVE.app/Contents/MacOS/_obspython.so \

echo "Doing MacDeployQt"
/usr/local/Cellar/qt/5.14.1/bin/macdeployqt ./OSSIALIVE.app

echo "Moving libobs-opengl.sh"
mv ./OSSIALIVE.app/Contents/MacOS/libobs-opengl.so ./OSSIALIVE.app/Contents/Frameworks

echo "Removing QT Network framework"
rm -f -r ./OSSIALIVE.app/Contents/Frameworks/QtNetwork.framework

echo "Inserting QT Network framework"
# put qt network in here becasuse streamdeck uses it
cp -R /usr/local/opt/qt/lib/QtNetwork.framework ./OSSIALIVE.app/Contents/Frameworks
chmod -R +w ./OSSIALIVE.app/Contents/Frameworks/QtNetwork.framework
rm -r ./OSSIALIVE.app/Contents/Frameworks/QtNetwork.framework/Headers
rm -r ./OSSIALIVE.app/Contents/Frameworks/QtNetwork.framework/Versions/5/Headers/
chmod 644 ./OSSIALIVE.app/Contents/Frameworks/QtNetwork.framework/Versions/5/Resources/Info.plist
install_name_tool -id @executable_path/../Frameworks/QtNetwork.framework/Versions/5/QtNetwork ./OSSIALIVE.app/Contents/Frameworks/QtNetwork.framework/Versions/5/QtNetwork
install_name_tool -change /usr/local/Cellar/qt/5.14.1/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore ./OSSIALIVE.app/Contents/Frameworks/QtNetwork.framework/Versions/5/QtNetwork

echo "decklink ui qt"
# decklink ui qt
install_name_tool -change /usr/local/opt/qt/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui ./OSSIALIVE.app/Contents/PlugIns/decklink-ouput-ui.so
install_name_tool -change /usr/local/opt/qt/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore ./OSSIALIVE.app/Contents/PlugIns/decklink-ouput-ui.so
install_name_tool -change /usr/local/opt/qt/lib/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets ./OSSIALIVE.app/Contents/PlugIns/decklink-ouput-ui.so

echo "frontend tools qt"
# frontend tools qt
install_name_tool -change /usr/local/opt/qt/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui ./OSSIALIVE.app/Contents/PlugIns/frontend-tools.so
install_name_tool -change /usr/local/opt/qt/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore ./OSSIALIVE.app/Contents/PlugIns/frontend-tools.so
install_name_tool -change /usr/local/opt/qt/lib/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets ./OSSIALIVE.app/Contents/PlugIns/frontend-tools.so

echo "vst qt"
# vst qt
install_name_tool -change /usr/local/opt/qt/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui ./OSSIALIVE.app/Contents/PlugIns/obs-vst.so
install_name_tool -change /usr/local/opt/qt/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore ./OSSIALIVE.app/Contents/PlugIns/obs-vst.so
install_name_tool -change /usr/local/opt/qt/lib/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets ./OSSIALIVE.app/Contents/PlugIns/obs-vst.so
install_name_tool -change /usr/local/opt/qt/lib/QtMacExtras.framework/Versions/5/QtMacExtras @executable_path/../Frameworks/QtMacExtras.framework/Versions/5/QtMacExtras ./OSSIALIVE.app/Contents/PlugIns/obs-vst.so

echo "Done packageApp.sh"
