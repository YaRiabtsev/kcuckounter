rm -rf build
cmake -B build/   -DCMAKE_INSTALL_PREFIX=$HOME/kde/usr/   -DQT_MAJOR_VERSION=6
cmake --build build/