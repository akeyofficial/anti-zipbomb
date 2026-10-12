# zipguard

A C++ tool that checks zip files for bomb behavior before extracting.

## Limits

- 500 MB total uncompressed
- 500 MB single file
- 100:1 compression ratio
- 10,000 files

Edit the constants at the top of `zipguard.cpp` to change these.

## Build

Requires `g++`, `zlib`, and `minizip`.

Debian/Ubuntu: `sudo apt install g++ zlib1g-dev libminizip-dev`

Termux: `pkg install g++ zlib libminizip-dev`

Compile: `g++ zipguard.cpp -o zipguard -lminizip -lz`

## Usage

`./zipguard <zipfile>`

## License

MIT
