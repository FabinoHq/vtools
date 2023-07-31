////////////////////////////////////////////////////////////////////////////////
//  _______                       __________________________________________  //
//  \\ .   \            _________/ . . . . . . . . . . . . . . . . . . .   /  //
//   \\ .   \       ___/ . . . . .    ____________________________________/   //
//    \\ .   \   __/. . .   _________/  / \ // .    \ //.  ///. _______/      //
//     \\ .   \_//      ___/.  ________/  ///.      ///.  ///. /____          //
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
//     System/SysClock.cpp : System Clock management                          //
////////////////////////////////////////////////////////////////////////////////
#include "SysClock.h"


////////////////////////////////////////////////////////////////////////////////
//  SysClock default constructor                                              //
////////////////////////////////////////////////////////////////////////////////
SysClock::SysClock() :
m_start(std::chrono::steady_clock::now())
{

}

////////////////////////////////////////////////////////////////////////////////
//  SysClock destructor                                                       //
////////////////////////////////////////////////////////////////////////////////
SysClock::~SysClock()
{

}


////////////////////////////////////////////////////////////////////////////////
//  Get elapsed time in seconds and reset the clock                           //
////////////////////////////////////////////////////////////////////////////////
double SysClock::getAndReset()
{
    double elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>
        (std::chrono::steady_clock::now() - m_start).count()*0.000000001;
    m_start = std::chrono::steady_clock::now();
    return elapsedTime;
}

////////////////////////////////////////////////////////////////////////////////
//  Get elapsed time in seconds and reset the clock                           //
////////////////////////////////////////////////////////////////////////////////
float SysClock::getAndResetF()
{
    float elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>
        (std::chrono::steady_clock::now() - m_start).count()*0.000000001f;
    m_start = std::chrono::steady_clock::now();
    return elapsedTime;
}
