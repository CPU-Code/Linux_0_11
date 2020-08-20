/*
 * @由于个人水平有限, 难免有些错误, 还请指点:  
 * @Author: cpu_code
 * @Date: 2020-08-12 19:19:54
 * @LastEditTime: 2020-08-20 16:00:17
 * @FilePath: \Linux_0_11\lib\wait.c
 * @Gitee: [https://gitee.com/cpu_code](https://gitee.com/cpu_code)
 * @Github: [https://github.com/CPU-Code](https://github.com/CPU-Code)
 * @CSDN: [https://blog.csdn.net/qq_44226094](https://blog.csdn.net/qq_44226094)
 * @Gitbook: [https://923992029.gitbook.io/cpucode/](https://923992029.gitbook.io/cpucode/)
 */
/*
 *  linux/lib/wait.c
 *
 *  (C) 1991  Linus Torvalds
 */

#define __LIBRARY__

// Linux 标准头文件。定义了各种符号常数和类型，并声明了各种函数。
// 如定义了__LIBRARY__，则还含系统调用号和内嵌汇编_syscall0()等
#include <unistd.h>

// 等待调用头文件。定义系统调用 wait()和 waitpid()及相关常数符号
#include <sys/wait.h>

/**
 * @function: 等待进程终止系统调用函数
 * @parameter: 
 * 		pid - 等待被终止进程的进程 id，或 用于指定特殊情况的其他特定数值；
 * 		wait_stat - 用于存放状态信息； 
 * 		options - WNOHANG 或 WUNTRACED 或是 0。
 * @return {type} 
 *     success: 
 *     error: 
 * @note: 函数： pid_t waitpid(pid_t pid, int * wait_stat, int options)
 */
_syscall3(pid_t,waitpid,pid_t,pid,int *,wait_stat,int,options)

// wait()系统调用
pid_t wait(int * wait_stat)
{
	// 直接调用 waitpid()函数
	return waitpid(-1, wait_stat, 0);
}
