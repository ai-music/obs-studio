REM *******************************************************************************
REM * This is to be run in the folder ABOVE the project folder to checkout a branch
REM * from GIT, run Cmake, Visual Studio build and Advanced Installer
REM * So why is this BAT file within the project folder - it's a convenient place
REM * to keep it so that it is together with the rest of the project.
REM *******************************************************************************

git clone --recursive https://github.com/ai-music/obs-studio.git

cd obs-studiofol
mkdir build64

SET branchName=%1
IF NOT DEFINED branchName SET branchName="Master"

echo "Checking out branch "%branchName%
git checkout %branchName%

git tag > tags.txts

REM To-Do get the latest tag by reading the last line of the file.

del tag.txt

cmake -DDepsPath:STRING=C:/OBS/Deps/win64 -DQTDIR:STRING=C:/Qt/5.14.2/msvc2017_64 -S C:/OBS/obs-studio -B C:/OBS/obs-studio/build64

cd build64
SET tag="0.1"

REM To-Do hard code tag into source code as version
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild" obs-studio.sln /p:Configuration=Debug /p:Platform=x64
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild" obs-studio.sln /p:Configuration=Release /p:Platform=x64
cd rundir\Release\bin\64bit
"C:\Qt\5.14.2\msvc2017_64\bin\WindeployQt" Obs64.exe
cd ..\..\..\Debug\bin\64bit
"C:\Qt\5.14.2\msvc2017_64\bin\WindeployQt" Obs64.Exe

cd ..\..\..\..\..

"C:\Program Files (x86)\Caphyon\Advanced Installer 16.9\bin\x86\advinst.exe" /edit OssiaLive.aip /SetVersion %tag%
"C:\Program Files (x86)\Caphyon\Advanced Installer 16.9\bin\x86\advinst.exe" /build OssiaLive.aip






