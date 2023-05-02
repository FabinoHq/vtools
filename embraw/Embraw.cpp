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
//     Embraw.cpp : Embedded Raw main class management                        //
////////////////////////////////////////////////////////////////////////////////
#include "Embraw.h"


////////////////////////////////////////////////////////////////////////////////
//  Embraw default constructor                                                //
////////////////////////////////////////////////////////////////////////////////
Embraw::Embraw()
{

}

////////////////////////////////////////////////////////////////////////////////
//  Embraw destructor                                                         //
////////////////////////////////////////////////////////////////////////////////
Embraw::~Embraw()
{

}


////////////////////////////////////////////////////////////////////////////////
//  Launch Embraw                                                             //
//  return : True if Embraw successfully started, false otherwise             //
////////////////////////////////////////////////////////////////////////////////
bool Embraw::launch(const std::string& filepath)
{
    // Run Embraw
    return run(filepath);
}

////////////////////////////////////////////////////////////////////////////////
//  Run Embraw                                                                //
//  return : True if Embraw successfully executed                             //
////////////////////////////////////////////////////////////////////////////////
bool Embraw::run(const std::string& filepath)
{
    // Get file name
    std::string filename = "";
    std::string outputname = "";
    std::string filepathstr = filepath;
    size_t filtitlelen = 0;
    bool filepathfound = false;

    // Extract file name from file path
    for (size_t i = filepath.length()-1; i > 0; --i)
    {
        if (filepath[i] == '/' || filepath[i] == '\\')
        {
            filepathfound = true;
            break;
        }
        filename += filepath[i];
        ++filtitlelen;
    }
    if (filepathfound)
    {
        std::reverse(filename.begin(), filename.end());
    }
    else
    {
        filename = filepath;
    }

    // Add .h extension to filename
    filename += ".h";

    // Load input raw file
    unsigned int dataSize = 0;
    std::ifstream input;
    input.open(filepath, std::ios::in | std::ios::binary);
    if (!input.is_open())
    {
        // Could not open input file
        return false;
    }

    // Read data size
    input.seekg(0, std::ios::end);
    dataSize = static_cast<unsigned int>(input.tellg());
    input.seekg(0, std::ios::beg);

    // Check data size
    if (dataSize <= 0)
    {
        // Invalid file size
        return false;
    }

    // Open output file
    std::ofstream output;
    output.open(filename, std::ios::out | std::ios::trunc);
    if (output.is_open())
    {
        // Write .h protection
        output << "#ifndef RAW_EMBEDDED_RAW_HEADER\n";
        output << "#define RAW_EMBEDDED_RAW_HEADER\n\n\n";

        // Write raw description
        output << "    ////////////////////////////////////";
        output << "////////////////////////////////////////\n";
        output << "    //  Raw embedded raw                ";
        output << "                                      //\n";
        output << "    ////////////////////////////////////";
        output << "////////////////////////////////////////\n";

        // Write data size
        output << "    const unsigned int RawDataSize = " <<
            dataSize << ";\n";
        output << "    const unsigned char RawData[" <<
            dataSize << "] =\n";
        output << "    {\n";
        output << "        ";

        // Write output embedded raw
        for (unsigned int i = 0; i < dataSize; ++i)
        {
            // Read input raw data
            char rawData = 0x00;
            if (!input.read(&rawData, 0x01))
            {
                // Could not read input data
                return false;
            }
            unsigned char data = static_cast<unsigned char>(rawData);

            // Convert raw data into hexadecimal string
            std::ostringstream oss;
            oss << std::hex << std::setfill('0') <<
                std::setw(2) << (int)data << std::dec;
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

            if (i == (dataSize-1))
            {
                // Last hexadecimal data
                output << "0x" << outStr << "\n    };\n\n\n";
            }
            else
            {
                // New line every 12 hexadecimal data
                if ((i+1) % 12 == 0)
                {
                    output << "0x" << outStr << ",\n        ";
                }
                else
                {
                    output << "0x" << outStr << ", ";
                }
            }
        }

        // Write .h protection
        output << "#endif // RAW_EMBEDDED_RAW_HEADER\n";

        // Close output file
        output.close();
    }

    // Close input file
    input.close();

    // Embraw successfully executed
    return true;
}
