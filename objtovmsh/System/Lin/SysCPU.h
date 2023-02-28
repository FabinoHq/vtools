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
//     System/Lin/SysCPU.h : System CPU management for Linux                  //
////////////////////////////////////////////////////////////////////////////////
#ifndef VTOOLS_OBJTOVMSH_SYSTEM_LIN_SYSCPU_HEADER
#define VTOOLS_OBJTOVMSH_SYSTEM_LIN_SYSCPU_HEADER

    #include "../System.h"

    #include <cstddef>
    #include <cstdint>


    ////////////////////////////////////////////////////////////////////////////
    //  Swap 2 bytes unsigned integer endianness                              //
    //  return : Swapped 2 bytes unsigned integer                             //
    ////////////////////////////////////////////////////////////////////////////
    #define SysByteSwap16 __builtin_bswap16

    ////////////////////////////////////////////////////////////////////////////
    //  Swap 4 bytes unsigned integer endianness                              //
    //  return : Swapped 4 bytes unsigned integer                             //
    ////////////////////////////////////////////////////////////////////////////
    #define SysByteSwap32 __builtin_bswap32

    ////////////////////////////////////////////////////////////////////////////
    //  Swap 8 bytes unsigned integer endianness                              //
    //  return : Swapped 8 bytes unsigned integer                             //
    ////////////////////////////////////////////////////////////////////////////
    #define SysByteSwap64 __builtin_bswap64


    ////////////////////////////////////////////////////////////////////////////
    //  Compute 32 bits scan forward                                          //
    //  return : Computed 32 bits scan forward                                //
    ////////////////////////////////////////////////////////////////////////////
    #define SysBitScanForward32(bits) (__builtin_ctz(bits))

    ////////////////////////////////////////////////////////////////////////////
    //  Compute 32 bits scan reverse                                          //
    //  return : Computed 32 bits scan reverse                                //
    ////////////////////////////////////////////////////////////////////////////
    #define SysBitScanReverse32(bits) (0x1F - __builtin_clz(bits))

    ////////////////////////////////////////////////////////////////////////////
    //  Compute 64 bits scan forward                                          //
    //  return : Computed 64 bits scan forward                                //
    ////////////////////////////////////////////////////////////////////////////
    #define SysBitScanForward64(bits) (__builtin_ctzll(bits))

    ////////////////////////////////////////////////////////////////////////////
    //  Compute 64 bits scan reverse                                          //
    //  return : Computed 64 bits scan reverse                                //
    ////////////////////////////////////////////////////////////////////////////
    #define SysBitScanReverse64(bits) (0x3F - __builtin_clzll(bits))


#endif // VTOOLS_OBJTOVMSH_SYSTEM_LIN_SYSCPU_HEADER
