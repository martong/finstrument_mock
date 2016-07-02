mclang=../build/bin/clang++
mclang_o=../build.origin/bin/clang++
CXX_FLAGS=-std=c++14 -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/usr/include

main: hook.o bar.o foo.o main.cpp hook.hpp
	${mclang_o} hook.o bar.o foo.o main.cpp -o main ${CXX_FLAGS}
hook.o: hook.cpp hook.hpp
	${mclang_o} -c hook.cpp -o hook.o ${CXX_FLAGS}
bar.o: bar.cpp
	${mclang} -c bar.cpp -o bar.o
foo.o: foo.cpp
	${mclang_o} -c foo.cpp -o foo.o ${CXX_FLAGS}
