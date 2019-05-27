# os-lab4-页面置换算法
### 安全1601   16281022   战泓夷
## 一、实验目的<br>
设计和实现最佳置换算法、先进先出置换算法、最近最久未使用置换算法、页面缓冲置换算法；通过页面访问序列随机发生器实现对上述算法的测试及性能比较。<br>
## 二、实验环境<br>
* 操作系统:windows<br>
* 软件：vs<br>
## 三、实验要求<br>
* 模拟的虚拟内存的地址为16位，页面大小为1K，模拟的物理内存有32K。<br>
* 页表用整数数组或结构数组来表示。<br>
* 页面访问序列串是一个整数序列，整数的取值范围为0到N - 1。页面访问序列串中的每个元素p表示对页面p的一次访问。<br>
* 页面访问序列随机生成说明<br>
  * 确定虚拟内存的尺寸N，工作集的起始位置p，工作集中包含的页数e，工作集移动率m（每处理m个页面访问则将起始位置p +1），以及一个范围在0和1之间的值t；<br>
  * 生成m个取值范围在p和p + e间的随机数，并记录到页面访问序列串中；<br>
  * 生成一个随机数r，0 ≤ r ≤ 1；<br>
  * 如果r < t，则为p生成一个新值，否则p = (p + 1) mod N；<br>
  * 如果想继续加大页面访问序列串的长度，请返回第2步，否则结束。<br>
## 四、实验内容<br>
### 1.相关背景介绍<br>
请求分页系统建立在基本分页系统基础之上，为了支持虚拟存储器功能而增加了请求调页功能和页面置换功能。请求分页是目前最常用的一种实现虚拟存储器的方法。<br>
在请求分页系统中，只要求将当前需要的一部分页面装入内存，便可以启动作业运行。在作业执行过程中，当所要访问的页面不在内存时，再通过调页功能将其调入，同时还可以通过置换功能将暂时不用的页面换出到外存上，以便腾出内存空间。<br>
为了实现请求分页，系统必须提供一定的硬件支持。除了需要一定容量的内存及外存的计算机系统，还需要有页表机制、缺页中断机构和地址变换机构。<br>
* 页表机制：请求分页系统的页表机制不同于基本分页系统，请求分页系统在一个作业运行之前不要求全部一次性调入内存，因此在作业的运行过程中，必然会出现要访问的页面不在内存的情况，如何发现和处理这种情况是请求分页系统必须解决的两个基本问题。为此，在请求页表项中增加了四个字段，如图所示。<br>
![input_txt](https://github.com/16281022/os-lab4-/blob/master/1.png)<br><br>
  * 状态位P：用于指示该页是否已调入内存，供程序访问时参考。<br>
  * 访问字段A：用于记录本页在一段时间内被访问的次数，或记录本页最近己有多长时间未被访问，供置换算法换出页面时参考。<br>
  * 修改位M：标识该页在调入内存后是否被修改过。<br>
  * 外存地址：用于指出该页在外存上的地址，通常是物理块号，供调入该页时参考。<br>
* 缺页中断机构:在请求分页系统中，每当所要访问的页面不在内存时，便产生一个缺页中断，请求操作系统将所缺的页调入内存。此时应将缺页的进程阻塞（调页完成唤醒)，如果内存中有空闲块，则分配一个块，将要调入的页装入该块，并修改页表中相应页表项，若此时内存中没有空闲块，则要淘汰某页（若被淘汰页在内存期间被修改过，则要将其写回外存)。<br>
* 地址变换机构：请求分页系统中的地址变换机构，是在分页系统地址变换机构的基础上，为实现虚拟内存，又增加了某些功能而形成的。<br>
![input_txt](https://github.com/16281022/os-lab4-/blob/master/2.jpg)<br><br>
* 工作集：多数程序都显示出高度的局部性，也就是说，在一个时间段内，一组页面被反复引用。这组被反复引用的页面随着时间的推移，其成员也会发生变化。有时这种变化是剧烈的，有时这种变化则是渐进的。我们把这组页面的集合称为工作集。<br>
* 缺页率：缺页率 = 缺页中断次数/页面访问次数<br>
### 该实验使用四个测试序列（前两个与后两个虚拟内存不同），如图所示：<br>
![input_txt](https://github.com/16281022/os-lab4-/blob/master/13.png)<br><br>
![input_txt](https://github.com/16281022/os-lab4-/blob/master/14.png)<br><br>
![input_txt](https://github.com/16281022/os-lab4-/blob/master/15.png)<br><br>
![input_txt](https://github.com/16281022/os-lab4-/blob/master/16.png)<br><br>
### 2.最佳置换算法<br>
* 基本思想<br>
选择永不使用或是在最长时间内不再被访问（即距现在最长时间才会被访问）的页面淘汰出内存评价。<br>
* 评价<br>
理想化算法，具有最好性能（对于固定分配页面方式，本法可保证获得最低的缺页率），但实际上却难于实现，故主要用于算法评价参照。<br>
* 函数说明<br>
  * void optimal(int n); //调入页面，执行置换<br>
  * void testOptimal()； //最佳置换算法实现<br> 
* 运行结果<br>
测试序列1：<br>
![input_txt](https://github.com/16281022/os-lab4-/blob/master/3.png)<br><br>
测试序列2：<br>
![input_txt](https://github.com/16281022/os-lab4-/blob/master/4.png)<br><br>
测试序列3：<br>
![input_txt](https://github.com/16281022/os-lab4-/blob/master/17.png)<br><br>
测试序列4：<br>
![input_txt](https://github.com/16281022/os-lab4-/blob/master/18.png)<br><br>
### 3.先进先出置换算法<br>
* 基本思想<br>
选择最先进入内存即在内存驻留时间最久的页面换出到外存，进程已调入内存的页面按进入先后次序链接成一个队列，并设置替换指针以指向最老页面。<br>
* 评价<br>
简单直观，但不符合进程实际运行规律，性能较差，故实际应用极少。<br>
* 函数说明<br>
  * void initQueue(pQueue q);//初始化队列<br>
  * void push(pQueue q, int num);//入栈：加入新的页面<br>
  * void pop(pQueue q);//出栈：将页面移出<br>
  * void destroy(pQueue q);//销毁队列<br>
  * bool findInQueue(pQueue q, int num);//查看页面是否在队列中<br>
  * void fifoTest();//调入页面，执行置换<br>
  * void fifo(pQueue q, int num);//先进先出置换算法实现<br>
* 运行结果<br>
测试序列1：<br>
![input_txt](https://github.com/16281022/os-lab4-/blob/master/5.png)<br><br>
测试序列2：<br>
![input_txt](https://github.com/16281022/os-lab4-/blob/master/6.png)<br><br>
测试序列3：<br>
![input_txt](https://github.com/16281022/os-lab4-/blob/master/19.png)<br><br>
测试序列4：<br>
![input_txt](https://github.com/16281022/os-lab4-/blob/master/20.png)<br><br>
### 4.最近最久未使用置换算法<br>
* 基本思想<br>
以“最近的过去”作为“最近的将来”的近似，选择最近一段时间最长时间未被访问的页面淘汰出内存。<br>
* 评价<br>
适用于各种类型的程序，性能较好，但需要较多的硬件支持。<br>
* 函数说明<br>
  * void LRU(int n);//调入页面，执行置换<br>
  * void testLRU();//最近最久未使用置换算法实现<br>
* 运行结果<br>
测试序列1：<br>
![input_txt](https://github.com/16281022/os-lab4-/blob/master/7.png)<br><br>
测试序列2：<br>
![input_txt](https://github.com/16281022/os-lab4-/blob/master/8.png)<br><br>
测试序列3：<br>
![input_txt](https://github.com/16281022/os-lab4-/blob/master/21.png)<br><br>
测试序列4：<br>
![input_txt](https://github.com/16281022/os-lab4-/blob/master/22.png)<br><br>
### 5.改进型Clock置换算法<br>
* 基本思想<br>
1）从查寻指针当前位置起扫描内存分页循环队列，选择A=0且M=0的第一个页面淘汰；若未找到，转②。<br>
2）开始第二轮扫描，选择A=0且M=1的第一个页面淘汰，同时将经过的所有页面访问位置0；若不能找到，转①。<br>
* 评价<br>
与简单Clock算法相比，可减少磁盘的I/O操作次数，但淘汰页的选择可能经历多次扫描，故实现算法自身的开销增大<br>
* 函数说明<br>
  * void test_Clock();调入页面，执行置换<br>
  * void updated_Clock(int n);//改进型Clock置换算法实现<br>
* 运行结果<br>
测试序列1：<br>
![input_txt](https://github.com/16281022/os-lab4-/blob/master/9.png)<br><br>
测试序列2：<br>
![input_txt](https://github.com/16281022/os-lab4-/blob/master/10.png)<br><br>
测试序列3：<br>
![input_txt](https://github.com/16281022/os-lab4-/blob/master/23.png)<br><br>
测试序列4：<br>
![input_txt](https://github.com/16281022/os-lab4-/blob/master/24.png)<br><br>
### 6.页面缓冲算法PBA<br>
* 基本思想<br>
  * 设立空闲页面链表和已修改页面链表。<br>
  * 采用可变分配和基于先进先出的局部置换策略，并规定被淘汰页先不做物理移动，而是依据是否修改分别挂到空闲页面链表或已修改页面链表的末尾。<br>
  * 空闲页面链表同时用于物理块分配。<br>
  * 当已修改页面链表达到一定长度如Z个页面时，一起将所有已修改页面写回磁盘，故可显著减少磁盘I/O操作次数。<br>
* 函数说明<br>
  * bool isInNodes(int n);//查看页面是否在链表中<br>
  * void addToLink(int data, int type);//向链表添加页面<br>
  * void emptyIdle();//将空闲链表上的所有页面送出内存<br>
  * void emptyModi();//将已修改页面链表上所有的链表送出内存<br>
  * void test_PBA();//调入页面，执行置换<br>
  * void PBA(int n);//页面缓冲算法实现<br>
* 运行结果<br>
测试序列1：<br>
![input_txt](https://github.com/16281022/os-lab4-/blob/master/11.png)<br><br>
测试序列2：<br>
![input_txt](https://github.com/16281022/os-lab4-/blob/master/12.png)<br><br>
测试序列3：<br>
![input_txt](https://github.com/16281022/os-lab4-/blob/master/25.png)<br><br>
测试序列4：<br>
![input_txt](https://github.com/16281022/os-lab4-/blob/master/26.png)<br><br>
## 五、实验结果分析<br>
### 1.实验结果比较<br>
| 算法名称 | 最佳置换算法 | 先进先出算法 | 最近最久未使用算法 | 改进型Clock置换算法 | 页面缓冲置换算法 |
| :-: | :-: | :-: | :-: |:-: |:-: |
| 缺页数 | 16 | 19 | 38 | 20 | 21 |
| 缺页率 | 0.500000 | 0.593750 | 1.187500 | 0.625000 | 0.656250 |

| 算法名称 | 最佳置换算法 | 先进先出算法 | 最近最久未使用算法 | 改进型Clock置换算法 | 页面缓冲置换算法 |
| :-: | :-: | :-: | :-: |:-: |:-: |
| 缺页数 | 11 | 18 | 36 | 21 | 10 |
| 缺页率 | 0.343750 | 1.125000 | 1.187500 | 0.656250 | 0.312500 |
### 2.算法比较<br>
