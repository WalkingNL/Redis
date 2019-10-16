原文在[这儿](often it is handy to let it automatically evict old data as you add new data)

### Redis用作LRU缓存
当Redis被用作缓存时，随着新数据的加入，那些老的数据就会被自动的淘汰掉。这种做法堪称实用，所以，在主流的*memcached*系统中，已默认化。实际上，对于LRU而言，只是众多支持这种淘汰策略的方法之一。本篇针对Redis的**maxmemory**指令(固定最大内存用量)，将会讨论非常广泛的主题，并且，也会深度上，细致介绍LRU算法在Redis中如何使用。从Redis4.0开始，引入了一种新的数据淘汰机制，叫做LRU(Least Frequently)，本篇会用一个单独的模块介绍它。

###### 最大内存配置指令
使用**maxmemory**配置指令指定Redis对数据集的内存用量。可以在配置文件**Redis.conf**中进行设置，不过后来，也可以在运行状态中，通过命令[CONFIG SET](https://redis.io/commands/config-set)进行设置。

例如，如果限制内存为100M，在Redis.conf文件中，使用下面的指令。

    maxmemory 100mb
默认情况下，对于64位系统，设置maxmemory为0，预示着内存使用无限制。但对于32位系统，maxmemory为0，则表示限制内存为3G。一旦内存的用量达到了这个限制，针对不同的行为，应对策略不同。对于那些会导致更多内存被使用的命令，Redis选择的策略是仅返回错误。或者它会淘汰掉那些老的数据，以便每次添加新数据时，返回到指定的限制。
###### 淘汰策略
具体来说，当**maxmemory**限制达到时，Redis遵从的行为通过使用配置指令**maxmemory-policy**进行配置。

可用的策略有以下：
  * **noeviction**: 当内存的限制达到，然后客户端尝试执行会导致更多内存被占用的命令时，直接返回错误(如write命令，[DEL](https://redis.io/commands/del)及部分特殊命令除外);
  * **allkeys-lru**: 在所有的键中，优先使用LRU算法进行淘汰。
  * **volatile-lru**: 对设置了过期时间的keys，优先使用LRU算法进行淘汰。
  * **allkeys-random**: 在所有键中，进行随机的删除。
  * **volatile-random**: 对设置了过期时间的键，在其中进行随机的删除。
  * **volatile-ttl**: 对设置了过期时间的键，优先尝试淘汰存活时间最短的。

如果没有键可淘汰，那么以**volatile-**开头的
