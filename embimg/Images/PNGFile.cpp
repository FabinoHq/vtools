////////////////////////////////////////////////////////////////////////////////
//  _______                       __________________________________________  //
//  \\ .   \            _________/ . . . . . . . . . . . . . . . . . . .   /  //
//   \\ .   \       ___/ . . . . .    ____________________________________/   //
//    \\ .   \   __/. . .   _________/  / \  // .    \ //.  ///. ______//     //
//     \\ .   \_//      ___/.  ____ ___/  / //.      ///.  ///. /_____        //
//      \\ .   \/     _/ //.  / //. /\   / //. /\   ///.  / \\ .      \       //
//       \\ .        /  //.  / //. / /  / //. / /  ///.  /   \\____    \      //
//        \\ .      /  //.  / //.  \/  / //.  \/  ///.  /________//    /      //
//         \\ .    /  //.  / // .     / // .     ///.       / .       /       //
//          \\____/  //___/  \\______/  \\______///________/_________/        //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////
//   This is free and unencumbered software released into the public domain.  //
//                                                                            //
//   Anyone is free to copy, modify, publish, use, compile, sell, or          //
//   distribute this software, either in source code form or as a compiled    //
//   binary, for any purpose, commercial or non-commercial, and by any        //
//   means.                                                                   //
//                                                                            //
//   In jurisdictions that recognize copyright laws, the author or authors    //
//   of this software dedicate any and all copyright interest in the          //
//   software to the public domain. We make this dedication for the benefit   //
//   of the public at large and to the detriment of our heirs and             //
//   successors. We intend this dedication to be an overt act of              //
//   relinquishment in perpetuity of all present and future rights to this    //
//   software under copyright law.                                            //
//                                                                            //
//   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,          //
//   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF       //
//   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   //
//   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR        //
//   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,    //
//   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR    //
//   OTHER DEALINGS IN THE SOFTWARE.                                          //
//                                                                            //
//   For more information, please refer to <https://unlicense.org>            //
////////////////////////////////////////////////////////////////////////////////
//    VTools : Virtual Tools                                                  //
//     Images/PNGFile.cpp : PNGFile image management                          //
////////////////////////////////////////////////////////////////////////////////
#include "PNGFile.h"


////////////////////////////////////////////////////////////////////////////////
//  PNGFile default constructor                                               //
////////////////////////////////////////////////////////////////////////////////
PNGFile::PNGFile() :
m_loaded(false),
m_image(0),
m_width(0),
m_height(0)
{

}

////////////////////////////////////////////////////////////////////////////////
//  PNGFile destructor                                                        //
////////////////////////////////////////////////////////////////////////////////
PNGFile::~PNGFile()
{
    if (m_image)
    {
        delete[] m_image;
    }
    m_image = 0;
    m_height = 0;
    m_width = 0;
    m_loaded = false;
}


////////////////////////////////////////////////////////////////////////////////
//  Set PNG file image                                                        //
//  return : True if PNG file image is successfully set                       //
////////////////////////////////////////////////////////////////////////////////
bool PNGFile::setImage(uint32_t width, uint32_t height,
    const unsigned char* image)
{
    // Check image data
    if (!image)
    {
        // Invalid image data
        return false;
    }

    // Check image loaded state
    if (m_loaded)
    {
        // Destroy current image
        destroyImage();
    }

    // Check image size
    if ((width <= 0) || (height <= 0) ||
        (width > PNGFileMaxImageWidth) ||
        (height > PNGFileMaxImageHeight))
    {
        // Invalid image size
        return false;
    }

    // Allocate image data
    size_t imageSize = (width*height*4);
    m_image = new (std::nothrow) unsigned char[imageSize];
    if (!m_image) return false;

    // Copy image data
    memcpy(m_image, image, imageSize);

    // Set image size
    m_width = width;
    m_height = height;

    // PNG file image is successfully set
    m_loaded = true;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Load PNG file                                                             //
//  return : True if PNG file is successfully loaded                          //
////////////////////////////////////////////////////////////////////////////////
bool PNGFile::loadImage(const std::string& filepath)
{
    // Check image loaded state
    if (m_loaded)
    {
        // Destroy current image
        destroyImage();
    }

    // Load PNG file
    std::ifstream pngFile;
    pngFile.open(filepath.c_str(), std::ios::in | std::ios::binary);
    if (!pngFile.is_open())
    {
        // Could not load PNG file
        return false;
    }

    // Read PNG file signature
    unsigned char pngSignature[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    pngFile.read((char*)pngSignature, 8);
    if (!pngFile)
    {
        // Could not read PNG file signature
        return false;
    }

    // Check PNG file signature
    if ((pngSignature[0] != PNGFileSignature[0]) ||
        (pngSignature[1] != PNGFileSignature[1]) ||
        (pngSignature[2] != PNGFileSignature[2]) ||
        (pngSignature[3] != PNGFileSignature[3]) ||
        (pngSignature[4] != PNGFileSignature[4]) ||
        (pngSignature[5] != PNGFileSignature[5]) ||
        (pngSignature[6] != PNGFileSignature[6]) ||
        (pngSignature[7] != PNGFileSignature[7]))
    {
        // Invalid PNG file signature
        return false;
    }

    // Read PNG file IHDR chunk header
    PNGFileChunkHeader pngIHDRChunkHeader = {0, {0, 0, 0, 0}};
    pngFile.read((char*)&pngIHDRChunkHeader, PNGFileChunkHeaderSize);
    if (!pngFile)
    {
        // Could not read PNG file IHDR chunk header
        return false;
    }
    pngIHDRChunkHeader.length = SysByteSwap32(pngIHDRChunkHeader.length);

    // Check PNG file IHDR chunk type
    if ((pngIHDRChunkHeader.type[0] != PNGFileIHDRChunkType[0]) ||
        (pngIHDRChunkHeader.type[1] != PNGFileIHDRChunkType[1]) ||
        (pngIHDRChunkHeader.type[2] != PNGFileIHDRChunkType[2]) ||
        (pngIHDRChunkHeader.type[3] != PNGFileIHDRChunkType[3]))
    {
        // Invalid PNG file IHDR chunk type
        return false;
    }

    // Check PNG file IHDR chunk length
    if (pngIHDRChunkHeader.length != PNGFileIHDRChunkSize)
    {
        // Invalid PNG file IHDR chunk length
        return false;
    }

    // Read PNG file IHDR chunk
    PNGFileIHDRChunk pngIHDRChunk = {0, 0, 0, 0, 0, 0, 0};
    pngFile.read((char*)&pngIHDRChunk, PNGFileIHDRChunkSize);
    if (!pngFile)
    {
        // Could not read PNG file IHDR chunk
        return false;
    }

    // Read PNG file IHDR chunk CRC
    uint32_t pngIHDRChunkCRC = 0;
    pngFile.read((char*)&pngIHDRChunkCRC, PNGFileChunkCRCSize);
    if (!pngFile)
    {
        // Could not read PNG file IHDR chunk CRC
        return false;
    }
    pngIHDRChunkCRC = SysByteSwap32(pngIHDRChunkCRC);

    // Check PNG file IHDR chunk CRC
    uint32_t checkIHDRChunkCRC = SysCRC32Default;
    checkIHDRChunkCRC = SysUpdateCRC32(
        checkIHDRChunkCRC, pngIHDRChunkHeader.type, PNGFileChunkHeaderTypeSize
    );
    checkIHDRChunkCRC = SysUpdateCRC32(
        checkIHDRChunkCRC, (unsigned char*)&pngIHDRChunk, PNGFileIHDRChunkSize
    );
    if ((checkIHDRChunkCRC^SysCRC32Final) != pngIHDRChunkCRC)
    {
        // Invalid PNG file IHDR chunk CRC
        return false;
    }

    // Swap PNG file IHDR chunk byte endianness
    pngIHDRChunk.width = SysByteSwap32(pngIHDRChunk.width);
    pngIHDRChunk.height = SysByteSwap32(pngIHDRChunk.height);

    // Check PNG file image size
    if ((pngIHDRChunk.width <= 0) || (pngIHDRChunk.height <= 0) ||
        (pngIHDRChunk.width > PNGFileMaxImageWidth) ||
        (pngIHDRChunk.height > PNGFileMaxImageHeight))
    {
        // Invalid PNG file image size
        return false;
    }

    // Check PNG file bit depth
    if (pngIHDRChunk.bitDepth != 8)
    {
        // Unsupported PNG file bit depth
        return false;
    }

    // Check PNG file compression
    if (pngIHDRChunk.compression != 0)
    {
        // Unsupported PNG file compression
        return false;
    }

    // Check PNG file filter
    if (pngIHDRChunk.filter != 0)
    {
        // Unsupported PNG file filter
        return false;
    }

    // Check PNG file interlace
    if (pngIHDRChunk.interlace != 0)
    {
        // Unsupported PNG file interlace
        return false;
    }

    // Load PNG file image data
    if (!loadPNGData(pngFile, pngIHDRChunk))
    {
        // Could not load PNG image data
        return false;
    }

    // Close PNG file
    pngFile.close();

    // PNG file is successfully loaded
    m_loaded = true;
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Save PNG file                                                             //
//  return : True if PNG file is successfully saved                           //
////////////////////////////////////////////////////////////////////////////////
bool PNGFile::saveImage(const std::string& filepath, PNGFileColorType colorType)
{
    // Check image loaded state
    if (!m_loaded)
    {
        // No current image to save
        return false;
    }

    // Save PNG file
    if (!savePNGImage(filepath, m_width, m_height, m_image, colorType))
    {
        // Could not save PNG file
        return false;
    }

    // PNG file is successfully saved
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Destroy PNG image                                                         //
////////////////////////////////////////////////////////////////////////////////
void PNGFile::destroyImage()
{
    if (m_image)
    {
        delete[] m_image;
    }
    m_image = 0;
    m_height = 0;
    m_width = 0;
    m_loaded = false;
}


////////////////////////////////////////////////////////////////////////////////
//  Save PNG image                                                            //
//  return : True if PNG image is successfully saved                          //
////////////////////////////////////////////////////////////////////////////////
bool PNGFile::savePNGImage(const std::string& filepath,
    uint32_t width, uint32_t height, const unsigned char* image,
    PNGFileColorType colorType)
{
    // Check image data
    if (!image)
    {
        // Invalid image data
        return false;
    }

    // Check image size
    if ((width <= 0) || (height <= 0) ||
        (width > PNGFileMaxImageWidth) ||
        (height > PNGFileMaxImageHeight))
    {
        // Invalid image size
        return false;
    }

    // Save PNG file
    std::ofstream pngFile;
    pngFile.open(
        filepath.c_str(), std::ios::out | std::ios::trunc | std::ios::binary
    );
    if (!pngFile.is_open())
    {
        // Could not save PNG file
        return false;
    }

    // Write PNG file signature
    pngFile.write((char*)PNGFileSignature, 8);
    if (!pngFile)
    {
        // Could not write PNG file signature
        return false;
    }

    // Write PNG file IHDR chunk header
    PNGFileChunkHeader pngIHDRChunkHeader;
    pngIHDRChunkHeader.length = SysByteSwap32(PNGFileIHDRChunkSize);
    pngIHDRChunkHeader.type[0] = PNGFileIHDRChunkType[0];
    pngIHDRChunkHeader.type[1] = PNGFileIHDRChunkType[1];
    pngIHDRChunkHeader.type[2] = PNGFileIHDRChunkType[2];
    pngIHDRChunkHeader.type[3] = PNGFileIHDRChunkType[3];
    pngFile.write((char*)&pngIHDRChunkHeader, PNGFileChunkHeaderSize);
    if (!pngFile)
    {
        // Could not write PNG file IHDR chunk header
        return false;
    }

    // Write PNG file IHDR chunk
    PNGFileIHDRChunk pngIHDRChunk;
    pngIHDRChunk.width = SysByteSwap32(width);
    pngIHDRChunk.height = SysByteSwap32(height);
    pngIHDRChunk.bitDepth = 8;
    pngIHDRChunk.colorType = (uint8_t)colorType;
    pngIHDRChunk.compression = 0;
    pngIHDRChunk.filter = 0;
    pngIHDRChunk.interlace = 0;
    pngFile.write((char*)&pngIHDRChunk, PNGFileIHDRChunkSize);
    if (!pngFile)
    {
        // Could not write PNG file IHDR chunk
        return false;
    }

    // Compute PNG file IHDR chunk CRC
    uint32_t pngIHDRChunkCRC = SysCRC32Default;
    pngIHDRChunkCRC = SysUpdateCRC32(
        pngIHDRChunkCRC, pngIHDRChunkHeader.type, PNGFileChunkHeaderTypeSize
    );
    pngIHDRChunkCRC = SysUpdateCRC32(
        pngIHDRChunkCRC, (unsigned char*)&pngIHDRChunk, PNGFileIHDRChunkSize
    );
    pngIHDRChunkCRC = SysByteSwap32(pngIHDRChunkCRC^SysCRC32Final);

    // Write PNG file IHDR chunk CRC
    pngFile.write((char*)&pngIHDRChunkCRC, PNGFileChunkCRCSize);
    if (!pngFile)
    {
        // Could not write PNG file IHDR chunk CRC
        return false;
    }

    // Swap PNG file IHDR chunk byte endianness
    pngIHDRChunk.width = SysByteSwap32(pngIHDRChunk.width);
    pngIHDRChunk.height = SysByteSwap32(pngIHDRChunk.height);

    // Save PNG file image data
    if (!savePNGData(pngFile, pngIHDRChunk, image))
    {
        // Could not save PNG image data
        return false;
    }

    // Write PNG file IEND chunk header
    PNGFileChunkHeader pngIENDChunkHeader;
    pngIENDChunkHeader.length = SysByteSwap32(PNGFileIENDChunkSize);
    pngIENDChunkHeader.type[0] = PNGFileIENDChunkType[0];
    pngIENDChunkHeader.type[1] = PNGFileIENDChunkType[1];
    pngIENDChunkHeader.type[2] = PNGFileIENDChunkType[2];
    pngIENDChunkHeader.type[3] = PNGFileIENDChunkType[3];
    pngFile.write((char*)&pngIENDChunkHeader, PNGFileChunkHeaderSize);
    if (!pngFile)
    {
        // Could not write PNG file IEND chunk header
        return false;
    }

    // Compute PNG file IEND chunk CRC
    uint32_t pngIENDChunkCRC = SysCRC32Default;
    pngIENDChunkCRC = SysUpdateCRC32(
        pngIENDChunkCRC, pngIENDChunkHeader.type, PNGFileChunkHeaderTypeSize
    );
    pngIENDChunkCRC = SysByteSwap32(pngIENDChunkCRC^SysCRC32Final);

    // Write PNG file IEND chunk CRC
    pngFile.write((char*)&pngIENDChunkCRC, PNGFileChunkCRCSize);
    if (!pngFile)
    {
        // Could not write PNG file IEND chunk CRC
        return false;
    }

    // Close PNG file
    pngFile.close();

    // PNG image is successfully saved
    return true;
}


////////////////////////////////////////////////////////////////////////////////
//  Load PNG file image data                                                  //
//  return : True if PNG file image data is successfully loaded               //
////////////////////////////////////////////////////////////////////////////////
bool PNGFile::loadPNGData(std::ifstream& pngFile,
    PNGFileIHDRChunk& pngIHDRChunk)
{
    // Set pixel depth
    uint32_t pixelDepth = 0;
    switch (pngIHDRChunk.colorType)
    {
        case PNGFILE_COLOR_GREYSCALE:
            pixelDepth = 1;
            break;
        case PNGFILE_COLOR_RGB:
            pixelDepth = 3;
            break;
        case PNGFILE_COLOR_PALETTE:
            // Unsupported PNG file color type
            return false;
        case PNGFILE_COLOR_GREYSCALE_ALPHA:
            pixelDepth = 2;
            break;
        case PNGFILE_COLOR_RGBA:
            pixelDepth = 4;
            break;
        default:
            // Unsupported PNG file color type
            return false;
    }

    // Read PNG file IDAT chunks headers
    std::streampos pngIDATstart = pngFile.tellg();
    size_t pngIDATChunksLength = 0;
    unsigned int pngIDATChunksCount = 0;
    PNGFileChunkHeader pngIDATChunkHeader;
    while (pngFile)
    {
        pngIDATChunkHeader = {0, {0, 0, 0, 0}};
        pngFile.read((char*)&pngIDATChunkHeader, PNGFileChunkHeaderSize);
        if (!pngFile)
        {
            // Could not read PNG file chunk header
            break;
        }
        pngIDATChunkHeader.length = SysByteSwap32(
            pngIDATChunkHeader.length
        );
        if ((pngIDATChunkHeader.type[0] == PNGFileIDATChunkType[0]) &&
            (pngIDATChunkHeader.type[1] == PNGFileIDATChunkType[1]) &&
            (pngIDATChunkHeader.type[2] == PNGFileIDATChunkType[2]) &&
            (pngIDATChunkHeader.type[3] == PNGFileIDATChunkType[3]))
        {
            // PNG file IDAT chunk header found
            pngIDATChunksLength += pngIDATChunkHeader.length;
            ++pngIDATChunksCount;
        }

        // Skip current chunk
        pngFile.ignore(pngIDATChunkHeader.length + PNGFileChunkCRCSize);
    }
    if (pngIDATChunksCount <= 0)
    {
        // Could not find any PNG file IDAT chunk header
        return false;
    }

    // Allocate raw image data
    unsigned char* rawData = new (std::nothrow)
        unsigned char[pngIDATChunksLength];
    if (!rawData)
    {
        // Could not allocate raw image data
        return false;
    }

    // Reset input file stream
    pngFile.clear();
    pngFile.seekg(pngIDATstart, std::ios::beg);

    // Read all IDAT chunks
    size_t rawDataOffset = 0;
    for (unsigned int i = 0; i < pngIDATChunksCount;)
    {
        pngIDATChunkHeader = {0, {0, 0, 0, 0}};
        pngFile.read((char*)&pngIDATChunkHeader, PNGFileChunkHeaderSize);
        if (!pngFile)
        {
            // Could not read PNG file chunk header
            if (rawData) { delete[] rawData; }
            return false;
        }
        pngIDATChunkHeader.length = SysByteSwap32(
            pngIDATChunkHeader.length
        );
        if ((pngIDATChunkHeader.type[0] != PNGFileIDATChunkType[0]) ||
            (pngIDATChunkHeader.type[1] != PNGFileIDATChunkType[1]) ||
            (pngIDATChunkHeader.type[2] != PNGFileIDATChunkType[2]) ||
            (pngIDATChunkHeader.type[3] != PNGFileIDATChunkType[3]))
        {
            // Skip current chunk
            pngFile.ignore(pngIDATChunkHeader.length + PNGFileChunkCRCSize);
        }
        else
        {
            // Read PNG file raw image data
            pngFile.read(
                (char*)&rawData[rawDataOffset], pngIDATChunkHeader.length
            );
            if (!pngFile)
            {
                // Could not read PNG raw image data
                if (rawData) { delete[] rawData; }
                return false;
            }

            // Read PNG file IDAT chunk CRC
            uint32_t pngIDATChunkCRC = 0;
            pngFile.read((char*)&pngIDATChunkCRC, PNGFileChunkCRCSize);
            if (!pngFile)
            {
                // Could not read PNG file IDAT chunk CRC
                if (rawData) { delete[] rawData; }
                return false;
            }
            pngIDATChunkCRC = SysByteSwap32(pngIDATChunkCRC);

            // Check PNG file IDAT chunk CRC
            uint32_t checkIDATChunkCRC = SysCRC32Default;
            checkIDATChunkCRC = SysUpdateCRC32(
                checkIDATChunkCRC, pngIDATChunkHeader.type,
                PNGFileChunkHeaderTypeSize
            );
            checkIDATChunkCRC = SysUpdateCRC32(
                checkIDATChunkCRC, &rawData[rawDataOffset],
                pngIDATChunkHeader.length
            );
            if ((checkIDATChunkCRC^SysCRC32Final) != pngIDATChunkCRC)
            {
                // Invalid PNG file IDAT chunk CRC
                if (rawData) { delete[] rawData; }
                return false;
            }

            // Increment raw data offset
            rawDataOffset += pngIDATChunkHeader.length;
            ++i;
        }
    }

    // Allocate decompressed data
    size_t pngDataSize =
        (pngIHDRChunk.width*pngIHDRChunk.height*pixelDepth)+pngIHDRChunk.height;
    unsigned char* pngData = new (std::nothrow) unsigned char[pngDataSize];
    if (!pngData)
    {
        // Could not allocate decompressed data
        if (rawData) { delete[] rawData; }
        return false;
    }

    // Decompress deflate data
    if (!ZLibDeflateDecompress(
        rawData, pngIDATChunksLength, pngData, &pngDataSize))
    {
        // Could not decompress deflate data
        if (pngData) { delete[] pngData; }
        if (rawData) { delete[] rawData; }
        return false;
    }

    // Allocate 32bits RGBA internal image data
    size_t imageSize = (pngIHDRChunk.width*pngIHDRChunk.height*4);
    m_image = new (std::nothrow) unsigned char[imageSize];
    if (!m_image)
    {
        // Could not allocate internal image data
        if (pngData) { delete[] pngData; }
        if (rawData) { delete[] rawData; }
        return false;
    }

    // Decode PNG image data
    switch (pngIHDRChunk.colorType)
    {
        case PNGFILE_COLOR_GREYSCALE:
            // Decode 8 bits greyscale PNG
            if (!decodePNG8bits(
                pngData, pngIHDRChunk.width, pngIHDRChunk.height))
            {
                // Could not decode 8 bits greyscale PNG
                if (pngData) { delete[] pngData; }
                if (rawData) { delete[] rawData; }
                if (m_image) { delete[] m_image; m_image = 0; }
                return false;
            }
            break;
        case PNGFILE_COLOR_RGB:
            // Decode 24 bits RGB PNG
            if (!decodePNG24bits(
                pngData, pngIHDRChunk.width, pngIHDRChunk.height))
            {
                // Could not decode 24 bits RGB PNG
                if (pngData) { delete[] pngData; }
                if (rawData) { delete[] rawData; }
                if (m_image) { delete[] m_image; m_image = 0; }
                return false;
            }
            break;
        case PNGFILE_COLOR_PALETTE:
            // Unsupported PNG file color type
            return false;
        case PNGFILE_COLOR_GREYSCALE_ALPHA:
            // Decode 16 bits greyscale-alpha PNG
            if (!decodePNG16bits(
                pngData, pngIHDRChunk.width, pngIHDRChunk.height))
            {
                // Could not decode 16 bits greyscale-alpha PNG
                if (pngData) { delete[] pngData; }
                if (rawData) { delete[] rawData; }
                if (m_image) { delete[] m_image; m_image = 0; }
                return false;
            }
            break;
        case PNGFILE_COLOR_RGBA:
            // Decode 32 bits RGBA PNG
            if (!decodePNG32bits(
                pngData, pngIHDRChunk.width, pngIHDRChunk.height))
            {
                // Could not decode 32 bits RGBA PNG
                if (pngData) { delete[] pngData; }
                if (rawData) { delete[] rawData; }
                if (m_image) { delete[] m_image; m_image = 0; }
                return false;
            }
            break;
        default:
            // Unsupported PNG file color type
            if (pngData) { delete[] pngData; }
            if (rawData) { delete[] rawData; }
            if (m_image) { delete[] m_image; m_image = 0; }
            return false;
    }

    // Destroy decompressed data
    if (pngData)
    {
        delete[] pngData;
    }

    // Destroy raw image data
    if (rawData)
    {
        delete[] rawData;
    }

    // Set image size
    m_width = pngIHDRChunk.width;
    m_height = pngIHDRChunk.height;

    // PNG file image data is successfully loaded
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Save PNG file image data                                                  //
//  return : True if PNG file image data is successfully saved                //
////////////////////////////////////////////////////////////////////////////////
bool PNGFile::savePNGData(std::ofstream& pngFile,
    PNGFileIHDRChunk& pngIHDRChunk, const unsigned char* image)
{
    // Check image data
    if (!image)
    {
        // Invalid image data
        return false;
    }

    // Set pixel depth
    uint32_t pixelDepth = 0;
    switch (pngIHDRChunk.colorType)
    {
        case PNGFILE_COLOR_GREYSCALE:
            pixelDepth = 1;
            break;
        case PNGFILE_COLOR_RGB:
            pixelDepth = 3;
            break;
        case PNGFILE_COLOR_PALETTE:
            // Unsupported PNG file color type
            return false;
        case PNGFILE_COLOR_GREYSCALE_ALPHA:
            pixelDepth = 2;
            break;
        case PNGFILE_COLOR_RGBA:
            pixelDepth = 4;
            break;
        default:
            // Unsupported PNG file color type
            return false;
    }

    // Allocate PNG data
    size_t pngDataSize =
        (pngIHDRChunk.width*pngIHDRChunk.height*pixelDepth)+pngIHDRChunk.height;
    unsigned char* pngData = new (std::nothrow) unsigned char[pngDataSize];
    if (!pngData)
    {
        // Could not allocate PNG data
        return false;
    }

    // Encode PNG image data
    switch (pngIHDRChunk.colorType)
    {
        case PNGFILE_COLOR_GREYSCALE:
            // Encode 8 bits greyscale PNG
            if (!encodePNG8bits(
                pngData, pngIHDRChunk.width, pngIHDRChunk.height, image))
            {
                // Could not encode 8 bits greyscale PNG
                if (pngData) { delete[] pngData; }
                return false;
            }
            break;
        case PNGFILE_COLOR_RGB:
            // Encode 24 bits RGB PNG
            if (!encodePNG24bits(
                pngData, pngIHDRChunk.width, pngIHDRChunk.height, image))
            {
                // Could not encode 24 bits RGB PNG
                if (pngData) { delete[] pngData; }
                return false;
            }
            break;
        case PNGFILE_COLOR_PALETTE:
            // Unsupported PNG file color type
            return false;
        case PNGFILE_COLOR_GREYSCALE_ALPHA:
            // Encode 16 bits greyscale-alpha PNG
            if (!encodePNG16bits(
                pngData, pngIHDRChunk.width, pngIHDRChunk.height, image))
            {
                // Could not encode 16 bits greyscale-alpha PNG
                if (pngData) { delete[] pngData; }
                return false;
            }
            break;
        case PNGFILE_COLOR_RGBA:
            // Encode 32 bits RGBA PNG
            if (!encodePNG32bits(
                pngData, pngIHDRChunk.width, pngIHDRChunk.height, image))
            {
                // Could not encode 32 bits RGBA PNG
                if (pngData) { delete[] pngData; }
                return false;
            }
            break;
        default:
            // Unsupported PNG file color type
            if (pngData) { delete[] pngData; }
            return false;
    }

    // Compute ZLib compressed data size
    size_t compressedDataSize = ZLibComputeDeflateCompressSize(pngDataSize);

    // Allocate compressed data
    unsigned char* compressedData = new (std::nothrow)
        unsigned char[compressedDataSize];
    if (!compressedData)
    {
        // Could not allocate compressed data
        if (pngData) { delete[] pngData; }
        return false;
    }

    // Compress deflate data
    if (!ZLibDeflateCompress(
        pngData, pngDataSize, compressedData, &compressedDataSize))
    {
        // Could not compress deflate data
        if (compressedData) { delete[] compressedData; }
        if (pngData) { delete[] pngData; }
        return false;
    }

    // Write PNG file IDAT chunk header
    PNGFileChunkHeader pngIDATChunkHeader;
    pngIDATChunkHeader.length = SysByteSwap32((uint32_t)compressedDataSize);
    pngIDATChunkHeader.type[0] = PNGFileIDATChunkType[0];
    pngIDATChunkHeader.type[1] = PNGFileIDATChunkType[1];
    pngIDATChunkHeader.type[2] = PNGFileIDATChunkType[2];
    pngIDATChunkHeader.type[3] = PNGFileIDATChunkType[3];
    pngFile.write((char*)&pngIDATChunkHeader, PNGFileChunkHeaderSize);
    if (!pngFile)
    {
        // Could not write PNG file chunk header
        if (compressedData) { delete[] compressedData; }
        if (pngData) { delete[] pngData; }
        return false;
    }

    // Write PNG file IDAT chunk
    pngFile.write((char*)compressedData, compressedDataSize);
    if (!pngFile)
    {
        // Could not write PNG file IDAT chunk
        if (compressedData) { delete[] compressedData; }
        if (pngData) { delete[] pngData; }
        return false;
    }

    // Compute PNG file IDAT chunk CRC
    uint32_t pngIDATChunkCRC = SysCRC32Default;
    pngIDATChunkCRC = SysUpdateCRC32(
        pngIDATChunkCRC, pngIDATChunkHeader.type, PNGFileChunkHeaderTypeSize
    );
    pngIDATChunkCRC = SysUpdateCRC32(
        pngIDATChunkCRC, compressedData, compressedDataSize
    );
    pngIDATChunkCRC = SysByteSwap32(pngIDATChunkCRC^SysCRC32Final);

    // Write PNG file IDAT chunk CRC
    pngFile.write((char*)&pngIDATChunkCRC, PNGFileChunkCRCSize);
    if (!pngFile)
    {
        // Could not write PNG file IDAT chunk CRC
        return false;
    }

    // Destroy compressed data
    if (compressedData)
    {
        delete[] compressedData;
    }

    // Destroy png data
    if (pngData)
    {
        delete[] pngData;
    }

    // PNG file image data is successfully saved
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Decode PNG 32 bits data                                                   //
//  return : True if PNG 32 bits data are successfully decoded                //
////////////////////////////////////////////////////////////////////////////////
bool PNGFile::decodePNG32bits(unsigned char* data,
    uint32_t width, uint32_t height)
{
    size_t inIndex = 0;
    size_t outIndex = 0;
    size_t scanlineSize = (width*4);
    for (uint32_t j = 0; j < height; ++j)
    {
        // Check scanline filter type
        switch (data[inIndex++])
        {
            case PNGFILE_FILTER_NONE:
            {
                // No filter
                memcpy(&m_image[outIndex], &data[inIndex], scanlineSize);
                outIndex += scanlineSize;
                inIndex += scanlineSize;
                break;
            }
            case PNGFILE_FILTER_SUB:
            {
                // Sub filter
                size_t prevIndex = inIndex;

                // Copy first pixel
                memcpy(&m_image[outIndex], &data[inIndex], 4);
                inIndex += 4;
                outIndex += 4;

                // Decode sub filtered scanline
                for (size_t i = 4; i < scanlineSize; ++i)
                {
                    m_image[outIndex++] =
                        (data[inIndex++] += data[prevIndex++]);
                }
                break;
            }
            case PNGFILE_FILTER_UP:
            {
                // Up filter
                size_t prevIndex = (inIndex-scanlineSize-1);
                unsigned char prevData = 0;

                // Decode up filtered scanline
                for (size_t i = 0; i < scanlineSize; ++i)
                {
                    if (j > 0) { prevData = data[prevIndex]; }
                    m_image[outIndex++] = (data[inIndex++] += prevData);
                    ++prevIndex;
                }
                break;
            }
            case PNGFILE_FILTER_AVERAGE:
            {
                // Average filter
                size_t prevIndex = (inIndex-scanlineSize-1);
                size_t prevIndex2 = inIndex;
                unsigned char prevData = 0;

                // Decode first pixel (up filtered)
                for (size_t i = 0; i < 4; ++i)
                {
                    if (j > 0) { prevData = (data[prevIndex]/2); }
                    m_image[outIndex++] = (data[inIndex++] += prevData);
                    ++prevIndex;
                }

                // Decode avarage filtered scanline
                for (size_t i = 4; i < scanlineSize; ++i)
                {
                    if (j > 0) { prevData = data[prevIndex]; }
                    m_image[outIndex++] = (data[inIndex++] +=
                        ((prevData + data[prevIndex2++])/2));
                    ++prevIndex;
                }
                break;
            }
            case PNGFILE_FILTER_PAETH:
            {
                // Paeth multibyte filter
                size_t prevIndex = (inIndex-scanlineSize-1);
                size_t prevIndex2 = prevIndex;
                size_t prevIndex3 = inIndex;
                unsigned char prevData = 0;
                unsigned char prevData2 = 0;

                // Decode first pixel
                for (size_t i = 0; i < 4; ++i)
                {
                    if (j > 0) { prevData = data[prevIndex]; }
                    m_image[outIndex++] = (data[inIndex++] += prevData);
                    ++prevIndex;
                }

                // Decode paeth filtered scanline
                for (size_t i = 4; i < scanlineSize; ++i)
                {
                    unsigned char currentData = data[prevIndex3++];
                    if (j > 0) { prevData = data[prevIndex]; }
                    if (j > 0) { prevData2 = data[prevIndex2++]; }
                    int p = prevData - prevData2;
                    int pc = currentData - prevData2;
                    int pa = (p < 0) ? -p : p;
                    int pb = (pc < 0) ? -pc : pc;
                    pc = ((p + pc) < 0) ? -(p + pc) : (p + pc);
                    if (pb < pa) { pa = pb; currentData = prevData; }
                    if (pc < pa) { currentData = prevData2; }
                    m_image[outIndex++] = (data[inIndex++] += currentData);
                    ++prevIndex;
                }
                break;
            }
            default:
                // Invalid filter type
                return false;
        }
    }

    // PNG 32 bits data are successfully decoded
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Encode PNG 32 bits data                                                   //
//  return : True if PNG 32 bits data are successfully encoded                //
////////////////////////////////////////////////////////////////////////////////
bool PNGFile::encodePNG32bits(unsigned char* data,
    uint32_t width, uint32_t height, const unsigned char* image)
{
    // Check image data
    if (!image)
    {
        // Invalid image data
        return false;
    }

    size_t inIndex = 0;
    size_t outIndex = 0;
    size_t scanlineSize = (width*4);
    for (uint32_t j = 0; j < height; ++j)
    {
        // No filter
        data[outIndex++] = PNGFILE_FILTER_NONE;

        // Encode unfiltered scanline
        memcpy(&data[outIndex], &image[inIndex], scanlineSize);
        outIndex += scanlineSize;
        inIndex += scanlineSize;
    }

    // PNG 32 bits data are successfully encoded
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Decode PNG 24 bits data                                                   //
//  return : True if PNG 24 bits data are successfully decoded                //
////////////////////////////////////////////////////////////////////////////////
bool PNGFile::decodePNG24bits(unsigned char* data,
    uint32_t width, uint32_t height)
{
    size_t inIndex = 0;
    size_t outIndex = 0;
    size_t scanlineSize = (width*3);
    for (uint32_t j = 0; j < height; ++j)
    {
        // Check scanline filter type
        switch (data[inIndex++])
        {
            case PNGFILE_FILTER_NONE:
            {
                // No filter
                for (size_t i = 0; i < scanlineSize; ++i)
                {
                    // RGB channels
                    m_image[outIndex++] = data[inIndex++];
                    // Alpha channel
                    if ((i % 3) == 2) { m_image[outIndex++] = 255; }
                }
                break;
            }
            case PNGFILE_FILTER_SUB:
            {
                // Sub filter
                size_t prevIndex = inIndex;

                // Copy first pixel
                memcpy(&m_image[outIndex], &data[inIndex], 3);
                inIndex += 3;
                outIndex += 3;
                // Add first pixel alpha channel
                m_image[outIndex++] = 255;

                // Decode sub filtered scanline
                for (size_t i = 3; i < scanlineSize; ++i)
                {
                    // RGB channels
                    m_image[outIndex++] =
                        (data[inIndex++] += data[prevIndex++]);
                    // Alpha channel
                    if ((i % 3) == 2) { m_image[outIndex++] = 255; }
                }
                break;
            }
            case PNGFILE_FILTER_UP:
            {
                // Up filter
                size_t prevIndex = (inIndex-scanlineSize-1);
                unsigned char prevData = 0;

                // Decode up filtered scanline
                for (size_t i = 0; i < scanlineSize; ++i)
                {
                    // RGB channels
                    if (j > 0) { prevData = data[prevIndex]; }
                    m_image[outIndex++] = (data[inIndex++] += prevData);
                    ++prevIndex;
                    // Alpha channel
                    if ((i % 3) == 2) { m_image[outIndex++] = 255; }
                }
                break;
            }
            case PNGFILE_FILTER_AVERAGE:
            {
                // Average filter
                size_t prevIndex = (inIndex-scanlineSize-1);
                size_t prevIndex2 = inIndex;
                unsigned char prevData = 0;

                // Decode first pixel (up filtered)
                for (size_t i = 0; i < 3; ++i)
                {
                    if (j > 0) { prevData = (data[prevIndex]/2); }
                    m_image[outIndex++] = (data[inIndex++] += prevData);
                    ++prevIndex;
                }
                // Add first pixel alpha channel
                m_image[outIndex++] = 255;

                // Decode average filtered scanline
                for (size_t i = 3; i < scanlineSize; ++i)
                {
                    // RGB channels
                    if (j > 0) { prevData = data[prevIndex]; }
                    m_image[outIndex++] = (data[inIndex++] +=
                        ((prevData + data[prevIndex2++])/2));
                    ++prevIndex;
                    // Alpha channel
                    if ((i % 3) == 2) { m_image[outIndex++] = 255; }
                }
                break;
            }
            case PNGFILE_FILTER_PAETH:
            {
                // Paeth multibyte filter
                size_t prevIndex = (inIndex-scanlineSize-1);
                size_t prevIndex2 = prevIndex;
                size_t prevIndex3 = inIndex;
                unsigned char prevData = 0;
                unsigned char prevData2 = 0;

                // Decode first pixel
                for (size_t i = 0; i < 3; ++i)
                {
                    if (j > 0) { prevData = data[prevIndex]; }
                    m_image[outIndex++] = (data[inIndex++] += prevData);
                    ++prevIndex;
                }
                // Add first pixel alpha channel
                m_image[outIndex++] = 255;

                // Decode paeth filtered scanline
                for (size_t i = 3; i < scanlineSize; ++i)
                {
                    // RGB channels
                    unsigned char currentData = data[prevIndex3++];
                    if (j > 0) { prevData = data[prevIndex]; }
                    if (j > 0) { prevData2 = data[prevIndex2++]; }
                    int p = prevData - prevData2;
                    int pc = currentData - prevData2;
                    int pa = (p < 0) ? -p : p;
                    int pb = (pc < 0) ? -pc : pc;
                    pc = ((p + pc) < 0) ? -(p + pc) : (p + pc);
                    if (pb < pa) { pa = pb; currentData = prevData; }
                    if (pc < pa) { currentData = prevData2; }
                    m_image[outIndex++] = (data[inIndex++] += currentData);
                    ++prevIndex;
                    // Alpha channel
                    if ((i % 3) == 2) { m_image[outIndex++] = 255; }
                }
                break;
            }
            default:
                // Invalid filter type
                return false;
        }
    }

    // PNG 24 bits data are successfully decoded
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Encode PNG 24 bits data                                                   //
//  return : True if PNG 24 bits data are successfully encoded                //
////////////////////////////////////////////////////////////////////////////////
bool PNGFile::encodePNG24bits(unsigned char* data,
    uint32_t width, uint32_t height, const unsigned char* image)
{
    // Check image data
    if (!image)
    {
        // Invalid image data
        return false;
    }

    size_t inIndex = 0;
    size_t outIndex = 0;
    size_t scanlineSize = (width*3);
    for (uint32_t j = 0; j < height; ++j)
    {
        // No filter
        data[outIndex++] = PNGFILE_FILTER_NONE;

        // Encode unfiltered scanline
        for (size_t i = 0; i < scanlineSize; ++i)
        {
            // RGB channels
            data[outIndex++] = image[inIndex++];
            // Alpha channel
            if ((i % 3) == 2) { ++inIndex; }
        }
    }

    // PNG 24 bits data are successfully encoded
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Decode PNG 16 bits data                                                   //
//  return : True if PNG 16 bits data are successfully decoded                //
////////////////////////////////////////////////////////////////////////////////
bool PNGFile::decodePNG16bits(unsigned char* data,
    uint32_t width, uint32_t height)
{
    size_t inIndex = 0;
    size_t outIndex = 0;
    size_t scanlineSize = (width*2);
    for (uint32_t j = 0; j < height; ++j)
    {
        // Check scanline filter type
        switch (data[inIndex++])
        {
            case PNGFILE_FILTER_NONE:
            {
                // No filter
                for (size_t i = 0; i < scanlineSize; i+=2)
                {
                    // RGB channels
                    m_image[outIndex++] = data[inIndex];
                    m_image[outIndex++] = data[inIndex];
                    m_image[outIndex++] = data[inIndex++];
                    // Alpha channel
                    m_image[outIndex++] = data[inIndex++];
                }
                break;
            }
            case PNGFILE_FILTER_SUB:
            {
                // Sub filter
                size_t prevIndex = inIndex;

                // Copy first pixel
                m_image[outIndex++] = data[inIndex];
                m_image[outIndex++] = data[inIndex];
                m_image[outIndex++] = data[inIndex++];
                // Alpha channel
                m_image[outIndex++] = data[inIndex++];

                // Decode sub filtered scanline
                for (size_t i = 2; i < scanlineSize; i+=2)
                {
                    // RGB channels
                    unsigned char greyscale =
                        (data[inIndex++] += data[prevIndex++]);
                    m_image[outIndex++] = greyscale;
                    m_image[outIndex++] = greyscale;
                    m_image[outIndex++] = greyscale;
                    // Alpha channel
                    m_image[outIndex++] =
                        (data[inIndex++] += data[prevIndex++]);
                }
                break;
            }
            case PNGFILE_FILTER_UP:
            {
                // Up filter
                size_t prevIndex = (inIndex-scanlineSize-1);
                unsigned char prevData = 0;

                // Decode up filtered scanline
                for (size_t i = 0; i < scanlineSize; i+=2)
                {
                    // RGB channels
                    if (j > 0) { prevData = data[prevIndex]; }
                    unsigned char greyscale = (data[inIndex++] += prevData);
                    ++prevIndex;
                    m_image[outIndex++] = greyscale;
                    m_image[outIndex++] = greyscale;
                    m_image[outIndex++] = greyscale;
                    // Alpha channel
                    if (j > 0) { prevData = data[prevIndex]; }
                    m_image[outIndex++] = (data[inIndex++] += prevData);
                    ++prevIndex;
                }
                break;
            }
            case PNGFILE_FILTER_AVERAGE:
            {
                // Average filter
                size_t prevIndex = (inIndex-scanlineSize-1);
                size_t prevIndex2 = inIndex;
                unsigned char prevData = 0;

                // Decode first pixel (up filtered)
                if (j > 0) { prevData = (data[prevIndex]/2); }
                unsigned char greyscale = (data[inIndex++] += prevData);
                ++prevIndex;
                m_image[outIndex++] = greyscale;
                m_image[outIndex++] = greyscale;
                m_image[outIndex++] = greyscale;
                // Alpha channel
                if (j > 0) { prevData = (data[prevIndex]/2); }
                m_image[outIndex++] = (data[inIndex++] += prevData);
                ++prevIndex;

                // Decode avarage filtered scanline
                for (size_t i = 2; i < scanlineSize; i+=2)
                {
                    // RGB channels
                    if (j > 0) { prevData = data[prevIndex]; }
                    greyscale = (data[inIndex++] +=
                        ((prevData + data[prevIndex2++])/2));
                    ++prevIndex;
                    m_image[outIndex++] = greyscale;
                    m_image[outIndex++] = greyscale;
                    m_image[outIndex++] = greyscale;
                    // Alpha channel
                    if (j > 0) { prevData = data[prevIndex]; }
                    m_image[outIndex++] = (data[inIndex++] +=
                        ((prevData + data[prevIndex2++])/2));
                    ++prevIndex;
                }
                break;
            }
            case PNGFILE_FILTER_PAETH:
            {
                // Paeth multibyte filter
                size_t prevIndex = (inIndex-scanlineSize-1);
                size_t prevIndex2 = prevIndex;
                size_t prevIndex3 = inIndex;
                unsigned char prevData = 0;
                unsigned char prevData2 = 0;

                // Decode first pixel
                if (j > 0) { prevData = data[prevIndex]; }
                unsigned char greyscale = (data[inIndex++] += prevData);
                ++prevIndex;
                m_image[outIndex++] = greyscale;
                m_image[outIndex++] = greyscale;
                m_image[outIndex++] = greyscale;
                // Alpha channel
                if (j > 0) { prevData = data[prevIndex]; }
                m_image[outIndex++] = (data[inIndex++] += prevData);
                ++prevIndex;

                // Decode paeth filtered scanline
                for (size_t i = 2; i < scanlineSize; i+=2)
                {
                    // RGB channels
                    unsigned char currentData = data[prevIndex3++];
                    if (j > 0) { prevData = data[prevIndex]; }
                    if (j > 0) { prevData2 = data[prevIndex2++]; }
                    int p = prevData - prevData2;
                    int pc = currentData - prevData2;
                    int pa = (p < 0) ? -p : p;
                    int pb = (pc < 0) ? -pc : pc;
                    pc = ((p + pc) < 0) ? -(p + pc) : (p + pc);
                    if (pb < pa) { pa = pb; currentData = prevData; }
                    if (pc < pa) { currentData = prevData2; }
                    greyscale = (data[inIndex++] += currentData);
                    ++prevIndex;
                    m_image[outIndex++] = greyscale;
                    m_image[outIndex++] = greyscale;
                    m_image[outIndex++] = greyscale;
                    // Alpha channel
                    currentData = data[prevIndex3++];
                    if (j > 0) { prevData = data[prevIndex]; }
                    if (j > 0) { prevData2 = data[prevIndex2++]; }
                    p = prevData - prevData2;
                    pc = currentData - prevData2;
                    pa = (p < 0) ? -p : p;
                    pb = (pc < 0) ? -pc : pc;
                    pc = ((p + pc) < 0) ? -(p + pc) : (p + pc);
                    if (pb < pa) { pa = pb; currentData = prevData; }
                    if (pc < pa) { currentData = prevData2; }
                    m_image[outIndex++] = (data[inIndex++] += currentData);
                    ++prevIndex;
                }
                break;
            }
            default:
                // Invalid filter type
                return false;
        }
    }

    // PNG 16 bits data are successfully decoded
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Encode PNG 16 bits data                                                   //
//  return : True if PNG 16 bits data are successfully encoded                //
////////////////////////////////////////////////////////////////////////////////
bool PNGFile::encodePNG16bits(unsigned char* data,
    uint32_t width, uint32_t height, const unsigned char* image)
{
    // Check image data
    if (!image)
    {
        // Invalid image data
        return false;
    }

    size_t inIndex = 0;
    size_t outIndex = 0;
    size_t scanlineSize = (width*2);
    for (uint32_t j = 0; j < height; ++j)
    {
        // No filter
        data[outIndex++] = PNGFILE_FILTER_NONE;

        // Encode unfiltered scanline
        for (size_t i = 0; i < scanlineSize; i+=2)
        {
            // Compute greyscale
            int greyscale = image[inIndex++];
            greyscale += image[inIndex++];
            greyscale += image[inIndex++];
            greyscale /= 3;
            if (greyscale <= 0) { greyscale = 0; }
            if (greyscale >= 255) { greyscale = 255; }
            data[outIndex++] = (unsigned char)greyscale;
            // Alpha channel
            data[outIndex++] = image[inIndex++];
        }
    }

    // PNG 16 bits data are successfully encoded
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Decode PNG 8 bits data                                                    //
//  return : True if PNG 8 bits data are successfully decoded                 //
////////////////////////////////////////////////////////////////////////////////
bool PNGFile::decodePNG8bits(unsigned char* data,
    uint32_t width, uint32_t height)
{
    size_t inIndex = 0;
    size_t outIndex = 0;
    size_t scanlineSize = (width*1);
    for (uint32_t j = 0; j < height; ++j)
    {
        // Check scanline filter type
        switch (data[inIndex++])
        {
            case PNGFILE_FILTER_NONE:
            {
                // No filter
                for (size_t i = 0; i < scanlineSize; ++i)
                {
                    // RGB channels
                    m_image[outIndex++] = data[inIndex];
                    m_image[outIndex++] = data[inIndex];
                    m_image[outIndex++] = data[inIndex++];
                    // Alpha channel
                    m_image[outIndex++] = 255;
                }
                break;
            }
            case PNGFILE_FILTER_SUB:
            {
                // Sub filter
                size_t prevIndex = inIndex;

                // Copy first pixel
                m_image[outIndex++] = data[inIndex];
                m_image[outIndex++] = data[inIndex];
                m_image[outIndex++] = data[inIndex++];
                // Add first pixel alpha channel
                m_image[outIndex++] = 255;

                // Decode sub filtered scanline
                for (size_t i = 1; i < scanlineSize; ++i)
                {
                    // RGB channels
                    unsigned char greyscale = 
                        (data[inIndex++] += data[prevIndex++]);
                    m_image[outIndex++] = greyscale;
                    m_image[outIndex++] = greyscale;
                    m_image[outIndex++] = greyscale;
                    // Alpha channel
                    m_image[outIndex++] = 255;
                }
                break;
            }
            case PNGFILE_FILTER_UP:
            {
                // Up filter
                size_t prevIndex = (inIndex-scanlineSize-1);
                unsigned char prevData = 0;

                // Decode up filtered scanline
                for (size_t i = 0; i < scanlineSize; ++i)
                {
                    // RGB channels
                    if (j > 0) { prevData = data[prevIndex]; }
                    unsigned char greyscale = (data[inIndex++] += prevData);
                    ++prevIndex;
                    m_image[outIndex++] = greyscale;
                    m_image[outIndex++] = greyscale;
                    m_image[outIndex++] = greyscale;
                    // Alpha channel
                    m_image[outIndex++] = 255;
                }
                break;
            }
            case PNGFILE_FILTER_AVERAGE:
            {
                // Average filter
                size_t prevIndex = (inIndex-scanlineSize-1);
                size_t prevIndex2 = inIndex;
                unsigned char prevData = 0;

                // Decode first pixel (up filtered)
                if (j > 0) { prevData = (data[prevIndex]/2); }
                unsigned char greyscale = (data[inIndex++] += prevData);
                ++prevIndex;
                m_image[outIndex++] = greyscale;
                m_image[outIndex++] = greyscale;
                m_image[outIndex++] = greyscale;
                // Add first pixel alpha channel
                m_image[outIndex++] = 255;

                // Decode average filtered scanline
                for (size_t i = 1; i < scanlineSize; ++i)
                {
                    // RGB channels
                    if (j > 0) { prevData = data[prevIndex]; }
                    greyscale = (data[inIndex++] +=
                        ((prevData + data[prevIndex2++])/2));
                    ++prevIndex;
                    m_image[outIndex++] = greyscale;
                    m_image[outIndex++] = greyscale;
                    m_image[outIndex++] = greyscale;
                    // Alpha channel
                    m_image[outIndex++] = 255;
                }
                break;
            }
            case PNGFILE_FILTER_PAETH:
            {
                // Paeth multibyte filter
                size_t prevIndex = (inIndex-scanlineSize-1);
                size_t prevIndex2 = prevIndex;
                size_t prevIndex3 = inIndex;
                unsigned char prevData = 0;
                unsigned char prevData2 = 0;

                // Decode first pixel
                if (j > 0) { prevData = data[prevIndex]; }
                unsigned char greyscale = (data[inIndex++] += prevData);
                ++prevIndex;
                m_image[outIndex++] = greyscale;
                m_image[outIndex++] = greyscale;
                m_image[outIndex++] = greyscale;
                // Add first pixel alpha channel
                m_image[outIndex++] = 255;

                // Decode paeth filtered scanline
                for (size_t i = 1; i < scanlineSize; ++i)
                {
                    // RGB channels
                    unsigned char currentData = data[prevIndex3++];
                    if (j > 0) { prevData = data[prevIndex]; }
                    if (j > 0) { prevData2 = data[prevIndex2++]; }
                    int p = prevData - prevData2;
                    int pc = currentData - prevData2;
                    int pa = (p < 0) ? -p : p;
                    int pb = (pc < 0) ? -pc : pc;
                    pc = ((p + pc) < 0) ? -(p + pc) : (p + pc);
                    if (pb < pa) { pa = pb; currentData = prevData; }
                    if (pc < pa) { currentData = prevData2; }
                    greyscale = (data[inIndex++] += currentData);
                    ++prevIndex;
                    m_image[outIndex++] = greyscale;
                    m_image[outIndex++] = greyscale;
                    m_image[outIndex++] = greyscale;
                    // Alpha channel
                    m_image[outIndex++] = 255;
                }
                break;
            }
            default:
                // Invalid filter type
                return false;
        }
    }

    // PNG 8 bits data are successfully decoded
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Encode PNG 8 bits data                                                    //
//  return : True if PNG 8 bits data are successfully encoded                 //
////////////////////////////////////////////////////////////////////////////////
bool PNGFile::encodePNG8bits(unsigned char* data,
    uint32_t width, uint32_t height, const unsigned char* image)
{
    // Check image data
    if (!image)
    {
        // Invalid image data
        return false;
    }

    size_t inIndex = 0;
    size_t outIndex = 0;
    size_t scanlineSize = (width*1);
    for (uint32_t j = 0; j < height; ++j)
    {
        // No filter
        data[outIndex++] = PNGFILE_FILTER_NONE;

        // Encode unfiltered scanline
        for (size_t i = 0; i < scanlineSize; ++i)
        {
            // Compute greyscale
            int greyscale = image[inIndex++];
            greyscale += image[inIndex++];
            greyscale += image[inIndex++];
            greyscale /= 3;
            if (greyscale <= 0) { greyscale = 0; }
            if (greyscale >= 255) { greyscale = 255; }
            data[outIndex++] = (unsigned char)greyscale;
            // Alpha channel
            ++inIndex;
        }
    }

    // PNG 8 bits data are successfully encoded
    return true;
}
