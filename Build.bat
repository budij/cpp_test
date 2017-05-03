@ECHO OFF

ECHO Generator       - %Generator%
ECHO VisualStudioDir - %VisualStudioDir%
ECHO ToolSet         - %ToolSet%

IF "%Generator%"=="" (
   ECHO Environment variable Generator is required
   exit /b 1
)
IF "%VisualStudioDir%"=="" (
   ECHO Environment variable VisualStudioDir is required
   exit /b 1
)

SET SolutionFile=cpp_test.sln

REM Creating the directories
IF EXIST Build32 RMDIR /Q /S Build32
IF EXIST Build64 RMDIR /Q /S Build64
MKDIR Build32
MKDIR Build64

CALL pushd %CD%
ECHO Setting Visual studio environment variables forx86 build
SET CMD="%VisualStudioDir%\vcvars32.bat"
ECHO Executing %CMD%
CALL %CMD%
CALL popd

CALL pushd Build32
ECHO Creating the project using cmake for 32-bit windows
SET CMD=cmake .. %ToolSet% -G"%Generator%"
ECHO Executing %CMD%
CALL %CMD%

ECHO Compiling in debug configuration
SET CMD=msbuild %SolutionFile% /property:Configuration=Debug /property:Platform=Win32 /verbosity:m /maxcpucount
ECHO Executing %CMD%
CALL %CMD%

ECHO Compiling in release configuration
SET CMD=msbuild %SolutionFile% /property:Configuration=Release /property:Platform=Win32 /verbosity:m /maxcpucount
ECHO Executing %CMD%
CALL %CMD%
CALL popd

ECHO.

CALL pushd %CD%
ECHO Setting Visual studio environment variables for x64 build
SET CMD="%VisualStudioDir%\amd64\vcvars64.bat"
ECHO Executing %CMD%
CALL %CMD%
CALL popd

CALL pushd Build64
ECHO Creating the project using cmake for 64-bit windows
SET CMD=cmake .. %ToolSet% -G"%Generator% Win64"
ECHO Executing %CMD%
CALL %CMD%

ECHO Compiling in debug configuration
SET CMD=msbuild %SolutionFile% /property:Configuration=Debug /property:Platform=x64 /verbosity:m /maxcpucount
ECHO Executing %CMD%
CALL %CMD%

ECHO Compiling in release configuration
SET CMD=msbuild %SolutionFile% /property:Configuration=Release /property:Platform=x64 /verbosity:m /maxcpucount
ECHO Executing %CMD%
CALL %CMD%
CALL popd
