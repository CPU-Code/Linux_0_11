<!--
 * @由于个人水平有限, 难免有些错误, 还请指点:  
 * @Author: cpu_code
 * @Date: 2020-08-13 22:49:08
 * @LastEditTime: 2020-08-19 18:38:27
 * @FilePath: \Linux_0_11\readme.md
 * @Gitee: [https://gitee.com/cpu_code](https://gitee.com/cpu_code)
 * @Github: [https://github.com/CPU-Code](https://github.com/CPU-Code)
 * @CSDN: [https://blog.csdn.net/qq_44226094](https://blog.csdn.net/qq_44226094)
 * @Gitbook: [https://923992029.gitbook.io/cpucode/](https://923992029.gitbook.io/cpucode/)
-->

# linux_0_11

linux 0.11 的内核刨析解析, 代码全部注释

-------------

## [boot__](boot)

- [ ] [boottsect__磁盘引导块](boot/boottsect.S)
- [ ] [head__](boot/head.s)
- [ ] [setup__操作系统加载](boot/setup.S)

----------------

## [fs__](fs)

- [ ] [bitmap__](fs/bitmap.c)
- [ ] [block_dev](fs/block_dev.c)
- [ ] [buffer](fs/buffer.c)
- [ ] [char_dev](fs/char_dev.c)
- [ ] [exec](fs/exec.c)
- [ ] [fcntl](fs/fcntl.c)
- [ ] [file_dev](fs/file_dev.c)
- [ ] [file_table](fs/file_table.c)
- [ ] [inode](fs/inode.c)
- [ ] [ioctl](fs/ioctl.c)
- [ ] [namei](fs/namei.c)
- [ ] [open](fs/open.c)
- [ ] [pipe](fs/pipe.c)
- [ ] [read_write](fs/read_write.c)
- [ ] [select](fs/select.c)
- [ ] [stat](fs/stat.c)
- [ ] [super](fs/super.c)
- [ ] [truncate](fs/truncate.c)

-----------------

## [include__](include)

- [ ] [a.out](include/a.out.h)
- [ ] [const](include/const.h)
- [ ] [ctype](include/ctype.h)
- [ ] [errno](include/errno.h)
- [ ] [fcntl](include/fcntl.h)
- [ ] [signal](include/signal.h)
- [ ] [stdarg](include/stdarg.h)
- [ ] [stddef](include/stddef.h)
- [ ] [string](include/string.h)
- [ ] [termios](include/termios.h)
- [ ] [time](include/time.h)
- [ ] [unistd](include/unistd.h)
- [ ] [utime](include/utime.h)

### [asm](include/asm)

- [ ] [io](include/asm/io.h)
- [ ] [memory](include/asm/memory.h)
- [ ] [segment](include/asm/segment.h)
- [ ] [systen](include/asm/systen.h)

### [linux](include/linux)

- [ ] [config](include/linux/config.h)
- [ ] [fdreg](include/linux/fdreg.h)
- [ ] [fs](include/linux/fs.h)
- [ ] [hdreg](include/linux/hdreg.h)
- [ ] [head](include/linux/head.h)
- [ ] [kernel](include/linux/kernel.h)
- [ ] [math_emu](include/linux/math_emu.h)
- [ ] [mm](include/linux/mm.h)
- [ ] [sched](include/linux/sched.h)
- [ ] [sys](include/linux/sys.h)
- [ ] [tty](include/linux/tty.h)

### [sys](include/sys)

- [ ] [param](include/sys/param.h)
- [ ] [resource](include/sys/resource.h)
- [ ] [stat](include/sys/stat.h)
- [ ] [time](include/sys/time.h)
- [ ] [times](include/sys/times.h)
- [ ] [types](include/sys/types.h)
- [ ] [utsname](include/sys/utsname.h)
- [ ] [wait](include/sys/wait.h)

--------------

## [init__](init)

- [x] [main__内核初始化](init/main.c)
- [ ] [main](init/main.s)

-------------

## [kernel__](kernel)

- [x] [asm__探测异常故障处理](kernel/asm.s)
- [ ] [exit](kernel/exit.c)
- [ ] [fork](kernel/fork.c)
- [x] [mktime__时间](kernel/mktime.c)
- [ ] [panic](kernel/panic.c)
- [ ] [printk](kernel/printk.c)
- [ ] [sched](kernel/sched.c)
- [ ] [signal](kernel/signal.c)
- [ ] [sys](kernel/sys.c)
- [ ] [sys_call](kernel/sys_call.s)
- [ ] [traps__](kernel/traps.c)
- [ ] [vsprintf](kernel/vsprintf.c)

### [blk_drv](kernel/blk_drv)

- [ ] [blk](kernel/blk_drv/blk.h)
- [ ] [floppy](kernel/blk_drv/floppy.c)
- [ ] [hd](kernel/blk_drv/hd.c)
- [ ] [ll_rw_blk](kernel/blk_drv/ll_rw_blk.c)
- [ ] [ramdisk](kernel/blk_drv/ramdisk.c)

### [chr_drv](kernel/chr_drv)

- [ ] [console](kernel/chr_drv/console.c)
- [ ] [keyboard](kernel/chr_drv/keyboard.S)
- [ ] [pty](kernel/chr_drv/pty.c)
- [ ] [rs_io](kernel/chr_drv/rs_io.s)
- [ ] [serial](kernel/chr_drv/serial.c)
- [ ] [tty_io](kernel/chr_drv/tty_io.c)
- [ ] [tty_ioctl](kernel/chr_drv/tty_ioctl.c)

### [math](kernel/math)

- [ ] [add](kernel/math/add.c)
- [ ] [compare](kernel/math/compare.c)
- [ ] [convert](kernel/math/convert.c)
- [ ] [div](kernel/math/div.c)
- [ ] [ea](kernel/math/ea.c)
- [ ] [error](kernel/math/error.c)
- [ ] [get_put](kernel/math/get_put.c)
- [ ] [math_emulate](kernel/math/math_emulate.c)
- [ ] [mul](kernel/math/mul.c)

-----------------

## [lib__](lib)

- [x] [_exit__调用内核的退出系统调用](lib/_exit.c)
- [x] [close__文件关闭函数](lib/close.c)
- [x] [ctype__对字符进行类型判断](lib/ctype.c)
- [x] [dup__复制文件描述符](lib/dup.c)
- [x] [errno__出错号](lib/errno.c)
- [x] [execve__加载并执行子进程](lib/execve.c)
- [ ] [malloc](lib/malloc.c)
- [ ] [open](lib/open.c)
- [ ] [setsid](lib/setsid.c)
- [ ] [string](lib/string.c)
- [ ] [wait](lib/wait.c)
- [ ] [write](lib/write.c)

-------------

## [mm__](mm)

- [ ] [memory__](mm/memory.c)
- [ ] [page](mm/page.s)
- [ ] [swap](mm/swap.c)

-----------------

## [test__测试代码](test)

- [x] [boot__简单的引导扇区启动](test/boot.s)

--------------------

## [tools__](tools)

- [x] [build__生成内核磁盘映像文件](tools/build.c)

--------------------------
