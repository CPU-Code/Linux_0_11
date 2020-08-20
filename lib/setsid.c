/*
 * @由于个人水平有限, 难免有些错误, 还请指点:  
 * @Author: cpu_code
 * @Date: 2020-08-12 19:19:54
 * @LastEditTime: 2020-08-20 15:48:53
 * @FilePath: \Linux_0_11\lib\setsid.c
 * @Gitee: [https://gitee.com/cpu_code](https://gitee.com/cpu_code)
 * @Github: [https://github.com/CPU-Code](https://github.com/CPU-Code)
 * @CSDN: [https://blog.csdn.net/qq_44226094](https://blog.csdn.net/qq_44226094)
 * @Gitbook: [https://923992029.gitbook.io/cpucode/](https://923992029.gitbook.io/cpucode/)
 */
/*
 *  linux/lib/setsid.c
 *
 *  (C) 1991  Linus Torvalds
 */

#define __LIBRARY__

// 如定义了__LIBRARY__，则还含系统调用号和内嵌汇编_syscall0()等。
#include <unistd.h>// Linux 标准头文件。定义了各种符号常数和类型，并声明了各种函数。

// 创建一个会话并设置进程组号
// 下面系统调用宏对应于函数： 
// pid_t setsid()。
// 返回：调用进程的会话标识符(session ID)。
_syscall0(pid_t,setsid)
