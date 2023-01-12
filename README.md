# 基于EasyX的简单五子棋游戏
## 2018年6月第一次游戏开发记录
## 问题要求及任务描述
#### 题目要求
编写一个五子棋的游戏程序
## 主要任务
 - 要有棋盘
 - 设计输、赢判断规则函数
 - 给出下棋过程
 - 实现人机对战
## 解决问题的主要思路和方法
#### 关键问题
 - 棋盘的绘制
 - 棋子的绘制，必须落在棋盘的交叉点处
 - 有棋子的地方不能重复落子
 - 棋子的黑白交替
 - 五子连珠的判断（即游戏输赢判断）
 - AI设计
#### 拟采用解决问题的方法及算法
- 棋盘的绘制：调用EasyX图形库，绘制坐标格。
- 绘制棋子：通过填充一个椭圆的方式来绘制棋子,
落子位置：首先定义一个二维数组a[][]，用二维数组的两个参数来对应一个交叉点的X,Y坐标，通过鼠标点击的位置所得坐标来整除棋盘格子的宽度（引入绝对值）。这样就得到了鼠标点击的位置的最近的一个交叉的位置。然后通过交叉点的位置信息，以交叉点为圆心来绘制相应的棋子。
- 有棋子的位置不能重复落子：可以给二维数组a[][]付初值 为“0”，当 capture[x][y] == 0 时表示无子可以落子，当capture[x][y] ! = 0 时就不能落子。
- 棋子的黑白交替：定义一个变量flag，通过递增的方式记录交替，为偶时则黑，为奇时则白。
- 五子连珠的判断： 对上一次落子的位置进行判断，分别从四个方向（横竖左斜右斜）判断是否有五个连续位置有capture[x][y] ==1（黑）或capture[x][y]==2（白）。
#### 主要算法和处理流程图以及类图等
![1](https://img-blog.csdnimg.cn/35df7ecbdb2d41b49805d1e2141b3fc6.png)

## 程序实现
#### 程序实现时应考虑的问题
- EasyX帮助文件的查阅与使用
- 程序布局以及各个函数的正确声明

#### 主要源代码及说明
##### 总体设计
主要有PLAYAI和PLAYHUMAN两种模式，且带有重启功能 
##### 详细设计
- AI基础设计
```cpp
void AiDefend()  //电脑防守
{
	//上 下 左 右 右上 左下 左上 右下
	int a[3] = { 0,1,-1 };
	MAX max;       //储存最大权重坐标
	int iWeight;   //权重值
	int iNum;      //用于计分
	int iScore[5] = { 0,1,10,100,1000 };
	int MaxWeight;
	int m, n;
	//同方向四黑 --》 马上堵截 +1000
	//同方向三黑 --》 马上堵截 +100
	//同方向两黑 --》 不必马上堵截 +10
	//同方向一黑 --》 不必马上堵截 +1
	//无黑子    --》 +0
	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			if (capture[x][y] != 0)  // 有子
			{
				AiDefendWeight[x][y] = 0;
				continue;
			}
			else
			{
				//对没落子的地方求权重
				//对八个方向搜索敌方棋子
				iWeight = 0;
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
							iNum = GetChessNum(HumanColor, x, y, a[i], a[j]);
						    iWeight += iScore[iNum];
					}
				}
				AiDefendWeight[x][y] = iWeight;
			}
		}
	}
	MaxWeight = AiDefendWeight[0][0];
	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			if (MaxWeight < AiDefendWeight[x][y])
			{
				MaxWeight = AiDefendWeight[x][y];
				max.x = x * 50;
				max.y = y * 50;      //大棋盘
				m = x;               //小棋盘
				n = y;
			}
		}
	}
	setfillcolor(WHITE);
	solidcircle(max.x, max.y, 20);   //落子
	capture[m][n] = 2;               //更改状态
}

int GetChessNum(int BW, int x, int y, int xStep, int yStep)
{
	int xTmp = x + xStep;  //坐标
	int yTmp = y + yStep;  //移动
	int Count = 0; //颜色一致的棋子数
	while (xTmp >= 0 && xTmp < 20 && yTmp >= 0 && yTmp < 20) //防止出界
	{
		if (capture[xTmp][yTmp] == BW)
		{
			Count++;
			xTmp += xStep;
			yTmp += yStep;
		}
		else
			break;
	}
	return Count;
}
```
    
- 连五判定函数

```cpp
int judgeWin(int a, int b)
{
	//坐标移动规则：
	//上移或左移为负
	//下移或右移为证
	int i, j;
	int t = 2 - flag % 2;
	for (i = a - 4, j = b; i <= a; i++)  
	{   // 边界不能落子
		if (i >= 1 && i <= 15 && t == capture[i][j] && t == capture[i + 1][j]
			&& t == capture[i + 2][j] && t == capture[i + 3][j] && t == capture[i + 4][j])
			//x轴方向
			return 1;
	}
	for (i = a, j = b - 4; i <= a; i++)
	{
		if (j >= 1 && j <= 15 && t == capture[i][j] && t == capture[i][j + 1]
			&& t == capture[i][j + 2] && t == capture[i][j + 3] && t == capture[i][j + 4])
			//y轴方向
			return 1;
	}
	for (i = a - 4, j = b - 4; i <= a, j <= b; i++, j++)
	{
		if (i >= 1 && i <= 15 && j >= 1 && j <= 15 && t == capture[i][j] && t == capture[i + 1][j + 1]
			&& t == capture[i + 2][j + 2] && t == capture[i + 3][j + 3] && t == capture[i + 4][j + 4])
			//左斜方向
			return 1;
	}
	for (i = a - 4, j = b + 4; i <= a, j >= 1; i++, j--)
	{
		if (i >= 1 && i <= 15 && j >= 1 && j <= 15 && t == capture[i][j] && t == capture[i + 1][j - 1]
			&& t == capture[i + 2][j - 2] && t == capture[i + 3][j - 3] && t == capture[i + 4][j - 4])
			//右斜方向
			return 1;
	}
	return 0;
}
```

- 主功能函数（下棋）

```cpp
void playChess1()
{
	int x = 0, y = 0;
	int a = 0, b = 0;
	MOUSEMSG m;
	HWND chuangkou; //窗口句柄
	chuangkou = GetHWnd(); //窗口置前

	while (1)
	{
		m = GetMouseMsg();
		for (int i = 1; i < 20; i++)
		{
			for (int j = 1; j < 20; j++)
			{
				//引入绝对值 提升游戏体验
				if (abs(m.x - i * 50) < 25 && abs(m.y - j * 50) < 25)
				{
					a = i;
					b = j;  //row & col
					x = i * 50;
					y = j * 50;
				}
			}
		}
		if (m.uMsg == WM_LBUTTONDOWN)   //检测到鼠标输入
			if (flag % 2 == 1)  //轮到电脑下 白棋为奇
			{
				return;
			}
			else  //轮到玩家下 黑棋为偶
			{
				if (capture[a][b] != 0)
				{
					MessageBox(chuangkou, "你好像不会下棋", "Error", MB_OK);
					continue;
				}
			mciSendString("play piaji.mp3", NULL, 0, NULL);
			setfillcolor(BLACK);     //
			solidcircle(x, y, 20);   //落子
			capture[a][b] = 1;       //状态更改
			flag++;                  //顺序更改
			}
		if (judgeWin(a, b))  //Find Win
		{
			if (flag % 2 == 1)  //看下一步是谁下
			{
				MessageBox(chuangkou, "Black is the winner!", "Game Over", MB_OK);
				RestartGame();
			}
			else {
				MessageBox(chuangkou, "White is the winner!", "Game Over", MB_OK);
				RestartGame();
			}
		}
		StartAI();   //AI输入
		if (judgeWin(a, b))  //Find Win
		{
			if (flag % 2 == 1)
			{
				MessageBox(chuangkou, "Black is the winner!", "Game Over", MB_YESNO);
				RestartGame();
			}
			else {
				MessageBox(chuangkou, "White is the winner!", "Game Over", MB_YESNO);
				RestartGame();
			}
		}
	}
}

void playChess2()
{
	int x = 0, y = 0;
	int a = 0, b = 0;
	MOUSEMSG m;
	HWND chuangkou; //窗口句柄
	chuangkou = GetHWnd(); //窗口置前

	while (1)
	{
		m = GetMouseMsg();
		for (int i = 1; i < 20; i++)
		{
			for (int j = 1; j < 20; j++)
			{
				//引入绝对值 提升游戏体验
				if (abs(m.x - i * 50) < 25 && abs(m.y - j * 50) < 25)
				{
					a = i;
					b = j;  //row & col
					x = i * 50;
					y = j * 50;
				}
			}
		}
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (capture[a][b] != 0)
			{
				//避免重复落子
				MessageBox(chuangkou, "你好像不会下棋", "Error", MB_OK);
				continue;
			}
			if (flag % 2 == 0)
			{
				setfillcolor(BLACK);
				solidcircle(x, y, 20);
				capture[a][b] = 1;
			}
			else
			{
				setfillcolor(WHITE);
				solidcircle(x, y, 20);
				capture[a][b] = 2;
			}
			flag++;
		}
		if (judgeWin(a, b))  //Find Win
		{
			if (flag % 2 == 1)
			{
				MessageBox(chuangkou, "Black is the winner!", "Game Over", MB_OK);
				RestartGame();
			}
			else {
				MessageBox(chuangkou, "White is the winner!", "Game Over", MB_OK);
				RestartGame();
			}
		}
	}
}
```

- 初始化（绘制棋盘）

```cpp
void initGame()
{
	AiColor = 2;       //电脑下的标志
	HumanColor = 1;    //人下的标志
	flag = 0;
	initgraph(1200, 800);      //界面大小
	loadimage(NULL, "1.jpg");  //引入背景图片
	setlinecolor(BLACK);       //设置画线颜色
	for (int x = 0; x < 20; x++)
	{
		for (int y = 0; y < 20; y++)
		{
			capture[x][y] = 0;    //清空棋盘
		}
	}
	for (int i = 1; i < 20; i++)
	{
		line(i * 50, 0, i * 50, 800);
		line(0, i * 50, 1000, i * 50);
	}
	setlinestyle(PS_SOLID, 2);     //
	line(1000, 0, 1000, 1000);     //画棋盘
	settextcolor(BLACK);           //
	setbkmode(0);
	settextstyle(40, 0, _T("微软雅黑"));    //字体
	outtextxy(1020, 60, "黑道圣徒");
	outtextxy(1055, 120, "VS");
	outtextxy(1020, 180, "白面书生");        //界面设计
}
```

- 主调控结构

```cpp
void initGame();      //初始化棋盘
void playChess1();    //人机
void playChess2();    //人人
void PLAYAI();        //选择人机模式
void PLAYHUMAN();     //选择人人模式
void StartAI();       //AI算法
void AiDefend();      //权重法防守战术
void ChooseMode();    //选择界面
void RestartGame();   //再来一局
int judgeWin(int a, int b);  //判定胜负
int GetChessNum(int Fix, int x, int y, int xStep, int yStep);
                      //遍历棋盘
```



## 测试
#### 游戏测试结果及分析
- 游戏模式测试：
 
![2](https://img-blog.csdnimg.cn/92c481d69ff44eb9ad280aded49f31b0.png)

- 检验判断胜利：
 ![3述](https://img-blog.csdnimg.cn/2fbbe621f7fc46c5b30ab0b8702b9adc.png)
![3](https://img-blog.csdnimg.cn/2eba52c483cc48e89a204a5b921e1a5a.png)
![4](https://img-blog.csdnimg.cn/d3f1c3391afb491385958917bff4e176.png)


 
 
游戏程序能正常运行，并且游戏功能正常。

## 小结
由于游戏开发本身具有的延展性，让我对此程序的优化之路非常乐观，即可添加的丰富模块让整个设计变得十分有趣。但开发过程中仍遇到了一些问题，比如程序不够精简，播放音效存在延迟等等。其中针对重新开始游戏的功能，我重新架构了main函数，并把初始化和棋局放在一起构成新的函数，引入了ChooseMode和RestartGame两个函数，以此避免不必要的赘余以及整个结构的清晰。
