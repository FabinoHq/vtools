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
    #include <cstdio>
    #include <vector>
    #include <new>


    ////////////////////////////////////////////////////////////////////////////
    //  VTools spirv strings constants                                        //
    ////////////////////////////////////////////////////////////////////////////
    const std::string VToolsSpirvVosHeader =
        "///////////////////////////////////////"
        "/////////////////////////////////////////\n"
        "//     _______                       __"
        "__________________________________     //\n"
        "//     \\\\ .   \\            _________/ ."
        " . . . . . . . . . . . . . . .   /     //\n"
        "//      \\\\ .   \\       ___/ . . . . .  "
        "  ______________________________/      //\n"
        "//       \\\\ .   \\   __/. . .   ________"
        "_/     /    // .  __________/          //\n"
        "//        \\\\ .   \\_//      ___/ .  ____"
        "_     /    // .  /______               //\n"
        "//         \\\\ .   \\/     _/ // .  /    "
        "\\\\    |    \\\\  .        \\              //\n"
        "//          \\\\ .        /   || .  |    "
        "||    |     \\\\______     \\             //\n"
        "//           \\\\ .      /    || .  \\____"
        "//    |  _________//     /             //\n"
        "//            \\\\ .    /     //  .      "
        "      / // . . . .      /              //\n"
        "//             \\\\____/     //__________"
        "_____/ //______________/               //\n"
        "//                                     "
        "                                       //\n"
        "///////////////////////////////////////"
        "/////////////////////////////////////////\n"
        "//   This is free and unencumbered soft"
        "ware released into the public domain.  //\n"
        "//                                     "
        "                                       //\n"
        "//   Anyone is free to copy, modify, pu"
        "blish, use, compile, sell, or          //\n"
        "//   distribute this software, either i"
        "n source code form or as a compiled    //\n"
        "//   binary, for any purpose, commercia"
        "l or non-commercial, and by any        //\n"
        "//   means.                            "
        "                                       //\n"
        "//                                     "
        "                                       //\n"
        "//   In jurisdictions that recognize co"
        "pyright laws, the author or authors    //\n"
        "//   of this software dedicate any and "
        "all copyright interest in the          //\n"
        "//   software to the public domain. We "
        "make this dedication for the benefit   //\n"
        "//   of the public at large and to the "
        "detriment of our heirs and             //\n"
        "//   successors. We intend this dedicat"
        "ion to be an overt act of              //\n"
        "//   relinquishment in perpetuity of al"
        "l present and future rights to this    //\n"
        "//   software under copyright law.     "
        "                                       //\n"
        "//                                     "
        "                                       //\n"
        "//   THE SOFTWARE IS PROVIDED \"AS IS\", "
        "WITHOUT WARRANTY OF ANY KIND,          //\n"
        "//   EXPRESS OR IMPLIED, INCLUDING BUT "
        "NOT LIMITED TO THE WARRANTIES OF       //\n"
        "//   MERCHANTABILITY, FITNESS FOR A PAR"
        "TICULAR PURPOSE AND NONINFRINGEMENT.   //\n"
        "//   IN NO EVENT SHALL THE AUTHORS BE L"
        "IABLE FOR ANY CLAIM, DAMAGES OR        //\n"
        "//   OTHER LIABILITY, WHETHER IN AN ACT"
        "ION OF CONTRACT, TORT OR OTHERWISE,    //\n"
        "//   ARISING FROM, OUT OF OR IN CONNECT"
        "ION WITH THE SOFTWARE OR THE USE OR    //\n"
        "//   OTHER DEALINGS IN THE SOFTWARE.   "
        "                                       //\n"
        "//                                     "
        "                                       //\n"
        "//   For more information, please refer"
        " to <https://unlicense.org>            //\n"
        "///////////////////////////////////////"
        "/////////////////////////////////////////\n"
        "//    VOS : Virtual Operating System   "
        "                                       //\n";

    const std::string VToolsSpirvCommentLine =
        "///////////////////////////////////////"
        "/////////////////////////////////////////\n";

    const std::string VToolsSpirvIncludeLine =
        "\n"
        "    #include \"../../System/System.h\"\n"
        "\n"
        "    #include <cstddef>\n"
        "    #include <cstdint>\n"
        "\n";

    const std::string VToolsSpirvTabCommentLine =
        "    ///////////////////////////////////"
        "/////////////////////////////////////////\n";


    ////////////////////////////////////////////////////////////////////////////
    //  VTools spirv configuration file                                       //
    ////////////////////////////////////////////////////////////////////////////
    const std::string VToolsSpirvConfigFile = "spirv.txt";

    ////////////////////////////////////////////////////////////////////////////
    //  VTools spirv compile batch file                                       //
    ////////////////////////////////////////////////////////////////////////////
    const std::string VToolsSpirvCompileBatch = "compile.bat";


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
            bool single(const std::string& folder, const std::string& filepath);

            ////////////////////////////////////////////////////////////////////
            //  Launch Spirv multi                                            //
            //  return : True if Spirv successfully executed, false otherwise //
            ////////////////////////////////////////////////////////////////////
            bool multi(const std::string& folder);


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
