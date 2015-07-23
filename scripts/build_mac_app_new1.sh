devpath=/Users/gudula/Documents/workspace/bil/release
appdir=$devpath/Biluna.app
qtdir=/Users/gudula/Qt5.3.1/5.3/clang_64

echo "Creating folders for Biluna"
mkdir $appdir
mkdir $appdir/Contents
mkdir $appdir/Contents/Frameworks
mkdir $appdir/Contents/Resources
mkdir $appdir/Contents/MacOs
mkdir $appdir/Contents/MacOs/plugins

echo "Copy info files and icon for Biluna"
cp -v -R biluna.icns $appdir/Contents/Resources/biluna.icns
cp -v -R PkgInfo $appdir/Contents/PkgInfo
cp -v -R Info.plist $appdir/Contents/Info.plist

echo "Copy Biluna exe, plugins and libs"
# cp -v -R $devpath/Assistant $appdir/Contents/MacOs/Assistant
cp -v -R $devpath/Biluna $appdir/Contents/MacOs/Biluna
cp -v -R $devpath/libdb.dylib $appdir/Contents/MacOs/libdb.dylib
cp -v -R $devpath/libdb.1.dylib $appdir/Contents/MacOs/libdb.1.dylib
cp -v -R $devpath/libdb.1.0.dylib $appdir/Contents/MacOs/libdb.1.0.dylib
cp -v -R $devpath/libdb.1.0.0.dylib $appdir/Contents/MacOs/libdb.1.0.0.dylib
cp -v -R $devpath/plugins/libacc_actionplugin.dylib $appdir/Contents/MacOs/plugins/libacc_actionplugin.dylib
cp -v -R $devpath/plugins/libcrm_actionplugin.dylib $appdir/Contents/MacOs/plugins/libcrm_actionplugin.dylib
cp -v -R $devpath/plugins/libpeng_actionplugin.dylib $appdir/Contents/MacOs/plugins/libpeng_actionplugin.dylib
cp -v -R $devpath/plugins/libsrm_actionplugin.dylib $appdir/Contents/MacOs/plugins/libsrm_actionplugin.dylib

echo "Change library paths for Biluna and actionplugins"
install_name_tool -change libdb.1.dylib @executable_path/libdb.1.dylib  $appdir/Contents/MacOs/Biluna
install_name_tool -change libdb.1.dylib @executable_path/libdb.1.dylib  $appdir/Contents/MacOs/plugins/libacc_actionplugin.dylib
install_name_tool -change libdb.1.dylib @executable_path/libdb.1.dylib  $appdir/Contents/MacOs/plugins/libcrm_actionplugin.dylib
install_name_tool -change libdb.1.dylib @executable_path/libdb.1.dylib  $appdir/Contents/MacOs/plugins/libpeng_actionplugin.dylib
install_name_tool -change libdb.1.dylib @executable_path/libdb.1.dylib  $appdir/Contents/MacOs/plugins/libsrm_actionplugin.dylib

echo "Change library paths for Assistant, does not work"
# install_name_tool -change $qtdir/lib/QtWebKitWidgets.framework/Versions/5/QtWebKitWidgets @executable_path/../Frameworks/QtWebKitWidgets.framework/Versions/5/QtWebKitWidgets  $appdir/Contents/MacOs/Assistant
# install_name_tool -change $qtdir/lib/QtMultimediaWidgets.framework/Versions/5/QtMultimediaWidgets @executable_path/../Frameworks/QtMultimediaWidgets.framework/Versions/5/QtMultimediaWidgets  $appdir/Contents/MacOs/Assistant
# install_name_tool -change $qtdir/lib/QtMultimedia.framework/Versions/5/QtMultimedia @executable_path/../Frameworks/QtMultimedia.framework/Versions/5/QtMultimedia  $appdir/Contents/MacOs/Assistant
# install_name_tool -change $qtdir/lib/QtGui.framework/Versions/5/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/5/QtGui  $appdir/Contents/MacOs/Assistant
# install_name_tool -change $qtdir/lib/QtCore.framework/Versions/5/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/5/QtCore  $appdir/Contents/MacOs/Assistant
# install_name_tool -change $qtdir/lib/QtNetwork.framework/Versions/5/QtNetwork @executable_path/../Frameworks/QtNetwork.framework/Versions/5/QtNetwork  $appdir/Contents/MacOs/Assistant
# install_name_tool -change $qtdir/lib/QtWidgets.framework/Versions/5/QtWidgets @executable_path/../Frameworks/QtWidgets.framework/Versions/5/QtWidgets  $appdir/Contents/MacOs/Assistant
# install_name_tool -change $qtdir/lib/QtQuick.framework/Versions/5/QtQuick @executable_path/../Frameworks/QtQuick.framework/Versions/5/QtQuick  $appdir/Contents/MacOs/Assistant
# install_name_tool -change $qtdir/lib/QtQml.framework/Versions/5/QtQml @executable_path/../Frameworks/QtQml.framework/Versions/5/QtQml  $appdir/Contents/MacOs/Assistant
# install_name_tool -change $qtdir/lib/QtPositioning.framework/Versions/5/QtPositioning @executable_path/../Frameworks/QtPositioning.framework/Versions/5/QtPositioning  $appdir/Contents/MacOs/Assistant
# install_name_tool -change $qtdir/lib/QtOpenGL.framework/Versions/5/QtOpenGL @executable_path/../Frameworks/QtOpenGL.framework/Versions/5/QtOpenGL  $appdir/Contents/MacOs/Assistant
# install_name_tool -change $qtdir/lib/QtPrintSupport.framework/Versions/5/QtPrintSupport @executable_path/../Frameworks/QtPrintSupport.framework/Versions/5/QtPrintSupport  $appdir/Contents/MacOs/Assistant
# install_name_tool -change $qtdir/lib/QtWebKit.framework/Versions/5/QtWebKit @executable_path/../Frameworks/QtWebKit.framework/Versions/5/QtWebKit  $appdir/Contents/MacOs/Assistant
# install_name_tool -change $qtdir/lib/QtSql.framework/Versions/5/QtSql @executable_path/../Frameworks/QtSql.framework/Versions/5/QtSql  $appdir/Contents/MacOs/Assistant
# install_name_tool -change $qtdir/lib/QtSensors.framework/Versions/5/QtSensors @executable_path/../Frameworks/QtSensors.framework/Versions/5/QtSensors  $appdir/Contents/MacOs/Assistant
# install_name_tool -change $qtdir/lib/QtHelp.framework/Versions/5/QtHelp @executable_path/../Frameworks/QtHelp.framework/Versions/5/QtHelp  $appdir/Contents/MacOs/Assistant

echo "Creating dmg file for Biluna"
$qtdir/bin/macdeployqt $appdir -dmg -verbose=3
