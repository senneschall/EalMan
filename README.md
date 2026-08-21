# EAL Manager

A cross-platform, architecture-independent reimplementation of Creative's *EaxMan.dll*.

## Introduction

**EalMan** is a library that decodes *.eal* files to get the EAX environment presets out.
The aim is to provide the same functionality as the *EaxMan.dll* from *Creative's EAX patches* for *Unreal Tournament*.
It uses the information aboud *.eal* file details explained in [senneschall/EAXManagerEAL](https://github.com/senneschall/EAXManagerEAL).

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

## Credits

The source code for *PlayEalFiles* contains pieces from [openal-soft](https://github.com/kcat/openal-soft), which is licensed unter LGPL. Those pieces remain under the LGPL.

Other pieces are heavily derived from the RIFF decoder and information [RIFF File Structure](https://johnloomis.org/cpe102/asgn/asgn1/riff.html)

Another helpful resource in making *PlayEalFiles* work was the [OpenAL short example](https://ffainelli.github.io/openal-example/)

And a final shoutout and thanks to *Createive* for those *EAX patches*, the folks behind *websound.ru* who saved those old SDKs. And to the contributors of [OldUnreal/UnrealTournamentPatches](https://github.com/OldUnreal/UnrealTournamentPatches) who keep this game alive.
