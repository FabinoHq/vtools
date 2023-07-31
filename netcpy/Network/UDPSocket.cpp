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
//     Network/UDPSocket.cpp : UDP Socket management                          //
////////////////////////////////////////////////////////////////////////////////
#include "UDPSocket.h"


////////////////////////////////////////////////////////////////////////////////
//  UDPSocket default constructor                                             //
////////////////////////////////////////////////////////////////////////////////
UDPSocket::UDPSocket() :
m_handle(UDPSocketInvalid)
{

}

////////////////////////////////////////////////////////////////////////////////
//  UDPSocket destructor                                                      //
////////////////////////////////////////////////////////////////////////////////
UDPSocket::~UDPSocket()
{
    closeSocket();
}


////////////////////////////////////////////////////////////////////////////////
//  Create UDP Socket                                                         //
////////////////////////////////////////////////////////////////////////////////
bool UDPSocket::createSocket(bool blocking)
{
    // Close current socket
    closeSocket();

    // IPv4
    int addrfamily = AF_INET;
    // IPv6
    //addrfamily = AF_INET6;

    // Create socket
    m_handle = socket(addrfamily, SOCK_DGRAM, IPPROTO_UDP);
    if (m_handle == UDPSocketInvalid)
    {
        // Unable to create socket
        return false;
    }

    // Set socket blocking state
    #ifdef VTOOLS_NETCPY_WINDOWS

        unsigned long blockMode = (blocking ? 0 : 1);
        if (ioctlsocket(m_handle, FIONBIO, &blockMode) != 0)
        {
            // Could not set socket blocking state
            return false;
        }

    #endif // VTOOLS_NETCPY_WINDOWS
    #ifdef VTOOLS_NETCPY_LINUX

        int statusFlags = fcntl(m_handle, F_GETFL);
        if (blocking) { statusFlags &= ~O_NONBLOCK; }
        else { statusFlags |= O_NONBLOCK; }
        if (fcntl(m_handle, F_SETFL, statusFlags) != 0)
        {
            // Could not set socket blocking state
            return false;
        }

    #endif // VTOOLS_NETCPY_LINUX

    // UDP Socket successfully created
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Close UDP Socket                                                          //
////////////////////////////////////////////////////////////////////////////////
void UDPSocket::closeSocket()
{
    if (m_handle != UDPSocketInvalid)
    {
        #ifdef VTOOLS_NETCPY_WINDOWS
            closesocket(m_handle);
        #endif // VTOOLS_NETCPY_WINDOWS
        #ifdef VTOOLS_NETCPY_LINUX
            close(m_handle);
        #endif // VTOOLS_NETCPY_LINUX
    }
    m_handle = UDPSocketInvalid;
}


////////////////////////////////////////////////////////////////////////////////
//  Bind UDP Socket to any IP address                                         //
////////////////////////////////////////////////////////////////////////////////
bool UDPSocket::bindSocket(uint16_t port)
{
    // Check socket handle
    if (m_handle == UDPSocketInvalid)
    {
        // Invalid socket handle
        return false;
    }

    // IPv4
    sockaddr_in addr4;
    addr4.sin_family = AF_INET;
    addr4.sin_port = htons(port);
    addr4.sin_addr.s_addr = htonl(INADDR_ANY);
    addr4.sin_zero[0] = 0;
    addr4.sin_zero[1] = 0;
    addr4.sin_zero[2] = 0;
    addr4.sin_zero[3] = 0;
    addr4.sin_zero[4] = 0;
    addr4.sin_zero[5] = 0;
    addr4.sin_zero[6] = 0;
    addr4.sin_zero[7] = 0;

    // Bind socket
    if (bind(m_handle, (sockaddr*)&addr4, sizeof(addr4)) != 0)
    {
        // Could not bind socket
        return false;
    }

    // IPv6
    /*sockaddr_in6 addr6;
    addr6.sin6_family = AF_INET6;
    addr6.sin6_port = htons(port);
    addr6.sin6_flowinfo = 0;
    addr6.sin6_addr = in6addr_any;
    addr6.sin6_scope_id = 0;

    // Bind socket
    if (bind(m_handle, (sockaddr*)&addr6, sizeof(addr6)) != 0)
    {
        // Could not bind socket
        return false;
    }*/

    // UDP Socket successfully bound
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Bind UDP Socket to IP address                                             //
////////////////////////////////////////////////////////////////////////////////
bool UDPSocket::bindSocket(IPAddress& ipaddress, uint16_t port)
{
    // Check socket handle
    if (m_handle == UDPSocketInvalid)
    {
        // Invalid socket handle
        return false;
    }

    // IPv4
    sockaddr_in addr4;
    addr4.sin_family = AF_INET;
    addr4.sin_port = htons(port);
    addr4.sin_addr = ipaddress.getAddress();
    addr4.sin_zero[0] = 0;
    addr4.sin_zero[1] = 0;
    addr4.sin_zero[2] = 0;
    addr4.sin_zero[3] = 0;
    addr4.sin_zero[4] = 0;
    addr4.sin_zero[5] = 0;
    addr4.sin_zero[6] = 0;
    addr4.sin_zero[7] = 0;

    // Bind socket
    if (bind(m_handle, (sockaddr*)&addr4, sizeof(addr4)) != 0)
    {
        // Could not bind socket
        return false;
    }

    // IPv6
    /*sockaddr_in6 addr6;
    addr6.sin6_family = AF_INET6;
    addr6.sin6_port = htons(port);
    addr6.sin6_flowinfo = 0;
    addr6.sin6_addr = ipaddress.getAddress();
    addr6.sin6_scope_id = 0;

    // Bind socket
    if (bind(m_handle, (sockaddr*)&addr6, sizeof(addr6)) != 0)
    {
        // Could not bind socket
        return false;
    }*/

    // UDP Socket successfully bound
    return true;
}


////////////////////////////////////////////////////////////////////////////////
//  Send data over UDP                                                        //
////////////////////////////////////////////////////////////////////////////////
bool UDPSocket::sendData(const char* data, size_t size,
    IPAddress& ipaddress, uint16_t port)
{
    // Check socket handle
    if (m_handle == UDPSocketInvalid)
    {
        // Invalid socket handle
        return false;
    }

    // Check data
    if (!data || (size <= 0))
    {
        // Invalid data
        return false;
    }

    // Remote address (IPv4)
    sockaddr_in addr4;
    addr4.sin_family = AF_INET;
    addr4.sin_port = htons(port);
    addr4.sin_addr = ipaddress.getAddress();
    addr4.sin_zero[0] = 0;
    addr4.sin_zero[1] = 0;
    addr4.sin_zero[2] = 0;
    addr4.sin_zero[3] = 0;
    addr4.sin_zero[4] = 0;
    addr4.sin_zero[5] = 0;
    addr4.sin_zero[6] = 0;
    addr4.sin_zero[7] = 0;

    // Remote address (IPv6)
    /*sockaddr_in6 addr6;
    addr6.sin6_family = AF_INET6;
    addr6.sin6_port = htons(port);
    addr6.sin6_flowinfo = 0;
    addr6.sin6_addr = ipaddress.getAddress();
    addr6.sin6_scope_id = 0;*/

    // Send data
    size_t dataSent = 0;
    int result = 0;
    do
    {
        result = sendto(
            m_handle, &data[dataSent],
            static_cast<int>(sizeof(char)*(size-dataSent)),
            0, (sockaddr*)&addr4, sizeof(addr4)
        );
        if (result < 0)
        {
            // Unable to send data
            return false;
        }
        dataSent += static_cast<size_t>(result);
    } while (dataSent < size);

    // Data successfully sent
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Receive data over UDP                                                     //
////////////////////////////////////////////////////////////////////////////////
bool UDPSocket::receiveData(char* data, size_t& size,
    IPAddress& ipaddress, uint16_t port)
{
    // Check socket handle
    if (m_handle == UDPSocketInvalid)
    {
        // Invalid socket handle
        size = 0;
        return false;
    }

    // Check data
    if (!data || (size <= 0))
    {
        // Invalid data
        size = 0;
        return false;
    }

    // Remote address (IPv4)
    sockaddr_in addr4;
    addr4.sin_family = AF_INET6;
    addr4.sin_port = htons(port);
    addr4.sin_addr.s_addr = 0;
    addr4.sin_zero[0] = 0;
    addr4.sin_zero[1] = 0;
    addr4.sin_zero[2] = 0;
    addr4.sin_zero[3] = 0;
    addr4.sin_zero[4] = 0;
    addr4.sin_zero[5] = 0;
    addr4.sin_zero[6] = 0;
    addr4.sin_zero[7] = 0;
    socklen_t addr4Size = sizeof(addr4);

    // Remote address (IPv6)
    /*sockaddr_in6 addr6;
    addr6.sin6_family = AF_INET6;
    addr6.sin6_port = htons(port);
    addr6.sin6_flowinfo = 0;
    #ifdef VTOOLS_NETCPY_WINDOWS
        addr6.sin6_addr.u.Word[0] = 0;
        addr6.sin6_addr.u.Word[1] = 0;
        addr6.sin6_addr.u.Word[2] = 0;
        addr6.sin6_addr.u.Word[3] = 0;
        addr6.sin6_addr.u.Word[4] = 0;
        addr6.sin6_addr.u.Word[5] = 0;
        addr6.sin6_addr.u.Word[6] = 0;
        addr6.sin6_addr.u.Word[7] = 0;
    #endif // VTOOLS_NETCPY_WINDOWS
    #ifdef VTOOLS_NETCPY_LINUX
        addr6.sin6_addr.s6_addr16[0] = 0;
        addr6.sin6_addr.s6_addr16[1] = 0;
        addr6.sin6_addr.s6_addr16[2] = 0;
        addr6.sin6_addr.s6_addr16[3] = 0;
        addr6.sin6_addr.s6_addr16[4] = 0;
        addr6.sin6_addr.s6_addr16[5] = 0;
        addr6.sin6_addr.s6_addr16[6] = 0;
        addr6.sin6_addr.s6_addr16[7] = 0;
    #endif // VTOOLS_NETCPY_LINUX
    addr6.sin6_scope_id = 0;
    socklen_t addr6Size = sizeof(addr6);*/

    // Receive data
    int result = recvfrom(m_handle, data,
        static_cast<int>(sizeof(char)*size),
        0, (sockaddr*)&addr4, &addr4Size
    );
    if (result <= 0)
    {
        // Unable to receive data
        size = 0;
        return false;
    }

    // Copy remote IP address (IPv4)
    ipaddress.setAddress(addr4.sin_addr);
    // Copy remote IP address (IPv6)
    //ipaddress.setAddress(addr6.sin6_addr);

    // Data successfully received
    size = result;
    return true;
}
