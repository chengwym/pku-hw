程序内存包含：程序的可执行机器代码，操作系统需要的一些信息，用来管理过程调
用和返回的运行时栈，以及用户分配的内存块（比如说用malloc 库函数分配的）

![image-20220913211110894](C:\Users\DELL\AppData\Roaming\Typora\typora-user-images\image-20220913211110894.png)

![image-20220913211527511](C:\Users\DELL\AppData\Roaming\Typora\typora-user-images\image-20220913211527511.png)

生成1 字节和2 字节数字的指令会保持剩下的字节不变；生成4 字节数字的指令会把高位4 个字节置为0 。

Imm(ra,rb,s) = imm+ra+rb*s

传送指令的两个操作数不能都指向内存位置。

movabsq 指令能够以任意64 位立即数值作为源操作数，并且只能以寄存器作为目的。

moveq如果传立即数，只能传32位补码表示的立即数， 随后把符号拓展到64位

cltq  %eax符号扩展到%rax

![image-20220913221741226](C:\Users\DELL\AppData\Roaming\Typora\typora-user-images\image-20220913221741226.png)

leaq将地址写入目的寄存器

移位操作移位量是立即数或者放在但自己额寄存器%cl中

![image-20220913223203708](C:\Users\DELL\AppData\Roaming\Typora\typora-user-images\image-20220913223203708.png)

CF 进位标志 ZF 零标志 SF 符号标志 OF 溢出标志

cmp S1，S2   S2-S1

test  S1，S2  S1&S2

![image-20220913225258038](C:\Users\DELL\AppData\Roaming\Typora\typora-user-images\image-20220913225258038.png)

set目的操作数为单字节寄存器元素或一个字节内存位置

![image-20220913225855909](C:\Users\DELL\AppData\Roaming\Typora\typora-user-images\image-20220913225855909.png)

它们会将目标指令的地址与紧跟在跳转指令后面那条指令的地址之间的差作为编码。这些地址偏移量可以编码为1 、2 或4 个字节

这些例子说明，当执行PC 相对寻址时，程序计数器的值是跳转指令后面的那条指令的地址，而不是跳转指令本身的地址。

为了理解为什么基千条件数据传送的代码会比基千条件控制转移的代码性能要好

