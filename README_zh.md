# Musl libc

musl，是MIT授权的针对Linux系统调用的API的标准C库实现，适用于广泛的部署环境。musl提供了高效的静态和动态链接支持， 为轻量级代码并且运行时开销低，在正确使用时具有强大的故障安全保证，以及标准一致性意义上的正确性和安全性。musl建立的原则是使用便于理解和维护的简单代码来实现目标。

musl的1.1版本系列涵盖了所有在ISO C99和POSIX 2008基础中定义的接口，以及一些非标准的为了与Linux、BSD和glibc功能兼容的接口。

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
-  mallocng堆内存分配器安全增强，默认开启meta指针混淆。地址随机化通过MALLOC_SECURE_ALL宏开关。可在编译命令中增加--gn-args="musl_secure_level=3"开启

等。在新增特性基础上，也进行了对于musl接口功能的完善与错误的修复。

## Musl使用文档

有关完整musl目标编译器工具链、系统的信息bootstrapping和基于musl构建的Linux发行版可以在官方网站上找到：http://www.musl-libc.org/

有关基本安装说明，请参阅附带的[INSTALL](https://gitee.com/openharmony/third_party_musl/blob/master/INSTALL)文件。

musl参考手册：https://musl.libc.org/doc/1.1.24/manual.html