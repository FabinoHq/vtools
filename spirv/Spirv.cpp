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
bool Spirv::single(const std::string& filepath)
{
    // Check shader file
    std::ifstream shaderfile;
    shaderfile.open(filepath, std::ios::in | std::ios::binary);
    if (!shaderfile.is_open())
    {
        return false;
    }
    shaderfile.close();

    // Compile shader
    std::string compileStr = (VToolsSpirvCompileBatch + filepath);
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
bool Spirv::multi()
{
    // Spirv multi successfully executed
    return true;
}
