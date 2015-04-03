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
REM %BIN2H% MIG.MOD music > music_mig.h
REM %BIN2H% AreYouExcited.MOD music > music_excited.h
REM %BIN2H% AXELF.MOD music > music_axelf.h
REM %BIN2H% PIANOPLK.MOD music > music_pianoplk.h
REM %BIN2H% tellit.mod music > music_tellit.h
REM %BIN2H% time-trap.mod music > music_timetrap.h
REM %BIN2H% test.mod music > music_test.h
REM %BIN2H% lotus2.mod music > music_lotus2.h
%BIN2H% mikrofon.mod music > music_mikrofon.h


echo =======================================================
echo Creating colormap for dithering...
echo =======================================================
REM %CONVERT% -size 16x16 xc: -channel R -fx "(i%%8)/7" -channel G -fx "(j%%8)/7" -channel B -fx "((i>>3&1)|(j>>2&2))/3" -scale 600%% %COLORMAP%

echo =======================================================
ECHO Converting images 280x400
echo =======================================================
CD %DATADIR%\images\280x400
REM %IMG2H% spacetits png 280 400
REM %IMG2H% test png 280 400

echo =======================================================
ECHO Converting sprites
echo =======================================================
CD %DATADIR%\images\sprites
REM %IMG2H% duck png 50 45
REM %IMG2H% ducky png 17 15
REM %IMG2H% badge png 40 40
Rem %IMG2H% ball png 32 32
Rem %IMG2H% ball_s png 16 16

echo =======================================================
ECHO Converting fonts
echo =======================================================
CD %DATADIR%\fonts
REM %FONT2H% xenon2 bmp
REM %FONT2H% blazingStar bmp
REM %FONT2H% blazingStar2 bmp
REM %FONT2H% bubsy bmp
REM %FONT2H% epicpin bmp
REM %FONT2H% ascii_vga png
REM %FONT2H% ascii_cga png
REM %FONT2H% vga8x16 png


cd %ROOTDIR%

echo =======================================================
echo DONE!
echo =======================================================

