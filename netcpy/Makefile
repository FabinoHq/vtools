################################################################################
##              ________           ______________________________________     ##
##             // .    /   _______/ . . . . . . . . . . . . . . . . .   /     ##
##            // .    /   // . . . .    _______________________________/      ##
##           // .     |  //      ______/ \\      |  // .   /_/     __/        ##
##          // .      | //    __// .   \_//     /  // .   /     __/           ##
##         // .       |//    / // .   _________/  // .       __/              ##
##        // .   /|   |/    / // .   /  \\    \  // .        \                ##
##       // .   /||        / // .    \__//    / // .   /\     \               ##
##      // .   / ||       / //  .            / // .   /  \     \              ##
##     //_____/  ||______/  \\______________/ //_____/    \____/              ##
##                                                                            ##
################################################################################
##   This is free and unencumbered software released into the public domain.  ##
##                                                                            ##
##   Anyone is free to copy, modify, publish, use, compile, sell, or          ##
##   distribute this software, either in source code form or as a compiled    ##
##   binary, for any purpose, commercial or non-commercial, and by any        ##
##   means.                                                                   ##
##                                                                            ##
##   In jurisdictions that recognize copyright laws, the author or authors    ##
##   of this software dedicate any and all copyright interest in the          ##
##   software to the public domain. We make this dedication for the benefit   ##
##   of the public at large and to the detriment of our heirs and             ##
##   successors. We intend this dedication to be an overt act of              ##
##   relinquishment in perpetuity of all present and future rights to this    ##
##   software under copyright law.                                            ##
##                                                                            ##
##   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,          ##
##   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF       ##
##   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   ##
##   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR        ##
##   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,    ##
##   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR    ##
##   OTHER DEALINGS IN THE SOFTWARE.                                          ##
##                                                                            ##
##   For more information, please refer to <https://unlicense.org>            ##
################################################################################
##    VTools : Virtual Tools                                                  ##
##     Makefile : Linux make compilation file                                 ##
################################################################################
CC=g++ -std=c++17 -O3 -fno-exceptions -fno-rtti \
	-fomit-frame-pointer -ffunction-sections \
	-fno-trapping-math -fno-math-errno -fno-signed-zeros
CFLAGS=-W -Wall
LDFLAGS=-pthread

all: netcpy

netcpy: main.o \
	Netcpy.o \
	System/SysThread.o System/SysClock.o System/SysCPU.o \
	System/Lin/SysMessage.o System/SysSettings.o \
	Network/IPAddress.o Network/TCPSocket.o Network/UDPSocket.o

	$(CC) -o netcpy \
	Netcpy.o \
	System/SysThread.o System/SysClock.o System/SysCPU.o \
	System/Lin/SysMessage.o System/SysSettings.o \
	Network/IPAddress.o Network/TCPSocket.o Network/UDPSocket.o \
	main.o $(LDFLAGS)


main.o: main.cpp
	$(CC) -o main.o -c main.cpp $(CFLAGS)

Netcpy.o: Netcpy.cpp
	$(CC) -o Netcpy.o -c Netcpy.cpp $(CFLAGS)


System/SysThread.o: System/SysThread.cpp
	$(CC) -o System/SysThread.o -c System/SysThread.cpp $(CFLAGS)

System/SysClock.o: System/SysClock.cpp
	$(CC) -o System/SysClock.o -c System/SysClock.cpp $(CFLAGS)

System/SysCPU.o: System/SysCPU.cpp
	$(CC) -o System/SysCPU.o -c System/SysCPU.cpp $(CFLAGS)

System/Lin/SysMessage.o: System/Lin/SysMessage.cpp
	$(CC) -o System/Lin/SysMessage.o -c System/Lin/SysMessage.cpp $(CFLAGS)

System/SysSettings.o: System/SysSettings.cpp
	$(CC) -o System/SysSettings.o -c System/SysSettings.cpp $(CFLAGS)


Network/IPAddress.o: Network/IPAddress.cpp
	$(CC) -o Network/IPAddress.o -c Network/IPAddress.cpp $(CFLAGS)

Network/TCPSocket.o: Network/TCPSocket.cpp
	$(CC) -o Network/TCPSocket.o -c Network/TCPSocket.cpp $(CFLAGS)

Network/UDPSocket.o: Network/UDPSocket.cpp
	$(CC) -o Network/UDPSocket.o -c Network/UDPSocket.cpp $(CFLAGS)


clean:
	rm -rf *.o
	rm -rf System/*.o
	rm -rf System/Lin/*.o
	rm -rf Network/*.o

mrproper: clean
	rm -rf netcpy
