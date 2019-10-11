可以在这里查看[原文](https://redis.io/topics/data-types-intro)

## Redis Sorted sets
Sorted sets是Redis中的一种数据类型，近似于一个Set和一个Hash的混合体。它拥有set的特征，这是因为它由唯一的，无重复的字符串元素组成。也因此，从某种意义上说，一个sorted set其实就是一个set。

但是，虽然sets内部的元素并未进行排序，但在一个sorted set里，每一个元素却都与一个被称为`score`的浮点型值相关联(这也就是为什么sorted set又跟hash有相似之处的原因，因为每一个元素都被映射为一个值)。

此外，sorted set中的元素会被按顺序放置(因此，在请求时并不会进行排序，这个顺序是用来表示Sorted set这个数据结构的特殊性的)，依据的规则如下：
  * 如果A和B的score值不等，那么如果A.score > B.score,则A>B.
  * 如果A和B的score值相等，那么如果A字符串的字典顺序大于B字符串，则A>B. 由于sorted set其元素具有唯一性，故A与B不可能相等。

一起看一个简单的示例，用一些被选定的Hacker的名字作为sorted set的元素，用他们的出生年份作为score。

    > zadd hackers 1940 "Alan Kay"
    (integer) 1

    > zadd hackers 1957 "Sophie Wilson"
    (integer) 1

    > zadd hackers 1953 "Richard Stallman"
    (integer) 1

    > zadd hackers 1949 "Anita Borg"
    (integer) 1

    > zadd hackers 1965 "Yukihiro Matsumoto"
    (integer) 1

    > zadd hackers 1914 "Hedy Lamarr"
    (integer) 1

    > zadd hackers 1916 "Claude Shannon"
    (integer) 1

    > zadd hackers 1969 "Linus Torvalds"
    (integer) 1

    > zadd hackers 1912 "Alan Turing"
    (integer) 1

表面上看，命令[ZADD](https://redis.io/commands/zadd)与命令[SADD](https://redis.io/commands/sadd)非常相似，但ZADD要比SADD多一个参数，这个参数就是`score`，它被放置在被添加进的元素前。[ZADD](https://redis.io/commands/zadd)其实也是一种可变参命令，因此，你可以随意指定多个score-value这样的分数-值对。当然了，上述例子中并没有体现这一点。

使用sorted set，返回按照hackers的出生年月排序的列表非常简单，因为实际上，他们已经排好序了。

实现说明：sorted set的实现经由一个包含skip list和hash table的双端口的数据结构完成。因此，每次执行添加一个元素的操作，Redis的时间效率为O(log(N))。这一点很好，但是当我们请求有序的元素时，Redis不必再做任何工作，因为所有元素已经排好序。

    > zrange hackers 0 -1
    1) "Alan Turing"
    2) "Hedy Lamarr"
    3) "Claude Shannon"
    4) "Alan Kay"
    5) "Anita Borg"
    6) "Richard Stallman"
    7) "Sophie Wilson"
    8) "Yukihiro Matsumoto"
    9) "Linus Torvalds"  
注意：0和-1意味着从索引0开始到最后一个元素(-1在这里的作用就像使用[LRANGE](https://redis.io/commands/lrange)命令一样)。

如果我想要按照相反的顺序对它们排序，即从最年轻到最老这样的顺序，该怎么做呢？使用[ZREVRANGE](https://redis.io/commands/zrevrange)命令代替[ZRANGE](https://redis.io/commands/zrange)命令：

    > zrevrange hackers 0 -1
    1) "Linus Torvalds"
    2) "Yukihiro Matsumoto"
    3) "Sophie Wilson"
    4) "Richard Stallman"
    5) "Anita Borg"
    6) "Alan Kay"
    7) "Claude Shannon"
    8) "Hedy Lamarr"
    9) "Alan Turing"
也可以加上`WItHSCORES`参数，返回scores：

    > zrange hackers 0 -1 withscores
    1) "Alan Turing"
    2) "1912"
    3) "Hedy Lamarr"
    4) "1914"
    5) "Claude Shannon"
    6) "1916"
    7) "Alan Kay"
    8) "1940"
    9) "Anita Borg"
    10) "1949"
    11) "Richard Stallman"
    12) "1953"
    13) "Sophie Wilson"
    14) "1957"
    15) "Yukihiro Matsumoto"
    16) "1965"
    17) "Linus Torvalds"
    18) "1969"
### 在区间上的操作
除了以上，Sorted set还有更强大的用处，就是他们可以在区间上操作。我们要获取包含1950年之前出生的所有人的信息，使用命令[ZRANGEBYSCORE](https://redis.io/commands/zrangebyscore)：

    > zrangebyscore hackers -inf 1950
    1) "Alan Turing"
    2) "Hedy Lamarr"
    3) "Claude Shannon"
    4) "Alan Kay"
    5) "Anita Borg"
我们请求Redis返回score位于闭区间[-inf, 1950]里的全部元素。

在区间上执行删除元素的操作也是可以的。比如从sorted set中删除在[1940, 1960]里出生的所有的hacker的信息：

    > zremrangebyscore hackers 1940 1960
    (integer) 4
[ZREMRANGEBYSCORE](https://redis.io/commands/zremrangebyscore)命令的名称获取不是最恰当的，但它的作用非常强大，执行它能返回被删除元素的个数。

另一个针对sorted set元素而言，非常有用的操作是`get-rank`。可以用这个命令请求被排序元素集中，某个元素的位置。

    > zrank hackers "Anita Borg"
    (integer) 4
`ZREVRANK`命令也是一种`get-rank`操作，不过元素在排序方式上，与上面的相反。

###### 字典分数(Lexicographical scores)
在最近的Redis2.8中，引入了一个新的特征，即允许按字典的顺序表示的范围获取元素。假设一个sorted set中的全部元素均是以相等的score值被插入(元素会与C函数`memcmp`比较，因此能够确保无排序规则，并且每个Redis实例将以相同的输出回应)。

在字典顺序的区间上，进行操作的主要命令有[ZRANGEBYLEX](https://redis.io/commands/zrangebylex), [ZREVRANGEBYLEX](https://redis.io/commands/zrevrangebylex), [ZREMRANGEBYLEX](https://redis.io/commands/zremrangebylex)以及[ZLEXCOUNT](https://redis.io/commands/zlexcount)。

例如，我们再次添加我们著名的hackers名单，但这次待添加的所有元素的score为0:

    > zadd hackers 0 "Alan Kay" 0 "Sophie Wilson" 0 "Richard Stallman" 0
    "Anita Borg" 0 "Yukihiro Matsumoto" 0 "Hedy Lamarr" 0 "Claude Shannon"
    0 "Linus Torvalds" 0 "Alan Turing"
由于sorted set的有序规则属性，现在所有元素都已经是按照字典顺序排列的了：

    > zrange hackers 0 -1
    1) "Alan Kay"
    2) "Alan Turing"
    3) "Anita Borg"
    4) "Claude Shannon"
    5) "Hedy Lamarr"
    6) "Linus Torvalds"
    7) "Richard Stallman"
    8) "Sophie Wilson"
    9) "Yukihiro Matsumoto"
使用[ZRANGEBYLEX](https://redis.io/commands/zrangebylex)命令请求字典区间：

    > zrangebylex hackers [B [P
    1) "Claude Shannon"
    2) "Hedy Lamarr"
    3) "Linus Torvalds"
可以请求区间以内的或是区间以外的(依赖首字母)，另外，字符串的正负无限性分别用+和-指定。了解更多信息参考这个文档。

这个特征的引入非常重要，因为它允许我们使用sorted sets作为总的索引。举个例子，如果你想要通过一个128位的无符号整型作为参数索引查找元素，你必须做的所有事情是添加元素到具有相同score(例如0)值但由16个字节组成128数字的大端的sorted set中。当按字典有序时(已原始字节序列为准)，由于大端数字实际上也是按照数值有序的，因此你可以在128位的空间上，请求范围，然后取得丢弃前缀的元素的值。

如果你想要在一个更为完整的demo的上下文中，了解这个特征，查看[这个](http://autocomplete.redis.io/)。

###### 更新score：排行榜
在进行下一个主题前，对sorted set的一项最后的说明。sorted set的score可以在任意时刻进行更新。只需要对已包含在sorted set中的元素调用命令[ZADD](https://redis.io/commands/zadd)，即可对元素的score(即position)进行更新，时间复杂度是O(log(N))。正因如此，当有巨量的更新任务时，sorted sets是完全能胜任的。

对于这个特征的一个普遍用例是排行榜。所以，通常的应用像Facebook的游戏，在那儿，你可以结合这项技能，取得得分最高的用户，再加上get-rank操作，能够展示top-N用户，以及用户在排行榜中的排名。
