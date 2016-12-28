### intalling Eigen
wget -O ./thirdparty/Eigen.tar.gz http://bitbucket.org/eigen/eigen/get/3.3.1.tar.gz
tar xvzf ./thirdparty/Eigen.tar.gz -C ./thirdparty
mv ./thirdparty/eigen-* ./thirdparty/eigen

### installing Boost
wget -O ./thirdparty/Boost.tar.gz https://sourceforge.net/projects/boost/files/boost/1.63.0/boost_1_63_0.tar.gz/download
tar xvzf ./thirdparty/Boost.tar.gz -C ./thirdparty
mv ./thirdparty/boost_* ./thirdparty/boost