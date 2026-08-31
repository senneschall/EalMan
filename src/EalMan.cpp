#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "Chunk.h"
#include "EalMan.h"

inline static uint32_t to_u32_le(
        const std::array<char, 4>&   a
        )
{
    return (uint32_t(static_cast<uint8_t>(a[0]))       ) |
           (uint32_t(static_cast<uint8_t>(a[1])) <<  8 ) |
           (uint32_t(static_cast<uint8_t>(a[2])) << 16 ) |
           (uint32_t(static_cast<uint8_t>(a[3])) << 24 );
}

inline static float scalarProduct(
    float aX,
    float aY,
    float aZ,
    float bX,
    float bY,
    float bZ
)
{
    return (aX * bX + aY * bY + aZ * bZ);
}

inline static bool isInFront(
    EMPoint point,
    float fX,
    float fY,
    float fZ,
    float nX,
    float nY,
    float nZ
    )
{
    /*
     * EaxMan.dll selects Front if scalarProduct >= 0 and Back otherwise
     * it also checks if the point is within a factor x thickness of the wall. Both values are hardcoded inside EaxMan.dll to be 1.0f and can't be overwritten by data inside a .eal
     * if the point is within this are, EaxMan.dll returns the Front child
     * this can be simplified to the single expression implemented here
     */
    return scalarProduct(
        (point.fX - fX),
        (point.fY - fY),
        (point.fZ - fZ),
        nX,
        nY,
        nZ
    ) >= -1.0f;
}

template<typename T>
int32_t readStruct(
        std::ifstream&   file,
        T&               target,
        uint32_t&        bytesRead
        )
{
    std::array<char, sizeof(T)> raw{};
    if (!file.read(raw.data(), raw.size())) { return toInt(EalError::FileInvalid); }
    bytesRead += sizeof(T);

    std::memcpy(&target, raw.data(), sizeof(T));
    return toInt(EalError::OK);
}

static int32_t readNumber(
        std::ifstream&   file,
        uint32_t&        number,
        uint32_t&        bytesRead
        )
{
    std::array<char, 4> bytes{};
    if (!file.read(bytes.data(), bytes.size())) { return toInt(EalError::FileInvalid); }
    else
    {
        number = to_u32_le(bytes);
        bytesRead += bytes.size();
        return toInt(EalError::OK);
    }
}

static int32_t readVer(
        std::ifstream&   file,
        const uint32_t   version,
        uint32_t&        bytesRead
        )
{
    uint32_t ver{};
    if (readNumber(file, ver, bytesRead) != toInt(EalError::OK) || ver != version) { return toInt(EalError::VersionInvalid); }
    return toInt(EalError::OK);
}

template <typename T>
static int32_t ReadArrayData(
        std::ifstream&    file,
        uint32_t          count,
        std::vector<T>&   dest,
        uint32_t&         bytesRead
        )
{
    dest.resize(count);
    if (!file.read(reinterpret_cast<char*>(dest.data()), dest.size() * sizeof(T))) { return toInt(EalError::VersionInvalid); }
    bytesRead += dest.size() * sizeof(T);
    return toInt(EalError::OK);
}
/*
static int32_t readString(
    std::ifstream& file,
    std::array<char, 260>& text,
    uint32_t& bytesRead
)
{
    if (!file.read(text.data(), text.size())) { return toInt(EalError::FileInvalid); }
    else
    {
        bytesRead += text.size();
        return toInt(EalError::OK);
    }
}
*/
int32_t EalMan::EalGlobals(
        std::ifstream&   file,
        const FourCC     fourcc,
        uint32_t&        bytesRead
        )
{
    switch (fourcc) {
        case FourCC::MAJV:
            return readVer(file, 2, bytesRead);
        case FourCC::MINV:
            return readVer(file, 1, bytesRead);
        case FourCC::EXEP:
            //return readString(file, m_data->defExep, bytesRead);
            file.ignore(260);
		    bytesRead += 260;
            return toInt(EalError::OK);
        case FourCC::CMDS:
            //return readString(file, m_data->defCmds, bytesRead);
            file.ignore(260);
		    bytesRead += 260;
            return toInt(EalError::OK);
        case FourCC::GDFM:
            return readStruct(file, m_data->gdfm, bytesRead) == toInt(EalError::OK) && isGDFMdataPlausible() ? toInt(EalError::OK) : toInt(EalError::FileInvalid);
        case FourCC::LISA:
            return readStruct(file, m_data->defListener, bytesRead) == toInt(EalError::OK) && isDEFLdataPlausible() ? toInt(EalError::OK) : toInt(EalError::FileInvalid);
        case FourCC::DENV:
            return readStruct(file, m_data->defEnvironment, bytesRead) == toInt(EalError::OK) && isDEFEdataPlausible() ? toInt(EalError::OK) : toInt(EalError::FileInvalid);
        case FourCC::DSRC:
            return readStruct(file, m_data->defSource, bytesRead) == toInt(EalError::OK) && isDEFSdataPlausible() ? toInt(EalError::OK) : toInt(EalError::FileInvalid);
        case FourCC::DFIL:
            //return readString(file, m_data->defFile, bytesRead);
            file.ignore(260);
		    bytesRead += 260;
            return toInt(EalError::OK);
        case FourCC::DMAT:
            return readStruct(file, m_data->defObstacle, bytesRead) == toInt(EalError::OK) && isDEFMdataPlausible() ? toInt(EalError::OK) : toInt(EalError::FileInvalid);
        default:
            return toInt(EalError::InvalidId);
    }
}

int32_t EalMan::EalList(
        std::ifstream&                                          file,
        std::bitset<static_cast<uint32_t>(FourCCreq::COUNT)>&   reqFourCC,
        uint32_t&                                               bytesRead
        )
{
    std::array<char, 4> fourCC{};
    if (!file.read(fourCC.data(), fourCC.size())) { return toInt(EalError::VersionInvalid); }
    bytesRead += fourCC.size();

    if (to_u32_le(fourCC) == static_cast<uint32_t>(FourCC::ENVP))
    { return EalEnvironmentAttributes(file, reqFourCC, bytesRead); }
    if (to_u32_le(fourCC) == static_cast<uint32_t>(FourCC::MATP))
    { return EalObstacleAttributes(file, bytesRead); }
    if (to_u32_le(fourCC) == static_cast<uint32_t>(FourCC::SRCP))
    { return EalSourceAttributes(file, bytesRead); }
    if (to_u32_le(fourCC) == static_cast<uint32_t>(FourCC::GEMP))
    { return EalGeometryAttributes(file, reqFourCC, bytesRead); }

    return toInt(EalError::OK);
}

int32_t EalMan::ReadNumsChunk(
        std::ifstream&   file,
        uint32_t&        count,
        uint32_t&        bytesRead
        )
{
    count = {};
    Chunk num{};
    if (num.read(file) == toInt(EalError::OK))
    {
        bytesRead += sizeof(num);
        if (num.fourCC != FourCC::NUM) { return toInt(EalError::FileInvalid); }
    }

    std::array<char, 4> nums{};
    if (!file.read(nums.data(), nums.size())) { return toInt(EalError::VersionInvalid); }
    bytesRead += nums.size();
    count = to_u32_le(nums);
    return toInt(EalError::OK);
}

int32_t EalMan::ReadNamsChunk(
        std::ifstream&              file,
        uint32_t                    count,
        std::vector<std::string>&   dest,
        uint32_t&                   bytesRead
        )
{
    Chunk nams{};
    if (nams.read(file) == toInt(EalError::OK))
    {
        bytesRead += sizeof(nams);
        if (nams.fourCC != FourCC::NAMS) { return toInt(EalError::FileInvalid); }
    }

    dest.reserve(dest.size() + count);
    for (uint32_t i = 0; i < count; i++)
    {
        std::array<char, 32> buffer{};
        if (!file.read(buffer.data(), buffer.size())) { return toInt(EalError::VersionInvalid); }
        bytesRead += buffer.size();
        if (std::find(buffer.begin(), buffer.end(), '\0') == buffer.end()) { buffer[31] = '\0'; } // null-terminate string if not present
        dest.emplace_back(buffer.data());
    }
    return toInt(EalError::OK);
}
/*
int32_t EalMan::ReadFilsChunk(
        std::ifstream&              file,
        uint32_t                    count,
        std::vector<std::string>&   dest,
        uint32_t&                   bytesRead
        )
{
    Chunk fils{};
    if (fils.read(file) == toInt(EalError::OK))
    {
        bytesRead += sizeof(fils);
        if (fils.fourCC != FourCC::FILS) { return toInt(EalError::FileInvalid); }
    }

    dest.reserve(dest.size() + count);
    for (uint32_t i = 0; i < count; i++)
    {
        std::array<char, 260> buffer{};
        if (!file.read(buffer.data(), buffer.size())) { return toInt(EalError::VersionInvalid); }
        bytesRead += buffer.size();
        dest.emplace_back(buffer.data());
    }
    return toInt(EalError::OK);
}
*/
template <typename T>
int32_t EalMan::ReadDataChunk(
        std::ifstream&    file,
        uint32_t          count,
        const FourCC      fourCC,
        std::vector<T>&   dest,
        uint32_t&         bytesRead
        )
{
    Chunk data{};
    if (data.read(file) == toInt(EalError::OK))
    {
        bytesRead += sizeof(data);
        if (data.fourCC != fourCC) { return toInt(EalError::FileInvalid); }
    }

    return ReadArrayData(file, count, dest, bytesRead);
}

int32_t EalMan::ReadGemaChunk(
        std::ifstream&   file,
        uint32_t&        bytesRead
        )
{
    Chunk gema{};
    if (gema.read(file) == toInt(EalError::OK))
    {
        bytesRead += sizeof(gema);
        if (gema.fourCC != FourCC::GEMA) { return toInt(EalError::FileInvalid); }
    }

    uint32_t nrSrcs{};
    uint32_t nrEnvs{};
    uint32_t nrUnknown{};
    uint32_t nrPlanes{};
    uint32_t nrLeaves{};

    if (   readNumber(file, nrSrcs, bytesRead)    != toInt(EalError::OK)
        || readNumber(file, nrEnvs, bytesRead)    != toInt(EalError::OK)
        || readNumber(file, nrUnknown, bytesRead) != toInt(EalError::OK)
        || readNumber(file, nrPlanes, bytesRead)  != toInt(EalError::OK)
        || readNumber(file, nrLeaves, bytesRead)  != toInt(EalError::OK))
    { return toInt(EalError::FileInvalid); }

    uint32_t envMatrixSize = (nrEnvs + 1) * (nrEnvs + 1);

    if (nrSrcs > 0) // sources are optional
    {
        if (ReadArrayData(file, nrSrcs, m_data->gemaSrcIDs, bytesRead) != toInt(EalError::OK)) { return toInt(EalError::FileInvalid); } // Source IDs
        if (ReadArrayData(file, nrSrcs, m_data->gemaSources, bytesRead) != toInt(EalError::OK)) { return toInt(EalError::FileInvalid); } // Source Pos
    }
    if (!(nrEnvs > 0   && ReadArrayData(file, nrEnvs, m_data->gemaEnvIDs, bytesRead)  == toInt(EalError::OK))) { return toInt(EalError::FileInvalid); } // Env IDs
    if (!(nrEnvs > 0 && ReadArrayData(file, nrEnvs, m_data->gemaDiffraction, bytesRead) == toInt(EalError::OK))) { return toInt(EalError::FileInvalid); } // Diffraction
    if (!(envMatrixSize > 1 && ReadArrayData(file, envMatrixSize, m_data->gemaEnvironmentMatrix, bytesRead)  == toInt(EalError::OK))) { return toInt(EalError::FileInvalid); } // Environment Matrix
    if (!(nrPlanes > 0 && ReadArrayData(file, nrPlanes, m_data->gemaBSPinnerNodes, bytesRead)  == toInt(EalError::OK))) { return toInt(EalError::FileInvalid); } // Plane tree
    if (!(nrLeaves > 0 && ReadArrayData(file, nrLeaves, m_data->gemaBSPouterNodes, bytesRead)  == toInt(EalError::OK))) { return toInt(EalError::FileInvalid); } // Leaf tree

    m_data->gemaNrSrcIDs = nrSrcs;
    m_data->gemaNrEnvIDs = nrEnvs;
    m_data->gemaNrInnerNodes = nrPlanes;
    m_data->gemaNrOuterNodes = nrLeaves;

    uint32_t nrDiffBox{};
    if (readNumber(file, nrDiffBox, bytesRead) !=toInt(EalError::OK)) { return toInt(EalError::FileInvalid); }
    if (nrDiffBox > 0) // diffraction boxes are optional
    {
        m_data->gemaNrDiffBox = nrDiffBox;
        if (ReadArrayData(file, nrDiffBox, m_data->gemaDiffBox, bytesRead) != toInt(EalError::OK)) { return toInt(EalError::FileInvalid); }
    }

    /*
     * comparing the read size to chunk size would make the lib more robust, but "DM-Morbias][.eal" has a wrong gema chunk size in the header and would fail here
     *
    if (  (1 + nrSrcs      * 4 ) * 4  // each sources struct has 4 members (one index, 3 coordinates); plus nrSrcs; each 4 bytes
        + (1 + nrEnvs      * 1 ) * 4  // each environment struct has 1 member; plus nrEnvs; each 4 bytes
        + (1 + nrUnknown   * 1 ) * 4  // unknown number 4 bytes
        + (0 + nrEnvs      * 1 ) * 4  // diffraction matrix consists of 4 bytes indizes
        + (0 + nrEnvObsIdx * 1 ) * 4  // environment matrix consists of 4 bytes indizes
        + (1 + nrPlanes    * 11) * 4  // each NodeA struct has 11 members; plus nrNodeA; each 4 bytes
        + (1 + nrLeaves    * 2 ) * 4  // each NodeB struct has 2 members; plus nrNodeB; each 4 bytes
        + (1 + nrDiffBox   * 7 ) * 4  // each Unknown struct has 7 members; plus nrUnkown; each 4 bytes
        != gema.length)             // should be the total gema chunk data size
    { return toInt(EalError::FileInvalid); }
     */

    if (isGEMAdataPlausible()) { toInt(EalError::FileInvalid); }
    return toInt(EalError::OK);
}

int32_t EalMan::GetID(
        const std::string&          name,
        int32_t&                    id,
        std::vector<std::string>&   dataset
        ) const
{
    id = {};
    if (!dataset.empty())
    {
        auto cnt = std::find(dataset.begin(), dataset.end(), name);
        if (cnt != dataset.end())
        {
            id = cnt - dataset.begin();
            return toInt(EalError::OK);
        }
    }
    return toInt(EalError::IdNotFound);
}

template <typename T>
int32_t EalMan::GetAttributes(
        const int32_t     id,
        const int32_t     length,
        T&                attributes,
        std::vector<T>&   dataset
        ) const
{
    attributes = {};
    if (id < 0 || id >= length) { return toInt(EalError::InvalidId); }

    attributes = dataset[id]; // boundary check above

    return toInt(EalError::OK);
}

int32_t EalMan::EalEnvironmentAttributes(
        std::ifstream&                                          file,
        std::bitset<static_cast<uint32_t>(FourCCreq::COUNT)>&   reqFourCC,
        uint32_t&                                               bytesRead
        )
{
    uint32_t nums{};
    int32_t res = ReadNumsChunk(file, nums, bytesRead);
    if (res != toInt(EalError::OK))
    { return res; }
    m_data->nrEnvironments = nums;
    if (m_data->nrEnvironments > 0)
    {
        if (ReadNamsChunk(file, m_data->nrEnvironments, m_data->envNames, bytesRead) != toInt(EalError::OK)) { return toInt(EalError::FileInvalid); }
        reqFourCC.set(static_cast<uint32_t>(FourCCreq::ENVP_NAMS));

        if (ReadDataChunk(file, m_data->nrEnvironments, FourCC::LISP, m_data->environments, bytesRead) != toInt(EalError::OK)) { return toInt(EalError::FileInvalid); }
        reqFourCC.set(static_cast<uint32_t>(FourCCreq::ENVP_LISP));
        if (!isLISPdataPlausible()) { return toInt(EalError::FileInvalid); }
    }
    return toInt(EalError::OK);
}

int32_t EalMan::EalObstacleAttributes(
        std::ifstream&   file,
        uint32_t&        bytesRead
        )
{
    uint32_t nums{};
    int32_t res = ReadNumsChunk(file, nums, bytesRead);
    if (res != toInt(EalError::OK))
    { return res; }
    m_data->nrObstacles = nums;
    if (m_data->nrObstacles > 0)
    {
        if (ReadNamsChunk(file, m_data->nrObstacles, m_data->obsNames, bytesRead) != toInt(EalError::OK)) { return toInt(EalError::FileInvalid); }

        if (ReadDataChunk(file, m_data->nrObstacles, FourCC::MATA, m_data->obstacles, bytesRead) != toInt(EalError::OK)) { return toInt(EalError::FileInvalid); }
        if (!isMATAdataPlausible()) { return toInt(EalError::FileInvalid); }
    }
    return toInt(EalError::OK);
}

int32_t EalMan::EalSourceAttributes(
        std::ifstream&   file,
        uint32_t&        bytesRead
        )
{
    uint32_t nums{};
    int32_t res = ReadNumsChunk(file, nums, bytesRead);
    if (res != toInt(EalError::OK))
    { return res; }
    m_data->nrSources = nums;
    if (m_data->nrSources > 0)
    {
        if (ReadNamsChunk(file, m_data->nrSources, m_data->srcNames, bytesRead) != toInt(EalError::OK)) { return toInt(EalError::FileInvalid); }

        //if (ReadFilsChunk(file, count, m_data->srcFileNames, bytesRead) != toInt(EalError::OK)) { return toInt(EalError::FileInvalid); }
        file.ignore(8 + static_cast<std::streamsize>(m_data->nrSources) * 260); // 8 bytes chunk + 260 bytes each string
		bytesRead += 8 + m_data->nrSources * 260;

        if (ReadDataChunk(file, m_data->nrSources, FourCC::SRCA, m_data->sources, bytesRead) != toInt(EalError::OK)) { return toInt(EalError::FileInvalid); }
        if (!isSRCAdataPlausible()) { return toInt(EalError::FileInvalid); }
    }
    return toInt(EalError::OK);
}

int32_t EalMan::EalGeometryAttributes(
        std::ifstream&                                          file,
        std::bitset<static_cast<uint32_t>(FourCCreq::COUNT)>&   reqFourCC,
        uint32_t&                                               bytesRead
        )
{
    uint32_t nums{};
    int32_t res = ReadNumsChunk(file, nums, bytesRead);
    if (res != toInt(EalError::OK))
    { return res; }
    if (nums > 0)
    {
        if (ReadNamsChunk(file, nums, m_data->geoNames, bytesRead) != toInt(EalError::OK)) { return toInt(EalError::FileInvalid); }
        reqFourCC.set(static_cast<uint32_t>(FourCCreq::GEMP_NAMS));

        //if (ReadFilsChunk(file, count, m_data->geomFileNames, bytesRead) != toInt(EalError::OK)) { return toInt(EalError::FileInvalid); }
        file.ignore(8 + static_cast<std::streamsize>(nums) * 260); // 8 bytes chunk + 260 bytes each string
		bytesRead += 8 + nums * 260;

        if (ReadGemaChunk(file, bytesRead) != toInt(EalError::OK)) { return toInt(EalError::FileInvalid); }
        reqFourCC.set(static_cast<uint32_t>(FourCCreq::GEMP_GEMA));
    }
    return toInt(EalError::OK);
}

inline bool EalMan::isGDFMdataPlausible() const
{
    return std::isfinite(m_data->gdfm.LFRatio);
}

inline bool EalMan::isDEFLdataPlausible() const
{
    return std::isfinite(m_data->defListener.fDistanceFactor)
            && std::isfinite(m_data->defListener.fRolloffFactor)
            && std::isfinite(m_data->defListener.fDopplerFactor);
}

inline bool EalMan::isDEFEdataPlausible() const
{
    return std::isfinite(m_data->defEnvironment.flRoomRolloffFactor)
            && std::isfinite(m_data->defEnvironment.flDecayTime)
            && std::isfinite(m_data->defEnvironment.flDecayHFRatio)
            && std::isfinite(m_data->defEnvironment.flReflectionsDelay)
            && std::isfinite(m_data->defEnvironment.flReverbDelay)
            && std::isfinite(m_data->defEnvironment.flEnvironmentSize)
            && std::isfinite(m_data->defEnvironment.flEnvironmentDiffusion)
            && std::isfinite(m_data->defEnvironment.flAirAbsorptionHF);
}

inline bool EalMan::isDEFSdataPlausible() const
{
    return std::isfinite(m_data->defSource.fConeXdir)
            && std::isfinite(m_data->defSource.fConeYdir)
            && std::isfinite(m_data->defSource.fConeZdir)
            && std::isfinite(m_data->defSource.fMinDistance)
            && std::isfinite(m_data->defSource.fMaxDistance)
            && std::isfinite(m_data->defSource.eaxAttributes.flRoomRolloffFactor)
            && std::isfinite(m_data->defSource.eaxAttributes.flObstructionLFRatio)
            && std::isfinite(m_data->defSource.eaxAttributes.flOcclusionLFRatio)
            && std::isfinite(m_data->defSource.eaxAttributes.flOcclusionRoomRatio)
            && std::isfinite(m_data->defSource.eaxAttributes.flAirAbsorptionFactor);
}


inline bool EalMan::isDEFMdataPlausible() const
{
    return std::isfinite(m_data->defObstacle.fLFRatio)
            && std::isfinite(m_data->defObstacle.fRoomRatio);
}

bool EalMan::isLISPdataPlausible() const
{
    bool plausible{true};
    for (const EAXListenerProperties& lp : m_data->environments)
    {
        plausible = plausible
                && std::isfinite(lp.flRoomRolloffFactor)
                && std::isfinite(lp.flDecayTime)
                && std::isfinite(lp.flDecayHFRatio)
                && std::isfinite(lp.flReflectionsDelay)
                && std::isfinite(lp.flReverbDelay)
                && std::isfinite(lp.flEnvironmentSize)
                && std::isfinite(lp.flEnvironmentDiffusion)
                && std::isfinite(lp.flAirAbsorptionHF)
                ;
    }
    return plausible;
}

bool EalMan::isMATAdataPlausible() const
{
    bool plausible{true};
    for (const MaterialAttributes& ma : m_data->obstacles)
    {
        plausible = plausible
                && std::isfinite(ma.fLFRatio)
                && std::isfinite(ma.fRoomRatio)
                ;
    }
    return plausible;
}

bool EalMan::isSRCAdataPlausible() const
{
    bool plausible{true};
    for (const SourceAttributes& sa : m_data->sources)
    {
        plausible = plausible
                && std::isfinite(sa.fConeXdir)
                && std::isfinite(sa.fConeYdir)
                && std::isfinite(sa.fConeZdir)
                && std::isfinite(sa.fMinDistance)
                && std::isfinite(sa.fMaxDistance)
                ;
    }
    return plausible;
}

bool EalMan::isGEMAdataPlausible() const
{
    bool plausible{true};
    plausible = plausible
            && (m_data->gemaNrSrcIDs == m_data->gemaSrcIDs.size())
            && (m_data->gemaNrSrcIDs == m_data->gemaSources.size())
            && (m_data->gemaNrEnvIDs == m_data->gemaEnvIDs.size())
            && (m_data->gemaNrEnvIDs == m_data->gemaDiffraction.size())
            && ((m_data->gemaNrEnvIDs + 1) * (m_data->gemaNrEnvIDs + 1) == m_data->gemaEnvironmentMatrix.size())
            && (m_data->gemaNrInnerNodes == m_data->gemaBSPinnerNodes.size())
            && (m_data->gemaNrOuterNodes == m_data->gemaBSPouterNodes.size())
            && (m_data->gemaNrDiffBox == m_data->gemaDiffBox.size())
            ;

    if (!plausible) { return plausible; }

    /* all environment IDs must be present inside the .eal */
    for (const int32_t& id : m_data->gemaEnvIDs)
    {
        plausible = plausible
                && (id >= static_cast<int32_t>(EMFLAG_IDDEFAULT))
                && (static_cast<uint32_t>(id) < m_data->nrEnvironments)
                ;
    }

    /* all source IDs must be present inside the .eal */
    for (const int32_t& id : m_data->gemaSrcIDs)
    {
        plausible = plausible
                && (id >= static_cast<int32_t>(EMFLAG_IDDEFAULT))
                && (static_cast<uint32_t>(id) < m_data->nrSources)
                ;
    }

    /* all source points must be valid floats */
    for (const EMPoint& ep : m_data->gemaSources)
    {
        plausible = plausible
                && std::isfinite(ep.fX)
                && std::isfinite(ep.fY)
                && std::isfinite(ep.fZ)
                ;
    }

    /* diffraction vector not yet fully understood */

    /* all indizes must point to valid obstacle IDs */
    for (const int32_t& idx : m_data->gemaEnvironmentMatrix)
    {
        plausible = plausible
                && (idx >= static_cast<int32_t>(EMFLAG_IDNONE))
                && (idx < static_cast<int32_t>(m_data->nrObstacles))
                ;
    }

    /* check all relevant members of the SplitNode */
    for (const SplitNode& node : m_data->gemaBSPinnerNodes)
    {
        plausible = plausible
                && std::isfinite(node.fX)
                && std::isfinite(node.fY)
                && std::isfinite(node.fZ)
                && std::isfinite(node.fNormalX)
                && std::isfinite(node.fNormalY)
                && std::isfinite(node.fNormalZ)
                && ((node.childFront & 0x80000000)
                        ? (((node.childFront & 0x7FFFFFFF) < m_data->gemaNrOuterNodes))
                        : (node.childFront <= m_data->gemaNrInnerNodes)
                        )
                && ((node.childBack & 0x80000000)
                        ? (((node.childBack & 0x7FFFFFFF) < m_data->gemaNrOuterNodes))
                        : (node.childBack <= m_data->gemaNrInnerNodes)
                        )
                ;
    }

    /* Zones must contain valid environment IDs */
    for (const Zone& zone : m_data->gemaBSPouterNodes)
    {
        plausible = plausible
                && (zone.indexEnvID >= static_cast<int32_t>(EMFLAG_IDNONE))
                && (zone.indexEnvID < static_cast<int32_t>(m_data->gemaNrEnvIDs))
                ;
    }

    return plausible;
}

EalMan::EalMan()
    : m_data(std::make_unique<EalData>())
{
}

EalMan::~EalMan() = default;

int32_t EalMan::GetDataSetSize(
        uint32_t&                         sizeDataSet,
        [[maybe_unused]] const uint32_t   flags
        ) const
{
    size_t memsize = sizeof(EalData);

    memsize += m_data->envNames.capacity() * sizeof(std::string);
    for (auto& s : m_data->envNames) { memsize += s.capacity(); }

    memsize += m_data->obsNames.capacity() * sizeof(std::string);
    for (auto& s : m_data->obsNames) { memsize += s.capacity(); }

    memsize += m_data->srcNames.capacity() * sizeof(std::string);
    for (auto& s : m_data->srcNames) { memsize += s.capacity(); }

    memsize += m_data->geoNames.capacity() * sizeof(std::string);
    for (auto& s : m_data->geoNames) { memsize += s.capacity(); }
    /*
    memsize += m_data->srcFileNames.capacity() * sizeof(std::string);
    for (auto& s : m_data->srcFileNames) { memsize += s.capacity(); }

    memsize += m_data->geomFileNames.capacity() * sizeof(std::string);
    for (auto& s : m_data->geomFileNames) { memsize += s.capacity(); }
    */
    memsize += m_data->environments.capacity() * sizeof(ListenerAttributes);
    memsize += m_data->obstacles.capacity() * sizeof(MaterialAttributes);
    memsize += m_data->sources.capacity() * sizeof(SourceAttributes);

    memsize += m_data->gemaEnvIDs.capacity() * sizeof(int32_t);
    memsize += m_data->gemaSrcIDs.capacity() * sizeof(int32_t);
    memsize += m_data->gemaSources.capacity() * sizeof(EMPoint);
    memsize += m_data->gemaDiffraction.capacity() * sizeof(int32_t);
    memsize += m_data->gemaEnvironmentMatrix.capacity() * m_data->gemaEnvironmentMatrix.capacity() * sizeof(int32_t);
    memsize += m_data->gemaBSPinnerNodes.capacity() * sizeof(SplitNode);
    memsize += m_data->gemaBSPouterNodes.capacity() * sizeof(Zone);
    memsize += m_data->gemaDiffBox.capacity() * sizeof(DiffractionBox);

    if (memsize > std::numeric_limits<uint32_t>::max()) { sizeDataSet = 0; } // eal file larger than 4GB
    else { sizeDataSet = static_cast<uint32_t>(memsize); }

    return toInt(EalError::OK);
}

int32_t EalMan::LoadDataSet(
        const std::string&                filename,
        [[maybe_unused]] const uint32_t   flags
        )
{
    std::filesystem::path filePath{filename};
    if (!std::filesystem::exists(filePath)) { return toInt(EalError::FileNotFound); }

    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) { return toInt(EalError::FileNotFound); }

    Chunk mainChunk{};
    if (mainChunk.read(file) != toInt(EalError::OK)
        || !mainChunk.Equals(FourCC::RIFF)
        || mainChunk.length == 0)
    { return toInt(EalError::FileInvalid); }

    std::array<char, 4> type{};
    if (!file.read(type.data(), type.size())
        || to_u32_le(type) != static_cast<uint32_t>(FourCC::EAL) )
    { return toInt(EalError::FileInvalid); }
    uint32_t bytesRead{4};

    std::bitset<static_cast<uint32_t>(FourCCreq::COUNT)> reqFourCC;
    while (bytesRead < mainChunk.length)
    {
        Chunk chunk{};
        if (chunk.read(file) == toInt(EalError::OK))
        {
            bytesRead += sizeof(chunk);

            if (chunk.Equals(FourCC::MAJV)) { if (EalGlobals(file, FourCC::MAJV, bytesRead) != toInt(EalError::OK)) { return toInt(EalError::VersionInvalid); }; continue; }
            if (chunk.Equals(FourCC::MINV)) { if (EalGlobals(file, FourCC::MINV, bytesRead) != toInt(EalError::OK)) { return toInt(EalError::VersionInvalid); }; continue; }
            if (chunk.Equals(FourCC::EXEP)) { if (EalGlobals(file, FourCC::EXEP, bytesRead) != toInt(EalError::OK)) { return toInt(EalError::FileInvalid); }; continue; }
            if (chunk.Equals(FourCC::CMDS)) { if (EalGlobals(file, FourCC::CMDS, bytesRead) != toInt(EalError::OK)) { return toInt(EalError::FileInvalid); }; continue; }
            if (chunk.Equals(FourCC::GDFM))
            {
                if (EalGlobals(file, FourCC::GDFM, bytesRead) != toInt(EalError::OK)) { return toInt(EalError::FileInvalid); };
                reqFourCC.set(static_cast<uint32_t>(FourCCreq::GDFM));
                continue;
            }
            if (chunk.Equals(FourCC::LISA))
            {
                if (EalGlobals(file, FourCC::LISA, bytesRead) != toInt(EalError::OK)) { return toInt(EalError::FileInvalid); };
                reqFourCC.set(static_cast<uint32_t>(FourCCreq::LISA));
                continue;
            }
            if (chunk.Equals(FourCC::DENV))
            {
                if (EalGlobals(file, FourCC::DENV, bytesRead) != toInt(EalError::OK)) { return toInt(EalError::FileInvalid); };
                reqFourCC.set(static_cast<uint32_t>(FourCCreq::DENV));
                continue;
            }
            if (chunk.Equals(FourCC::DSRC))
            {
                if (EalGlobals(file, FourCC::DSRC, bytesRead) != toInt(EalError::OK)) { return toInt(EalError::FileInvalid); };
                reqFourCC.set(static_cast<uint32_t>(FourCCreq::DSRC));
                continue;
            }
            if (chunk.Equals(FourCC::DFIL)) { if (EalGlobals(file, FourCC::DFIL, bytesRead) != toInt(EalError::OK)) { return toInt(EalError::FileInvalid); }; continue; }
            if (chunk.Equals(FourCC::DMAT))
            {
                if (EalGlobals(file, FourCC::DMAT, bytesRead) != toInt(EalError::OK)) { return toInt(EalError::FileInvalid); };
                reqFourCC.set(static_cast<uint32_t>(FourCCreq::DMAT));
                continue;
            }
            if (chunk.Equals(FourCC::LIST)) { if (EalList(file, reqFourCC, bytesRead) != toInt(EalError::OK)) { return toInt(EalError::FileInvalid); }; continue; }
        }
        else { break; }
    }

    //if (bytesRead != mainChunk.length) { return toInt(EalError::FileInvalid); } // test disabled because "DM-Morbias][.eal" has wrong chunk sizes in its data and would fail here
    if (!reqFourCC.all()) { return toInt(EalError::FileInvalid); } // at least one required Chunk could not be parsed

    return toInt(EalError::OK);
}

int32_t EalMan::FreeDataSet(
        [[maybe_unused]] const uint32_t   flags
        )
{
    m_data = std::make_unique<EalData>();
    return toInt(EalError::OK);
}

int32_t EalMan::GetListenerAttributes(
        ListenerAttributes&   lstAttributes
        ) const
{
    lstAttributes = m_data->defListener;
    return toInt(EalError::OK);
}

int32_t EalMan::GetSourceID(
        const std::string&   srcName,
        int32_t&             srcID
        ) const
{
    srcID = {};
    if (srcName.size() < 1) { return toInt(EalError::IdNotFound); }
    return GetID(srcName, srcID, m_data->srcNames);
}

int32_t EalMan::GetSourceAttributes(
        const int32_t       srcID,
        SourceAttributes&   srcAttributes
        ) const
{
    if (srcID == static_cast<int32_t>(EMFLAG_IDDEFAULT))
    {
        srcAttributes = m_data->defSource;
        return toInt(EalError::OK);
    }
    return GetAttributes(srcID, m_data->nrSources,  srcAttributes, m_data->sources);
}

int32_t EalMan::GetSourceNumInstances(
        const int32_t   srcID,
        int32_t&        srcInstances
        ) const
{
    srcInstances = {};
    if (srcID < 0 || static_cast<uint32_t>(srcID) >= m_data->nrSources) { return toInt(EalError::InvalidId); }

    srcInstances = std::count(m_data->gemaSrcIDs.begin(), m_data->gemaSrcIDs.end(), srcID);

    return toInt(EalError::OK);
}

int32_t EalMan::GetSourceInstancePos(
        const int32_t   srcID,
        const int32_t   srcInstance,
        EMPoint&        posInstance
        ) const
{
    posInstance = {};
    int32_t srcInst{};
    if (GetSourceNumInstances(srcID, srcInst) != toInt(EalError::OK)) { return toInt(EalError::InvalidId); } // already checks if srcID is valid
    if (srcInstance < 0 || srcInstance >= srcInst) { return toInt(EalError::InvalidId); }
    //if (m_data->gemaSources.size < m_data->gemaNrSrcIDs) { return toInt(EalError::InvalidId); } // not needed as long as a failing LoadDataSet() is handled by the user

    int32_t cnt{};
    for (uint32_t i = 0; i < m_data->gemaNrSrcIDs; i++)
    {
        if (m_data->gemaSrcIDs[i] == srcID) // loop does boundary check here
        {
            if (cnt < srcInstance) // srcInstance's element not yet found
            { cnt++; continue; }
            else // the initial checks ensure that this "stop and return the element" is reached
            { posInstance = m_data->gemaSources[i]; break; } // boundary check done in LoadDataSet()
        }
    }

    return toInt(EalError::OK);
}

int32_t EalMan::GetEnvironmentID(
        const std::string&   envName,
        int32_t&             envID
        ) const
{
    return GetID(envName, envID, m_data->envNames);
}

int32_t EalMan::GetEnvironmentAttributes(
        const int32_t            envID,
        EAXListenerProperties&   envAttributes
        ) const
{
    if (envID == static_cast<int32_t>(EMFLAG_IDDEFAULT))
    {
        envAttributes = m_data->defEnvironment;
        return toInt(EalError::OK);
    }
    return GetAttributes(envID, m_data->nrEnvironments, envAttributes, m_data->environments);
}

int32_t EalMan::GetMaterialID(
        const std::string&   matName,
        int32_t&             matID
        ) const
{
    return GetID(matName, matID, m_data->obsNames);
}

int32_t EalMan::GetMaterialAttributes(
        const int32_t         matID,
        MaterialAttributes&   matAttributes
        ) const
{
    if (matID == static_cast<int32_t>(EMFLAG_IDDEFAULT))
    {
        matAttributes = m_data->defObstacle;
        return toInt(EalError::OK);
    }
    return GetAttributes(matID, m_data->nrObstacles, matAttributes, m_data->obstacles);
}

int32_t EalMan::GetGeometrySetID(
        const std::string&   geomName,
        int32_t&             geomID
        ) const
{
    return GetID(geomName, geomID, m_data->geoNames);
}

int32_t EalMan::GetListenerDynamicAttributes(
        const int32_t    geomID,
        const EMPoint&   lstPos,
        int32_t&         envID,
        const uint32_t   flags
        )
{
    envID = {};
    //int32_t length = (int32_t)m_data->geoNames.size();
    //if (geomID < 0 || geomID >= length) { return toInt(EalError::IdNotFound); } // all UT .eal files only have a single geometry set, so the selection is skipped for now
    if (geomID != 0) { return toInt(EalError::IdNotFound); }

    if (lstPos.fX < -32768.0 || lstPos.fX > +32768.0
            || lstPos.fY < -32768.0 || lstPos.fY > +32768.0
            || lstPos.fZ < -32768.0 || lstPos.fZ > +32768.0)
    { return toInt(EalError::IdNotFound); }

    SplitNode node{};
    uint32_t child{0};
    while (!(child & 0x80000000)) // MSB not set -> index refers to a SplitNode; we're still traversing the tree
    {
        if (child < m_data->gemaNrInnerNodes )
        { node = m_data->gemaBSPinnerNodes[child]; }
        else { return toInt(EalError::IdNotFound); }

        child = (isInFront(lstPos, node.fX, node.fY, node.fZ, node.fNormalX, node.fNormalY, node.fNormalZ))
            ? node.childFront
            : node.childBack;
    }
    child &= 0x7FFFFFFF; // MSB unset to recover the índex of a Zone
    if (child >= m_data->gemaNrOuterNodes)
    { return toInt(EalError::IdNotFound); }
    Zone zone = m_data->gemaBSPouterNodes[child];

    if (zone.indexEnvID >= 0 && zone.indexEnvID < static_cast<int32_t>(m_data->gemaNrEnvIDs))
    { envID = m_data->gemaEnvIDs[zone.indexEnvID]; }
    else
    { envID = static_cast<int32_t>(EMFLAG_IDDEFAULT); }

    if (flags == EMFLAG_LOCKPOSITION)
    {
        m_listenerPosition = lstPos;
        m_listenerEnvIDIndex = EMFLAG_IDDEFAULT;
        for (uint32_t i = 0; i < m_data->gemaNrEnvIDs; i++)
        {
            if (m_data->gemaEnvIDs[i] == envID)
            {
                m_listenerEnvIDIndex = i;
                break;
            }
        }
    }

    return toInt(EalError::OK);
}

int32_t EalMan::GetSourceDynamicAttributes(
        const int32_t    geomID,
        const EMPoint&   srcPos,
        int32_t&         srcObstruction,
        float&           srcObstructionLF,
        int32_t&         srcOcclusion,
        float&           srcOcclusionLF,
        float&           srcOcclusionRM,
        EMPoint&         virtPos,
        const uint32_t   flags
        )
{
    // defaults
    srcObstruction = {0};
    srcObstructionLF = {0.0f};
    srcOcclusion = {0};
    srcOcclusionLF = {0.25f};
    srcOcclusionRM = {0.5f};
    virtPos = srcPos; // not implemented in EAX Manager either

    int32_t length = static_cast<int32_t>(m_data->geoNames.size());
    if (geomID < 0 || geomID >= length) { return toInt(EalError::IdNotFound); }

    if (srcPos.fX < -32768.0 || srcPos.fX > +32768.0
            || srcPos.fY < -32768.0 || srcPos.fY > +32768.0
            || srcPos.fZ < -32768.0 || srcPos.fZ > +32768.0)
    { return toInt(EalError::IdNotFound); }

    int32_t envID_srcPos{};
    uint32_t src_EnvIDIndex{EMFLAG_IDDEFAULT};
    if (GetListenerDynamicAttributes(geomID, srcPos, envID_srcPos, 0) == toInt(EalError::OK))
    {
        for (uint32_t i = 0; i < m_data->gemaNrEnvIDs; i++)
        {
            if (m_data->gemaEnvIDs[i] == envID_srcPos)
            {
                src_EnvIDIndex = i;
                break;
            }
        }
    }

    const int32_t obsID = m_data->gemaEnvironmentMatrix[
        (m_data->gemaNrEnvIDs + 1) * (m_listenerEnvIDIndex + 1) //row (nrEnv + 1) * (lstPosID + 1) -- lstPosID starts with EMFLAG_IDDEFAULT=-1
            + src_EnvIDIndex // column
    ];

    if (obsID > static_cast<int32_t>(EMFLAG_IDNONE))
    {
        if (obsID >= 0 && obsID < static_cast<int32_t>(m_data->nrObstacles))
        {
            if (m_data->obstacles[obsID].dwFlags == EMMATERIAL_OBSTRUCTS)
            {
                srcObstruction = m_data->obstacles[obsID].lLevel;
                srcObstructionLF = m_data->obstacles[obsID].fLFRatio;
            }
            else
            {
                srcOcclusion = m_data->obstacles[obsID].lLevel;
                srcOcclusionLF = m_data->obstacles[obsID].fLFRatio;
                srcOcclusionRM = m_data->obstacles[obsID].fRoomRatio;
            }
        }
        else // default Obstacle
        {
            if (m_data->defObstacle.dwFlags == EMMATERIAL_OBSTRUCTS)
            {
                srcObstruction = m_data->defObstacle.lLevel;
                srcObstructionLF = m_data->defObstacle.fLFRatio;
            }
            else
            {
                srcOcclusion = m_data->defObstacle.lLevel;
                srcOcclusionLF = m_data->defObstacle.fLFRatio;
                srcOcclusionRM = m_data->defObstacle.fRoomRatio;
            }
        }
    }

    if ((flags & EMFLAG_NODIFFRACTION) != 0)
    {
        return toInt(EalError::OK);
    }

    if (m_listenerEnvIDIndex == static_cast<uint32_t>(src_EnvIDIndex)) // source and listener in the same environment
    {

    }

    return toInt(EalError::OK);
}
