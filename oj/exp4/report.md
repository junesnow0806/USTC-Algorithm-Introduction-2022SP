# 算法基础实验4报告

***

梁峻滔 PB19051175

***

## 1. 次小生成树

先使用Prim算法找出最小生成树，再考察不在最小生成树中的每条边e, 将e加入到最小生成树后, 删除所形成的环上权值最大的边, 得到新的生成树, 记录这样得到的所有生成树中的权值最小值, 就是次小生成树的权值. 

在使用Prim算法计算最小生成树的权值时, 用一个二维数组 h 记录每个点对之间在MST路径上最大权值边的权值. 设u是新加入MST的顶点, v是u的前驱顶点(u加入MST时与u相连的顶点), 则对所有已经在MST中的顶点t, 有
$$
h[t][u] = min(u.key, h[t][v])\\
h[u][t] = min(u.key, h[v][t])
$$
最后考察将不在MST中的一条边 e 加入到树中时, 设 u 和 v 是 e 的两个顶点, 则 h\[u]\[v] 就是将该边加入到MST后所形成的环上权值最大的边的权值, 此时得到的生成树的权值为
$$
T\_weight\_e = MST\_weight - h[u][v] + w[u][v]
$$
那么次小生成树的权值就是
$$
sub\_MST\_weight = \mathop{min}\limits_{e∉MST}\{T\_weight\_e\}
$$
实现时印象比较深刻的一个坑是: 伪代码算法中一个顶点可以随机访问(即取即用), 修改key值后会反映到优先队列中位置的变化, 而实现时STL优先队列中的元素只要入队了就不可修改, 所以不能像算法那样很方便地使用优先队列来实现(除非自己维护).

## 2. 可达性查询

主要思路: 找出图中的所有强连通分量, 并生成分量图, 查询 u 是否可达 v 时, 如果两个点在同一个强连通分量中, 则可达; 否则从 u 所在的强连通分量出发, 在分量图中做DFS, 如果可以到达 v 所在分量, 则可达, 否则不可达.

为每个顶点设置一个cno域, 表示该顶点所属强连通分量编号. 首先对 G 进行一次DFS, 记录每个顶点的 f 值(explore finish stamp). 之后按 f 值降序对$G^T$ 进行DFS,  对每个顶点进行强连通分量编号的赋值, 这样实际上就找出了所有的强连通分量. 之后遍历每一条边, 生成分量之间的邻接表. 最后查询时根据分量邻接表进行分量DFS来查询.

实现时印象比较深刻的一个坑是$G^T$中的顶点是$G$的一个副本, 两者的f值信息和cno信息并不会自动同步, 需要辨别什么时候使用哪一个.

## 3. 顶点距离

### Floyd-Warshall算法

使用动态规划的思想. 定义 $d^k[i][j]$ 为从顶点 i 到顶点 j 的 **所有中间顶点只在 { 1, 2, ..., k } 集合中**的所有路径中的最短距离. 则状态转移方程为
$$
d^k[i][j] =
\begin{cases}
w[i][j] \quad k = 0\\
min\{ d^{k-1}[i][j], d^{k-1}[i][k]+d^{k-1}[k][j] \} \quad k > 0
\end{cases}
$$
  由于$d^k$只依赖于$d^{k-1}$, 而且最后的结果只需要$d^n$, 所以并不需要创建多个d数组, 只用一个即可. 采用如下算法:

![image-20220520205237454](D:\USTC\Algotithm2022\oj\exp4\report.assets\image-20220520205237454.png)

算法时间复杂度为$O(n^3)$, 与边数无关. 在使用Floyd-Warshall算法计算前, 需要先执行一次 Bellman-Ford 算法检测是否存在负环.

### Johnson算法

基本思想: 将整个图的权重调整为全部非负且能保持最短路径, 然后对每个顶点调用 Dijkstra 算法求解. 调整权重为
$$
\hat{w}[i][j] = w[i][j] + h[i] - h[j]
$$
这样的话, 对于固定起点和终点的一条路径$i_1, ..., i_l$, 路径权重和为
$$
\sum_{j=1}^{<l}\hat{w}[i_j][i_{j+1}] = h[i_1] - h[i_l] + \sum_{j=1}^{<l}w[i_j][i_{j+1}]
$$
对于固定的起点$i_1$和终点$i_l$, $\hat{w}$ 求和最小的路径也是$w$求和最小的路径. 

接下来考虑使$\hat{w}$非负. 考虑单源最短路径问题中有
$$
d[s][j] \leq d[s][i] + w[i][j]
$$
因此有
$$
w[i][j] + d[s][i] - d[s][j] \geq 0
$$
因此可以选定一个s, 然后令$h[i] = d[s][j]$. 考虑负边和图中可能存在多个强连通分量的问题, 考虑增加一个源点s, s到原图中每个顶点连一条权重为0的边, 然后用Bellman-Ford算法计算s到原图中所有顶点的最短距离作为h值. 

然后使用 $\hat{w}$ 使用 n 次 Dijkstra 算法计算最短距离, 最后再换算回去. 采用如下算法:

![image-20220520211631992](D:\USTC\Algotithm2022\oj\exp4\report.assets\image-20220520211631992.png)

时间复杂度为$O(nm\ log\ n)$.

### 两个算法的优缺点分析

从两个算法的理论时间复杂度来看, 对于稀疏图(边数m较小), Johnson算法应该有更好的性能表现, 而对于稠密图, 例如 $m \gt n^2$ 时, Floyd-Warshall算法的性能应该表现得更好. 但是对于稀疏图, Johnson算法的时间性能应该表现得更好.

构造**稠密图**测试样例的python程序如下:

```python
import random

n = 700 # 顶点数
m = 800000 # 边数
file = open("4-3-gen.txt", "w")
file.write(str(n))
file.write(" ")
file.write(str(m))
file.write("\n")
maxw = 10000
edges = set()
count = 0
while True:
    w = random.randint(-1, maxw)
    u = random.randint(1, 600)
    v = random.randint(1, 600)
    while v == u:
        v = random.randint(1, 600)
    edge = (u, v, w)
    if edge not in edges:
        edges.add(edge)
        count += 1
    if count >= m:
        break
    
for edge in edges:
    u = edge[0]
    v = edge[1]
    w = edge[2]
    file.write(str(u))
    file.write(" ")
    file.write(str(v))
    file.write(" ")
    file.write(str(w))
    file.write("\n")

q = random.randint(1, 4200)
file.write(str(q))
file.write("\n")
for count in range(0, q):
    u = random.randint(1, 600)
    v = random.randint(1, 600)
    while v == u:
        v = random.randint(1, 600)
    file.write(str(u))
    file.write(" ")
    file.write(str(v))
    file.write("\n")
    
file.close()
```

两种算法的运行时间分别为

| Floyd-Warshall | Johnson   |
| -------------- | --------- |
| 13780.2ms      | 19972.6ms |

测试中确实是 Floyd-Warshall 算法的时间性能表现得更好.

将顶点数和边数均改为1000, 生成新的测试数据后再次运行比较, 得

| Floyd-Warshall | Johnson   |
| -------------- | --------- |
| 16612.4ms      | 10804.9ms |

在顶点数多的稀疏图中确实是 Johnson 算法表现得更好.

#### Floyd-Warshall算法优缺点

* 优点
    * 主体只包括一个三重循环, 容易实现
    * 空间开销少, 主要只包含权重矩阵和d数组
    * 在稠密图中性能表现更好
* 缺点
    * 理论上在顶点数多的稀疏图中的时间性能不及 Johnson 算法

#### Johnson算法优缺点

* 优点
    * 理论上在顶点数多的稀疏图中的时间性能会远远好于 Floyd-Warshall 算法
* 缺点
    * 逻辑更复杂, 实现起来也更复杂(需要实现 Dijkstra 算法和重新调整权值矩阵)
    * 空间开销大更大(Dijsktra算法中需要维护一个优先队列, 重新调整权值矩阵也需要存下新的权值矩阵)
    * 在稠密图中性能表现不及 Floyd-Warshall 算法

踩的坑: Bellman-Ford算法发现负环时如果是输出信息后exit(-1)就会导致runtime error, 需要从main返回.

## 4. 最大流

两种贪心改进:

* shortest path: 使用 BFS 寻找增广路径.
* fattest path: 使用带贪心选择的 DFS 寻找增广路径, DFS **选择下一个邻接顶点时选择剩余容量最大的边所对应的另一端顶点**. 实现时使用一个栈存放 DFS 探索过的顶点, 当从当前顶点出发找不到增广路径时可以通过弹出来回溯.

### 分析两个算法的优缺点

根据课堂上的讨论, shortest 贪心最多会增广$O(nm)$次, 而 largest 贪心最多会增广$O(m\ log|f^*|)$次. 直观上, 保持最大流量不变时, 随着顶点数增多, largest 贪心方法应该会显示出更好的时间性能. 而保持顶点数不变时, 随着最大流流量的增加, shortest 贪心方法应该会显示出更好的时间性能.

构造如下unit, 对该unit使用 shortest 贪心需要增广3次, 而使用 largest 贪心需要增广2次, 直观上 largest 贪心会更快.

![微信图片_20220521122430](D:\USTC\Algotithm2022\oj\exp4\report.assets\微信图片_20220521122430.jpg)

为了消除实现上的差异所带来的时间开销, 考虑将该 unit 复制多份来构成最后的图, 如下所示.

![微信图片_202205211224301](D:\USTC\Algotithm2022\oj\exp4\report.assets\微信图片_202205211224301.jpg)

构造测试数据的 python 源码为

```python
unit_num = 2000 # 指定unit数
n = 5 * (unit_num-1) + 6
edges = []
for i in range(0, unit_num):
    v1 = 5 * i + 1
    v2 = 5 * i + 2
    v3 = 5 * i + 3
    v4 = 5 * i + 4
    v5 = 5 * i + 5
    v6 = 5 * i + 6
    edges.append([v1, v2, 1000])
    edges.append([v1, v4, 5])
    edges.append([v2, v4, 1000])
    edges.append([v2, v3, 5])
    edges.append([v4, v5, 1000])
    edges.append([v3, v5, 1000])
    edges.append([v3, v6, 5])
    edges.append([v5, v6, 1000])

file = open("../testcase/4-4-gen.txt", "w")
file.write(str(n))
file.write(' ')
file.write(str(len(edges)))
file.write('\n')
for edge in edges:
    u = edge[0]
    v = edge[1]
    w = edge[2]
    file.write(str(u))
    file.write(' ')
    file.write(str(v))
    file.write(' ')
    file.write(str(w))
    file.write('\n')
file.write(str(1))
file.write(' ')
file.write(str(n))
file.close()
```

运行结果为

![image-20220521122952238](D:\USTC\Algotithm2022\oj\exp4\report.assets\image-20220521122952238.png)

随着unit数的增多, 两者的运行时间差会增大. 在该测试数据上 largest 贪心优于 shortest 贪心.

而保持unit数不变(例如固定为1000), 增大最大流流量时(从2000到10000), 增大到一定程度后, shortest贪心的运行时间与largest贪心的运行时间差值缩减不明显了, 而且仍然是 largest 贪心比 shortest 贪心快. 可能的原因是: 上面讨论的增广次数是**最多增广次数**, 对于路径不复杂(例如这里所给的构造样例)的图, 两种方法实际需要的增广次数并不会达到上述上界的量级. 这里不继续深入研究了.

#### shortest贪心

* 优点

    选择增广路径的实现相对简单, 基本直接套用BFS框架即可.

* 缺点

    * 时间复杂度与顶点数有关, 对于顶点数多的图, 时间性能一般会不如 largest 贪心.
    * 空间开销可能相对较大(需要将多层顶点都存放在队列中)

#### largest贪心

* 优点

    * 时间复杂度与顶点数无关, 虽然与最大流流量$|f^*|$有关, 但是关于$|f^*|$是对数量级, 因此对于较大范围的$|f^*|$, 其时间性能一般都会优于 shortest 贪心.
    * 空间开销可能相对较小(只需要将当前路径上的顶点存放在栈中)

* 缺点

    选择增广路径的实现相对复杂, 在 DFS 时需要增加对下一条边的选择(选择剩余容量最大).
