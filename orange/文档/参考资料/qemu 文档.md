以下是 QEMU 的主要常用参数以及详细解释，帮助您在使用时能够快速找到所需功能的配置方式。

---

### **全局参数**
#### **1. `-M <machine>`**
- 设置虚拟机的机器类型（硬件架构）。  
- 示例：
  ```bash
  qemu-system-i386 -M pc
  ```
- 常见选项：
  - `pc`：传统 PC（默认）。
  - `q35`：现代化 PC 平台，支持 PCIe。
  - `virt`：专为 ARM 设计的虚拟化平台。
  - `none`：不指定具体的硬件架构，便于测试。

---

#### **2. `-m <size>`**
- 分配虚拟机内存大小（单位为 MB 或 GB）。  
- 示例：
  ```bash
  qemu-system-i386 -m 512M
  qemu-system-i386 -m 2G
  ```

---

#### **3. `-cpu <model>`**
- 指定虚拟 CPU 的模型。  
- 示例：
  ```bash
  qemu-system-i386 -cpu pentium
  ```
- 常见 CPU 类型：
  - `max`：支持目标架构的最大化功能集。
  - `host`：模拟与宿主机一致的 CPU。
  - `pentium`、`qemu64`：常见的模拟 CPU 类型。

---

#### **4. `-smp <n>`**
- 设置虚拟机的虚拟 CPU 核心数。
- 示例：
  ```bash
  qemu-system-i386 -smp 4
  ```
- 参数可选：
  - `n`：CPU 总数。
  - `sockets`、`cores`、`threads`：详细配置。

---

### **磁盘与存储参数**
#### **5. `-drive`**
- 配置虚拟机的磁盘驱动器。
- 示例：
  ```bash
  -drive file=disk.img,index=0,media=disk,format=raw
  ```
- 参数解释：
  - `file=<path>`：磁盘镜像文件路径。
  - `index=<n>`：磁盘索引编号。
  - `media=disk|cdrom`：媒体类型。
  - `format=<fmt>`：镜像格式，如 `raw`、`qcow2`。
  - `if=ide|scsi|virtio`：磁盘接口类型。

---

#### **6. `-cdrom <file>`**
- 加载一个 ISO 文件作为 CD-ROM 驱动器。  
- 示例：
  ```bash
  qemu-system-i386 -cdrom boot.iso
  ```

#### **7. `-hda` / `-hdb`**
- 指定硬盘镜像文件（旧版语法，推荐使用 `-drive`）。  
- 示例：
  ```bash
  qemu-system-i386 -hda disk.img
  ```

---

### **网络参数**
#### **8. `-netdev` 和 `-device`**
- 设置虚拟机的网络设备。  
- 示例：
  ```bash
  -netdev user,id=mynet0 -device e1000,netdev=mynet0
  ```
- 参数解释：
  - `user`：用户模式网络（默认）。
  - `id=<id>`：网络设备标识。
  - `e1000`：虚拟化的 Intel 千兆网卡。

#### **9. `-nic`**
- 简化网络配置方式（QEMU 4.0 之后引入）。  
- 示例：
  ```bash
  qemu-system-i386 -nic user
  ```

---

### **启动相关参数**
#### **10. `-kernel`**
- 指定加载的内核文件。
- 示例：
  ```bash
  qemu-system-i386 -kernel bzImage
  ```

#### **11. `-append`**
- 向内核传递命令行参数。  
- 示例：
  ```bash
  qemu-system-i386 -append "root=/dev/sda console=ttyS0"
  ```

#### **12. `-initrd`**
- 加载内核初始化 RAM 磁盘文件。
- 示例：
  ```bash
  qemu-system-i386 -initrd initrd.img
  ```

---

### **调试与控制参数**
#### **13. `-s`**
- 开启 GDB 服务器，默认监听在 `localhost:1234`。

#### **14. `-S`**
- 启动后暂停虚拟机，等待调试器连接。

#### **15. `-monitor`**
- 启用 QEMU 内部的监控控制台。
- 示例：
  ```bash
  qemu-system-i386 -monitor stdio
  ```

#### **16. `-serial`**
- 配置串口输出。
- 示例：
  ```bash
  qemu-system-i386 -serial stdio
  ```

#### **17. `-nographic`**
- 禁用虚拟机的图形输出，专注于命令行。

---

### **加速与优化参数**
#### **18. `-enable-kvm`**
- 使用 KVM（内核虚拟机）加速（macOS 不支持）。

#### **19. `-accel hvf`**
- 使用 macOS 的 Hypervisor.framework 加速。

#### **20. `-rtc`**
- 配置虚拟机的 RTC（实时时钟）。
- 示例：
  ```bash
  qemu-system-i386 -rtc base=localtime
  ```

---

### **示例组合**
#### **1. 基本运行命令**
```bash
qemu-system-i386 -m 256M -drive file=disk.img,format=raw -serial stdio
```

#### **2. 加载内核与调试**
```bash
qemu-system-i386 -m 128M -kernel bzImage -append "console=ttyS0" -serial stdio -s -S
```

#### **3. 带网络的完整启动**
```bash
qemu-system-i386 -m 512M -nic user -drive file=disk.img,format=qcow2 -monitor stdio
```

如果需要更详细的解释，欢迎随时咨询！