# banim

## dependencies
```
sudo pacman -S cmake make gcc pkgconf glfw glew cairo mesa zip unzip tar curl ninja
```

clone vcpkg
```
cd ~ && git clone https://github.com/Microsoft/vcpkg.git
```

bootstrap vcpkg
```
cd ~/vcpkg && ./bootstrap-vcpkg.sh
```

add VCPKG_ROOT environment variable to bashrc and source
``` 
echo 'export VCPKG_ROOT=$HOME/vcpkg' >> ~/.bashrc && source ~/.bashrc
```

verify vcpkg is set
```
export VCPKG_ROOT=$HOME/vcpkg && echo $VCPKG_ROOT
```

make build directory
```
cd /home/lifrankfan/Projects/banim && mkdir -p build && cd build
```

configure rpoject with cmake using vcpkg toolchain
```
export VCPKG_ROOT=$HOME/vcpkg && cmake .. -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
```

make the project
```
make
```

Run project
./project_name
