/*
 * @由于个人水平有限, 难免有些错误, 还请指点:  
 * @Author: cpu_code
 * @Date: 2020-08-12 19:19:54
 * @LastEditTime: 2020-08-19 20:03:22
 * @FilePath: \Linux_0_11\lib\open.c
 * @Gitee: [https://gitee.com/cpu_code](https://gitee.com/cpu_code)
 * @Github: [https://github.com/CPU-Code](https://github.com/CPU-Code)
 * @CSDN: [https://blog.csdn.net/qq_44226094](https://blog.csdn.net/qq_44226094)
 * @Gitbook: [https://923992029.gitbook.io/cpucode/](https://923992029.gitbook.io/cpucode/)
 */
/*
 *  linux/lib/open.c
 *
 *  (C) 1991  Linus Torvalds
 */

#define __LIBRARY__

// 如定义了__LIBRARY__，则还含系统调用号和内嵌汇编_syscall0()等
#include <unistd.h>		// Linux 标准头文件。定义了各种符号常数和类型，并声明了各种函数

// 标准参数头文件。以宏的形式定义变量参数列表。
// 主要说明了-个 类型(va_list) 和 三个宏(va_start, va_arg 和 va_end)，
// 用于 vsprintf、 vprintf、 vfprintf 函数
#include <stdarg.h>		

/**
 * @function:  打开一个文件或在文件不存在时创建一个文件
 * @parameter: 
 * 		filename - 文件名
 * 		flag - 文件打开标志
 * @return {type} 
 *     success: 文件描述符
 *     error: 置出错码，并返回-1
 * @note: 
 */
int open(const char * filename, int flag, ...)
{
	//定义了一个寄存器变量 res，该变量将被保存在一个寄存器中，以便于高效访问和操作
	// 指定存放的寄存器（例如 eax），就可写成“ register int res asm("ax"); ”。
	register int res;
	va_list arg;

	// 利用 va_start()宏函数，取得 flag 后面参数的指针，
	va_start(arg, flag);

	// 调用系统中断 int 0x80，功能 open 进行文件打开操作
	// %0 - eax(返回的描述符或出错码)； 
	// %1 - eax(系统中断调用功能号__NR_open)；
	// %2 - ebx(文件名 filename)； 
	// %3 - ecx(打开文件标志 flag)； 
	// %4 - edx(后随参数文件属性 mode)。
	__asm__("int $0x80"
		:"=a" (res)
		:"0" (__NR_open),"b" (filename),"c" (flag),
		"d" (va_arg(arg,int)));

	if (res >= 0)
	{
		// 系统中断调用返回值 >= 0，表示是一个文件描述符，则直接返回之
		return res;
	}
	
	// 小于 0，则代表一个出错码。设置该出错码
	errno = -res;
	
	//返回-1
	return -1;
}
