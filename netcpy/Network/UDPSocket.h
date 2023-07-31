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
//     Network/UDPSocket.h : UDP Socket management                            //
////////////////////////////////////////////////////////////////////////////////
#ifndef VTOOLS_NETCPY_NETWORK_UDPSOCKET_HEADER
#define VTOOLS_NETCPY_NETWORK_UDPSOCKET_HEADER

    #include "Network.h"
    #include "IPAddress.h"

    #include <cstddef>
    #include <cstdint>
    #include <cstring>


    ////////////////////////////////////////////////////////////////////////////
    //  UDPSocket invalid socket constant                                     //
    ////////////////////////////////////////////////////////////////////////////
    #ifdef VTOOLS_NETCPY_WINDOWS
        const SocketHandle UDPSocketInvalid = INVALID_SOCKET;
    #endif // VTOOLS_NETCPY_WINDOWS
    #ifdef VTOOLS_NETCPY_LINUX
        const SocketHandle UDPSocketInvalid = -1;
    #endif // VTOOLS_NETCPY_LINUX

    ////////////////////////////////////////////////////////////////////////////
    //  UDPSocket maximum back log (pending connections queue length)         //
    ////////////////////////////////////////////////////////////////////////////
    const int UDPSocketMaxBackLog = SOMAXCONN;


    ////////////////////////////////////////////////////////////////////////////
    //  UDPSocket class definition                                            //
    ////////////////////////////////////////////////////////////////////////////
    class UDPSocket
    {
        public:
            ////////////////////////////////////////////////////////////////////
            //  UDPSocket default constructor                                 //
            ////////////////////////////////////////////////////////////////////
            UDPSocket();

            ////////////////////////////////////////////////////////////////////
            //  UDPSocket destructor                                          //
            ////////////////////////////////////////////////////////////////////
            ~UDPSocket();


            ////////////////////////////////////////////////////////////////////
            //  Create UDP Socket                                             //
            ////////////////////////////////////////////////////////////////////
            bool createSocket(bool blocking = true);

            ////////////////////////////////////////////////////////////////////
            //  Close UDP Socket                                              //
            ////////////////////////////////////////////////////////////////////
            void closeSocket();


            ////////////////////////////////////////////////////////////////////
            //  Bind UDP Socket to any IP address                             //
            ////////////////////////////////////////////////////////////////////
            bool bindSocket(uint16_t port);

            ////////////////////////////////////////////////////////////////////
            //  Bind UDP Socket to IP address                                 //
            ////////////////////////////////////////////////////////////////////
            bool bindSocket(IPAddress& ipaddress, uint16_t port);


            ////////////////////////////////////////////////////////////////////
            //  Send data over UDP                                            //
            ////////////////////////////////////////////////////////////////////
            bool sendData(const char* data, size_t size,
                IPAddress& ipaddress, uint16_t port);

            ////////////////////////////////////////////////////////////////////
            //  Receive data over UDP                                         //
            ////////////////////////////////////////////////////////////////////
            bool receiveData(char* data, size_t& size,
                IPAddress& ipaddress, uint16_t port);


        private:
            ////////////////////////////////////////////////////////////////////
            //  UDPSocket private copy constructor : Not copyable             //
            ////////////////////////////////////////////////////////////////////
            UDPSocket(const UDPSocket&) = delete;

            ////////////////////////////////////////////////////////////////////
            //  UDPSocket private copy operator : Not copyable                //
            ////////////////////////////////////////////////////////////////////
            UDPSocket& operator=(const UDPSocket&) = delete;


        private:
            SocketHandle            m_handle;       // UDPSocket handle
    };


#endif // VTOOLS_NETCPY_NETWORK_UDPSOCKET_HEADER
