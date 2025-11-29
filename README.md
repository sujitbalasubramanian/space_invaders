# Space Invaders

A classic Space Invaders clone written in C with support for **native** and **WebAssembly (WASM)** builds using Emscripten.

**[Play Now](https://si.sujitbalasubramanian.in)**

---

## Setup
Requirements:
- `git`, `cmake`, `clang`/`gcc`
- Emscripten (for WASM builds)

Clone the repository:
```bash
git clone --recursive https://github.com/sujitbalasubramanian/space_invaders.git
```

Install Emscripten (for WASM builds)
```bash
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source emsdk_env.sh
```

---

## Build
Native:
```bash
rm -rf build/
cmake -B build
cmake --build build
./build/space_invaders
```

Wasm:
```bash
rm -rf build/
emcmake cmake -B build
cmake --build build
emrun ./build/space_invaders.html
```
