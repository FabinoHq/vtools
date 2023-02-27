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
//     Spirv.cpp : Spirv main class management                                //
////////////////////////////////////////////////////////////////////////////////
#include "Spirv.h"


////////////////////////////////////////////////////////////////////////////////
//  Spirv default constructor                                                 //
////////////////////////////////////////////////////////////////////////////////
Spirv::Spirv()
{

}

////////////////////////////////////////////////////////////////////////////////
//  Spirv destructor                                                          //
////////////////////////////////////////////////////////////////////////////////
Spirv::~Spirv()
{

}


////////////////////////////////////////////////////////////////////////////////
//  Launch Spirv single                                                       //
//  return : True if Spirv successfully executed, false otherwise             //
////////////////////////////////////////////////////////////////////////////////
bool Spirv::single(const std::string& folder, const std::string& filepath)
{
    // Set root folder
    std::string rootfolder = "";
    size_t rootpathlen = 0;
    bool rootpathfound = false;
    for (size_t i = folder.length()-1; i > 0; --i)
    {
        if (folder[i] == '/' || folder[i] == '\\')
        {
            rootpathfound = true;
            break;
        }
        ++rootpathlen;
    }
    if (rootpathfound)
    {
        for (size_t i = 0; i < (folder.length()-rootpathlen); ++i)
        {
            rootfolder += folder[i];
        }
    }
    if (rootfolder.empty())
    {
        rootfolder = "";
    }

    // Check shader file
    std::ifstream shaderfile;
    shaderfile.open(filepath, std::ios::in | std::ios::binary);
    if (!shaderfile.is_open())
    {
        return false;
    }
    shaderfile.close();

    // Compile shader
    std::string compileStr = (
        rootfolder + VToolsSpirvCompileBatch + " " + filepath
    );
    system(compileStr.c_str());
    std::string inputpath = (filepath + ".spv");
    std::string outputpath = (inputpath + ".h");

    // Open input shader file
    std::ifstream input;
    input.open(inputpath, std::ios::in | std::ios::binary);
    if (input.is_open())
    {
        // Get input shader size
        std::streampos begin, end;
        begin = input.tellg();
        input.seekg(0, std::ios::end);
        end = input.tellg();

        // Read input shader bytes
        std::vector<char> bytes(static_cast<size_t>(end - begin));
        input.seekg(0, std::ios::beg);
        input.read(&bytes[0], end - begin);
        input.close();

        // Delete spv file
        remove(inputpath.c_str());

        // Set spirv compiler byte
        if (bytes.size() >= 8)
        {
            bytes[8] = 0;
        }

        // Convert bytes to uint32_t
        uint32_t* uints = reinterpret_cast<uint32_t*>(bytes.data());

        // Open output file
        std::ofstream output;
        output.open(outputpath, std::ios::out | std::ios::trunc);
        if (output.is_open())
        {
            // Write header
            output << "#ifndef SHADERS_SHADER_HEADER\n";
            output << "#define SHADERS_SHADER_HEADER\n\n\n";

            // Write shader description
            output << "    ////////////////////////////////////";
            output << "////////////////////////////////////////\n";
            output << "    //  Shader                          ";
            output << "                                      //\n";
            output << "    ////////////////////////////////////";
            output << "////////////////////////////////////////\n";

            // Write shader size
            output << "    const size_t ShaderTypeShader" <<
                "Size = " << bytes.size() << ";\n";

            output << "    const uint32_t ShaderTypeShader" <<
                "[] =\n    {\n";

            // Write hexadecimal data
            for (size_t i = 0; i < bytes.size() / 4; ++i)
            {
                // Convert data to hexadecimal string
                std::ostringstream oss;
                oss << std::hex << std::setfill('0') <<
                    std::setw(8) << uints[i] << std::dec;
                std::string outStr = oss.str();

                // Uppercase hexadecimal
                for (size_t j = 0; j < outStr.size(); ++j)
                {
                    if (outStr[j] == 'a') { outStr[j] = 'A'; }
                    if (outStr[j] == 'b') { outStr[j] = 'B'; }
                    if (outStr[j] == 'c') { outStr[j] = 'C'; }
                    if (outStr[j] == 'd') { outStr[j] = 'D'; }
                    if (outStr[j] == 'e') { outStr[j] = 'E'; }
                    if (outStr[j] == 'f') { outStr[j] = 'F'; }
                }

                // Write tab each 4 hexadecimal data
                if ((i) % 4 == 0)
                {
                    output << "        ";
                }

                // Write hexadecimal string
                output << "0x" << outStr;
                if ((i+1) >= (bytes.size() / 4))
                {
                    // Last hexadecimal data
                    output << "u\n    };\n\n";
                }
                else
                {
                    // New line every 4 hexadecimal data
                    if ((i+1) % 4 == 0)
                    {
                        output << "u,\n";
                    }
                    else
                    {
                        output << "u, ";
                    }
                }
            }

            // Close output file
            output << "\n#endif // SHADERS_SHADER_HEADER\n";
            output.close();
        }

        // Close input shader file
        input.close();
    }

    // Spirv single successfully executed
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Launch Spirv multi                                                        //
//  return : True if Spirv successfully executed, false otherwise             //
////////////////////////////////////////////////////////////////////////////////
bool Spirv::multi(const std::string& folder)
{
    // Set root folder
    std::string rootfolder = "";
    size_t rootpathlen = 0;
    bool rootpathfound = false;
    for (size_t i = folder.length()-1; i > 0; --i)
    {
        if (folder[i] == '/' || folder[i] == '\\')
        {
            rootpathfound = true;
            break;
        }
        ++rootpathlen;
    }
    if (rootpathfound)
    {
        for (size_t i = 0; i < (folder.length()-rootpathlen); ++i)
        {
            rootfolder += folder[i];
        }
    }
    if (rootfolder.empty())
    {
        rootfolder = "";
    }

    // Input settings
    std::string token = "";
    std::string inputFolder = "";
    std::string outputFolder = "";
    int inputCount = 0;

    // Open input settings
    std::ifstream spirvinput;
    spirvinput.open((rootfolder + VToolsSpirvConfigFile), std::ios::in);
    if (!spirvinput.is_open())
    {
        // Could not open input settings
        return false;
    }

    // Read input settings
    std::getline(spirvinput, inputFolder);
    std::getline(spirvinput, outputFolder);
    spirvinput >> inputCount;
    std::getline(spirvinput, token);

    for (int i = 0; i < inputCount; ++i)
    {
        // Shaders settings
        int shadersCount = 0;
        std::string vertexShader = "";
        std::string fragmentShader = "";
        std::string outputName = "";
        std::string headerName = "";
        std::string defineName = "";
        std::string vertexComment = "";
        std::string fragmentComment = "";
        std::string vertexName = "";
        std::string fragmentName = "";

        // Read shaders settings
        spirvinput >> shadersCount;
        std::getline(spirvinput, token);
        if (shadersCount == 1)
        {
            // Fragment shader only
            std::getline(spirvinput, fragmentShader);
            std::getline(spirvinput, outputName);
            std::getline(spirvinput, headerName);
            std::getline(spirvinput, defineName);
            std::getline(spirvinput, fragmentComment);
            std::getline(spirvinput, fragmentName);

            // Open output file
            std::ofstream output;
            output.open(
                (outputFolder + outputName), std::ios::out | std::ios::trunc
            );
            if (!output.is_open())
            {
                // Could not open output file
                return false;
            }

            // Write VOS header
            output << VToolsSpirvVosHeader;
            output << headerName << '\n';
            output << VToolsSpirvCommentLine;
            output << "#ifndef " << defineName << '\n';
            output << "#define " << defineName << '\n';
            output << VToolsSpirvIncludeLine << '\n';

            // Write fragment shader header
            output << VToolsSpirvTabCommentLine;
            output << fragmentComment << '\n';
            output << VToolsSpirvTabCommentLine;

            // Check fragment shader file
            std::ifstream fFile;
            fFile.open(
                (inputFolder + fragmentShader), std::ios::in | std::ios::binary
            );
            if (!fFile.is_open())
            {
                return false;
            }
            fFile.close();

            // Compile fragment shader
            std::string compileStr = (
                rootfolder + VToolsSpirvCompileBatch + " " +
                inputFolder + fragmentShader
            );
            system(compileStr.c_str());
            std::string fReadStr = (inputFolder + fragmentShader + ".spv");

            // Open input fragment shader
            std::ifstream inputFShader;
            inputFShader.open(fReadStr, std::ios::in | std::ios::binary);
            if (!inputFShader.is_open())
            {
                // Could not open input fragment shader
                return false;
            }

            // Get input shader size
            std::streampos begin, end;
            begin = inputFShader.tellg();
            inputFShader.seekg(0, std::ios::end);
            end = inputFShader.tellg();

            // Read input shader bytes
            std::vector<char> bytes(static_cast<size_t>(end - begin));
            inputFShader.seekg(0, std::ios::beg);
            inputFShader.read(&bytes[0], end - begin);
            inputFShader.close();

            // Delete spv file
            remove(fReadStr.c_str());

            // Set spirv compiler byte
            if (bytes.size() >= 8)
            {
                bytes[8] = 0;
            }

            // Convert bytes to uint32_t
            uint32_t* uints = reinterpret_cast<uint32_t*>(bytes.data());

            // Write fragment shader size
            output << "    const size_t " << fragmentName <<
                "Size = " << bytes.size() << ";\n";

            output << "    const uint32_t " << fragmentName <<
                "[] =\n    {\n";

            // Write hexadecimal data
            for (size_t i = 0; i < bytes.size() / 4; ++i)
            {
                // Convert data to hexadecimal string
                std::ostringstream oss;
                oss << std::hex << std::setfill('0') <<
                    std::setw(8) << uints[i] << std::dec;
                std::string outStr = oss.str();

                // Uppercase hexadecimal
                for (size_t j = 0; j < outStr.size(); ++j)
                {
                    if (outStr[j] == 'a') { outStr[j] = 'A'; }
                    if (outStr[j] == 'b') { outStr[j] = 'B'; }
                    if (outStr[j] == 'c') { outStr[j] = 'C'; }
                    if (outStr[j] == 'd') { outStr[j] = 'D'; }
                    if (outStr[j] == 'e') { outStr[j] = 'E'; }
                    if (outStr[j] == 'f') { outStr[j] = 'F'; }
                }

                // Write tab each 4 hexadecimal data
                if ((i) % 4 == 0)
                {
                    output << "        ";
                }

                // Write hexadecimal string
                output << "0x" << outStr;
                if ((i+1) >= (bytes.size() / 4))
                {
                    // Last hexadecimal data
                    output << "u\n    };\n\n";
                }
                else
                {
                    // New line every 4 hexadecimal data
                    if ((i+1) % 4 == 0)
                    {
                        output << "u,\n";
                    }
                    else
                    {
                        output << "u, ";
                    }
                }
            }

            // Close output file
            output << "\n#endif // " << defineName << '\n';
            output.close();
        }
        else if (shadersCount == 2)
        {
            // Vertex and Fragment shader
            std::getline(spirvinput, vertexShader);
            std::getline(spirvinput, fragmentShader);
            std::getline(spirvinput, outputName);
            std::getline(spirvinput, headerName);
            std::getline(spirvinput, defineName);
            std::getline(spirvinput, vertexComment);
            std::getline(spirvinput, fragmentComment);
            std::getline(spirvinput, vertexName);
            std::getline(spirvinput, fragmentName);

            // Open output file
            std::ofstream output;
            output.open(
                (outputFolder + outputName), std::ios::out | std::ios::trunc
            );
            if (!output.is_open())
            {
                // Could not open output file
                return false;
            }

            // Write VOS header
            output << VToolsSpirvVosHeader;
            output << headerName << '\n';
            output << VToolsSpirvCommentLine;
            output << "#ifndef " << defineName << '\n';
            output << "#define " << defineName << '\n';
            output << VToolsSpirvIncludeLine << '\n';

            // Write vertex shader header
            output << VToolsSpirvTabCommentLine;
            output << vertexComment << '\n';
            output << VToolsSpirvTabCommentLine;

            // Check vertex shader file
            std::ifstream vFile;
            vFile.open(
                (inputFolder + vertexShader), std::ios::in | std::ios::binary
            );
            if (!vFile.is_open())
            {
                return false;
            }
            vFile.close();

            // Compile vertex shader
            std::string compileStr = (
                rootfolder + VToolsSpirvCompileBatch + " " +
                inputFolder + vertexShader
            );
            system(compileStr.c_str());
            std::string vReadStr = (inputFolder + vertexShader + ".spv");

            // Open input vertex shader
            std::ifstream inputVShader;
            inputVShader.open(vReadStr, std::ios::in | std::ios::binary);
            if (!inputVShader.is_open())
            {
                // Could not open input vertex shader
                return false;
            }

            // Get input shader size
            std::streampos begin, end;
            begin = inputVShader.tellg();
            inputVShader.seekg(0, std::ios::end);
            end = inputVShader.tellg();

            // Read input shader bytes
            std::vector<char> bytes(static_cast<size_t>(end - begin));
            inputVShader.seekg(0, std::ios::beg);
            inputVShader.read(&bytes[0], end - begin);
            inputVShader.close();

            // Delete spv file
            remove(vReadStr.c_str());

            // Set spirv compiler byte
            if (bytes.size() >= 8)
            {
                bytes[8] = 0;
            }

            // Convert bytes to uint32_t
            uint32_t* uints = reinterpret_cast<uint32_t*>(bytes.data());

            // Write vertex shader size
            output << "    const size_t " << vertexName <<
                "Size = " << bytes.size() << ";\n";

            output << "    const uint32_t " << vertexName <<
                "[] =\n    {\n";

            // Write hexadecimal data
            for (size_t i = 0; i < bytes.size() / 4; ++i)
            {
                // Convert data to hexadecimal string
                std::ostringstream oss;
                oss << std::hex << std::setfill('0') <<
                    std::setw(8) << uints[i] << std::dec;
                std::string outStr = oss.str();

                // Uppercase hexadecimal
                for (size_t j = 0; j < outStr.size(); ++j)
                {
                    if (outStr[j] == 'a') { outStr[j] = 'A'; }
                    if (outStr[j] == 'b') { outStr[j] = 'B'; }
                    if (outStr[j] == 'c') { outStr[j] = 'C'; }
                    if (outStr[j] == 'd') { outStr[j] = 'D'; }
                    if (outStr[j] == 'e') { outStr[j] = 'E'; }
                    if (outStr[j] == 'f') { outStr[j] = 'F'; }
                }

                // Write tab each 4 hexadecimal data
                if ((i) % 4 == 0)
                {
                    output << "        ";
                }

                // Write hexadecimal string
                output << "0x" << outStr;
                if ((i+1) >= (bytes.size() / 4))
                {
                    // Last hexadecimal data
                    output << "u\n    };\n\n";
                }
                else
                {
                    // New line every 4 hexadecimal data
                    if ((i+1) % 4 == 0)
                    {
                        output << "u,\n";
                    }
                    else
                    {
                        output << "u, ";
                    }
                }
            }


            // Check fragment shader file
            std::ifstream fFile;
            fFile.open(
                (inputFolder + vertexShader), std::ios::in | std::ios::binary
            );
            if (!fFile.is_open())
            {
                return false;
            }
            fFile.close();

            // Compile fragment shader
            compileStr = (
                rootfolder + VToolsSpirvCompileBatch + " " +
                inputFolder + fragmentShader
            );
            system(compileStr.c_str());
            std::string fReadStr = (inputFolder + fragmentShader + ".spv");

            // Open input fragment shader
            std::ifstream inputFShader;
            inputFShader.open(fReadStr, std::ios::in | std::ios::binary);
            if (!inputFShader.is_open())
            {
                // Could not open input fragment shader
                return false;
            }

            // Write fragment shader header
            output << VToolsSpirvTabCommentLine;
            output << fragmentComment << '\n';
            output << VToolsSpirvTabCommentLine;

            // Get input shader size
            begin = inputFShader.tellg();
            inputFShader.seekg(0, std::ios::end);
            end = inputFShader.tellg();

            // Read input shader bytes
            std::vector<char> bytes2(static_cast<size_t>(end - begin));
            inputFShader.seekg(0, std::ios::beg);
            inputFShader.read(&bytes2[0], end - begin);
            inputFShader.close();

            // Delete spv file
            remove(fReadStr.c_str());

            // Set spirv compiler byte
            if (bytes2.size() >= 8)
            {
                bytes2[8] = 0;
            }

            // Convert bytes to uint32_t
            uints = reinterpret_cast<uint32_t*>(bytes2.data());

            // Write fragment shader
            output << "    const size_t " << fragmentName <<
                "Size = " << bytes2.size() << ";\n";

            output << "    const uint32_t " << fragmentName <<
                "[] =\n    {\n";

            for (size_t i = 0; i < bytes2.size() / 4; ++i)
            {
                // Convert data to hexadecimal string
                std::ostringstream oss;
                oss << std::hex << std::setfill('0') <<
                    std::setw(8) << uints[i] << std::dec;
                std::string outStr = oss.str();

                // Uppercase hexadecimal
                for (size_t j = 0; j < outStr.size(); ++j)
                {
                    if (outStr[j] == 'a') { outStr[j] = 'A'; }
                    if (outStr[j] == 'b') { outStr[j] = 'B'; }
                    if (outStr[j] == 'c') { outStr[j] = 'C'; }
                    if (outStr[j] == 'd') { outStr[j] = 'D'; }
                    if (outStr[j] == 'e') { outStr[j] = 'E'; }
                    if (outStr[j] == 'f') { outStr[j] = 'F'; }
                }

                // Write tab each 4 hexadecimal data
                if ((i) % 4 == 0)
                {
                    output << "        ";
                }

                // Write hexadecimal string
                output << "0x" << outStr;
                if ((i+1) >= (bytes2.size() / 4))
                {
                    // Last hexadecimal data
                    output << "u\n    };\n\n";
                }
                else
                {
                    // New line every 4 hexadecimal data
                    if ((i+1) % 4 == 0)
                    {
                        output << "u,\n";
                    }
                    else
                    {
                        output << "u, ";
                    }
                }
            }

            // Close output file
            output << "\n#endif // " << defineName << '\n';
            output.close();
        }
        else
        {
            // Unknown shaders settings
            return false;
        }
    }

    // Spirv multi successfully executed
    return true;
}
