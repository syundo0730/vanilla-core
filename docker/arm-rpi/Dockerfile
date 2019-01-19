FROM debian:stretch-slim as builder
RUN set -x \
        && apt update \
        && apt install -y \
                cmake \
                wget \
                git \
                swig3.0 \
                python-dev \
                nodejs-dev \
                libjson-c-dev
WORKDIR /opt
RUN git clone https://github.com/raspberrypi/tools.git \
        && cd tools \
        && cp -r arm-bcm2708/arm-rpi-4.9.3-linux-gnueabihf /opt

RUN wget http://http.us.debian.org/debian/pool/main/p/python2.7/libpython2.7-dev_2.7.9-2+deb8u1_armhf.deb \
        && dpkg -x libpython2.7-dev_2.7.9-2+deb8u1_armhf.deb libpython2.7-dev_2.7.9-2_armhf_extracted \
        && cp -r libpython2.7-dev_2.7.9-2_armhf_extracted/usr/include/arm-linux-gnueabihf/python2.7/ /opt/arm-rpi-4.9.3-linux-gnueabihf/arm-linux-gnueabihf \
        && cp -r libpython2.7-dev_2.7.9-2_armhf_extracted/usr/lib/python2.7/config-arm-linux-gnueabihf/ /opt/arm-rpi-4.9.3-linux-gnueabihf/lib/python2.7 \
        && cp -r libpython2.7-dev_2.7.9-2_armhf_extracted/usr/include/python2.7/ /opt/arm-rpi-4.9.3-linux-gnueabihf

ENV CC /opt/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc
ENV CXX /opt/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-g++
ENV PATH $PATH:/opt/arm-rpi-4.9.3-linux-gnueabihf/bin

WORKDIR /opt/dependencies
# install boost
RUN wget -O ./Boost.tar.gz https://sourceforge.net/projects/boost/files/boost/1.63.0/boost_1_63_0.tar.gz/download \
        && tar xzf ./Boost.tar.gz \
        && mv ./boost_* ./boost
COPY user-config.jam /opt/dependencies/boost/
ENV BOOST_BUILD_PATH /opt/dependencies/boost
RUN cd ./boost \
        && ./bootstrap.sh \
        && ./b2 install -j2 --prefix=/opt/dependencies/boost --toolset=gcc --link=static; exit 0

# install googleTest
RUN git clone https://github.com/google/googletest.git \
        && cd googletest \
        && mkdir build \
        && cd build \
        && cmake .. \
        && make

# install yaml-cpp
RUN git clone https://github.com/jbeder/yaml-cpp.git \
        && cd yaml-cpp \
        && mkdir build \
        && cd build \
        && cmake -DCMAKE_INSTALL_PREFIX:PATH=/opt/dependencies/yaml-cpp -DBUILD_SHARED_LIBS=OFF .. \
        && make \
        && make install

# install mraa
RUN git clone https://github.com/intel-iot-devkit/mraa.git \
        && cd mraa \
        && mkdir build \
        && cd build \
        && cmake -DCMAKE_INSTALL_PREFIX:PATH=/opt/dependencies/mraa -DBUILD_SHARED_LIBS=OFF -DBUILDARCH=arm .. \
        && make \
        && make install

# install websocketpp
RUN git clone https://github.com/zaphoyd/websocketpp.git \
        && cd websocketpp \
        && mkdir build \
        && cd build \
        && cmake -DCMAKE_INSTALL_PREFIX:PATH=/opt/dependencies/websocketpp .. \
        && make \
        && make install

# install Eigen
RUN wget -O ./Eigen.tar.gz http://bitbucket.org/eigen/eigen/get/3.3.4.tar.gz \
        && tar xzf ./Eigen.tar.gz \
        && mv ./eigen-* ./eigen

# install json
WORKDIR /opt/dependencies/json
RUN wget https://github.com/nlohmann/json/releases/download/v2.1.1/json.hpp


FROM debian:stretch-slim
RUN set -x \
        && apt update \
        && apt install -y cmake
ENV DEP_ROOT /opt/dependencies
# copy compiler
COPY --from=builder /opt/arm-rpi-4.9.3-linux-gnueabihf /opt/arm-rpi-4.9.3-linux-gnueabihf
ENV CC /opt/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc
ENV CXX /opt/arm-rpi-4.9.3-linux-gnueabihf/bin/arm-linux-gnueabihf-g++
ENV PATH $PATH:/opt/arm-rpi-4.9.3-linux-gnueabihf/bin
# copy boost
COPY --from=builder ${DEP_ROOT}/boost/include ${DEP_ROOT}/boost/include
COPY --from=builder ${DEP_ROOT}/boost/lib ${DEP_ROOT}/boost/lib
# copy googleTest
ENV GTEST_ROOT ${DEP_ROOT}/googletest
COPY --from=builder ${GTEST_ROOT}/googletest/include ${GTEST_ROOT}/googletest/include
COPY --from=builder ${GTEST_ROOT}/googlemock/include ${GTEST_ROOT}/googlemock/include
COPY --from=builder ${GTEST_ROOT}/build/lib ${GTEST_ROOT}/lib
# copy yaml-cpp
COPY --from=builder ${DEP_ROOT}/yaml-cpp/include ${DEP_ROOT}/yaml-cpp/include
COPY --from=builder ${DEP_ROOT}/yaml-cpp/lib/libyaml-cpp.a ${DEP_ROOT}/yaml-cpp/lib/libyaml-cpp.a
# copy mraa
COPY --from=builder ${DEP_ROOT}/mraa/include ${DEP_ROOT}/mraa/include
COPY --from=builder ${DEP_ROOT}/mraa/lib ${DEP_ROOT}/mraa/lib
# copy websocketpp
COPY --from=builder ${DEP_ROOT}/websocketpp/include ${DEP_ROOT}/websocketpp/include
# copy eigen
COPY --from=builder ${DEP_ROOT}/eigen ${DEP_ROOT}/eigen
# copy json
COPY --from=builder ${DEP_ROOT}/json ${DEP_ROOT}/json