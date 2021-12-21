**EulerOperation**

**Created by Willken**

编译运行环境：macOS Monterey 版本12.0.1  芯片：Apple M1

环境要求：通过homebrew安装`GLFW`,`GLUT`

运行方式：

​	1）进入build目录

​	2）执行：`cmake .`

​	3）执行：`make`

​	4）执行：`./EulerOperation-zwk model1.zwk model2.zwk model3.zwk`

操作说明：

Switch view directions:  direction buttons ⬆️ ⬇️ ⬅️ ➡️ / click and drag the left button

Switch solids:  Enter / click the right button



.zwk文件格式说明：

第一行表示sweeping向量（x, y, z）以及距离 d

接下来依次是点的坐标（x, y, z）以及环的标记（0是外环，1，2，3等是内环）

演示视频地址：https://www.bilibili.com/video/BV1Br4y1k7PK?spm_id_from=333.999.0.0
