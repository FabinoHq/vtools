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
//     Compress/ZLib.h : ZLib compression management                          //
////////////////////////////////////////////////////////////////////////////////
#ifndef VTOOLS_EMBIMG_COMPRESS_ZLIB_HEADER
#define VTOOLS_EMBIMG_COMPRESS_ZLIB_HEADER

    #include "../System/System.h"
    #include "../System/SysCPU.h"
    #include "../System/SysCRC.h"

    #include <cstddef>
    #include <cstdint>
    #include <cstring>


    ////////////////////////////////////////////////////////////////////////////
    //  ZLib Huffman deflate constants                                        //
    ////////////////////////////////////////////////////////////////////////////
    #define ZLIB_DEFLATE_PRECODE_SIZE 128
    #define ZLIB_DEFLATE_OFFSET_SIZE 402
    #define ZLIB_DEFLATE_LITLEN_SIZE 1334
    #define ZLIB_DEFLATE_PRECODE_SYMBOLS 19
    #define ZLIB_DEFLATE_OFFSET_SYMBOLS 32
    #define ZLIB_DEFLATE_LITLEN_SYMBOLS 288
    #define ZLIB_DEFLATE_LENGTHS_SYMBOLS 457
    #define ZLIB_DEFLATE_MAX_SYMBOLS 288
    #define ZLIB_DEFLATE_MAX_CODEWORD_LEN 15
    #define ZLIB_DEFLATE_MAX_PRECODE_LEN 7
    #define ZLIB_DEFLATE_MAX_OFFSET_LEN 15
    #define ZLIB_DEFLATE_MAX_LITLEN_LEN 15
    #define ZLIB_DEFLATE_PRECODE_TABLEBITS 7
    #define ZLIB_DEFLATE_OFFSET_TABLEBITS 8
    #define ZLIB_DEFLATE_LITLEN_TABLEBITS 10

    ////////////////////////////////////////////////////////////////////////////
    //  ZLib deflate compression level hint enumeration                       //
    ////////////////////////////////////////////////////////////////////////////
    enum ZLibCompressionLevelHint
    {
        ZLIB_DEFLATE_FASTEST_COMPRESSION = 0,
        ZLIB_DEFLATE_FAST_COMPRESSION = 1,
        ZLIB_DEFLATE_DEFAULT_COMPRESSION = 2,
        ZLIB_DEFLATE_SLOWEST_COMPRESSION = 3
    };

    ////////////////////////////////////////////////////////////////////////////
    //  ZLib deflate block types enumeration                                  //
    ////////////////////////////////////////////////////////////////////////////
    enum ZLibDeflateBlockType
    {
        ZLIB_DEFLATE_BLOCK_UNCOMPRESSED = 0,
        ZLIB_DEFLATE_BLOCK_STATIC_HUFFMAN = 1,
        ZLIB_DEFLATE_BLOCK_DYNAMIC_HUFFMAN = 2
    };

    ////////////////////////////////////////////////////////////////////////////
    //  ZLib Huffman precode permutations                                     //
    ////////////////////////////////////////////////////////////////////////////
    const uint8_t ZLibDeflatePrecodePermutations[ZLIB_DEFLATE_PRECODE_SYMBOLS] =
    {
        0x10, 0x11, 0x12, 0x00, 0x08, 0x07, 0x09, 0x06, 0x0A,
        0x05, 0x0B, 0x04, 0x0C, 0x03, 0x0D, 0x02, 0x0E, 0x01,
        0x0F
    };

    ////////////////////////////////////////////////////////////////////////////
    //  ZLib Huffman decode table                                             //
    ////////////////////////////////////////////////////////////////////////////
    const uint32_t ZLibDeflateDecodeTable[ZLIB_DEFLATE_PRECODE_SYMBOLS] =
    {
        0x00000000u, 0x00000100u, 0x00000200u, 0x00000300u,
        0x00000400u, 0x00000500u, 0x00000600u, 0x00000700u,
        0x00000800u, 0x00000900u, 0x00000A00u, 0x00000B00u,
        0x00000C00u, 0x00000D00u, 0x00000E00u, 0x00000F00u,
        0x00001000u, 0x00001100u, 0x00001200u
    };

    ////////////////////////////////////////////////////////////////////////////
    //  ZLib Huffman offset table                                             //
    ////////////////////////////////////////////////////////////////////////////
    const uint32_t ZLibDeflateOffsetTable[ZLIB_DEFLATE_OFFSET_SYMBOLS] =
    {
        0x00000100u, 0x00000200u, 0x00000300u, 0x00000400u,
        0x01000500u, 0x01000700u, 0x02000900u, 0x02000D00u,
        0x03001100u, 0x03001900u, 0x04002100u, 0x04003100u,
        0x05004100u, 0x05006100u, 0x06008100u, 0x0600C100u,
        0x07010100u, 0x07018100u, 0x08020100u, 0x08030100u,
        0x09040100u, 0x09060100u, 0x0A080100u, 0x0A0C0100u,
        0x0B100100u, 0x0B180100u, 0x0C200100u, 0x0C300100u,
        0x0D400100u, 0x0D600100u, 0x0E800100u, 0x0EC00100u
    };

    ////////////////////////////////////////////////////////////////////////////
    //  ZLib Huffman litlen table                                             //
    ////////////////////////////////////////////////////////////////////////////
    const uint32_t ZLibDeflateLitlenTable[ZLIB_DEFLATE_LITLEN_SYMBOLS] =
    {
        0x40000000u, 0x40000100u, 0x40000200u, 0x40000300u,
        0x40000400u, 0x40000500u, 0x40000600u, 0x40000700u,
        0x40000800u, 0x40000900u, 0x40000A00u, 0x40000B00u,
        0x40000C00u, 0x40000D00u, 0x40000E00u, 0x40000F00u,
        0x40001000u, 0x40001100u, 0x40001200u, 0x40001300u,
        0x40001400u, 0x40001500u, 0x40001600u, 0x40001700u,
        0x40001800u, 0x40001900u, 0x40001A00u, 0x40001B00u,
        0x40001C00u, 0x40001D00u, 0x40001E00u, 0x40001F00u,
        0x40002000u, 0x40002100u, 0x40002200u, 0x40002300u,
        0x40002400u, 0x40002500u, 0x40002600u, 0x40002700u,
        0x40002800u, 0x40002900u, 0x40002A00u, 0x40002B00u,
        0x40002C00u, 0x40002D00u, 0x40002E00u, 0x40002F00u,
        0x40003000u, 0x40003100u, 0x40003200u, 0x40003300u,
        0x40003400u, 0x40003500u, 0x40003600u, 0x40003700u,
        0x40003800u, 0x40003900u, 0x40003A00u, 0x40003B00u,
        0x40003C00u, 0x40003D00u, 0x40003E00u, 0x40003F00u,
        0x40004000u, 0x40004100u, 0x40004200u, 0x40004300u,
        0x40004400u, 0x40004500u, 0x40004600u, 0x40004700u,
        0x40004800u, 0x40004900u, 0x40004A00u, 0x40004B00u,
        0x40004C00u, 0x40004D00u, 0x40004E00u, 0x40004F00u,
        0x40005000u, 0x40005100u, 0x40005200u, 0x40005300u,
        0x40005400u, 0x40005500u, 0x40005600u, 0x40005700u,
        0x40005800u, 0x40005900u, 0x40005A00u, 0x40005B00u,
        0x40005C00u, 0x40005D00u, 0x40005E00u, 0x40005F00u,
        0x40006000u, 0x40006100u, 0x40006200u, 0x40006300u,
        0x40006400u, 0x40006500u, 0x40006600u, 0x40006700u,
        0x40006800u, 0x40006900u, 0x40006A00u, 0x40006B00u,
        0x40006C00u, 0x40006D00u, 0x40006E00u, 0x40006F00u,
        0x40007000u, 0x40007100u, 0x40007200u, 0x40007300u,
        0x40007400u, 0x40007500u, 0x40007600u, 0x40007700u,
        0x40007800u, 0x40007900u, 0x40007A00u, 0x40007B00u,
        0x40007C00u, 0x40007D00u, 0x40007E00u, 0x40007F00u,
        0x40008000u, 0x40008100u, 0x40008200u, 0x40008300u,
        0x40008400u, 0x40008500u, 0x40008600u, 0x40008700u,
        0x40008800u, 0x40008900u, 0x40008A00u, 0x40008B00u,
        0x40008C00u, 0x40008D00u, 0x40008E00u, 0x40008F00u,
        0x40009000u, 0x40009100u, 0x40009200u, 0x40009300u,
        0x40009400u, 0x40009500u, 0x40009600u, 0x40009700u,
        0x40009800u, 0x40009900u, 0x40009A00u, 0x40009B00u,
        0x40009C00u, 0x40009D00u, 0x40009E00u, 0x40009F00u,
        0x4000A000u, 0x4000A100u, 0x4000A200u, 0x4000A300u,
        0x4000A400u, 0x4000A500u, 0x4000A600u, 0x4000A700u,
        0x4000A800u, 0x4000A900u, 0x4000AA00u, 0x4000AB00u,
        0x4000AC00u, 0x4000AD00u, 0x4000AE00u, 0x4000AF00u,
        0x4000B000u, 0x4000B100u, 0x4000B200u, 0x4000B300u,
        0x4000B400u, 0x4000B500u, 0x4000B600u, 0x4000B700u,
        0x4000B800u, 0x4000B900u, 0x4000BA00u, 0x4000BB00u,
        0x4000BC00u, 0x4000BD00u, 0x4000BE00u, 0x4000BF00u,
        0x4000C000u, 0x4000C100u, 0x4000C200u, 0x4000C300u,
        0x4000C400u, 0x4000C500u, 0x4000C600u, 0x4000C700u,
        0x4000C800u, 0x4000C900u, 0x4000CA00u, 0x4000CB00u,
        0x4000CC00u, 0x4000CD00u, 0x4000CE00u, 0x4000CF00u,
        0x4000D000u, 0x4000D100u, 0x4000D200u, 0x4000D300u,
        0x4000D400u, 0x4000D500u, 0x4000D600u, 0x4000D700u,
        0x4000D800u, 0x4000D900u, 0x4000DA00u, 0x4000DB00u,
        0x4000DC00u, 0x4000DD00u, 0x4000DE00u, 0x4000DF00u,
        0x4000E000u, 0x4000E100u, 0x4000E200u, 0x4000E300u,
        0x4000E400u, 0x4000E500u, 0x4000E600u, 0x4000E700u,
        0x4000E800u, 0x4000E900u, 0x4000EA00u, 0x4000EB00u,
        0x4000EC00u, 0x4000ED00u, 0x4000EE00u, 0x4000EF00u,
        0x4000F000u, 0x4000F100u, 0x4000F200u, 0x4000F300u,
        0x4000F400u, 0x4000F500u, 0x4000F600u, 0x4000F700u,
        0x4000F800u, 0x4000F900u, 0x4000FA00u, 0x4000FB00u,
        0x4000FC00u, 0x4000FD00u, 0x4000FE00u, 0x4000FF00u,
        0x00000000u, 0x00030000u, 0x00040000u, 0x00050000u,
        0x00060000u, 0x00070000u, 0x00080000u, 0x00090000u,
        0x000A0000u, 0x000B0100u, 0x000D0100u, 0x000F0100u,
        0x00110100u, 0x00130200u, 0x00170200u, 0x001B0200u,
        0x001F0200u, 0x00230300u, 0x002B0300u, 0x00330300u,
        0x003B0300u, 0x00430400u, 0x00530400u, 0x00630400u,
        0x00730400u, 0x00830500u, 0x00A30500u, 0x00C30500u,
        0x00E30500u, 0x01020000u, 0x01020000u, 0x01020000u
    };

    ////////////////////////////////////////////////////////////////////////////
    //  ZLib deflate decode tables                                            //
    ////////////////////////////////////////////////////////////////////////////
    struct ZLibDeflateDecodeTables
    {
        union
        {
            uint8_t precode[ZLIB_DEFLATE_PRECODE_SYMBOLS];
            struct
            {
                uint8_t lengths[ZLIB_DEFLATE_LENGTHS_SYMBOLS];
                uint32_t decode[ZLIB_DEFLATE_PRECODE_SIZE];
            } l;
            uint32_t litlen[ZLIB_DEFLATE_LITLEN_SIZE];
        } u;
        uint32_t offset[ZLIB_DEFLATE_OFFSET_SIZE];
        uint16_t sorted[ZLIB_DEFLATE_MAX_SYMBOLS];
    };

    ////////////////////////////////////////////////////////////////////////////
    //  Compute compressed ZLib deflate data size                             //
    //  return : Computed compressed ZLib deflate data size                   //
    ////////////////////////////////////////////////////////////////////////////
    size_t ZLibComputeDeflateCompressSize(size_t inSize);

    ////////////////////////////////////////////////////////////////////////////
    //  Compress ZLib deflate data                                            //
    //  return : True if the data is successfully compressed                  //
    ////////////////////////////////////////////////////////////////////////////
    bool ZLibDeflateCompress(unsigned char in[], size_t inSize,
        unsigned char out[], size_t* outSize);

    ////////////////////////////////////////////////////////////////////////////
    //  Build ZLib Huffman decode table                                       //
    //  return : True if the Huffman decode table is successfully created     //
    ////////////////////////////////////////////////////////////////////////////
    bool ZLibBuildDecodeTable(uint32_t decodeTable[], uint8_t lengthsTable[],
        uint16_t sortedSymbols[], const uint32_t decodeResults[],
        const uint32_t symbolsCount, const uint32_t codewordLen,
        const uint32_t tableBits);

    ////////////////////////////////////////////////////////////////////////////
    //  Decompress ZLib deflate data                                          //
    //  return : True if the data is successfully decompressed                //
    ////////////////////////////////////////////////////////////////////////////
    bool ZLibDeflateDecompress(unsigned char in[], size_t inSize,
        unsigned char out[], size_t* outSize);


#endif // VTOOLS_EMBIMG_COMPRESS_ZLIB_HEADER
