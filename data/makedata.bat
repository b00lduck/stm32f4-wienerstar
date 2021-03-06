@ECHO OFF
d:
SET ROOTDIR=%1
SET DATADIR=%ROOTDIR%\data
SET BIN2H=%ROOTDIR%\tools\bin2h.exe
SET IMG2H=CALL %DATADIR%\img2h.bat
SET FONT2H=CALL %DATADIR%\font2h.bat
SET CONVERT=%ROOTDIR%\tools\convert.exe
SET IMAGECONVERT=java -jar %ROOTDIR%\tools\ImageConverter.jar
SET IMAGECONVERTBW=java -jar %ROOTDIR%\tools\ImageConverterBW.jar
SET COLORMAP=%DATADIR%\images\colormap_332.gif

echo ===========================================================
echo ==== makedata.bat CONFIG ==================================
echo ROOTDIR: 	   %ROOTDIR%
echo DATADIR:      %DATADIR%
echo BIN2H:        %ROOTDIR%\tools\bin2h.exe
echo IMG2H:        CALL %DATADIR%\img2h.bat
echo FONT2H:       CALL %DATADIR%\font2h.bat
echo CONVERT:      %ROOTDIR%\tools\convert.exe
echo IMAGECONVERT: java -jar %ROOTDIR%\tools\ImageConverter.jar
echo COLORMAP:     %DATADIR%\images\colormap_332.gif
echo ===========================================================

echo =======================================================
ECHO Converting MOD-files...
echo =======================================================
cd %DATADIR%\music\mod
%BIN2H% test.mod music > music_test.h
%BIN2H% mikrofon.mod music > music_mikrofon.h


echo =======================================================
echo Creating colormap for dithering...
echo =======================================================
%CONVERT% -size 16x16 xc: -channel R -fx "(i%%8)/7" -channel G -fx "(j%%8)/7" -channel B -fx "((i>>3&1)|(j>>2&2))/3" -scale 600%% %COLORMAP%

echo =======================================================
ECHO Converting images 280x400
echo =======================================================
CD %DATADIR%\images\280x400
%IMG2H% test png 280 400

echo =======================================================
ECHO Converting images 280x200
echo =======================================================
CD %DATADIR%\images\280x200
%IMG2H% title png 280 200

echo =======================================================
ECHO Converting sprites
echo =======================================================
CD %DATADIR%\images\sprites
%IMG2H% duck png 50 45
%IMG2H% ducky png 17 15
%IMG2H% badge png 40 40
%IMG2H% ball png 32 32
%IMG2H% ball_s png 16 16

echo =======================================================
ECHO Converting fonts
echo =======================================================
CD %DATADIR%\fonts
%FONT2H% xenon2 bmp
%FONT2H% blazingStar bmp
%FONT2H% vga8x16 png


cd %ROOTDIR%

echo =======================================================
echo DONE!
echo =======================================================

