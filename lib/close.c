/*
 * @由于个人水平有限, 难免有些错误, 还请指点:  
 * @Author: cpu_code
 * @Date: 2020-08-12 19:19:54
 * @LastEditTime: 2020-08-19 17:41:07
 * @FilePath: \Linux_0_11\lib\close.c
 * @Gitee: [https://gitee.com/cpu_code](https://gitee.com/cpu_code)
 * @Github: [https://github.com/CPU-Code](https://github.com/CPU-Code)
 * @CSDN: [https://blog.csdn.net/qq_44226094](https://blog.csdn.net/qq_44226094)
 * @Gitbook: [https://923992029.gitbook.io/cpucode/](https://923992029.gitbook.io/cpucode/)
 */
/*
 *  linux/lib/close.c
 *
 *  (C) 1991  Linus Torvalds
 */

#define __LIBRARY__

// 如定义了__LIBRARY__，则还含系统调用号和内嵌汇编 syscall0()等。
#include <unistd.h>     // Linux 标准头文件。定义了各种符号常数和类型，并声明了各种函数。

// 关闭文件函数。
// 下面该宏对应函数原型： 
// int close(int fd)。 
// 它直接调用系统中断 int 0x80，参数是__NR_close。
// fd : 文件描述符。
_syscall1(int,close,int,fd)
