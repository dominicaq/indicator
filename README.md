# Attitude Indicator

#### Building the Project

1. **Create the build directory and navigate to it**:
```
mkdir build
cd build
```

2. **Generate Visual Studio project files and build**:
For regular build:
```
cmake -G "Visual Studio 17 2022" ..
cmake --build . --config Release
```
For enabling console output:
```
cmake -G "Visual Studio 17 2022" -DSHOW_CONSOLE=ON ..
cmake --build . --config Debug
```

This will set up and build the Attitude Indicator project in your development environment.
