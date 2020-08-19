/*
 * @由于个人水平有限, 难免有些错误, 还请指点:  
 * @Author: cpu_code
 * @Date: 2020-08-12 19:19:54
 * @LastEditTime: 2020-08-19 17:39:10
 * @FilePath: \Linux_0_11\lib\_exit.c
 * @Gitee: [https://gitee.com/cpu_code](https://gitee.com/cpu_code)
 * @Github: [https://github.com/CPU-Code](https://github.com/CPU-Code)
 * @CSDN: [https://blog.csdn.net/qq_44226094](https://blog.csdn.net/qq_44226094)
 * @Gitbook: [https://923992029.gitbook.io/cpucode/](https://923992029.gitbook.io/cpucode/)
 */
/*
 *  linux/lib/_exit.c
 *
 *  (C) 1991  Linus Torvalds
 */

#define __LIBRARY__		// 定义一个符号常量

// 若定义了 __LIBRARY__ ，则还含 系统调用号 和 内嵌汇编 syscall0()等。
#include <unistd.h>		// Linux 标准头文件。定义了各种符号常数和类型，并声明了各种函数。

/**
 * @function: 内核使用的程序(退出)终止函数。
 * @parameter: 
 * 		exit_code - 退出码
 * @return {type} 
 *     success: 
 *     error: 
 * @note: 关键字 volatile 用于告诉编译器 gcc， 该函数不会返回。
 * 		这样可让 gcc 产生更好一些的代码，更重要的是使用这个关键字可以避免产生某些假警告信息。
 * 		等同于 gcc 的函数属性说明： void do_exit(int error_code) __attribute__ ((noreturn));
 */
volatile void _exit(int exit_code)
{
	
	// 直接调用系统中断 int 0x80，功能号 __NR_exit
	// %0 - eax(系统调用号 __NR_exit)
	// %1 - ebx(退出码 exit_code)
	__asm__("int $0x80"::"a" (__NR_exit),"b" (exit_code));
}
