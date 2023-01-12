
// 待完成：
// 1.AI进攻完善
// 2.小bug修复
// 3.悔棋

#include <graphics.h>
#include <stdio.h>
#include <windows.h>  
#pragma comment(lib, "Winmm.lib")

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

typedef struct MAX {
	int x;
	int y;
}max;

int AiDefendWeight[20][20] = { 0 };
int AiColor;
int HumanColor;

int flag = 0;  // who's turn?
int capture[20][20] = { 0 };  // whether chess exists


int main() {
	ChooseMode();
	getchar();
	return 0;
}

void PLAYAI()
{
	initGame();
	playChess1();
}

void PLAYHUMAN()
{
	initGame();
	playChess2();
}

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

//BW判断该方向上颜色相同的子，即是黑子还是白子
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

void ChooseMode()
{
	HWND chuangkou;
	chuangkou = GetHWnd();
	int ret = MessageBox(chuangkou, TEXT("想来一盘紧张刺激的人机大战吗？"), TEXT("CHOOSE"), MB_YESNO | MB_ICONQUESTION);
	if (ret == IDYES)
	{
		PLAYAI();
	}
	else
	{
		PLAYHUMAN();
	}
}


void RestartGame()
{
	HWND chuangkou;
	chuangkou = GetHWnd();
	int ret = MessageBox(chuangkou, TEXT("大侠重新来过？"), TEXT("CHOOSE"), MB_YESNO | MB_ICONQUESTION);
		if (ret == IDYES)
		{
			int ret2 = MessageBox(chuangkou, TEXT("你确定你能赢吗？"), TEXT("Watch Out"), MB_OK | MB_ICONWARNING);
			if (ret2 == IDOK)
				MessageBox(chuangkou, TEXT("请你认真考虑哦！"), TEXT("你会被虐的"), MB_OK);
			ChooseMode();
		}
		else
		{
			int ret2 = MessageBox(chuangkou, TEXT("请继续加油哦！"), TEXT("再见"), MB_OK | MB_ICONWARNING);
			if (ret2 == IDOK)
			    exit(0);
		}
}

void StartAI()
{
	HWND chuangkou;
	chuangkou = GetHWnd();
	MAX max;
	max.x = 0;
	max.y = 0;
	if (flag % 2 == 0)
	{
		return;
	}
	AiDefend();
	flag++;
	return;
}