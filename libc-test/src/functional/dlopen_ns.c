#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include "test.h"

typedef void(* FUNC)(); // 定义函数指针类型的别名
const char* dllName = "libdlopen_ns_dso.so";
const char* dllName2 = "libdlopen_dso.so";
const char* errPath_ns = "src/common";

// 测试通过so路径打开. 命名空间非严格隔离时,通过路径总是可加载.
void test_open_by_path(const char* dllPath)
{
    void* handle = dlopen(dllPath, RTLD_LAZY);

    if(!handle)
        t_error("dlopen_by_path handle get error %s open failed: %s\n", dllName, dlerror());

    if(dlclose(handle))
        t_error("dlclose_by_path handle %s close failed : %s \n", dllName, dlerror());
}

//有配置文件时，通过短名称打开
void test_open_by_ini_name()
{
    Dl_namespace dlns;
    dlns_init(&dlns, "ns1");

    void* handle = dlopen_ns(&dlns, dllName, RTLD_LAZY);
    if(!dlns_create(&dlns, errPath_ns))
        t_error("dlns_create_by_ini_name should failed but succeed \n");
    if(!handle)
        t_error("dlopen_ns_by_ini_name handle get error %s open failed: %s\n", dllName, dlerror());

    if(dlclose(handle))
        t_error("dlclose_by_ini_name handle %s close failed : %s \n", dllName, dlerror());
}

//有配置文件时，测试通过api设置的namespace加载
void test_open_by_ini_api()
{
    Dl_namespace dlns1, dlns2;
    dlns_init(&dlns1, "dlns_test");
    dlns_init(&dlns2, "ns1");
    dlns_create(&dlns1, NULL);
    dlns_inherit(&dlns1, &dlns2 , dllName);

    void* handle = dlopen_ns(&dlns1, dllName, RTLD_LAZY);
    if(!handle)
        t_error("dlopen_ns_by_ini_api handle get error %s open failed: %s\n", dllName, dlerror());

    if(dlclose(handle))
        t_error("dlclose_by_ini_api handle %s close failed : %s \n", dllName, dlerror());
}

//不同命名空间打开相同so
void test_open_by_sameso_in_diff_ns(char * dllPath_ns)
{
    Dl_namespace dlns1, dlns2;
    dlns_init(&dlns1, "dlns_test1");
    dlns_init(&dlns2, "dlns_test2");
    dlns_create(&dlns1, dllPath_ns);
    dlns_create(&dlns2, dllPath_ns);

    void* handle1 = dlopen_ns(&dlns1, dllName, RTLD_LAZY);
    if(!handle1)
        t_error("dlopen_ns_by_sameso_diff_ns handle1 get error %s open failed : %s \n", dllName, dlerror());

    void* handle2 = dlopen_ns(&dlns2, dllName, RTLD_LAZY);
    if(!handle2)
        t_error("dlopen_ns_by_sameso_diff_ns handle2 get error %s open failed : %s \n", dllName, dlerror());

    if(dlclose(handle1))
        t_error("dlclose_by_sameso_diff_ns handle1 %s close failed : %s \n", dllName, dlerror());
    if(dlclose(handle2))
        t_error("dlclose_by_sameso_diff_ns handle2 %s close failed : %s \n", dllName, dlerror());
}

// 有配置文件时，测试在同一namespace多次加载相同动态库
void test_open_by_ini_mtso_ns()
{
    Dl_namespace dlns;
    dlns_init(&dlns, "ns1");
    void* handle1 = dlopen_ns(&dlns, dllName, RTLD_LAZY);
    if(!handle1)
        t_error("dlopen_ns_by_ini_mtso_ns handle1 get error %s open failed : %s \n" , dllName, dlerror());

    void* handle2 = dlopen_ns(&dlns, dllName, RTLD_LAZY);
    if(!handle2)
        t_error("dlopen_ns_by_ini_mtso_ns handle2 get error %s open failed : %s \n" , dllName, dlerror());

    if(dlclose(handle1) )
        t_error("dlclose_by_ini_mtso_ns handle1 %s close failed : %s \n" , dllName, dlerror());
    if(dlclose(handle2) )
        t_error("dlclose_by_ini_mtso_ns handle2 %s close failed : %s \n" , dllName, dlerror());
}

//测试配置文件,隔离性严格隔离 两个有allowed_libs(其中一个不在env lib permitt 路径中),一个没有allowed_libs
void test_open_ini_separated(char* dllPath)
{
    Dl_namespace dlns2, dlns3, dlns4;
    // ns2 , ns3 , ns4 为严格隔离情况
    // ns2 的 so 在 lib_path 下,设置了 allowed_path
    // ns3 的 lib_path 设置的错误路径,无 allowed_path.
    // ns4 的 在allowed_lib 下给的是错误的so
    dlns_init(&dlns2, "ns2");
    dlns_init(&dlns3, "ns3");
    dlns_init(&dlns4, "ns4");

    void* handle_ns2 = dlopen_ns(&dlns2, dllPath, RTLD_LAZY);
    if(!handle_ns2)
        t_error("dlopen_ns_by_ini_separated handle_ns2 get error %s open failed : %s \n", dllName, dlerror());

    void* handle_ns3 = dlopen_ns(&dlns3, dllPath, RTLD_LAZY);
    if(handle_ns3){
        t_error("dlopen_ns_by_ini_separated handle_ns3 %s should not open successfully \n", dllName);
        dlclose(handle_ns3);
    }

    if(dlclose(handle_ns2))
        t_error("dlclose_by_inherit_transitivity handle_ns2 %s close failed : %s\n", dllName, dlerror());

    void* handle_ns4 = dlopen_ns(&dlns4, dllPath, RTLD_LAZY);
    if(handle_ns4){
        t_error("dlopen_ns_by_ini_separated handle_ns4 %s should not open successfully \n", dllName);
        dlclose(handle_ns4);
    }
}

//测试配置文件,非严格隔离情况, 传入短名称
void test_open_ini_by_falseseparated(char * dllPath_ns)
{
    Dl_namespace dlns5, dlns6;
    //API 创建 ns5,ns6 为非严格隔离情况
    dlns_init(&dlns5, "ns5");
    dlns_init(&dlns6, "ns6");
    dlns_create(&dlns5, dllPath_ns);
    // ns6 传入lib_path 是错误路径,故打不开so
    dlns_create(&dlns6, errPath_ns);
    //根据短名称打开
    void* handle_ns5 = dlopen_ns(&dlns5, dllName, RTLD_LAZY);
    if(!handle_ns5)
        t_error("dlopen_ns_by_ini_by_falseseparated handle_ns5 get error %s open failed : %s \n", dllName, dlerror());

    void* handle_ns6 = dlopen_ns(&dlns6, dllName, RTLD_LAZY);
    if(handle_ns6){
        t_error("dlopen_ns_by_ini_separated handle_ns6 %s should not open successfully \n", dllName);
        dlclose(handle_ns6);
    }

    if(dlclose(handle_ns5))
        t_error("dlclose_by_ini_by_falseseparated handle_ns5 %s  close failed : %s\n", dllName, dlerror());


}

//检查继承是否具有传递性
void test_open_by_inherit_transitivity(char* dllPath_ns)
{
    Dl_namespace transitivity_A, transitivity_B, transitivity_C;
    dlns_init(&transitivity_A, "transitivity_A");
    dlns_init(&transitivity_B, "transitivity_B");
    dlns_init(&transitivity_C, "transitivity_C");
    dlns_create(&transitivity_A, NULL);
    dlns_create(&transitivity_B, NULL);
    dlns_create(&transitivity_C, dllPath_ns);
    dlns_inherit(&transitivity_A, &transitivity_B, NULL);
    dlns_inherit(&transitivity_B, &transitivity_C, dllName);
    //如果A能打开,证明有传递性

    void* handleA = dlopen_ns(&transitivity_A, dllName, RTLD_LAZY);
    if(handleA){
        t_error("dlopen_ns_by_inherit_transitivity handleA %s should not open successfully \n", dllName);
        dlclose(handleA);
    }

    void* handleB = dlopen_ns(&transitivity_B, dllName, RTLD_LAZY);
    if(!handleB)
        t_error("dlopen_ns_by_inherit_transitivity handleB get error %s open failed : %s \n", dllName, dlerror());

    void* handleC = dlopen_ns(&transitivity_C, dllName, RTLD_LAZY);
    if(!handleC)
        t_error("dlopen_ns_by_inherit_transitivity handleC get error %s open failed : %s \n", dllName, dlerror());

    if(dlclose(handleC))
        t_error("dlclose_by_inherit_transitivity handleC %s close failed : %s\n", dllName, dlerror());
    if(dlclose(handleB))
        t_error("dlclose_by_inherit_transitivity handleB %s close failed : %s\n", dllName, dlerror());

}

//在配置文件中A没有继承关系，A无法打开的库，通过调用继承接口去B（已加载该库）中尝试加载该库
void test_open_by_ini_no_inherit(char* dllPath_ns)
{
    Dl_namespace no_inherit_A, no_inherit_B;
    dlns_init(&no_inherit_A, "no_inherit_A");  //配置文件中A没有所继承的ns
    dlns_init(&no_inherit_B, "no_inherit_B");  //用于接口中实现与A的继承关系所对应的ns
    dlns_create(&no_inherit_A, NULL);
    dlns_create(&no_inherit_B, dllPath_ns);
    void* handle = dlopen_ns(&no_inherit_A, dllName, RTLD_LAZY);  //libdlopenns_dso.so在A中无法找到

    if(handle){
        t_error("dlopen_ns_by_ini_no_inherit handle %s should not open successfully \n", dllName);
        dlclose(handle);
    }

    void* handle2 = dlopen_ns(&no_inherit_B, dllName, RTLD_LAZY);
    if(!handle2)
        t_error("dlopen_ns_by_ini_no_inherit handle2 get error %s open failed : %s \n", dllName, dlerror());

    dlns_inherit(&no_inherit_A, &no_inherit_B, dllName);

    void* handle3 = dlopen_ns(&no_inherit_A, dllName, RTLD_LAZY);  //通过B的继承关系再次加载库

    if(!handle3)
       t_error("dlopen_ns_by_ini_no_inherit handle3 get error %s open failed : %s \n", dllName, dlerror());

    if(dlclose(handle3))
        t_error("dlclose_by_ini_no_inherit handle3 %s close failed : %s\n", dllName, dlerror());
    if(dlclose(handle2))
        t_error("dlclose_by_ini_no_inherit handle2 %s close failed : %s\n", dllName, dlerror());
}

//dlns_inherit是否正常工作
void test_open_by_dlns_inherir(char* dllPath_ns)
{
    Dl_namespace inherir_A, inherir_B;
    dlns_init(&inherir_A, "inherir_A");
    dlns_init(&inherir_B, "inherir_B");
    dlns_create(&inherir_A, NULL);
    dlns_create(&inherir_B, dllPath_ns);
    dlns_inherit(&inherir_A, &inherir_B, NULL);  //B共享所有库，有A要加载的库

    void* handle = dlopen_ns(&inherir_A, dllName, RTLD_LAZY);
    if(!handle)
        t_error("dlopen_ns_by_dlns_inherir handle get error %s open failed : %s \n", dllName, dlerror());

    if(dlclose(handle))
        t_error("dlclose_by_dlns_inherir handle %s close failed : %s\n", dllName, dlerror());

    dlns_inherit(&inherir_A, &inherir_B, dllName2);  //B共享指定库，不是A要加载的库,覆盖原有继承关系

    void* handle2 = dlopen_ns(&inherir_A, dllName, RTLD_LAZY);
    if(handle2){
        t_error("dlopen_ns_by_ini_no_inherit handle2 %s should not open successfully \n", dllName);
        dlclose(handle2);
    }
}

//A通过B、C共享项获取加载的库
void test_open_by_shared_libs(char* dllPath_ns)
{
    Dl_namespace shared_libs_A, shared_libs_B, shared_libs_C;
    dlns_init(&shared_libs_A, "shared_libs_A");
    dlns_init(&shared_libs_B, "shared_libs_B");
    dlns_init(&shared_libs_C, "shared_libs_C");
    dlns_create(&shared_libs_A, NULL);
    dlns_create(&shared_libs_B, errPath_ns);
    dlns_create(&shared_libs_C, dllPath_ns);
    dlns_inherit(&shared_libs_A, &shared_libs_B, NULL);  //B共享所有库，但无A要加载的库

    void* handle = dlopen_ns(&shared_libs_A, dllName, RTLD_LAZY);
    if(handle){
        t_error("dlopen_ns_by_shared_libs handle %s should not open successfully \n", dllName);
        dlclose(handle);
    }
    void* handle2 = dlopen_ns(&shared_libs_B, dllName, RTLD_LAZY);
    if(handle2){
        t_error("dlopen_ns_by_shared_libs handle2 %s should not open successfully \n", dllName);
        dlclose(handle2);
    }

    dlns_inherit(&shared_libs_A, &shared_libs_C, "libdlopen_dso.so:libdlopen_ns_dso.so");  //C共享固定库中包含A要加载的库

    void* handle3 = dlopen_ns(&shared_libs_C, dllName, RTLD_LAZY);
    if(!handle3)
        t_error("dlopen_ns_by_shared_libs handle3 get error %s open failed : %s \n", dllName, dlerror());

    void* handle4 = dlopen_ns(&shared_libs_A, dllName, RTLD_LAZY);
    if(!handle4)
        t_error("dlopen_ns_by_shared_libs handle4 get error %s open failed : %s \n", dllName, dlerror());

    if(dlclose(handle4))
        t_error("dlclose_by_shared_libs handle4 %s close failed : %s\n", dllName, dlerror());
    if(dlclose(handle3))
        t_error("dlclose_by_shared_libs handle3 %s close failed : %s\n", dllName, dlerror());
}


//add ini inherit test
void test_open_by_iinheritB_shared_libs(char* dllPath_ns)
{
    Dl_namespace inherit_B;
    dlns_init(&inherit_B, "inherit_B");  //配置文件中B继承自ns2
    void* handle = dlopen_ns(&inherit_B, dllName, RTLD_LAZY); //inherit_b将从ns2中获取dllName库

    if(!handle){
        t_error("dlopen_ns_by_iinheritB_shared_libs handle get error %s open failed : %s \n", dllName, dlerror());
    }

    if(dlclose(handle))
        t_error("dlopen_ns_by_iinheritB_shared_libs handle %s close failed : %s\n", dllName, dlerror());
}


int main(int argc, char *argv[])
{
    char buf[512],path[512];
    char* i;
    //带so的路径
    if (!t_pathrel(buf, sizeof buf, argv[0], "libdlopen_ns_dso.so")) {
        t_error("failed to obtain relative path to libdlopen_ns_dso.so\n");
        return 1;
	}
    //包含so的路径
    if (!t_pathrel(path, sizeof path, argv[0], "")) {
        t_error("failed to obtain relative path to path\n");
        return 1;
	}
    path[strlen (path) -1 ] ='\0';
    test_open_by_path(buf);
    test_open_by_ini_name();
    test_open_by_ini_api();
    test_open_ini_separated(buf);
    test_open_ini_by_falseseparated(path);
    test_open_by_dlns_inherir(path);
    test_open_by_ini_no_inherit(path);
    test_open_by_shared_libs(path);
    test_open_by_sameso_in_diff_ns(path);
    test_open_by_ini_mtso_ns();
    test_open_by_inherit_transitivity(path);
    test_open_by_iinheritB_shared_libs(path);

    return t_status;

}