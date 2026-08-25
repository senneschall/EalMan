# EAL Manager

A cross-platform, architecture-independent reimplementation of Creative's *EaxMan.dll*.

## Introduction

**EalMan** is a library that decodes *.eal* files to get the EAX environment presets out.
The aim is to provide the same functionality as the *EaxMan.dll* from *Creative's EAX patches* for *Unreal Tournament*.
It uses the information about *.eal* file details explained in [senneschall/EAXManagerEAL](https://github.com/senneschall/EAXManagerEAL).

### What it does

Just like *EAXManager*, *EalMan* does not set the actual *DirectSound3d (EAX) / OpenAL (EFX)* parameters, but simply returns the proper values for them.
Programs using *EalMan* must then ensure that these parameters are actually set.

## Use EalMan

The API is kept compatible to EAXMan, but not as a COM-Object.

## Usage

The same as EAXMan

## How to build

Compile with any C++17 compatible compiler.
No further dependencies were introduced, by design.

## License

The aim of this project is to enable those lovely *EAX* effects on as many games as possible. This means, it must be integrated into a patch/mod/remake of those games. Many thanks to everybody who contributes to patches for those old games.

If there is any chance that **EalMan** can help in any way, please use it. If the license is of any problem, do not hesitate to contact me. We'll find a way.