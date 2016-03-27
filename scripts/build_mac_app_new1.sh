devpath=/Users/gudula/Documents/biluna/bil/release
appdir=$devpath/Biluna.app
qtdir=/Users/gudula/Qt5.5.1/5.5/clang_64
# opensslpath=/usr/local/ssl

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
cp -v -R $devpath/plugins/libpcalc_actionplugin.dylib $appdir/Contents/MacOs/plugins/libpcalc_actionplugin.dylib
cp -v -R $devpath/plugins/libpeng_actionplugin.dylib $appdir/Contents/MacOs/plugins/libpeng_actionplugin.dylib
cp -v -R $devpath/plugins/libsrm_actionplugin.dylib $appdir/Contents/MacOs/plugins/libsrm_actionplugin.dylib

echo "Change library paths for Biluna and actionplugins"
install_name_tool -change libdb.1.dylib @executable_path/libdb.1.dylib  $appdir/Contents/MacOs/Biluna
install_name_tool -change libdb.1.dylib @executable_path/libdb.1.dylib  $appdir/Contents/MacOs/plugins/libacc_actionplugin.dylib
install_name_tool -change libdb.1.dylib @executable_path/libdb.1.dylib  $appdir/Contents/MacOs/plugins/libcrm_actionplugin.dylib
install_name_tool -change libdb.1.dylib @executable_path/libdb.1.dylib  $appdir/Contents/MacOs/plugins/libpcalc_actionplugin.dylib
install_name_tool -change libdb.1.dylib @executable_path/libdb.1.dylib  $appdir/Contents/MacOs/plugins/libpeng_actionplugin.dylib
install_name_tool -change libdb.1.dylib @executable_path/libdb.1.dylib  $appdir/Contents/MacOs/plugins/libsrm_actionplugin.dylib

echo "Creating dmg file for Biluna"
$qtdir/bin/macdeployqt $appdir -dmg -verbose=3
