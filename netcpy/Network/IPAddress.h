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
//     Network/IPAddress.h : IP address management                            //
////////////////////////////////////////////////////////////////////////////////
#ifndef VTOOLS_NETCPY_NETWORK_IPADDRESS_HEADER
#define VTOOLS_NETCPY_NETWORK_IPADDRESS_HEADER

    #include "Network.h"

    #include <cstdint>
    #include <cstring>
    #include <string>


    ////////////////////////////////////////////////////////////////////////////
    //  IPAddress class definition                                            //
    ////////////////////////////////////////////////////////////////////////////
    class IPAddress
    {
        public:
            ////////////////////////////////////////////////////////////////////
            //  IPAddress default constructor                                 //
            ////////////////////////////////////////////////////////////////////
            IPAddress();

            ////////////////////////////////////////////////////////////////////
            //  IPAddress copy constructor                                    //
            ////////////////////////////////////////////////////////////////////
            IPAddress(const IPAddress& ipaddress);

            ////////////////////////////////////////////////////////////////////
            //  IPAddress destructor                                          //
            ////////////////////////////////////////////////////////////////////
            ~IPAddress();


            ////////////////////////////////////////////////////////////////////
            //  Set IP address v4 representation                              //
            ////////////////////////////////////////////////////////////////////
            inline void setAddress(in_addr ipaddress)
            {
                m_ipaddress.s_addr = ipaddress.s_addr;
            }

            ////////////////////////////////////////////////////////////////////
            //  Set IP address v4 integer                                     //
            ////////////////////////////////////////////////////////////////////
            inline void setInteger(uint32_t ipaddress)
            {
                m_ipaddress.s_addr = htonl(ipaddress);
            }

            ////////////////////////////////////////////////////////////////////
            //  Set IP address v4 string                                      //
            ////////////////////////////////////////////////////////////////////
            inline bool setString(const std::string& ipaddress)
            {
                return (inet_pton(AF_INET,ipaddress.c_str(),&m_ipaddress)==1);
            }


            ////////////////////////////////////////////////////////////////////
            //  Resolve host name into IP address                             //
            ////////////////////////////////////////////////////////////////////
            bool resolveHostName(const std::string& hostName);


            ////////////////////////////////////////////////////////////////////
            //  Get IP address v4 representation                              //
            ////////////////////////////////////////////////////////////////////
            inline in_addr getAddress() const
            {
                return m_ipaddress;
            }

            ////////////////////////////////////////////////////////////////////
            //  Get IP address v4 integer                                     //
            ////////////////////////////////////////////////////////////////////
            inline uint32_t getInteger() const
            {
                return ntohl(m_ipaddress.s_addr);
            }

            ////////////////////////////////////////////////////////////////////
            //  Get IP address v4 string                                      //
            ////////////////////////////////////////////////////////////////////
            std::string getString() const;


            ////////////////////////////////////////////////////////////////////
            //  IPAddress copy operator                                       //
            ////////////////////////////////////////////////////////////////////
            inline IPAddress& operator=(const IPAddress& ipaddress)
            {
                m_ipaddress.s_addr = ipaddress.m_ipaddress.s_addr;
                return *this;
            }

            ////////////////////////////////////////////////////////////////////
            //  IPAddress equal to operator                                   //
            ////////////////////////////////////////////////////////////////////
            inline bool operator==(const IPAddress& ipaddress)
            {
                return (m_ipaddress.s_addr == ipaddress.m_ipaddress.s_addr);
            }


        private:
            in_addr         m_ipaddress;        // IPAddress v4 representation
    };


#endif // VTOOLS_NETCPY_NETWORK_IPADDRESS_HEADER
