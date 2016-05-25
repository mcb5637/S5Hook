:: S5Hook buildscript 
:: <2016> yoq
@echo off

:: stage 0 loader
utils\nasm -isrc/ -o bin/stage0.bin src/loader_stage0.asm
utils\yoqXpand < bin/stage0.bin > output/stage0.yx

:: stage 1 loader
utils\nasm -isrc/ -o bin/stage1.bin src/loader_stage1.asm
utils\yoqXpand < bin/stage1.bin > output/stage1.yx

:: main
utils\nasm -isrc/ -o bin/S5Hook.bin src/S5Hook.asm 
utils\yoqXpand < bin/S5Hook.bin > output/S5Hook.yx

:: create Lua file
copy /y "src\S5Hook.lua" "output\S5Hook.lua" >nul
utils\yoqTempl "output\S5Hook.lua"
del output\S5Hook.yx
del output\stage0.yx
del output\stage1.yx
pause