AT&T 汇编和 Intel 汇编风格在语法上存在较大差异，但它们本质上描述的还是同样的底层硬件指令集。以下是针对 Intel 汇编用户快速上手 AT&T 汇编的关键点与示例：

---

## **1. 主要语法差异**

### **1.1 寄存器**
- 两种风格对寄存器的命名一致（如 `eax`、`ebx`）。
- 无需适应新的寄存器名称。

---

### **1.2 操作数顺序**
AT&T 汇编使用 **"源操作数 -> 目的操作数"** 的顺序，与 Intel 的顺序相反。

- Intel：  
  ```
  mov eax, ebx   ; 将 ebx 的值复制到 eax
  ```
- AT&T：  
  ```
  movl %ebx, %eax   # 将 %ebx 的值复制到 %eax
  ```

---

### **1.3 操作数的前缀**
- **寄存器** 前必须加 `%`。  
  - 示例：`%eax`, `%ebx`
- **立即数** 前必须加 `$`。  
  - 示例：`$0x10` 表示立即数 `16`
- **内存操作数** 使用无前缀的符号。

---

### **1.4 指令的后缀**
AT&T 汇编通过指令后缀表示操作数的大小：
- `b`（byte）：8 位操作（如 `movb`）。
- `w`（word）：16 位操作（如 `movw`）。
- `l`（long）：32 位操作（如 `movl`）。
- `q`（quad）：64 位操作（如 `movq`）。

---

### **1.5 内存寻址**
内存操作数的表示方法与 Intel 不同：
- **通用格式**：`disp(base, index, scale)`  
  - `disp`：偏移量（立即数）。
  - `base`：基址寄存器。
  - `index`：变址寄存器。
  - `scale`：比例因子（1、2、4、8）。
- 示例：
  - Intel：
    ```
    mov eax, [ebx+4*esi+8]
    ```
  - AT&T：
    ```
    movl 8(%ebx, %esi, 4), %eax
    ```

---

### **1.6 汇编注释**
- AT&T 汇编的注释以 `#` 开头，而非 `;`。
  ```asm
  movl %eax, %ebx   # 将 eax 的值复制到 ebx
  ```

---

## **2. 常用指令对比**

| 功能               | Intel                 | AT&T                        |
|--------------------|-----------------------|-----------------------------|
| 数据传送           | `mov eax, ebx`        | `movl %ebx, %eax`           |
| 加法               | `add eax, 5`          | `addl $5, %eax`             |
| 减法               | `sub eax, ebx`        | `subl %ebx, %eax`           |
| 位移               | `shl eax, 1`          | `shll $1, %eax`             |
| 逻辑与             | `and eax, 0xFF`       | `andl $0xFF, %eax`          |
| 跳转               | `jmp label`           | `jmp label`                 |
| 条件跳转           | `je label`            | `je label`                  |

---

## **3. 示例代码**

以下代码实现两个数相加并存储到寄存器中。

### Intel 风格
```asm
section .text
    global _start
_start:
    mov eax, 5        ; 将 5 存入 eax
    mov ebx, 3        ; 将 3 存入 ebx
    add eax, ebx      ; eax = eax + ebx
    mov ecx, eax      ; 将结果存入 ecx
    hlt               ; 停止执行
```

### AT&T 风格
```asm
.section .text
    .global _start
_start:
    movl $5, %eax     # 将 5 存入 %eax
    movl $3, %ebx     # 将 3 存入 %ebx
    addl %ebx, %eax   # %eax = %eax + %ebx
    movl %eax, %ecx   # 将结果存入 %ecx
    hlt               # 停止执行
```

---

## **4. AT&T 汇编调试**
使用 `gdb` 调试 AT&T 汇编时，观察寄存器和内存内容的方法与 Intel 汇编一致。可以使用以下命令：
- `info registers`：查看寄存器状态。
- `x <address>`：查看内存内容。

---

## **5. 快速转换总结**
| 差异点          | Intel 示例              | AT&T 示例                   |
|------------------|-------------------------|-----------------------------|
| 操作数顺序       | `mov eax, ebx`          | `movl %ebx, %eax`           |
| 立即数前缀       | `mov eax, 10`           | `movl $10, %eax`            |
| 寄存器前缀       | `add eax, ebx`          | `addl %ebx, %eax`           |
| 内存寻址         | `mov eax, [ebx+4*esi]`  | `movl (%ebx, %esi, 4), %eax`|
| 指令大小后缀     | 无                      | `b`、`w`、`l`、`q`          |

---

快速适应的关键是牢记 **操作数顺序** 和 **操作数前缀** 的变化，熟练后会发现 AT&T 汇编也很直观。随时可以咨询更复杂的用法！


AT&T 汇编中常见的伪指令（Pseudo-instructions）用于控制程序的布局、定义数据、声明符号等。伪指令不会直接翻译为机器码，而是由汇编器处理为实际的机器指令或相关信息。以下是 AT&T 汇编中常见的伪指令及其功能：

---

### **1. 段定义与标志**
#### **`.section`**
- 定义或切换到指定的段（section）。
- 示例：
  ```asm
  .section .text     # 代码段
  .section .data     # 数据段
  ```

#### **`.text`**
- 指定代码段。
- 示例：
  ```asm
  .text
  ```

#### **`.data`**
- 指定可读写的数据段。
- 示例：
  ```asm
  .data
  ```

#### **`.bss`**
- 定义未初始化的数据段。
- 示例：
  ```asm
  .bss
  ```

#### **`.global` 或 `.globl`**
- 声明全局符号（函数或变量），使其可被其他文件访问。
- 示例：
  ```asm
  .globl _start
  _start:
      movl $0, %eax
  ```

---

### **2. 数据定义**
#### **`.byte`**
- 定义 8 位数据。
- 示例：
  ```asm
  .byte 0x41          # 存储 ASCII 字符 'A'
  ```

#### **`.word`**
- 定义 16 位数据。
- 示例：
  ```asm
  .word 0x1234        # 存储 16 位整数
  ```

#### **`.long`**
- 定义 32 位数据。
- 示例：
  ```asm
  .long 0x12345678    # 存储 32 位整数
  ```

#### **`.quad`**
- 定义 64 位数据。
- 示例：
  ```asm
  .quad 0x1122334455667788  # 存储 64 位整数
  ```

#### **`.ascii`**
- 定义一个以字符串形式存储的数据，不带结尾的 NULL。
- 示例：
  ```asm
  .ascii "Hello"      # 存储 'H', 'e', 'l', 'l', 'o'
  ```

#### **`.asciz`**
- 定义一个以字符串形式存储的数据，并附加结尾的 NULL 字节。
- 示例：
  ```asm
  .asciz "Hello"      # 存储 'H', 'e', 'l', 'l', 'o', '\0'
  ```

#### **`.space`**
- 分配指定字节数的空间，并初始化为零。
- 示例：
  ```asm
  .space 16           # 分配 16 个字节，初始化为 0
  ```

#### **`.fill`**
- 分配指定字节数的空间，并用指定值填充。
- 示例：
  ```asm
  .fill 4, 2, 0x1234  # 分配 4 个 16 位的空间，每个填充为 0x1234
  ```

---

### **3. 对齐与填充**
#### **`.align`**
- 将下一指令或数据对齐到指定字节边界。
- 示例：
  ```asm
  .align 4            # 对齐到 4 字节边界
  ```

---

### **4. 符号与标签**
#### **`.equ` 或 `.set`**
- 定义符号的值，相当于常量。
- 示例：
  ```asm
  .equ BUFFER_SIZE, 256
  movl $BUFFER_SIZE, %eax
  ```

#### **`.label`**
- 定义一个局部标签（以 `.` 开头，仅在本文件可见）。
- 示例：
  ```asm
  .L1:
      addl $1, %eax
      jmp .L1
  ```

---

### **5. 调试与元信息**
#### **`.file`**
- 指定源文件名，通常用于调试信息。
- 示例：
  ```asm
  .file "main.c"
  ```

#### **`.line`**
- 指定源代码的行号，通常用于调试信息。
- 示例：
  ```asm
  .line 42
  ```

---

### **6. 链接相关**
#### **`.comm`**
- 定义一个未初始化的全局变量，并分配空间。
- 示例：
  ```asm
  .comm my_var, 4     # 分配一个全局变量 my_var，占用 4 字节
  ```

#### **`.lcomm`**
- 定义一个未初始化的局部变量，并分配空间。
- 示例：
  ```asm
  .lcomm buffer, 128  # 分配一个局部变量 buffer，占用 128 字节
  ```

#### **`.extern`**
- 声明外部符号。
- 示例：
  ```asm
  .extern external_function
  ```

#### **`.type`**
- 指定符号的类型（如函数或对象）。
- 示例：
  ```asm
  .type _start, @function
  ```

#### **`.size`**
- 指定符号的大小。
- 示例：
  ```asm
  .size buffer, 128
  ```

---

### **7. 示例代码**
以下代码演示了部分伪指令的使用：
```asm
.section .data
message:
    .asciz "Hello, World!\n"

.section .text
.globl _start
_start:
    movl $4, %eax        # 系统调用：write
    movl $1, %ebx        # 文件描述符：stdout
    movl $message, %ecx  # 指向字符串
    movl $14, %edx       # 字符串长度
    int $0x80            # 调用内核

    movl $1, %eax        # 系统调用：exit
    xorl %ebx, %ebx      # 状态码：0
    int $0x80            # 调用内核
```

---

通过这些伪指令，您可以方便地组织代码、定义数据和调试程序。如果有更多特定需求，可以随时提问！