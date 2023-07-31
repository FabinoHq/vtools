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
//     Netcpy.h : Netcpy Main class management                                //
////////////////////////////////////////////////////////////////////////////////
#ifndef VTOOLS_NETCPY_NETCPY_HEADER
#define VTOOLS_NETCPY_NETCPY_HEADER

    #include "System/System.h"
    #include "System/SysMessage.h"
    #include "System/SysCPU.h"
    #include "System/SysCRC.h"
    #include "System/SysClock.h"
    #include "System/SysSleep.h"
    #include "System/SysSettings.h"
    #include "Network/Network.h"
    #include "Network/IPAddress.h"
    #include "Network/TCPSocket.h"

    #include <iostream>
    #include <string>
    #include <sstream>
    #include <fstream>
    #include <cstddef>
    #include <cstdint>
    #include <new>


    ////////////////////////////////////////////////////////////////////////////
    //  Netcpy constants                                                      //
    ////////////////////////////////////////////////////////////////////////////
    #define NETCPY_MAX_THREADS 0        // 8
    #define NETCPY_BUFFER_SIZE 1024
    #define NETCPY_MAXFILENAME_SIZE 512
    #define NETCPY_TRANSFER_BUFFER_SIZE 32768
    #define NETCPY_SEND_MAX_TRIES 128
    #define NETCPY_DISPLAY_UPDATE 0.5

    ////////////////////////////////////////////////////////////////////////////
    //  Netcpy TCP header                                                     //
    ////////////////////////////////////////////////////////////////////////////
    #define NETCPY_TCP_HEADER_SIZE 8
    const char NetcpyTcpHeader[NETCPY_TCP_HEADER_SIZE] = "netcpy1";
    const uint64_t NetcpyTcpHeader2 = 51421896512674ul;
    const uint64_t NetcpyTcpHeader3 = 17324814988423971542ul;


    ////////////////////////////////////////////////////////////////////////////
    //  Netcpy main class definition                                          //
    ////////////////////////////////////////////////////////////////////////////
    class Netcpy
    {
        public:
            ////////////////////////////////////////////////////////////////////
            //  Netcpy default constructor                                    //
            ////////////////////////////////////////////////////////////////////
            Netcpy();

            ////////////////////////////////////////////////////////////////////
            //  Netcpy destructor                                             //
            ////////////////////////////////////////////////////////////////////
            ~Netcpy();


            ////////////////////////////////////////////////////////////////////
            //  Launch netcpy                                                 //
            //  return : True if netcpy successfully started, false otherwise //
            ////////////////////////////////////////////////////////////////////
            bool launch(char* filepath);

            ////////////////////////////////////////////////////////////////////
            //  Run netcpy                                                    //
            ////////////////////////////////////////////////////////////////////
            bool run(char* filepath);

            ////////////////////////////////////////////////////////////////////
            //  Close netcpy                                                  //
            ////////////////////////////////////////////////////////////////////
            void close();


            ////////////////////////////////////////////////////////////////////
            //  Check input file                                              //
            ////////////////////////////////////////////////////////////////////
            bool checkInputFile(char* filepath);

            ////////////////////////////////////////////////////////////////////
            //  Check output file                                             //
            ////////////////////////////////////////////////////////////////////
            bool checkOutputFile();

            ////////////////////////////////////////////////////////////////////
            //  Compute file checksums                                        //
            ////////////////////////////////////////////////////////////////////
            bool fileChecksums();


            ////////////////////////////////////////////////////////////////////
            //  Run netcpy server                                             //
            ////////////////////////////////////////////////////////////////////
            bool server(char* filepath);

            ////////////////////////////////////////////////////////////////////
            //  Run netcpy client                                             //
            ////////////////////////////////////////////////////////////////////
            bool client(char* filepath);

            ////////////////////////////////////////////////////////////////////
            //  Handshake server                                              //
            ////////////////////////////////////////////////////////////////////
            bool handshakeServer(char* filepath);

            ////////////////////////////////////////////////////////////////////
            //  Handshake client                                              //
            ////////////////////////////////////////////////////////////////////
            bool handshakeClient(char* filepath);

            ////////////////////////////////////////////////////////////////////
            //  Transfer file                                                 //
            ////////////////////////////////////////////////////////////////////
            bool transferFile(char* filepath);


            ////////////////////////////////////////////////////////////////////
            //  PreSend data (prepare send buffer)                            //
            ////////////////////////////////////////////////////////////////////
            void preSend();

            ////////////////////////////////////////////////////////////////////
            //  Send data                                                     //
            ////////////////////////////////////////////////////////////////////
            bool send();

            ////////////////////////////////////////////////////////////////////
            //  Send file buffer                                              //
            ////////////////////////////////////////////////////////////////////
            bool sendFilebuf(size_t bufSize);

            ////////////////////////////////////////////////////////////////////
            //  Receive data                                                  //
            ////////////////////////////////////////////////////////////////////
            bool receive();

            ////////////////////////////////////////////////////////////////////
            //  Receive file buffer                                           //
            ////////////////////////////////////////////////////////////////////
            bool receiveFilebuf(size_t bufSize);


            ////////////////////////////////////////////////////////////////////
            //  Display file transfer progress                                //
            ////////////////////////////////////////////////////////////////////
            void displayProgress(double percents, double bitrate);


        private:
            ////////////////////////////////////////////////////////////////////
            //  Netcpy private copy constructor : Not copyable                //
            ////////////////////////////////////////////////////////////////////
            Netcpy(const Netcpy&) = delete;

            ////////////////////////////////////////////////////////////////////
            //  Netcpy private copy operator : Not copyable                   //
            ////////////////////////////////////////////////////////////////////
            Netcpy& operator=(const Netcpy&) = delete;


        private:
            TCPSocket           m_localSock;            // Local TCP socket
            TCPSocket           m_distantSock;          // Distant TCP socket
            IPAddress           m_distantAddr;          // Distant address
            uint16_t            m_startingPort;         // Starting port
            char*               m_buffer;               // Buffer

            bool                m_sending;              // Send or receive
            std::string         m_fileName;             // File name
            size_t              m_fileSize;             // File size
            uint32_t            m_crc32;                // CRC 32
            uint32_t            m_adler32;              // Adler 32

            SysClock            m_updateTime;           // Update time
            SysClock            m_totalTime;            // Total transfer time
            double              m_currentTime;          // Current transfer time
            double              m_displayTime;          // Display update time
    };


#endif // VTOOLS_NETCPY_NETCPY_HEADER
