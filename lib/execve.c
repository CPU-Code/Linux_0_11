/*
 * @由于个人水平有限, 难免有些错误, 还请指点:  
 * @Author: cpu_code
 * @Date: 2020-08-12 19:19:54
 * @LastEditTime: 2020-08-19 18:37:45
 * @FilePath: \Linux_0_11\lib\execve.c
 * @Gitee: [https://gitee.com/cpu_code](https://gitee.com/cpu_code)
 * @Github: [https://github.com/CPU-Code](https://github.com/CPU-Code)
 * @CSDN: [https://blog.csdn.net/qq_44226094](https://blog.csdn.net/qq_44226094)
 * @Gitbook: [https://923992029.gitbook.io/cpucode/](https://923992029.gitbook.io/cpucode/)
 */
/*
 *  linux/lib/execve.c
 *
 *  (C) 1991  Linus Torvalds
 */

#define __LIBRARY__

// 如定义了__LIBRARY__，则还含系统调用号和内嵌汇编_syscall0()等
#include <unistd.h>         // Linux 标准头文件。定义了各种符号常数和类型，并声明了各种函数

/**
 * @function: 加载并执行子进程(其他程序)函数
 * @parameter: 
 *      file - 被执行程序文件名
 *      argv - 命令行参数指针数组
 *      envp - 环境变量指针数组
 * @return {type} 
 *     success: 
 *     error: 
 * @note: 调用宏函数对应： int execve(const char * file, char ** argv, char ** envp)
 *        直接调用了系统中断 int 0x80 ，参数是 __NR_execve。参见 include/unistd.h 和 fs/exec.c
 */
_syscall3(int,execve,const char *,file,char **,argv,char **,envp)
