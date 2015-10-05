#!/bin/bash

NEW_OPENSSL_VERSION="1.0.2a"
CURRENT_OPENSSL_VERSION="0.9.8"
CURRENT_OPENSSL_LOCATION="/usr/lib/"

APP_NAME="AppName"

# Uncomment next row if you don't know current ssl location
# otool -L $APP_NAME.app/Contents/MacOS/$APP_NAME | grep "libssl"; exit 0

# Download
curl -O http://www.openssl.org/source/openssl-$NEW_OPENSSL_VERSION.tar.gz
tar -xvzf openssl-$NEW_OPENSSL_VERSION.tar.gz
mv openssl-$NEW_OPENSSL_VERSION openssl_x86_64
# Build
cd openssl_x86_64
./Configure darwin64-x86_64-cc -shared --openssldir="@rpath"
make
cd ../
# Copy to bundle
mkdir -p $APP_NAME.app/Contents/Frameworks/
mv openssl_x86_64/libcrypto.1.0.0.dylib $APP_NAME.app/Contents/Frameworks/libcrypto.1.0.0.dylib
mv openssl_x86_64/libssl.1.0.0.dylib $APP_NAME.app/Contents/Frameworks/libssl.1.0.0.dylib
# Clean
rm -r openssl_x86_64
rm openssl-$NEW_OPENSSL_VERSION.tar.gz
# Modify application binary 
cd $APP_NAME.app/Contents/MacOS
install_name_tool -change $CURRENT_OPENSSL_LOCATIONlibcrypto.$CURRENT_OPENSSL_VERSION.dylib @executable_path/../Frameworks/libcrypto.1.0.0.dylib $APP_NAME
install_name_tool -change $CURRENT_OPENSSL_LOCATIONlibssl.$CURRENT_OPENSSL_VERSION.dylib @executable_path/../Frameworks/libssl.1.0.0.dylib $APP_NAME

echo "End of line!"