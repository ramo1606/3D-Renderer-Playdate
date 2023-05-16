# Playdate_Template
Template for playdate

This is a template to start a new playdate project using Lua and C.

Best way I have found is work on VSCode is with sumneko lua extension and Playdate Debug extension. This way you can always debug your Lua Code.

If you need to work on C code then you can:
- mkdir build
- cd build
- cmake ..
- Open Solution in Visual Studio
- Click project and "Set as Startup Project"
- Work on your C code, debug if need.

When you are done with C development: 
- Open X64 Native Tools Command Prompt for VS
- Go to your project folder
- cd build

For Simulator
- cmake .. -G "NMake Makefiles"
- nmake

For Playdate
- cmake .. -G "NMake Makefiles" --toolchain=<path to SDK>/C_API/buildsupport/arm.cmake
- nmake

And continue with your Lua Development on VSCode where you can use the Lua debugger again.

When you’re ready to do a release build, regenerate the build targets by passing -DCMAKE_BUILD_TYPE=Release argument to CMake