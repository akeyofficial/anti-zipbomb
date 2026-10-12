# Anti-Zipbomb

A C++ tool that checks zip files for bomb behavior before extracting.

## Build

Requires `g++`, `zlib`, and `minizip`.

Debian/Ubuntu:
```bash
sudo apt install g++ zlib1g-dev libminizip-dev
```

Termux:
```bash
pkg install g++ zlib libminizip-dev
```

Compile:
```bash
g++ zipguard.cpp -o zipguard -lminizip -lz
```

## Usage

```bash
./zipguard <zipfile>
```

## License

MIT
