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
//     Spirv.h : Spirv main class management                                  //
////////////////////////////////////////////////////////////////////////////////
#ifndef VTOOLS_SPIRV_SPIRV_HEADER
#define VTOOLS_SPIRV_SPIRV_HEADER

    #include <iostream>
    #include <iomanip>
    #include <string>
    #include <sstream>
    #include <fstream>
    #include <cstddef>
    #include <cstdint>
    #include <new>


    ////////////////////////////////////////////////////////////////////////////
    //  Spirv main class definition                                           //
    ////////////////////////////////////////////////////////////////////////////
    class Spirv
    {
        public:
            ////////////////////////////////////////////////////////////////////
            //  Spirv default constructor                                     //
            ////////////////////////////////////////////////////////////////////
            Spirv();

            ////////////////////////////////////////////////////////////////////
            //  Spirv destructor                                              //
            ////////////////////////////////////////////////////////////////////
            ~Spirv();


            ////////////////////////////////////////////////////////////////////
            //  Launch Spirv single                                           //
            //  return : True if Spirv successfully executed, false otherwise //
            ////////////////////////////////////////////////////////////////////
            bool single(const std::string& filepath);

            ////////////////////////////////////////////////////////////////////
            //  Launch Spirv multi                                            //
            //  return : True if Spirv successfully executed, false otherwise //
            ////////////////////////////////////////////////////////////////////
            bool multi();


        private:
            ////////////////////////////////////////////////////////////////////
            //  Spirv private copy constructor : Not copyable                 //
            ////////////////////////////////////////////////////////////////////
            Spirv(const Spirv&) = delete;

            ////////////////////////////////////////////////////////////////////
            //  Spirv private copy operator : Not copyable                    //
            ////////////////////////////////////////////////////////////////////
            Spirv& operator=(const Spirv&) = delete;


        private:
    };


#endif // VTOOLS_SPIRV_SPIRV_HEADER
