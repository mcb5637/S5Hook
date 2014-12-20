:: S5Hook buildscript 
:: <2014> yoq
@echo off

:: stage 1 loader
utils\nasm -isrc/ -o bin/loader.bin src/loader.asm 

:: main
utils\nasm -isrc/ -o bin/S5Hook.bin src/S5Hook.asm 
utils\yoqXpand < bin/S5Hook.bin > output/S5Hook.yx
set /p STAGE2=<output/S5Hook.yx

:: create Lua file
copy /y "src\S5Hook.lua" "output\S5Hook.lua" >nul
utils\yoqTempl "output\S5Hook.lua"
del output\S5Hook.yx