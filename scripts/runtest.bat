@echo off

@REM 开始时间
set /a startS=%time:~6,2%
set /a startM=%time:~3,2%

@REM 本地文件所在目录
set LOCAL=\\wsl$\ubuntu-20.04\home\OHOS\out\ohos-arm-release\tests\unittest\libc-test\src
@REM 远程传输目标目录
set REMOTE=/data/tests/libc-test/src
@REM runtest脚本所在目录
set SHDIR=\\wsl$\Ubuntu-20.04\home\OHOS\third_party\musl\scripts

@REM 检查设备是否连接
echo HDC device checking...
for /F "usebackq delims==" %%c in (`hdc list targets`) DO (
    echo Device list:
    echo %%c | findstr "[Empty]" && goto noDevice || echo %%c && goto hdcStart
)

@REM 在单板创建目录,原先并不存在相应目录，也无法在传输时创建，因此需要预先创建好才能传输到相应位置。
:hdcStart
echo.
echo now mkdir...
hdc shell mkdir /data/tests
hdc shell mkdir /data/tests/libc-test
hdc shell mkdir %REMOTE%
hdc shell mkdir %REMOTE%/api
hdc shell mkdir %REMOTE%/common
hdc shell mkdir %REMOTE%/functional
hdc shell mkdir %REMOTE%/math
hdc shell mkdir %REMOTE%/musl
hdc shell mkdir %REMOTE%/regression
echo Done.
goto hdcSend

@REM 传输文件,单板上执行runtest.sh，将结果REPORT返回到.bat所在目录
:hdcSend
for /F %%i in ('dir %LOCAL% /S /B') do (
    for %%b in ("%%i\..") do (
        echo Sending %%~nb/%%~nxi
        hdc file send -sync %%i %REMOTE%/%%~nb/%%~nxi
        hdc shell chmod 777 %REMOTE%/%%~nb/%%~nxi
    )
)
echo Done.
echo.
echo Sending run.sh
hdc file send %SHDIR%\run.sh %REMOTE%/runtest.sh
hdc shell chmod 777 %REMOTE%/runtest.sh
echo.
echo ============================
echo ALL files send finished.
echo.
echo hdc shell .%REMOTE%/runtest.sh
hdc shell .%REMOTE%/runtest.sh
echo.
echo hdc file recv %REMOTE%/REPORT %~dp0REPORT
hdc file recv %REMOTE%/REPORT %~dp0REPORT
goto end

@REM 若检查无设备连接，提示检查设备。
:noDevice
echo Device not found,please check your device.
goto end

:end
echo.
set /a endS=%time:~6,2%
set /a endM=%time:~3,2%
set /a diffS_=%endS%-%startS%
set /a diffM_=%endM%-%startM%
echo All Done. Time cost:%diffM_%m%diffS_%s
@REM 完成所用时间
echo.
pause
exit
