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
//     System/SysThread.cpp : System Thread management                        //
////////////////////////////////////////////////////////////////////////////////
#include "SysThread.h"


////////////////////////////////////////////////////////////////////////////////
//  SysThread default constructor                                             //
////////////////////////////////////////////////////////////////////////////////
SysThread::SysThread() :
m_thread(0),
m_mutex(),
m_running(false),
m_standby(false)
{

}

////////////////////////////////////////////////////////////////////////////////
//  SysThread virtual destructor                                              //
////////////////////////////////////////////////////////////////////////////////
SysThread::~SysThread()
{
    // Stop eventual running thread
    stop();
}

////////////////////////////////////////////////////////////////////////////////
//  Start the thread                                                          //
////////////////////////////////////////////////////////////////////////////////
bool SysThread::start()
{
    // Stop eventual running thread
    stop();

    // Start the thread
    m_thread = new (std::nothrow) std::thread(&SysThread::run, this);
    if (!m_thread) return false;

    // Thread successfully started
    return true;
}

////////////////////////////////////////////////////////////////////////////////
//  Stop the thread                                                           //
////////////////////////////////////////////////////////////////////////////////
void SysThread::stop()
{
    if (m_thread)
    {
        // Request thread stop
        m_mutex.lock();
        m_running = false;
        m_mutex.unlock();

        // Wait for the thread to stop
        m_thread->join();

        // Clear thread memory
        delete m_thread;
        m_thread = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////
//  Set the thread's standby mode                                             //
////////////////////////////////////////////////////////////////////////////////
void SysThread::standby(bool standbyMode)
{
    m_mutex.lock();
    m_standby = standbyMode;
    m_mutex.unlock();
}

////////////////////////////////////////////////////////////////////////////////
//  Thread virtual process                                                    //
////////////////////////////////////////////////////////////////////////////////
void SysThread::process()
{
    // Default process : Standby thread
    SysSleep(SysThreadStandbySleepTime);
}


////////////////////////////////////////////////////////////////////////////////
//  Thread subroutine                                                         //
////////////////////////////////////////////////////////////////////////////////
void SysThread::run()
{
    // Set running and standby state
    bool running = false;
    bool standby = false;

    m_mutex.lock();
    m_running = running = true;
    m_standby = standby = false;
    m_mutex.unlock();

    // Run the thread
    while (running)
    {
        if (standby)
        {
            // Thread standby mode
            SysSleep(SysThreadStandbySleepTime);
        }
        else
        {
            // Thread process
            process();
        }

        // Update running and standby states
        m_mutex.lock();
        running = m_running;
        standby = m_standby;
        m_mutex.unlock();
    }
}
