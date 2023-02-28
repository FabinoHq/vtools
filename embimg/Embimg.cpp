////////////////////////////////////////////////////////////////////////////////
//  _______                       __________________________________________  //
//  \\ .   \            _________/ . . . . . . . . . . . . . . . . . . .   /  //
//   \\ .   \       ___/ . . . . .    ____________________________________/   //
//    \\ .   \   __/. . .   _________/  / \ // .    \ //.  ///. _______/      //
//     \\ .   \_//      ___/.  ____ ___/  ///.      ///.  ///. /____          //
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
//     Embimg.cpp : Embedded Image main class management                      //
////////////////////////////////////////////////////////////////////////////////
#include "Embimg.h"


////////////////////////////////////////////////////////////////////////////////
//  Embimg default constructor                                                //
////////////////////////////////////////////////////////////////////////////////
Embimg::Embimg()
{

}

////////////////////////////////////////////////////////////////////////////////
//  Embimg destructor                                                         //
////////////////////////////////////////////////////////////////////////////////
Embimg::~Embimg()
{

}


////////////////////////////////////////////////////////////////////////////////
//  Launch Embimg                                                             //
//  return : True if Embimg successfully started, false otherwise             //
////////////////////////////////////////////////////////////////////////////////
bool Embimg::launch(const std::string& filepath)
{
    // Check system CPU
    if (!SysCPUCheck())
    {
        // Invalid system CPU
        return false;
    }

    // Run Embimg
    return run(filepath);
}

////////////////////////////////////////////////////////////////////////////////
//  Run Embimg                                                                //
//  return : True if Embimg successfully executed                             //
////////////////////////////////////////////////////////////////////////////////
bool Embimg::run(const std::string& filepath)
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

    // Load input PNG file
    PNGFile img;
    if (!img.loadImage(filepath))
    {
        // Could not load input PNG file
        return false;
    }
    unsigned int imageSize = (img.getWidth()*img.getHeight()*4);
    unsigned char* pixelData = img.getImage();

    // Open output file
    std::ofstream output;
    output.open(filename, std::ios::out | std::ios::trunc);
    if (output.is_open())
    {
        // Write .h protection
        output << "#ifndef IMAGES_EMBEDDED_IMG_HEADER\n";
        output << "#define IMAGES_EMBEDDED_IMG_HEADER\n\n\n";

        // Write image description
        output << "    ////////////////////////////////////";
        output << "////////////////////////////////////////\n";
        output << "    //  Img embedded image              ";
        output << "                                      //\n";
        output << "    ////////////////////////////////////";
        output << "////////////////////////////////////////\n";

        // Write image size
        output << "    const unsigned int ImgImageWidth = " <<
            img.getWidth() << ";\n";
        output << "    const unsigned int ImgImageHeight = " <<
            img.getHeight() << ";\n";
        output << "    const unsigned char ImgImage[" <<
            img.getWidth() << " * " << img.getHeight() << " * 4] =\n";
        output << "    {\n";
        output << "        ";

        // Write output embedded image
        for (unsigned int i = 0; i < imageSize; ++i)
        {
            // Convert pixel data into hexadecimal string
            std::ostringstream oss;
            oss << std::hex << std::setfill('0') <<
                std::setw(2) << (int)pixelData[i] << std::dec;
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

            if (i == (imageSize-1))
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
        output << "#endif // IMAGES_EMBEDDED_IMG_HEADER\n";

        // Close output file
        output.close();
    }

    // Destroy PNG image
    img.destroyImage();

    // Embimg successfully executed
    return true;
}
