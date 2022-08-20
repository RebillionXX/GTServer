mkdir build
cd build
cmake .. -DCMAKE_PREFIX_PATH="C:\vcpkg\installed\x64-windows" -DOPENSSL_ROOT_DIR="C:\vcpkg\installed\x64-windows"
cmake --build .
pause