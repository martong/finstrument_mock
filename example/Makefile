gtest_include_dir=/Users/mg/local/googletest/googletest/include
gtest_lib=/Users/mg/local/googletest/build/googlemock/gtest/libgtest.a

mclang=../build/bin/clang++
mclang_o=../build.origin/bin/clang++
CXX_FLAGS=-std=c++14 -isystem /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk/usr/include

main: hook.o bar.o foo.o called.o main.cpp hook.hpp
	${mclang_o} hook.o bar.o foo.o called.o ${gtest_lib} main.cpp -o main ${CXX_FLAGS} -isystem ${gtest_include_dir}
hook.o: hook.cpp hook.hpp
	${mclang_o} -c hook.cpp -o hook.o ${CXX_FLAGS}
bar.o: bar.cpp
	${mclang} -fsanitize=mock -c bar.cpp -o bar.o
foo.o: foo.cpp called.cpp
	${mclang_o} -c foo.cpp -o foo.o ${CXX_FLAGS}
called.o: called.cpp
	${mclang_o} -c called.cpp -o called.o ${CXX_FLAGS}

clean:
	rm *.o
