FROM productize/docker-arm-embedded:17.04
RUN apt-get update -qq && apt-get install -y python2.7 python-pip
RUN pip install mbed-cli && mbed toolchain -G GCC_ARM