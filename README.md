# EAL Manager

A cross-platform, architecture-independent reimplementation of Creative's *EaxMan.dll*.

## Introduction

**EalMan** is a library that decodes *.eal* files to retrieve the EAX environment parameters.
The aim is to provide the same functionality as the *EaxMan.dll* from *Creative's EAX patches* for *Unreal Tournament*.
It uses the information about *.eal* file details explained in [senneschall/EAXManagerEAL](https://github.com/senneschall/EAXManagerEAL).

### What it does

Just like *EAXManager*, *EalMan* does not set the actual *DirectSound3d (EAX) / OpenAL (EFX)* parameters, but simply returns the proper values for them.
Programs using *EalMan* must then ensure that these parameters are actually set.

## Use EalMan

The API is kept compatible to EAXMan.

1. Load an *.eal* file first:
```
std::unique_ptr<EalMan> ealman = std::make_unique<EalMan>();
ealman->LoadDataSet(filename, 0);
```

2. Query static listener attribues:
```
ListenerAttributes lstAttributes{};
ealMan->GetListenerAttributes(lstAttributes);
```

3. Query dynamic listener attribues (UT eal files only use one geometry set, so geomID=0 can be hardcoded) if a new environment ID appears:
```
int32_t envID{};
EMPoint posListener{coordinateX, coordinateY, coordinateZ};
ealMan->GetListenerDynamicAttributes(0, posListener, envID, 0);

EAXListenerProperties envAttributes{};
ealMan->GetEnvironmentAttributes(envID, envAttributes);
```

4. Query dynamic source attribues for each sound (you first need to store the listener position, which is done by querying the listener attributes and setting the EMFLAG_LOCKPOSITION flag):
```
int32_t envID{};
EMPoint posListener{coordinateX, coordinateY, coordinateZ};
ealMan->GetListenerDynamicAttributes(0, posListener, envID, EMFLAG_LOCKPOSITION);

EMPoint posSource{coordinateX, coordinateY, coordinateZ};
int32_t srcObstruction{};
float srcObstructionLF{};
int32_t srcOcclusion{};
float srcOcclusionLF{};
float srcOcclusionRM{};
EMPoint vPos{};
ealMan->GetSourceDynamicAttributes(0, posSource, srcObstruction, srcObstructionLF, srcOcclusion, srcOcclusionLF, srcOcclusionRM, vPos, 0);
```

5. Use the parameters to set up EFX calls. The documentation for this can be found in the *Effects Extension Guide.pdf* file included with the openAL SDK.

## How to build

Compile with any C++17 compatible compiler.
No further dependencies were introduced, by design.

## Testing

**EalMan** was thoroughly tested against the *EaxMan.dll* included in *Creative's EAX patches* for *Unreal Tournament*.
Those patches ship *EaxMan.dll* in *v2.0.1.1*, created on *9. March 2000*.
Some EAX SDKs contain an *EaxMan.dll* *v2.0.1.3*.

### Compatibility

The calculated function results were compared with those of *EaxMan.dll*.
It has been verified that over 1.5×10^12 test cases with different coordinates within the playable area of all levels yield the same results.

Discrepancies can be observed in rare cases (1 in 10^8) and can be attributed to rounding errors.
*EaxMan.dll* was compiled in 2000, at a time when SSE instruction set extensions did not yet exist, which is why it uses x87 instructions for all floating-point calculations.
Those commands use an 80-bit representation internally and keep intermediary results on the stack, depending on the compiler generated code specifically.
So it depends entirely on the compiler as to which step the rounding takes place.
This is a well-known behavior that was resolved with the introduction of SSE2.
In this approach, a 32-bit representation is maintained for each intermediate result, ensuring that the result of the floating-point calculation is independent of compiler optimizations.
In other words, rounding errors add up more easily with SSE2.

*EaxMan.dll* and *Eal Manager* perform the same calculation steps, in which the position of the source (or listener) is compared to zones defined in these *.eal* files.
It then decides if the sound comes from within these zone or from outside.
If the coordinate lies exactly on the zone boundary, the rounding error can determine whether the sound comes from within or not.

This is a purely theoretical discussion, because in practice, you would have to position the player with subpixel precision to perceive even a slight difference in sound.
In practice, *Eal Manager* is fully compatible with *EaxMan.dll*.
When considering individual bits, deviations can be observed in the edge cases described.

### Performance

Three API calls have non-negligible performance cost; these were tested and compared with *EaxMan.dll*.
All performance measurements were carried out on an [*AMD Ryzen™ 7 5800X3D*](https://www.cpubenchmark.net/cpu.php?cpu=AMD+Ryzen+7+5800X3D&id=4823).

#### LoadDataSet()

The average API response time: **2.7 ms** (*EaxMan.dll*) - **1.7 ms** (*Eal Manager*).

*Eal Manager* carries out as much data validations as possible in this function, which is called only once during loading.
But *EaxMan.dll* must perform expensive file processing manually. *Eal Manager* benefits from modern STL functions.
Also, *Eal Manager* is better optimized. 
For example, it only processes .eal files with a single geometry set correctly - all UT99 .eal files contain only one - where *EaxMan.dll* could process more complex geometry set structures.

<details>
<summary>Details</summary>

| filename              |  EaxMan average (ms) |  EalMan average (ms) |
| --------------------- | -------------------- | -------------------- |
| `CTF-Command.eal`     | 2.58                 | 1.51                 |
| `CTF-Coret.eal`       | 2.92                 | 1.50                 |
| `CTF-dreary.eal`      | 3.97                 | 1.60                 |
| `CTF-EternalCave.eal` | 2.00                 | 1.38                 |
| `CTF-Face.eal`        | 2.44                 | 1.49                 |
| `CTF-Gauntlet.eal`    | 3.41                 | 1.54                 |
| `CTF-LavaGiant.eal`   | 2.52                 | 1.44                 |
| `CTF-Niven.eal`       | 2.94                 | 1.47                 |
| `CTF-November.eal`    | 3.27                 | 1.91                 |
| `DM-Barricade.eal`    | 3.48                 | 1.73                 |
| `DM-Codex.eal`        | 1.69                 | 1.58                 |
| `DM-Conveyor.eal`     | 2.60                 | 1.69                 |
| `DM-Curse][.eal`      | 5.28                 | 1.70                 |
| `DM-Deck16][.eal`     | 7.73                 | 1.85                 |
| `DM-Fetid.eal`        | 1.49                 | 1.49                 |
| `DM-Fractal.eal`      | 1.07                 | 2.13                 |
| `DM-Gothic.eal`       | 2.81                 | 1.45                 |
| `DM-Grinder.eal`      | 2.10                 | 3.04                 |
| `DM-Hyperblast.eal`   | 1.99                 | 1.40                 |
| `DM-Kgalleon.eal`     | 1.81                 | 1.37                 |
| `DM-Liandri.eal`      | 2.69                 | 1.46                 |
| `DM-Morbias][.eal`    | 5.92                 | 1.67                 |
| `DM-Morpheus.eal`     | 1.13                 | 2.12                 |
| `DM-Oblivion.eal`     | 1.74                 | 1.43                 |
| `DM-Peak.eal`         | 2.74                 | 1.53                 |
| `DM-Phobos.eal`       | 3.45                 | 1.49                 |
| `DM-Pressure.eal`     | 1.01                 | 2.02                 |
| `DM-Pyramid.eal`      | 5.10                 | 1.61                 |
| `DM-Stalwart.eal`     | 1.05                 | 2.10                 |
| `DM-StalwartXL.eal`   | 1.78                 | 1.38                 |
| `DM-Tempest.eal`      | 1.57                 | 2.45                 |
| `DM-Turbine.eal`      | 2.11                 | 1.41                 |
| `DM-Zeto.eal`         | 1.59                 | 1.28                 |

</details>

#### GetListenerDynamicAttributes()

The average API response time: **224 ns** (*EaxMan.dll*) - **81 ns** (*Eal Manager*).

*EaxMan.dll* performs data validation in this function.
So it needs to do exception handling, which is quite expensive.
*Eal Manager* does as much data validation as possible in the `LoadDataSet()` function, so it runs faster here.

But the larger impact on performance is the fact, that this function performs almost exclusively floating-point calculations.
*EaxMan.dll* was compiled in the year 2000 and uses only the x87 instruction set, while *Eal Manager* was compiled with support for SSE2.

<details>
<summary>Details</summary>

| filename              | EaxMan average (ns) | EalMan average (ns) |
| --------------------- | ------------------- | ------------------- |
| `CTF-Command.eal`     | 237.65              | 83.72               |
| `CTF-Coret.eal`       | 140.45              | 58.30               |
| `CTF-dreary.eal`      | 150.01              | 60.18               |
| `CTF-EternalCave.eal` | 297.05              | 103.22              |
| `CTF-Face.eal`        | 152.79              | 58.93               |
| `CTF-Gauntlet.eal`    | 276.20              | 95.56               |
| `CTF-LavaGiant.eal`   | 167.77              | 63.50               |
| `CTF-Niven.eal`       | 303.01              | 103.26              |
| `CTF-November.eal`    | 273.79              | 94.02               |
| `DM-Barricade.eal`    | 128.63              | 51.89               |
| `DM-Codex.eal`        | 185.33              | 68.45               |
| `DM-Conveyor.eal`     | 256.67              | 90.02               |
| `DM-Curse][.eal`      | 204.45              | 78.19               |
| `DM-Deck16][.eal`     | 301.43              | 103.34              |
| `DM-Fetid.eal`        | 131.54              | 52.12               |
| `DM-Fractal.eal`      | 434.92              | 144.49              |
| `DM-Gothic.eal`       | 188.56              | 69.40               |
| `DM-Grinder.eal`      | 498.96              | 167.19              |
| `DM-Hyperblast.eal`   | 122.22              | 49.90               |
| `DM-Kgalleon.eal`     | 119.96              | 49.44               |
| `DM-Liandri.eal`      | 99.17               | 44.66               |
| `DM-Morbias][.eal`    | 393.32              | 133.13              |
| `DM-Morpheus.eal`     | 252.43              | 89.69               |
| `DM-Oblivion.eal`     | 102.91              | 45.47               |
| `DM-Peak.eal`         | 77.68               | 38.13               |
| `DM-Phobos.eal`       | 89.66               | 41.22               |
| `DM-Pressure.eal`     | 274.50              | 95.65               |
| `DM-Pyramid.eal`      | 320.49              | 107.39              |
| `DM-Stalwart.eal`     | 200.81              | 73.51               |
| `DM-StalwartXL.eal`   | 183.30              | 68.52               |
| `DM-Tempest.eal`      | 364.60              | 124.53              |
| `DM-Turbine.eal`      | 321.95              | 109.97              |
| `DM-Zeto.eal`         | 127.26              | 52.81               |

</details>

#### GetSourceDynamicAttributes()

The average API response time: **259 ns** (*EaxMan.dll*) - **115 ns** (*Eal Manager*).

This function processes diffraction boxes, so *Eal Manager* profits from SSE2 whereas *EaxMan.dll* only uses x87.
The difference is a bit smaller because for some sources, the diffraction box calculations can be skipped.

<details>
<summary>Details</summary>

| filename              | EaxMan average (ns) | EalMan average (ns) |
| --------------------- | ------------------- | ------------------- |
| `CTF-Command.eal`     | 245.14              | 93.20               |
| `CTF-Coret.eal`       | 142.77              | 63.79               |
| `CTF-dreary.eal`      | 208.84              | 118.88              |
| `CTF-EternalCave.eal` | 301.33              | 109.15              |
| `CTF-Face.eal`        | 156.53              | 64.63               |
| `CTF-Gauntlet.eal`    | 294.61              | 123.93              |
| `CTF-LavaGiant.eal`   | 170.75              | 68.92               |
| `CTF-Niven.eal`       | 310.21              | 112.69              |
| `CTF-November.eal`    | 284.62              | 107.09              |
| `DM-Barricade.eal`    | 307.94              | 193.24              |
| `DM-Codex.eal`        | 326.99              | 201.47              |
| `DM-Conveyor.eal`     | 262.72              | 98.44               |
| `DM-Curse][.eal`      | 207.72              | 85.29               |
| `DM-Deck16][.eal`     | 448.02              | 241.35              |
| `DM-Fetid.eal`        | 134.09              | 57.00               |
| `DM-Fractal.eal`      | 440.61              | 151.36              |
| `DM-Gothic.eal`       | 191.67              | 74.46               |
| `DM-Grinder.eal`      | 506.14              | 175.70              |
| `DM-Hyperblast.eal`   | 209.74              | 118.40              |
| `DM-Kgalleon.eal`     | 219.79              | 137.08              |
| `DM-Liandri.eal`      | 114.59              | 56.24               |
| `DM-Morbias][.eal`    | 404.22              | 146.37              |
| `DM-Morpheus.eal`     | 275.72              | 110.06              |
| `DM-Oblivion.eal`     | 105.82              | 51.65               |
| `DM-Peak.eal`         | 95.88               | 52.70               |
| `DM-Phobos.eal`       | 91.88               | 46.11               |
| `DM-Pressure.eal`     | 278.31              | 103.89              |
| `DM-Pyramid.eal`      | 309.54              | 108.10              |
| `DM-Stalwart.eal`     | 219.12              | 89.03               |
| `DM-StalwartXL.eal`   | 212.99              | 99.86               |
| `DM-Tempest.eal`      | 496.09              | 246.25              |
| `DM-Turbine.eal`      | 449.17              | 225.88              |
| `DM-Zeto.eal`         | 129.37              | 59.05               |

</details>

## License

The aim of this project is to enable those lovely *EAX* effects on as many games as possible. This means, openAL and EFX calls therefore must be integrated into a patch/mod/remake of those games. Many thanks to everybody who contributes to patches for those old games.

If there is any chance that **EalMan** can help in any way, please use it. If the license is of any problem, do not hesitate to contact me. We'll find a way.
