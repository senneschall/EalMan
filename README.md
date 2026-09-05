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

The API is kept compatible to EAXMan.

## Usage

The same as EAXMan

## How to build

Compile with any C++17 compatible compiler.
No further dependencies were introduced, by design.

## Testing

**EalMan** has been extensively tested with the *EaxMan.dll* included in *Creative's EAX patches* for *Unreal Tournament*.
This *EaxMan.dll* is *v2.0.1.1*, created on *9. March 2000*.
Some EAX SDKs contain an *EaxMan.dll* *v2.0.1.3*.

### Compatibility

The calculated function results were compared with those of *EaxMan.dll*.

### Performance

Three API calls have non-negligible performance cost; these were tested and compared with *EaxMan.dll*.
All performance measurements were carried out on an [*AMD Ryzen™ 7 5800X3D*](https://www.cpubenchmark.net/cpu.php?cpu=AMD+Ryzen+7+5800X3D&id=4823).

#### LoadDataSet()

The average API response time: **5.0 ms** (*EaxMan.dll*) - **1.7 ms** (*Eal Manager*).

*Eal Manager* carries out as much data validations as possible in this function, which is called only once during loading.
But *EaxMan.dll* must perform expensive file processing manually. *Eal Manager* benefits from modern STL functions.
Also, *Eal Manager* is better optimized. 
For example, it only processes .eal files with a single geometry set correctly - all UT99 .eal files contain only one - where *EaxMan.dll* could process more complex geometry set structures.

<details>
<summary>Details</summary>
| filename              | EaxMan average (ns) | EalMan average (ns) |
| --------------------- | ------------------- | ------------------- |
| `CTF-Command.eal`     | 2664438.880         | 157227.750          |
| `CTF-Coret.eal`       | 2980610.630         | 152988.510          |
| `CTF-dreary.eal`      | 4025438.330         | 164278.830          |
| `CTF-EternalCave.eal` | 2051306.910         | 140245.040          |
| `CTF-Face.eal`        | 2532192.810         | 153523.180          |
| `CTF-Gauntlet.eal`    | 3481236.210         | 158687.120          |
| `CTF-LavaGiant.eal`   | 2577733.360         | 147990.160          |
| `CTF-Niven.eal`       | 2987316.030         | 154318.820          |
| `CTF-November.eal`    | 3262531.530         | 172200.410          |
| `DM-Barricade.eal`    | 3523795.370         | 154984.810          |
| `DM-Codex.eal`        | 1717097.220         | 144663.480          |
| `DM-Conveyor.eal`     | 2741587.670         | 151315.580          |
| `DM-Curse][.eal`      | 5379041.790         | 174834.510          |
| `DM-Deck16][.eal`     | 7910864.080         | 190183.910          |
| `DM-Fetid.eal`        | 1588498.620         | 147912.300          |
| `DM-Fractal.eal`      | 10991734.070        | 215924.250          |
| `DM-Gothic.eal`       | 2915488.720         | 147399.810          |
| `DM-Grinder.eal`      | 21706263.010        | 328303.320          |
| `DM-Hyperblast.eal`   | 2030765.260         | 145271.660          |
| `DM-Kgalleon.eal`     | 1896171.620         | 139875.110          |
| `DM-Liandri.eal`      | 2784924.990         | 155596.470          |
| `DM-Morbias][.eal`    | 6075913.170         | 173952.040          |
| `DM-Morpheus.eal`     | 11719272.470        | 222653.000          |
| `DM-Oblivion.eal`     | 1802249.910         | 149448.110          |
| `DM-Peak.eal`         | 2861610.820         | 151019.190          |
| `DM-Phobos.eal`       | 3596954.090         | 154684.530          |
| `DM-Pressure.eal`     | 10572825.190        | 215199.130          |
| `DM-Pyramid.eal`      | 5271962.080         | 165852.880          |
| `DM-Stalwart.eal`     | 10933381.860        | 212475.710          |
| `DM-StalwartXL.eal`   | 1855818.290         | 142063.420          |
| `DM-Tempest.eal`      | 16360938.370        | 255518.780          |
| `DM-Turbine.eal`      | 2158901.610         | 145334.010          |
| `DM-Zeto.eal`         | 1613612.880         | 133189.260          |
</details>

#### GetListenerDynamicAttributes()

The average API response time: **161 ns** (*EaxMan.dll*) - **37 ns** (*Eal Manager*).

*EaxMan.dll* performs data validation in this function.
So it needs to do exception handling, which is quite expensive.
*Eal Manager* does as much data validation as possible in the `LoadDataSet()` function, so it runs much faster here.

<details>
<summary>Details</summary>
| filename              | EaxMan average (ns) | EalMan average (ns) |
| --------------------- | ------------------- | ------------------- |
| `CTF-Command.eal`     | 182.282             | 41.207              |
| `CTF-Coret.eal`       | 108.362             | 22.679              |
| `CTF-dreary.eal`      | 122.966             | 25.967              |
| `CTF-EternalCave.eal` | 300.558             | 74.007              |
| `CTF-Face.eal`        | 261.563             | 62.375              |
| `CTF-Gauntlet.eal`    | 119.974             | 24.539              |
| `CTF-LavaGiant.eal`   | 167.494             | 36.844              |
| `CTF-Niven.eal`       | 113.330             | 24.225              |
| `CTF-November.eal`    | 219.432             | 49.450              |
| `DM-Barricade.eal`    | 119.776             | 28.351              |
| `DM-Codex.eal`        | 69.838              | 13.650              |
| `DM-Conveyor.eal`     | 237.357             | 56.973              |
| `DM-Curse][.eal`      | 146.627             | 34.794              |
| `DM-Deck16][.eal`     | 181.108             | 40.936              |
| `DM-Fetid.eal`        | 72.130              | 12.908              |
| `DM-Fractal.eal`      | 142.201             | 34.246              |
| `DM-Gothic.eal`       | 168.230             | 37.935              |
| `DM-Grinder.eal`      | 267.207             | 67.122              |
| `DM-Hyperblast.eal`   | 102.976             | 20.172              |
| `DM-Kgalleon.eal`     | 85.504              | 16.040              |
| `DM-Liandri.eal`      | 103.460             | 21.391              |
| `DM-Morbias][.eal`    | 189.493             | 42.404              |
| `DM-Morpheus.eal`     | 504.173             | 137.555             |
| `DM-Oblivion.eal`     | 74.996              | 14.069              |
| `DM-Peak.eal`         | 44.853              | 8.922               |
| `DM-Phobos.eal`       | 145.422             | 32.611              |
| `DM-Pressure.eal`     | 150.179             | 32.483              |
| `DM-Pyramid.eal`      | 268.525             | 65.212              |
| `DM-Stalwart.eal`     | 111.055             | 21.394              |
| `DM-StalwartXL.eal`   | 117.183             | 21.465              |
| `DM-Tempest.eal`      | 232.794             | 56.267              |
| `DM-Turbine.eal`      | 108.018             | 23.765              |
| `DM-Zeto.eal`         | 69.096              | 12.129              |
</details>

#### GetSourceDynamicAttributes()

The average API response time: **414 ns** (*EaxMan.dll*) - **108 ns** (*Eal Manager*).

*EaxMan.dll* contains the prototype code for processing diffraction boxes.
These diffraction calculations are quite expensive, even though the results are ultimately discarded entirely.
*Eal Manager* therefore skips these calculations, resulting in a performance boost.

<details>
<summary>Details</summary>
| filename              | EaxMan average (ns) | EalMan average (ns) |
| --------------------- | ------------------- | ------------------- |
| `CTF-Command.eal`     | 444.006             | 115.992             |
| `CTF-Coret.eal`       | 429.405             | 112.877             |
| `CTF-dreary.eal`      | 556.146             | 151.769             |
| `CTF-EternalCave.eal` | 336.847             | 86.067              |
| `CTF-Face.eal`        | 259.250             | 62.159              |
| `CTF-Gauntlet.eal`    | 286.131             | 50.337              |
| `CTF-LavaGiant.eal`   | 275.879             | 67.108              |
| `CTF-Niven.eal`       | 445.945             | 113.412             |
| `CTF-November.eal`    | 378.354             | 100.809             |
| `DM-Barricade.eal`    | 500.743             | 130.414             |
| `DM-Codex.eal`        | 243.672             | 56.388              |
| `DM-Conveyor.eal`     | 454.688             | 121.786             |
| `DM-Curse][.eal`      | 708.058             | 202.795             |
| `DM-Deck16][.eal`     | 319.894             | 66.008              |
| `DM-Fetid.eal`        | 162.061             | 32.521              |
| `DM-Fractal.eal`      | 877.986             | 277.364             |
| `DM-Gothic.eal`       | 317.308             | 79.867              |
| `DM-Grinder.eal`      | 910.432             | 265.670             |
| `DM-Hyperblast.eal`   | 277.749             | 69.330              |
| `DM-Kgalleon.eal`     | 228.524             | 44.353              |
| `DM-Liandri.eal`      | 68.372              | 13.559              |
| `DM-Morbias][.eal`    | 665.674             | 175.831             |
| `DM-Morpheus.eal`     | 850.454             | 245.345             |
| `DM-Oblivion.eal`     | 122.184             | 23.240              |
| `DM-Peak.eal`         | 49.412              | 9.064               |
| `DM-Phobos.eal`       | 691.452             | 194.997             |
| `DM-Pressure.eal`     | 360.090             | 89.547              |
| `DM-Pyramid.eal`      | 474.048             | 128.486             |
| `DM-Stalwart.eal`     | 353.985             | 90.588              |
| `DM-StalwartXL.eal`   | 158.095             | 27.499              |
| `DM-Tempest.eal`      | 747.375             | 193.562             |
| `DM-Turbine.eal`      | 600.485             | 130.574             |
| `DM-Zeto.eal`         | 110.569             | 19.907              |
</details>

## License

The aim of this project is to enable those lovely *EAX* effects on as many games as possible. This means, it must be integrated into a patch/mod/remake of those games. Many thanks to everybody who contributes to patches for those old games.

If there is any chance that **EalMan** can help in any way, please use it. If the license is of any problem, do not hesitate to contact me. We'll find a way.
