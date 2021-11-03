# Build `hololux` and copy artifacts to `dependencies`

## Debug

```bash
cd ..
mkdir dbuild && cd dbuild
conan install .. -s build_type=Debug
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j
python ../demo/collect_depends.py . ../demo/dependencies
```

## Release

```bash
cd ..
mkdir build && cd build
conan install ..
cmake ..
make -j
python ../demo/collect_depends.py . ../demo/dependencies
```

# Build `hololux_demo`

```bash
mkdir build && cd build
cmake ..
make -j
```
