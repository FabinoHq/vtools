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

    // Write output vmsh file
    if (!write(filepath + ".vmsh"))
    {
        // Could not write output vmsh file
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
    uint16_t curint = 0;
    int32_t curindex = 0;
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
            if ((curtok == OBJTOKEN_FACE_TEXCOORD) ||
                (curtok == OBJTOKEN_FACE_NORMAL))
            {
                curtok = OBJTOKEN_FACE_VERTEX;
                ++curindex;
            }
            continue;
        }

        // Parse current token
        switch (ch)
        {
            // Vertex
            case 'v':
                curtok = OBJTOKEN_VERTEX;
                curindex = 0;
                break;

            // Texcoord
            case 't':
                if (curtok == OBJTOKEN_VERTEX) { curtok = OBJTOKEN_TEXCOORD; }
                curindex = 0;
                break;

            // Normal
            case 'n':
                if (curtok == OBJTOKEN_VERTEX) { curtok = OBJTOKEN_NORMAL; }
                curindex = 0;
                break;

            // Face
            case 'f':
                curtok = OBJTOKEN_FACE_VERTEX;
                curindex = 0;
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
                        switch (curindex)
                        {
                            case 0:
                                m_vertices.push_back(Vertex());
                                m_vertices.back().x = curfloat;
                                m_vertices.back().y = 0.0f;
                                m_vertices.back().z = 0.0f;
                                break;
                            case 1:
                                m_vertices.back().y = curfloat;
                                break;
                            case 2:
                                m_vertices.back().z = curfloat;
                                break;
                            default:
                                break;
                        }
                        ++curindex;
                        break;

                    // Texcoord
                    case OBJTOKEN_TEXCOORD:
                        input.putback(ch);
                        input >> curfloat;
                        switch (curindex)
                        {
                            case 0:
                                m_texcoords.push_back(Texcoord());
                                m_texcoords.back().x = curfloat;
                                m_texcoords.back().y = 0.0f;
                                break;
                            case 1:
                                m_texcoords.back().y = curfloat;
                                break;
                            default:
                                break;
                        }
                        ++curindex;
                        break;

                    // Normal
                    case OBJTOKEN_NORMAL:
                        input.putback(ch);
                        input >> curfloat;
                        switch (curindex)
                        {
                            case 0:
                                m_normals.push_back(Normal());
                                m_normals.back().x = curfloat;
                                m_normals.back().y = 0.0f;
                                m_normals.back().z = 0.0f;
                                break;
                            case 1:
                                m_normals.back().y = curfloat;
                                break;
                            case 2:
                                m_normals.back().z = curfloat;
                                break;
                            default:
                                break;
                        }
                        ++curindex;
                        break;

                    // Face vertex
                    case OBJTOKEN_FACE_VERTEX:
                        if ((ch != '.') && (ch != '-')) { input.putback(ch); }
                        input >> curint;
                        switch (curindex)
                        {
                            case 0:
                                m_faces.push_back(Face());
                                m_faces.back().quad = false;
                                m_faces.back().vertex[0] = curint;
                                m_faces.back().vertex[1] = 0;
                                m_faces.back().vertex[2] = 0;
                                m_faces.back().vertex[3] = 0;
                                m_faces.back().texcoord[0] = 0;
                                m_faces.back().texcoord[1] = 0;
                                m_faces.back().texcoord[2] = 0;
                                m_faces.back().texcoord[3] = 0;
                                m_faces.back().normal[0] = 0;
                                m_faces.back().normal[1] = 0;
                                m_faces.back().normal[2] = 0;
                                m_faces.back().normal[3] = 0;
                                break;
                            case 1:
                                m_faces.back().vertex[1] = curint;
                                break;
                            case 2:
                                m_faces.back().vertex[2] = curint;
                                break;
                            case 3:
                                m_faces.back().quad = true;
                                m_faces.back().vertex[3] = curint;
                                break;
                            default:
                                // Invalid (only tris or quads are valids)
                                return false;
                        }
                        break;

                    // Face texcoord
                    case OBJTOKEN_FACE_TEXCOORD:
                        if ((ch != '.') && (ch != '-')) { input.putback(ch); }
                        input >> curint;
                        switch (curindex)
                        {
                            case 0:
                                m_faces.back().texcoord[0] = curint;
                                break;
                            case 1:
                                m_faces.back().texcoord[1] = curint;
                                break;
                            case 2:
                                m_faces.back().texcoord[2] = curint;
                                break;
                            case 3:
                                m_faces.back().quad = true;
                                m_faces.back().texcoord[3] = curint;
                                break;
                            default:
                                // Invalid (only tris or quads are valids)
                                return false;
                        }
                        break;

                    // Face normal
                    case OBJTOKEN_FACE_NORMAL:
                        if ((ch != '.') && (ch != '-')) { input.putback(ch); }
                        input >> curint;
                        switch (curindex)
                        {
                            case 0:
                                m_faces.back().normal[0] = curint;
                                break;
                            case 1:
                                m_faces.back().normal[1] = curint;
                                break;
                            case 2:
                                m_faces.back().normal[2] = curint;
                                break;
                            case 3:
                                m_faces.back().quad = true;
                                m_faces.back().normal[3] = curint;
                                break;
                            default:
                                // Invalid (only tris or quads are valids)
                                return false;
                        }
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

////////////////////////////////////////////////////////////////////////////////
//  Write vmsh file                                                           //
//  return : True if vmsh file is successfully written                        //
////////////////////////////////////////////////////////////////////////////////
bool ObjToVmsh::write(const std::string& filepath)
{
    // Open output vmsh file
    std::ofstream output;
    output.open(filepath, std::ios::out | std::ios::trunc);
    if (!output.is_open())
    {
        // Could not open output vmsh file
        return false;
    }

    output << "Vertices :\n";
    for (int i = 0; i < m_vertices.size(); ++i)
    {
        output << m_vertices[i].x << ' ' <<
            m_vertices[i].y << ' ' << m_vertices[i].z << '\n';
    }

    output << "Texcoords :\n";
    for (int i = 0; i < m_texcoords.size(); ++i)
    {
        output << m_texcoords[i].x << ' ' << m_texcoords[i].y << '\n';
    }

    output << "Normals :\n";
    for (int i = 0; i < m_normals.size(); ++i)
    {
        output << m_normals[i].x << ' ' <<
            m_normals[i].y << ' ' << m_normals[i].z << '\n';
    }

    output << "Faces :\n";
    for (int i = 0; i < m_faces.size(); ++i)
    {
        output << (m_faces[i].quad?("quad"):("tri")) << '\n';
        output << m_faces[i].vertex[0] << '/' <<
            m_faces[i].texcoord[0] << '/' << m_faces[i].normal[0] << ' ';
        output << m_faces[i].vertex[1] << '/' <<
            m_faces[i].texcoord[1] << '/' << m_faces[i].normal[1] << ' ';
        output << m_faces[i].vertex[2] << '/' <<
            m_faces[i].texcoord[2] << '/' << m_faces[i].normal[2] << ' ';

        if (m_faces[i].quad)
        {
            output << m_faces[i].vertex[3] << '/' <<
                m_faces[i].texcoord[3] << '/' << m_faces[i].normal[3] << ' ';
        }

        output << '\n';
    }

    // Output vmsh file successfully written
    output.close();
    return true;
}
