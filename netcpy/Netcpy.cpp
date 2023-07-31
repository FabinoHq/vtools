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
//     Netcpy.cpp : Netcpy Main class management                              //
////////////////////////////////////////////////////////////////////////////////
#include "Netcpy.h"


////////////////////////////////////////////////////////////////////////////////
//  Netcpy default constructor                                                //
////////////////////////////////////////////////////////////////////////////////
Netcpy::Netcpy() :
m_localSock(),
m_distantSock(),
m_distantAddr(),
m_startingPort(0),
m_fileName(""),
m_fileSize(0),
m_crc32(0),
m_adler32(0),
m_buffer(0),
m_bufSize(0),
m_sending(false),
m_updateTime(),
m_totalTime(),
m_currentTime(0.0),
m_displayTime(0.0)
{

}

////////////////////////////////////////////////////////////////////////////////
//  Netcpy destructor                                                         //
////////////////////////////////////////////////////////////////////////////////
Netcpy::~Netcpy()
{
    if (m_buffer) { delete[] m_buffer; }
    m_buffer = 0;
}


////////////////////////////////////////////////////////////////////////////////
//  Launch netcpy                                                             //
//  return : True if netcpy successfully started, false otherwise             //
////////////////////////////////////////////////////////////////////////////////
bool Netcpy::launch(char* filepath)
{
    // Launch netcpy
    std::cout << "VTools : netcpy\n";

    // Check system CPU
    if (!SysCPUCheck())
    {
        // Invalid system CPU
        return false;
    }

    // Allocate buffer
    size_t bufferSize = NETCPY_BUFFER_SIZE;
    if (NETCPY_TRANSFER_BUFFER_SIZE >= bufferSize)
    {
        bufferSize = NETCPY_TRANSFER_BUFFER_SIZE;
    }
    m_buffer = new (std::nothrow) char[bufferSize];
    if (!m_buffer)
    {
        std::cout << "Could not allocated buffer\n";
        return false;
    }
    memset(m_buffer, 0, bufferSize);

    // Run netcpy
    if (!run(filepath))
    {
        // Netcpy error
        std::string eof;
        std::getline(std::cin, eof);
        return false;
    }

    // Netcpy successfully terminated
    std::string eof;
    std::getline(std::cin, eof);
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Run netcpy                                                                //
////////////////////////////////////////////////////////////////////////////////
bool Netcpy::run(char* filepath)
{
    // Get host address
    std::string hostStr;
    if (!filepath)
    {
        std::cout << "Receive a file\n";
    }
    else
    {
        std::cout << "Send : " << filepath << "\n";
    }
    std::cout << "Host Address (leave empty for server) :\n";
    std::getline(std::cin, hostStr);

    // Get starting port
    std::string portStr;
    std::cout << "Starting port :\n";
    std::getline(std::cin, portStr);
    std::cout << '\n';
    std::istringstream iss(portStr);
    int64_t portInt = 0;
    iss >> portInt;
    if ((portInt <= 0) || (portInt > (65535-NETCPY_MAX_THREADS)))
    {
        std::cout << "Invalid port\n";
        return false;
    }
    if (portInt <= 0) { portInt = 0; }
    if (portInt >= 65535) { portInt = 65535; }
    m_startingPort = static_cast<uint16_t>(portInt);

    // Start netcpy
    if (hostStr.empty())
    {
        // Server
        std::cout << "Netcpy server\n";
        std::cout << "Using ports : [" << m_startingPort << "] to [" <<
            (m_startingPort+NETCPY_MAX_THREADS) << "] included\n";

        if (filepath) { if (!checkInputFile(filepath)) { return false; } }
        return server(filepath);
    }
    else
    {
        // Client
        std::cout << "Checking IP : " << hostStr << '\n';
        if (!m_distantAddr.setString(hostStr))
        {
            std::cout << "Resolving hostname...\n";
            if (!m_distantAddr.resolveHostName(hostStr))
            {
                std::cout << "Cannot resolve host IP address\n";
                return false;
            }
        }
        std::cout << "Netcpy client\n";
        std::cout << "Host Address : " << m_distantAddr.getString() << '\n';
        std::cout << "Using ports : [" << m_startingPort << "] to [" <<
            (m_startingPort+NETCPY_MAX_THREADS) << "] included\n";

        if (filepath) { if (!checkInputFile(filepath)) { return false; } }
        return client(filepath);
    }

    // Netcpy successfully executed
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Close netcpy                                                              //
////////////////////////////////////////////////////////////////////////////////
void Netcpy::close()
{
    m_distantSock.closeSocket();
    m_localSock.closeSocket();
}


////////////////////////////////////////////////////////////////////////////////
//  Check input file                                                          //
////////////////////////////////////////////////////////////////////////////////
bool Netcpy::checkInputFile(char* filepath)
{
    // Check filepath
    if (!filepath)
    {
        // Invalid filepath
        return false;
    }

    // Check file
    std::ifstream file;
    file.open(filepath, std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        std::cout << "Could not read file : " << filepath << '\n';
        return false;
    }

    // Check file size
    file.seekg(0, std::ios::end);
    m_fileSize = file.tellg();
    file.close();
    if (m_fileSize <= 0)
    {
        // Invalid file size
        std::cout << "Invalid file size : " << filepath << '\n';
        return false;
    }

    // Extract file name
    int filepathLen = static_cast<int>(strlen(filepath));
    int i;
    for (i = (filepathLen-1); i > 0; --i)
    {
        if ((filepath[i] == '/') || (filepath[i] == '\\')) { break; }
    }

    if (i == 0)
    {
        m_fileName = std::string(filepath);
    }
    else
    {
        m_fileName = std::string(&filepath[i+1]);
    }

    if (m_fileName.empty())
    {
        std::cout << "Invalid file name\n";
        return false;
    }
    if (m_fileName.length() >= NETCPY_MAXFILENAME_SIZE)
    {
        std::cout << "File name is too long\n";
        return false;
    }

    // Compute file checksums
    std::cout << "Compute file checksums...\n";
    if (!fileChecksums())
    {
        std::cout << "Could not compute file checksums\n";
        return false;
    }

    // File is valid
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Check output file                                                         //
////////////////////////////////////////////////////////////////////////////////
bool Netcpy::checkOutputFile()
{
    // Check file
    std::ifstream file;
    file.open(m_fileName, std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        // Output file is valid
        return true;
    }
    file.close();

    // Try another files
    for (int i = 0; i < 100; ++i)
    {
        std::ostringstream currentName;
        currentName << m_fileName;
        currentName << ".";
        currentName << i;
        file.open(currentName.str(), std::ios::in | std::ios::binary);
        if (!file.is_open())
        {
            // Output file is valid
            m_fileName = currentName.str();
            return true;
        }
        file.close();
    }

    // Invalid output file
    return false;
}

////////////////////////////////////////////////////////////////////////////////
//  Compute file checksums                                                    //
////////////////////////////////////////////////////////////////////////////////
bool Netcpy::fileChecksums()
{
    // Check file
    std::ifstream file;
    file.open(m_fileName, std::ios::in | std::ios::binary);
    if (!file.is_open())
    {
        // Invalid file name
        return false;
    }

    // Compute file checksums
    size_t chunksCnt = (m_fileSize / NETCPY_TRANSFER_BUFFER_SIZE);
    size_t trailCnt = (m_fileSize % NETCPY_TRANSFER_BUFFER_SIZE);
    m_crc32 = SysCRC32Default;
    m_adler32 = SysAdler32Default;

    // File chunks
    for (size_t i = 0; i < chunksCnt; ++i)
    {
        file.read(m_buffer, NETCPY_TRANSFER_BUFFER_SIZE);
        m_crc32 = SysUpdateCRC32(
            m_crc32, (unsigned char*)m_buffer, NETCPY_TRANSFER_BUFFER_SIZE
        );
        m_adler32 = SysUpdateAdler32(
            m_adler32, (unsigned char*)m_buffer, NETCPY_TRANSFER_BUFFER_SIZE
        );
    }

    // File trail
    if (trailCnt > 0)
    {
        file.read(m_buffer, trailCnt);
        m_crc32 = SysUpdateCRC32(
            m_crc32, (unsigned char*)m_buffer, trailCnt
        );
        m_adler32 = SysUpdateAdler32(
            m_adler32, (unsigned char*)m_buffer, trailCnt
        );
    }
    m_crc32 ^= SysCRC32Final;

    // File checksums successfully computed
    file.close();
    return true;
}


////////////////////////////////////////////////////////////////////////////////
//  Run netcpy server                                                         //
////////////////////////////////////////////////////////////////////////////////
bool Netcpy::server(char* filepath)
{
    // Start server
    if (!m_localSock.createSocket())
    {
        std::cout << "TCP Socket creation error\n";
        return false;
    }

    if (!m_localSock.bindSocket(m_startingPort))
    {
        std::cout << "TCP Socket bind error\n";
        return false;
    }

    if (!m_localSock.listenPort())
    {
        std::cout << "TCP Socket listen error\n";
        close();
        return false;
    }

    // Wait for a client to connect
    std::cout << "Waiting for a client to connect...\n";
    if (!m_localSock.acceptConnection(m_distantSock, m_distantAddr))
    {
        std::cout << "Error while accepting client connection\n";
        close();
        return false;
    }

    // Client connected
    std::cout << "Client connected : " <<
        m_distantAddr.getString() << "\n\n";

    // Handshake
    if (!handshakeServer(filepath))
    {
        std::cout << "Server handshake failed\n";
        close();
        return false;
    }

    // Transfer file
    if (!transferFile(filepath))
    {
        std::cout << "File transfer failed\n";
        close();
        return false;
    }

    // Close network connection
    close();

    // Verify checksums
    if (!m_sending)
    {
        std::cout << "Compute file checksums...\n";
        uint32_t crc32 = m_crc32;
        uint32_t adler32 = m_adler32;
        if (!fileChecksums())
        {
            std::cout << "Could not compute file checksums\n";
            return false;
        }
        if (m_crc32 != crc32)
        {
            std::cout << "Invalid CRC32\n";
            return false;
        }
        if (m_adler32 != adler32)
        {
            std::cout << "Invalid Adler32\n";
            return false;
        }
        std::cout << "File checksums are valids\n";
    }

    // Netcpy server successfully executed
    close();
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Run netcpy client                                                         //
////////////////////////////////////////////////////////////////////////////////
bool Netcpy::client(char* filepath)
{
    // Start client
    if (!m_distantSock.createSocket())
    {
        std::cout << "TCP Socket creation error\n";
        return false;
    }

    // Connect to distant address
    if (!m_distantSock.connectSocket(m_distantAddr, m_startingPort))
    {
        std::cout << "Could not connect to host : " <<
            m_distantAddr.getString() << " on port : " <<
            m_startingPort << '\n';
        return false;
    }

    // Client connected
    std::cout << "Connected to server : " <<
        m_distantAddr.getString() << "\n\n";

    // Handshake
    if (!handshakeClient(filepath))
    {
        std::cout << "Client handshake failed\n";
        close();
        return false;
    }

    // Transfer file
    if (!transferFile(filepath))
    {
        std::cout << "File transfer failed\n";
        close();
        return false;
    }

    // Close network connection
    close();

    // Verify checksums
    if (!m_sending)
    {
        std::cout << "Compute file checksums...\n";
        uint32_t crc32 = m_crc32;
        uint32_t adler32 = m_adler32;
        if (!fileChecksums())
        {
            std::cout << "Could not compute file checksums\n";
            return false;
        }
        if (m_crc32 != crc32)
        {
            std::cout << "Invalid CRC32\n";
            return false;
        }
        if (m_adler32 != adler32)
        {
            std::cout << "Invalid Adler32\n";
            return false;
        }
        std::cout << "File checksums are valids\n";
    }

    // Netcpy client successfully executed
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Handshake server                                                          //
////////////////////////////////////////////////////////////////////////////////
bool Netcpy::handshakeServer(char* filepath)
{
    // First TCP header
    if (!receive()) { close(); return false; }
    uint64_t cliTcpHeader2 = 0;
    uint64_t cliTcpHeader3 = 0;
    memcpy(
        &cliTcpHeader2, &m_buffer[NETCPY_TCP_HEADER_SIZE], sizeof(uint64_t)
    );
    memcpy(
        &cliTcpHeader3, &m_buffer[NETCPY_TCP_HEADER_SIZE+8], sizeof(uint64_t)
    );
    if ((cliTcpHeader2 != NetcpyTcpHeader2) ||
        (cliTcpHeader2 != NetcpyTcpHeader2))
    {
        std::cout << "Netcpy TCP headers (uint64_t values) error\n";
        close();
        return false;
    }

    // "Send or receive" state from client
    if (m_buffer[NETCPY_TCP_HEADER_SIZE+16] == 1)
    {
        if (!filepath)
        {
            // File size
            m_fileSize = 0;
            uint64_t filesize = 0;
            memcpy(
                &filesize, &m_buffer[NETCPY_TCP_HEADER_SIZE+17],
                sizeof(uint64_t)
            );
            m_fileSize = static_cast<size_t>(filesize);
            // File checksums
            memcpy(
                &m_crc32, &m_buffer[NETCPY_TCP_HEADER_SIZE+25],
                sizeof(uint32_t)
            );
            memcpy(
                &m_adler32, &m_buffer[NETCPY_TCP_HEADER_SIZE+29],
                sizeof(uint32_t)
            );
            // File name length
            uint64_t filenameLen = 0;
            memcpy(
                &filenameLen, &m_buffer[NETCPY_TCP_HEADER_SIZE+33],
                sizeof(uint64_t)
            );
            // File name
            m_fileName.resize(filenameLen);
            memcpy(
                m_fileName.data(), &m_buffer[NETCPY_TCP_HEADER_SIZE+41],
                filenameLen
            );

            // Check file receive
            if (!checkOutputFile())
            {
                std::cout << "Error : file already exists (" <<
                    m_fileName << ")\n";
                close();
                return false;
            }

            // Receive mode
            m_sending = false;
        }
        else
        {
            std::cout << "Error : Too many files to transfer\n";
            close();
            return false;
        }
    }
    else
    {
        if (!filepath)
        {
            std::cout << "Error : No file to transfer\n";
            close();
            return false;
        }
        else
        {
            // Sending mode
            m_sending = true;
        }
    }

    // File transfer infos to client
    preSend();
    m_buffer[NETCPY_TCP_HEADER_SIZE+16] = 0;
    if (m_sending)
    {
        m_buffer[NETCPY_TCP_HEADER_SIZE+16] = 1;
        // File size
        uint64_t filesize = static_cast<uint64_t>(m_fileSize);
        memcpy(
            &m_buffer[NETCPY_TCP_HEADER_SIZE+17],
            &filesize, sizeof(uint64_t)
        );
        // File checksums
        memcpy(
            &m_buffer[NETCPY_TCP_HEADER_SIZE+25],
            &m_crc32, sizeof(uint32_t)
        );
        memcpy(
            &m_buffer[NETCPY_TCP_HEADER_SIZE+29],
            &m_adler32, sizeof(uint32_t)
        );
        // File name length
        uint64_t filenameLen = static_cast<uint64_t>(m_fileName.length());
        memcpy(
            &m_buffer[NETCPY_TCP_HEADER_SIZE+33],
            &filenameLen, sizeof(uint64_t)
        );
        // File name
        memcpy(
            &m_buffer[NETCPY_TCP_HEADER_SIZE+41],
            m_fileName.data(), filenameLen
        );
    }
    if (!send()) { close(); return false; }

    // Handshake server successfully executed
    return true;
}

////////////////////////////////////////////////////////////////////////////////
// Handshake client                                                           //
////////////////////////////////////////////////////////////////////////////////
bool Netcpy::handshakeClient(char* filepath)
{
    // First TCP header
    preSend();
    memcpy(
        &m_buffer[NETCPY_TCP_HEADER_SIZE], &NetcpyTcpHeader2, sizeof(uint64_t)
    );
    memcpy(
        &m_buffer[NETCPY_TCP_HEADER_SIZE+8], &NetcpyTcpHeader3, sizeof(uint64_t)
    );

    // "Send or receive" state to server
    m_buffer[NETCPY_TCP_HEADER_SIZE+16] = 0;
    if (filepath)
    {
        m_buffer[NETCPY_TCP_HEADER_SIZE+16] = 1;
        // File size
        uint64_t filesize = static_cast<uint64_t>(m_fileSize);
        memcpy(
            &m_buffer[NETCPY_TCP_HEADER_SIZE+17],
            &filesize, sizeof(uint64_t)
        );
        // File checksums
        memcpy(
            &m_buffer[NETCPY_TCP_HEADER_SIZE+25],
            &m_crc32, sizeof(uint32_t)
        );
        memcpy(
            &m_buffer[NETCPY_TCP_HEADER_SIZE+29],
            &m_adler32, sizeof(uint32_t)
        );
        // File name length
        uint64_t filenameLen = static_cast<uint64_t>(m_fileName.length());
        memcpy(
            &m_buffer[NETCPY_TCP_HEADER_SIZE+33],
            &filenameLen, sizeof(uint64_t)
        );
        // File name
        memcpy(
            &m_buffer[NETCPY_TCP_HEADER_SIZE+41],
            m_fileName.data(), filenameLen
        );
    }
    if (!send()) { close(); return false; }

    // File transfer infos from server
    if (!receive()) { close(); return false; }
    if (m_buffer[NETCPY_TCP_HEADER_SIZE+16] == 1)
    {
        if (!filepath)
        {
            // File size
            m_fileSize = 0;
            uint64_t filesize = 0;
            memcpy(
                &filesize, &m_buffer[NETCPY_TCP_HEADER_SIZE+17],
                sizeof(uint64_t)
            );
            m_fileSize = static_cast<size_t>(filesize);
            // File checksums
            memcpy(
                &m_crc32, &m_buffer[NETCPY_TCP_HEADER_SIZE+25],
                sizeof(uint32_t)
            );
            memcpy(
                &m_adler32, &m_buffer[NETCPY_TCP_HEADER_SIZE+29],
                sizeof(uint32_t)
            );
            // File name length
            uint64_t filenameLen = 0;
            memcpy(
                &filenameLen, &m_buffer[NETCPY_TCP_HEADER_SIZE+33],
                sizeof(uint64_t)
            );
            // File name
            m_fileName.resize(filenameLen);
            memcpy(
                m_fileName.data(), &m_buffer[NETCPY_TCP_HEADER_SIZE+41],
                filenameLen
            );

            // Check file receive
            if (!checkOutputFile())
            {
                std::cout << "Error : file already exists (" <<
                    m_fileName << ")\n";
                close();
                return false;
            }

            // Receive mode
            m_sending = false;
        }
        else
        {
            std::cout << "Error : Too many files to transfer\n";
            close();
            return false;
        }
    }
    else
    {
        if (!filepath)
        {
            std::cout << "Error : No file to transfer\n";
            close();
            return false;
        }
        else
        {
            // Sending mode
            m_sending = true;
        }
    }

    // Handshake client successfully executed
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Transfer file                                                             //
////////////////////////////////////////////////////////////////////////////////
bool Netcpy::transferFile(char* filepath)
{
    // File transfer infos
    size_t chunksCnt = (m_fileSize / NETCPY_TRANSFER_BUFFER_SIZE);
    size_t trailCnt = (m_fileSize % NETCPY_TRANSFER_BUFFER_SIZE);
    std::cout << "Transfering file...\n";
    std::cout << "File : " << m_fileName << '\n';
    std::cout << "Size : " << m_fileSize << " bytes\n";
    std::cout << "Chunks : " << chunksCnt << '\n';
    std::cout << "Trail length : " << trailCnt << '\n';
    std::cout << "CRC32 : " << m_crc32 << '\n';
    std::cout << "Adler32 : " << m_adler32 << '\n';
    std::cout << "\n";

    // Reset timers
    m_totalTime.reset();
    m_updateTime.reset();
    m_currentTime = 0.0;
    m_displayTime = 0.0;

    // Transfer file
    if (m_sending)
    {
        // Send file
        std::ifstream file;
        file.open(filepath, std::ios::in | std::ios::binary);
        if (!file.is_open())
        {
            close();
            return false;
        }

        // File chunks
        for (size_t i = 0; i < chunksCnt; ++i)
        {
            // Send file chunk
            file.read(m_buffer, NETCPY_TRANSFER_BUFFER_SIZE);
            if (!sendFilebuf(NETCPY_TRANSFER_BUFFER_SIZE))
            {
                close();
                return false;
            }

            // Update transfer progress
            double updateTime = m_updateTime.getAndReset();
            m_currentTime += updateTime;
            m_displayTime += updateTime;
            if (m_displayTime >= NETCPY_DISPLAY_UPDATE)
            {
                // Compute percents and bitrate
                double percents = (i*1.0)/(chunksCnt*1.0);
                double bitrate = (chunksCnt*NETCPY_TRANSFER_BUFFER_SIZE*1.0);
                if (m_currentTime > 0) { bitrate /= m_currentTime; }

                // Display transfer progress
                displayProgress(percents, bitrate);
            }
        }

        // File trail
        if (trailCnt > 0)
        {
            file.read(m_buffer, trailCnt);
            if (!sendFilebuf(trailCnt)) { close(); return false; }
        }
        file.close();

        displayProgress(100.0, 0.0);
        std::cout << "\n";

        // Send Ack
        preSend();
        m_buffer[NETCPY_TCP_HEADER_SIZE+0] = 'A';
        m_buffer[NETCPY_TCP_HEADER_SIZE+1] = 'C';
        m_buffer[NETCPY_TCP_HEADER_SIZE+2] = 'K';
        m_buffer[NETCPY_TCP_HEADER_SIZE+3] = '\0';
        if (!send()) { close(); return false; }

        // Receive Ack
        if (!receive()) { close(); return false; }
        if ((m_buffer[NETCPY_TCP_HEADER_SIZE+0] != 'A') ||
            (m_buffer[NETCPY_TCP_HEADER_SIZE+1] != 'C') ||
            (m_buffer[NETCPY_TCP_HEADER_SIZE+2] != 'K') ||
            (m_buffer[NETCPY_TCP_HEADER_SIZE+3] != '\0'))
        {
            std::cout << "Netcpy ACK error\n";
            close();
            return false;
        }
    }
    else
    {
        // Receive file
        std::ofstream file;
        file.open(
            m_fileName, std::ios::out | std::ios::binary | std::ios::trunc
        );
        if (!file.is_open())
        {
            std::cout << "File write error\n";
            close();
            return false;
        }

        // File chunks
        for (size_t i = 0; i < chunksCnt; ++i)
        {
            if (!receiveFilebuf(NETCPY_TRANSFER_BUFFER_SIZE))
            {
                close();
                return false;
            }
            file.write(m_buffer, NETCPY_TRANSFER_BUFFER_SIZE);

            // Update transfer progress
            double updateTime = m_updateTime.getAndReset();
            m_currentTime += updateTime;
            m_displayTime += updateTime;
            if (m_displayTime >= NETCPY_DISPLAY_UPDATE)
            {
                // Compute percents and bitrate
                double percents = (i*1.0)/(chunksCnt*1.0);
                double bitrate = (chunksCnt*NETCPY_TRANSFER_BUFFER_SIZE*1.0);
                if (m_currentTime > 0) { bitrate /= m_currentTime; }

                // Display transfer progress
                displayProgress(percents, bitrate);
            }
        }

        // File trail
        if (trailCnt > 0)
        {
            if (!receiveFilebuf(trailCnt)) { close(); return false; }
            file.write(m_buffer, trailCnt);
        }
        file.close();

        displayProgress(100.0, 0.0);
        std::cout << "\n";

        // Receive Ack
        if (!receive()) { close(); return false; }
        if ((m_buffer[NETCPY_TCP_HEADER_SIZE+0] != 'A') ||
            (m_buffer[NETCPY_TCP_HEADER_SIZE+1] != 'C') ||
            (m_buffer[NETCPY_TCP_HEADER_SIZE+2] != 'K') ||
            (m_buffer[NETCPY_TCP_HEADER_SIZE+3] != '\0'))
        {
            std::cout << "Netcpy ACK error\n";
            close();
            return false;
        }

        // Send Ack
        preSend();
        m_buffer[NETCPY_TCP_HEADER_SIZE+0] = 'A';
        m_buffer[NETCPY_TCP_HEADER_SIZE+1] = 'C';
        m_buffer[NETCPY_TCP_HEADER_SIZE+2] = 'K';
        m_buffer[NETCPY_TCP_HEADER_SIZE+3] = '\0';
        if (!send()) { close(); return false; }
    }

    // File sucessfully transfered
    std::cout << "File sucessfully transfered\n";
    std::cout << "Total transfer time : " <<
        m_totalTime.getElapsedTime() << " s" << '\n';
    return true;
}


////////////////////////////////////////////////////////////////////////////////
//  PreSend data (prepare send buffer)                                        //
////////////////////////////////////////////////////////////////////////////////
void Netcpy::preSend()
{
    // Reset buffer
    memset(m_buffer, 0, NETCPY_BUFFER_SIZE);
    memcpy(m_buffer, NetcpyTcpHeader, NETCPY_TCP_HEADER_SIZE);
}

////////////////////////////////////////////////////////////////////////////////
//  Send data                                                                 //
////////////////////////////////////////////////////////////////////////////////
bool Netcpy::send()
{
    // Send data
    if (!m_distantSock.sendData(m_buffer, NETCPY_BUFFER_SIZE))
    {
        std::cout << "TCP Socket send error\n";
        return false;
    }

    // Data successfully sent
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Send file buffer                                                          //
////////////////////////////////////////////////////////////////////////////////
bool Netcpy::sendFilebuf(size_t bufSize)
{
    // Send data
    if (!m_distantSock.sendData(m_buffer, bufSize))
    {
        std::cout << "TCP Socket send error\n";
        return false;
    }

    // File buffer successfully sent
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Receive data                                                              //
////////////////////////////////////////////////////////////////////////////////
bool Netcpy::receive()
{
    // Receive data
    memset(m_buffer, 0, NETCPY_BUFFER_SIZE);
    size_t recvLen = 0;
    do
    {
        m_bufSize = (NETCPY_BUFFER_SIZE - recvLen);
        if (!m_distantSock.receiveData(&m_buffer[recvLen], m_bufSize))
        {
            std::cout << "TCP Socket receive error\n";
            return false;
        }
        recvLen += m_bufSize;
    } while (recvLen < NETCPY_BUFFER_SIZE);
    m_bufSize = recvLen;

    // Check final buffer size
    if (m_bufSize != NETCPY_BUFFER_SIZE)
    {
        std::cout << "Netcpy TCP receive size error\n";
        std::cout << "Expected : " << NETCPY_BUFFER_SIZE <<
            ", received : " << m_bufSize << '\n';
        return false;
    }

    // Data sucessfully received
    if (memcmp(m_buffer, NetcpyTcpHeader, NETCPY_TCP_HEADER_SIZE) != 0)
    {
        std::cout << "Netcpy TCP header error\n";
        return false;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Receive file buffer                                                       //
////////////////////////////////////////////////////////////////////////////////
bool Netcpy::receiveFilebuf(size_t bufSize)
{
    // Receive data
    size_t recvLen = 0;
    do
    {
        m_bufSize = (bufSize - recvLen);
        if (!m_distantSock.receiveData(&m_buffer[recvLen], m_bufSize))
        {
            std::cout << "TCP Socket receive error\n";
            return false;
        }
        recvLen += m_bufSize;
    } while (recvLen < bufSize);
    m_bufSize = recvLen;

    // Check final buffer size
    if (m_bufSize != bufSize)
    {
        std::cout << "Netcpy TCP receive size error\n";
        std::cout << "Expected : " << bufSize <<
            ", received : " << m_bufSize << '\n';
        return false;
    }

    // Data sucessfully received
    return true;
}


////////////////////////////////////////////////////////////////////////////////
//  Display file transfer progress                                            //
////////////////////////////////////////////////////////////////////////////////
void Netcpy::displayProgress(double percents, double bitrate)
{
    // Compute int percents
    int percentsInt = static_cast<int>(percents*100.0);
    if (percentsInt <= 0) { percentsInt = 0; }
    if (percentsInt >= 100) { percentsInt = 100; }

    // Display progress bar
    std::cout << "\r|  [";
    for (int j = 0; j < 50; ++j)
    {
        if ((j*2) <= percentsInt) { std::cout << "="; }
        else { std::cout << "-"; }
    }

    // Display percents
    std::cout << "] ";
    if (percentsInt <= 99) { std::cout << "0"; }
    if (percentsInt <= 9) { std::cout << "0"; }
    std::cout << percentsInt << "%  |  ";

    // Display bitrate
    if (bitrate >= 1000000000.0)
    {
        bitrate /= 1000000000.0;
        std::cout << bitrate << " Gb/s";
    }
    else if (bitrate >= 1000000.0)
    {
        bitrate /= 1000000.0;
        std::cout << bitrate << " Mb/s";
    }
    else if (bitrate >= 1000.0)
    {
        bitrate /= 1000.0;
        std::cout << bitrate << " kb/s";
    }
    else
    {
        std::cout << bitrate << " bits/s";
    }
    m_displayTime = 0.0;
}
