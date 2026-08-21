#pragma once
#include <gtest/gtest.h>
#include "EalMan.h"

struct TestParameter
{
    std::string filename{};
    EAXDiffractionModel gdfm{};
    ListenerAttributes lisa{};
    EAXListenerProperties denv{};
    SourceAttributes dsrc{};
    MaterialAttributes dobs{};
    uint32_t nrEnvs{};
    std::vector<EAXListenerProperties> envp{};
    std::vector<std::string> envNames{};
    uint32_t nrSrcs{};
    std::vector<SourceAttributes> srcp{};
    std::vector<std::string> srcNames{};
    std::vector<std::vector<EMPoint>> srcPos{};
    uint32_t nrObst{};
    std::vector<MaterialAttributes> obstacles{};
    std::vector<std::string> obsNames{};
};

class UT99EalFiles : public ::testing::TestWithParam<TestParameter>
{
protected:
    EalMan man;
};

/************************************************************************
* all those following values have been read with Creative's EAX Manager *
************************************************************************/

TestParameter ctfcommand{
    "CTF-Command.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    8,
    {EAXListenerProperties{-1500, -237, 0.0f, 1.285f, 0.621f, -600, 0.006187f, 1039, 0.009519f, 13, 6.425f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1000, -1000, 0.0f, 1.49f, 0.83f, -10000, 0.061f, 1014, 0.025f, 18, 17.5f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1000, 0, 0.0f, 1.65f, 1.5f, -1363, 0.008f, -1153, 0.012f, 20, 8.3f, 1.0f, -5.0f, 31}, EAXListenerProperties{-1000, -300, 0.0f, 2.31f, 0.64f, -711, 0.012f, 83, 0.017f, 5, 11.6f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1000, -300, 0.0f, 1.549492f, 0.64f, -364, 0.008049f, 685, 0.011403f, 5, 7.781f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1000, -300, 0.0f, 0.739001f, 0.64f, 278, 0.003839f, 1072, 0.005439f, 5, 3.711f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1000, -1000, 0.0f, 2.81f, 0.14f, 429, 0.014f, 1023, 0.021f, 21, 1.7f, 0.8f, -5.0f, 63}, EAXListenerProperties{-1000, -4000, 0.0f, 1.49f, 0.1f, -449, 0.007f, 1700, 0.011f, 22, 1.8f, 1.0f, -5.0f, 63}},
    {"Corridor", "Big hall", "Low hall", "Medium hall", "Medium room", "Small room", "Pipe corridor", "Underwater"},
    0,
    {},
    {},
    {},
    1,
    {MaterialAttributes{-6849, 0.178f, 0.0f, 3}},
    {"Walls"}
};

TestParameter ctfcoret{
    "CTF-Coret.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    4,
    {EAXListenerProperties{-1000, -237, 0.0f, 2.281f, 0.79f, 247, 0.013f, 192, 0.02f, 13, 13.5f, 0.753f, -5.0f, 63}, EAXListenerProperties{-1000, -237, 0.0f, 3.099f, 0.79f, -55, 0.016f, -137, 0.02f, 13, 13.5f, 0.795f, -5.0f, 63}, EAXListenerProperties{-1000, -237, 0.0f, 1.19f, 0.79f, 548, 0.008f, 521, 0.02f, 13, 13.5f, 0.753f, -5.0f, 63}, EAXListenerProperties{-1000, -237, 0.0f, 4.462f, 0.79f, -205, 0.049f, -301, 0.029f, 13, 13.5f, 0.877f, -5.0f, 63}},
    {"Room 2", "Room 3", "Room 1", "Room 4"},
    1,
    {SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"footsteps.wav"},
    {{}},
    5,
    {MaterialAttributes{-548, 0.25f, 0.5f, 3}, MaterialAttributes{-1233, 0.25f, 0.5f, 3}, MaterialAttributes{-1918, 0.25f, 0.5f, 3}, MaterialAttributes{-3014, 0.25f, 0.5f, 3}, MaterialAttributes{-3973, 0.25f, 0.5f, 3}},
    {"Occlude1", "Occlude2", "Occlude3", "Occlude4", "Occlude5"}
};

TestParameter ctfdreary{
    "CTF-dreary.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    6,
    {EAXListenerProperties{-1000, -4000, 0.0f, 1.49f, 0.1f, -449, 0.007f, 1700, 0.011f, 22, 1.8f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1000, -300, 0.0f, 1.549492f, 0.64f, -364, 0.008049f, 429, 0.011403f, 5, 7.781f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1507, -237, 0.0f, 1.8274f, 0.79f, -874, 0.008799f, 734, 0.013536f, 13, 9.137f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1000, -300, 0.0f, 2.31f, 0.64f, -711, 0.012f, 83, 0.017f, 5, 11.6f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1000, -237, 0.0f, 0.7422f, 0.79f, -92, 0.003574f, 1516, 0.005498f, 13, 3.711f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1000, -300, 0.0f, 1.009032f, 0.64f, 8, 0.005242f, 802, 0.007426f, 5, 5.067f, 1.0f, -5.0f, 63}},
    {"Acid tank", "Medium chamber", "Corridor", "Large chamber", "Small corridor", "Small chamber"},
    2,
    {SourceAttributes{EAXBufferProperties{1000, 0, 1000, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 500.0f, 1000000.0f, 1, 0}, SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"Clip clop", "stone04.wav"},
    {{}, {}},
    1,
    {MaterialAttributes{-6027, 0.247f, 0.5f, 3}},
    {"Dreary occlusion"}
};

TestParameter ctfeternalcave{
    "CTF-EternalCave.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    5,
    {EAXListenerProperties{-1233, -822, 0.0f, 3.916f, 0.725f, 96, 0.015f, 27, 0.022f, 8, 14.6f, 0.918f, -5.0f, 63}, EAXListenerProperties{-1233, -822, 0.0f, 6.915f, 0.36f, -356, 0.144f, -466, 0.026f, 10, 50.299999f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1233, -4000, 0.0f, 1.49f, 0.1f, -449, 0.007f, 1342, 0.011f, 22, 1.8f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1233, -1233, 0.0f, 2.071777f, 0.777f, 96, 0.006492f, 44, 0.017853f, 8, 11.848f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1233, -1096, 0.0f, 6.642f, 0.36f, -356, 0.16f, -137, 0.027f, 10, 50.299999f, 1.0f, -5.0f, 63}},
    {"Caves", "Outside", "Under Wasser", "Cave Rooms", "Red Flag"},
    2,
    {SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}, SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"footsteps.wav", "gun-uzi.wav"},
    {{}, {}},
    6,
    {MaterialAttributes{-3151, 0.055f, 1.096f, 3}, MaterialAttributes{-548, 0.25f, 0.5f, 3}, MaterialAttributes{-1233, 0.25f, 0.5f, 3}, MaterialAttributes{-1918, 0.25f, 0.5f, 3}, MaterialAttributes{-3014, 0.25f, 0.5f, 3}, MaterialAttributes{-3973, 0.25f, 0.5f, 3}},
    {"Liquid", "Occlude1", "Occlude2", "Occlude3", "Occlude4", "Occlude5"}
};

TestParameter ctfface{
    "CTF-Face.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    5,
    {EAXListenerProperties{-1000, -151, 0.0f, 7.56f, 0.91f, -626, 0.02f, 774, 0.03f, 25, 1.0f, 0.5f, -5.0f, 31}, EAXListenerProperties{-1000, -2000, 0.0f, 2.031939f, 0.5f, -2735, 0.244105f, -2195, 0.1f, 19, 57.958f, 0.21f, -5.0f, 63}, EAXListenerProperties{-1000, -300, 0.0f, 0.739001f, 0.64f, 278, 0.003839f, 1072, 0.005439f, 5, 3.711f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1507, -300, 0.0f, 1.279461f, 0.64f, -198, 0.006647f, 595, 0.009416f, 5, 6.425f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1000, -300, 0.0f, 2.31f, 0.64f, -711, 0.012f, 83, 0.017f, 5, 11.6f, 1.0f, -5.0f, 63}},
    {"Outer space", "Outside", "Small room", "Medium room", "Large room"},
    0,
    {},
    {},
    {},
    1,
    {MaterialAttributes{-10000, 0.274f, 10.0f, 3}},
    {"Tower walls"}
};

TestParameter ctfgauntlet{
    "CTF-Gauntlet.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    9,
    {EAXListenerProperties{-1500, -300, 0.0f, 1.121639f, 0.59f, -972, 0.005269f, 687, 0.008281f, 12, 1.355f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1500, -300, 0.0f, 1.49f, 0.59f, -1219, 0.007f, 441, 0.011f, 12, 1.8f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1500, -476, 0.0f, 2.6408f, 0.59f, -361, 0.012226f, 138, 0.018339f, 6, 13.204f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1500, -476, 0.0f, 1.285f, 0.59f, 264, 0.005949f, 764, 0.008924f, 6, 6.425f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1500, -300, 0.0f, 3.97997f, 0.64f, -507, 0.058f, -388, 0.02929f, 5, 19.986f, 1.0f, -5.0f, 63}, EAXListenerProperties{-4932, -454, 0.0f, 1.463f, 0.647f, -3219, 0.001053f, -1945, 0.001579f, 2, 1.0f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1500, -300, 0.0f, 0.739001f, 0.64f, 278, 0.003839f, 1072, 0.005439f, 5, 3.711f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1500, -300, 0.0f, 0.46897f, 0.64f, 673, 0.002436f, 1467, 0.003451f, 5, 2.355f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1000, -4000, 0.0f, 1.49f, 0.1f, -449, 0.007f, 1700, 0.011f, 22, 1.8f, 1.0f, -5.0f, 63}},
    {"Tunnel", "Tunnel mouth", "Large chamber", "Small chamber", "Big room", "Underground", "Small room", "Tiny room", "In the lava aargh!"},
    1,
    {SourceAttributes{EAXBufferProperties{1000, 0, 1000, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"stone04.wav"},
    {{}},
    1,
    {MaterialAttributes{-685, 1.0f, 0.0f, 1}},
    {"Walls"}
};

TestParameter ctflavagiant{
    "CTF-LavaGiant.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    5,
    {EAXListenerProperties{-1200, -1100, 0.0f, 2.553f, 0.83f, -1863, 0.132f, -301, 0.025f, 18, 17.5f, 0.685f, -5.0f, 63}, EAXListenerProperties{-1900, -237, 0.0f, 1.0f, 0.79f, -350, 0.004815f, 1257, 0.007407f, 13, 5.0f, 0.877f, -5.0f, 63}, EAXListenerProperties{-3151, -237, 0.0f, 1.0f, 0.79f, 548, 0.029f, 1257, 0.007407f, 13, 5.0f, 0.836f, -5.0f, 63}, EAXListenerProperties{-1781, -959, 0.0f, 0.796552f, 0.64f, 1000, 0.021f, 521, 0.005862f, 5, 4.0f, 0.589f, -5.0f, 63}, EAXListenerProperties{-2107, -1370, 0.0f, 2.101f, 0.7f, -688, 0.010719f, 539, 0.015543f, 7, 10.505f, 0.836f, -5.0f, 63}},
    {"Quarry", "Stone corridor", "Stone corridor light", "Under stairs", "inner base"},
    2,
    {SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}, SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"Footsteps.wav", "EMOD_DOG.wav"},
    {{{-2089.275391f, -565.761658f, -1054.709961f}, {-3936.774658f, 349.271942f, -440.669617f}, {4985.162598f, 189.072998f, -1131.110229f}, {5183.341797f, 135.751862f, -44.264244f}}, {}},
    3,
    {MaterialAttributes{-2192, 0.25f, 0.5f, 3}, MaterialAttributes{-1644, 0.25f, 0.0f, 1}, MaterialAttributes{-1400, 0.25f, 0.5f, 3}},
    {"for under stairs", "Obstruction for tunnels", "generic light occlusion"}
};

TestParameter ctfniven{
    "CTF-Niven.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    5,
    {EAXListenerProperties{-822, -500, 0.0f, 3.371f, 0.595f, -808, 0.037f, -795, 0.029f, 7, 19.6f, 0.904f, -5.0f, 32}, EAXListenerProperties{-1096, -548, 0.0f, 3.099f, 0.777f, 96, 0.021f, -301, 0.026f, 5, 11.6f, 0.808f, -5.0f, 63}, EAXListenerProperties{-1233, -822, 0.0f, 3.099f, 0.256f, 699, 0.012f, 356, 0.021f, 21, 1.7f, 0.8f, -5.0f, 63}, EAXListenerProperties{-1096, -274, 0.0f, 1.463f, 0.699f, 397, 0.008f, 192, 0.008f, 7, 19.6f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1096, -4000, 0.0f, 1.49f, 0.1f, -449, 0.007f, 1342, 0.011f, 22, 1.8f, 1.0f, -5.0f, 63}},
    {"Hallway", "Bunker", "Pipe", "Tight Space", "Under Wasser"},
    4,
    {SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}, SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}, SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}, SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"man-newsgood.wav", "door.wav", "footsteps.wav", "bubbles.wav"},
    {{}, {}, {}, {}},
    6,
    {MaterialAttributes{-3151, 0.055f, 1.096f, 3}, MaterialAttributes{-548, 0.25f, 0.5f, 3}, MaterialAttributes{-1233, 0.25f, 0.5f, 3}, MaterialAttributes{-1918, 0.25f, 0.5f, 3}, MaterialAttributes{-3014, 0.25f, 0.5f, 3}, MaterialAttributes{-3973, 0.25f, 0.5f, 3}},
    {"Liquid", "Occlude1", "Occlude2", "Occlude3", "Occlude4", "Occlude5"}
};

TestParameter ctfnovember{
    "CTF-November.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    7,
    {EAXListenerProperties{-1233, -274, 0.0f, 4.734f, 1.427f, -356, 0.053f, 27, 0.04f, 7, 19.6f, 0.712f, -5.0f, 63}, EAXListenerProperties{-1233, -4000, 0.0f, 1.49f, 0.1f, -449, 0.007f, 1342, 0.011f, 22, 1.8f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1233, -822, 0.0f, 3.099f, 0.256f, 548, 0.012f, 521, 0.021f, 21, 1.7f, 0.8f, -5.0f, 63}, EAXListenerProperties{-1233, -500, 0.0f, 1.736f, 0.7f, -507, 0.012f, 356, 0.01f, 7, 19.6f, 0.753f, -5.0f, 63}, EAXListenerProperties{-1233, -500, 0.0f, 3.92f, 0.7f, -959, 0.037f, 27, 0.029f, 7, 19.6f, 0.904f, -5.0f, 63}, EAXListenerProperties{-1233, -274, 0.0f, 1.463f, 0.699f, 397, 0.008f, 521, 0.004f, 7, 19.6f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1233, -137, 0.0f, 2.007f, 0.64f, 96, 0.008f, 27, 0.017f, 5, 11.6f, 0.918f, -5.0f, 63}},
    {"Swimming Pool Stylee", "Under Wasser", "Pipe", "Small chamber", "Large Chamber", "Tight Space", "Bunker"},
    7,
    {SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}, SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}, SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}, SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}, SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}, SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}, SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"thumb.wav", "grass.wav", "machine01.wav", "man-newsgood.wav", "monster02.wav", "servo02.wav", "footsteps.wav"},
    {{}, {}, {}, {}, {}, {}, {}},
    6,
    {MaterialAttributes{-3973, 0.25f, 0.5f, 3}, MaterialAttributes{-548, 0.25f, 0.5f, 3}, MaterialAttributes{-1233, 0.25f, 0.5f, 3}, MaterialAttributes{-1918, 0.25f, 0.5f, 3}, MaterialAttributes{-3014, 0.25f, 0.5f, 3}, MaterialAttributes{-3151, 0.055f, 1.096f, 3}},
    {"Occlude5", "Occlude1", "Occlude2", "Occlude3", "Occlude4", "Liquid"}
};

TestParameter dmbarricade{
    "DM-Barricade.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    8,
    {EAXListenerProperties{-1096, -1233, 0.0f, 2.826f, 0.907f, -658, 0.062f, -959, 0.008f, 17, 100.0f, 0.644f, -5.0f, 31}, EAXListenerProperties{-1918, -237, 0.0f, 2.826f, 0.855f, 548, 0.004f, 521, 0.02f, 13, 13.5f, 0.863f, -5.0f, 0}, EAXListenerProperties{-2055, -822, 0.0f, 1.736f, 0.959f, 699, 0.07f, -795, 0.025f, 18, 17.5f, 0.507f, -5.0f, 63}, EAXListenerProperties{-1370, -1507, 0.0f, 4.462f, 0.36f, 397, 0.012f, -137, 0.02f, 13, 13.5f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1644, -548, 0.0f, 2.826f, 0.542f, 397, 0.012f, 192, 0.02f, 13, 13.5f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1644, -274, 0.0f, 2.281f, 0.647f, 397, 0.012f, 521, 0.02f, 13, 13.5f, 1.0f, -5.0f, 63}, EAXListenerProperties{-959, -454, 0.0f, 0.645f, 0.83f, -1646, 0.002f, 521, 0.003f, 2, 1.9f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1233, -274, 0.0f, 2.826f, 0.647f, 1000, 0.008f, -301, 0.02f, 13, 13.5f, 1.0f, -5.0f, 63}},
    {"Outside", "Tower", "Courtyard", "Deep Dungeon", "Shallow Dungeon", "Castle Rooms", "Wooden Room", "Castle Passages"},
    3,
    {SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}, SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}, SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"servo02.wav", "footsteps.wav", "door.wav"},
    {{}, {}, {}},
    2,
    {MaterialAttributes{-822, 0.247f, 0.5f, 3}, MaterialAttributes{-1781, 0.247f, 0.5f, 3}},
    {"Small Occlusion", "Big Occlusion"}
};

TestParameter dmcodex{
    "DM-Codex.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    4,
    {EAXListenerProperties{-137, -1644, 0.0f, 1.463f, 0.83f, -1411, 0.033f, 183, 0.003f, 2, 1.9f, 1.0f, -5.0f, 63}, EAXListenerProperties{-274, -454, 0.0f, 0.4f, 0.83f, -1646, 0.002f, 53, 0.003f, 2, 1.9f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1233, 0, 0.0f, 1.736f, 0.49f, 96, 0.021f, 192, 0.02f, 13, 13.5f, 1.0f, -5.0f, 63}, EAXListenerProperties{-274, 0, 0.0f, 1.65f, 0.725f, 548, 0.008f, -466, 0.012f, 20, 8.3f, 0.685f, -5.0f, 31}},
    {"Biggest", "Elevator", "Corridors", "tight hall"},
    1,
    {SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 27.398001f, 1000000.0f, 1, 0}},
    {"Drum.wav"},
    {{}},
    1,
    {MaterialAttributes{-3699, 0.25f, 0.0f, 1}},
    {"Boxes"}
};

TestParameter dmconveyor{
    "DM-Conveyor.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    7,
    {EAXListenerProperties{-1233, -1233, 0.0f, 3.099f, 0.308f, 397, 0.004f, 356, 0.021f, 21, 1.7f, 0.89f, -5.0f, 63}, EAXListenerProperties{-1096, -274, 0.0f, 6.915f, 0.725f, -55, 0.053f, 27, 0.033f, 9, 36.200001f, 1.0f, -5.0f, 31}, EAXListenerProperties{-959, -237, 0.0f, 3.644f, 0.79f, 96, 0.013f, -466, 0.02f, 13, 13.5f, 0.767f, -5.0f, 63}, EAXListenerProperties{-959, -237, 0.0f, 2.007f, 0.79f, 247, 0.008f, -137, 0.02f, 13, 13.5f, 0.767f, -5.0f, 63}, EAXListenerProperties{-1096, -237, 0.0f, 3.371f, 1.349f, -205, 0.013f, -137, 0.02f, 13, 13.5f, 0.712f, -5.0f, 63}, EAXListenerProperties{-1096, -237, 0.0f, 1.736f, 0.725f, 548, 0.004f, -137, 0.02f, 13, 13.5f, 0.575f, -5.0f, 63}, EAXListenerProperties{-1370, -300, 0.0f, 3.099f, 0.64f, 397, 0.012f, 356, 0.017f, 5, 11.6f, 0.918f, -5.0f, 63}},
    {"In The Lift", "Largest Hall", "Large Passages", "Smaller Passageways", "Stairwell", "Alcoves", "Room"},
    1,
    {SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"footsteps.wav"},
    {{}},
    2,
    {MaterialAttributes{-959, 0.342f, 0.5f, 3}, MaterialAttributes{-3014, 0.082f, 0.5f, 3}},
    {"Standard Occlusion", "UnderLava !"}
};

TestParameter dmcurse{
    "DM-Curse][.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    9,
    {EAXListenerProperties{-1370, -300, 0.0f, 0.918f, 0.933f, -356, 0.003839f, 849, 0.005439f, 5, 3.711f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1233, -300, 0.0f, 1.463f, 0.829f, -55, 0.003839f, 685, 0.005439f, 5, 3.711f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1507, -1000, 0.0f, 1.49f, 0.83f, -3219, 0.021f, 27, 0.052f, 18, 17.5f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1000, -685, 0.0f, 3.371f, 0.699f, -55, 0.013f, -137, 0.02f, 13, 13.5f, 0.89f, -5.0f, 63}, EAXListenerProperties{-1370, -685, 0.0f, 2.007f, 0.699f, 96, 0.013f, 356, 0.02f, 13, 13.5f, 0.699f, -5.0f, 63}, EAXListenerProperties{-548, -454, 0.0f, 0.645f, 0.83f, -1646, 0.002f, 53, 0.003f, 2, 1.9f, 1.0f, -5.0f, 63}, EAXListenerProperties{-137, -2055, 0.0f, 0.4f, 0.83f, -55, 0.002f, 53, 0.003f, 2, 1.9f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1233, -454, 0.0f, 0.918f, 0.83f, -1646, 0.002f, 849, 0.003f, 2, 1.9f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1370, -685, 0.0f, 1.358367f, 0.699f, 435, 0.008799f, 695, 0.013536f, 13, 9.137f, 0.877f, -5.0f, 63}},
    {"Small Alcoves", "Medium Alcoves", "Rooftop Area", "Big Stone Corridor", "Stone Corridors", "Wood1", "Wood2", "wood3", "Tight Corridor"},
    1,
    {SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"footsteps.wav"},
    {{}},
    2,
    {MaterialAttributes{-1096, 0.25f, 0.5f, 3}, MaterialAttributes{-2192, 0.25f, 0.5f, 3}},
    {"Small Occlusion", "Large Occlusion"}
};

TestParameter dmdeck16{
    "DM-Deck16][.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    6,
    {EAXListenerProperties{-1000, -476, 0.0f, 4.32f, 0.985f, 397, 0.029f, -137, 0.03f, 6, 21.6f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1370, -476, 0.0f, 4.32f, 0.49f, -789, 0.02f, -289, 0.03f, 6, 21.6f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1000, -100, 0.0f, 3.371f, 0.83f, -4091, 0.0f, 356, 0.061138f, 0, 41.685001f, 1.0f, -5.0f, 63}, EAXListenerProperties{-822, -300, 0.0f, 3.099f, 0.568f, -507, 0.008f, -1123, 0.0f, 5, 40.328999f, 0.0f, -5.0f, 63}, EAXListenerProperties{-1507, -2603, 0.0f, 1.19f, 0.907f, -205, 0.049f, 1836, 0.001466f, 5, 1.0f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1000, -4000, 0.0f, 1.49f, 0.1f, -449, 0.007f, 1700, 0.011f, 22, 1.8f, 1.0f, -5.0f, 63}},
    {"Big Verb, High Refl", "Soft Verb", "Big Verb, Lo Refl", "Light", "Hi Refl", "Underwata"},
    2,
    {SourceAttributes{EAXBufferProperties{0, 0, -55, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000000.0f, 1, 0}, SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"footsteps", "Drum.wav"},
    {{}, {}},
    3,
    {MaterialAttributes{-3699, 0.25f, 0.0f, 1}, MaterialAttributes{-4795, 0.0f, 0.0f, 1}, MaterialAttributes{-4247, 0.397f, 0.5f, 3}},
    {"Boxes", "Underwata", "Water Occlude"}
};

TestParameter dmfetid{
    "DM-Fetid.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    2,
    {EAXListenerProperties{-685, -2329, 0.0f, 2.281f, 0.256f, -3068, 0.025f, 1178, 0.011f, 0, 7.5f, 1.0f, -5.0f, 63}, EAXListenerProperties{-548, -1000, 0.0f, 2.81f, 0.14f, 699, 0.014f, 1023, 0.021f, 21, 1.7f, 0.8f, -5.0f, 63}},
    {"tight echo", "Tight Tube"},
    1,
    {SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"Drum.wav"},
    {{}},
    0,
    {},
    {}
};

TestParameter dmfractal{
    "DM-Fractal.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    3,
    {EAXListenerProperties{-1233, -685, 0.0f, 1.19f, 0.83f, 1000, 0.033f, 2000, 0.018f, 0, 2.356f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1507, -1507, 0.0f, 3.644f, 1.297f, -9699, 0.029f, 2000, 0.0f, 0, 2.356f, 1.0f, -5.0f, 63}, EAXListenerProperties{-137, -1233, 0.0f, 1.19f, 1.167f, -2602, 0.007f, 200, 0.011f, 0, 7.5f, 1.0f, -5.0f, 63}},
    {"Small", "Large", "Med"},
    1,
    {SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"Drum.wav"},
    {{}},
    0,
    {},
    {}
};

TestParameter dmgothic{
    "DM-Gothic.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    4,
    {EAXListenerProperties{-274, 0, 0.0f, 1.65f, 1.87f, -808, 0.008f, -1123, 0.012f, 20, 8.3f, 0.685f, -5.0f, 31}, EAXListenerProperties{-1370, -1000, 0.0f, 7.188f, 0.829f, -519, 0.053f, -137, 0.027287f, 10, 45.751999f, 0.932f, -5.0f, 0}, EAXListenerProperties{-1233, 0, 0.0f, 1.736f, 0.49f, 96, 0.021f, 192, 0.02f, 13, 13.5f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1370, -300, 0.0f, 1.736f, 1.74f, -507, 0.025f, 192, 0.01339f, 5, 9.137f, 0.918f, -5.0f, 63}},
    {"Courtyard", "Big Halls", "Corridors", "8 Sided Room"},
    1,
    {SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"footsteps.wav"},
    {{}},
    1,
    {MaterialAttributes{-1233, 0.25f, 0.5f, 3}},
    {"Standard Occlusion"}
};

TestParameter dmgrinder{
    "DM-Grinder.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    8,
    {EAXListenerProperties{-137, -2192, 0.0f, 2.7f, 0.79f, -1214, 0.013f, 395, 0.02f, 13, 13.5f, 1.0f, -5.0f, 63}, EAXListenerProperties{-685, -1507, 0.0f, 1.19f, 0.59f, 1000, 0.007f, 521, 0.011f, 12, 19.986f, 0.301f, -5.0f, 32}, EAXListenerProperties{-137, -698, 0.0f, 2.826f, 0.33f, -205, 0.02f, 16, 0.03f, 9, 36.200001f, 0.63f, -5.0f, 63}, EAXListenerProperties{0, -4384, 0.0f, 2.7f, 0.79f, 1000, 0.013f, 27, 0.02f, 13, 13.5f, 1.0f, -5.0f, 63}, EAXListenerProperties{-959, -4658, 0.0f, 4.462f, 0.23f, -2003, 0.022506f, 94, 0.033759f, 10, 56.603001f, 1.0f, -5.0f, 159}, EAXListenerProperties{-411, -698, 0.0f, 1.285f, 0.33f, -2616, 0.00355f, 356, 0.005325f, 9, 6.425f, 1.0f, -5.0f, 159}, EAXListenerProperties{-411, -1370, 0.0f, 1.49f, 0.86f, 1000, 0.021f, -466, 0.011f, 14, 7.5f, 0.3f, -5.0f, 63}, EAXListenerProperties{-1370, -698, 0.0f, 3.916f, 0.33f, -1166, 0.02f, 685, 0.03f, 9, 36.200001f, 1.0f, -5.0f, 63}},
    {"Med Verb", "Tight Verb", "Med Verb, Lo Relf", "Small verb", "Jail Cell", "Small Verb", "Hi Refl", "Big Verb"},
    1,
    {SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"Drum.wav"},
    {{}},
    0,
    {},
    {}
};

TestParameter dmhyperblast{
    "DM-Hyperblast.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    5,
    {EAXListenerProperties{-822, -3425, 0.0f, 9.914f, 0.985f, -356, 0.3f, 192, 0.1f, 25, 1.0f, 0.5f, -5.0f, 31}, EAXListenerProperties{-1370, -1000, 0.0f, 3.371f, 0.14f, -55, 0.029f, 685, 0.012f, 21, 1.7f, 0.493f, -5.0f, 63}, EAXListenerProperties{-1507, -1200, 0.0f, 3.916f, 0.54f, -55, 0.021f, 356, 0.011f, 3, 1.4f, 0.808f, -5.0f, 63}, EAXListenerProperties{-1000, -1370, 0.0f, 3.099f, 1.115f, -1110, 0.016f, 27, 0.027f, 4, 2.5f, 0.808f, -5.0f, 31}, EAXListenerProperties{-548, -1781, 0.0f, 2.007f, 0.1f, 699, 0.001f, 521, 0.001f, 1, 1.4f, 1.0f, -5.0f, 63}},
    {"Outer Space", "Curved Corridor", "Straight Corridor", "Room", "Tiny Space"},
    3,
    {SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}, SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}, SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"footsteps.wav", "elecloop.wav", "servo01.wav"},
    {{}, {}, {}},
    4,
    {MaterialAttributes{-1644, 0.25f, 0.5f, 3}, MaterialAttributes{-1370, 0.25f, 0.0f, 1}, MaterialAttributes{-3151, 0.342f, 0.0f, 3}, MaterialAttributes{-685, 0.25f, 0.5f, 3}},
    {"Standard Occlusion", "Standard Obstruction", "Space Occludes", "Small Occlusion"}
};

TestParameter dmkgalleon{
    "DM-Kgalleon.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    5,
    {EAXListenerProperties{-1000, -4000, 0.0f, 1.49f, 0.1f, -449, 0.007f, 1700, 0.011f, 22, 1.8f, 1.0f, -5.0f, 63}, EAXListenerProperties{-959, -2500, 0.0f, 1.49f, 0.21f, -2780, 0.0f, -1945, 0.0f, 17, 100.0f, 0.27f, -5.0f, 31}, EAXListenerProperties{-137, -2055, 0.0f, 0.4f, 0.83f, -55, 0.002f, 53, 0.003f, 2, 1.9f, 1.0f, -5.0f, 63}, EAXListenerProperties{-274, -454, 0.0f, 0.4f, 0.83f, -1646, 0.002f, 53, 0.003f, 2, 1.9f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1000, -454, 0.0f, 0.4f, 0.83f, -1646, 0.002f, 849, 0.003f, 2, 1.9f, 1.0f, -5.0f, 63}},
    {"Underwata", "Outside", "Wood2", "Wood1", "wood3"},
    1,
    {SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"Drum.wav"},
    {{}},
    3,
    {MaterialAttributes{-3699, 0.25f, 0.0f, 1}, MaterialAttributes{-4795, 0.0f, 0.0f, 1}, MaterialAttributes{-3699, 0.25f, 0.0f, 1}},
    {"Boxes", "Underwata", "Obstacle1"}
};

TestParameter dmliandri{
    "DM-Liandri.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    5,
    {EAXListenerProperties{-1000, -1000, 0.0f, 6.37f, 0.803f, -55, 0.09f, 27, 0.021f, 21, 9.137f, 0.8f, -5.0f, 32}, EAXListenerProperties{-1233, -1000, 0.0f, 1.463f, 0.595f, 397, 0.004f, 1178, 0.021f, 21, 1.7f, 0.8f, -5.0f, 63}, EAXListenerProperties{-822, -1000, 0.0f, 4.734f, 0.542f, 548, 0.014f, 192, 0.048f, 21, 1.7f, 0.8f, -5.0f, 0}, EAXListenerProperties{-1096, -548, 0.0f, 2.281f, 0.49f, 247, 0.016f, 192, 0.021f, 21, 1.7f, 0.726f, -5.0f, 63}, EAXListenerProperties{-1233, -959, 0.0f, 3.949564f, 0.542f, -1216, 0.018555f, 192, 0.029158f, 3, 3.711f, 0.795f, -5.0f, 63}},
    {"Central Core", "In The Lift", "Stairwell", "Corridor", "A room"},
    1,
    {SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"footsteps.wav"},
    {{}},
    1,
    {MaterialAttributes{-1096, 0.274f, 0.5f, 3}},
    {"Standard Occlusion"}
};

TestParameter dmmorbias{
    "DM-Morbias][.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 0.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    4,
    {EAXListenerProperties{-1507, -237, 0.0f, 1.736f, 0.647f, 247, 0.07f, -137, 0.014f, 13, 9.137f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1096, -500, 0.0f, 3.92f, 0.7f, -1230, 0.02f, -2, 0.029f, 7, 19.6f, 0.836f, -5.0f, 63}, EAXListenerProperties{-1370, -1233, 0.0f, 1.949417f, 0.59f, 397, 0.032708f, 192, 0.014392f, 12, 2.355f, 0.685f, -5.0f, 63}, EAXListenerProperties{-1096, -1507, 0.0f, 2.3694f, 0.7f, -507, 0.021f, -137, 0.017529f, 7, 11.847f, 0.836f, -5.0f, 63}},
    {"Corridor", "Main hall", "Chamber", "balcony"},
    1,
    {SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"footsteps.wav"},
    {{}},
    3,
    {MaterialAttributes{-1233, 0.25f, 0.5f, 3}, MaterialAttributes{-2329, 0.25f, 0.5f, 3}, MaterialAttributes{-685, 0.329f, 0.5f, 3}},
    {"Weak", "Strong", "from balcony"}
};

TestParameter dmmorpheus{
    "DM-Morpheus.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    16,
    {EAXListenerProperties{-1000, 0, 0.0f, 3.099f, 2.0f, -626, 0.02f, 774, 0.03f, 25, 1.0f, 0.5f, -100.0f, 31}, EAXListenerProperties{-1000, -1000, 0.0f, 2.81f, 0.14f, 429, 0.014f, -3260, 0.021f, 21, 1.7f, 0.8f, -5.0f, 159}, EAXListenerProperties{-1000, -274, 0.0f, 1.49f, 2.0f, -2560, 0.162f, -1123, 0.088f, 15, 38.0f, 1.0f, -5.0f, 159}, EAXListenerProperties{-1000, 0, 0.0f, 1.49f, 0.334f, -2164, 0.222f, 1014, 0.1f, 17, 100.0f, 0.534f, -5.0f, 31}, EAXListenerProperties{-1000, -300, 0.0f, 4.734f, 0.59f, -3521, 0.0f, -466, 0.1f, 12, 26.767f, 1.0f, -5.0f, 159}, EAXListenerProperties{-1000, -476, 0.0f, 4.32f, 0.985f, -789, 0.02f, -289, 0.03f, 6, 21.6f, 1.0f, -5.0f, 159}, EAXListenerProperties{-1370, -476, 0.0f, 5.007f, 0.59f, 397, 0.049f, -289, 0.03f, 6, 21.6f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1370, -100, 0.0f, 1.49f, 0.83f, -2602, 0.007f, 200, 0.011f, 0, 7.5f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1000, -1200, 0.0f, 1.49f, 0.126f, -2616, 0.053f, 1030, 0.011f, 3, 1.4f, 1.0f, -5.0f, 159}, EAXListenerProperties{-1096, 0, 0.0f, 3.916f, 2.0f, -2602, 0.007f, 200, 0.011f, 0, 7.5f, 1.0f, -5.0f, 63}, EAXListenerProperties{-2055, -151, 0.0f, 7.56f, 0.91f, -356, 0.255f, 774, 0.03f, 25, 1.0f, 0.5f, -5.0f, 31}, EAXListenerProperties{-685, 0, 0.0f, 8.005f, 2.0f, -1713, 0.0f, -466, 0.0f, 24, 1.8f, 0.6f, -5.0f, 31}, EAXListenerProperties{-1000, -100, 0.0f, 3.371f, 0.83f, -4091, 0.038906f, -301, 0.061138f, 0, 41.685001f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1000, -2500, 0.0f, 8.005f, 0.21f, -658, 0.197f, -1434, 0.1f, 17, 100.0f, 0.27f, -5.0f, 31}, EAXListenerProperties{-1000, -4000, 0.0f, 0.3f, 0.1f, 1000, 0.002f, -1630, 0.03f, 11, 1.9f, 1.0f, -5.0f, 159}, EAXListenerProperties{-1918, -300, 0.0f, 0.199138f, 0.64f, 1000, 0.008f, 2000, 0.001466f, 5, 1.0f, 1.0f, -5.0f, 159}},
    {"Gonna Hurl", "In A Box", "One Good Slap", "Stadium PA", "Subway", "Big Warehouse", "Grand Central Station", "In the Stall", "Metal Cave", "Sibilance", "Simply Wrong", "Sizzlin' Chatter", "Tiled hall", "Echoey Tails", "REAL Small Room", "Tin Can"},
    1,
    {SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"Drum.wav"},
    {{}},
    0,
    {},
    {}
};

TestParameter dmoblivion{
    "DM-Oblivion.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    5,
    {EAXListenerProperties{-1096, -100, 0.0f, 0.918f, 0.985f, -356, 0.007f, 27, 0.011f, 0, 7.5f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1370, 0, 0.0f, 2.281f, 1.5f, -507, 0.008f, -630, 0.012f, 20, 8.3f, 0.63f, -5.0f, 31}, EAXListenerProperties{-1233, 0, 0.0f, 2.826f, 0.178f, -1411, 0.025f, -466, 0.004f, 4, 2.5f, 0.945f, -5.0f, 2}, EAXListenerProperties{-1233, -5753, 0.0f, 1.463f, 2.0f, 548, 0.016f, 27, 0.002f, 1, 1.4f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1096, -3014, 0.0f, 2.007f, 0.438f, -356, 0.012f, -466, 0.017f, 5, 11.6f, 0.644f, -5.0f, 63}},
    {"Passage", "Storage", "ControlRoom", "In The Box !", "Other Rooms"},
    1,
    {SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"footsteps.wav"},
    {{}},
    3,
    {MaterialAttributes{-1233, 0.205f, 0.5f, 3}, MaterialAttributes{-2603, 0.205f, 0.0f, 1}, MaterialAttributes{-3288, 0.301f, 0.548f, 3}},
    {"Standard Occlusion", "Standard Obstruction", "Box Occlusion"}
};

TestParameter dmpeak{
    "DM-Peak.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    8,
    {EAXListenerProperties{-959, -2500, 0.0f, 1.49f, 0.21f, -2780, 0.0f, -1945, 0.0f, 17, 100.0f, 0.27f, -5.0f, 31}, EAXListenerProperties{-274, -454, 0.0f, 0.4f, 0.83f, -1646, 0.002f, 53, 0.003f, 2, 1.9f, 1.0f, -5.0f, 63}, EAXListenerProperties{-137, -2055, 0.0f, 0.4f, 0.83f, -55, 0.002f, 53, 0.003f, 2, 1.9f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1000, -454, 0.0f, 0.4f, 0.83f, -1646, 0.002f, 849, 0.003f, 2, 1.9f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1233, -1000, 0.0f, 1.736f, 0.829f, -356, 0.053f, 1836, 0.027287f, 10, 45.751999f, 0.932f, -5.0f, 0}, EAXListenerProperties{-1233, 0, 0.0f, 1.736f, 0.49f, 96, 0.021f, 192, 0.02f, 13, 13.5f, 1.0f, -5.0f, 63}, EAXListenerProperties{-274, 0, 0.0f, 1.65f, 0.725f, 548, 0.008f, -466, 0.012f, 20, 8.3f, 0.685f, -5.0f, 31}, EAXListenerProperties{-1096, -476, 0.0f, 4.32f, 0.59f, -789, 0.02f, -289, 0.03f, 6, 21.6f, 1.0f, -5.0f, 63}},
    {"Outside", "Wood1", "Wood2", "wood3", "Big Halls", "Corridors", "tight hall", "Fallin"},
    1,
    {SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"Drum.wav"},
    {{}},
    2,
    {MaterialAttributes{-3699, 0.25f, 0.0f, 1}, MaterialAttributes{-3699, 0.25f, 0.0f, 3}},
    {"Boxes", "stone occlusion"}
};

TestParameter dmphobos{
    "DM-Phobos.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    9,
    {EAXListenerProperties{-1000, -3562, 0.0f, 20.0f, 1.271f, -10000, 0.0f, 27, 0.0f, 17, 100.0f, 0.781f, -5.0f, 31}, EAXListenerProperties{-1233, -300, 0.0f, 1.009032f, 0.725f, 247, 0.004f, 1014, 0.007426f, 5, 5.067f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1000, -237, 0.0f, 3.1836f, 0.79f, -1357, 0.015328f, 192, 0.023582f, 13, 15.918f, 0.781f, -5.0f, 63}, EAXListenerProperties{-1000, -237, 0.0f, 2.007f, 0.79f, -1100, 0.011408f, 521, 0.017551f, 13, 11.847f, 0.685f, -5.0f, 63}, EAXListenerProperties{-1000, -698, 0.0f, 6.097f, 0.33f, 548, 0.008f, 192, 0.022183f, 9, 26.767f, 0.822f, -5.0f, 63}, EAXListenerProperties{-1000, -237, 0.0f, 1.285f, 0.79f, -55, 0.006187f, 521, 0.009519f, 13, 6.425f, 0.685f, -5.0f, 63}, EAXListenerProperties{-1370, -300, 0.0f, 2.281f, 0.959f, 397, 0.008049f, 192, 0.011403f, 5, 7.781f, 0.849f, -5.0f, 63}, EAXListenerProperties{-1000, -959, 0.0f, 4.734f, 0.699f, -2164, 0.14f, 27, 0.085f, 0, 2.155f, 1.0f, -5.0f, 0}, EAXListenerProperties{-1000, -237, 0.0f, 4.189f, 0.79f, 247, 0.008f, 27, 0.019563f, 13, 3.711f, 1.0f, -5.0f, 0}},
    {"Outside", "Lift Shaft", "Stairwells", "Smaller Stairwells", "Centre Shaft", "Alcoves", "Room areas", "Bay", "Corridor"},
    3,
    {SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}, SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}, SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"ES2_GUN1.wav", "EP2_Bees.wav", "footsteps.wav"},
    {{}, {}, {}},
    1,
    {MaterialAttributes{-1507, 0.25f, 0.5f, 3}},
    {"Standard Occlusion"}
};

TestParameter dmpressure{
    "DM-Pressure.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    5,
    {EAXListenerProperties{-1000, -500, 0.0f, 4.462f, 1.245f, -959, 0.045f, -140, 0.03f, 7, 19.6f, 0.863f, -5.0f, 31}, EAXListenerProperties{-1233, -1200, 0.0f, 2.007f, 0.54f, -205, 0.037f, 190, 0.019f, 3, 1.4f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1644, -151, 0.0f, 2.553f, 0.829f, -10000, 0.004f, 520, 0.1f, 25, 1.0f, 1.0f, -5.0f, 31}, EAXListenerProperties{-1507, -959, 0.0f, 1.49f, 0.647f, -205, 0.016f, 520, 0.011f, 3, 1.4f, 1.0f, -5.0f, 31}, EAXListenerProperties{-1233, -3973, 0.0f, 1.463f, 0.1f, -2466, 0.053f, 1010, 0.0f, 22, 1.8f, 1.0f, -5.0f, 31}},
    {"Great Big Hall", "Large Passage", "Pressure Room", "Small Passage", "Under the water"},
    0,
    {},
    {},
    {},
    4,
    {MaterialAttributes{-1370, 0.205f, 0.411f, 3}, MaterialAttributes{-3151, 0.055f, 1.096f, 3}, MaterialAttributes{-1918, 0.041f, 0.5f, 3}, MaterialAttributes{-3836, 0.0f, 1.37f, 3}},
    {"Adjoining rooms", "Pressure Room Walls", "Stone Walls", "Water Tank Walls"}
};

TestParameter dmpyramid{
    "DM-Pyramid.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    3,
    {EAXListenerProperties{0, -685, 2.055f, 3.099f, 0.568f, -4123, 0.0f, -795, 0.044f, 17, 100.0f, 0.27f, 0.0f, 31}, EAXListenerProperties{-1000, -151, 0.0f, 2.008f, 0.91f, -626, 0.02f, 849, 0.03f, 25, 1.0f, 0.5f, -5.0f, 31}, EAXListenerProperties{-2055, -151, 0.0f, 7.56f, 0.91f, -356, 0.255f, 774, 0.03f, 25, 1.0f, 0.5f, -5.0f, 31}},
    {"Space", "All Over the Place", "Simply Wrong"},
    1,
    {SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"Drum.wav"},
    {{}},
    0,
    {},
    {}
};

TestParameter dmstalwart{
    "DM-Stalwart.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    13,
    {EAXListenerProperties{-1000, -3562, 0.0f, 10.05f, 0.1f, -602, 0.02f, 198, 0.03f, 10, 50.299999f, 0.548f, -5.0f, 159}, EAXListenerProperties{-1000, 0, 0.0f, 2.91f, 0.464f, -602, 0.015f, -302, 0.022f, 8, 14.6f, 1.0f, -5.0f, 31}, EAXListenerProperties{-137, -3425, 0.0f, 4.32f, 0.59f, 247, 0.029f, -289, 0.03f, 6, 45.752998f, 0.0f, -5.0f, 32}, EAXListenerProperties{0, -3014, 0.0f, 3.644f, 1.5f, -1851, 0.014036f, -301, 0.021053f, 20, 14.562f, 1.0f, -5.0f, 31}, EAXListenerProperties{-1000, -5068, 0.0f, 9.914f, 1.115f, 247, 0.086f, -5726, 0.014f, 4, 2.5f, 1.0f, 0.0f, 159}, EAXListenerProperties{-1507, -1781, 0.0f, 7.24f, 0.256f, -1166, 0.02f, 16, 0.03f, 9, 36.200001f, 1.0f, -5.0f, 159}, EAXListenerProperties{-1000, 0, 0.0f, 2.7f, 2.0f, -1214, 0.013f, 27, 0.02f, 13, 13.5f, 1.0f, -5.0f, 159}, EAXListenerProperties{-1918, -300, 0.0f, 0.199138f, 0.64f, 1000, 0.008f, 2000, 0.001466f, 5, 1.0f, 1.0f, -5.0f, 159}, EAXListenerProperties{-1000, -800, 0.0f, 1.49f, 0.67f, 548, 0.049f, -1691, 0.011f, 16, 7.5f, 0.5f, -5.0f, 159}, EAXListenerProperties{-1233, -698, 0.0f, 7.24f, 0.33f, -1166, 0.02f, 16, 0.03f, 9, 36.200001f, 0.63f, -5.0f, 159}, EAXListenerProperties{-959, -4658, 0.0f, 4.462f, 0.23f, -2003, 0.022506f, 94, 0.033759f, 10, 56.603001f, 1.0f, -5.0f, 159}, EAXListenerProperties{-411, -698, 0.0f, 1.285f, 0.33f, -2616, 0.00355f, 356, 0.005325f, 9, 6.425f, 1.0f, -5.0f, 159}, EAXListenerProperties{-1000, -1370, 0.0f, 1.49f, 0.86f, -1110, 0.007f, -466, 0.011f, 14, 7.5f, 0.3f, -5.0f, 159}},
    {"Catacombs", "Cavemen", "Dark Corridor", "HUGE Church", "Mello Echo", "Museum", "Shimmer Verb", "Tin Can", "Close Quarters", "Hanger With Echo", "Jail Cell", "Men's Room", "Slap Happy"},
    1,
    {SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"Drum.wav"},
    {{}},
    0,
    {},
    {}
};

TestParameter dmstalwartxl{
    "DM-StalwartXL.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    6,
    {EAXListenerProperties{-1096, -274, 0.0f, 2.553f, 0.959f, -959, 0.016f, 192, 0.011f, 0, 7.5f, 1.0f, -5.0f, 63}, EAXListenerProperties{-959, -411, 0.0f, 2.007f, 1.037f, 247, 0.012f, -137, 0.011f, 0, 7.5f, 1.0f, -5.0f, 63}, EAXListenerProperties{-959, -411, 0.0f, 3.644f, 1.063f, 548, 0.012f, -301, 0.011f, 0, 7.5f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1233, -548, 0.0f, 1.736f, 1.037f, -507, 0.012f, 356, 0.011f, 0, 7.5f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1096, -822, 0.0f, 1.19f, 1.141f, 96, 0.008f, 356, 0.011f, 0, 7.5f, 1.0f, -5.0f, 63}, EAXListenerProperties{-1000, -100, 0.0f, 0.373f, 1.193f, -55, 0.004f, 192, 0.011f, 0, 7.5f, 1.0f, -5.0f, 63}},
    {"Large Concrete Room", "Corridor", "Long Corridor", "Small Concrete Room", "Tiny Concrete Room", "Box"},
    2,
    {SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}, SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"footsteps.wav", "elecloop.wav"},
    {{}, {}},
    2,
    {MaterialAttributes{-1370, 0.25f, 0.0f, 1}, MaterialAttributes{-1370, 0.25f, 0.0f, 3}},
    {"Obstruct", "Occlude"}
};

TestParameter dmtempest{
    "DM-Tempest.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    3,
    {EAXListenerProperties{-1000, -476, 0.0f, 4.32f, 0.985f, 397, 0.029f, -137, 0.03f, 6, 21.6f, 1.0f, -5.0f, 63}, EAXListenerProperties{-822, -300, 0.0f, 3.099f, 0.568f, -507, 0.008f, -1123, 0.0f, 5, 40.328999f, 0.0f, -5.0f, 63}, EAXListenerProperties{-1370, -476, 0.0f, 4.32f, 0.49f, -789, 0.02f, -289, 0.03f, 6, 21.6f, 1.0f, -5.0f, 63}},
    {"Big Verb, High Refl", "Light", "Soft Verb"},
    1,
    {SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"Drum.wav"},
    {{}},
    1,
    {MaterialAttributes{-3699, 0.25f, 0.0f, 1}},
    {"Boxes"}
};

TestParameter dmturbine{
    "DM-Turbine.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    6,
    {EAXListenerProperties{-959, 0, 0.0f, 2.007f, 1.349f, 247, 0.004f, 356, 0.003f, 13, 10.493f, 0.726f, -5.0f, 0}, EAXListenerProperties{-1233, -300, 0.0f, 1.19f, 0.803f, -55, 0.002436f, 1014, 0.003451f, 5, 2.355f, 0.877f, -5.0f, 63}, EAXListenerProperties{-1096, -548, 0.0f, 4.734f, 1.323f, -205, 0.012f, 27, 0.059f, 7, 15.918f, 1.0f, -5.0f, 32}, EAXListenerProperties{-959, -274, 0.0f, 2.826f, 0.959f, 548, 0.016f, -301, 0.015f, 5, 11.6f, 0.932f, -5.0f, 63}, EAXListenerProperties{-1096, -548, 0.0f, 2.553f, 0.933f, -205, 0.008f, 27, 0.016f, 7, 15.918f, 1.0f, -5.0f, 32}, EAXListenerProperties{-1233, -1000, 0.0f, 2.826f, 0.438f, 548, 0.016f, 356, 0.015f, 21, 1.5f, 0.671f, -5.0f, 0}},
    {"Water Area", "Alcove", "Big Room", "Low Ceiling Room", "Stairway", "Pipe"},
    1,
    {SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"footsteps.wav"},
    {{}},
    2,
    {MaterialAttributes{-548, 0.25f, 0.5f, 3}, MaterialAttributes{-1507, 0.25f, 0.5f, 3}},
    {"Small Occlusion", "Big Occlusion"}
};

TestParameter dmzeto{
    "DM-Zeto.eal",
    EAXDiffractionModel{0, 0.0f, 0},
    ListenerAttributes{1.0f, 1.0f, 1.0f},
    EAXListenerProperties{0, 0, 0.0f, 0.0f, 0.0f, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0.0f, 0},
    SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.0f, 0.0f, 0, 0.0f, 0}, 0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0},
    MaterialAttributes{0, 0.0f, 0.0f, 0},
    1,
    {EAXListenerProperties{-1507, -548, 0.0f, 6.37f, 0.751f, -205, 0.013f, 27, 0.01f, 13, 13.5f, 0.726f, -5.0f, 63}},
    {"Environment1"},
    1,
    {SourceAttributes{EAXBufferProperties{0, 0, 0, 0, 0.0f, 0, 0.0f, 0, 0.25f, 0.5f, 0, 1.0f, 7}, 360, 360, 0, 0.0f, 0.0f, -1.0f, 1.0f, 1000000.0f, 1, 0}},
    {"footsteps.wav"},
    {{}},
    2,
    {MaterialAttributes{-2192, 0.25f, 0.5f, 3}, MaterialAttributes{-2192, 0.25f, 0.5f, 3}},
    {"Occlude", "Obstruct"}
};

INSTANTIATE_TEST_CASE_P(
    AllEalFiles,
    UT99EalFiles,
    ::testing::Values(
        ctfcommand,
        ctfcoret,
        ctfdreary,
        ctfeternalcave,
        ctfface,
        ctfgauntlet,
        ctflavagiant,
        ctfniven,
        ctfnovember,
        dmbarricade,
        dmcodex,
        dmconveyor,
        dmcurse,
        dmdeck16,
        dmfetid,
        dmfractal,
        dmgothic,
        dmgrinder,
        dmhyperblast,
        dmkgalleon,
        dmliandri,
        dmmorbias,
        dmmorpheus,
        dmoblivion,
        dmpeak,
        dmphobos,
        dmpressure,
        dmpyramid,
        dmstalwart,
        dmstalwartxl,
        dmtempest,
        dmturbine,
        dmzeto
    ));