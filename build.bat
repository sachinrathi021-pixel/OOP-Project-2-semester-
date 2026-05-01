@echo off
REM Build script for Metal Slug - Main Menu + Biome System
REM Uses MSYS2 g++ with SFML 3.1.0
set GCC=C:\msys64\ucrt64\bin\g++.exe
set SFML_DIR=.cursor\SFML-3.1.0
set SFML_INCLUDE=%SFML_DIR%\include
set SFML_LIB=%SFML_DIR%\lib
set SFML_BIN=%SFML_DIR%\bin
echo ============================================
echo   Building Metal Slug - Full Build
echo ============================================
echo.
echo Compiling source files...
%GCC% -std=c++17 -c main.cpp -o main.o -I%SFML_INCLUDE%
if %ERRORLEVEL% NEQ 0 goto :error
%GCC% -std=c++17 -c Game.cpp -o Game.o -I%SFML_INCLUDE%
if %ERRORLEVEL% NEQ 0 goto :error
%GCC% -std=c++17 -c Screen.cpp -o Screen.o -I%SFML_INCLUDE%
if %ERRORLEVEL% NEQ 0 goto :error
%GCC% -std=c++17 -c LandingScreen.cpp -o LandingScreen.o -I%SFML_INCLUDE%
if %ERRORLEVEL% NEQ 0 goto :error
%GCC% -std=c++17 -c WelcomeScreen.cpp -o WelcomeScreen.o -I%SFML_INCLUDE%
if %ERRORLEVEL% NEQ 0 goto :error
%GCC% -std=c++17 -c LevelSelectScreen.cpp -o LevelSelectScreen.o -I%SFML_INCLUDE%
if %ERRORLEVEL% NEQ 0 goto :error
%GCC% -std=c++17 -c GameScreen.cpp -o GameScreen.o -I%SFML_INCLUDE%
if %ERRORLEVEL% NEQ 0 goto :error
%GCC% -std=c++17 -c AudioManager.cpp -o AudioManager.o -I%SFML_INCLUDE%
if %ERRORLEVEL% NEQ 0 goto :error
%GCC% -std=c++17 -c PerlinNoise.cpp -o PerlinNoise.o -I%SFML_INCLUDE%
if %ERRORLEVEL% NEQ 0 goto :error
%GCC% -std=c++17 -c NoiseProfile.cpp -o NoiseProfile.o -I%SFML_INCLUDE%
if %ERRORLEVEL% NEQ 0 goto :error
%GCC% -std=c++17 -c Biome.cpp -o Biome.o -I%SFML_INCLUDE%
if %ERRORLEVEL% NEQ 0 goto :error
%GCC% -std=c++17 -c PlainsBiome.cpp -o PlainsBiome.o -I%SFML_INCLUDE%
if %ERRORLEVEL% NEQ 0 goto :error
%GCC% -std=c++17 -c AerialBiome.cpp -o AerialBiome.o -I%SFML_INCLUDE%
if %ERRORLEVEL% NEQ 0 goto :error
%GCC% -std=c++17 -c AquaticBiome.cpp -o AquaticBiome.o -I%SFML_INCLUDE%
if %ERRORLEVEL% NEQ 0 goto :error
%GCC% -std=c++17 -c Level.cpp -o Level.o -I%SFML_INCLUDE%
if %ERRORLEVEL% NEQ 0 goto :error
echo.
echo Linking...
%GCC% -o MetalSlug.exe main.o Game.o Screen.o LandingScreen.o WelcomeScreen.o LevelSelectScreen.o GameScreen.o AudioManager.o PerlinNoise.o NoiseProfile.o Biome.o PlainsBiome.o AerialBiome.o AquaticBiome.o Level.o -L%SFML_LIB% -lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system
if %ERRORLEVEL% NEQ 0 goto :error
echo.
echo Copying DLLs...
copy "%SFML_BIN%\sfml-graphics-3.dll" . >nul 2>&1
copy "%SFML_BIN%\sfml-window-3.dll" . >nul 2>&1
copy "%SFML_BIN%\sfml-system-3.dll" . >nul 2>&1
copy "%SFML_BIN%\sfml-audio-3.dll" . >nul 2>&1
echo.
echo ============================================
echo   BUILD SUCCESSFUL! Run MetalSlug.exe
echo ============================================
echo.
REM Clean up object files
del *.o 2>nul
goto :end
:error
echo.
echo ============================================
echo   BUILD FAILED! Check errors above.
echo ============================================
echo.
:end
