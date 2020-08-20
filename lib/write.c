/*
 * @由于个人水平有限, 难免有些错误, 还请指点:  
 * @Author: cpu_code
 * @Date: 2020-08-12 19:19:54
 * @LastEditTime: 2020-08-20 15:58:53
 * @FilePath: \Linux_0_11\lib\write.c
 * @Gitee: [https://gitee.com/cpu_code](https://gitee.com/cpu_code)
 * @Github: [https://github.com/CPU-Code](https://github.com/CPU-Code)
 * @CSDN: [https://blog.csdn.net/qq_44226094](https://blog.csdn.net/qq_44226094)
 * @Gitbook: [https://923992029.gitbook.io/cpucode/](https://923992029.gitbook.io/cpucode/)
 */
/*
 *  linux/lib/write.c
 *
 *  (C) 1991  Linus Torvalds
 */

#define __LIBRARY__

// Linux 标准头文件。定义了各种符号常数和类型，并声明了各种函数。
// 如定义了__LIBRARY__，则还含系统调用号和内嵌汇编_syscall0()等。
#include <unistd.h>

/**
 * @function: 写文件系统调用函数
 * @parameter: 
 *      fd - 文件描述符
 *      buf - 写缓冲区指针
 *      count - 写字节数
 * @return {type} 
 *     success: 写入的字节数(0 表示写入 0 字节)
 *     error: -1，并且设置了出错号
 * @note: 函数： 
 *   int write(int fd, const char * buf, off_t count)
 */
_syscall3(int,write,int,fd,const char *,buf,off_t,count)
