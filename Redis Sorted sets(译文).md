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

实现注意：sorted set的实现经由一个包含skip list和hash table的双端口的数据结构完成。因此，每次执行添加一个元素的操作，Redis的时间效率为O(log(N))。这很好，但是当我们请求有序的元素时，Redis不需要做任何工作，因为所有元素已经排好序。
