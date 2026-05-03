# RishiOS

RishiOS is an x86-64 operating system written from scratch in C and x86 Assembly. It includes a custom bootloader and kernel, and is designed to run in an emulated environment.
Why? I was awestruck with how computers with silicon transistors are able to fire in such a way that we can run elegant software on them. In order to fully learn to appreciate all Unix-based devices, I decided to build an OS of my own, with a simple command-line retro feel to it.

## System Requirements

The OS targets the following hardware configuration:

- x86-64 architecture
- 1G page support (`pdpe1gb`)
- xAPIC
- SSE2
- PS/2 keyboard
- ATA disk (IDE interface)
- No hard disk required at runtime, runs entirely in RAM

## Project Structure

| Directory | Description |
|---|---|
| `bootloader/` | Stage 1/2 bootloader written in x86 Assembly |
| `kernel/` | Kernel source in C and Assembly |
| `user/` | User-space programs |
| `scripts/` | Build and run scripts |
| `bin/` | Compiled binaries |
| `boot.img` | Bootable disk image (stable version) |
| `os.img` | Full OS disk image (FAT16 in-progress) |
| `bochsrc.bxrc` | Bochs emulator configuration for FAT16 version |
| `old.bxrc` | Bochs emulator configuration for stable version |

## Dependencies

Building from source requires:

- `nasm`
- `x86_64-elf-gcc` (cross-compiler, do **NOT** use the system `gcc`)
- `make`

Running the OS requires one of:

- **QEMU** (recommended, works on Linux and macOS)
- **Bochs 2.7** (Windows, using `bochsrc.bxrc`)

## Setup

### Pre-built Image

If you would prefer a pre-built image, that is provided to you in releases. For contribution, however, it is necessary to build the system from a cross-compiler (x86_64-elf-gcc`, in this case).

### Linux

Install dependencies:

```bash
sudo apt install nasm gcc-x86-64-linux-gnu qemu-system-x86
```

Build and run:

```bash
bash scripts/compile_run.sh
```

### macOS

Install dependencies via Homebrew:

```bash
brew install nasm x86_64-elf-gcc qemu
```

Build:

```bash
bash scripts/compile_run.sh
```
> `compile_run.sh` may give errors depending on the version that the script targeted. If all else fails, one can use the image file from releases.

Run:

```bash
qemu-system-x86_64 \
  -m 1G \
  -smp 1 \
  -cpu pentium3,+lm,+sse2,+apic,+pdpe1gb \
  -drive file=boot.img,format=raw,if=ide,media=disk \
  -machine type=pc-i440fx-9.2
```

> Note: `-enable-kvm` and `-mem-path /dev/hugepages` are Linux-only flags and must be omitted on macOS.

### Windows

Install [Bochs 2.7](https://bochs.sourceforge.io/) and open `bochsrc.bxrc` from the repo root, or use WSL (Debian) for building combined with Bochs on the Windows side:

```bat
scripts\windowsrun.bat
```

Requires WSL with Debian, NASM, and `x86_64-elf-gcc` installed inside WSL.

## Results

Here is how RishiOS currently looks like from the QEMU emulator:

https://github.com/user-attachments/assets/eab2a865-72d5-4514-ad51-c73e30dd092b

> This is from a MacOS system

## Suggestions for me?
I am new to the world of OS development, and would love any suggestions for better code, or just tips in general :)
