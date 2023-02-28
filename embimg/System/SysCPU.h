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
//     System/SysCPU.h : System CPU management                                //
////////////////////////////////////////////////////////////////////////////////
#ifndef VTOOLS_EMBIMG_SYSTEM_SYSCPU_HEADER
#define VTOOLS_EMBIMG_SYSTEM_SYSCPU_HEADER

    #include "System.h"
    #include "../Images/PNGFile.h"

    #include <cstddef>
    #include <cstdint>
    #include <limits>


    ////////////////////////////////////////////////////////////////////////////
    //  Platform dependent CPU management                                     //
    ////////////////////////////////////////////////////////////////////////////
    #ifdef VTOOLS_EMBIMG_WINDOWS
        #include "Win/SysCPU.h"
    #endif // VTOOLS_EMBIMG_WINDOWS

    #ifdef VTOOLS_EMBIMG_LINUX
        #include "Lin/SysCPU.h"
    #endif // VTOOLS_EMBIMG_LINUX


    ////////////////////////////////////////////////////////////////////////////
    //  8 bits integer limits                                                 //
    ////////////////////////////////////////////////////////////////////////////
    const int64_t SysInt8MinLimit = -128;
    const int64_t SysInt8MaxLimit = 127;

    ////////////////////////////////////////////////////////////////////////////
    //  16 bits integer limits                                                //
    ////////////////////////////////////////////////////////////////////////////
    const int64_t SysInt16MinLimit = -32768;
    const int64_t SysInt16MaxLimit = 32767;

    ////////////////////////////////////////////////////////////////////////////
    //  32 bits integer limits                                                //
    ////////////////////////////////////////////////////////////////////////////
    const int64_t SysInt32MinLimit = -2147483648;
    const int64_t SysInt32MaxLimit = 2147483647;


    ////////////////////////////////////////////////////////////////////////////
    //  Check system CPU                                                      //
    //  return : True if the CPU is ready, false otherwise                    //
    ////////////////////////////////////////////////////////////////////////////
    bool SysCPUCheck();

    ////////////////////////////////////////////////////////////////////////////
    //  Check system CPU char representation                                  //
    //  return : True if the system CPU char representation is correct        //
    ////////////////////////////////////////////////////////////////////////////
    bool SysCPUCheckChar();

    ////////////////////////////////////////////////////////////////////////////
    //  Check system CPU bool representation                                  //
    //  return : True if the system CPU bool representation is correct        //
    ////////////////////////////////////////////////////////////////////////////
    bool SysCPUCheckBool();

    ////////////////////////////////////////////////////////////////////////////
    //  Check system CPU int representation                                   //
    //  return : True if the system CPU int representation is correct         //
    ////////////////////////////////////////////////////////////////////////////
    bool SysCPUCheckInt();

    ////////////////////////////////////////////////////////////////////////////
    //  Check system CPU float representation                                 //
    //  return : True if the system CPU float representation is correct       //
    ////////////////////////////////////////////////////////////////////////////
    bool SysCPUCheckFloat();

    ////////////////////////////////////////////////////////////////////////////
    //  Check system CPU double representation                                //
    //  return : True if the system CPU double representation is correct      //
    ////////////////////////////////////////////////////////////////////////////
    bool SysCPUCheckDouble();

    ////////////////////////////////////////////////////////////////////////////
    //  Check system CPU endianness                                           //
    //  return : True if the system CPU is little-endian                      //
    ////////////////////////////////////////////////////////////////////////////
    bool SysCPUCheckEndianness();

    ////////////////////////////////////////////////////////////////////////////
    //  Check system CPU images representations                               //
    //  return : True if the system CPU images representations are correct    //
    ////////////////////////////////////////////////////////////////////////////
    bool SysCPUCheckImages();


#endif // VTOOLS_EMBIMG_SYSTEM_SYSCPU_HEADER
