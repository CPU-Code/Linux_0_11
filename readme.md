<!--
 * @由于个人水平有限, 难免有些错误, 还请指点:  
 * @Author: cpu_code
 * @Date: 2020-08-13 22:49:08
 * @LastEditTime: 2020-09-08 17:45:47
 * @FilePath: \Linux_0_11\readme.md
 * @Gitee: [https://gitee.com/cpu_code](https://gitee.com/cpu_code)
 * @Github: [https://github.com/CPU-Code](https://github.com/CPU-Code)
 * @CSDN: [https://blog.csdn.net/qq_44226094](https://blog.csdn.net/qq_44226094)
 * @Gitbook: [https://923992029.gitbook.io/cpucode/](https://923992029.gitbook.io/cpucode/)
-->

# linux_0_11

linux 0.11 的内核刨析解析, 代码全部注释

-------------

## [boot__引导启动](boot)

- [ ] [boottsect__磁盘引导块](boot/boottsect.S)
- [ ] [head__连接在 system 模块的最前部分](boot/head.s)
- [ ] [setup__操作系统加载](boot/setup.S)

----------------

## [fs__文件系统实现](fs)

- [ ] [bitmap__处理文件系统中 i 节点和逻辑数据块](fs/bitmap.c)
- [ ] [block_dev__块数据读和写](fs/block_dev.c)
- [ ] [buffer__管理高速缓冲区](fs/buffer.c)
- [ ] [char_dev__字符设备读写](fs/char_dev.c)
- [ ] [exec__执行程序函数 do_execve()](fs/exec.c)
- [ ] [fcntl__文件 i/o 控制](fs/fcntl.c)
- [ ] [file_dev__基于 i 节点和描述符结构的文件读写](fs/file_dev.c)
- [ ] [file_table__定义文件句柄结构数组](fs/file_table.c)
- [ ] [inode__文件系统 i 节点操作](fs/inode.c)
- [ ] [ioctl__实现字符设备的 io 控制](fs/ioctl.c)
- [ ] [namei__文件系统中目录名和文件名](fs/namei.c)
- [ ] [open__修改文件属性和创建与关闭文件](fs/open.c)
- [ ] [pipe__读写与创建管道](fs/pipe.c)
- [ ] [read_write__文件读/写和定位](fs/read_write.c)
- [ ] [select__处理多个文件进行 I/O 操作](fs/select.c)
- [ ] [stat__获取文件状态](fs/stat.c)
- [ ] [super__文件系统超级块的处理](fs/super.c)
- [ ] [truncate__删除文件时释放文件所占用的设备数据](fs/truncate.c)

-----------------

## [include__头文件](include)

- [ ] [a.out__执行文件格式和一些宏](include/a.out.h)
- [ ] [const__常数符号](include/const.h)
- [ ] [ctype__字符类型](include/ctype.h)
- [ ] [errno__错误号](include/errno.h)
- [ ] [fcntl__文件控制](include/fcntl.h)
- [ ] [signal__信号](include/signal.h)
- [ ] [stdarg__标准参数](include/stdarg.h)
- [ ] [stddef__标准定义](include/stddef.h)
- [ ] [string__字符串](include/string.h)
- [ ] [termios__终端输入输出](include/termios.h)
- [ ] [time__时间类型](include/time.h)
- [ ] [unistd__Linux 标准](include/unistd.h)
- [ ] [utime__用户时间](include/utime.h)

### [asm__CPU 体系结构](include/asm)

- [ ] [io__ io 端口操作](include/asm/io.h)
- [ ] [memory__内存拷贝](include/asm/memory.h)
- [ ] [segment__段操作](include/asm/segment.h)
- [ ] [systen__系统](include/asm/systen.h)

### [linux__Linux 内核专用](include/linux)

- [ ] [config__内核配置](include/linux/config.h)
- [ ] [fdreg__软驱](include/linux/fdreg.h)
- [ ] [fs__文件系统](include/linux/fs.h)
- [ ] [hdreg__硬盘参数](include/linux/hdreg.h)
- [ ] [head__段描述符](include/linux/head.h)
- [ ] [kernel__内核](include/linux/kernel.h)
- [ ] [math_emu__数学协处理器仿真](include/linux/math_emu.h)
- [ ] [mm__内存管理](include/linux/mm.h)
- [ ] [sched__调度程序](include/linux/sched.h)
- [ ] [sys__系统调用](include/linux/sys.h)
- [ ] [tty__串行通信](include/linux/tty.h)

### [sys__系统数据结构](include/sys)

- [ ] [param__参数值](include/sys/param.h)
- [ ] [resource__系统资源的界限限制和利用率](include/sys/resource.h)
- [ ] [stat__文件状态](include/sys/stat.h)
- [ ] [time__定义 timeval 结构和 itimerval 结构](include/sys/time.h)
- [ ] [times__运行时间结构 tms](include/sys/times.h)
- [ ] [types__系统数据类型](include/sys/types.h)
- [ ] [utsname__系统名称结构](include/sys/utsname.h)
- [ ] [wait__等待调用](include/sys/wait.h)

--------------

## [init__内核初始化](init)

- [x] [main__内核初始化](init/main.c)
- [ ] [main](init/main.s)

-------------

## [kernel__内核进程调度、信号处理、系统调用](kernel)

- [x] [asm__探测异常故障处理](kernel/asm.s)
- [ ] [exit__处理进程终止](kernel/exit.c)
- [ ] [fork__sys_fork()](kernel/fork.c)
- [x] [mktime__时间函数 mktime()](kernel/mktime.c)
- [ ] [panic__显示内核出错信息并停机](kernel/panic.c)
- [ ] [printk__内核专用信息显示](kernel/printk.c)
- [ ] [sched__调度函数](kernel/sched.c)
- [ ] [signal__信号处理](kernel/signal.c)
- [ ] [sys__系统调用](kernel/sys.c)
- [ ] [sys_call__Linux 系统调用](kernel/sys_call.s)
- [ ] [traps__处理异常故障](kernel/traps.c)
- [ ] [vsprintf__字符串格式化](kernel/vsprintf.c)

### [blk_drv__块设备驱动](kernel/blk_drv)

- [ ] [blk__块设备结构和数据块请求结构](kernel/blk_drv/blk.h)
- [ ] [floppy__对软盘数据块的读/写](kernel/blk_drv/floppy.c)
- [ ] [hd__对硬盘数据块进行读/写](kernel/blk_drv/hd.c)
- [ ] [ll_rw_blk__低层块设备数据读/写](kernel/blk_drv/ll_rw_blk.c)
- [ ] [ramdisk__内存虚拟盘](kernel/blk_drv/ramdisk.c)

### [chr_drv__字符设备](kernel/chr_drv)

- [ ] [console__控制台初始化和写](kernel/chr_drv/console.c)
- [ ] [keyboard__键盘中断处理](kernel/chr_drv/keyboard.S)
- [ ] [pty](kernel/chr_drv/pty.c)
- [ ] [rs_io__串行接口的中断处理](kernel/chr_drv/rs_io.s)
- [ ] [serial__UART初始化](kernel/chr_drv/serial.c)
- [ ] [tty_io__tty 字符设备读/写](kernel/chr_drv/tty_io.c)
- [ ] [tty_ioctl__ io 控制接口](kernel/chr_drv/tty_ioctl.c)

### [math__数学协处理器仿真处理](kernel/math)

- [ ] [add__加法运算](kernel/math/add.c)
- [ ] [compare__比较累加器中两个临时实数的大小](kernel/math/compare.c)
- [ ] [convert__用户数据格式与临时实数格式之间的数据类型转换](kernel/math/convert.c)
- [ ] [div__除法运算](kernel/math/div.c)
- [ ] [ea__浮点计算指令的有效地址值](kernel/math/ea.c)
- [ ] [error__出错信号](kernel/math/error.c)
- [ ] [get_put__对用户内存中数据的访问](kernel/math/get_put.c)
- [ ] [math_emulate__异常中断处理/浮点指令仿真](kernel/math/math_emulate.c)
- [ ] [mul__乘法指令](kernel/math/mul.c)

-----------------

## [lib__内核库函数](lib)

- [x] [_exit__调用内核的退出系统调用](lib/_exit.c)
- [x] [close__文件关闭函数](lib/close.c)
- [x] [ctype__对字符进行类型判断](lib/ctype.c)
- [x] [dup__复制文件描述符](lib/dup.c)
- [x] [errno__出错号](lib/errno.c)
- [x] [execve__加载并执行子进程](lib/execve.c)
- [ ] [malloc__内存分配](lib/malloc.c)
- [x] [open__打开文件](lib/open.c)
- [x] [setsid__创建新会话](lib/setsid.c)
- [x] [string__空声明](lib/string.c)
- [x] [wait__等待进程终止](lib/wait.c)
- [x] [write__写文件](lib/write.c)

-------------

## [mm__内存管理](mm)

- [ ] [memory__内存进行初始化](mm/memory.c)
- [ ] [page__内存页面异常中断](mm/page.s)
- [ ] [swap__管理主内存中物理页面和高速二级存储空间之间的页面交换](mm/swap.c)

-----------------

## [test__测试代码](test)

- [x] [boot__简单的引导扇区启动](test/boot.s)

--------------------

## [tools__生成内核 Image 文件的工具](tools)

- [x] [build__生成内核磁盘映像文件](tools/build.c)

--------------------------
