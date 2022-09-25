@echo off

@REM 本地OpenHarmony源码目录,需根据实际环境修改
set LOCAL=\\wsl$\ubuntu-20.04\home\code\OHOS
@REM 本地编译环境的out目录
set OUTDIR=out\rk3568

@REM 本地用例生成目录
set TESTDIR=%LOCAL%\%OUTDIR%\musl\libc-test
@REM 动态链接库生成目录
set DYNLIB=%LOCAL%\%OUTDIR%\musl\libc-test-lib
@REM 远程传输目标目录
set REMOTE=/data/tests/libc-test
set REMOTEFEXT=/data/tests/libc-test/src/functionalext
set REMOTESYSTEM=/system/lib
@REM runtest脚本所在目录
set SHDIR=%LOCAL%\third_party\musl\scripts

@REM 开始时间
set /a startS=%time:~6,2%
set /a startM=%time:~3,2%

@REM 检查设备是否连接
echo checking HDC device 
for /F "usebackq delims==" %%c in (`hdc list targets`) DO (
    echo Device list:
    echo %%c | findstr "[Empty]" && goto noDevice || echo %%c && goto CheckDirectory
)

:CheckDirectory
echo checking TESTDIR
if exist %TESTDIR% (
	echo TESTDIR: %TESTDIR%
) else (
	echo Error: path "%TESTDIR%" was not found.
	goto end
)


@REM 在单板创建目录, 需要预先创建好才能传输到相应位置。
:hdcStart
echo.
echo now mkdir...
hdc shell mkdir /data/tests
hdc shell mkdir %REMOTE%
hdc shell mkdir %REMOTE%/src

@REM 创建临时文件夹,用于存放用例生成的临时文件
hdc shell mkdir /tmp
hdc shell mkdir /dev/shm
echo Mkdir done.
goto hdcSend

@REM 传输文件,单板上执行runtest.sh，将结果REPORT返回到.bat所在目录
:hdcSend
for %%i in (%TESTDIR%\*) do (
	echo %%i
	hdc file send %%i %REMOTE%/src
)
for %%j in (%DYNLIB%\*) do (
	echo %%j
	hdc file send %%j %REMOTE%/src
)
hdc shell chmod +x %REMOTE%/src/*

hdc shell mount -o rw,remount /
hdc shell chmod 777 /etc

hdc file send %SHDIR%\runtest.sh %REMOTE%/runtest.sh
hdc shell chmod +x %REMOTE%/runtest.sh
hdc shell %REMOTE%/runtest.sh

echo hdc file recv %REMOTE%/REPORT %~dp0REPORT
hdc file recv %REMOTE%/REPORT %~dp0REPORT

:end
echo.
set /a endS=%time:~6,2%
set /a endM=%time:~3,2%
set /a diffS_=%endS%-%startS%
set /a diffM_=%endM%-%startM%

pause
exit