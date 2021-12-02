# Build `hololux` and copy artifacts to `dependencies`

## Dependency

- `gcc 8`
- `Python >= 3.9`
- `Conan`
- `CMake >= 3.16`
- `patchelf`

Install `patchelf`:

```bash
sudo apt install patchelf
```

## Debug

```bash
cd dependencies
mkdir dbuild && cd dbuild
conan install .. -s build_type=Debug -s compiler.libcxx=libstdc++
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCONAN_LIBCXX=libstdc++
make -j
python ../../collect_depends.py . ..
```

## Release

```bash
cd dependencies
mkdir build && cd build
conan install .. -s compiler.libcxx=libstdc++
cmake .. -DCONAN_LIBCXX=libstdc++
make -j
python ../../collect_depends.py . ..
```

# Build `hololux_demo`

```bash
mkdir build && cd build
cmake ..
make -j
```

# Release library built under Ubuntu 16.04

```bash
docker pull ubuntu:16.04
docker create --name hololux-demo -it ubuntu:16.04 /bin/bash
docker cp setup_ubuntu16.04.sh hololux-demo:/
docker start -i hololux-demo
bash setup_ubuntu16.04.sh
```

Now set up git ssh key and conan repository, clone code to container. Perform above steps and push the artifacts.

NOTE: This container won't pass any of the tests.
