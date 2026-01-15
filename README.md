<p align="center">
	<img src="icon.jpg" width="25%" />
</p>

- Ciallo～(∠・ω< )⌒☆ 是一个不依赖任何标准库，仅内联了少量汇编实现的轻量级玩具库，可以在单一线程中同时攻略多个香香软软的小函数

- 这只是一个实验性质的库，主要用于给我自己练手，不要真的不要真的真的不要将这个库用于实际生产，会变得很不幸
- 这个库目前只支持 x86_64 架构的 cpu ，并且在 Windows 操作系统中存在不小问题

# 使用

1. 下载 `ciallo.h` 与 `ciallo.c` 两个文件到本地，放在编译器找得到的位置，在源文件头部添加以下这条预处理指令
    ```c
    #include "ciallo.h"
    ```

2. 之后只需要在代码中调用以下四个宏即可，最下面有解释
    ```c
    Ciaolima;
    Ciallo(fun, arg);
    Ciallo_;
    Ciavanllo;
    ```

3. 编译时建议先使用 `gcc -O0 -c ciallo.c -o ciallo.o` 编译得到 `ciallo.o` ，再将 `ciallo.o` 与其他源文件一起编译，不要千万不要千万千万不要启用 `-O2` 甚至 `-O3` 级别的优化来编译 `ciallo.c`，会变得没法运行

# 关于四个宏

### Ciaolima

- 该宏用于初始化，如果想让当前函数与其调用的函数共享同一个线程，需要在调用其它函数前先调用这个宏，被调用的函数内部也需要调用这个宏

### Ciallo(fun, arg)

- 该宏用于调用函数，参数 `fun` 为函数名或函数指针，类型必须为 `void (*)(void *)` ，参数 `arg` 为函数的参数，类型为 `void *`

### Ciallo_

- 该宏用于切换函数，会按照调用 `Ciallo(fun, arg)` 的先后顺序，切换到下一个未执行完的函数，如果其他函数都执行完了，就切换到当前函数 ~~，应该没有副作用~~

### Ciavanllo

- 该宏用于回收函数，类似于 `return` 但所有通过 `Ciallo(fun, arg)` 调用的函数都必须调用这个宏

---

[这里有个简单的示例](https://github.com/BreakTheMyth/ciallo/blob/main/main.c)
