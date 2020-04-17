git clone --recursive https://github.com/ai-music/obs-studio.git

cd obs-studio

SET branchName=%1
IF NOT DEFINED branchName SET branchName="Master"

echo "Checking out branch "%branchName%
git checkout %branchName%

cmake -DDepsPath:STRING=C:/OBS/Deps/win64
cmake -DQTDIR:STRING=C:/Qt/5.14.2/msvc2017_64
cmake -S . -B build64






