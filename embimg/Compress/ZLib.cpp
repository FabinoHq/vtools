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
//     Compress/ZLib.cpp : ZLib compression management                        //
////////////////////////////////////////////////////////////////////////////////
#include "ZLib.h"


////////////////////////////////////////////////////////////////////////////////
//  Compute compressed ZLib deflate data size                                 //
//  return : Computed compressed ZLib deflate data size                       //
////////////////////////////////////////////////////////////////////////////////
size_t ZLibComputeDeflateCompressSize(size_t inSize)
{
    bool computed = false;
    size_t outSize = 2;

    // Compute deflate data compressed size
    while (!computed)
    {
        // Check remaining size
        if (inSize <= 65535)
        {
            outSize += inSize;
            inSize = 0;
            computed = true;
        }
        else
        {
            outSize += 65535;
            inSize -= 65535;
        }

        // Block infos
        outSize += 1;
        // Block size
        outSize += 4;
    }

    // Adler32 CRC
    outSize += 4;

    // Compressed ZLib deflate data size successfully computed
    return outSize;
}

////////////////////////////////////////////////////////////////////////////////
//  Compress ZLib deflate data                                                //
//  return : True if the data is successfully compressed                      //
////////////////////////////////////////////////////////////////////////////////
bool ZLibDeflateCompress(unsigned char in[], size_t inSize,
    unsigned char out[], size_t* outSize)
{
    bool compressed = false;
    size_t inIndex = 0;
    size_t outIndex = 0;

    // Check data buffers
    if (!in || !out || (inSize <= 0) || (*outSize < 2))
    {
        // Invalid data buffers
        return false;
    }

    // Write ZLib header
    uint16_t zlibHeader = ((8 << 8) | (7 << 12));
    zlibHeader |= (ZLIB_DEFLATE_FASTEST_COMPRESSION << 6);
    zlibHeader |= (31 - (zlibHeader % 31));
    out[outIndex++] = ((zlibHeader >> 8) & 0xFF);
    out[outIndex++] = (zlibHeader & 0xFF);

    // Compress deflate data
    uint32_t bitsCount = 0;
    while (!compressed)
    {
        // Check remaining size
        uint16_t blockSize = 65535;
        if ((inSize-inIndex) <= 65535)
        {
            blockSize = (uint16_t)(inSize-inIndex);
            compressed = true;
        }

        // Check output buffer size
        if ((outIndex + 5) > *outSize) return false;

        // Write final block state
        out[outIndex] = 0;
        out[outIndex] |= ((compressed ? 1 : 0) << bitsCount);
        ++bitsCount;

        // Write block type
        out[outIndex] |= (ZLIB_DEFLATE_BLOCK_UNCOMPRESSED << bitsCount);
        ++bitsCount;

        // Align output
        bitsCount = 0;
        ++outIndex;

        // Write block size
        out[outIndex++] = (blockSize & 0xFF);
        out[outIndex++] = ((blockSize >> 8) & 0xFF);
        uint16_t nBlockSize = ((uint16_t)~blockSize);
        out[outIndex++] = (nBlockSize & 0xFF);
        out[outIndex++] = ((nBlockSize >> 8) & 0xFF);

        // Check buffers sizes
        if ((inIndex + blockSize) > inSize) return false;
        if ((outIndex + blockSize) > *outSize) return false;

        // Copy uncompressed block
        memcpy(&out[outIndex], &in[inIndex], blockSize);
        inIndex += blockSize;
        outIndex += blockSize;
    }

    // Write Adler32 CRC
    if ((outIndex + 4) > *outSize) return false;
    uint32_t zlibAdler32 = SysAdler32(in, inSize);
    out[outIndex++] = ((zlibAdler32 >> 24) & 0xFF);
    out[outIndex++] = ((zlibAdler32 >> 16) & 0xFF);
    out[outIndex++] = ((zlibAdler32 >> 8) & 0xFF);
    out[outIndex++] = (zlibAdler32 & 0xFF);

    // Set output data size
    if (outIndex > *outSize)
    {
        // Invalid output data size
        return false;
    }
    *outSize = outIndex;

    // Data is successfully compressed
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Build ZLib Huffman decode table                                           //
//  return : True if the Huffman decode table is successfully created         //
////////////////////////////////////////////////////////////////////////////////
bool ZLibBuildDecodeTable(uint32_t decodeTable[], uint8_t lengthsTable[],
    uint16_t sortedSymbols[], const uint32_t decodeResults[],
    const uint32_t symbolsCount, const uint32_t codewordLen,
    const uint32_t tableBits)
{
    uint32_t lenCounts[ZLIB_DEFLATE_MAX_CODEWORD_LEN+1] = {0};
    uint32_t offsets[ZLIB_DEFLATE_MAX_CODEWORD_LEN+1] = {0};
    uint32_t codespace = 0;

    // Count codewords for each length
    for (uint32_t i = 0; i < symbolsCount; ++i)
    {
        ++lenCounts[lengthsTable[i]];
    }

    // Compute offsets and codespace for sorting symbols
    offsets[0] = 0;
    offsets[1] = lenCounts[0];
    for (uint32_t i = 1; i < codewordLen; ++i)
    {
        offsets[i+1] = (offsets[i] + lenCounts[i]);
        codespace = ((codespace << 1) + lenCounts[i]);
    }
    codespace = ((codespace << 1) + lenCounts[codewordLen]);

    // Sort symbols
    for (uint16_t k = 0; k < symbolsCount; ++k)
    {
        sortedSymbols[offsets[lengthsTable[k]]++] = k;
    }
    sortedSymbols += offsets[0];

    // Check codespace
    if (codespace > (0x00000001u << codewordLen))
    {
        // Code overflows the codespace
        return false;
    }
    else if (codespace < (0x00000001u << codewordLen))
    {
        uint32_t entry = (decodeResults[0] | 1);
        if (codespace != 0)
        {
            // Incomplete code
            if ((codespace != (0x00000001u << (codewordLen-1))) ||
                (lenCounts[1] != 1))
            {
                // Invalid code
                return false;
            }

            // Single used symbol with codeword length 1
            entry = (decodeResults[*sortedSymbols] | 1);
        }

        // Compute decode table
        for (uint32_t i = 0; i < (0x00000001u << tableBits); ++i)
        {
            decodeTable[i] = entry;
        }

        // Huffman decode table successfully created
        return true;
    }

    // Complete codespace
    uint32_t codeword = 0;
    uint32_t i = 1;
    uint32_t count = 0;
    while ((count = lenCounts[i]) == 0) { ++i; }
    uint32_t tableEnd = (0x00000001u << i);
    while (i <= tableBits)
    {
        // Process codewords
        do
        {
            decodeTable[codeword] = (decodeResults[*sortedSymbols++] | i);
            if (codeword == (tableEnd-1))
            {
                // Last codeword
                for (; i < tableBits; ++i)
                {
                    memcpy(&decodeTable[tableEnd], decodeTable,
                        tableEnd*sizeof(uint32_t));
                    tableEnd <<= 1;
                }

                // Huffman decode table successfully created
                return true;
            }

            // Increment codeword
            uint32_t currentBit =
                (0x00000001u << SysBitScanReverse32(codeword ^ (tableEnd-1)));
            codeword &= (currentBit - 1);
            codeword |= currentBit;
        } while(--count);

        // Next codeword length
        do
        {
            if (++i <= tableBits)
            {
                memcpy(&decodeTable[tableEnd], decodeTable,
                    tableEnd*sizeof(uint32_t));
                tableEnd <<= 1;
            }
        } while ((count = lenCounts[i]) == 0);
    }

    // Process codewords with length greater than tableBits
    tableEnd = (0x00000001u << tableBits);
    uint32_t subtablePrefix = 0xFFFFFFFFu;
    uint32_t subtableStart = 0;
    for (;;)
    {
        if ((codeword & ((0x00000001u << tableBits)-1)) != subtablePrefix)
        {
            // Compute subtable length
            uint32_t subtableBits = (i - tableBits);
            subtablePrefix = (codeword & ((0x00000001u << tableBits) - 1));
            subtableStart = tableEnd;
            codespace = count;
            while (codespace < (0x00000001u << subtableBits))
            {
                ++subtableBits;
                codespace =
                    ((codespace << 1) + lenCounts[tableBits + subtableBits]);
            }
            tableEnd = (subtableStart + (0x00000001u << subtableBits));

            // Compute decode table from subtable
            decodeTable[subtablePrefix] =
                (0x80000000 | (subtableStart << 8) | subtableBits);
        }

        // Fill subtable for the current codeword
        uint32_t entry = (decodeResults[*sortedSymbols++] | (i - tableBits));
        uint32_t curTable = subtableStart + (codeword >> tableBits);
        uint32_t stride = (0x00000001u << (i - tableBits));
        do
        {
            decodeTable[curTable] = entry;
            curTable += stride;
        } while (curTable < tableEnd);

        // Check for last codeword
        if (codeword == ((0x00000001u << i) - 1))
        {
            // Huffman decode table successfully created
            return true;
        }

        // Increment codeword
        uint32_t currentBit = (0x00000001u << SysBitScanReverse32(
            codeword ^ ((0x00000001u << i) - 1))
        );
        codeword &= (currentBit - 1);
        codeword |= currentBit;
        --count;
        while (count == 0)
        {
            count = lenCounts[++i];
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//  Decompress ZLib deflate data                                              //
//  return : True if the data is successfully decompressed                    //
////////////////////////////////////////////////////////////////////////////////
bool ZLibDeflateDecompress(unsigned char in[], size_t inSize,
    unsigned char out[], size_t* outSize)
{
    bool decompressed = false;
    bool staticTablesLoaded = false;
    size_t inIndex = 0;
    size_t outIndex = 0;
    size_t endIndex = (inSize-4);
    uint32_t bitsLeft = 0;
    uint64_t current = 0;
    size_t overrun = 0;
    ZLibDeflateDecodeTables tables;

    // Check data buffers
    if (!in || !out || (inSize < 4) || (*outSize <= 0))
    {
        // Invalid data buffers
        return false;
    }

    // Check ZLib header
    uint16_t zlibHeader = (in[inIndex++] << 8);
    zlibHeader |= in[inIndex++];
    // FCHECK
    if ((zlibHeader % 31) != 0) return false;
    // CM
    if (((zlibHeader >> 8) & 0x000F) != 0x0008) return false;
    // CINFO
    if ((zlibHeader >> 12) > 0x0007) return false;
    // FDICT     
    if ((zlibHeader >> 5) & 0x0001) return false;

    // Decompress deflate data
    while (!decompressed)
    {
        // Read next 17 bits
        if (bitsLeft < 17)
        {
            do
            {
                if (inIndex < endIndex)
                {
                    current |= (uint64_t)in[inIndex++] << bitsLeft;
                }
                else
                {
                    ++overrun;
                }
                bitsLeft += 8;
            } while(bitsLeft <= 55);
        }

        // Read final block state
        decompressed = (current & 0x01);
        current >>= 1;
        bitsLeft -= 1;

        // Read block type
        unsigned char blockType = (current & 0x03);
        current >>= 2;
        bitsLeft -= 2;

        // Uncompressed block type
        if (blockType == ZLIB_DEFLATE_BLOCK_UNCOMPRESSED)
        {
            // Align input
            inIndex -= (bitsLeft >> 3) - overrun;
            overrun = 0;
            current = 0;
            bitsLeft = 0;

            // Check input buffer size
            if ((inIndex + 4) > endIndex) return false;

            // Read block size
            uint16_t blockSize = in[inIndex++];
            blockSize |= (in[inIndex++] << 8);
            uint16_t nBlockSize = in[inIndex++];
            nBlockSize |= (in[inIndex++] << 8);

            // Check block size
            if (blockSize != ((uint16_t)~nBlockSize)) return false;

            // Check buffers sizes
            if ((inIndex + blockSize) > endIndex) return false;
            if ((outIndex + blockSize) > *outSize) return false;

            // Copy uncompressed block
            memcpy(&out[outIndex], &in[inIndex], blockSize);
            inIndex += blockSize;
            outIndex += blockSize;
        }
        else
        {
            uint32_t litlenSymbols = 0;
            uint32_t offsetSymbols = 0;
            if (blockType == ZLIB_DEFLATE_BLOCK_STATIC_HUFFMAN)
            {
                // Static huffman block type
                if (!staticTablesLoaded)
                {
                    // Build static tables
                    uint32_t i = 0;
                    for (i = 0; i < 144; ++i)
                    {
                        tables.u.l.lengths[i] = 8;
                    }
                    for (; i < 256; ++i)
                    {
                        tables.u.l.lengths[i] = 9;
                    }
                    for (; i < 280; ++i)
                    {
                        tables.u.l.lengths[i] = 7;
                    }
                    for (; i < 288; ++i)
                    {
                        tables.u.l.lengths[i] = 8;
                    }
                    for (; i < 320; ++i)
                    {
                        tables.u.l.lengths[i] = 5;
                    }
                    litlenSymbols = ZLIB_DEFLATE_LITLEN_SYMBOLS;
                    offsetSymbols = ZLIB_DEFLATE_OFFSET_SYMBOLS;
                }
            }
            else if (blockType == ZLIB_DEFLATE_BLOCK_DYNAMIC_HUFFMAN)
            {
                // Dynamic huffman block type
                staticTablesLoaded = false;

                // Read codeword length counts
                litlenSymbols = ((current & 0x1F) + 257);
                current >>= 5;
                bitsLeft -= 5;

                offsetSymbols = ((current & 0x1F) + 1);
                current >>= 5;
                bitsLeft -= 5;

                uint32_t precodeSymbols = ((current & 0x0F) + 4);
                current >>= 4;
                bitsLeft -= 4;

                // Compute precode symbols
                uint32_t i = 0;
                for (i = 0; i < precodeSymbols; ++i)
                {
                    // Read next 3 bits
                    if (bitsLeft < 3)
                    {
                        do
                        {
                            if (inIndex < endIndex)
                            {
                                current |= (uint64_t)in[inIndex++] << bitsLeft;
                            }
                            else
                            {
                                ++overrun;
                            }
                            bitsLeft += 8;
                        } while(bitsLeft <= 55);
                    }

                    // Read precode table entry
                    uint8_t precode = (current & 0x07);
                    current >>= 3;
                    bitsLeft -= 3;
                    tables.u.precode[ZLibDeflatePrecodePermutations[i]] =
                        precode;
                }
                for (; i < ZLIB_DEFLATE_PRECODE_SYMBOLS; ++i)
                {
                    // Fill unused precode symbols with zeros
                    tables.u.precode[ZLibDeflatePrecodePermutations[i]] = 0;
                }

                // Build decode table
                if (!ZLibBuildDecodeTable(
                    tables.u.l.decode, tables.u.precode,
                    tables.sorted, ZLibDeflateDecodeTable,
                    ZLIB_DEFLATE_PRECODE_SYMBOLS, ZLIB_DEFLATE_MAX_PRECODE_LEN,
                    ZLIB_DEFLATE_PRECODE_TABLEBITS))
                {
                    // Could not build decode table
                    return false;
                }

                // Expand litlen and offset codeword lengths
                for (i = 0; i < (litlenSymbols+offsetSymbols); )
                {
                    // Read next 14 bits
                    if (bitsLeft < 14)
                    {
                        do
                        {
                            if (inIndex < endIndex)
                            {
                                current |= (uint64_t)in[inIndex++] << bitsLeft;
                            }
                            else
                            {
                                ++overrun;
                            }
                            bitsLeft += 8;
                        } while(bitsLeft <= 55);
                    }

                    // Read next precode symbol
                    uint32_t entry = tables.u.l.decode[((uint32_t)current &
                        ((0x00000001u << ZLIB_DEFLATE_MAX_PRECODE_LEN) - 1))];
                    current >>= (entry & 0xFF);
                    bitsLeft -= (entry & 0xFF);
                    uint32_t presym = (entry >> 8);

                    // Handle explicit codeword lengths
                    if (presym < 16)
                    {
                        tables.u.l.lengths[i++] = (uint8_t)presym;
                        continue;
                    }

                    if (presym == 16)
                    {
                        // Repeat previous length 6 times
                        if (i == 0) { return false; }
                        uint8_t repVal = tables.u.l.lengths[i-1];
                        uint32_t repCount = ((current & 0x03) + 3);
                        current >>= 2;
                        bitsLeft -= 2;
                        tables.u.l.lengths[i+0] = repVal;
                        tables.u.l.lengths[i+1] = repVal;
                        tables.u.l.lengths[i+2] = repVal;
                        tables.u.l.lengths[i+3] = repVal;
                        tables.u.l.lengths[i+4] = repVal;
                        tables.u.l.lengths[i+5] = repVal;
                        i += repCount;
                    }
                    else if (presym == 17)
                    {
                        // Repeat zero 10 times
                        uint32_t repCount = ((current & 0x07) + 3);
                        current >>= 3;
                        bitsLeft -= 3;
                        tables.u.l.lengths[i+0] = 0;
                        tables.u.l.lengths[i+1] = 0;
                        tables.u.l.lengths[i+2] = 0;
                        tables.u.l.lengths[i+3] = 0;
                        tables.u.l.lengths[i+4] = 0;
                        tables.u.l.lengths[i+5] = 0;
                        tables.u.l.lengths[i+6] = 0;
                        tables.u.l.lengths[i+7] = 0;
                        tables.u.l.lengths[i+8] = 0;
                        tables.u.l.lengths[i+9] = 0;
                        i += repCount;
                    }
                    else
                    {
                        // Repeat zero 138 times
                        uint32_t repCount = ((current & 0x7F) + 11);
                        current >>= 7;
                        bitsLeft -= 7;
                        memset(
                            &tables.u.l.lengths[i], 0, sizeof(uint8_t)*repCount
                        );
                        i += repCount;
                    }
                }
            }
            else
            {
                // Invalid block type
                return false;
            }

            if (!staticTablesLoaded)
            {
                // Build offset table
                if (!ZLibBuildDecodeTable(
                    tables.offset, tables.u.l.lengths+litlenSymbols,
                    tables.sorted, ZLibDeflateOffsetTable, offsetSymbols,
                    ZLIB_DEFLATE_MAX_OFFSET_LEN, ZLIB_DEFLATE_OFFSET_TABLEBITS))
                {
                    // Could not build offset table
                    return false;
                }

                // Build litlen table
                if (!ZLibBuildDecodeTable(
                    tables.u.litlen, tables.u.l.lengths,
                    tables.sorted, ZLibDeflateLitlenTable, litlenSymbols,
                    ZLIB_DEFLATE_MAX_LITLEN_LEN, ZLIB_DEFLATE_LITLEN_TABLEBITS))
                {
                    // Could not build litlen table
                    return false;
                }

                // Static decode tables are loaded
                if (blockType == ZLIB_DEFLATE_BLOCK_STATIC_HUFFMAN)
                {
                    staticTablesLoaded = true;
                }
            }

            // Decompress Huffman block
            for (;;)
            {
                // Read next 15 bits
                if (bitsLeft < 15)
                {
                    do
                    {
                        if (inIndex < endIndex)
                        {
                            current |= (uint64_t)in[inIndex++] << bitsLeft;
                        }
                        else
                        {
                            ++overrun;
                        }
                        bitsLeft += 8;
                    } while(bitsLeft <= 55);
                }

                // Decode litlen symbol
                uint32_t entry = tables.u.litlen[((uint32_t)current &
                    ((0x00000001u << ZLIB_DEFLATE_LITLEN_TABLEBITS) - 1))];
                if (entry & 0x80000000)
                {
                    // Litlen subtable required
                    current >>= ZLIB_DEFLATE_LITLEN_TABLEBITS;
                    bitsLeft -= ZLIB_DEFLATE_LITLEN_TABLEBITS;
                    entry = tables.u.litlen[
                        ((entry >> 8) & 0xFFFF) + ((uint32_t)current &
                        ((0x00000001u << (entry & 0xFF)) - 1))
                    ];
                }
                current >>= (entry & 0xFF);
                bitsLeft -= (entry & 0xFF);
                if (entry & 0x40000000)
                {
                    // Literal
                    if (outIndex >= *outSize) { return false; }
                    out[outIndex++] = (uint8_t)(entry >> 8);
                    continue;
                }
                entry >>= 8;

                // Read next 56 bits
                if (bitsLeft < 56)
                {
                    do
                    {
                        if (inIndex < endIndex)
                        {
                            current |= (uint64_t)in[inIndex++] << bitsLeft;
                        }
                        else
                        {
                            ++overrun;
                        }
                        bitsLeft += 8;
                    } while(bitsLeft <= 55);
                }

                // Compute full length
                uint32_t len =
                    ((uint32_t)current & ((0x00000001u << (entry & 0xFF))-1));
                current >>= (entry & 0xFF);
                bitsLeft -= (entry & 0xFF);
                len += (entry >> 8);

                // Block done
                if ((len-1) >= (*outSize - outIndex))
                {
                    if (len != 0) { return false; }
                    break;
                }

                // Decode offset symbol
                entry = tables.offset[((uint32_t)current &
                    ((0x00000001u << ZLIB_DEFLATE_OFFSET_TABLEBITS) - 1))];
                if (entry & 0x80000000)
                {
                    // Offset subtable required
                    current >>= ZLIB_DEFLATE_OFFSET_TABLEBITS;
                    bitsLeft -= ZLIB_DEFLATE_OFFSET_TABLEBITS;
                    entry = tables.offset[
                        ((entry >> 8) & 0xFFFF) + ((uint32_t)current &
                        ((0x00000001u << (entry & 0xFF)) - 1))
                    ];
                }
                current >>= (entry & 0xFF);
                bitsLeft -= (entry & 0xFF);
                entry >>= 8;

                // Read next 14 bits
                if (bitsLeft < 14)
                {
                    do
                    {
                        if (inIndex < endIndex)
                        {
                            current |= (uint64_t)in[inIndex++] << bitsLeft;
                        }
                        else
                        {
                            ++overrun;
                        }
                        bitsLeft += 8;
                    } while(bitsLeft <= 55);
                }

                // Compute extra offset
                uint32_t offset =
                    ((uint32_t)current & ((0x00000001u << (entry>>16))-1));
                current >>= (entry >> 16);
                bitsLeft -= (entry >> 16);
                offset += (entry & 0xFFFF);
                if (offset > outIndex) { return false; }

                // Copy to output buffer
                const uint8_t* src = &out[outIndex-offset];
                uint8_t* dst = &out[outIndex];
                outIndex += len;
                *dst++ = *src++;
                *dst++ = *src++;
                do
                {
                    *dst++ = *src++;
                } while(dst < &out[outIndex]);
            }
        }
    }

    // Align input
    inIndex -= ((bitsLeft >> 3) - overrun);

    // Read Adler32 CRC
    if ((inIndex + 4) > inSize) { return false; }
    uint32_t zlibAdler32 = (in[inIndex++] << 24);
    zlibAdler32 |= (in[inIndex++] << 16);
    zlibAdler32 |= (in[inIndex++] << 8);
    zlibAdler32 |= in[inIndex++];

    // Check Adler32 CRC
    if (zlibAdler32 != SysAdler32(out, *outSize))
    {
        // Invalid Adler32 CRC
        return false;
    }

    // Set output data size
    if (outIndex > *outSize)
    {
        // Invalid output data size
        return false;
    }
    *outSize = outIndex;

    // Data is successfully decompressed
    return true;
}
