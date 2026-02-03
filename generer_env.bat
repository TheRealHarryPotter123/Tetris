echo off
REM naviger à la racine du projet
cd %~dp0

REM créer le dossier build s'il n'existe pas
if not exist build (
    mkdir build
)

REM naviger dans le dossier build
cd build

REM générer les fichiers de build avec CMake
cmake ..

REM revenir à la racine du projet
cd ..