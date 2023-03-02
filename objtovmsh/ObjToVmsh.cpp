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

    // Compute vertices and indices
    if (!compute())
    {
        // Could not compute vertices and indices
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

    // Clear vertices, texcoords, normals, and faces
    m_vertices.clear();
    m_texcoords.clear();
    m_normals.clear();
    m_faces.clear();

    // Read input obj file
    bool reading = true;
    ObjToken curtok = OBJTOKEN_NONE;
    float curfloat = 0.0f;
    int32_t curint = 0;
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
                        if (ch != '.') { input.putback(ch); }
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
                                // Invalid (only triangles or quads are valid)
                                return false;
                        }
                        break;

                    // Face texcoord
                    case OBJTOKEN_FACE_TEXCOORD:
                        if (ch != '.') { input.putback(ch); }
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
                                // Invalid (only triangles or quads are valid)
                                return false;
                        }
                        break;

                    // Face normal
                    case OBJTOKEN_FACE_NORMAL:
                        if (ch != '.') { input.putback(ch); }
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
                                // Invalid (only triangles or quads are valid)
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
//  Compute vertices and indices                                              //
//  return : True if vertices and indices are computed                        //
////////////////////////////////////////////////////////////////////////////////
bool ObjToVmsh::compute()
{
    // Check for quads
    bool quads = false;
    for (int32_t i = 0; i < m_faces.size(); ++i)
    {
        if (m_faces[i].quad)
        {
            quads = true;
            break;
        }
    }

    // Triangulate mesh
    if (quads)
    {
        std::vector<Face> faces = m_faces;
        m_faces.clear();
        for (int32_t i = 0; i < faces.size(); ++i)
        {
            if (faces[i].quad)
            {
                // Triangulate quad
                m_faces.push_back(Face());
                m_faces.back().quad = false;
                m_faces.back().vertex[0] = faces[i].vertex[0];
                m_faces.back().vertex[1] = faces[i].vertex[1];
                m_faces.back().vertex[2] = faces[i].vertex[2];
                m_faces.back().vertex[3] = 0;
                m_faces.back().texcoord[0] = faces[i].texcoord[0];
                m_faces.back().texcoord[1] = faces[i].texcoord[1];
                m_faces.back().texcoord[2] = faces[i].texcoord[2];
                m_faces.back().texcoord[3] = 0;
                m_faces.back().normal[0] = faces[i].normal[0];
                m_faces.back().normal[1] = faces[i].normal[1];
                m_faces.back().normal[2] = faces[i].normal[2];
                m_faces.back().normal[3] = 0;

                m_faces.push_back(Face());
                m_faces.back().quad = false;
                m_faces.back().vertex[0] = faces[i].vertex[2];
                m_faces.back().vertex[1] = faces[i].vertex[3];
                m_faces.back().vertex[2] = faces[i].vertex[0];
                m_faces.back().vertex[3] = 0;
                m_faces.back().texcoord[0] = faces[i].texcoord[2];
                m_faces.back().texcoord[1] = faces[i].texcoord[3];
                m_faces.back().texcoord[2] = faces[i].texcoord[0];
                m_faces.back().texcoord[3] = 0;
                m_faces.back().normal[0] = faces[i].normal[2];
                m_faces.back().normal[1] = faces[i].normal[3];
                m_faces.back().normal[2] = faces[i].normal[0];
                m_faces.back().normal[3] = 0;
            }
            else
            {
                // Copy triangle
                m_faces.push_back(Face());
                m_faces.back().quad = false;
                m_faces.back().vertex[0] = faces[i].vertex[0];
                m_faces.back().vertex[1] = faces[i].vertex[1];
                m_faces.back().vertex[2] = faces[i].vertex[2];
                m_faces.back().vertex[3] = 0;
                m_faces.back().texcoord[0] = faces[i].texcoord[0];
                m_faces.back().texcoord[1] = faces[i].texcoord[1];
                m_faces.back().texcoord[2] = faces[i].texcoord[2];
                m_faces.back().texcoord[3] = 0;
                m_faces.back().normal[0] = faces[i].normal[0];
                m_faces.back().normal[1] = faces[i].normal[1];
                m_faces.back().normal[2] = faces[i].normal[2];
                m_faces.back().normal[3] = 0;
            }
        }
    }

    // Clear vertices and indices
    m_verts.clear();
    m_indices.clear();

    // Compute all vertices for each triangles
    int16_t curIndex = 0;
    std::vector<VertexData> verticesData;
    for (int32_t i = 0; i < m_faces.size(); ++i)
    {
        for (int32_t j = 0; j < 3; ++j)
        {
            // Get obj indices
            int32_t vertexIndex = (m_faces[i].vertex[j])-1;
            int32_t texcoordIndex = (m_faces[i].texcoord[j])-1;
            int32_t normalIndex = (m_faces[i].normal[j])-1;

            // Vertex data
            VertexData vertexData;
            vertexData.copy = false;
            vertexData.index = 0;
            vertexData.vx = 0.0f;
            vertexData.vy = 0.0f;
            vertexData.vz = 0.0f;
            vertexData.tx = 0.0f;
            vertexData.ty = 0.0f;
            vertexData.nx = 0.0f;
            vertexData.ny = 0.0f;
            vertexData.nz = 0.0f;

            // Set vertex data
            if (vertexIndex >= 0)
            {
                // Check vertex index
                if (vertexIndex >= m_vertices.size())
                {
                    // Invalid vertex index
                    return false;
                }
                vertexData.vx = m_vertices[vertexIndex].x;
                vertexData.vy = m_vertices[vertexIndex].y;
                vertexData.vz = m_vertices[vertexIndex].z;
            }
            else
            {
                // Todo : handle negative indices
                return false;
            }

            // Set texcoord data
            if (texcoordIndex >= 0)
            {
                // Check vertex index
                if (texcoordIndex >= m_texcoords.size())
                {
                    // Invalid vertex index
                    return false;
                }
                vertexData.tx = m_texcoords[texcoordIndex].x;
                vertexData.ty = m_texcoords[texcoordIndex].y;
            }
            else
            {
                // Todo : handle negative indices
                return false;
            }

            // Set normal data
            if (normalIndex >= 0)
            {
                // Check normal index
                if (normalIndex >= m_normals.size())
                {
                    // Invalid normal index
                    return false;
                }
                vertexData.nx = m_normals[normalIndex].x;
                vertexData.ny = m_normals[normalIndex].y;
                vertexData.nz = m_normals[normalIndex].z;
            }
            else
            {
                // Todo : handle negative indices
                return false;
            }

            // Reverse texcoord Y axis
            vertexData.ty = 1.0f-vertexData.ty;

            // Clamp texcoord
            if (vertexData.tx <= 0.0f) { vertexData.tx = 0.0f; }
            if (vertexData.tx >= 1.0f) { vertexData.tx = 1.0f; }
            if (vertexData.ty <= 0.0f) { vertexData.ty = 0.0f; }
            if (vertexData.ty >= 1.0f) { vertexData.ty = 1.0f; }

            // Normalize normal
            Vector3 normalVector;
            normalVector.set(vertexData.nx, vertexData.ny, vertexData.nz);
            normalVector.normalize();
            vertexData.nx = normalVector.vec[0];
            vertexData.ny = normalVector.vec[1];
            vertexData.nz = normalVector.vec[2];

            // Clamp normal
            if (vertexData.nx <= -1.0f) {vertexData.nx = -1.0f; }
            if (vertexData.nx >= 1.0f) { vertexData.nx = 1.0f; }
            if (vertexData.ny <= -1.0f) { vertexData.ny = -1.0f; }
            if (vertexData.ny >= 1.0f) { vertexData.ny = 1.0f; }
            if (vertexData.nz <= -1.0f) { vertexData.nz = -1.0f; }
            if (vertexData.nz >= 1.0f) { vertexData.nz = 1.0f; }

            // Check if the vertex already exists
            bool vertexFound = false;
            for (int32_t k = 0; k < verticesData.size(); ++k)
            {
                if (vertsAreEqual(vertexData.vx, verticesData[k].vx) &&
                    vertsAreEqual(vertexData.vy, verticesData[k].vy) &&
                    vertsAreEqual(vertexData.vz, verticesData[k].vz) &&
                    vertsAreEqual(vertexData.tx, verticesData[k].tx) &&
                    vertsAreEqual(vertexData.ty, verticesData[k].ty) &&
                    vertsAreEqual(vertexData.nx, verticesData[k].nx) &&
                    vertsAreEqual(vertexData.ny, verticesData[k].ny) &&
                    vertsAreEqual(vertexData.nz, verticesData[k].nz))
                {
                    // Add existing vertex
                    verticesData.push_back(VertexData());
                    verticesData.back().copy = true;
                    verticesData.back().index = verticesData[k].index;
                    verticesData.back().vx = 0.0f;
                    verticesData.back().vy = 0.0f;
                    verticesData.back().vz = 0.0f;
                    verticesData.back().tx = 0.0f;
                    verticesData.back().ty = 0.0f;
                    verticesData.back().nx = 0.0f;
                    verticesData.back().ny = 0.0f;
                    verticesData.back().nz = 0.0f;
                    vertexFound = true;
                    break;
                }
            }

            if (!vertexFound)
            {
                // Add new vertex
                verticesData.push_back(VertexData());
                verticesData.back().copy = false;
                verticesData.back().index = curIndex++;
                verticesData.back().vx = vertexData.vx;
                verticesData.back().vy = vertexData.vy;
                verticesData.back().vz = vertexData.vz;
                verticesData.back().tx = vertexData.tx;
                verticesData.back().ty = vertexData.ty;
                verticesData.back().nx = vertexData.nx;
                verticesData.back().ny = vertexData.ny;
                verticesData.back().nz = vertexData.nz;
            }
        }
    }

    // Add final vertices and indices
    for (int32_t k = 0; k < verticesData.size(); ++k)
    {
        if (!verticesData[k].copy)
        {
            // Add new vertex
            m_verts.push_back(verticesData[k].vx);
            m_verts.push_back(verticesData[k].vy);
            m_verts.push_back(verticesData[k].vz);
            m_verts.push_back(verticesData[k].tx);
            m_verts.push_back(verticesData[k].ty);
            m_verts.push_back(verticesData[k].nx);
            m_verts.push_back(verticesData[k].ny);
            m_verts.push_back(verticesData[k].nz);
        }

        // Add new index
        m_indices.push_back(verticesData[k].index);
    }

    // Vertices and indices are successfully computed
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
    output.open(filepath, std::ios::out | std::ios::trunc | std::ios::binary);
    if (!output.is_open())
    {
        // Could not open output vmsh file
        return false;
    }

    // Write VMSH header
    char header[] = "VMSH";
    char majorVersion = 1;
    char minorVersion = 0;
    output.write(header, sizeof(char)*4);
    output.write(&majorVersion, sizeof(char));
    output.write(&minorVersion, sizeof(char));

    // Write static mesh type
    char animated = 0;
    output.write(&animated, sizeof(char));

    // Write vertices and indices count
    uint32_t verticesCount = static_cast<uint32_t>(m_verts.size());
    uint32_t indicesCount = static_cast<uint32_t>(m_indices.size());
    output.write((char*)&verticesCount, sizeof(uint32_t));
    output.write((char*)&indicesCount, sizeof(uint32_t));

    // Write vertices
    output.write((char*)m_verts.data(), sizeof(float)*verticesCount);

    // Write indices
    output.write((char*)m_indices.data(), sizeof(uint16_t)*indicesCount);

    // Output vmsh file successfully written
    output.close();
    return true;
}
