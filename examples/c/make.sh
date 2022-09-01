export Torch_DIR="/home/yshen/libtorch"
export OpenCV_DIR="/usr/local/opencv"

rm -rf build
mkdir build
cd build
cmake ..
#cmake --build . --config Release
make
#cp ./main ../bin
#cd ..
#rm -r build
