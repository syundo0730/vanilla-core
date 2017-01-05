### installing cross compiler
wget -O ./thirdparty/EdisonCompiler.tar.bz2 http://iotdk.intel.com/sdk/3.5/edison/iot-devkit-toolchain-osx-32bit-edison-20160606.tar.bz2
mkdir -p ./thirdparty/edison-cross-compiler
tar xvzf ./thirdparty/EdisonCompiler.tar.bz2 -C ./thirdparty/edison-cross-compiler

### installing Eigen
wget -O ./thirdparty/Eigen.tar.gz http://bitbucket.org/eigen/eigen/get/3.3.4.tar.gz
tar xvzf ./thirdparty/Eigen.tar.gz -C ./thirdparty
mv ./thirdparty/eigen-* ./thirdparty/eigen

### installing Boost
wget -O ./thirdparty/Boost.tar.gz https://sourceforge.net/projects/boost/files/boost/1.63.0/boost_1_63_0.tar.gz/download
tar xvzf ./thirdparty/Boost.tar.gz -C ./thirdparty
mv ./thirdparty/boost_* ./thirdparty/boost

### get matplotlib
mkdir -p ./thirdparty/matplotlib-cpp
wget -O ./thirdparty/matplotlib-cpp/matplotlibcpp.h https://raw.githubusercontent.com/lava/matplotlib-cpp/master/matplotlibcpp.h

### get websocketpp
git clone git@github.com:zaphoyd/websocketpp.git ./thirdparty/websocketpp

### get nlohmann/json
mkdir -p ./thirdparty/json
wget -O ./thirdparty/json/json.hpp https://github.com/nlohmann/json/releases/download/v2.1.1/json.hpp

### get boost/di
mkdir -p ./thirdparty/boost/stage/include/boost/di
wget -O ./thirdparty/boost/stage/include/boost/di/di.hpp https://raw.githubusercontent.com/boost-experimental/di/cpp14/include/boost/di.hpp

### install fruit
wget -O ./thirdparty/fruit.tar.gz https://github.com/google/fruit/archive/v2.0.4.tar.gz
tar xvzf ./thirdparty/fruit.tar.gz -C ./thirdparty
mv ./thirdparty/fruit-* ./thirdparty/fruit
cd ./thirdparty/fruit
cmake -DCMAKE_INSTALL_PREFIX=./stage -DBUILD_SHARED_LIBS=OFF -DCMAKE_C_COMPILER=/opt/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc -DCMAKE_CXX_COMPILER=/opt/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-c++
make -j
sudo make install
cd ../../

### install googleTest
cd ./thirdparty
git clone https://github.com/google/googletest.git
cd googletest
mkdir build
cd build
cmake ..
-DCMAKE_C_COMPILER=/usr/bin/gcc
-DCMAKE_CXX_COMPILER=/usr/bin/g++
make
cd ../../../

### install yaml-cpp
cd ./thirdparty
git clone https://github.com/jbeder/yaml-cpp.git
cd yaml-cpp
mkdir build
cd build
cmake ..
-DCMAKE_C_COMPILER=/usr/bin/gcc
-DCMAKE_CXX_COMPILER=/usr/bin/g++
make
cd ../../../