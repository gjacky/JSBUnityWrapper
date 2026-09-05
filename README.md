Clone della repository JSB

Clona la repository nella folder di progetto:

git clone --recurse-submodules https://github.com/JSBSim-Team/jsbsim.git

Build del progetto

Per compilare il progetto con CMake e Visual Studio 2022, eseguire i seguenti comandi dalla directory principale:

mkdir build
cd build
cmake .. -G "Visual Studio 18 2026" -A x64