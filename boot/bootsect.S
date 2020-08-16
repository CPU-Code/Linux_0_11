/*
 * @由于个人水平有限, 难免有些错误, 还请指点:  
 * @Author: cpu_code
 * @Date: 2020-08-12 19:19:54
 * @LastEditTime: 2020-08-15 21:43:27
 * @FilePath: \Linux_0_11\boot\bootsect.S
 * @Gitee: [https://gitee.com/cpu_code](https://gitee.com/cpu_code)
 * @Github: [https://github.com/CPU-Code](https://github.com/CPU-Code)
 * @CSDN: [https://blog.csdn.net/qq_44226094](https://blog.csdn.net/qq_44226094)
 * @Gitbook: [https://923992029.gitbook.io/cpucode/](https://923992029.gitbook.io/cpucode/)
 */
!
! SYS_SIZE is the number of clicks (16 bytes) to be loaded.
! 0x3000 is 0x30000 bytes = 196kB, more than enough for current
! versions of linux
; SYS_SIZE 是要加载的系统模块长度。 长度单位是节（ paragraph ） ，每节 16 字节。 
; 这里 0x3000 共为 0x30000 字节 = 196KB。 若以 1024 字节为 1KB 计，则应该是 192KB。
; 当该值为 0x8000 时，表示内核最大为 512KB。
; 因为内存 0x90000 处开始存放移动后的 bootsect 和 setup 的代码，因此该值最大不得超过 0x9000（表示 584KB）。
!
; 头文件 linux/config.h 中定义了内核用到的一些常数符号和 Linus 自己使用的默认硬盘参数块。
; 例如其中定义了以下一些常数：
; DEF_SYSSIZE = 0x3000 - 默认系统模块长度。单位是节，每节为 16 字节；
; DEF_INITSEG = 0x9000 - 默认本程序代码移动目的段位置；
; DEF_SETUPSEG = 0x9020 - 默认 setup 程序代码段位置；
; DEF_SYSSEG = 0x1000 - 默认从磁盘加载系统模块到内存的段位置。
#include <linux/config.h>
; 定义一个标号或符号。指明编译连接后 system 模块的大小。
SYSSIZE = DEF_SYSSIZE
!
!	bootsect.s		(C) 1991 Linus Torvalds
!	modified by Drew Eckhardt 
; Drew Eckhardt 修改
!
! bootsect.s is loaded at 0x7c00 by the bios-startup routines, and moves
! iself out of the way to address 0x90000, and jumps there.
; bootsect.s 被 ROM BIOS 启动子程序加载至 0x7c00 (31KB)处，并将自己移到了地址 0x90000 (576KB)处，并跳转至那里。
!
! It then loads 'setup' directly after itself (0x90200), and the system
! at 0x10000, using BIOS interrupts. 
; 它然后使用 BIOS 中断将'setup'直接加载到自己的后面(0x90200)(576.5KB)，并将 system 加载到地址 0x10000 处。
!
! NOTE! currently system is at most 8*65536 bytes long. This should be no
! problem, even in the future. I want to keep it simple. This 512 kB
! kernel size should be enough, especially as this doesn't contain the
! buffer cache as in minix
; 注意! 目前的内核系统最大长度限制为 (8 * 65536)(512KB) 字节，即使是在将来这也应该没有问题的。
; 我想让它保持简单明了。这样 512KB 的最大内核长度应该足够了, 尤其是这里没有象MINIX 中一样包含缓冲区高速缓冲。
!
! The loader has been made as simple as possible, and continuos
! read errors will result in a unbreakable loop. Reboot by hand. It
! loads pretty fast by getting whole sectors at a time whenever possible.
; 加载程序已经做得够简单了，所以持续地读操作出错将导致死循环。只能手工重启。
; 只要可能，通过一次读取所有的扇区，加载过程可以做得很快。

; 感叹号'!'或分号';' 为注释文字

; 伪指令（伪操作符） .globl 或.global 用于定义随后的标识符是外部的或全局的，即使不使用也会强制引入。
; .text、 .data 和.bss 用于分别定义 当前代码段、数据段 和 未初始化数据段。
; 在链接多个目标模块时，链接程序（ ld86） 会根据它们的类别把各个目标模块中的相应段分别 组合（合并）在一起。
; 这里把三个段都定义在同一重叠地址范围中，因此本程序实际上不分段。
; 另外，后面带冒号的字符串是标号，例如下面的'begtext:'。
; 一条汇编语句通常由 标号（可选）、指令助记符（指令名）和 操作数 三个字段组成。
; 标号位于一条指令的第一个字段。它代表其所在位置的地址，通常指明一个跳转指令的目标位置

; 汇编指示符（或称为汇编伪指令、伪操作符）
; begtext, begdata, begbss' 等标号就是它的操作数
.globl begtext, begdata, begbss, endtext, enddata, endbss

; 正文段
.text
begtext:

; 数据段
.data
begdata:

; 未初始化数据段
.bss
begbss:
.text
; 等号'='（ 或符号'EQU'）用于定义标识符 BOOTSEG所代表的值
; setup 程序代码占用磁盘扇区数(setup-sectors)值；
SETUPLEN = 4				! nr of setup-sectors
; bootsect 代码所在内存原始段地址；
BOOTSEG  = 0x07c0			! original address of boot-sector
; 将 bootsect 移到位置 0x90000 - 避开系统模块占用处；
INITSEG  = DEF_INITSEG			! we move boot here - out of the way
; setup 程序从内存 0x90200 处开始；
SETUPSEG = DEF_SETUPSEG			! setup starts here
; system 模块加载到 0x10000（ 64 KB） 处；
SYSSEG   = DEF_SYSSEG			! system loaded at 0x10000 (65536).
; 停止加载的段地址；
ENDSEG   = SYSSEG + SYSSIZE		! where to stop loading

! ROOT_DEV & SWAP_DEV are now written by "build".
; 根文件系统设备号 ROOT_DEV 和 交换设备号 SWAP_DEV 现在由 tools 目录下的 build 程序写入。
; 设备号 0x306 指定根文件系统设备是第 2 个硬盘的第 1 个分区。 
; 当年 Linus 是在第 2 个硬盘上安装了 Linux 0.11 系统，所以这里 ROOT_DEV 被设置为 0x306。
; 在编译这个内核时你可以根据自己根文件系统所在设备位置修改这个设备号。 
; 例如，若你的根文件系统在第 1 个硬盘的第 1 个分区上，那么该值应该为 0x0301，即（ 0x01, 0x03） 。 
;这个设备号是 Linux 系统老式的硬盘设备号命名方式，硬盘设备号具体值的含义如下：
; 设备号 = 主设备号 * 256 + 次设备号（也即 dev_no = (major << 8) + minor ）
; （主设备号： 1-内存, 2-磁盘, 3-硬盘, 4-ttyx, 5-tty, 6-并行口, 7-非命名管道）
; 0x300 - /dev/hd0 - 代表整个第 1 个硬盘；
; 0x301 - /dev/hd1 - 第 1 个盘的第 1 个分区；
; …
; 0x304 - /dev/hd4 - 第 1 个盘的第 4 个分区；
; 0x305 - /dev/hd5 - 代表整个第 2 个硬盘；
; 0x306 - /dev/hd6 - 第 2 个盘的第 1 个分区；
; …
; 0x309 - /dev/hd9 - 第 2 个盘的第 4 个分区；
; 从 Linux 内核 0.95 版后就已经使用与现在内核相同的命名方法了。

; 根文件系统设备使用与 系统引导时同样的设备；
ROOT_DEV = 0
; 交换设备使用与 系统引导时同样的设备；
SWAP_DEV = 0

; 伪指令 entry 迫使链接程序在生成的执行程序（ a.out）中包含指定的标识符或标号。这里是程序执行开始点。


; 迫使链接器 ld86 在生成的可执行文件中包括进其后指定的标号'start'
; 告知链接程序，程序从 start 标号开始执行。
entry start

start:
; 将自身(bootsect)从目前段位置 0x07c0(31KB) 移动到 0x9000(576KB) 处，共 256 字（ 512 字节），
; 然后跳转到移动后代码的 go 标号处，也即本程序的下一语句处。
	mov	ax,#BOOTSEG		; 将 ds 段寄存器置为 0x07c0 
	mov	ds,ax			; 初始化数据段寄存器 ds 和 es	
	mov	ax,#INITSEG		;  将 es 段寄存器置为 0x9000
	mov	es,ax
	mov	cx,#256			; 设置移动计数值 = 256 字（ 512 字节）
	sub	si,si			; 源地址 ds:si = 0x07C0:0x0000
	sub	di,di			; 目的地址 es:di = 0x9000:0x0000
	rep					; 重复执行并递减 cx 的值，直到 cx = 0 为止
	movw				;  movs 指令。从内存[si]处移动 cx 个字到[di]处
	; 段间（ Inter-segment） 远跳转语句，就跳转到下一条指令
	; INITSEG 指出跳转到的段地址， 标号 go 是段内偏移地址。
	jmpi	go,INITSEG

/* 从下面开始， CPU 在已移动到 0x90000 位置处的代码中执行。
这段代码设置几个段寄存器，包括 栈寄存器 ss 和 sp。
栈指针 sp 只要指向远大于 512 字节偏移（即地址 0x90200） 处都可以。
因为从 0x90200 地址开始处还要放置 setup 程序，而此时 setup程序大约为 4 个扇区，
因此 sp 要指向大于（ 0x200 + 0x200 * 4 + 堆栈大小）位置处。
这里 sp设置为 0x9ff00 - 12（参数表长度），即 sp = 0xfef4 。
在此位置之上会存放一个自建的驱动器参数表，见下面说明。
实际上 BIOS 把 引导扇区 加载到 0x7c00 处并把 执行权 交给 引导程 序时，ss = 0x00 ， sp = 0xfffe。
push ax 指令的期望作用是想暂时把 段值保留在栈 中，然后等下面执行完判断 磁道扇区数 后再弹出栈，并给 段寄存器 fs 和 gs 赋值 。
但是由于 mov ss,ax  mov sp,dx 修改了栈段的位置，
因此除非在执行 栈弹出 操作之前把 栈段恢复到原位置 ，否则这样设计就是错误的。
所以这里存在一个 bug。改正的方法之一是去掉 push	ax ，并把 后面的 pop ax 修改成 mov ax,cs 。
*/
go:	mov	ax,cs		;  将 ds 、 es 和 ss 都置成移动后代码所在的段处(0x9000)
	mov	dx,#0xfef4	! arbitrary value >>512 - disk parm size

	mov	ds,ax
	mov	es,ax
	push	ax	; 临时保存段值（ 0x9000），后面pop使用。

	mov	ss,ax		! put stack at 0x9ff00 - 12.
	mov	sp,dx
/*
 *	Many BIOS's default disk parameter tables will not 
 *	recognize multi-sector reads beyond the maximum sector number
 *	specified in the default diskette parameter tables - this may
 *	mean 7 sectors in some cases.
 *
 *	Since single sector reads are slow and out of the question,
 *	we must take care of this by creating new parameter tables
 *	(for the first disk) in RAM.  We will set the maximum sector
 *	count to 18 - the most we will encounter on an HD 1.44.  
 *
 *	High doesn't hurt.  Low does.
 *
 *	Segments are as follows: ds=es=ss=cs - INITSEG,
 *		fs = 0, gs = parameter table segment
 */
/*
* 对于多扇区读操作所读的扇区数 超过 默认磁盘参数表中指定的 最大扇区数 时，
* 很多 BIOS 将不能进行正确识别。在某些情况下是 7 个扇区。
*
* 由于单扇区读操作太慢，不予以考虑，因此我们必须通过在内存中重创建新的参数表（为第 1 个驱动器）来解决这个问题。
* 我们将把其中最大扇区数设置为 18 -- 即在 1.44MB 磁盘上会碰到的最大数值。
*
* 这个数值大了不会出问题，但是太小就不行了。
*
* 段寄存器将被设置成： ds=es=ss=cs - 都为 INITSEG（ 0x9000） ，
* fs = 0， gs = 参数表所在段值。
*/
/*
BIOS 设置的中断 0x1E 实际上并不是一个中断, 其对应中断向量的地方被放置了软驱参数表的地址。
该中断向量位于内存 0x1E * 4 = 0x78 处。
这段代码首先从内存 0x0000:0x0078 处复制 原软驱参数表 到 0x9000:0xfef4 处，
然后修改表中偏移 4 处的 每磁道最大扇区数为 18。 表长 12 字节。
*/
	push	#0		; 置段寄存器 fs = 0
	pop	fs			;  fs:bx 指向存有软驱参数表地址处（指针的指针）
	mov	bx,#0x78		! fs:bx is parameter table address
	/*
	指令 seg fs 表示其下一条语句的操作数在 fs 段中。 该指令仅影响其下一条语句。
	这里把 fs:bx 所指内存位置处的表地址放到寄存器对 gs:si 中作为原地址。
	寄存器对 es:di = 0x9000:0xfef4 作为目的地址。
	*/
	seg fs
	lgs	si,(bx)			! gs:si is source

	mov	di,dx			! es:di is destination
	mov	cx,#6			! copy 12 bytes
	cld					; 清方向标志。复制时指针递增

	rep					; 复制 12 字节的软驱参数表到 0x9000:0xfef4 处
	seg gs	
	movw

	mov	di,dx			;  es:di 指向新表，然后修改表中偏移 4 处的最大扇区数
	movb	4(di),*18		! patch sector count

	seg fs				; 让中断向量 0x1E 的值指向新表
	mov	(bx),di
	seg fs
	mov	2(bx),es

	pop	ax				; 此时 ax 中是上面 push	ax 保留下来的段值（ 0x9000） 
	mov	fs,ax			; 设置 fs = gs = 0x9000，恢复原段值
	mov	gs,ax
	
	xor	ah,ah			;复位软盘控制器，让其采用新参数	! reset FDC 
	xor	dl,dl			; dl = 0，第 1 个软驱
	int 	0x13	

! load the setup-sectors directly after the bootblock.
! Note that 'es' is already set up.
/* 
在 bootsect 程序块后紧根着加载 setup 模块的代码数据。
注意 es 已经设置好了。（在移动代码时 es 已经指向目的段地址处 0x9000） 。
xor	dx, dx --> jnc	ok_load_setup 利用 ROM BIOS 中断 INT 0x13 
将 setup 模块从磁盘第 2 个扇区开始读到 0x90200 开始处，共读 4 个扇区。
在读操作过程中如果读出错，则显示磁盘上出错扇区位置，然后复位驱动器并重试，没有退路。

INT 0x13 读扇区使用调用参数设置如下：
ah = 0x02 - 读磁盘扇区到内存； 
al = 需要读出的扇区数量；
ch = 磁道(柱面)号的低 8 位； 
cl = 开始扇区(位 0-5)，磁道号高 2 位(位 6-7)；
dh = 磁头号； 
dl = 驱动器号（如果是硬盘则位 7 要置位）；
es:bx -> 指向数据缓冲区； 如果出错则 CF 标志置位， ah 中是出错码。
*/
load_setup:
	xor	dx, dx			! drive 0, head 0
	mov	cx,#0x0002		! sector 2, track 0
	mov	bx,#0x0200		! address = 512, in INITSEG
	mov	ax,#0x0200+SETUPLEN	! service 2, nr of sectors
	int	0x13			! read it
	jnc	ok_load_setup		! ok - continue

	push	ax			; 显示出错信息。出错码入栈 ! dump error code
	call	print_nl	; 屏幕光标回车
	mov	bp, sp			; ss:bp 指向欲显示的字 （ word）
	call	print_hex	; 显示十六进制值
	pop	ax	
	
	xor	dl, dl			; 复位磁盘控制器，重试 ! reset FDC
	xor	ah, ah
	int	0x13
	j	load_setup		; j 即 jmp 指令

ok_load_setup:

! Get disk drive parameters, specifically nr of sectors/track
/*
这段代码利用 BIOS INT 0x13 功能 8 来取磁盘驱动器的参数。 实际是取每磁道扇区数，并保存在位置 sectors 处。
取磁盘驱动器参数 INT 0x13 调用格式和返回信息如下：
ah = 0x08 dl = 驱动器号（如果是硬盘则要置位 7 为 1） 。
返回信息：
如果出错则 CF 置位，并且 ah = 状态码。
ah = 0， al = 0， bl = 驱动器类型（ AT/PS2）
ch = 最大磁道号的低 8 位， cl = 每磁道最大扇区数(位 0-5)，最大磁道号高 2 位(位 6-7)
dh = 最大磁头数， dl = 驱动器数量，
es:di -> 软驱磁盘参数表。
*/
	xor	dl,dl
	mov	ah,#0x08		! AH=8 is get drive parameters
	int	0x13
	xor	ch,ch
	/*
	下面指令表示下一条语句的操作数在 cs 段寄存器所指的段中。 它只影响其下一条语句。
	实际上，由于本程序代码和数据都被设置处于同一个段中，即段寄存器 cs 和 ds、 es 的值相同，
	因此本程序中此处可以不使用该指令。
	*/
	seg cs
	/*
	下句保存每磁道扇区数。 对于软盘来说（ dl=0），其最大磁道号不会超过 256， ch 已经足够表示它，
	因此 cl 的位 6-7 肯定为 0。 又 146 行已置 ch=0，因此此时 cx 中是每磁道扇区数。
	*/
	mov	sectors,cx
	mov	ax,#INITSEG
	mov	es,ax			; 因为上面取磁盘参数中断改了 es 值，这里重新改回。

! Print some inane message

	mov	ah,#0x03		! read cursor pos
	xor	bh,bh
	int	0x10
	
	mov	cx,#9
	mov	bx,#0x0007		! page 0, attribute 7 (normal)
	mov	bp,#msg1
	mov	ax,#0x1301		! write string, move cursor
	int	0x10

! ok, we've written the message, now
! we want to load the system (at 0x10000)

	mov	ax,#SYSSEG
	mov	es,ax		! segment of 0x010000
	call	read_it
	call	kill_motor
	call	print_nl

! After that we check which root-device to use. If the device is
! defined (!= 0), nothing is done and the given device is used.
! Otherwise, either /dev/PS0 (2,28) or /dev/at0 (2,8), depending
! on the number of sectors that the BIOS reports currently.

	seg cs
	mov	ax,root_dev
	or	ax,ax
	jne	root_defined
	seg cs
	mov	bx,sectors
	mov	ax,#0x0208		! /dev/ps0 - 1.2Mb
	cmp	bx,#15
	je	root_defined
	mov	ax,#0x021c		! /dev/PS0 - 1.44Mb
	cmp	bx,#18
	je	root_defined
undef_root:
	jmp undef_root
root_defined:
	seg cs
	mov	root_dev,ax

! after that (everyting loaded), we jump to
! the setup-routine loaded directly after
! the bootblock:

	jmpi	0,SETUPSEG

! This routine loads the system at address 0x10000, making sure
! no 64kB boundaries are crossed. We try to load it as fast as
! possible, loading whole tracks whenever we can.
!
! in:	es - starting address segment (normally 0x1000)
!
sread:	.word 1+SETUPLEN	! sectors read of current track
head:	.word 0			! current head
track:	.word 0			! current track

read_it:
	mov ax,es
	test ax,#0x0fff
die:	jne die			! es must be at 64kB boundary
	xor bx,bx		! bx is starting address within segment
rp_read:
	mov ax,es
	cmp ax,#ENDSEG		! have we loaded all yet?
	jb ok1_read
	ret
ok1_read:
	seg cs
	mov ax,sectors
	sub ax,sread
	mov cx,ax
	shl cx,#9
	add cx,bx
	jnc ok2_read
	je ok2_read
	xor ax,ax
	sub ax,bx
	shr ax,#9
ok2_read:
	call read_track
	mov cx,ax
	add ax,sread
	seg cs
	cmp ax,sectors
	jne ok3_read
	mov ax,#1
	sub ax,head
	jne ok4_read
	inc track
ok4_read:
	mov head,ax
	xor ax,ax
ok3_read:
	mov sread,ax
	shl cx,#9
	add bx,cx
	jnc rp_read
	mov ax,es
	add ah,#0x10
	mov es,ax
	xor bx,bx
	jmp rp_read

read_track:
	pusha
	pusha	
	mov	ax, #0xe2e 	! loading... message 2e = .
	mov	bx, #7
 	int	0x10
	popa		

	mov dx,track
	mov cx,sread
	inc cx
	mov ch,dl
	mov dx,head
	mov dh,dl
	and dx,#0x0100
	mov ah,#2
	
	push	dx				! save for error dump
	push	cx
	push	bx
	push	ax

	int 0x13
	jc bad_rt
	add	sp, #8   	
	popa
	ret

bad_rt:	push	ax				! save error code
	call	print_all			! ah = error, al = read
	
	
	xor ah,ah
	xor dl,dl
	int 0x13
	

	add	sp, #10
	popa	
	jmp read_track

/*
 *	print_all is for debugging purposes.  
 *	It will print out all of the registers.  The assumption is that this is
 *	called from a routine, with a stack frame like
 *	dx 
 *	cx
 *	bx
 *	ax
 *	error
 *	ret <- sp
 *
*/
 
print_all:
	mov	cx, #5		! error code + 4 registers
	mov	bp, sp	

print_loop:
	push	cx		! save count left
	call	print_nl	! nl for readability
	jae	no_reg		! see if register name is needed
	
	mov	ax, #0xe05 + 0x41 - 1
	sub	al, cl
	int	0x10

	mov	al, #0x58 	! X
	int	0x10

	mov	al, #0x3a 	! :
	int	0x10

no_reg:
	add	bp, #2		! next register
	call	print_hex	! print it
	pop	cx
	loop	print_loop
	ret

print_nl:
	mov	ax, #0xe0d	! CR
	int	0x10
	mov	al, #0xa	! LF
	int 	0x10
	ret

/*
 *	print_hex is for debugging purposes, and prints the word
 *	pointed to by ss:bp in hexadecmial.
*/

print_hex:
	mov	cx, #4		! 4 hex digits
	mov	dx, (bp)	! load word into dx
print_digit:
	rol	dx, #4		! rotate so that lowest 4 bits are used
	mov	ah, #0xe	
	mov	al, dl		! mask off so we have only next nibble
	and	al, #0xf
	add	al, #0x30	! convert to 0 based digit, '0'
	cmp	al, #0x39	! check for overflow
	jbe	good_digit
	add	al, #0x41 - 0x30 - 0xa 	! 'A' - '0' - 0xa

good_digit:
	int	0x10
	loop	print_digit
	ret


/*
 * This procedure turns off the floppy drive motor, so
 * that we enter the kernel in a known state, and
 * don't have to worry about it later.
 */
kill_motor:
	push dx
	mov dx,#0x3f2
	xor al, al
	outb
	pop dx
	ret

sectors:
	.word 0

msg1:
	.byte 13,10
	.ascii "Loading"	; 调用 BIOS 中断显示的信息

.org 506
swap_dev:
	.word SWAP_DEV
root_dev:
	.word ROOT_DEV
boot_flag:
	.word 0xAA55	; 有效引导扇区标志， 供 BIOS 加载引导扇区使用

.text
endtext:
.data
enddata:
.bss
endbss:

