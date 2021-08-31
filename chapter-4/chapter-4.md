# Chap 4 BPF跟踪

### <font color=peach>4.1 probe</font>

传统的Linux中使用的探针程序需要被编译成内核模块，可能会在生产环境中产生灾难性的问题。BPF借助跟踪探针收集信息并进行调试与分析，加载到内核之前会被BPF验证器验证因而更加安全。

#### <font color=Magenta>4.1.1 kernel probe</font>

内核探针分为kprobes和kretprobes，二者的使用取决于你要将BPF程序插入指令执行周期的哪个阶段。内核探针没有稳定的应用程序二进制接口（ABI），所以可能需要适配不同版本的内核。

1. <font color=Orange>kprobes</font>

   kprobes允许在任意内核指令执行前插入BPF程序。

2. <font color=Orange>kretprobes</font>

   kretprobes在内核指令有返回值时插入BPF程序。

#### <font color=Magenta>4.1.2 tracepoint</font>

/sys/kernel/debug/tracing/events/bpf/目录下应该会有一些BPF可用的tracepoint，我使用的ubuntu18.04（内核版本5.4.0-81-generic）可能过低，并没有该目录。

#### <font color=Magenta>4.1.3 userspace probe</font>

用户空间探针分为uprobes和uretprobes，二者的使用取决于你要将BPF程序插入指令执行周期的哪个阶段。也能跟踪用户程序使用到的库中的指令。<font color=red>用户空间的程序一旦重命名，BPF跟踪也将失效，因为bpf.attach_uprobe和bpf.attach_uretprobe都是attach到一个静态的程序路径上的。</font>

1. <font color=Orange>uprobes</font>

   uprobes允许在任意用户指令执行前插入BPF程序。

2. <font color=Orange>uretprobes</font>

   uretprobes在用户指令有返回值时插入BPF程序，允许BPF代码从寄存器访问返回值。

#### <font color=Magenta>4.1.4 user static define_trace</font>

USDT因DTrace工具变得流行，有低开销，便捷和支持跟踪的语言丰富的优势（C,Python,Ruby,Go,Node.js ）。

### <font color=peach>4.2 跟踪数据可视化</font>

将BPF跟踪到的数据聚合起来，可视化呈现，如火焰图、直方图。



## CODE

book是书中的样例代码。

my_examples/uprobe仿照uprobe和uretprobe测量C程序函数的执行时间。



## Env

火焰图:

https://github.com/brendangregg/FlameGraph










