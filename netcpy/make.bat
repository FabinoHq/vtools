::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
::     _______                       ____________________________________     ::
::     \\ .   \            _________/ . . . . . . . . . . . . . . . .   /     ::
::      \\ .   \       ___/ . . . . .    ______________________________/      ::
::       \\ .   \   __/. . .   _________/     /    // .  __________/          ::
::        \\ .   \_//      ___/ .  _____     /    // .  /______               ::
::         \\ .   \/     _/ // .  /    \\    |    \\  .        \              ::
::          \\ .        /   || .  |    ||    |     \\______     \             ::
::           \\ .      /    || .  \____//    |  _________//     /             ::
::            \\ .    /     //  .            / // . . . .      /              ::
::             \\____/     //_______________/ //______________/               ::
::                                                                            ::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
::   This is free and unencumbered software released into the public domain.  ::
::                                                                            ::
::   Anyone is free to copy, modify, publish, use, compile, sell, or          ::
::   distribute this software, either in source code form or as a compiled    ::
::   binary, for any purpose, commercial or non-commercial, and by any        ::
::   means.                                                                   ::
::                                                                            ::
::   In jurisdictions that recognize copyright laws, the author or authors    ::
::   of this software dedicate any and all copyright interest in the          ::
::   software to the public domain. We make this dedication for the benefit   ::
::   of the public at large and to the detriment of our heirs and             ::
::   successors. We intend this dedication to be an overt act of              ::
::   relinquishment in perpetuity of all present and future rights to this    ::
::   software under copyright law.                                            ::
::                                                                            ::
::   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,          ::
::   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF       ::
::   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   ::
::   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR        ::
::   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,    ::
::   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR    ::
::   OTHER DEALINGS IN THE SOFTWARE.                                          ::
::                                                                            ::
::   For more information, please refer to <https://unlicense.org>            ::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
::    VTools : Virtual Tools                                                  ::
::     make.bat : Windows make compilation file                               ::
::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

:: Get Visual Studio installation path
for /f "delims=" %%i in ('call^
 "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"^
 -all -legacy -property installationPath') do set VCInstallationPath=%%i

:: Setup Visual Studio environment
call "%VCInstallationPath%\VC\Auxiliary\Build\vcvars64.bat"

:: Build Netcpy
call msbuild /p:Configuration=Release Netcpy.sln

:: Delete previous binary
del /q netcpy.exe

:: Copy output binary
copy x64\Release\netcpy.exe netcpy.exe

:: System pause
pause
