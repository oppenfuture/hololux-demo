# Build `hololux` and copy artifacts to `dependencies`

## Dependency

- `gcc 8`
- `Python >= 3.6`
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
conan install .. -s build_type=Debug
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j
python ../../collect_depends.py . ..
```

## Release

```bash
cd dependencies
mkdir build && cd build
conan install ..
cmake ..
make -j
python ../../collect_depends.py . ..
```

# Build `hololux_demo`

```bash
mkdir build && cd build
cmake ..
make -j
```
