## 跳跃表
好的博客文章，就像一篇篇动人的故事一样，我喜欢那样的故事。我总是不太讲故事，主要还是因为不会讲。也许正因如此，我想讲一次能让自己喜欢的故事。哈哈。。。

### 我和跳跃表(skiplist)的故事
2010年冬季的一天，自学完链表之后。当时就有想过，如果把指针域改成数组的形式会怎样？可又想，数据域就一个整型值，指针域那么大，不是太浪费了吗[当时的想法就这么简单，以至于阉割了自己更早接触到跳表的可能]？至此，这个想法也就终止了。几年后，在算法与数据结构那本书中第一次接触到跳表时，不禁有些遗憾。下面，首先简单看一下单链表，这里用C语言的结构体表示，如下。如果每一个节点用指针连起来，结构上如下面的图所示。
    
    struct LinkedListNode
    {
        T data;
        struct LinkedListNode* next;
    };

![](https://github.com/WalkingNL/Pics/blob/master/List1.jpg)

以上就是单链表，跳表与它相差就在于指针域是一个数组，用C语言结构体表示如下。这么一个看似在单链表上的小小的变化，带来的应用潜力无疑是巨大的。同样，把跳表的每一个节点连起来，如下图所示。

    struct LinkedListNode
    {
        T data;
        struct LinkedListNode* next[NUMLAYER]; // NUMLAYER表示指针的个数
    };
![](https://github.com/WalkingNL/Pics/blob/master/SkipList1.jpg)

当然了，按照跳表的定义，以上还不能称跳表，但是它已经具备跳表的**形**。在忽略空间因素的情况下，单纯从上面两个图的观感上，很明显跳表要复杂一些，但同时也带来了查询、插入等操作效率上的提高。嗯，就一些吗？想想看，如果有些节点的指针域再增加一些，会怎样？在真正进入正题之前，用几句话谈谈感想。
1. 第一次认识链表后，隔了至少5年，我才真正接触到跳表这种结构。更为遗憾的是，带我认识跳表的那本经典的数据结构书，一直在我的卧榻之侧。
2. 刚接触链表之后，就想到过指针域变成数组，却最终变成了一个浅尝辄止的想法。事后许多事情证明这是莫大的缺失。

#### 跳跃表
就[SkipList](https://www.csee.umbc.edu/courses/undergraduate/341/fall01/Lectures/SkipLists/skip_lists/skip_lists.html)的结构而言，本质上他依然属于线性的数据结构，这一点和普通链表无差。跳跃表的出现，大大提升了线性链式结构的查询、插入、删除等操作的效率。那么，他具体是如何做到的呢？接下来我们用两个小结的篇幅分析一下。
#### 跳跃表的查询及插入分析
对比上面的图，这里作稍微的调整，如下

![](https://github.com/WalkingNL/Pics/blob/master/list2.jpg)

![](https://github.com/WalkingNL/Pics/blob/master/SkipList2.jpg)


#### 跳跃表的性能分析

#### Redis中的跳跃表

####
