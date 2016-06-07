mclang="../build/bin/clang++"
mclang_o="../build.origin/bin/clang++"
${mclang_o} -c hook.cpp -o hook.o -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/usr/include
${mclang} -c CallExpr.cpp -o CallExpr.o
${mclang_o} -c foo.cpp -o foo.o -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/usr/include
${mclang_o} hook.o CallExpr.o foo.o main.cpp -o main
