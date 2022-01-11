#!/bin/sh

rm -rf tmp
mkdir tmp
cd tmp

##########################################################
## Install moudles
install_module() {
    git clone --depth=1 https://github.com/$1/$2.git

    cd $2
    mkdir -p build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make -j4
    cmake --build . --target install

    cd ../../
}
##########################################################

install_module yhirose cpp-httplib
install_module open-source-parsers jsoncpp
install_module gabime spdlog
install_module SRombauts SQLiteCpp
install_module reo7sp tgbot-cpp

cd ../
rm -rf tmp
