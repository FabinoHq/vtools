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
//     Network/Network.h : Netcpy Network management                          //
////////////////////////////////////////////////////////////////////////////////
#ifndef VTOOLS_NETCPY_NETWORK_NETWORK_HEADER
#define VTOOLS_NETCPY_NETWORK_NETWORK_HEADER

    #include "../System/System.h"


    ////////////////////////////////////////////////////////////////////////////
    //  Windows network management                                            //
    ////////////////////////////////////////////////////////////////////////////
    #ifdef VTOOLS_NETCPY_WINDOWS

        #include <winsock2.h>
        #include <Ws2tcpip.h>

        #define SocketHandle UINT_PTR

    #endif // VTOOLS_NETCPY_WINDOWS

    ////////////////////////////////////////////////////////////////////////////
    //  Linux network management                                              //
    ////////////////////////////////////////////////////////////////////////////
    #ifdef VTOOLS_NETCPY_LINUX

        #include <sys/socket.h>
        #include <netinet/in.h>
        #include <netinet/tcp.h>
        #include <netinet/udp.h>
        #include <arpa/inet.h>
        #include <netdb.h>
        #include <fcntl.h>
        #include <unistd.h>

        #define SocketHandle int

    #endif // VTOOLS_NETCPY_LINUX


#endif // VTOOLS_NETCPY_NETWORK_NETWORK_HEADER
