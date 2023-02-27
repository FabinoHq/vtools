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
//     Embimg.h : Embedded Image main class management                        //
////////////////////////////////////////////////////////////////////////////////
#ifndef VTOOLS_EMBIMG_EMBIMG_HEADER
#define VTOOLS_EMBIMG_EMBIMG_HEADER

    #include "System/System.h"
    #include "System/SysCPU.h"
    #include "Images/PNGFile.h"

    #include <iostream>
    #include <cstddef>
    #include <cstdint>
    #include <new>


    ////////////////////////////////////////////////////////////////////////////
    //  Embimg main class definition                                          //
    ////////////////////////////////////////////////////////////////////////////
    class Embimg
    {
        public:
            ////////////////////////////////////////////////////////////////////
            //  Embimg default constructor                                    //
            ////////////////////////////////////////////////////////////////////
            Embimg();

            ////////////////////////////////////////////////////////////////////
            //  Embimg destructor                                             //
            ////////////////////////////////////////////////////////////////////
            ~Embimg();


            ////////////////////////////////////////////////////////////////////
            //  Launch Embimg                                                 //
            //  return : True if Embimg successfully started, false otherwise //
            ////////////////////////////////////////////////////////////////////
            bool launch();

            ////////////////////////////////////////////////////////////////////
            //  Run Embimg                                                    //
            //  return : True if Embimg successfully executed                 //
            ////////////////////////////////////////////////////////////////////
            bool run();


        private:
            ////////////////////////////////////////////////////////////////////
            //  Embimg private copy constructor : Not copyable                //
            ////////////////////////////////////////////////////////////////////
            Embimg(const Embimg&) = delete;

            ////////////////////////////////////////////////////////////////////
            //  Embimg private copy operator : Not copyable                   //
            ////////////////////////////////////////////////////////////////////
            Embimg& operator=(const Embimg&) = delete;


        private:
    };


#endif // VTOOLS_EMBIMG_EMBIMG_HEADER
