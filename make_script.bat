@echo off
if not defined build_dir goto :missing_variables

mkdir "%build_dir%\hehu_mf\functions"

pushd %tools_dir%\binmake
@echo on
binmake.exe "%source_dir%\config.cpp" "%build_dir%\hehu_mf\config.bin"
@echo off
popd

@echo on
copy "%source_dir%\functions\" "%build_dir%\hehu_mf\functions"

"%tools_dir%\filebank\filebank.exe" -property prefix=hehu_mf "%build_dir%\hehu_mf"

@echo off
mkdir "%target_dir%\@hehu_mf\addons"

@echo on
copy "%build_dir%\hehu_mf.pbo" "%target_dir%\@hehu_mf\addons"
copy "%source_dir%\mod.cpp" "%target_dir%\@hehu_mf"
copy "%source_dir%\README.md" "%target_dir%\@hehu_mf\readme.txt"

exit 0

:missing_variables
echo You need to set up your make.bat file and run that one instead!
pause
exit 1