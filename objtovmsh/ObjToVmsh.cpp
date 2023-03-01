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
//     ObjToVmsh.cpp : ObjToVmsh main class management                        //
////////////////////////////////////////////////////////////////////////////////
#include "ObjToVmsh.h"


////////////////////////////////////////////////////////////////////////////////
//  ObjToVmsh default constructor                                             //
////////////////////////////////////////////////////////////////////////////////
ObjToVmsh::ObjToVmsh()
{

}

////////////////////////////////////////////////////////////////////////////////
//  ObjToVmsh destructor                                                      //
////////////////////////////////////////////////////////////////////////////////
ObjToVmsh::~ObjToVmsh()
{

}


////////////////////////////////////////////////////////////////////////////////
//  Launch ObjToVmsh                                                          //
//  return : True if ObjToVmsh successfully started                           //
////////////////////////////////////////////////////////////////////////////////
bool ObjToVmsh::launch(const std::string& filepath)
{
    // Check system CPU
    if (!SysCPUCheck())
    {
        // Invalid system CPU
        return false;
    }

    // Run ObjToVmsh
    return run(filepath);
}

////////////////////////////////////////////////////////////////////////////////
//  Run ObjToVmsh                                                             //
//  return : True if ObjToVmsh successfully executed                          //
////////////////////////////////////////////////////////////////////////////////
bool ObjToVmsh::run(const std::string& filepath)
{
    // Read input obj file
    if (!read(filepath))
    {
        // Could not read input obj file
        return false;
    }

    // ObjToVmsh successfully executed
    return true;
}


////////////////////////////////////////////////////////////////////////////////
//  Read obj file                                                             //
//  return : True if obj file is successfully read                            //
////////////////////////////////////////////////////////////////////////////////
bool ObjToVmsh::read(const std::string& filepath)
{
    // Open input obj file
    std::ifstream input;
    input.open(filepath, std::ios::in);
    if (!input.is_open())
    {
        // Could not open input obj file
        return false;
    }

    // Read input obj file
    bool reading = true;
    ObjToken curtok = OBJTOKEN_NONE;
    float curfloat = 0.0f;
    int16_t curint = 0;
    std::string curstr = "";
    while (reading)
    {
        // Get current token
        char ch = 0;
        do
        {
            if (!input.get(ch))
            {
                reading = false;
                break;
            }
        } while (isspace(ch) && (ch != ' '));

        // Faces space separators
        if (ch == ' ')
        {
            if ((curtok == OBJTOKEN_FACE_VERTEX) ||
                (curtok == OBJTOKEN_FACE_TEXCOORD) ||
                (curtok == OBJTOKEN_FACE_NORMAL))
            {
                curtok = OBJTOKEN_FACE_VERTEX;
            }
            continue;
        }

        // Parse current token
        switch (ch)
        {
            // Vertex
            case 'v':
                curtok = OBJTOKEN_VERTEX;
                break;

            // Texcoord
            case 't':
                if (curtok == OBJTOKEN_VERTEX) { curtok = OBJTOKEN_TEXCOORD; }
                break;

            // Normal
            case 'n':
                if (curtok == OBJTOKEN_VERTEX) { curtok = OBJTOKEN_NORMAL; }
                break;

            // Face
            case 'f':
                curtok = OBJTOKEN_FACE_VERTEX;
                break;

            // Faces type separator
            case '/':
                switch (curtok)
                {
                    case OBJTOKEN_FACE_VERTEX:
                        curtok = OBJTOKEN_FACE_TEXCOORD;
                        break;
                    case OBJTOKEN_FACE_TEXCOORD:
                        curtok = OBJTOKEN_FACE_NORMAL;
                        break;
                    default:
                        break;
                }
                break;

            // Value
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
            case '.': case '-':
                switch (curtok)
                {
                    // Vertex
                    case OBJTOKEN_VERTEX:
                        input.putback(ch);
                        input >> curfloat;
                        break;

                    // Texcoord
                    case OBJTOKEN_TEXCOORD:
                        input.putback(ch);
                        input >> curfloat;
                        break;

                    // Normal
                    case OBJTOKEN_NORMAL:
                        input.putback(ch);
                        input >> curfloat;
                        break;

                    // Face vertex
                    case OBJTOKEN_FACE_VERTEX:
                        input.putback(ch);
                        input >> curint;
                        break;

                    // Face texcoord
                    case OBJTOKEN_FACE_TEXCOORD:
                        input.putback(ch);
                        input >> curint;
                        break;

                    // Face normal
                    case OBJTOKEN_FACE_NORMAL:
                        input.putback(ch);
                        input >> curint;
                        break;

                    // Unknown
                    default:
                        break;
                }
                break;

            // Comment
            case '#':
                std::getline(input, curstr);
                curtok = OBJTOKEN_NONE;
                break;

            // Unknown
            default:
                curtok = OBJTOKEN_NONE;
                break;
        }
    }

    // Input obj file successfully read
    input.close();
    return true;
}
