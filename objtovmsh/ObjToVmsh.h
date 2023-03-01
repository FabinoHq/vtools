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
//     ObjToVmsh.h : ObjToVmsh main class management                          //
////////////////////////////////////////////////////////////////////////////////
#ifndef VTOOLS_OBJTOVMSH_OBJTOVMSH_HEADER
#define VTOOLS_OBJTOVMSH_OBJTOVMSH_HEADER

    #include "System/System.h"
    #include "System/SysCPU.h"

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
    //  ObjToken enumeration                                                  //
    ////////////////////////////////////////////////////////////////////////////
    enum ObjToken
    {
        OBJTOKEN_NONE = 0,
        OBJTOKEN_VERTEX = 1,
        OBJTOKEN_TEXCOORD = 2,
        OBJTOKEN_NORMAL = 3,
        OBJTOKEN_FACE_VERTEX = 4,
        OBJTOKEN_FACE_TEXCOORD = 5,
        OBJTOKEN_FACE_NORMAL = 6
    };


    ////////////////////////////////////////////////////////////////////////////
    //  Vertex data structure                                                 //
    ////////////////////////////////////////////////////////////////////////////
    struct Vertex
    {
        float x;
        float y;
        float z;
    };

    ////////////////////////////////////////////////////////////////////////////
    //  Texcoord data structure                                               //
    ////////////////////////////////////////////////////////////////////////////
    struct Texcoord
    {
        float x;
        float y;
    };

    ////////////////////////////////////////////////////////////////////////////
    //  Normal data structure                                                 //
    ////////////////////////////////////////////////////////////////////////////
    struct Normal
    {
        float x;
        float y;
        float z;
    };

    ////////////////////////////////////////////////////////////////////////////
    //  Face data structure                                                   //
    ////////////////////////////////////////////////////////////////////////////
    struct Face
    {
        bool quad;
        uint16_t vertex[4];
        uint16_t texcoord[4];
        uint16_t normal[4];
    };


    ////////////////////////////////////////////////////////////////////////////
    //  ObjToVmsh main class definition                                       //
    ////////////////////////////////////////////////////////////////////////////
    class ObjToVmsh
    {
        public:
            ////////////////////////////////////////////////////////////////////
            //  ObjToVmsh default constructor                                 //
            ////////////////////////////////////////////////////////////////////
            ObjToVmsh();

            ////////////////////////////////////////////////////////////////////
            //  ObjToVmsh destructor                                          //
            ////////////////////////////////////////////////////////////////////
            ~ObjToVmsh();


            ////////////////////////////////////////////////////////////////////
            //  Launch ObjToVmsh                                              //
            //  return : True if ObjToVmsh successfully started               //
            ////////////////////////////////////////////////////////////////////
            bool launch(const std::string& filepath);

            ////////////////////////////////////////////////////////////////////
            //  Run ObjToVmsh                                                 //
            //  return : True if ObjToVmsh successfully executed              //
            ////////////////////////////////////////////////////////////////////
            bool run(const std::string& filepath);


        private:
            ////////////////////////////////////////////////////////////////////
            //  Read obj file                                                 //
            //  return : True if obj file is successfully read                //
            ////////////////////////////////////////////////////////////////////
            bool read(const std::string& filepath);

            ////////////////////////////////////////////////////////////////////
            //  Write vmsh file                                               //
            //  return : True if vmsh file is successfully written            //
            ////////////////////////////////////////////////////////////////////
            bool write(const std::string& filepath);


        private:
            ////////////////////////////////////////////////////////////////////
            //  ObjToVmsh private copy constructor : Not copyable             //
            ////////////////////////////////////////////////////////////////////
            ObjToVmsh(const ObjToVmsh&) = delete;

            ////////////////////////////////////////////////////////////////////
            //  ObjToVmsh private copy operator : Not copyable                //
            ////////////////////////////////////////////////////////////////////
            ObjToVmsh& operator=(const ObjToVmsh&) = delete;


        private:
            std::vector<Vertex>     m_vertices;     // Obj vertices
            std::vector<Texcoord>   m_texcoords;    // Obj texcoords
            std::vector<Normal>     m_normals;      // Obj normals
            std::vector<Face>       m_faces;        // Obj faces
    };


#endif // VTOOLS_OBJTOVMSH_OBJTOVMSH_HEADER
