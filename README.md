# SecondAID
**SecondAID** is a powerful, standalone script editor and real-time debugger for The Guild 2 based on reverse engineering of AID protocol.

<img width="1728" height="971" alt="Przechwycenie obrazu ekranu_2026-01-05_02-44-47" src="https://github.com/user-attachments/assets/53aba74f-795b-4f85-a365-162fdf3e44ca" />

# Installation/Running

## Windows

Just copy SecondAID-GUI.exe and luacheck.exe into the game folder (where Guild2.exe is) and run SecondAID-GUI.exe

## Linux

Luacheck is not included with Linux version of SecondAID, so you have to install it via your distros package manager beforehand, eg. on Arch:
```
  sudo pacman -S luacheck
```
Then just copy SecondAID-GUI_linux into your game folder and run.

# Building

## As Release Bundle

You can use the build_release_bundles.sh script to build both a portable linux executable, and cross-compile a Windows executable, zipped with luacheck. 

It's mainly used for automatically generating primary releases for SecondAID.

## For Linux

```
  mkdir build
  cd build
  cmake ..
```

## For Windows (MinGW64)

```
  mkdir build_win
  cd build_win
  cmake -DCMAKE_TOOLCHAIN_FILE=../Toolchain-mingw.cmake ..
```
*disclaimer: it works for cross-compiling from linux to windows. It should probably work when compiling Windows->Windows, but i don't have a way to test this*

