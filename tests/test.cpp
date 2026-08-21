#include <gtest/gtest.h>
#include "src/EalMan.h"
#include "EalFileContents.h"

namespace LoadDataSet
{
    namespace OriginalEALFiles
    {
        TEST_P(UT99EalFiles, LoadDataSet_ValidFile)
        {
            TestParameter par = GetParam();
            int32_t result = man.LoadDataSet(par.filename, 0);
            EXPECT_EQ(result, toInt(EalError::OK));
        }
    }

    namespace FailingEALFiles
    {
        TEST(FailingEalFiles, LoadDataSet_FileNotFound)
        {
            EalMan man;
            int32_t result = man.LoadDataSet("FileDoesNotExist.eal", 0);
            EXPECT_EQ(result, toInt(EalError::FileNotFound));
        }

        TEST(FailingEalFiles, LoadDataSet_MissingRIFFHeader)
        {
            EalMan man;
            int32_t result = man.LoadDataSet("RIFFHeader.eal", 0);
            EXPECT_EQ(result, toInt(EalError::FileInvalid));
        }

        TEST(FailingEalFiles, LoadDataSet_WrongChunkSize)
        {
            EalMan man;
            int32_t result = man.LoadDataSet("WrongMainChunkSize.eal", 0);
            EXPECT_EQ(result, toInt(EalError::FileInvalid));
        }

        TEST(FailingEalFiles, LoadDataSet_FileTypeNotEal)
        {
            EalMan man;
            int32_t result = man.LoadDataSet("FileType.eal", 0);
            EXPECT_EQ(result, toInt(EalError::FileInvalid));
        }

        TEST(FailingEalFiles, LoadDataSet_FileNameWrongExtension)
        {
            EalMan man;
            int32_t result = man.LoadDataSet("CTF-Face.bin", 0);
            EXPECT_EQ(result, toInt(EalError::OK));
        }

        TEST(FailingEalFiles, LoadDataSet_DataTypeError)
        {
            EalMan man;
            int32_t result = man.LoadDataSet("gdfmFloatNaN.eal", 0);
            EXPECT_EQ(result, toInt(EalError::FileInvalid));
        }

        TEST(FailingEalFiles, LoadDataSet_MissingRequiredChunk)
        {
            EalMan man;
            int32_t result = man.LoadDataSet("gdfmMissing.eal", 0);
            EXPECT_EQ(result, toInt(EalError::FileInvalid));
        }

        TEST(FailingEalFiles, LoadDataSet_MissingOptionalChunk)
        {
            EalMan man;
            int32_t result = man.LoadDataSet("dfilMissing.eal", 0);
            EXPECT_EQ(result, toInt(EalError::OK));
        }

        TEST(FailingEalFiles, LoadDataSet_ChunkMisnamed)
        {
            EalMan man;
            int32_t result = man.LoadDataSet("NoExep.eal", 0);
            EXPECT_EQ(result, toInt(EalError::OK));
        }

        TEST(FailingEalFiles, LoadDataSet_WrongMajv)
        {
            EalMan man;
            int32_t result = man.LoadDataSet("WrongMajv.eal", 0);
            EXPECT_EQ(result, toInt(EalError::VersionInvalid));
        }

        TEST(FailingEalFiles, LoadDataSet_WrongMinv)
        {
            EalMan man;
            int32_t result = man.LoadDataSet("WrongMinv.eal", 0);
            EXPECT_EQ(result, toInt(EalError::VersionInvalid));
        }

        TEST(FailingEalFiles, LoadDataSet_GemaChunkMissingPart)
        {
            EalMan man;
            int32_t result = man.LoadDataSet("GemaChunkMissingPart.eal", 0);
            EXPECT_EQ(result, toInt(EalError::FileInvalid));
        }

        TEST(FailingEalFiles, LoadDataSet_GemaChunkTreeSizeTooBig)
        {
            EalMan man;
            int32_t result = man.LoadDataSet("GemaChunkTreeSizeTooBig.eal", 0);
            EXPECT_EQ(result, toInt(EalError::FileInvalid));
        }
    }
}

namespace GetDataSetSize
{
    namespace OriginalEALFiles
    {
        TEST(LibUse, GetDataSetSize_CTFFace)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("CTF-Face.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            uint32_t size = 0;
            int32_t resSize = man.GetDataSetSize(size, 0);
            EXPECT_EQ(resSize, toInt(EalError::OK));
            EXPECT_GT(size, sizeof(EalData));
        }

        TEST(LibUse, GetDataSetSize_Empty)
        {
            EalMan man;
            uint32_t size = 0;
            int32_t resSize = man.GetDataSetSize(size, 0);
            EXPECT_EQ(resSize, toInt(EalError::OK));
            EXPECT_EQ(size, sizeof(EalData));
        }
    }
}

namespace FreeDataSet
{
    namespace OriginalEALFiles
    {
        TEST(LibUse, FreeDataSet_CTFFace)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("CTF-Face.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            uint32_t datasize = 0;
            int32_t resSize = man.GetDataSetSize(datasize, 0);
            ASSERT_EQ(resSize, toInt(EalError::OK));
            ASSERT_GT(datasize, sizeof(EalData));
            int32_t resFree = man.FreeDataSet(0);
            EXPECT_EQ(resFree, toInt(EalError::OK));
            uint32_t freesize = 0;
            int32_t resEmpty = man.GetDataSetSize(freesize, 0);
            EXPECT_EQ(resEmpty, toInt(EalError::OK));
            EXPECT_EQ(freesize, sizeof(EalData));
        }
    }
}

namespace GetListenerAttributes
{
    namespace OriginalEALFiles
    {
        TEST_P(UT99EalFiles, GetListenerAttributes_ValidFile)
        {
            TestParameter par = GetParam();
            int32_t resLoad = man.LoadDataSet(par.filename, 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            ListenerAttributes la{};
            int32_t resGLA = man.GetListenerAttributes(la);
            EXPECT_EQ(resGLA, toInt(EalError::OK));
            EXPECT_FLOAT_EQ(la.fDistanceFactor, par.lisa.fDistanceFactor);
            EXPECT_FLOAT_EQ(la.fDopplerFactor, par.lisa.fDopplerFactor);
            EXPECT_FLOAT_EQ(la.fRolloffFactor, par.lisa.fRolloffFactor);
        }
    }

    namespace FailingGetListenerAttributesRequests
    {
        TEST(FailingEalFiles, GetListenerAttributes_NoDataSetLoaded)
        {
            EalMan man;
            ListenerAttributes la{};
            int32_t resGLA = man.GetListenerAttributes(la);
            EXPECT_EQ(resGLA, toInt(EalError::OK));
            EXPECT_FLOAT_EQ(la.fDistanceFactor, {});
            EXPECT_FLOAT_EQ(la.fDopplerFactor, {});
            EXPECT_FLOAT_EQ(la.fRolloffFactor, {});
        }

        TEST(FailingEalFiles, GetListenerAttributes_UninitializedListenerAttributes)
        {
            EalMan man;
            ListenerAttributes la;
            int32_t resGLA = man.GetListenerAttributes(la);
            EXPECT_EQ(resGLA, toInt(EalError::OK));
            EXPECT_FLOAT_EQ(la.fDistanceFactor, {});
            EXPECT_FLOAT_EQ(la.fDopplerFactor, {});
            EXPECT_FLOAT_EQ(la.fRolloffFactor, {});
        }
    }
}

namespace GetSourceID
{
    namespace OriginalEALFiles
    {
        TEST_P(UT99EalFiles, GetSourceID_ValidFile)
        {
            TestParameter par = GetParam();
            int32_t resLoad = man.LoadDataSet(par.filename, 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            if (par.nrSrcs > 0)
            {
                for (uint32_t i = 0; i < par.nrSrcs; i++)
                {
                    ASSERT_EQ(par.nrSrcs, par.srcNames.size());
                    int32_t srcID{};
                    int32_t resGSI = man.GetSourceID(par.srcNames[i], srcID);
                    EXPECT_EQ(resGSI, toInt(EalError::OK));
                    EXPECT_EQ(srcID, i);
                }
            }
        }
    }

    namespace FailingSourceIDRequests
    {
        TEST(FailingEalFiles, GetSourceID_InvalidSourceName)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("DM-Morpheus.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            int32_t srcID{};
            int32_t resGSI = man.GetSourceID("DoesNotExist", srcID);
            EXPECT_EQ(resGSI, toInt(EalError::IdNotFound));
            EXPECT_EQ(srcID, {});
        }

        TEST(FailingEalFiles, GetSourceID_EmptySourceName)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("DM-Morpheus.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            int32_t srcID{};
            int32_t resGSI = man.GetSourceID("", srcID);
            EXPECT_EQ(resGSI, toInt(EalError::IdNotFound));
            EXPECT_EQ(srcID, {});
        }

        TEST(FailingEalFiles, GetSourceID_NoDataSetLoaded)
        {
            EalMan man;
            int32_t srcID{};
            int32_t resGSI = man.GetSourceID("DoesNotExist", srcID);
            EXPECT_EQ(resGSI, toInt(EalError::IdNotFound));
            EXPECT_EQ(srcID, {});
        }

        TEST(FailingEalFiles, GetSourceID_UninitializedSourcsID)
        {
            EalMan man;
            int32_t srcID;
            int32_t resGSI = man.GetSourceID("DoesNotExist", srcID);
            EXPECT_EQ(resGSI, toInt(EalError::IdNotFound));
            EXPECT_EQ(srcID, {});
        }
    }
}

namespace GetSourceAttributes
{
    namespace OriginalEALFiles
    {
        TEST_P(UT99EalFiles, GetSourceAttributes_ValidFile)
        {
            TestParameter par = GetParam();
            int32_t resLoad = man.LoadDataSet(par.filename, 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            if (par.nrSrcs > 0)
            {
                for (uint32_t i = 0; i < par.nrSrcs; i++)
                {
                    ASSERT_EQ(par.nrSrcs, par.srcNames.size());
                    int32_t srcID{};
                    int32_t resGSI = man.GetSourceID(par.srcNames[i], srcID);
                    ASSERT_EQ(resGSI, toInt(EalError::OK));
                    SourceAttributes sa{};
                    int32_t resGLA = man.GetSourceAttributes(srcID, sa);
                    EXPECT_EQ(resGLA, toInt(EalError::OK));
                    EXPECT_EQ(sa.eaxAttributes.lDirect, par.srcp[i].eaxAttributes.lDirect);
                    EXPECT_EQ(sa.eaxAttributes.lDirectHF, par.srcp[i].eaxAttributes.lDirectHF);
                    EXPECT_EQ(sa.eaxAttributes.lRoom, par.srcp[i].eaxAttributes.lRoom);
                    EXPECT_EQ(sa.eaxAttributes.lRoomHF, par.srcp[i].eaxAttributes.lRoomHF);
                    EXPECT_FLOAT_EQ(sa.eaxAttributes.flRoomRolloffFactor, par.srcp[i].eaxAttributes.flRoomRolloffFactor);
                    EXPECT_EQ(sa.eaxAttributes.lObstruction, par.srcp[i].eaxAttributes.lObstruction);
                    EXPECT_FLOAT_EQ(sa.eaxAttributes.flObstructionLFRatio, par.srcp[i].eaxAttributes.flObstructionLFRatio);
                    EXPECT_EQ(sa.eaxAttributes.lOcclusion, par.srcp[i].eaxAttributes.lOcclusion);
                    EXPECT_FLOAT_EQ(sa.eaxAttributes.flOcclusionLFRatio, par.srcp[i].eaxAttributes.flOcclusionLFRatio);
                    EXPECT_FLOAT_EQ(sa.eaxAttributes.flOcclusionRoomRatio, par.srcp[i].eaxAttributes.flOcclusionRoomRatio);
                    EXPECT_EQ(sa.eaxAttributes.lOutsideVolumeHF, par.srcp[i].eaxAttributes.lOutsideVolumeHF);
                    EXPECT_FLOAT_EQ(sa.eaxAttributes.flAirAbsorptionFactor, par.srcp[i].eaxAttributes.flAirAbsorptionFactor);
                    EXPECT_EQ(sa.eaxAttributes.dwFlags, par.srcp[i].eaxAttributes.dwFlags);
                    EXPECT_EQ(sa.ulInsideConeAngle, par.srcp[i].ulInsideConeAngle);
                    EXPECT_EQ(sa.ulOutsideConeAngle, par.srcp[i].ulOutsideConeAngle);
                    EXPECT_EQ(sa.ulOutsideConeAngle, par.srcp[i].ulOutsideConeAngle);
                    EXPECT_FLOAT_EQ(sa.fConeXdir, par.srcp[i].fConeXdir);
                    EXPECT_FLOAT_EQ(sa.fConeYdir, par.srcp[i].fConeYdir);
                    EXPECT_FLOAT_EQ(sa.fConeZdir, par.srcp[i].fConeZdir);
                    EXPECT_FLOAT_EQ(sa.fMinDistance, par.srcp[i].fMinDistance);
                    EXPECT_FLOAT_EQ(sa.fMaxDistance, par.srcp[i].fMaxDistance);
                    EXPECT_EQ(sa.lDupCount, par.srcp[i].lDupCount);
                    EXPECT_EQ(sa.lPriority, par.srcp[i].lPriority);
                }
            }
        }
    }

    namespace FailingGetSourceAttributesRequests
    {
        TEST(FailingEalFiles, GetSourceAttributes_SourceIDtooLow)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("DM-StalwartXL.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            SourceAttributes sa{};
            int32_t resGLA = man.GetSourceAttributes(-1, sa);
            EXPECT_EQ(resGLA, toInt(EalError::InvalidId));
            EXPECT_EQ(sa.eaxAttributes.lDirect, {});
            EXPECT_EQ(sa.eaxAttributes.lDirectHF, {});
            EXPECT_EQ(sa.eaxAttributes.lRoom, {});
            EXPECT_EQ(sa.eaxAttributes.lRoomHF, {});
            EXPECT_FLOAT_EQ(sa.eaxAttributes.flRoomRolloffFactor, {});
            EXPECT_EQ(sa.eaxAttributes.lObstruction, {});
            EXPECT_FLOAT_EQ(sa.eaxAttributes.flObstructionLFRatio, {});
            EXPECT_EQ(sa.eaxAttributes.lOcclusion, {});
            EXPECT_FLOAT_EQ(sa.eaxAttributes.flOcclusionLFRatio, {});
            EXPECT_FLOAT_EQ(sa.eaxAttributes.flOcclusionRoomRatio, {});
            EXPECT_EQ(sa.eaxAttributes.lOutsideVolumeHF, {});
            EXPECT_FLOAT_EQ(sa.eaxAttributes.flAirAbsorptionFactor, {});
            EXPECT_EQ(sa.eaxAttributes.dwFlags, {});
            EXPECT_EQ(sa.ulInsideConeAngle, {});
            EXPECT_EQ(sa.ulOutsideConeAngle, {});
            EXPECT_EQ(sa.ulOutsideConeAngle, {});
            EXPECT_FLOAT_EQ(sa.fConeXdir, {});
            EXPECT_FLOAT_EQ(sa.fConeYdir, {});
            EXPECT_FLOAT_EQ(sa.fConeZdir, {});
            EXPECT_FLOAT_EQ(sa.fMinDistance, {});
            EXPECT_FLOAT_EQ(sa.fMaxDistance, {});
            EXPECT_EQ(sa.lDupCount, {});
            EXPECT_EQ(sa.lPriority, {});
        }

        TEST(FailingEalFiles, GetSourceAttributes_SourceIDtooHigh)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("DM-Stalwart.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            SourceAttributes sa{};
            int32_t resGLA = man.GetSourceAttributes(123456, sa);
            EXPECT_EQ(resGLA, toInt(EalError::InvalidId));
            EXPECT_EQ(sa.eaxAttributes.lDirect, {});
            EXPECT_EQ(sa.eaxAttributes.lDirectHF, {});
            EXPECT_EQ(sa.eaxAttributes.lRoom, {});
            EXPECT_EQ(sa.eaxAttributes.lRoomHF, {});
            EXPECT_FLOAT_EQ(sa.eaxAttributes.flRoomRolloffFactor, {});
            EXPECT_EQ(sa.eaxAttributes.lObstruction, {});
            EXPECT_FLOAT_EQ(sa.eaxAttributes.flObstructionLFRatio, {});
            EXPECT_EQ(sa.eaxAttributes.lOcclusion, {});
            EXPECT_FLOAT_EQ(sa.eaxAttributes.flOcclusionLFRatio, {});
            EXPECT_FLOAT_EQ(sa.eaxAttributes.flOcclusionRoomRatio, {});
            EXPECT_EQ(sa.eaxAttributes.lOutsideVolumeHF, {});
            EXPECT_FLOAT_EQ(sa.eaxAttributes.flAirAbsorptionFactor, {});
            EXPECT_EQ(sa.eaxAttributes.dwFlags, {});
            EXPECT_EQ(sa.ulInsideConeAngle, {});
            EXPECT_EQ(sa.ulOutsideConeAngle, {});
            EXPECT_EQ(sa.ulOutsideConeAngle, {});
            EXPECT_FLOAT_EQ(sa.fConeXdir, {});
            EXPECT_FLOAT_EQ(sa.fConeYdir, {});
            EXPECT_FLOAT_EQ(sa.fConeZdir, {});
            EXPECT_FLOAT_EQ(sa.fMinDistance, {});
            EXPECT_FLOAT_EQ(sa.fMaxDistance, {});
            EXPECT_EQ(sa.lDupCount, {});
            EXPECT_EQ(sa.lPriority, {});
        }

        TEST(FailingEalFiles, GetSourceAttributes_NoDataSetLoaded)
        {
            EalMan man;
            SourceAttributes sa{};
            int32_t resGLA = man.GetSourceAttributes(0, sa);
            EXPECT_EQ(resGLA, toInt(EalError::InvalidId));
            EXPECT_EQ(sa.eaxAttributes.lDirect, {});
            EXPECT_EQ(sa.eaxAttributes.lDirectHF, {});
            EXPECT_EQ(sa.eaxAttributes.lRoom, {});
            EXPECT_EQ(sa.eaxAttributes.lRoomHF, {});
            EXPECT_FLOAT_EQ(sa.eaxAttributes.flRoomRolloffFactor, {});
            EXPECT_EQ(sa.eaxAttributes.lObstruction, {});
            EXPECT_FLOAT_EQ(sa.eaxAttributes.flObstructionLFRatio, {});
            EXPECT_EQ(sa.eaxAttributes.lOcclusion, {});
            EXPECT_FLOAT_EQ(sa.eaxAttributes.flOcclusionLFRatio, {});
            EXPECT_FLOAT_EQ(sa.eaxAttributes.flOcclusionRoomRatio, {});
            EXPECT_EQ(sa.eaxAttributes.lOutsideVolumeHF, {});
            EXPECT_FLOAT_EQ(sa.eaxAttributes.flAirAbsorptionFactor, {});
            EXPECT_EQ(sa.eaxAttributes.dwFlags, {});
            EXPECT_EQ(sa.ulInsideConeAngle, {});
            EXPECT_EQ(sa.ulOutsideConeAngle, {});
            EXPECT_EQ(sa.ulOutsideConeAngle, {});
            EXPECT_FLOAT_EQ(sa.fConeXdir, {});
            EXPECT_FLOAT_EQ(sa.fConeYdir, {});
            EXPECT_FLOAT_EQ(sa.fConeZdir, {});
            EXPECT_FLOAT_EQ(sa.fMinDistance, {});
            EXPECT_FLOAT_EQ(sa.fMaxDistance, {});
            EXPECT_EQ(sa.lDupCount, {});
            EXPECT_EQ(sa.lPriority, {});
        }

        TEST(FailingEalFiles, GetSourceAttributes_UninitializedSourcsID)
        {
            EalMan man;
            SourceAttributes sa;
            int32_t resGLA = man.GetSourceAttributes(0, sa);
            EXPECT_EQ(resGLA, toInt(EalError::InvalidId));
            EXPECT_EQ(sa.eaxAttributes.lDirect, {});
            EXPECT_EQ(sa.eaxAttributes.lDirectHF, {});
            EXPECT_EQ(sa.eaxAttributes.lRoom, {});
            EXPECT_EQ(sa.eaxAttributes.lRoomHF, {});
            EXPECT_FLOAT_EQ(sa.eaxAttributes.flRoomRolloffFactor, {});
            EXPECT_EQ(sa.eaxAttributes.lObstruction, {});
            EXPECT_FLOAT_EQ(sa.eaxAttributes.flObstructionLFRatio, {});
            EXPECT_EQ(sa.eaxAttributes.lOcclusion, {});
            EXPECT_FLOAT_EQ(sa.eaxAttributes.flOcclusionLFRatio, {});
            EXPECT_FLOAT_EQ(sa.eaxAttributes.flOcclusionRoomRatio, {});
            EXPECT_EQ(sa.eaxAttributes.lOutsideVolumeHF, {});
            EXPECT_FLOAT_EQ(sa.eaxAttributes.flAirAbsorptionFactor, {});
            EXPECT_EQ(sa.eaxAttributes.dwFlags, {});
            EXPECT_EQ(sa.ulInsideConeAngle, {});
            EXPECT_EQ(sa.ulOutsideConeAngle, {});
            EXPECT_EQ(sa.ulOutsideConeAngle, {});
            EXPECT_FLOAT_EQ(sa.fConeXdir, {});
            EXPECT_FLOAT_EQ(sa.fConeYdir, {});
            EXPECT_FLOAT_EQ(sa.fConeZdir, {});
            EXPECT_FLOAT_EQ(sa.fMinDistance, {});
            EXPECT_FLOAT_EQ(sa.fMaxDistance, {});
            EXPECT_EQ(sa.lDupCount, {});
            EXPECT_EQ(sa.lPriority, {});
        }
    }
}

namespace GetSourceNumInstances
{
    namespace OriginalEALFiles
    {
        TEST_P(UT99EalFiles, GetSourceNumInstances_ValidFile)
        {
            TestParameter par = GetParam();
            int32_t resLoad = man.LoadDataSet(par.filename, 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            if (par.nrSrcs > 0)
            {
                for (uint32_t i = 0; i < par.nrSrcs; i++)
                {
                    ASSERT_EQ(par.nrSrcs, par.srcNames.size());
                    ASSERT_EQ(par.nrSrcs, par.srcPos.size());
                    int32_t srcID{};
                    int32_t resGSI = man.GetSourceID(par.srcNames[i], srcID);
                    ASSERT_EQ(resGSI, toInt(EalError::OK));
                    int32_t sni{};
                    int32_t resGSNI = man.GetSourceNumInstances(srcID, sni);
                    EXPECT_EQ(resGSNI, toInt(EalError::OK));
                    EXPECT_EQ(sni, par.srcPos[i].size());
                }
            }
        }
    }

    namespace FailingSourceNumInstancesRequests
    {
        TEST(FailingEalFiles, GetSourceNumInstances_SourceIDtooLow)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("DM-Phobos.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            int32_t sni{};
            int32_t resGSNI = man.GetSourceNumInstances(-2, sni);
            EXPECT_EQ(resGSNI, toInt(EalError::InvalidId));
            EXPECT_EQ(sni, {});
        }

        TEST(FailingEalFiles, GetSourceNumInstances_SourceIDtooHigh)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("CTF-November.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            int32_t sni{};
            int32_t resGSNI = man.GetSourceNumInstances(55667788, sni);
            EXPECT_EQ(resGSNI, toInt(EalError::InvalidId));
            EXPECT_EQ(sni, {});
        }

        TEST(FailingEalFiles, GetSourceNumInstances_SourceNumInstancesUninitialized)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("CTF-LavaGiant.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            int32_t sni;
            int32_t resGSNI = man.GetSourceNumInstances(0, sni);
            EXPECT_EQ(resGSNI, toInt(EalError::OK));
            EXPECT_EQ(sni, 4);
            int32_t sni2;
            resGSNI = man.GetSourceNumInstances(1, sni2);
            EXPECT_EQ(resGSNI, toInt(EalError::OK));
            EXPECT_EQ(sni2, 0);
        }
    }
}

namespace GetSourceInstancePos
{
    namespace OriginalEALFiles
    {
        TEST_P(UT99EalFiles, GetSourceInstancePos_ValidFile)
        {
            TestParameter par = GetParam();
            int32_t resLoad = man.LoadDataSet(par.filename, 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            if (par.nrSrcs > 0)
            {
                for (uint32_t i = 0; i < par.nrSrcs; i++)
                {
                    ASSERT_EQ(par.nrSrcs, par.srcNames.size());
                    ASSERT_EQ(par.nrSrcs, par.srcPos.size());
                    int32_t srcID{};
                    int32_t resGSI = man.GetSourceID(par.srcNames[i], srcID);
                    ASSERT_EQ(resGSI, toInt(EalError::OK));
                    int32_t sni{};
                    int32_t resGSNI = man.GetSourceNumInstances(srcID, sni);
                    ASSERT_EQ(resGSNI, toInt(EalError::OK));
                    ASSERT_EQ(sni, par.srcPos[i].size());
                    for (int32_t k = 0; k < sni; k++)
                    {
                        EMPoint pos{};
                        int32_t resGSNI = man.GetSourceInstancePos(srcID, k, pos);
                        EXPECT_EQ(sni, par.srcPos[i].size());
                        EXPECT_FLOAT_EQ(pos.fX, par.srcPos[i][k].fX);
                        EXPECT_FLOAT_EQ(pos.fY, par.srcPos[i][k].fY);
                        EXPECT_FLOAT_EQ(pos.fZ, par.srcPos[i][k].fZ);
                    }
                }
            }
        }
    }

    namespace FailingGetSourceInstancePosRequests
    {
        TEST(FailingEalFiles, GetSourceInstancePos_SourceIDtooLow)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("DM-Gothic.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            EMPoint pos{};
            int32_t resGSNI = man.GetSourceInstancePos(-2, 0, pos);
            EXPECT_EQ(resGSNI, toInt(EalError::InvalidId));
            EXPECT_FLOAT_EQ(pos.fX, {});
            EXPECT_FLOAT_EQ(pos.fY, {});
            EXPECT_FLOAT_EQ(pos.fZ, {});
        }

        TEST(FailingEalFiles, GetSourceInstancePos_SourceIDtooHigh)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("CTF-Coret.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            EMPoint pos{};
            int32_t resGSNI = man.GetSourceInstancePos(32123, 0, pos);
            EXPECT_EQ(resGSNI, toInt(EalError::InvalidId));
            EXPECT_FLOAT_EQ(pos.fX, {});
            EXPECT_FLOAT_EQ(pos.fY, {});
            EXPECT_FLOAT_EQ(pos.fZ, {});
        }

        TEST(FailingEalFiles, GetSourceInstancePos_InstanceIDtooLow)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("CTF-LavaGiant.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            EMPoint pos{};
            int32_t resGSNI = man.GetSourceInstancePos(0, -1, pos);
            EXPECT_EQ(resGSNI, toInt(EalError::InvalidId));
            EXPECT_FLOAT_EQ(pos.fX, {});
            EXPECT_FLOAT_EQ(pos.fY, {});
            EXPECT_FLOAT_EQ(pos.fZ, {});
        }

        TEST(FailingEalFiles, GetSourceInstancePos_InstanceIDtooHigh)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("CTF-November.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            EMPoint pos{};
            int32_t resGSNI = man.GetSourceInstancePos(0, 123, pos);
            EXPECT_EQ(resGSNI, toInt(EalError::InvalidId));
            EXPECT_FLOAT_EQ(pos.fX, {});
            EXPECT_FLOAT_EQ(pos.fY, {});
            EXPECT_FLOAT_EQ(pos.fZ, {});
        }

        TEST(FailingEalFiles, GetSourceInstancePos_PosUninitialized)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("CTF-LavaGiant.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            EMPoint pos;
            int32_t resGSNI = man.GetSourceInstancePos(0, 1, pos);
            EXPECT_EQ(resGSNI, toInt(EalError::OK));
            EXPECT_NEAR(pos.fX, -3936.774658f, 0.1f);
            EXPECT_NEAR(pos.fY, 349.271942f, 0.01f);
            EXPECT_NEAR(pos.fZ, -440.669617f, 0.01f);
        }
    }
}

namespace GetEnvironmentID
{
    namespace OriginalEALFiles
    {
        TEST_P(UT99EalFiles, GetEnvironmentID_ValidFile)
        {
            TestParameter par = GetParam();
            int32_t resLoad = man.LoadDataSet(par.filename, 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            if (par.nrEnvs > 0)
            {
                for (uint32_t i = 0; i < par.nrEnvs; i++)
                {
                    ASSERT_EQ(par.nrEnvs, par.envNames.size());
                    int32_t envID{};
                    int32_t resGEI = man.GetEnvironmentID(par.envNames[i], envID);
                    EXPECT_EQ(resGEI, toInt(EalError::OK));
                    EXPECT_EQ(envID, i);
                }
            }
        }
    }

    namespace FailingGetEnvironmentIDRequests
    {
        TEST(FailingEalFiles, GetEnvironmentID_InvalidEnvName)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("DM-Curse][.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            int32_t envID{};
            int32_t resGEI = man.GetEnvironmentID("DoesNotExist", envID);
            EXPECT_EQ(resGEI, toInt(EalError::IdNotFound));
            EXPECT_EQ(envID, {});
        }

        TEST(FailingEalFiles, GetEnvironmentID_EmptyEnvName)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("DM-Peak.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            int32_t envID{};
            int32_t resGEI = man.GetEnvironmentID("", envID);
            EXPECT_EQ(resGEI, toInt(EalError::IdNotFound));
            EXPECT_EQ(envID, {});
        }

        TEST(FailingEalFiles, GetEnvironmentID_NoDataSetLoaded)
        {
            EalMan man;
            int32_t envID{};
            int32_t resGEI = man.GetEnvironmentID("DoesNotExist", envID);
            EXPECT_EQ(resGEI, toInt(EalError::IdNotFound));
            EXPECT_EQ(envID, {});
        }

        TEST(FailingEalFiles, GetEnvironmentID_UninitializedEnvID)
        {
            EalMan man;
            int32_t envID;
            int32_t resGEI = man.GetEnvironmentID("DoesNotExist", envID);
            EXPECT_EQ(resGEI, toInt(EalError::IdNotFound));
            EXPECT_EQ(envID, {});
        }
    }
}

namespace GetEnvironmentAttributes
{
    namespace OriginalEALFiles
    {
        TEST_P(UT99EalFiles, GetEnvironmentAttributes_ValidFile)
        {
            TestParameter par = GetParam();
            int32_t resLoad = man.LoadDataSet(par.filename, 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            if (par.nrEnvs > 0)
            {
                for (uint32_t i = 0; i < par.nrEnvs; i++)
                {
                    ASSERT_EQ(par.nrEnvs, par.envNames.size());
                    int32_t envID{};
                    int32_t resGEI = man.GetEnvironmentID(par.envNames[i], envID);
                    ASSERT_EQ(resGEI, toInt(EalError::OK));
                    EAXListenerProperties ea{};
                    int32_t resGLA = man.GetEnvironmentAttributes(envID, ea);
                    EXPECT_EQ(resGLA, toInt(EalError::OK));
                    EXPECT_EQ(ea.lRoom, par.envp[i].lRoom);
                    EXPECT_EQ(ea.lRoomHF, par.envp[i].lRoomHF);
                    //EXPECT_FLOAT_EQ(ea.flRoomRolloffFactor, par.envp[i].flRoomRolloffFactor);
                    EXPECT_NEAR(ea.flRoomRolloffFactor, par.envp[i].flRoomRolloffFactor, std::abs(ea.flRoomRolloffFactor + par.envp[i].flRoomRolloffFactor) / 2000);
                    //EXPECT_FLOAT_EQ(ea.flDecayTime, par.envp[i].flDecayTime);
                    EXPECT_NEAR(ea.flDecayTime, par.envp[i].flDecayTime, std::abs(ea.flDecayTime + par.envp[i].flDecayTime) / 2000);
                    //EXPECT_FLOAT_EQ(ea.flDecayHFRatio, par.envp[i].flDecayHFRatio);
                    EXPECT_NEAR(ea.flDecayHFRatio, par.envp[i].flDecayHFRatio, std::abs(ea.flDecayHFRatio + par.envp[i].flDecayHFRatio) / 2000);
                    EXPECT_EQ(ea.lReflections, par.envp[i].lReflections);
                    //EXPECT_FLOAT_EQ(ea.flReflectionsDelay, par.envp[i].flReflectionsDelay);
                    EXPECT_NEAR(ea.flReflectionsDelay, par.envp[i].flReflectionsDelay, std::abs(ea.flReflectionsDelay + par.envp[i].flReflectionsDelay) / 2000);
                    EXPECT_EQ(ea.lReverb, par.envp[i].lReverb);
                    //EXPECT_FLOAT_EQ(ea.flReverbDelay, par.envp[i].flReverbDelay);
                    EXPECT_NEAR(ea.flReverbDelay, par.envp[i].flReverbDelay, std::abs(ea.flReverbDelay + par.envp[i].flReverbDelay) / 2000);
                    EXPECT_EQ(ea.dwEnvironment, par.envp[i].dwEnvironment);
                    //EXPECT_FLOAT_EQ(ea.flEnvironmentSize, par.envp[i].flEnvironmentSize);
                    EXPECT_NEAR(ea.flEnvironmentSize, par.envp[i].flEnvironmentSize, std::abs(ea.flEnvironmentSize + par.envp[i].flEnvironmentSize) / 2000);
                    //EXPECT_FLOAT_EQ(ea.flEnvironmentDiffusion, par.envp[i].flEnvironmentDiffusion);
                    EXPECT_NEAR(ea.flEnvironmentDiffusion, par.envp[i].flEnvironmentDiffusion, std::abs(ea.flEnvironmentDiffusion + par.envp[i].flEnvironmentDiffusion) / 2000);
                    //EXPECT_FLOAT_EQ(ea.flAirAbsorptionHF, par.envp[i].flAirAbsorptionHF);
                    EXPECT_NEAR(ea.flAirAbsorptionHF, par.envp[i].flAirAbsorptionHF, std::abs(ea.flAirAbsorptionHF + par.envp[i].flAirAbsorptionHF) / 2000);
                    EXPECT_EQ(ea.dwFlags, par.envp[i].dwFlags);
                }
            }
        }
    }

    namespace FailingGetEnvironmentAttributesRequests
    {
        TEST(FailingEalFiles, GetEnvironmentAttributes_EnvIDtooLow)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("DM-Pressure.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            EAXListenerProperties ea{};
            int32_t resGLA = man.GetEnvironmentAttributes(-1, ea);
            EXPECT_EQ(resGLA, toInt(EalError::InvalidId));
            EXPECT_EQ(ea.lRoom, {});
            EXPECT_EQ(ea.lRoomHF, {});
            EXPECT_FLOAT_EQ(ea.flRoomRolloffFactor, {});
            EXPECT_FLOAT_EQ(ea.flDecayTime, {});
            EXPECT_FLOAT_EQ(ea.flDecayHFRatio, {});
            EXPECT_EQ(ea.lReflections, {});
            EXPECT_FLOAT_EQ(ea.flReflectionsDelay, {});
            EXPECT_EQ(ea.lReverb, {});
            EXPECT_FLOAT_EQ(ea.flReverbDelay, {});
            EXPECT_EQ(ea.dwEnvironment, {});
            EXPECT_FLOAT_EQ(ea.flEnvironmentSize, {});
            EXPECT_FLOAT_EQ(ea.flEnvironmentDiffusion, {});
            EXPECT_FLOAT_EQ(ea.flAirAbsorptionHF, {});
            EXPECT_EQ(ea.dwFlags, {});
        }

        TEST(FailingEalFiles, GetEnvironmentAttributes_EnvIDtooHigh)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("DM-Codex.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            EAXListenerProperties ea{};
            int32_t resGLA = man.GetEnvironmentAttributes(32168, ea);
            EXPECT_EQ(resGLA, toInt(EalError::InvalidId));
            EXPECT_EQ(ea.lRoom, {});
            EXPECT_EQ(ea.lRoomHF, {});
            EXPECT_FLOAT_EQ(ea.flRoomRolloffFactor, {});
            EXPECT_FLOAT_EQ(ea.flDecayTime, {});
            EXPECT_FLOAT_EQ(ea.flDecayHFRatio, {});
            EXPECT_EQ(ea.lReflections, {});
            EXPECT_FLOAT_EQ(ea.flReflectionsDelay, {});
            EXPECT_EQ(ea.lReverb, {});
            EXPECT_FLOAT_EQ(ea.flReverbDelay, {});
            EXPECT_EQ(ea.dwEnvironment, {});
            EXPECT_FLOAT_EQ(ea.flEnvironmentSize, {});
            EXPECT_FLOAT_EQ(ea.flEnvironmentDiffusion, {});
            EXPECT_FLOAT_EQ(ea.flAirAbsorptionHF, {});
            EXPECT_EQ(ea.dwFlags, {});
        }

        TEST(FailingEalFiles, GetEnvironmentAttributes_NoDataSetLoaded)
        {
            EalMan man;
            EAXListenerProperties ea{};
            int32_t resGLA = man.GetEnvironmentAttributes(0, ea);
            EXPECT_EQ(resGLA, toInt(EalError::InvalidId));
            EXPECT_EQ(ea.lRoom, {});
            EXPECT_EQ(ea.lRoomHF, {});
            EXPECT_FLOAT_EQ(ea.flRoomRolloffFactor, {});
            EXPECT_FLOAT_EQ(ea.flDecayTime, {});
            EXPECT_FLOAT_EQ(ea.flDecayHFRatio, {});
            EXPECT_EQ(ea.lReflections, {});
            EXPECT_FLOAT_EQ(ea.flReflectionsDelay, {});
            EXPECT_EQ(ea.lReverb, {});
            EXPECT_FLOAT_EQ(ea.flReverbDelay, {});
            EXPECT_EQ(ea.dwEnvironment, {});
            EXPECT_FLOAT_EQ(ea.flEnvironmentSize, {});
            EXPECT_FLOAT_EQ(ea.flEnvironmentDiffusion, {});
            EXPECT_FLOAT_EQ(ea.flAirAbsorptionHF, {});
            EXPECT_EQ(ea.dwFlags, {});
        }

        TEST(FailingEalFiles, GetEnvironmentAttributes_UninitializedEnvAttributes)
        {
            EalMan man;
            EAXListenerProperties ea;
            int32_t resGLA = man.GetEnvironmentAttributes(0, ea);
            EXPECT_EQ(resGLA, toInt(EalError::InvalidId));
            EXPECT_EQ(ea.lRoom, {});
            EXPECT_EQ(ea.lRoomHF, {});
            EXPECT_FLOAT_EQ(ea.flRoomRolloffFactor, {});
            EXPECT_FLOAT_EQ(ea.flDecayTime, {});
            EXPECT_FLOAT_EQ(ea.flDecayHFRatio, {});
            EXPECT_EQ(ea.lReflections, {});
            EXPECT_FLOAT_EQ(ea.flReflectionsDelay, {});
            EXPECT_EQ(ea.lReverb, {});
            EXPECT_FLOAT_EQ(ea.flReverbDelay, {});
            EXPECT_EQ(ea.dwEnvironment, {});
            EXPECT_FLOAT_EQ(ea.flEnvironmentSize, {});
            EXPECT_FLOAT_EQ(ea.flEnvironmentDiffusion, {});
            EXPECT_FLOAT_EQ(ea.flAirAbsorptionHF, {});
            EXPECT_EQ(ea.dwFlags, {});
        }
    }
}

namespace GetMaterialID
{
    namespace OriginalEALFiles
    {
        TEST_P(UT99EalFiles, GetMaterialID_ValidFile)
        {
            TestParameter par = GetParam();
            int32_t resLoad = man.LoadDataSet(par.filename, 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            if (par.nrObst > 0)
            {
                for (uint32_t i = 0; i < par.nrObst; i++)
                {
                    ASSERT_EQ(par.nrObst, par.obsNames.size());
                    int32_t obsID{};
                    int32_t resGMI = man.GetMaterialID(par.obsNames[i], obsID);
                    EXPECT_EQ(resGMI, toInt(EalError::OK));
                    EXPECT_EQ(obsID, i);
                }
            }
        }
    }

    namespace FailingGetMaterialIDRequests
    {
        TEST(FailingEalFiles, GetMaterialID_InvalidMatName)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("CTF-Niven.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            int32_t matID{};
            int32_t resGMI = man.GetMaterialID("DoesNotExist", matID);
            EXPECT_EQ(resGMI, toInt(EalError::IdNotFound));
            EXPECT_EQ(matID, {});
        }

        TEST(FailingEalFiles, GetMaterialID_EmptyEnvName)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("DM-Morbias][.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            int32_t matID{};
            int32_t resGMI = man.GetMaterialID("", matID);
            EXPECT_EQ(resGMI, toInt(EalError::IdNotFound));
            EXPECT_EQ(matID, {});
        }

        TEST(FailingEalFiles, GetMaterialID_NoDataSetLoaded)
        {
            EalMan man;
            int32_t matID{};
            int32_t resGMI = man.GetMaterialID("", matID);
            EXPECT_EQ(resGMI, toInt(EalError::IdNotFound));
            EXPECT_EQ(matID, {});
        }

        TEST(FailingEalFiles, GetMaterialID_UninitializedEnvID)
        {
            EalMan man;
            int32_t matID;
            int32_t resGMI = man.GetMaterialID("", matID);
            EXPECT_EQ(resGMI, toInt(EalError::IdNotFound));
            EXPECT_EQ(matID, {});
        }
    }
}

namespace GetMaterialAttributes
{
    namespace OriginalEALFiles
    {
        TEST_P(UT99EalFiles, GetMaterialAttributes_ValidFile)
        {
            TestParameter par = GetParam();
            int32_t resLoad = man.LoadDataSet(par.filename, 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            if (par.nrObst > 0)
            {
                for (uint32_t i = 0; i < par.nrObst; i++)
                {
                    ASSERT_EQ(par.nrObst, par.obsNames.size());
                    int32_t obsID{};
                    int32_t resGMI = man.GetMaterialID(par.obsNames[i], obsID);
                    ASSERT_EQ(resGMI, toInt(EalError::OK));
                    MaterialAttributes ma{};
                    int32_t resGLA = man.GetMaterialAttributes(obsID, ma);
                    EXPECT_EQ(resGLA, toInt(EalError::OK));
                    EXPECT_EQ(ma.lLevel, par.obstacles[i].lLevel);
                    EXPECT_FLOAT_EQ(ma.fLFRatio, par.obstacles[i].fLFRatio);
                    EXPECT_FLOAT_EQ(ma.fRoomRatio, par.obstacles[i].fRoomRatio);
                    EXPECT_EQ(ma.dwFlags, par.obstacles[i].dwFlags);
                }
            }
        }
    }

    namespace FailingGetMaterialAttributesRequests
    {
        TEST(FailingEalFiles, GetMaterialAttributes_MatIDtooLow)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("DM-Tempest.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            MaterialAttributes ma{};
            int32_t resGMA = man.GetMaterialAttributes(-1, ma);
            EXPECT_EQ(resGMA, toInt(EalError::InvalidId));
            EXPECT_EQ(ma.lLevel, {});
            EXPECT_FLOAT_EQ(ma.fLFRatio, {});
            EXPECT_FLOAT_EQ(ma.fRoomRatio, {});
            EXPECT_EQ(ma.dwFlags, {});
        }

        TEST(FailingEalFiles, GetMaterialAttributes_EnvIDtooHigh)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("DM-Pressure.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            MaterialAttributes ma{};
            int32_t resGMA = man.GetMaterialAttributes(999, ma);
            EXPECT_EQ(resGMA, toInt(EalError::InvalidId));
            EXPECT_EQ(ma.lLevel, {});
            EXPECT_FLOAT_EQ(ma.fLFRatio, {});
            EXPECT_FLOAT_EQ(ma.fRoomRatio, {});
            EXPECT_EQ(ma.dwFlags, {});
        }

        TEST(FailingEalFiles, GetMaterialAttributes_NoDataSetLoaded)
        {
            EalMan man;
            MaterialAttributes ma{};
            int32_t resGMA = man.GetMaterialAttributes(0, ma);
            EXPECT_EQ(resGMA, toInt(EalError::InvalidId));
            EXPECT_EQ(ma.lLevel, {});
            EXPECT_FLOAT_EQ(ma.fLFRatio, {});
            EXPECT_FLOAT_EQ(ma.fRoomRatio, {});
            EXPECT_EQ(ma.dwFlags, {});
        }

        TEST(FailingEalFiles, GetMaterialAttributes_UninitializedEnvAttributes)
        {
            EalMan man;
            MaterialAttributes ma;
            int32_t resGMA = man.GetMaterialAttributes(0, ma);
            EXPECT_EQ(resGMA, toInt(EalError::InvalidId));
            EXPECT_EQ(ma.lLevel, {});
            EXPECT_FLOAT_EQ(ma.fLFRatio, {});
            EXPECT_FLOAT_EQ(ma.fRoomRatio, {});
            EXPECT_EQ(ma.dwFlags, {});
        }
    }
}

namespace GetGeometrySetID
{
    namespace OriginalEALFiles
    {
        /*
        * EAX Manager does not provide any means to get the geometry set name, so this function would not have been used by EaxMan.dll
        * therefore there is way to get the names with EAX Manager and compare them to EalMan
        * some eal files have custom names, most have the default name "Geometry1"
        * it's sufficient to only test one file, which is done in the test below
        TEST_P(UT99EalFiles, GetGeometrySetID_ValidFile)
        {
            TestParameter par = GetParam();
            int32_t resLoad = man.LoadDataSet(par.filename, 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            int32_t geoID{};
            int32_t resGGS = man.GetGeometrySetID("Geometry1", geoID); // not alle .eal files have "Geometry1"
            EXPECT_EQ(resGGS, toInt(EalError::OK));
            EXPECT_EQ(geoID, 0);
        }
        */

        TEST(LavaGiantGeometrySetID, GetGeometrySetID_ValidFile)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("CTF-LavaGiant.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            int32_t geoID{};
            int32_t resGGS = man.GetGeometrySetID("Geometry1", geoID);
            EXPECT_EQ(resGGS, toInt(EalError::OK));
            EXPECT_EQ(geoID, 0);
        }
    }

    namespace FailingGetGeometrySetIDRequests
    {
        TEST(FailingEalFiles, GetGeometrySetID_InvalidMatName)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("CTF-Niven.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            int32_t geoID{};
            int32_t resGGI = man.GetGeometrySetID("DoesNotExist", geoID);
            EXPECT_EQ(resGGI, toInt(EalError::IdNotFound));
            EXPECT_EQ(geoID, {});
        }

        TEST(FailingEalFiles, GetGeometrySetID_EmptyEnvName)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("DM-Fetid.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            int32_t geoID{};
            int32_t resGGI = man.GetGeometrySetID("", geoID);
            EXPECT_EQ(resGGI, toInt(EalError::IdNotFound));
            EXPECT_EQ(geoID, {});
        }

        TEST(FailingEalFiles, GetGeometrySetID_NoDataSetLoaded)
        {
            EalMan man;
            int32_t geoID{};
            int32_t resGGI = man.GetGeometrySetID("", geoID);
            EXPECT_EQ(resGGI, toInt(EalError::IdNotFound));
            EXPECT_EQ(geoID, {});
        }

        TEST(FailingEalFiles, GetGeometrySetID_UninitializedEnvID)
        {
            EalMan man;
            int32_t geoID;
            int32_t resGGI = man.GetGeometrySetID("", geoID);
            EXPECT_EQ(resGGI, toInt(EalError::IdNotFound));
            EXPECT_EQ(geoID, {});
        }
    }
}

namespace GetListenerDynamicAttributes
{
    namespace OriginalEALFiles
    {
        /*
        TEST(LibUse, GetListenerDynamicAttributes_CTFFace)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("CTF-Face.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            uint32_t datasize = 0;
            int32_t resSize = man.GetDataSetSize(datasize, 0);
            ASSERT_EQ(resSize, toInt(EalError::OK));
            ASSERT_GT(datasize, sizeof(EalData));
            int32_t resFree = man.FreeDataSet(0);
            EXPECT_EQ(resFree, toInt(EalError::OK));
            uint32_t freesize = 0;
            int32_t resEmpty = man.GetDataSetSize(freesize, 0);
            EXPECT_EQ(resEmpty, toInt(EalError::OK));
            EXPECT_EQ(freesize, sizeof(EalData));
        }
        */
    }

    namespace FailingGetListenerDynamicAttributesRequests
    {
        TEST(FailingEalFiles, GetListenerDynamicAttributes_GeomIDtooLow)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("DM-Codex.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            int32_t envID{};
            EMPoint pos{};
            int32_t resGDA = man.GetListenerDynamicAttributes(-1, pos, envID, 0);
            EXPECT_EQ(resGDA, toInt(EalError::IdNotFound));
            EXPECT_EQ(envID, {});
        }

        TEST(FailingEalFiles, GetListenerDynamicAttributes_GeomIDtooHigh)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("DM-Grinder.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            int32_t envID{};
            EMPoint pos{};
            int32_t resGDA = man.GetListenerDynamicAttributes(1337, pos, envID, 0);
            EXPECT_EQ(resGDA, toInt(EalError::IdNotFound));
            EXPECT_EQ(envID, {});
        }

        TEST(FailingEalFiles, GetListenerDynamicAttributes_PosOutOfBoundXLow)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("CTF-Niven.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            int32_t envID{};
            EMPoint pos{ -32769.0f, 0.0f, 0.0f };
            int32_t resGDA = man.GetListenerDynamicAttributes(0, pos, envID, 0);
            EXPECT_EQ(resGDA, toInt(EalError::IdNotFound));
            EXPECT_EQ(envID, {});
        }

        TEST(FailingEalFiles, GetListenerDynamicAttributes_PosOutOfBoundXHigh)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("DM-Fractal.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            int32_t envID{};
            EMPoint pos{ 50000.0f, 0.0f, 0.0f };
            int32_t resGDA = man.GetListenerDynamicAttributes(0, pos, envID, 0);
            EXPECT_EQ(resGDA, toInt(EalError::IdNotFound));
            EXPECT_EQ(envID, {});
        }

        TEST(FailingEalFiles, GetListenerDynamicAttributes_PosOutOfBoundYLow)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("DM-Kgalleon.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            int32_t envID{};
            EMPoint pos{ 0.0f, -36000.0f, 0.0f };
            int32_t resGDA = man.GetListenerDynamicAttributes(0, pos, envID, 0);
            EXPECT_EQ(resGDA, toInt(EalError::IdNotFound));
            EXPECT_EQ(envID, {});
        }

        TEST(FailingEalFiles, GetListenerDynamicAttributes_PosOutOfBoundYHigh)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("CTF-dreary.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            int32_t envID{};
            EMPoint pos{ 0.0f, 33000.0f, 0.0f };
            int32_t resGDA = man.GetListenerDynamicAttributes(0, pos, envID, 0);
            EXPECT_EQ(resGDA, toInt(EalError::IdNotFound));
            EXPECT_EQ(envID, {});
        }

        TEST(FailingEalFiles, GetListenerDynamicAttributes_PosOutOfBoundZlow)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("DM-Liandri.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            int32_t envID{};
            EMPoint pos{ 0.0f, 0.0f, -35000.0f };
            int32_t resGDA = man.GetListenerDynamicAttributes(0, pos, envID, 0);
            EXPECT_EQ(resGDA, toInt(EalError::IdNotFound));
            EXPECT_EQ(envID, {});
        }

        TEST(FailingEalFiles, GetListenerDynamicAttributes_PosOutOfBoundZHigh)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("DM-Morbias][.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            int32_t envID{};
            EMPoint pos{0.0f, 0.0f, 40000.0f};
            int32_t resGDA = man.GetListenerDynamicAttributes(0, pos, envID, 0);
            EXPECT_EQ(resGDA, toInt(EalError::IdNotFound));
            EXPECT_EQ(envID, {});
        }

        TEST(FailingEalFiles, GetListenerDynamicAttributes_NoDataSetLoaded)
        {
            EalMan man;
            int32_t envID{};
            EMPoint pos{};
            int32_t resGDA = man.GetListenerDynamicAttributes(0, pos, envID, 0);
            EXPECT_EQ(resGDA, toInt(EalError::IdNotFound));
            EXPECT_EQ(envID, {});
        }

        TEST(FailingEalFiles, GetListenerDynamicAttributes_UninitializedEnvID)
        {
            EalMan man;
            int32_t envID;
            EMPoint pos{};
            int32_t resGDA = man.GetListenerDynamicAttributes(0, pos, envID, 0);
            EXPECT_EQ(resGDA, toInt(EalError::IdNotFound));
            EXPECT_EQ(envID, {});
        }
    }
}

namespace GetSourceDynamicAttributes
{
    namespace OriginalEALFiles
    {
        /*
        TEST(LibUse, GetSourceDynamicAttributes_CTFFace)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("CTF-Face.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            uint32_t datasize = 0;
            int32_t resSize = man.GetDataSetSize(datasize, 0);
            ASSERT_EQ(resSize, toInt(EalError::OK));
            ASSERT_GT(datasize, sizeof(EalData));
            int32_t resFree = man.FreeDataSet(0);
            EXPECT_EQ(resFree, toInt(EalError::OK));
            uint32_t freesize = 0;
            int32_t resEmpty = man.GetDataSetSize(freesize, 0);
            EXPECT_EQ(resEmpty, toInt(EalError::OK));
            EXPECT_EQ(freesize, sizeof(EalData));
        }
        */
    }

    namespace FailingGetSourceDynamicAttributesRequests
    {
        TEST(FailingEalFiles, GetSourceDynamicAttributes_GeomIDtooLow)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("DM-Codex.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            EMPoint pos{};
            int32_t obs{};
            float olf{};
            int32_t occ{};
            float clf{};
            float orm{};
            EMPoint vPos{};
            int32_t resGDA = man.GetSourceDynamicAttributes(-1, pos, obs, olf, occ, clf, orm, vPos, 0);
            EXPECT_EQ(resGDA, toInt(EalError::IdNotFound));
            EXPECT_EQ(obs, {});
            EXPECT_FLOAT_EQ(olf, {});
            EXPECT_EQ(occ, {});
            EXPECT_FLOAT_EQ(clf, {});
            EXPECT_FLOAT_EQ(orm, {});
        }

        TEST(FailingEalFiles, GetSourceDynamicAttributes_GeomIDtooHigh)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("DM-Grinder.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            EMPoint pos{};
            int32_t obs{};
            float olf{};
            int32_t occ{};
            float clf{};
            float orm{};
            EMPoint vPos{};
            int32_t resGDA = man.GetSourceDynamicAttributes(12321, pos, obs, olf, occ, clf, orm, vPos, 0);
            EXPECT_EQ(resGDA, toInt(EalError::IdNotFound));
            EXPECT_EQ(obs, {});
            EXPECT_FLOAT_EQ(olf, {});
            EXPECT_EQ(occ, {});
            EXPECT_FLOAT_EQ(clf, {});
            EXPECT_FLOAT_EQ(orm, {});
        }

        TEST(FailingEalFiles, GetSourceDynamicAttributes_PosOutOfBoundXLow)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("CTF-Niven.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            EMPoint pos{ -32769.0f, 0.0f, 0.0f };
            int32_t obs{};
            float olf{};
            int32_t occ{};
            float clf{};
            float orm{};
            EMPoint vPos{};
            int32_t resGDA = man.GetSourceDynamicAttributes(0, pos, obs, olf, occ, clf, orm, vPos, 0);
            EXPECT_EQ(resGDA, toInt(EalError::IdNotFound));
            EXPECT_EQ(obs, {});
            EXPECT_FLOAT_EQ(olf, {});
            EXPECT_EQ(occ, {});
            EXPECT_FLOAT_EQ(clf, {});
            EXPECT_FLOAT_EQ(orm, {});
        }

        TEST(FailingEalFiles, GetSourceDynamicAttributes_PosOutOfBoundXHigh)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("DM-Fractal.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            EMPoint pos{ 50000.0f, 0.0f, 0.0f };
            int32_t obs{};
            float olf{};
            int32_t occ{};
            float clf{};
            float orm{};
            EMPoint vPos{};
            int32_t resGDA = man.GetSourceDynamicAttributes(0, pos, obs, olf, occ, clf, orm, vPos, 0);
            EXPECT_EQ(resGDA, toInt(EalError::IdNotFound));
            EXPECT_EQ(obs, {});
            EXPECT_FLOAT_EQ(olf, {});
            EXPECT_EQ(occ, {});
            EXPECT_FLOAT_EQ(clf, {});
            EXPECT_FLOAT_EQ(orm, {});
        }

        TEST(FailingEalFiles, GetSourceDynamicAttributes_PosOutOfBoundYLow)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("DM-Kgalleon.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            EMPoint pos{ 0.0f, -36000.0f, 0.0f };
            int32_t obs{};
            float olf{};
            int32_t occ{};
            float clf{};
            float orm{};
            EMPoint vPos{};
            int32_t resGDA = man.GetSourceDynamicAttributes(0, pos, obs, olf, occ, clf, orm, vPos, 0);
            EXPECT_EQ(resGDA, toInt(EalError::IdNotFound));
            EXPECT_EQ(obs, {});
            EXPECT_FLOAT_EQ(olf, {});
            EXPECT_EQ(occ, {});
            EXPECT_FLOAT_EQ(clf, {});
            EXPECT_FLOAT_EQ(orm, {});
        }

        TEST(FailingEalFiles, GetSourceDynamicAttributes_PosOutOfBoundYHigh)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("CTF-dreary.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            EMPoint pos{ 0.0f, 33000.0f, 0.0f };
            int32_t obs{};
            float olf{};
            int32_t occ{};
            float clf{};
            float orm{};
            EMPoint vPos{};
            int32_t resGDA = man.GetSourceDynamicAttributes(0, pos, obs, olf, occ, clf, orm, vPos, 0);
            EXPECT_EQ(resGDA, toInt(EalError::IdNotFound));
            EXPECT_EQ(obs, {});
            EXPECT_FLOAT_EQ(olf, {});
            EXPECT_EQ(occ, {});
            EXPECT_FLOAT_EQ(clf, {});
            EXPECT_FLOAT_EQ(orm, {});
        }

        TEST(FailingEalFiles, GetSourceDynamicAttributes_PosOutOfBoundZlow)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("DM-Liandri.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            EMPoint pos{ 0.0f, 0.0f, -35000.0f };
            int32_t obs{};
            float olf{};
            int32_t occ{};
            float clf{};
            float orm{};
            EMPoint vPos{};
            int32_t resGDA = man.GetSourceDynamicAttributes(0, pos, obs, olf, occ, clf, orm, vPos, 0);
            EXPECT_EQ(resGDA, toInt(EalError::IdNotFound));
            EXPECT_EQ(obs, {});
            EXPECT_FLOAT_EQ(olf, {});
            EXPECT_EQ(occ, {});
            EXPECT_FLOAT_EQ(clf, {});
            EXPECT_FLOAT_EQ(orm, {});
        }

        TEST(FailingEalFiles, GetSourceDynamicAttributes_PosOutOfBoundZHigh)
        {
            EalMan man;
            int32_t resLoad = man.LoadDataSet("DM-Morbias][.eal", 0);
            ASSERT_EQ(resLoad, toInt(EalError::OK));
            EMPoint pos{ 0.0f, 0.0f, 40000.0f };
            int32_t obs{};
            float olf{};
            int32_t occ{};
            float clf{};
            float orm{};
            EMPoint vPos{};
            int32_t resGDA = man.GetSourceDynamicAttributes(0, pos, obs, olf, occ, clf, orm, vPos, 0);
            EXPECT_EQ(resGDA, toInt(EalError::IdNotFound));
            EXPECT_EQ(obs, {});
            EXPECT_FLOAT_EQ(olf, {});
            EXPECT_EQ(occ, {});
            EXPECT_FLOAT_EQ(clf, {});
            EXPECT_FLOAT_EQ(orm, {});
        }

        TEST(FailingEalFiles, GetSourceDynamicAttributes_NoDataSetLoaded)
        {
            EalMan man;
            int32_t envID{};
            EMPoint pos{};
            int32_t obs{};
            float olf{};
            int32_t occ{};
            float clf{};
            float orm{};
            EMPoint vPos{};
            int32_t resGDA = man.GetSourceDynamicAttributes(0, pos, obs, olf, occ, clf, orm, vPos, 0);
            EXPECT_EQ(resGDA, toInt(EalError::IdNotFound));
            EXPECT_EQ(obs, {});
            EXPECT_FLOAT_EQ(olf, {});
            EXPECT_EQ(occ, {});
            EXPECT_FLOAT_EQ(clf, {});
            EXPECT_FLOAT_EQ(orm, {});
        }

        TEST(FailingEalFiles, GetSourceDynamicAttributes_UninitializedVars)
        {
            EalMan man;
            EMPoint pos{};
            int32_t obs;
            float olf;
            int32_t occ;
            float clf;
            float orm;
            EMPoint vPos;
            int32_t resGDA = man.GetSourceDynamicAttributes(0, pos, obs, olf, occ, clf, orm, vPos, 0);
            EXPECT_EQ(resGDA, toInt(EalError::IdNotFound));
            EXPECT_EQ(obs, {});
            EXPECT_FLOAT_EQ(olf, {});
            EXPECT_EQ(occ, {});
            EXPECT_FLOAT_EQ(clf, {});
            EXPECT_FLOAT_EQ(orm, {});
        }
    }
}
