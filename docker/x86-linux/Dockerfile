FROM debian:stretch-slim as builder
RUN set -x \
        && apt update \
        && apt install -y \
                build-essential \
                cmake \
                wget \
                git
WORKDIR /opt/dependencies
# install boost
RUN wget -O ./Boost.tar.gz https://sourceforge.net/projects/boost/files/boost/1.63.0/boost_1_63_0.tar.gz/download \
        && tar xvzf ./Boost.tar.gz \
        && mv ./boost_* ./boost \
        && cd ./boost \
        && ./bootstrap.sh \
        && ./b2 install -j2 --prefix=/opt/dependencies/boost --toolset=gcc --link=static

# install googleTest
RUN git clone https://github.com/google/googletest.git \
        && cd googletest \
        && mkdir build \
        && cd build \
        && cmake .. -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++ \
        && make

# install yaml-cpp
RUN git clone https://github.com/jbeder/yaml-cpp.git \
        && cd yaml-cpp \
        && mkdir build \
        && cd build \
        && cmake .. -DCMAKE_C_COMPILER=/usr/bin/gcc -DCMAKE_CXX_COMPILER=/usr/bin/g++ \
        && make

# install Eigen
RUN wget -O ./Eigen.tar.gz http://bitbucket.org/eigen/eigen/get/3.3.4.tar.gz \
        && tar xvzf ./Eigen.tar.gz \
        && mv ./eigen-* ./eigen


FROM debian:stretch-slim
RUN set -x \
        && apt update \
        && apt install -y \
                build-essential \
                cmake
ENV DEP_ROOT /opt/dependencies
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
COPY --from=builder ${DEP_ROOT}/yaml-cpp/build/libyaml-cpp.a ${DEP_ROOT}/yaml-cpp/lib/libyaml-cpp.a
# copy eigen
COPY --from=builder ${DEP_ROOT}/eigen ${DEP_ROOT}/eigen
