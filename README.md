# Car Park Challenge

For graphics this code uses SDL library. No other third party component was used. The code has some explanatory comments.

## Building and running
### MSbuild
To build and run the project SDL library should be installed. In the project file change `C:\Development\SDL2`
to wherever SDL is installed. The code can be built and run with the following commands.
```
msbuild /t:Build
debug\Wolf3DChallenge.exe
```

### GCC
The code was tested on Ubuntu 16.04. Install SDL. Build and run the code with the commands:
```
make
make run
```


