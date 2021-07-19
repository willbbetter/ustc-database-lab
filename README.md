# ustc-database-lab
中科大软院数据库实验

编译环境：visual studio 2019
主要问题：
（1）嵌入内联汇编函数GetCycleCount(void)获得较高计时精度，编译可能出现问题
（2）类内变量占用过多内存，超过默认栈大小1M，出现栈溢出，需在VS内重新设置堆栈大小，比如50M
