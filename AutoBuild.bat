git clone --recursive https://github.com/ai-music/obs-studio.git

cd obs-studio
mkdir build64

SET branchName=%1
IF NOT DEFINED branchName SET branchName="Master"

echo "Checking out branch "%branchName%
git checkout %branchName%

git tag > tags.txt

cmake -DDepsPath:STRING=C:/OBS/Deps/win64 -DQTDIR:STRING=C:/Qt/5.14.2/msvc2017_64 -S C:/OBS/obs-studio -B C:/OBS/obs-studio/build64

cd build64
"C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\MSBuild\Current\Bin\MSBuild" obs-studio.sln /p:Configuration=Release /p:Platform=x64
cd ..

SET tag="0.1"
"C:\Program Files (x86)\Caphyon\Advanced Installer 16.9\bin\x86\advinst.exe" /edit OssiaLive.aip /SetVersion %tag%
"C:\Program Files (x86)\Caphyon\Advanced Installer 16.9\bin\x86\advinst.exe" /build OssiaLive.aip




