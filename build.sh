#! /bin/bash 

CONFIG=debug
CONFIG_CMAKE=Debug
BUILD_ALL=NO

copy_libs()
{
    cp -r ../nsengine/lib deps/nsengine
    cp -r ../nsengine/include deps/nsengine
}

copy_dirs()
{
    cp -r resources bin
    cp -r import bin
    mkdir -p bin/logs
}

build()
{
    mkdir -p build/$CONFIG
    cd build/$CONFIG
    cmake -DCMAKE_BUILD_TYPE=$CONFIG_CMAKE\
	  -DPLATFORM=x64\
          -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ../..
    make -j8
    mv compile_commands.json ..
    cd ../..
}

for var in "$@"
do
    if [ "$var" = "-d" ]; then
	CONFIG=debug
	CONFIG_CMAKE=Debug
    elif [ "$var" = "-r" ]; then
	CONFIG=release
	CONFIG_CMAKE=Release
    elif [ "$var" = "-c" ]; then
	rm -r -f build/$CONFIG
	rm -r -f bin
	rm -r -f lib
    elif [ "$var" = "-call" ]; then
	rm -r -f build
	rm -r -f bin
	rm -r -f lib
    elif [ "$var" = "-all" ]; then
	BUILD_ALL=YES
    fi
done

if [ "$BUILD_ALL" = NO ]; then
    copy_libs
    build
    copy_dirs
else
    copy_libs
    CONFIG=debug
    CONFIG_CMAKE=Debug
    build
    CONFIG=release
    CONFIG_CMAKE=Release
    build
    copy_dirs
fi
