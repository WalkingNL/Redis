原文在[这儿](often it is handy to let it automatically evict old data as you add new data)

### Redis用作LRU缓存
当Redis被用作缓存时，随着新数据的加入，那些老的数据就会被自动的淘汰掉。这种做法堪称实用，所以，在主流的*memcached*系统中，已默认化。实际上，对于LRU而言，只是众多支持这种淘汰策略的方法之一。本篇针对Redis的**maxmemory**指令(固定最大内存用量)，将会讨论非常广泛的主题，并且，也会深度上，细致介绍LRU算法在Redis中如何使用。从Redis4.0开始，引入了一种新的数据淘汰机制，叫做LRU(Least Frequently)，本篇会用一个单独的模块介绍它。

###### 最大内存配置指令
使用**maxmemory**配置指令指定Redis对数据集的内存用量。可以在配置文件**Redis.conf**中进行设置，不过后来，也可以在运行状态中，通过命令[CONFIG SET](https://redis.io/commands/config-set)进行设置。

例如，如果限制内存为100M，在Redis.conf文件中，使用下面的指令。

    maxmemory 100mb
默认情况下，对于64位系统，设置maxmemory为0，预示着内存使用无限制。但对于32位系统，maxmemory为0，则表示限制内存为3G。一旦内存的用量达到了这个限制，针对不同的行为，应对策略不同。对于那些会导致更多内存被使用的命令，Redis的选择的策略是仅返回错误。对于每次添加新的数据这种情况而言，Redis会淘汰老的数据到指定的内存限制以内。

###### 淘汰策略

