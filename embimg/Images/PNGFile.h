////////////////////////////////////////////////////////////////////////////////
//  _______                       __________________________________________  //
//  \\ .   \            _________/ . . . . . . . . . . . . . . . . . . .   /  //
//   \\ .   \       ___/ . . . . .    ____________________________________/   //
//    \\ .   \   __/. . .   _________/  / \ // .    \ //.  ///. _______/      //
//     \\ .   \_//      ___/.  ____ ___/  ///.      ///.  ///. /____          //
//      \\ .   \/     _/ //.  / //. /\   ///. /\   ///.  / \\ .     \         //
//       \\ .        /  //.  / //. / /  ///. / /  ///.  /   \\____   \        //
//        \\ .      /  //.  / //.  \/  ///.  \/  ///.  /________//   /        //
//         \\ .    /  //.  / // .     /// .     ///.       / .      /         //
//          \\____/  //___/  \\______/ \\______///________/________/          //
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
//     Images/PNGFile.h : PNGFile image management                            //
////////////////////////////////////////////////////////////////////////////////
#ifndef VTOOLS_EMBIMG_IMAGES_PNGFILE_HEADER
#define VTOOLS_EMBIMG_IMAGES_PNGFILE_HEADER

    #include "../System/System.h"
    #include "../System/SysCPU.h"
    #include "../System/SysCRC.h"
    #include "../Compress/ZLib.h"

    #include <cstddef>
    #include <cstdint>
    #include <cstring>
    #include <string>
    #include <fstream>
    #include <new>


    ////////////////////////////////////////////////////////////////////////////
    //  PNGFile image settings                                                //
    ////////////////////////////////////////////////////////////////////////////
    const unsigned char PNGFileSignature[8] =
        {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
    const uint32_t PNGFileMaxImageWidth = 4096;
    const uint32_t PNGFileMaxImageHeight = 4096;


    ////////////////////////////////////////////////////////////////////////////
    //  PNGFile Chunk header structure                                        //
    ////////////////////////////////////////////////////////////////////////////
    struct PNGFileChunkHeader
    {
        uint32_t        length;
        unsigned char   type[4];
    };
    const uint32_t PNGFileChunkHeaderSize = 8;
    const uint32_t PNGFileChunkHeaderTypeSize = 4;
    const uint32_t PNGFileChunkCRCSize = 4;

    ////////////////////////////////////////////////////////////////////////////
    //  PNGFile IHDR chunk structure                                          //
    ////////////////////////////////////////////////////////////////////////////
    const unsigned char PNGFileIHDRChunkType[4] = {0x49, 0x48, 0x44, 0x52};
    struct PNGFileIHDRChunk
    {
        uint32_t    width;
        uint32_t    height;
        uint8_t     bitDepth;
        uint8_t     colorType;
        uint8_t     compression;
        uint8_t     filter;
        uint8_t     interlace;
    };
    const uint32_t PNGFileIHDRChunkSize = 13;

    ////////////////////////////////////////////////////////////////////////////
    //  PNGFile IDAT chunk type                                               //
    ////////////////////////////////////////////////////////////////////////////
    const unsigned char PNGFileIDATChunkType[4] = {0x49, 0x44, 0x41, 0x54};

    ////////////////////////////////////////////////////////////////////////////
    //  PNGFile IEND chunk type                                               //
    ////////////////////////////////////////////////////////////////////////////
    const unsigned char PNGFileIENDChunkType[4] = {0x49, 0x45, 0x4E, 0x44};
    const uint32_t PNGFileIENDChunkSize = 0;

    ////////////////////////////////////////////////////////////////////////////
    //  PNGFile color type enumeration                                        //
    ////////////////////////////////////////////////////////////////////////////
    enum PNGFileColorType
    {
        PNGFILE_COLOR_GREYSCALE = 0,
        PNGFILE_COLOR_RGB = 2,
        PNGFILE_COLOR_PALETTE = 3,
        PNGFILE_COLOR_GREYSCALE_ALPHA = 4,
        PNGFILE_COLOR_RGBA = 6
    };

    ////////////////////////////////////////////////////////////////////////////
    //  PNGFile filter type enumeration                                       //
    ////////////////////////////////////////////////////////////////////////////
    enum PNGFileFilterType
    {
        PNGFILE_FILTER_NONE = 0,
        PNGFILE_FILTER_SUB = 1,
        PNGFILE_FILTER_UP = 2,
        PNGFILE_FILTER_AVERAGE = 3,
        PNGFILE_FILTER_PAETH = 4
    };


    ////////////////////////////////////////////////////////////////////////////
    //  PNGFile class definition                                              //
    ////////////////////////////////////////////////////////////////////////////
    class PNGFile
    {
        public:
            ////////////////////////////////////////////////////////////////////
            //  PNGFile default constructor                                   //
            ////////////////////////////////////////////////////////////////////
            PNGFile();

            ////////////////////////////////////////////////////////////////////
            //  PNGFile destructor                                            //
            ////////////////////////////////////////////////////////////////////
            ~PNGFile();


            ////////////////////////////////////////////////////////////////////
            //  Set PNG file image                                            //
            //  return : True if PNG file image is successfully set           //
            ////////////////////////////////////////////////////////////////////
            bool setImage(uint32_t width, uint32_t height,
                const unsigned char* image);

            ////////////////////////////////////////////////////////////////////
            //  Load PNG file                                                 //
            //  return : True if PNG file is successfully loaded              //
            ////////////////////////////////////////////////////////////////////
            bool loadImage(const std::string& filepath);

            ////////////////////////////////////////////////////////////////////
            //  Save PNG file                                                 //
            //  return : True if PNG file is successfully saved               //
            ////////////////////////////////////////////////////////////////////
            bool saveImage(const std::string& filepath,
                PNGFileColorType colorType = PNGFILE_COLOR_RGBA);

            ////////////////////////////////////////////////////////////////////
            //  Destroy PNG image                                             //
            ////////////////////////////////////////////////////////////////////
            void destroyImage();


            ////////////////////////////////////////////////////////////////////
            //  Get PNG file loaded state                                     //
            //  return : True if PNG file is loaded                           //
            ////////////////////////////////////////////////////////////////////
            inline bool isLoaded() const
            {
                return m_loaded;
            }

            ////////////////////////////////////////////////////////////////////
            //  Get PNG file image data                                       //
            //  return : PNG file image data                                  //
            ////////////////////////////////////////////////////////////////////
            inline unsigned char* getImage()
            {
                return m_image;
            }

            ////////////////////////////////////////////////////////////////////
            //  Get PNG file image width                                      //
            //  return : PNG file image width in pixels                       //
            ////////////////////////////////////////////////////////////////////
            inline uint32_t getWidth() const
            {
                return m_width;
            }

            ////////////////////////////////////////////////////////////////////
            //  Get PNG file image height                                     //
            //  return : PNG file image height in pixels                      //
            ////////////////////////////////////////////////////////////////////
            inline uint32_t getHeight() const
            {
                return m_height;
            }


            ////////////////////////////////////////////////////////////////////
            //  Save PNG image                                                //
            //  return : True if PNG image is successfully saved              //
            ////////////////////////////////////////////////////////////////////
            static bool savePNGImage(const std::string& filepath,
                uint32_t width, uint32_t height, const unsigned char* image,
                PNGFileColorType colorType = PNGFILE_COLOR_RGBA);


        private:
            ////////////////////////////////////////////////////////////////////
            //  PNGFile private copy constructor : Not copyable               //
            ////////////////////////////////////////////////////////////////////
            PNGFile(const PNGFile&) = delete;

            ////////////////////////////////////////////////////////////////////
            //  PNGFile private copy operator : Not copyable                  //
            ////////////////////////////////////////////////////////////////////
            PNGFile& operator=(const PNGFile&) = delete;


            ////////////////////////////////////////////////////////////////////
            //  Load PNG file image data                                      //
            //  return : True if PNG file image data is successfully loaded   //
            ////////////////////////////////////////////////////////////////////
            bool loadPNGData(std::ifstream& pngFile,
                PNGFileIHDRChunk& pngIHDRChunk);

            ////////////////////////////////////////////////////////////////////
            //  Save PNG file image data                                      //
            //  return : True if PNG file image data is successfully saved    //
            ////////////////////////////////////////////////////////////////////
            static bool savePNGData(std::ofstream& pngFile,
                PNGFileIHDRChunk& pngIHDRChunk, const unsigned char* image);

            ////////////////////////////////////////////////////////////////////
            //  Decode PNG 32 bits data                                       //
            //  return : True if PNG 32 bits data are successfully decoded    //
            ////////////////////////////////////////////////////////////////////
            bool decodePNG32bits(unsigned char* data,
                uint32_t width, uint32_t height);

            ////////////////////////////////////////////////////////////////////
            //  Encode PNG 32 bits data                                       //
            //  return : True if PNG 32 bits data are successfully encoded    //
            ////////////////////////////////////////////////////////////////////
            static bool encodePNG32bits(unsigned char* data,
                uint32_t width, uint32_t height, const unsigned char* image);

            ////////////////////////////////////////////////////////////////////
            //  Decode PNG 24 bits data                                       //
            //  return : True if PNG 24 bits data are successfully decoded    //
            ////////////////////////////////////////////////////////////////////
            bool decodePNG24bits(unsigned char* data,
                uint32_t width, uint32_t height);

            ////////////////////////////////////////////////////////////////////
            //  Encode PNG 24 bits data                                       //
            //  return : True if PNG 24 bits data are successfully encoded    //
            ////////////////////////////////////////////////////////////////////
            static bool encodePNG24bits(unsigned char* data,
                uint32_t width, uint32_t height, const unsigned char* image);

            ////////////////////////////////////////////////////////////////////
            //  Decode PNG 16 bits data                                       //
            //  return : True if PNG 16 bits data are successfully decoded    //
            ////////////////////////////////////////////////////////////////////
            bool decodePNG16bits(unsigned char* data,
                uint32_t width, uint32_t height);

            ////////////////////////////////////////////////////////////////////
            //  Encode PNG 16 bits data                                       //
            //  return : True if PNG 16 bits data are successfully encoded    //
            ////////////////////////////////////////////////////////////////////
            static bool encodePNG16bits(unsigned char* data,
                uint32_t width, uint32_t height, const unsigned char* image);

            ////////////////////////////////////////////////////////////////////
            //  Decode PNG 8 bits data                                        //
            //  return : True if PNG 8 bits data are successfully decoded     //
            ////////////////////////////////////////////////////////////////////
            bool decodePNG8bits(unsigned char* data,
                uint32_t width, uint32_t height);

            ////////////////////////////////////////////////////////////////////
            //  Encode PNG 8 bits data                                        //
            //  return : True if PNG 8 bits data are successfully encoded     //
            ////////////////////////////////////////////////////////////////////
            static bool encodePNG8bits(unsigned char* data,
                uint32_t width, uint32_t height, const unsigned char* image);


        private:
            bool                m_loaded;       // Image loaded state
            unsigned char*      m_image;        // Image data
            uint32_t            m_width;        // Image width
            uint32_t            m_height;       // Image height
    };


#endif // VTOOLS_EMBIMG_IMAGES_PNGFILE_HEADER
