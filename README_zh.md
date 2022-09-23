# Musl libc

musl，是MIT授权的针对Linux系统调用的标准C库的实现API，适用于广泛的部署环境。musl提供高效的静态和动态链接支持， 轻量级代码和低运行时开销、在正确的情况下强大的故障安全保证使用，以及在标准一致性意义上的正确性和安全。musl是在易于理解和维护的简单代码的原则上建立的。

针对musl的1.1版本系列涵盖了所有接口在ISO C99和POSIX 2008基础中定义，以及一些与Linux、BSD和glibc功能。

## 目录结构

```
README_zh.md            #中文说明
include					#musl头文件
ldso					#动态链接器
libc-test				#关于musl的测试用例
ndk_musl_include		#生成ndk版本的头文件
ndk_script				#生成ndk版本的脚本
porting                 #linux liteos平台对musl适配文件
src						#musl文件源
scripts					#测试用脚本
```

## Openharmony对于musl的适配

musl编译框架时编译，使用BUILD.gn进行配置编译。

基于openharmony的需求，为musl新增的特性：

-  加载器地址随机化，RELRO共享机制 
-  提供加载器namespace机制 
-  OHOS容器中能够运行依赖bionic的库 
-  musl全球化接口适配locale数据能力 

等。在新增特性基础上，也进行了对于musl接口功能的完善与错误的修复。

## Musl使用文档

有关完整musl目标编译器工具链、系统的信息bootstrapping和基于musl构建的Linux发行版可以在官方网站上找到：http://www.musl-libc.org/

有关基本安装说明，请参阅附带的[INSTALL](https://gitee.com/openharmony/third_party_musl/blob/master/INSTALL)文件。

musl参考手册：https://musl.libc.org/doc/1.1.24/manual.html