# water-ripple-simulation

这是一个基于 SDL2 库的 C++ 水面波浪可视化程序。该程序使用了经典的 波动方程 (Wave Equation) 离散模拟算法，能够实时生成逼真的水面涟漪效果，并支持鼠标交互。
<img width="396" height="424" alt="img_v3_0212o_ef0e2a70-4544-4b22-83e5-02ef5988844g" src="https://github.com/user-attachments/assets/1c373a35-6387-4d07-80ee-3d954ae579e2" />


### 核心功能
- 实时模拟 ：通过双缓冲高度场模拟水波的扩散、干涉和衰减。
- 交互体验 ：点击或移动鼠标可在水面产生涟漪。
- 动态效果 ：程序会自动随机产生“雨滴”，保持水面活跃。
- 视觉渲染 ：将水面高度映射为深蓝到浅蓝的渐变色，模拟光影感。
### 代码实现
你可以查看 main.cpp 获取完整源码。以下是关键算法部分的解释：

1. 波动方程模拟 我们使用两个二维数组 current 和 previous 存储每个像素点的水面高度。每一帧，我们根据周围像素的高度来计算当前像素的新高度：

```
current[i][j] = (previous[i-1][j] + previous[i+1]
[j] + 
                 previous[i][j-1] + previous[i][j
                 +1]) / 2.0f - current[i][j];
current[i][j] *= DAMPING; // DAMPING (如 0.98) 模拟能
量损耗，使水波逐渐平息
```
2. 颜色映射 将模拟出的高度值映射到 RGB 空间，产生深浅交替的蓝色效果：

```
int b = 150 + static_cast<int>(val);      // 基础蓝色
int g = 100 + static_cast<int>(val * 0.8f); // 绿色
分量
int r = 50 + static_cast<int>(val * 0.5f);  // 少量
红色提升亮度
```
### 如何编译与运行
如果你已经在本地安装了 SDL2 库，可以使用以下命令编译：

```
# 通用编译命令
clang++ -std=c++11 -O3 main.cpp $(pkg-config 
--cflags --libs sdl2) -o water_sim

# 运行程序
./water_sim
```
注意 ：在当前MacBook Air（Apple M3）环境下，由于系统库路径的特殊性，我成功通过以下针对性的命令完成了编译：

```
# 编译命令
clang++ -arch x86_64 -std=c++11 -O3 main.cpp \
-I/Library/Developer/CommandLineTools/SDKs/MacOSX15.
2.sdk/usr/include/c++/v1 \
-isysroot /Library/Developer/CommandLineTools/SDKs/
MacOSX15.2.sdk \
$(pkg-config --cflags --libs sdl2) -o water_sim

# 运行程序
./water_sim
```
### 操作说明
- 鼠标点击/拖动 ：在水面制造涟漪。
- 自动雨滴 ：即使不操作，程序也会随机产生小涟漪。
- 关闭窗口 ：退出程序。
你可以直接运行已编译好的 water_sim 可执行文件来查看效果。
