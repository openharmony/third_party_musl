@echo off

@REM 本地OpenHarmony源码目录,需根据实际环境修改
set LOCAL=\\wsl$\ubuntu-20.04\home\code\OHOS
@REM 本地编译环境的out目录
set OUTDIR=out\rk3568

@REM 本地用例生成目录
set TESTDIR=%LOCAL%\%OUTDIR%\tests\unittest\libc-test\src
set TESTDIRFEXT=%LOCAL%\%OUTDIR%\tests\unittest\libc-test\src\functionalext
@REM 动态链接库生成目录
set DYNLIB=%LOCAL%\%OUTDIR%\musl\libc-test-lib
@REM 远程传输目标目录
set REMOTE=/data/tests/libc-test/src
set REMOTEFEXT=/data/tests/libc-test/src/functionalext
set REMOTESYSTEM=/system/lib
@REM runtest脚本所在目录
set SHDIR=%LOCAL%\third_party\musl\scripts

@REM 开始时间
set /a startS=%time:~6,2%
set /a startM=%time:~3,2%

@REM 检查设备是否连接
echo HDC device checking...
for /F "usebackq delims==" %%c in (`hdc list targets`) DO (
    echo Device list:
    echo %%c | findstr "[Empty]" && goto noDevice || echo %%c && goto hdcStart
)

@REM 在单板创建目录, 需要预先创建好才能传输到相应位置。
:hdcStart
echo.
echo now mkdir...
hdc shell mkdir /data/tests
hdc shell mkdir /data/tests/libc-test
hdc shell mkdir %REMOTE%

@REM 创建临时文件夹,用于存放用例生成的临时文件
hdc shell mkdir /tmp
hdc shell mkdir /dev/shm
echo Mkdir done.
goto hdcSend

@REM 传输文件,单板上执行runtest.sh，将结果REPORT返回到.bat所在目录
:hdcSend
for /D %%i in (%TESTDIR%\*) do (
	echo [MKdir] %REMOTE%/%%~ni
	hdc shell mkdir %REMOTE%/%%~ni
	@REM Check local subdirectory
    for /D %%j in (%%i\*) do (
		echo [MKdir] %REMOTE%/%%~ni/%%~nj
		hdc shell mkdir %REMOTE%/%%~ni/%%~nj
		@REM Send test cases to remote subdirectory
		for %%k in (%%~j\*) do (
			echo Sending %%k %REMOTE%/%%~ni/%%~nj
			hdc file send %%k %REMOTE%/%%~ni/%%~nj
		)
		hdc shell chmod +x %REMOTE%/%%~ni/%%~nj/*
    )
	@REM Send test cases to remote directory
	for %%l in (%%~i\*) do (
		echo hdc file send %%l %REMOTE%/%%~ni
		hdc file send %%l %REMOTE%/%%~ni
	)
	hdc shell chmod +x %REMOTE%/%%~ni/*
)


@REM 动态库传输
hdc shell mount -o rw,remount /
hdc shell chmod 777 /etc
hdc file send %LOCAL%\third_party\musl\porting\linux\user\ldso\ld-musl-namespace.ini /etc/ld-musl-namespace.ini
hdc file send %DYNLIB%\libdlopen_dso.so %REMOTE%/functional/libdlopen_dso.so
hdc file send %DYNLIB%\libdlopen_ns_dso.so %REMOTE%/functional/libdlopen_ns_dso.so
hdc file send %DYNLIB%\libdlclose_reset_dso.so %REMOTE%/functional/libdlclose_reset_dso.so
hdc file send %DYNLIB%\libtls_align_dso.so %REMOTE%/functional/libtls_align_dso.so
hdc file send %DYNLIB%\libtls_init_dso.so %REMOTE%/functional/libtls_init_dso.so
hdc file send %DYNLIB%\libtls_get_new-dtv_dso.so %REMOTE%/regression/libtls_get_new-dtv_dso.so
hdc file send %DYNLIB%\libdlopen_ns_dso.so %REMOTEFEXT%/dlns/libdlopen_ns_dso.so
hdc file send %DYNLIB%\libdlopen_ns_dso.so %REMOTESYSTEM%/libdlopen_ns_dso.so
hdc file send %DYNLIB%\libdso_easy_symver.so %REMOTESYSTEM%/libdso_easy_symver.so
hdc file send %DYNLIB%\libdso_hard_symver.so %REMOTESYSTEM%/libdso_hard_symver.so
hdc file send %DYNLIB%\libdso_no_symver.so %REMOTESYSTEM%/libdso_no_symver.so
hdc file send %DYNLIB%\libdso_symver.so %REMOTESYSTEM%/libdso_symver.so
hdc file send %DYNLIB%\libldso_randomization_dep.so %REMOTEFEXT%/ldso_randomization/libldso_randomization_dep.so
hdc file send %DYNLIB%\libldso_randomization_dep_a.so %REMOTEFEXT%/ldso_randomization/libldso_randomization_dep_a.so
hdc file send %DYNLIB%\libldso_randomization_dep_b.so %REMOTEFEXT%/ldso_randomization/libldso_randomization_dep_b.so
hdc file send %DYNLIB%\libldso_randomization_dep_c.so %REMOTEFEXT%/ldso_randomization/libldso_randomization_dep_c.so
hdc file send %DYNLIB%\libldso_randomization_dep_d.so %REMOTEFEXT%/ldso_randomization/libldso_randomization_dep_d.so
hdc file send %DYNLIB%\libldso_randomization_dep_e.so %REMOTEFEXT%/ldso_randomization/libldso_randomization_dep_e.so
hdc file send %DYNLIB%\libdlopen_ext_relro_dso.so %REMOTEFEXT%/relro/libdlopen_ext_relro_dso.so

@REM 构造测试所需环境
hdc shell mkdir %REMOTE%/functionalext/dlns/B
hdc shell mkdir %REMOTE%/functionalext/dlns/C
hdc shell mkdir %REMOTE%/functionalext/dlns/D
hdc shell cp %REMOTEFEXT%/dlns/libdlopen_ns_dso.so %REMOTEFEXT%/dlns/sharedlibtest.so
hdc shell cp %REMOTEFEXT%/dlns/libdlopen_ns_dso.so %REMOTEFEXT%/dlns/B/libB.so
hdc shell cp %REMOTEFEXT%/dlns/libdlopen_ns_dso.so %REMOTEFEXT%/dlns/C/libC.so
hdc shell cp %REMOTEFEXT%/dlns/libdlopen_ns_dso.so %REMOTEFEXT%/dlns/D/libD.so
hdc shell cp %REMOTEFEXT%/dlns/libdlopen_ns_dso.so %REMOTEFEXT%/dlns/separated_0900.so
hdc shell cp %REMOTEFEXT%/dlns/libdlopen_ns_dso.so %REMOTEFEXT%/dlns/set_lib_path_0200.so
hdc shell cp %REMOTEFEXT%/dlns/libdlopen_ns_dso.so %REMOTEFEXT%/dlns/inherit_0300.so
hdc shell cp %REMOTEFEXT%/dlns/libdlopen_ns_dso.so %REMOTEFEXT%/dlns/inherit_0700.so
hdc shell cp %REMOTEFEXT%/dlns/libdlopen_ns_dso.so %REMOTEFEXT%/dlns/inherit_0800.so
hdc shell cp %REMOTEFEXT%/dlns/libdlopen_ns_dso.so %REMOTEFEXT%/dlns/inherit_1100.so
hdc shell mv /etc/ld-musl-namespace-arm.ini /etc/ld-musl-namespace-arm-temp.ini
hdc shell mv /etc/ld-musl-namespace-arm-test.ini /etc/ld-musl-namespace-arm.ini


@REM 修改动态库权限
hdc shell chmod a+x	%REMOTE%/functional/libdlopen_dso.so
hdc shell chmod a+x	%REMOTE%/functional/libdlopen_ns_dso.so
hdc shell chmod a+x %REMOTE%/functional/libdlclose_reset_dso.so
hdc shell chmod a+x	%REMOTE%/functional/libtls_align_dso.so
hdc shell chmod a+x	%REMOTE%/functional/libtls_init_dso.so
hdc shell chmod a+x %REMOTE%/regression/libtls_get_new-dtv_dso.so
echo Test cases sending finished.
echo.
goto sendSH


@REM 发送脚本并执行用例
:sendSH
echo Sending runtest.sh
hdc file send %SHDIR%\runtest.sh %REMOTE%/runtest.sh
hdc shell chmod a+x %REMOTE%/runtest.sh
echo runtest.sh has been transported.
echo.
echo hdc shell .%REMOTE%/runtest.sh
hdc shell .%REMOTE%/runtest.sh
echo.
echo ================================
echo The test cases have been executed.

@REM 恢复现场
echo Start restore the scene
echo hdc shell mv /etc/ld-musl-namespace-arm.ini /etc/ld-musl-namespace-arm-test.inherit_0300
echo ...
hdc shell mv /etc/ld-musl-namespace-arm.ini /etc/ld-musl-namespace-arm-test.ini
hdc shell mv /etc/ld-musl-namespace-arm-temp.ini /etc/ld-musl-namespace-arm.ini
hdc shell rm %REMOTEFEXT%/dlns/sharedlibtest.so -rf
hdc shell rm %REMOTEFEXT%/dlns/B/libB.so -rf
hdc shell rm %REMOTEFEXT%/dlns/C/libC.so -rf
hdc shell rm %REMOTEFEXT%/dlns/D/libD.so -rf
hdc shell rm %REMOTEFEXT%/dlns/separated_0900.so -rf
hdc shell rm %REMOTEFEXT%/dlns/set_lib_path_0200.so -rf
hdc shell rm %REMOTEFEXT%/dlns/inherit_0300.so -rf
hdc shell rm %REMOTEFEXT%/dlns/inherit_0700.so -rf
hdc shell rm %REMOTEFEXT%/dlns/inherit_0800.so -rf
hdc shell rm %REMOTEFEXT%/dlns/inherit_1100.so -rf
hdc shell rm /dev/shm -rf
echo.
echo hdc file recv %REMOTE%/REPORT %~dp0REPORT
hdc file recv %REMOTE%/REPORT %~dp0REPORT
goto end

@REM 提示检查设备连接。
:noDevice
echo Device not found,please check your device.
goto end

@REM 完成所用时间
:end
echo.
set /a endS=%time:~6,2%
set /a endM=%time:~3,2%
set /a diffS_=%endS%-%startS%
set /a diffM_=%endM%-%startM%

@REM REPORT文件比较 
start python %LOCAL%\third_party\musl\scripts\compare.py
if exist "%LOCAL%\third_party\musl\scripts\\result.html" (
  echo Test failed,please checking result.html!
) else (
  echo Test successful!
)  
echo file compareing finished

echo All items finished.
echo Time cost:%diffM_%m%diffS_%s .
echo.
pause
exit