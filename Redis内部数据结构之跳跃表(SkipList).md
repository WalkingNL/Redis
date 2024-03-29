*我用c语言简单实现了一个跳跃表，参看这个主题外面的SkipList.cpp文件(更多的代码实现，可以参考[这里](https://www.geeksforgeeks.org/skip-list-set-2-insertion/))。不过在本文关于跳跃表的介绍中，为了篇幅的缘故，我采用了伪代码*

*对于skip list，中文叫法多有两种，跳表或是跳跃表，本文统一称 跳跃表*

## 跳跃表
这里有一个MIT的讲解跳跃表的[视频](https://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-046j-introduction-to-algorithms-sma-5503-fall-2005/video-lectures/lecture-12-skip-lists/)，如果听不懂的话，视频的下方有英文文字，这儿还有对应的[PPT](https://ocw.mit.edu/courses/electrical-engineering-and-computer-science/6-046j-introduction-to-algorithms-sma-5503-fall-2005/video-lectures/lecture-12-skip-lists/lec12.pdf)，可以结合起来，慢慢看。另外，还有很多的博客文章，对跳跃表本身的介绍已经足够多了。不过，我还是想发表一些自己的想法，所以计划写这篇文章。想写很多内容，所以不打算结题。哈哈，这篇文章估计会写的非常长。

### 认识跳跃表(skiplist)
2010年冬季的一天，自学完链表之后。当时就有想过，如果把指针域改成数组的形式会怎样？可又想，数据域就一个整型值，指针域那么大，不是太浪费了吗[当时的想法就这么简单，以至于阉割了自己更早接触到跳跃表的可能]？至此，这个想法也就终止了。几年后，在算法与数据结构那本书中第一次接触到跳跃表时，不禁有些遗憾。下面，首先简单看一下单链表，这里用C语言的结构体表示，如下。如果每一个节点用指针连起来，结构上如下面的图所示。
    
    struct LinkedListNode
    {
        T data;
        struct LinkedListNode* next;
    };

![](https://github.com/WalkingNL/Pics/blob/master/List1.jpg)

以上就是单链表，跳跃表与它相差就在于指针域是一个数组，用C语言结构体表示如下。这么一个看似在单链表上的小小的变化，带来的应用潜力无疑是巨大的。同样，把跳跃表的每一个节点连起来，如下图所示。

    struct LinkedListNode
    {
        T data;
        struct LinkedListNode* next[NUMLAYER]; // NUMLAYER表示指针的个数
    };
![](https://github.com/WalkingNL/Pics/blob/master/SkipList1.jpg)

当然了，按照跳跃表的定义，以上还不能称跳跃表，但是它已经具备跳跃表的**形**。在忽略空间因素的情况下，单纯从上面两个图的观感上，很明显跳跃表要复杂一些，但同时也带来了查询、插入等操作效率上的提高。嗯，就一些吗？想想看，如果有些节点的指针域再增加一些，会怎样？在真正进入正题之前，用几句话谈谈感想。
1. 第一次认识链表后，隔了至少5年，我才真正接触到跳跃表这种结构。更为遗憾的是，带我认识跳跃表的那本经典的数据结构书，一直在我的卧榻之侧。
2. 刚接触链表之后，就想到过指针域变成数组，却最终变成了一个浅尝辄止的想法。事后许多事情证明这是莫大的缺失。

#### 跳跃表
就[SkipList](https://www.csee.umbc.edu/courses/undergraduate/341/fall01/Lectures/SkipLists/skip_lists/skip_lists.html)的结构而言，本质上他依然属于线性的数据结构，这一点和普通链表无差。跳跃表的出现，大大提升了线性链式结构的查询、插入、删除等操作的效率。那么，具体是如何做到的呢？接下来我们用两个小结的篇幅分析一下。
#### 跳跃表的查询及插入分析
对比上面的图，这里作稍微的调整，让链表与跳跃表中的节点按值从小到大排列，如下。

对于`查询`操作。如果要查询值为37的节点，就需要对比5个节点。而对于`插入`操作，假如要在值为37的节点之后，插入一个值为41的节点。先要确定好插入的位置，然后才能执行插入操作。而确定好插入的位置，需要对比6个节点，然后再插入。

![](https://github.com/WalkingNL/Pics/blob/master/list2.jpg)


如果是跳跃表，对于`查询`操作，同样如果查询值为37的节点，只需要对比3个节点的值；对于`插入`操作，要在37的节点之后，插入值为41的节点，也只需要对比4个节点，就能确定好插入位置。

![](https://github.com/WalkingNL/Pics/blob/master/SkipList2.jpg)

以上简单的描述中，能够看出，对于单链表而言，查询数据的时间复杂度是O(n)。虽然插入操作本身时间复杂度是O(1)，但是插入数据前先要确定好插入位置，这就相当于先要做查询操作，而查询操作的时间复杂度是O(n)。

我们知道，对于元素值有序的数组结构，采用二分查找，能够让查询操作的时间复杂度提升到O(logn)。但是对链表而言，由于节点分布的位置是离散的，无法采用二分查找的思想。但是通过**合理的配置**每一个节点指针域的个数，就像跳跃表这样的结构，能够达到近似二分查找的效果。上面的例子中，如果查询37的节点，单链表中需要对比5个节点，而跳跃表只需要对比3个。因为里面都包含了目标节点(值为37的节点)，如果各自去掉这个节点，可以发现，前者对比的节点数正好是后者的两倍。如果在上面跳跃表的基础上，对某些节点再增加一层指针域，如下图。分别在第一个节点以及第5个节点上，增加了一层指针域。这样一来，如果查询37的节点，除去目标节点外，只需要对比一个节点。

![](https://github.com/WalkingNL/Pics/blob/master/SkipList.jpg)

想象一下，如果我们的链特别长，假设有1000000个节点，那么通过在特定节点的指针域上增加指针的方式，这样可以让查询操作的时间效率直逼二分查找(下文会分析)。接下来我们继续分析。在上文中，提到了**合理的配置**这样的字眼儿，究竟如何进行合理配置呢？说清楚这个问题，不妨作一个过程——从空链开始构建一个全新的跳跃表(因为作图有点浪费空间，所以下面的图在风格上稍微有一小点儿的变化)

![](https://github.com/WalkingNL/Pics/blob/master/SkipList3.jpg)

上图展示了从0个有效节点(指非空节点)构建一个完整的跳跃表的过程。一共插入了6个节点，也就是进行了6次插入的操作。整个过程能够看到跳跃表的一个明显特点是节点之间按值有序排列。此外，节点指针域的个数也不尽相同。那么这个个数该如何确定呢？想想看，如果每个节点指针域的个数都完全相同的话，那么跳跃表就完全退化成了链表的形式，像下图所示的情形。这看不起来不仅毫无美感可言，而且显得十分臃肿，浪费了大量的存储空间。所以，确定一个节点指针域个数，是维持跳跃表**动态**性能的关键一点。

![](https://github.com/WalkingNL/Pics/blob/master/LinkList5.jpg)

###### 查询 及 插入 操作的伪代码
把代码放在这里考虑到两个方面的原因。第一点，代码并不是最重要的，如果真正理解了原理，相信代码也能自己写出来；第二点，代码的存在对于有些人，往往会干扰对文字的阅读。
查询：

    find(list, key) // 在跳跃表中查找值为key的节点
        node = list->header; // 取得跳跃表头节点
        for i = list->levelCount to 0 do // 节点的层数，从上往下进行
            while node->forward[i]->data < key do // 对比每一个节点中的key
                node = node->forward[i];
        node = node->forward[0];
        if node->data == key then
            return node->value; // 查找成功
        else return // 未找到
插入：

    insert(list, key, value) // 在跳跃表list中，插入值为value的节点
        update[1...level]; // 临时节点
        node = list->header; // 获取跳跃表头
        for i = list->levelCount to 0 do
            while node->forward[i]->data < key do
                node = node->forward[i];
            update[i] = node; // 临时节点指向目标节点的前一个节点
        node = node->forward[0]; // 目标节点
        if node->data == key then // key值相同，只需要修改相应的value即可，不需要在重新申请信息的节点
            node->value = value;
        else // 如果key不相同
            v = randomLevel(); // 首先随机生成一个节点指针阈的层数。作为示例，这里给出一个随机算法
            if v > list->levelCount then // 判断与当前链中的节点的层数大小
                for i = list->level + 1 to v do // 当前链中，节点的层数需要改变
                    update[i] = list->header;
                list->level = v;
             node = newNode(v, key, val); // 申请新的节点空间
             for i = 1 to level do // 逐层修改指针
                node->forward[i] = update[i]->forward[i]
                update[i]->forward[i] = node;


    int randomLevel()
    {
        int v = 1; // 至少一层
        while (random() < p && v < MaxLevel) // p为指定的概率，当小于这个概率时，才能满足
            v = v + 1; // 至少为两层，所以 +1
        return v;
    }

##### 跳跃表的动态特性
**为了方便解释，从这一小节开始，把节点指针域的个数改称为索引个数**。

如果频繁进行插入操作，而节点指针域的个数固定在一个区间内，一直不变。那么随着节点数量越来越多，跳跃表的查询等操作的性能也会受影响。在上面的图中，我们看到节点指针域的个数位于区间[1, 4]。为了方便解释，在下图中，仅展示指针域。可以看出，第二级索引在第一级索引的基础上，每隔一个第一级索引，增加一个；第三级索引在第二级索引基础上，每间隔一个二级索引，增加一个；同理，第四季索引在第三级索引的基础上。图中一共包含16个有效节点，假设，节点的值是序列 {3, 17, 29, 31, 43, 53, 61, 67, 73, 79, 89, 91, 97, 103, 107, 113}。如果要查找节点29，查询的节点序列是{73, 43, 29}，如果查询103，序列是{3, 73, 97, 103}。整个查询的过程与二分查找如出一辙(可以按照二分查找的方式作一下尝试)。

但是，如果现在继续在下图示的链上进行插入，而节点的索引个数始终保持在区间[1, 4]上，可以很容易的发现，查找的效率会越来越低。因此，要保证在跳跃表上各种操作的性能，就必须维持好节点索引个数与节点个数之间的**动态平衡**关系。这一点应该不难理解，相信如果熟悉B+树，AVL或红黑树这种数据结构的，都应该知道，保证这些结构性能的是维持好树的平衡性。对于B+树来说，维持平衡性是通过节点的分裂来完成；对于红黑树、AVL树，维持树结构的平衡是通过左右旋的方式实现。那么，维持跳跃表的动态平衡关系应该怎样进行呢？比较常见的一种方式是通过随机函数Rand()。比如随机函数生成了K，说明该节点拥有K级索引。当然了，随机函数的选择也是非常讲究的，需要从概率上保证跳跃表的平衡。对于怎样选择随机函数，就不在这里提了，可以参考本文附带的。

![](https://github.com/WalkingNL/Pics/blob/master/SkipList7.jpg)

#### 跳跃表的性能分析
对比普通链表，跳跃表是不是很浪费内存呢？好吧，换个说法吧。这么大的空间成本，能够换回多少的时间回报呢？如过空间/时间比，能体现出一分钱一分货的简单价值观，我觉得也就够了，关键得看如何去解释。那接下来，从时空的角度，分别分析一下吧。
##### 空间性能分析
###### 先看看投资
为了保证查询、插入等操作的性能，节点需要存储大量的索引，这就不可避免的造成空间性能的损失。那么这个空间复杂度具体为多少呢？

现在假设原始一个链表的的大小为n，也即第一级索引的个数为n，第二级索引的为n/2，以此类推，直到只有2个节点为止。很显然，这个一个等比数列。跳跃表的空间消耗从第二级开始算，因为原始链表本来就有一级索引的。求和后，结果大约为`n - 2`。那么，说明跳跃表的空间复杂度是O(n)。

    n, n/2, n/4, ......, 8, 4, 2
    n/2 + n/4 + ... + 2 = n - 2。
**如果这个空间消耗太大，想要降低空间复杂度呢**？上面的例子中，上层索引与其紧接的下层索引之间的间隔为1.那么可以通过增加这个间隔的数量，来减少需要的索引个数。如下图所示，间隔两个。那么第二级索引需要n/3，第三级索引需要n/9。也是一个等比数列，通过求和公式，得到索引节点的大需要n/2个。说明空间复杂度仍未O(n)。但是实际上，要比间隔为1的情况，节省了有一半的空间。
    
    n/3, n/9, ......, 9, 3
    n/3 + n/9 + ... + 3 = 

![](https://github.com/WalkingNL/Pics/blob/master/SkipList8.jpg)

之前在公司做性能优化，就采用过一次跳跃表，你可以看这个[链接](https://github.com/WalkingNL/Cache/blob/master/cache.md)。因为需求是要尽可能的提高时间性能，所以并没有太多的关注空间消耗。在实际开发过程中，指针域，也就是存放索引节点的地方，相比于值域而言，占的空间其实很少。所以很多时候，这部分空间损耗是可以忍受的。当然了这也要根据实际情况来说。
    
##### 时间性能
###### 再算算回报
有了上文的描述，跳跃表的结构已经清晰了。简单查询/插入操作也有了印象(在想到这些操作的时候，头脑里面是否会有画面感呢？)，依照着这幅图，再仔细的分析一下这个结构。每一个位于2^i位置的节点都有一个来自于距该节点2^i远的节点直接指向它。比如位置在2^2的节点，在据它有2^2远的地方有一个节点指向它。可以看出，节点的数量仅仅翻了一倍，但是进行一次搜索最多需要比较的节点的个数是⌈log*N*⌉，也就不难得出查询操作的时间复杂度是O(log*N*)。因为在跳跃表中，一次查询的过程从一个节点出发前进到一个新的节点，如过这个新的节点不是目标节点，且它的值比目标节点的大，那么会倒回到这个新节点前面的节点，然后下降到低一级的指针再往前走。每一步的时间消耗都至多为O(log*N*)。注意到这本质就是一个二分搜索的过程。

对于查询而言，真的很美好。但对于插入来说，情况就有些不同了，毕竟插入操作涉及对结构的更改。这会导致跳跃表节点分布不均匀。试想一下，如过在上面的图中插入一个节点会怎样。严格维护跳跃表结构的道理如同严格维持一棵AVL树一样，显得过于严苛，这种做法对于插入频繁的场景，是没有太多好处的。也因此，在保证性能还可以的前提下，结构上一定的松散在实际中，应用价值更大，就像RB树。对于跳跃表，通过随机决定新节点指针阈的数量，而不是硬性决定。实验表明跳跃表时间性能如同许多平衡搜索树一样的高效而且，它的实现更为简单(相比于实现一个平衡二叉树)。

![](https://github.com/WalkingNL/Pics/blob/master/SkipList7.jpg)

### Redis中的SkipList
在进一步阅读本节的内容之前，可以先看一下我翻译的一篇[文章](https://github.com/WalkingNL/Redis/blob/master/Redis%20Sorted%20sets(译文).md)，当然了，你也可以直接去官网阅读，这是[地址](https://redis.io/topics/data-types-intro)。

上面的内容，基础性的介绍了跳跃表。在本节中，将会剖析跳跃表在Redis中的应用。准确的说是在Sorted set中的应用。SkipList作为在Redis中，有序集合键的底层实现之一(另一个是hash table)，当一个有序集合包含的元素比较多，又或者有序集合中元素的成员(member)是比较长的字符串时，Redis就会选择跳跃表作为有序集合的底层实现。此外，在Redis中，另一个用到SkipList这种数据结构的地方就是在集群节点中(源自黄健宏的《Redis设计与实现》一书)。

Redis的跳跃表由`server.h/zskiplistNode`和`server.h/zskiplist`两个结构体定义，源码如下所示。

    /* ZSETs use a specialized version of Skiplists */
    typedef struct zskiplistNode {
        sds ele; // 保存成员对象。在之前的版本中，这里直接是 robj *obj，但之后这里做了一个转换，详细过程见下面的代码
        double score; // 成为分值，跳跃表中，节点按各自所保存的分值从小到大排列
        struct zskiplistNode *backward; // 后退指针，指向当前节点的前一个节点。作用是从后往前的遍历
        struct zskiplistLevel {
            struct zskiplistNode *forward; // 前进指针，指向当前后续的指针
            unsigned long span; // 跨度，记录forward指向的节点，和当前节点的距离
        } level[]; // 
    } zskiplistNode;

    typedef struct zskiplist {
        struct zskiplistNode *header, *tail; // 分别指向跳跃表结构的头节点及尾节点
        unsigned long length; // 当前跳跃表中包含的节点的个数
        int level; // 记录跳跃表中，层数的最大值，也即指针域个数的最大值
    } zskiplist;
    
    
 `sds ele`的出现，为什么要做这样的一个转换呢？
 
     typedef struct client {
        size_t querybuf_peak;   /* Recent (100ms or more) peak of querybuf size. */
        int argc;               /* Num of arguments of current command. */
        robj **argv;  // 当前命令参数
        struct redisCommand *cmd, *lastcmd;  /* Last command executed. */
    } client;

/* This generic command implements both ZADD and ZINCRBY. */
  
    void zaddGenericCommand(client *c, int flags) {
        static char *nanerr = "resulting score is not a number (NaN)";
        robj *key = c->argv[1];
        robj *zobj;
        sds ele;
        ......

        for (j = 0; j < elements; j++) {
            ......
            ele = c->argv[scoreidx+1+j*2]->ptr; // 由于argv是client结构体的成员变量，能够看到argv是robj类型的，
            .....
        }
    }












