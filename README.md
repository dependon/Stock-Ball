# 2022/3/14
新浪api更新，现在目前可以正常使用大部分功能的日线图了


# 股票球，采用的是qt和新浪的api来设计，目前只有查看功能，2021年1月17日开始开发，后续可能会持续更新（可能跟心情有关）

# k线图在 Windows下获取数据有问题，还没来得及做，目前在Linux下没有问题，并打了一个deb包qt5.11.3制作的，可以玩一下，Windows下的版本会在以后加吧(2021/4/1)

## 编译
只需要qt的原生环境即可，目前开发环境是5.11.3,系统是Uniontech OS国产操作系统，测试其他linux和windows10均可以使用，当前无外部依赖（后续心情好会增加点有意思的东西）
## 如何使用
打开程序，会出现一个小球，如图所示，右键打开设置主界面可以配置，数据库采用的是本地数据库sqlite，不需要额外创建，代码里面有创建

linux会出现在右上角，windows会出现在屏幕中间

![image](./img/1.png)

![image](./img/4.png)



右下角有托盘，可以影藏小球



![image](./img/3.png)



自选可以查看每日的涨跌

![image](./img/2.png)



持有界面就可以配置小球的每日涨跌幅度

![image](./img/7.png)



通过添加我的持股添加

![image](./img/5.png)



双击行可以更改数据，右键可以删除数据

![image](./img/6.png)

曲线图
![image](./img/8.png)
![image](./img/9.png)
## 联系我（欢迎提供意见和帮助）

liuminghang0821@qq.com

## 参考
小球的设计参考
https://github.com/sonichy/HTYFloatBall
曲线图参考
https://github.com/cetcjinjian/QT_StockFigure
