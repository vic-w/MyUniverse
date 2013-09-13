REM  copy the required binaries to the Release folder

if exist src\Release echo "Release Folder already exists"
if not exist src\Release mkdir src\Release

xcopy InnoSetup\Files\* src\Release /y 

REM  copy the required binaries to the Debugfolder

if exist src\Debug echo "Debug Folder already exists"
if not exist src\Debug mkdir src\Debug

xcopy InnoSetup\Files\calibmain.ini src\Debug /y
xcopy InnoSetup\Files\calibprev.ini src\Debug /y
xcopy InnoSetup\Files\config.ini src\Debug /y

xcopy InnoSetup\Files\DevIL.dll src\Debug /y
xcopy InnoSetup\Files\iconv.dll src\Debug /y
xcopy InnoSetup\Files\ILU.dll src\Debug /y
xcopy InnoSetup\Files\ILUT.dll src\Debug /y
xcopy InnoSetup\Files\libxml2.dll src\Debug /y
xcopy InnoSetup\Files\MyUniverseControl.pptm src\Debug /y
xcopy InnoSetup\Files\zlib1.dll src\Debug /y
xcopy InnoSetup\Files\regMyUniverseControl.exe src\Debug /y

xcopy 3rdparty\opencv231\bin\Debug\opencv_calib3d231d.dll src\Debug /y
xcopy 3rdparty\opencv231\bin\Debug\opencv_core231d.dll src\Debug /y
xcopy 3rdparty\opencv231\bin\Debug\opencv_features2d231d.dll src\Debug /y
xcopy 3rdparty\opencv231\bin\Debug\opencv_flann231d.dll src\Debug /y
xcopy 3rdparty\opencv231\bin\Debug\opencv_highgui231d.dll src\Debug /y
xcopy 3rdparty\opencv231\bin\Debug\opencv_imgproc231d.dll src\Debug /y

xcopy 3rdparty\Qt4.84\bin\QtCored4.dll src\Debug /y
xcopy 3rdparty\Qt4.84\bin\QtGuid4.dll src\Debug /y
xcopy 3rdparty\Qt4.84\bin\QtTestd4.dll src\Debug /y


