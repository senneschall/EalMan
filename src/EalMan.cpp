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
    const EMPoint point,
    const EMPoint base,
    const EMPoint normal
    )
{
    /*
     * EaxMan.dll selects Front if scalarProduct >= 0 and Back otherwise
     * it also checks if the point is within a factor x thickness of the wall. Both values are hardcoded inside EaxMan.dll to be 1.0f and can't be overwritten by data inside a .eal
     * if the point is within this are, EaxMan.dll returns the Front child
     * this can be simplified to the single expression implemented here
     */
    return scalarProduct(
        (point.fX - base.fX),
        (point.fY - base.fY),
        (point.fZ - base.fZ),
        normal.fX,
        normal.fY,
        normal.fZ
    ) >= -1.0f;
}
/*
static bool getIntersectionPoint(
    const EMPoint coordPointA,
    const EMPoint coordPointB,
    const EMPoint coordPlaneBase,
    const EMPoint vecPlaneNormal,
    EMPoint& intersection
    )
{
    intersection = {};
    float denominator =
              (coordPointB.fX - coordPointA.fX) * vecPlaneNormal.fX
            + (coordPointB.fY - coordPointA.fY) * vecPlaneNormal.fY
            + (coordPointB.fZ - coordPointA.fZ) * vecPlaneNormal.fZ;

    if (std::fpclassify(denominator) == FP_ZERO) { return false; }

    float partB = (
            (
                  (coordPlaneBase.fX - coordPointA.fX) * vecPlaneNormal.fX
                + (coordPlaneBase.fY - coordPointA.fY) * vecPlaneNormal.fY
                + (coordPlaneBase.fZ - coordPointA.fZ) * vecPlaneNormal.fZ
            ) / ( denominator )
        );
    float partA = 1.0f - partB;

    intersection.fX = partA * coordPointA.fX + partB * coordPointB.fX;
    intersection.fY = partA * coordPointA.fY + partB * coordPointB.fY;
    intersection.fZ = partA * coordPointA.fZ + partB * coordPointB.fZ;
    return true;
}

static bool isIntersectInsideDiffbox(
    const EMPoint& intersect,
    const uint32_t boxSide,
    const EMPoint& dbMin,
    const EMPoint& dbMax
)
{
    switch (boxSide)
    {
    case 0:
    case 1:
        // outside Z, therefore testing X/Y
        if (
            intersect.fX >= dbMin.fX
            && intersect.fX <= dbMax.fX
            && intersect.fY >= dbMin.fY
            && intersect.fY <= dbMax.fY
            )
        { return true; }
        else
        { return false; }

    case 2:
    case 3:
        // outside Y, therefore testing X/Z
        if (
            intersect.fX >= dbMin.fX
            && intersect.fX <= dbMax.fX
            && intersect.fZ >= dbMin.fZ
            && intersect.fZ <= dbMax.fZ
            )
        { return true; }
        else
        { return false; }

    case 4:
    case 5:
        // outside X, therefore testing Y/Z
        if (
            intersect.fY >= dbMax.fY
            && intersect.fY <= dbMax.fY
            && intersect.fZ >= dbMax.fZ
            && intersect.fZ <= dbMax.fZ
            )
        { return true; }
        else 
        { return false; }

    default:
        return true;
    }
}

static void normalize(
    EMPoint& v
)
{
    float len = sqrtf(scalarProduct(v.fX, v.fY, v.fZ, v.fX, v.fY, v.fZ));

    if (std::fpclassify(len) != FP_ZERO)
    {
        v.fX /= len;
        v.fY /= len;
        v.fZ /= len;
    }
}

static uint32_t computeDiffraction(
    const EMPoint& listener,
    const EMPoint& source,
    const EMPoint& intersection,
    const uint32_t boxSide,
    const EMPoint& boxMin,
    const EMPoint& boxMax,
    const float    angleMaxAttenuation
)
{
    EMPoint edgePoint = intersection;

    // 1. Projektion des Schnittpunkts auf die relevante Box-Kante
    switch (boxSide)
    {
    case 0:
    case 1:
        // projection onto X/Y
        if (listener.fY <= boxMin.fY || listener.fY >= boxMax.fY)
        {
            edgePoint.fY =
                (intersection.fY >= (boxMin.fY + boxMax.fY) * 0.5f)
                ? boxMin.fY
                : boxMax.fY;
        }
        else
        {
            edgePoint.fX =
                (intersection.fX >= (boxMin.fX + boxMax.fX) * 0.5f)
                ? boxMin.fX
                : boxMax.fX;
        }
        break;

    case 2:
    case 3:
        // projection onto X/Z
        if (listener.fZ < boxMin.fZ && listener.fZ > boxMax.fZ)
        {
            edgePoint.fX =
                (intersection.fX >= (boxMin.fX + boxMax.fX) * 0.5f)
                ? boxMin.fX
                : boxMax.fX;
        }
        else
        {
            edgePoint.fZ =
                (intersection.fZ >= (boxMin.fZ + boxMax.fZ) * 0.5f)
                ? boxMax.fZ
                : boxMin.fZ;
        }
        break;

    case 4:
    case 5:
        // projection onto Y/Z
        if (listener.fY <= boxMin.fY || listener.fY >= boxMax.fY)
        {
            edgePoint.fY =
                (intersection.fY >= (boxMin.fY + boxMax.fY) * 0.5f)
                ? boxMin.fY
                : boxMax.fY;
        }
        else
        {
            edgePoint.fZ =
                (intersection.fZ >= (boxMin.fZ + boxMax.fZ) * 0.5f)
                ? boxMax.fZ
                : boxMin.fZ;
        }
        break;

    default:
        return 0;
    }

    EMPoint lst = {
        listener.fX - edgePoint.fX,
        listener.fY - edgePoint.fY,
        listener.fZ - edgePoint.fZ
    };
    normalize(lst);

    EMPoint src = {
        source.fX - edgePoint.fX,
        source.fY - edgePoint.fY,
        source.fZ - edgePoint.fZ
    };
    normalize(src);

    // angle between lst-edge and src-edge
    float dot = scalarProduct(lst.fX, lst.fY, lst.fZ, src.fX, src.fY, src.fZ);

    // dot < 0 > angle > 90° -> attenuation possible
    if (dot < 0.0f)
    {
        float angle = dot + 1.0f; // 1 - cos(theta)

        if (angle < angleMaxAttenuation * 0.011111111f)
        {
            return static_cast<uint32_t>(angle);
        }
    }

    return 0;
}
*/
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
int32_t EalMan::ReadArrayData(
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
            /* return readString(file, m_data->defExep, bytesRead); */
            file.ignore(260);
		    bytesRead += 260;
            return toInt(EalError::OK);
        case FourCC::CMDS:
            /* return readString(file, m_data->defCmds, bytesRead); */
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
            /* return readString(file, m_data->defFile, bytesRead); */
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

    const uint32_t envMatrixSize = (nrEnvs + 1) * (nrEnvs + 1);

    if (nrSrcs > 0) // sources are optional
    {
        if (ReadArrayData(file, nrSrcs, m_data->gemaSrcIDs, bytesRead) != toInt(EalError::OK)) { return toInt(EalError::FileInvalid); } // Source IDs
        if (ReadArrayData(file, nrSrcs, m_data->gemaSources, bytesRead) != toInt(EalError::OK)) { return toInt(EalError::FileInvalid); } // Source Pos
    }
    if (!(nrEnvs > 0   && ReadArrayData(file, nrEnvs, m_data->gemaEnvIDs, bytesRead)  == toInt(EalError::OK))) { return toInt(EalError::FileInvalid); } // Env IDs
    if (!(nrEnvs > 0 && ReadArrayData(file, nrEnvs, m_data->gemaDiffraction, bytesRead) == toInt(EalError::OK))) { return toInt(EalError::FileInvalid); } // Diffraction
    if (!(envMatrixSize > 1 && ReadArrayData(file, envMatrixSize, m_data->gemaEnvironmentMatrix, bytesRead)  == toInt(EalError::OK))) { return toInt(EalError::FileInvalid); } // Environment Matrix
    if (!(nrPlanes > 0 && ReadArrayData(file, nrPlanes, m_data->gemaBSPinnerNodes, bytesRead)  == toInt(EalError::OK))) { return toInt(EalError::FileInvalid); } // BSP inner tree
    if (!(nrLeaves > 0 && ReadArrayData(file, nrLeaves, m_data->gemaBSPouterNodes, bytesRead)  == toInt(EalError::OK))) { return toInt(EalError::FileInvalid); } // BSP outer tree

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
    const int32_t res = ReadNumsChunk(file, nums, bytesRead);
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
    const int32_t res = ReadNumsChunk(file, nums, bytesRead);
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
    const int32_t res = ReadNumsChunk(file, nums, bytesRead);
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
    const int32_t res = ReadNumsChunk(file, nums, bytesRead);
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
                && std::isfinite(node.base.fX)
                && std::isfinite(node.base.fY)
                && std::isfinite(node.base.fZ)
                && std::isfinite(node.base.fX)
                && std::isfinite(node.base.fY)
                && std::isfinite(node.base.fZ)
                && ((node.childFront & 0x80000000)
                        ? (((node.childFront & 0x7FFFFFFF) < m_data->gemaNrOuterNodes))
                        : (node.childFront < m_data->gemaNrInnerNodes)
                        )
                && ((node.childBack & 0x80000000)
                        ? (((node.childBack & 0x7FFFFFFF) < m_data->gemaNrOuterNodes))
                        : (node.childBack < m_data->gemaNrInnerNodes)
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

    /* if (bytesRead != mainChunk.length) { return toInt(EalError::FileInvalid); } */ // test disabled because "DM-Morbias][.eal" has wrong chunk sizes in its data and would fail here
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
    if (srcInstance < 0 || srcInstance >= srcInst) { return toInt(EalError::InstanceNotFound); }
    /* if (m_data->gemaSources.size < m_data->gemaNrSrcIDs) { return toInt(EalError::InvalidId); } */

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
    if (geomID != 0
            || m_data->gemaNrInnerNodes == 0
            || m_data->gemaNrOuterNodes == 0)
    { return toInt(EalError::IdNotFound); }

    if (lstPos.fX < -32768.0 || lstPos.fX > +32768.0
            || lstPos.fY < -32768.0 || lstPos.fY > +32768.0
            || lstPos.fZ < -32768.0 || lstPos.fZ > +32768.0)
    { return toInt(EalError::IdNotFound); }

    SplitNode node{};
    uint32_t child{0};
    while (!(child & 0x80000000)) // MSB not set -> index refers to a SplitNode; we're still traversing the tree
    {
        /* if (child < m_data->gemaNrInnerNodes ) */ // existance of some elements inside vector ensured at function start
        { node = m_data->gemaBSPinnerNodes[child]; }
        /* else { return toInt(EalError::IdNotFound); } */

        child = (isInFront(lstPos, node.base, node.normal))
            ? node.childFront
            : node.childBack;
    }
    child &= 0x7FFFFFFF; // MSB unset to recover the índex of a Zone
    /* if (child >= m_data->gemaNrOuterNods)
    { return toInt(EalError::IdNotFound); } */
    const Zone zone = m_data->gemaBSPouterNodes[child];

    if (zone.indexEnvID >= 0)
    { envID = m_data->gemaEnvIDs[zone.indexEnvID]; }
    else
    { envID = static_cast<int32_t>(EMFLAG_IDDEFAULT); }

    if (flags == EMFLAG_LOCKPOSITION)
    {
        m_listenerPosition = lstPos;
        m_listenerEnvIDIndex = static_cast<int32_t>(EMFLAG_IDDEFAULT);
        for (uint32_t i = 0; i < m_data->gemaNrEnvIDs; i++)
        {
            if (m_data->gemaEnvIDs[i] == envID)
            {
                m_listenerEnvIDIndex = static_cast<int32_t>(i);
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
        [[maybe_unused]] const uint32_t   flags
        )
{
    // defaults
    srcObstruction = {0};
    srcObstructionLF = {0.0f};
    srcOcclusion = {0};
    srcOcclusionLF = {0.25f};
    srcOcclusionRM = {0.5f};
    virtPos = srcPos; // not implemented in EAX Manager either

    /* int32_t length = static_cast<int32_t>(m_data->geoNames.size()); */ // UT .eal files only have 1 geometry, so skip this
    /* if (geomID < 0 || geomID >= length ) { return toInt(EalError::IdNotFound); } */
    if (geomID != 0
            || m_data->gemaNrEnvIDs == 0)
    { return toInt(EalError::IdNotFound); }

    if (srcPos.fX < -32768.0 || srcPos.fX > +32768.0
            || srcPos.fY < -32768.0 || srcPos.fY > +32768.0
            || srcPos.fZ < -32768.0 || srcPos.fZ > +32768.0)
    { return toInt(EalError::IdNotFound); }

    int32_t envID_srcPos{};
    int32_t src_EnvIDIndex = static_cast<int32_t>(EMFLAG_IDDEFAULT);
    if (GetListenerDynamicAttributes(geomID, srcPos, envID_srcPos, 0) == toInt(EalError::OK))
    {
        for (uint32_t i = 0; i < m_data->gemaNrEnvIDs; i++)
        {
            if (m_data->gemaEnvIDs[i] == envID_srcPos)
            {
                src_EnvIDIndex = static_cast<int32_t>(i);
                break;
            }
        }
    }

    const int32_t obsID = m_data->gemaEnvironmentMatrix[
        (m_data->gemaNrEnvIDs + 1) * (m_listenerEnvIDIndex + 1) //row (nrEnv + 1) * (lstPosID + 1) -- lstPosID starts with EMFLAG_IDDEFAULT=-1
        + (src_EnvIDIndex + 1) // column
    ];

    if (obsID > static_cast<int32_t>(EMFLAG_IDNONE))
    {
        if (obsID >= 0)
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
/*
    if ((flags & EMFLAG_NODIFFRACTION) != 0)
    {
        return toInt(EalError::OK);
    }

    if (m_listenerEnvIDIndex == src_EnvIDIndex) // source and listener in the same environment
    {
        if (m_data->gemaNrDiffBox > 0)
        {
            uint32_t i{0};
            for (; i < m_data->gemaNrDiffBox; i++)
            {
                if (m_data->gemaDiffBox[i].lSubspaceID == m_listenerEnvIDIndex) { break; }
                if (m_data->gemaDiffBox[i].lSubspaceID > m_listenerEnvIDIndex) { return toInt(EalError::OK); }
            }
            if (i == m_data->gemaNrDiffBox) { return toInt(EalError::OK); }

            for (; i < m_data->gemaNrDiffBox; i++)
            {
                if (m_data->gemaDiffBox[i].lSubspaceID == m_listenerEnvIDIndex) { break; }

                std::bitset<6> relLstPos;

                if (m_data->gemaDiffBox[i].empMin.fX <= m_listenerPosition.fX)
                {
                    if (m_data->gemaDiffBox[i].empMax.fX < m_listenerPosition.fX)
                    { relLstPos.set(4); }
                }
                else
                { relLstPos.set(5); }

                if (m_data->gemaDiffBox[i].empMin.fY <= m_listenerPosition.fY)
                {
                    if (m_data->gemaDiffBox[i].empMax.fY <= m_listenerPosition.fY)
                    { relLstPos.set(2); }
                }
                else
                { relLstPos.set(3); }

                if (m_data->gemaDiffBox[i].empMin.fZ <= m_listenerPosition.fZ)
                {
                    if (m_data->gemaDiffBox[i].empMax.fZ <= m_listenerPosition.fZ)
                    {
                        relLstPos.set(0);
                    }
                }
                else
                {
                    relLstPos.set(1);
                }

                std::bitset<6> relSrcPos;

                if (m_data->gemaDiffBox[i].empMin.fX <= srcPos.fX)
                {
                    if (m_data->gemaDiffBox[i].empMax.fX <= srcPos.fX)
                    { relSrcPos.set(4); }
                }
                else
                { relSrcPos.set(5); }

                if (m_data->gemaDiffBox[i].empMin.fY <= srcPos.fY)
                {
                    if (m_data->gemaDiffBox[i].empMax.fY <= srcPos.fY)
                    { relSrcPos.set(2); }
                }
                else
                { relSrcPos.set(3); }

                if (m_data->gemaDiffBox[i].empMin.fZ <= srcPos.fZ)
                {
                    if (m_data->gemaDiffBox[i].empMax.fZ < srcPos.fZ)
                    { relSrcPos.set(0); }
                }
                else
                { relSrcPos.set(1); }
                */

                /*
                 * Bitset:
                 * 0b00x00000 -> (lst/src) Pos.X < diffBox->empMin.X
                 * 0b000x0000 -> (lst/src) Pos.X > diffBox->empMax.X
                 * 0b0000x000 -> (lst/src) Pos.Y < diffBox->empMin.Y
                 * 0b00000x00 -> (lst/src) Pos.Y > diffBox->empMax.Y
                 * 0b000000x0 -> (lst/src) Pos.Z < diffBox->empMin.Z
                 * 0b0000000x -> (lst/src) Pos.Z > diffBox->empMax.Z
                 */

                /*
                uint32_t cntExceedBound{0};
                uint32_t crossType{0};
                EMPoint basePointA{0};
                EMPoint basePointB{0};
                EMPoint vecNormalA{0};
                EMPoint vecNormalB{0};
                if (relLstPos == relSrcPos)
                {
                    switch (relLstPos.to_ulong())
                    {
                    case 0b00000001: // X inside (Min/Max) | Y inside (Min/Max) | Z > Max.Z
                        cntExceedBound = 1;
                        crossType = 1;
                        basePointA = m_data->gemaDiffBox[i].empMax;
                        vecNormalA = { 0.0f, 0.0f, 1.0f };
                        break;
                    case 0b00000010: // X inside (Min/Max) | Y inside (Min/Max) | Z < Min.Z
                        cntExceedBound = 1;
                        crossType = 0;
                        basePointA = m_data->gemaDiffBox[i].empMin;
                        vecNormalA = { 0.0f, 0.0f, -1.0f };
                        break;
                    case 0b00000100: // X inside (Min/Max) | Y > Max.Y          | Z inside (Min/Max)
                        cntExceedBound = 1;
                        crossType = 2;
                        basePointA = m_data->gemaDiffBox[i].empMax;
                        vecNormalA = { 0.0f, 1.0f, 0.0f };
                        break;
                    case 0b00000101: // X inside (Min/Max) | Y > Max.Y          | Z > Max.Z
                        cntExceedBound = 2;
                        crossType = 2;
                        basePointA = m_data->gemaDiffBox[i].empMax;
                        vecNormalA = { 0.0f, 1.0f, 0.0f };
                        basePointB = m_data->gemaDiffBox[i].empMax;
                        vecNormalB = { 0.0f, 0.0f, 1.0f };
                        break;
                    case 0b00000110: // X inside (Min/Max) | Y > Max.Y          | Z < Min.Z
                        cntExceedBound = 2;
                        crossType = 2;
                        basePointA = m_data->gemaDiffBox[i].empMax;
                        vecNormalA = { 0.0f, 1.0f, 0.0f };
                        basePointB = m_data->gemaDiffBox[i].empMin;
                        vecNormalB = { 0.0f, 0.0f, -1.0f };
                        break;
                    case 0b00001000: // X inside (Min/Max) | Y < Min.Y          | Z inside (Min/Max)
                        cntExceedBound = 1;
                        crossType = 3;
                        basePointA = m_data->gemaDiffBox[i].empMin;
                        vecNormalA = { 0.0f, -1.0f, 0.0f };
                        break;
                    case 0b00001001: // X inside (Min/Max) | Y < Min.Y          | Z > Max.Z
                        cntExceedBound = 2;
                        crossType = 3;
                        basePointA = m_data->gemaDiffBox[i].empMin;
                        vecNormalA = { 0.0f, -1.0f, 0.0f };
                        basePointB = m_data->gemaDiffBox[i].empMax;
                        vecNormalB = { 0.0f, 0.0f, 1.0f };
                        break;
                    case 0b00001010: // X inside (Min/Max) | Y < Min.Y          | Z < Min.Z
                        cntExceedBound = 2;
                        crossType = 3;
                        basePointA = m_data->gemaDiffBox[i].empMin;
                        vecNormalA = { 0.0f, -1.0f, 0.0f };
                        basePointB = m_data->gemaDiffBox[i].empMin;
                        vecNormalB = { 0.0f, 0.0f, -1.0f };
                        break;
                    case 0b00010000: // X > Max.X          | Y inside (Min/Max) | Z inside (Min/Max)
                        cntExceedBound = 1;
                        crossType = 5;
                        basePointA = m_data->gemaDiffBox[i].empMax;
                        vecNormalA = { 1.0f, 0.0f, 0.0f };
                        break;
                    case 0b00010001: // X > Max.X          | Y inside (Min/Max) | Z > Max.Z
                        cntExceedBound = 2;
                        crossType = 5;
                        basePointA = m_data->gemaDiffBox[i].empMax;
                        vecNormalA = { 1.0f, 0.0f, 0.0f };
                        basePointB = m_data->gemaDiffBox[i].empMax;
                        vecNormalB = { 0.0f, 0.0f, 1.0f };
                        break;
                    case 0b00010010: // X > Max.X          | Y inside (Min/Max) | Z < Min.Z
                        cntExceedBound = 2;
                        crossType = 5;
                        basePointA = m_data->gemaDiffBox[i].empMax;
                        vecNormalA = { 1.0f, 0.0f, 0.0f };
                        basePointB = m_data->gemaDiffBox[i].empMin;
                        vecNormalB = { 0.0f, 0.0f, -1.0f };
                        break;
                    case 0b00010100: // X > Max.X          | Y > Max.Y          | Z inside (Min/Max)
                        cntExceedBound = 2;
                        crossType = 5;
                        basePointA = m_data->gemaDiffBox[i].empMax;
                        vecNormalA = { 1.0f, 0.0f, 0.0f };
                        basePointB = m_data->gemaDiffBox[i].empMax;
                        vecNormalB = { 0.0f, 1.0f, 0.0f };
                        break;
                    case 0b00010101: // X > Max.X          | Y > Max.Y          | Z > Max.Z
                        cntExceedBound = 3;
                        crossType = 5;
                        basePointA = m_data->gemaDiffBox[i].empMax;
                        vecNormalA = { 1.0f, 0.0f, 0.0f };
                        basePointB = m_data->gemaDiffBox[i].empMax;
                        vecNormalB = { 0.0f, 1.0f, 0.0f };
                        break;
                    case 0b00010110: // X > Max.X          | Y > Max.Y          | Z < Min.Z
                        cntExceedBound = 3;
                        crossType = 5;
                        basePointA = m_data->gemaDiffBox[i].empMax;
                        vecNormalA = { 1.0f, 0.0f, 0.0f };
                        basePointB = m_data->gemaDiffBox[i].empMax;
                        vecNormalB = { 0.0f, 1.0f, 0.0f };
                        break;
                    case 0b00011000: // X > Max.X          | Y < Min.Y          | Z inside (Min/Max)
                        cntExceedBound = 2;
                        crossType = 5;
                        basePointA = m_data->gemaDiffBox[i].empMax;
                        vecNormalA = { 1.0f, 0.0f, 0.0f };
                        basePointB = m_data->gemaDiffBox[i].empMin;
                        vecNormalB = { 0.0f, -1.0f, 0.0f };
                        break;
                    case 0b00011001: // X > Max.X          | Y < Min.Y          | Z > Max.Z
                        cntExceedBound = 3;
                        crossType = 5;
                        basePointA = m_data->gemaDiffBox[i].empMax;
                        vecNormalA = { 1.0f, 0.0f, 0.0f };
                        basePointB = m_data->gemaDiffBox[i].empMin;
                        vecNormalB = { 0.0f, -1.0f, 0.0f };
                        break;
                    case 0b00011010: // X > Max.X          | Y < Min.Y          | Z < Min.Z
                        cntExceedBound = 3;
                        crossType = 5;
                        basePointA = m_data->gemaDiffBox[i].empMax;
                        vecNormalA = { 1.0f, 0.0f, 0.0f };
                        basePointB = m_data->gemaDiffBox[i].empMin;
                        vecNormalB = { 0.0f, -1.0f, 0.0f };
                        break;
                    case 0b00100000: // X < Min.X          | Y inside (Min/Max) | Z inside (Min/Max)
                        cntExceedBound = 1;
                        crossType = 4;
                        basePointA = m_data->gemaDiffBox[i].empMin;
                        vecNormalA = { -1.0f, 0.0f, 0.0f };
                        break;
                    case 0b00100001: // X < Min.X          | Y inside (Min/Max) | Z > Max.Z
                        cntExceedBound = 2;
                        crossType = 4;
                        basePointA = m_data->gemaDiffBox[i].empMin;
                        vecNormalA = { -1.0f, 0.0f, 0.0f };
                        basePointB = m_data->gemaDiffBox[i].empMax;
                        vecNormalB = { 0.0f, 0.0f, 1.0f };
                        break;
                    case 0b00100010: // X < Min.X          | Y inside (Min/Max) | Z < Min.Z
                        cntExceedBound = 2;
                        crossType = 4;
                        basePointA = m_data->gemaDiffBox[i].empMin;
                        vecNormalA = { -1.0f, 0.0f, 0.0f };
                        basePointB = m_data->gemaDiffBox[i].empMin;
                        vecNormalB = { 0.0f, 0.0f, -1.0f };
                        break;
                    case 0b00100100: // X < Min.X          | Y > Max.Y          | Z inside (Min/Max)
                        cntExceedBound = 2;
                        crossType = 4;
                        basePointA = m_data->gemaDiffBox[i].empMin;
                        vecNormalA = { -1.0f, 0.0f, 0.0f };
                        basePointB = m_data->gemaDiffBox[i].empMax;
                        vecNormalB = { 0.0f, 1.0f, 0.0f };
                        break;
                    case 0b00100101: // X < Min.X          | Y > Max.Y          | Z > Max.Z
                        cntExceedBound = 3;
                        crossType = 4;
                        basePointA = m_data->gemaDiffBox[i].empMin;
                        vecNormalA = { -1.0f, 0.0f, 0.0f };
                        basePointB = m_data->gemaDiffBox[i].empMax;
                        vecNormalB = { 0.0f, 1.0f, 0.0f };
                        break;
                    case 0b00100110: // X < Min.X          | Y > Max.Y          | Z < Min.Z
                        cntExceedBound = 3;
                        crossType = 4;
                        basePointA = m_data->gemaDiffBox[i].empMin;
                        vecNormalA = { -1.0f, 0.0f, 0.0f };
                        basePointB = m_data->gemaDiffBox[i].empMax;
                        vecNormalB = { 0.0f, 1.0f, 0.0f };
                        break;
                    case 0b00101000: // X < Min.X          | Y < Min.Y          | Z inside (Min/Max)
                        cntExceedBound = 2;
                        crossType = 4;
                        basePointA = m_data->gemaDiffBox[i].empMin;
                        vecNormalA = { -1.0f, 0.0f, 0.0f };
                        basePointB = m_data->gemaDiffBox[i].empMin;
                        vecNormalB = { 0.0f, -1.0f, 0.0f };
                        break;
                    case 0b00101001: // X < Min.X          | Y < Min.Y          | Z > Max.Z
                        cntExceedBound = 3;
                        crossType = 4;
                        basePointA = m_data->gemaDiffBox[i].empMin;
                        vecNormalA = { -1.0f, 0.0f, 0.0f };
                        basePointB = m_data->gemaDiffBox[i].empMin;
                        vecNormalB = { 0.0f, -1.0f, 0.0f };
                        break;
                    case 0b00101010: // X < Min.X          | Y < Min.Y          | Z < Min.Z
                        cntExceedBound = 3;
                        crossType = 4;
                        basePointA = m_data->gemaDiffBox[i].empMin;
                        vecNormalA = { -1.0f, 0.0f, 0.0f };
                        basePointB = m_data->gemaDiffBox[i].empMin;
                        vecNormalB = { 0.0f, -1.0f, 0.0f };
                        break;
                    default: //case0 // X inside (Min/Max) | Y inside (Min/Max) | Z inside (Min/Max)
                        break;
                    }

                    for (uint32_t i = 0; i < cntExceedBound; i++)
                    {
                    }
                }
            }

            if (m_data->gdfm.MaxAttenuation < 0)
            {
                srcObstruction = std::min(srcObstruction, m_data->gdfm.MaxAttenuation);
                srcObstructionLF = std::min(srcObstructionLF, m_data->gdfm.LFRatio);
            }
        }
    }
*/
    return toInt(EalError::OK);
}
