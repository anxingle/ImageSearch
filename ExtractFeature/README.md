Configure.h : 
	保存工程全局变量
Learn.h :
	学习码本
Train.h :
	抽取库图片特征
Test.cpp :
	单张图片测试以及整个文件夹图片的测试 

注意事项：
	SurfFeature.h : 抽取特征方式的选择（关键点或denseSurf，是否去除全0特征行）
	NearestNeighbors.h : 求两张图片最小距离的方式选择

	在工程当前目录下的log.txt中，可以查看是否有特征全为0的异常图片