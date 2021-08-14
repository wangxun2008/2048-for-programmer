//ggcc图形库
//ggcc_graphics.h

/*----------------------------------------------------------------------
介绍：
	本图形库
	1）是基于控制台的伪图形库，借鉴于easyx, 可以理解为ggcc.h的升级版
	2）采用24位真彩色渲染图像，抛弃了旧版的4位色渲染，渲染效果更加
	3）删除了繁琐的、落后的、底层的旧版函数，是新一代的ggcc标准
	4）修正了曾经不规范的定义，让使用者更加容易
	5）删除了许多多余的代码，渲染速度更快
	6）适用于Dev-cpp
信息：
	版本：v1.6
	作者：王循
	开始时间：2021-7-9 17:00
-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------
工作日志：

|  日期	 | 工作量 | 总行数 | 描述
__________________________________________________________________
2021-7-9	625行	625行	完成了各种底层函数
2021-7-10 	443行	1068行	正式进入图形渲染部分，完成了点、线条、
							矩形、圆、椭圆的各种渲染方法
2021-7-11   227行	1295行	完成了文字处理、批量绘图部分
2021-7-12    76行   1371行  完成了多边形的渲染
2021-7-19   276行	1647行	完成了控制栏的绘制，完善了程序，修复
							了一系列bug
2021-7-23	 54行	1701行	完成了颜色的混合与反转
2021-8-3	-11行	1690行	修复了一系列bug
2021-8-9	 65行	1755行  完成了圆角矩形的绘制，修复了线条、多
							边形、BFS洪水填充的bug 

-----------------------------------------------------------------------*/

#include <bits/stdc++.h>
#include <stdio.h>
#include <conio.h>
#include <cstdlib>
#include <windows.h>
#include <fstream>
#include <pthread.h>
#include <direct.h>

// 颜色
#define	BLACK			0x000000
#define	BLUE			0xAA0000
#define	GREEN			0x00AA00
#define	CYAN			0xAAAA00
#define	RED				0x0000AA
#define	MAGENTA			0xAA00AA
#define	BROWN			0x0055AA
#define	LIGHTGRAY		0xAAAAAA
#define	DARKGRAY		0x555555
#define	LIGHTBLUE		0xFF5555
#define	LIGHTGREEN		0x55FF55
#define	LIGHTCYAN		0xFFFF55
#define	LIGHTRED		0x5555FF
#define	LIGHTMAGENTA	0xFF55FF
#define	YELLOW			0x55FFFF
#define	WHITE			0xFFFFFF

//VT常量
#define ENABLE_PROCESSED_INPUT              0x0001
#define ENABLE_LINE_INPUT                   0x0002
#define ENABLE_ECHO_INPUT                   0x0004
#define ENABLE_WINDOW_INPUT                 0x0008
#define ENABLE_MOUSE_INPUT                  0x0010
#define ENABLE_INSERT_MODE                  0x0020
#define ENABLE_QUICK_EDIT_MODE              0x0040
#define ENABLE_EXTENDED_FLAGS               0x0080
#define ENABLE_AUTO_POSITION                0x0100
#define ENABLE_VIRTUAL_TERMINAL_INPUT       0x0200
#define ENABLE_PROCESSED_OUTPUT             0x0001
#define ENABLE_WRAP_AT_EOL_OUTPUT           0x0002
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING  0x0004
#define DISABLE_NEWLINE_AUTO_RETURN         0x0008
#define ENABLE_LVB_GRID_WORLDWIDE           0x0010

// 填充样式
#define	BDIAGONAL_FILL		0x0001				// /// 填充
#define CROSS_FILL			0x0002				// +++ 填充
#define DIAGCROSS_FILL		0x0004				// xxx 填充
#define DOT_FILL			0x0008				// xxx 填充
#define FDIAGONAL_FILL		0x0010				// \\\ 填充
#define HORIZONTAL_FILL		0x0020				// === 填充
#define VERTICAL_FILL		0x0040				// ||| 填充

// 填充动作
#define FLOODFILLBORDER		0x0000				// 填充动作在颜色参数围成的封闭区域内填充
#define FLOODFILLSURFACE	0x0001				// 填充动作在颜色参数指定的连续颜色表面填充

// 背景填充模式
#define OPAQUE              0x0000              // 默认背景色 
#define TRANSPARENT         0x0001				// 透明 

// 未完成标识符
#define pass ;

// 定义颜色转换宏
#define BGR(color) ((((color)&0xFF)<<16)|((color)&0xFF00FF00)|(((color)&0xFF0000)>>16))

// 鼠标点击
#define click(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)

// 窗口缓冲区
#define wbs 2000								// 缓冲区边长
#define cshc 300								// 窗口不显示缓冲区边长 

using namespace std;



//----------矩形存储----------
struct RectAngle {
	POINT p1,p2;
	bool inRectangle(POINT p) {
		return (p.x>=p1.x&&p.x<=p2.x&&p.y>=p1.y&&p.y<=p2.y);
	}
};

//----------多边形存储----------
struct Region {
	vector<POINT> p;
	bool inRegion(POINT pt) {
		int nCross=0;
		for(int i=0; i<p.size(); i++) {
			POINT p1;
			POINT p2;
			p1=p[i];
			p2=p[(i+1)%p.size()];
			if(p1.y==p2.y)continue;
			if(pt.y<min(p1.y,p2.y))continue;
			if(pt.y>=max(p1.y,p2.y))continue;
			double x=(double)(pt.y-p1.y)*(double)(p2.x-p1.x)/(double)(p2.y-p1.y)+p1.x;
			if(x>pt.x)nCross++;
		}
		bool flag=nCross%2;
		if(!flag)for(int i=0; i<p.size(); i++)
				if(p[i].x==pt.x&&p[i].y==pt.y) {
					flag=true;
					break;
				}
		return flag;
	}
	int pSum() {
		return p.size();
	}
};

//----------圆的存储----------
struct Circle {
	POINT p;
	int r;
	bool inCircle(POINT pt) {
		int a=abs(pt.x-p.x);
		int b=abs(pt.y-p.y);
		return sqrt(a*a+b*b)<=r;
	}
};

//----------点的储存----------
struct Pixel {
	POINT p;
	bool inPixel(POINT pt) {
		return (pt.x==p.x&&pt.y==p.x);
	}
};

//----------画线样式----------
struct LINESTYLE {
	int style;				// 画线样式(无用，为兼容easyx)
	int thickness;			// 线的宽度
};

//----------填充样式----------
struct FILLSTYLE {
	int style;				// 填充样式
};

//----------图像储存---------
struct IMAGE {
	//这里还没有东西呢
	pass;
};

//----------临时结构体----------
struct PointXYZ {
	PointXYZ() = default;
	PointXYZ(float x_,float y_,float theta_):x(x_),y(y_),theta(theta) {}
	float x {0.0};
	float y {0.0};
	float theta {0.0};
};

//----------字体储存----------
// 设置当前字体样式
//		nHeight: 字符的平均高度；
//		nWidth: 字符的平均宽度(0 表示自适应)；
//		lpszFace: 字体名称；
//		nEscapement: 字符串的书写角度(单位 0.1 度)；
//		nOrientation: 每个字符的书写角度(单位 0.1 度)；
//		nWeight: 字符的笔画粗细(0 表示默认粗细)；
//		bItalic: 是否斜体；
//		bUnderline: 是否下划线；
//		bStrikeOut: 是否删除线；
//		fbCharSet: 指定字符集；
//		fbOutPrecision: 指定文字的输出精度；
//		fbClipPrecision: 指定文字的剪辑精度；
//		fbQuality: 指定文字的输出质量；
//		fbPitchAndFamily: 指定以常规方式描述字体的字体系列。



//----------绘制变量----------
int linecolor=RGB(0,0,0);	// 线条颜色
int fillcolor=RGB(0,0,0);	// 填充色
int bkcolor=RGB(0,0,0);		// 背景色
int textcolor=RGB(0,0,0);	// 字体色

int nowbcolor=RGB(0,0,0);	// 当前字体背景色
int nowwcolor=RGB(0,0,0);	// 当前字体前景色

bool ctrl=true;				// 是否显示控制栏
int choose_ctrl_time[10];	// 控制栏焦点选择时间

//----------系统变量----------
HANDLE hIn;
HANDLE hOut;
DWORD dwInMode, dwOutMode;
HWND hwnd = GetConsoleWindow();
RECT rc;
CONSOLE_CURSOR_INFO cciCursor;
const int WINx = GetSystemMetrics(SM_CXSCREEN);		// 屏幕分辨率
const int WINy = GetSystemMetrics(SM_CYSCREEN);		// 屏幕分辨率
int cx = GetSystemMetrics(SM_CXSCREEN);				// 窗口宽
int cy = GetSystemMetrics(SM_CYSCREEN);				// 窗口高
double textX = 8.0;									// 字体宽
double textY = 16.0;								// 字体高
int win_TOP = false;								// 是否置顶
int StartGameTime=clock();							// 开始游戏时间
int LastFPSTime=0;									// 上一次检测FPS时间
int FPS=0;											// 当前FPS
int FPSSum=0;										// 所有FPS总数
int LastFPS=0;										// 上一次检测的FPS

//----------绘图设备变量----------
RectAngle Cliprgn= {{0,0},{0,0}};					// 绘图裁剪区
LINESTYLE Linestyle;								// 当前画线样式
FILLSTYLE Fillstyle;								// 当前填充样式
int EPOTWcolor[wbs+1][wbs+1];						// 窗口上每个像素点的颜色
int EPOTWwcolor[wbs+1][wbs+1];						// 窗口上每个像素点的字体颜色
string EPOTWtext[wbs+1][wbs+1];						// 窗口上每个像素点上写的字
int lEPOTWcolor[wbs+1][wbs+1];						// 上次窗口上每个像素点的颜色
int lEPOTWwcolor[wbs+1][wbs+1];						// 上次窗口上每个像素点的字体颜色
string lEPOTWtext[wbs+1][wbs+1];					// 上次窗口上每个像素点上写的字
int bkmode;											// 背景填充模式
bool batchDraw;										// 是否开启批量绘图
POINT mouse_deviation1= {0,0};						// 鼠标偏差（加减）单位：像素
POINT mouse_deviation2= {1.0,1.0};					// 鼠标偏差（乘倍）
int orx=0;											// 坐标原点x
int ory=0;											// 坐标原点y

//----------声明函数----------

// 绘图模式相关函数

HWND initgraph(int w, int h,bool _ctrl);				// 初始化图形环境
void closegraph();										// 关闭图形环境

// 绘图环境设置
void cleardevice();					// 清屏
void setcliprgn(RectAngle hrgn);	// 设置当前绘图设备的裁剪区
void clearcliprgn();				// 清除裁剪区的屏幕内容
void getlinestyle(LINESTYLE* pstyle);						// 获取当前画线样式
void setlinestyle(const LINESTYLE* pstyle);					// 设置当前画线样式
void setlinestyle(int style, int thickness = 1);			// 设置当前画线样式
void getfillstyle(FILLSTYLE* pstyle);						// 获取当前填充样式
void setfillstyle(FILLSTYLE pstyle);						// 设置当前填充样式
int  getrop2();						// 获取前景的二元光栅操作模式
void setrop2(int mode);				// 设置前景的二元光栅操作模式
int  getpolyfillmode();				// 获取多边形填充模式
void setpolyfillmode(int mode);		// 设置多边形填充模式
void graphdefaults();				// 重置所有绘图设置为默认值
int getlinecolor();					// 获取当前线条颜色
void setlinecolor(int color);		// 设置当前线条颜色
int gettextcolor();					// 获取当前文字颜色
void settextcolor(int color);		// 设置当前文字颜色
int getfillcolor();					// 获取当前填充颜色
void setfillcolor(int color);		// 设置当前填充颜色
int getbkcolor();					// 获取当前绘图背景色
void setbkcolor(int color);			// 设置当前绘图背景色
int  getbkmode();					// 获取背景混合模式
void setbkmode(int mode);			// 设置背景混合模式

// 颜色模型转换函数
int RGBtoGRAY(int rgb);
void RGBtoHSL(int rgb, float *H, float *S, float *L);
void RGBtoHSV(int rgb, float *H, float *S, float *V);
int HSLtoRGB(float H, float S, float L);
int HSVtoRGB(float H, float S, float V);


// 绘图函数
int getpixel(int x, int y);				// 获取点的颜色
void putpixel(int x, int y, int color);	// 画点
void line(int x1, int y1, int x2, int y2);		// 画线
void rectangle	   (int left, int top, int right, int bottom);	// 画矩形
void system_rectangle(int left, int top, int right, int bottom);// 画矩形
void fillrectangle (int left, int top, int right,
                    int bottom);	// 画填充矩形(有边框)
void solidrectangle(int left, int top, int right,
                    int bottom);	// 画填充矩形(无边框)
void system_solidrectangle(int left, int top, int right,
                           int bottom);	// 画填充矩形(无边框)
void clearrectangle(int left, int top, int right, int bottom);	// 清空矩形区域
void circle		(int x, int y, int radius);		// 画圆
void fillcircle (int x, int y, int radius);		// 画填充圆(有边框)
void solidcircle(int x, int y, int radius);		// 画填充圆(无边框)
void clearcircle(int x, int y, int radius);		// 清空圆形区域
void ellipse	 (int left, int top, int right, int bottom);	// 画椭圆
void fillellipse (int left, int top, int right,
                  int bottom);	// 画填充椭圆(有边框)
void solidellipse(int left, int top, int right,
                  int bottom);	// 画填充椭圆(无边框)
void clearellipse(int left, int top, int right, int bottom);	// 清空椭圆形区域
void roundrect	   (int left, int top, int right, int bottom, int ellipsewidth,
                    int ellipseheight);		// 画圆角矩形
void fillroundrect (int left, int top, int right, int bottom, int ellipsewidth,
                    int ellipseheight);		// 画填充圆角矩形(有边框)
void solidroundrect(int left, int top, int right, int bottom, int ellipsewidth,
                    int ellipseheight);		// 画填充圆角矩形(无边框)
void clearroundrect(int left, int top, int right, int bottom, int ellipsewidth,
                    int ellipseheight);		// 清空圆角矩形区域
void arc	 (int left, int top, int right, int bottom, double stangle,
              double endangle);	// 画椭圆弧(起始角度和终止角度为弧度制)
void pie	 (int left, int top, int right, int bottom, double stangle,
              double endangle);	// 画椭圆扇形(起始角度和终止角度为弧度制)
void fillpie (int left, int top, int right, int bottom, double stangle,
              double endangle);	// 画填充椭圆扇形(有边框)
void solidpie(int left, int top, int right, int bottom, double stangle,
              double endangle);	// 画填充椭圆扇形(无边框)
void clearpie(int left, int top, int right, int bottom, double stangle,
              double endangle);	// 清空椭圆扇形区域
void polyline	 (const POINT *points, int num);								// 画多条连续的线
void polygon	 (const POINT *points, int num);								// 画多边形
void fillpolygon (const POINT *points,
                  int num);								// 画填充的多边形(有边框)
void solidpolygon(const POINT *points,
                  int num);								// 画填充的多边形(无边框)
void clearpolygon(const POINT *points, int num);								// 清空多边形区域
void polybezier(const POINT *points, int num);									// 画贝塞尔曲线
void floodfill(int x, int y, int color,
               int filltype = FLOODFILLBORDER);	// 填充区域

// 文字相关函数
void outtextxy(int x, int y, string str);	// 在指定位置输出字符串
void system_outtextxy(int x, int y, string str);	// 在指定位置输出字符串
int textwidth(string str);					// 获取字符串占用的像素宽
int textheight(string str);				// 获取字符串占用的像素高
int drawtext(string str, RECT* pRect,
             UINT uFormat);	// 在指定区域内以指定格式输出字符串
void settextstyle(int nHeight, int nWidth, string lpszFace);
void settextstyle(int nHeight, int nWidth, string lpszFace, int nEscapement,
                  int nOrientation, int nWeight, bool bItalic, bool bUnderline, bool bStrikeOut);
void settextstyle(int nHeight, int nWidth, string lpszFace, int nEscapement,
                  int nOrientation, int nWeight, bool bItalic, bool bUnderline, bool bStrikeOut,
                  BYTE fbCharSet, BYTE fbOutPrecision, BYTE fbClipPrecision, BYTE fbQuality,
                  BYTE fbPitchAndFamily);
void settextstyle(const LOGFONT *font);	// 设置当前字体样式
void gettextstyle(LOGFONT *font);		// 获取当前字体样式

// 图像处理函数
void loadimage(IMAGE *pDstImg, string pImgFile, int nWidth = 0,
               int nHeight = 0, bool bResize =
                   false);					// 从图片文件获取图像(bmp/gif/jpg/png/tif/emf/wmf/ico)
void loadimage(IMAGE *pDstImg, string pResType, string pResName,
               int nWidth = 0, int nHeight = 0,
               bool bResize = false);	// 从资源文件获取图像(bmp/gif/jpg/png/tif/emf/wmf/ico)
void saveimage(string pImgFile,
               IMAGE* pImg = NULL);								// 保存图像(bmp/gif/jpg/png/tif)
void getimage(IMAGE *pDstImg, int srcX, int srcY, int srcWidth,
              int srcHeight);												// 从当前绘图设备获取图像
void putimage(int dstX, int dstY, const IMAGE *pSrcImg,
              DWORD dwRop = SRCCOPY);												// 绘制图像到屏幕
void putimage(int dstX, int dstY, int dstWidth, int dstHeight,
              const IMAGE *pSrcImg, int srcX, int srcY,
              DWORD dwRop = SRCCOPY);		// 绘制图像到屏幕(指定宽高)
void rotateimage(IMAGE *dstimg, IMAGE *srcimg, double radian,
                 int bkcolor = BLACK, bool autosize = false, bool highquality = true);// 旋转图像
void Resize(IMAGE* pImg, int width, int height);	// 调整绘图设备的大小
DWORD* GetImageBuffer(IMAGE* pImg = NULL);			// 获取绘图设备的显存指针
IMAGE* GetWorkingImage();							// 获取当前绘图设备
void SetWorkingImage(IMAGE* pImg = NULL);			// 设置当前绘图设备
HDC GetImageHDC(IMAGE* pImg = NULL);				// 获取绘图设备句柄(HDC)

// 其它函数
int	getwidth();			// 获取绘图区宽度
int	getheight();		// 获取绘图区高度
void BeginBatchDraw();	// 开始批量绘制
void FlushBatchDraw();	// 执行未完成的绘制任务
void FlushBatchDraw(int left, int top, int right,
                    int bottom);	// 执行指定区域内未完成的绘制任务
void EndBatchDraw();	// 结束批量绘制，并执行未完成的绘制任务
void EndBatchDraw(int left, int top, int right,
                  int bottom);	// 结束批量绘制，并执行指定区域内未完成的绘制任务
HWND GetHWnd();								// 获取绘图窗口句柄(HWND)
string GetGGCCVer();						// 获取 GGCC 当前版本
void settextsize(double sizex,double sizey);// 设置字体大小

//----------函数-----------
string change_its(int a) {
	/*这个函数用于转换int到string*/
	int b=abs(a);
	string ans;
	if(b==0)return "0";
	while(b) {
		ans=char(b%10+'0')+ans;
		b/=10;
	}
	if(a<0)ans='-'+ans;
	return ans;
}

POINT GetMousePos() {
	/*这个函数用于获取鼠标位置*/
	POINT pt1,pt2,pt;
	CONSOLE_FONT_INFO cfi;
	GetCursorPos(&pt1);
	ScreenToClient(hwnd,&pt1);
	GetCurrentConsoleFont(hOut, FALSE, &cfi);
	pt2= {cfi.dwFontSize.X*2+mouse_deviation1.x,
	      cfi.dwFontSize.Y+mouse_deviation1.y
	     };
	pt= {1.0*pt1.x/pt2.x,1.0*pt1.y/pt2.y};
	pt.x*=mouse_deviation2.x;
	pt.y*=mouse_deviation2.y;
	return pt;
}

void modeset(int a1,int a2) {
	/*这个函数用于调整窗口大小 */
	string a=change_its(a1),b=change_its(a2);
	system(("mode "+a+","+b).c_str());
}

bool inSight(int x,int y) {
	/*这个函数用于判断坐标(x,y)是否在窗口可显示的范围内 */
	return (x>=0&&y>=0&&x<cx&&y<cy);
}

bool checkPoint(int x,int y) {
	/*这个函数用于判断坐标(x,y)是否可显示*/
	return inSight(x,y)&&!batchDraw;
}

HWND initgraph(int w, int h, bool _ctrl=false) {
	/*这个函数用于初始化绘图窗口。
	width
	绘图窗口的宽度。
	height
	绘图窗口的高度。*/
	cout<<"初始化控制台图形化界面中......"<<endl<<endl;
	cout<<"图形化引擎版本信息:"<<endl;
	cout<<GetGGCCVer()<<endl<<endl;
	cx=w,cy=h;
	hIn = GetStdHandle(STD_INPUT_HANDLE);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	cout<<"[Finished]初始化窗口输入输出句柄"<<endl;
	GetConsoleMode(hIn, &dwInMode);
	GetConsoleMode(hOut, &dwOutMode);
	dwInMode |= ENABLE_VIRTUAL_TERMINAL_INPUT;
	dwOutMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hIn, dwInMode);
	SetConsoleMode(hOut, dwOutMode);
	cout<<"[Finished]初始化ANSI控制码"<<endl;

	GetWindowRect(hwnd, &rc);
	SetWindowLongPtr(hwnd,
	                 GWL_STYLE, GetWindowLong(hwnd,
	                         GWL_STYLE) & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX);
	SetWindowPos(hwnd,
	             NULL,
	             rc.left,
	             rc.top,
	             rc.right - rc.left, rc.bottom - rc.top,
	             NULL);
	ctrl=_ctrl;
	settextsize(textX,textY);
	if(ctrl)modeset(w*2,h+1);
	else modeset(w*2,h);
	cout<<"初始化控制台图形化界面中......"<<endl<<endl;
	cout<<"图形化引擎版本信息:"<<endl;
	cout<<GetGGCCVer()<<endl<<endl;
	cout<<"[Finished]初始化窗口输入输出句柄"<<endl;
	cout<<"[Finished]初始化ANSI控制码"<<endl;
	cout<<"[Finished]重置窗口大小" <<endl;
	if(GetConsoleCursorInfo(hOut, &cciCursor)) {
		cciCursor.bVisible = FALSE;
		SetConsoleCursorInfo(hOut, &cciCursor);
	}
	dwInMode &= ~ENABLE_QUICK_EDIT_MODE;
	dwInMode &= ~ENABLE_MOUSE_INPUT;
	SetConsoleMode(hIn, dwInMode);
	for(int i=0; i<=wbs; i++)
		for(int j=0; j<=wbs; j++) {
			EPOTWcolor[i][j]=bkcolor;
			EPOTWwcolor[i][j]=bkcolor;
			EPOTWtext[i][j]="  ";
		}
	cout<<"[Finished]初始化图形渲染界面"<<endl;
	system("cls");
	return hwnd;
}

void setorigin(int x,int y) {
	orx=x;
	ory=y;
}

void draw_ctrl(int Ctrl=0) {
	/*这个函数用于绘制控制栏*/
	if(ctrl) {
		int fc=fillcolor;
		int bkm=bkmode;
		int bkc=bkcolor;
		int tc=textcolor;
		POINT mp=GetMousePos();
		bkmode=0;
		fillcolor=RGB(197,186,180);
		bkcolor=RGB(96,169,230);
		textcolor=RGB(50,50,50);
		system_solidrectangle(0,cy,cx-1,cy);
		system_outtextxy(0,cy,"  ·GGCC·  ");
		bkmode=1;
		fillcolor=fc;
		bkmode=bkm;
		bkcolor=bkc;
		textcolor=tc;
	}
}

void update_ctrl(int Ctrl=0) {
	/*这个函数用于更新控制栏*/
	FPS++;
	FPSSum++;
	if(clock()-LastFPSTime>=1000) {
		LastFPS=FPS;
		FPS=0;
		LastFPSTime=clock();
	}
	if(ctrl) {
		int fc=fillcolor;
		int bkm=bkmode;
		int bkc=bkcolor;
		int tc=textcolor;
		int GameTime=(clock()-StartGameTime)/1000;
		POINT mp=GetMousePos();
		bkmode=0;
		for(int i=0; i<=9; i++)
			if(choose_ctrl_time[i]>1)
				choose_ctrl_time[i]-=2;
		if(mp.x>=cx-4&&mp.x<=cx&&mp.y==cy+1&&hwnd==GetConsoleWindow()) {
			if(choose_ctrl_time[mp.x-(cx-4)]<23)
				choose_ctrl_time[mp.x-(cx-4)]+=4;
			if(mp.x-(cx-4)==0) {
				if(click(VK_LBUTTON)) {
					if(textX)textX++,textY+=2;
					settextsize(textX,textY);
				}
			} else if(mp.x-(cx-4)==1) {
				if(click(VK_LBUTTON)) {
					if(textX)textX--,textY-=2;
					settextsize(textX,textY);
				}
			} else if(mp.x-(cx-4)==2) {
				if(click(VK_LBUTTON)) {
					win_TOP=!win_TOP;
					if(win_TOP)
						SetWindowPos(hwnd,HWND_TOPMOST, 0, 0, 0, 0,
						             SWP_NOMOVE | SWP_DRAWFRAME | SWP_NOSIZE);
					else
						SetWindowPos(hwnd,HWND_NOTOPMOST, 0, 0, 0, 0,
						             SWP_NOMOVE | SWP_DRAWFRAME | SWP_NOSIZE);
				}
			} else if(mp.x-(cx-4)==3) {
				if(click(VK_LBUTTON))ShowWindow(hwnd,SW_MINIMIZE);
			}
		}
		fillcolor=RGB(197,186,180);
		bkcolor=RGB(255,100,100);
		textcolor=RGB(50,50,50);
		string Button[4]= {"﹢","﹣","↑","↓"};
		bkmode=0;
		for(int i=0; i<=3; i++) {
			int cct=choose_ctrl_time[i];
			bkcolor=RGB(197-cct*3,186-cct*3,180-cct*3);
			system_outtextxy(cx-4+i,cy,Button[i]);
		}
		bkmode=1;
		bkcolor=RGB(255,100,100);
		textcolor=RGB(50,50,50);
		if(cx>10)
			system_outtextxy(6,cy,"Pos:("+change_its(mp.x)+","+change_its(mp.y)+")    ");
		if(cx>30)
			system_outtextxy(14,cy,"FPS:"+change_its(LastFPS)+"/"+change_its(FPSSum/GameTime)+"    ");
		if(cx>50)
			if(GameTime<60)system_outtextxy(21,cy,"GameTime:"+change_its(GameTime)+" sec    ");
			else if(GameTime<3600)system_outtextxy(21,cy,"GameTime:"+change_its(GameTime/60)+" min    ");
			else if(GameTime<86400)system_outtextxy(21,cy,"GameTime:"+change_its(GameTime/3600)+" hour    ");
			else system_outtextxy(21,cy,"GameTime:"+change_its(GameTime/86400)+" day    ");
		time_t t = time(NULL);
		struct tm* stime=localtime(&t);
		if(cx>20) {
			string Time="Time:"+
			            (change_its(1900+stime->tm_year)+'-'+
			             change_its(1+stime->tm_mon)+'-'+
			             change_its(stime->tm_mday)+' '+
			             change_its(stime->tm_hour)+':'+
			             change_its(stime->tm_min)+':'+
			             change_its(stime->tm_sec))+"    ";
			system_outtextxy(cx-18,cy,Time);
		}
		fillcolor=fc;
		bkmode=bkm;
		bkcolor=bkc;
		textcolor=tc;
	}
}

void mousetext(string text) {
	/*这个函数用于在鼠标旁显示提示字幕*/
	POINT mp=GetMousePos();
	SetConsoleCursorPosition(hOut, {mp.x<<1,mp.y});
}

void closegraph() {
	/*这个函数用于关闭绘图窗口。*/
	exit(0);
}

void cleardevice() {
	/*这个函数用于清空绘图设备。
	具体的，是用当前背景色清空绘图设备
	并将当前点移至 (0, 0)。*/
	if(nowbcolor^bkcolor&&!batchDraw) {
		printf("\033[48;2;%d;%d;%dm",bkcolor%256,(bkcolor>>8)%256,bkcolor>>16);
		nowbcolor=bkcolor;
	}
	if(!batchDraw)system("cls");
	for(int i=0; i<=wbs; i++)
		for(int j=0; j<=wbs; j++) {
			EPOTWcolor[i][j]=bkcolor;
			EPOTWwcolor[i][j]=bkcolor;
			EPOTWtext[i][j]="  ";
		}
}

void setcliprgn(RectAngle hrgn) {
	/*这个函数用于设置当前绘图设备的裁剪区。*/
	if(hrgn.p1.x>hrgn.p2.x)hrgn.p1.x^=hrgn.p2.x,hrgn.p2.x^=hrgn.p1.x,
		                                  hrgn.p1.x^=hrgn.p2.x;
	if(hrgn.p1.y>hrgn.p2.y)hrgn.p1.y^=hrgn.p2.y,hrgn.p2.y^=hrgn.p1.y,
		                                  hrgn.p1.y^=hrgn.p2.y;
	Cliprgn=hrgn;
}

void clearcliprgn() {
	/*这个函数用于清空裁剪区*/
	COORD pos;
	if(nowbcolor^bkcolor&&!batchDraw) {
		printf("\033[48;2;%d;%d;%dm",bkcolor%256,(bkcolor>>8)%256,bkcolor>>16);
		nowbcolor=bkcolor;
	}
	for(int i=Cliprgn.p1.y; i<=Cliprgn.p2.y; i++) {
		SetConsoleCursorPosition(hOut, {Cliprgn.p1.x<<1,i});
		for(int j=Cliprgn.p1.x; j<=Cliprgn.p2.x; j++) {
			if(checkPoint(j,i))printf("  ");
			EPOTWcolor[j+cshc][i+cshc]=bkcolor;
			EPOTWwcolor[j+cshc][i+cshc]=bkcolor;
			EPOTWtext[j+cshc][i+cshc]="  ";
		}
	}
}

void getlinestyle(LINESTYLE &pstyle) {
	/*这个函数用于获取当前设备画线样式。*/
	pstyle=Linestyle;
}

void setlinestyle(LINESTYLE pstyle) {
	/*这个函数用于设置当前设备画线样式。*/
	Linestyle=pstyle;
}

void setlinestyle(int style, int thickness) {
	/*[重载]这个函数用于设置当前设备画线样式。*/
	Linestyle.style=style;
	Linestyle.thickness=thickness;
}

void getfillstyle(FILLSTYLE &pstyle) {
	/*这个函数用于获取当前设备填充样式。*/
	pstyle=Fillstyle;
}

void setfillstyle(FILLSTYLE pstyle) {
	/*这个函数用于设置当前设备填充样式。*/
	Fillstyle=pstyle;
}

int getrop2() {
	/*这个函数用于获取当前设备二元光栅操作模式。*/
	pass;
}
void setrop2(int mode) {
	/*这个函数用于设置当前设备二元光栅操作模式。*/
	pass;
}

int getpolyfillmode() {
	/*这个函数用于获取当前设备多边形填充模式。*/
	pass;
}

void setpolyfillmode(int mode) {
	/*这个函数用于设置当前设备多边形填充模式。*/
	pass;
}

void graphdefaults() {
	/*这个函数用于重置视图、当前点、绘图色、背景色、线形、填充样式、字体为默认值。*/
	int linecolor=RGB(0,0,0);	//线条颜色
	int fillcolor=RGB(0,0,0);	//填充色
	int bkcolor=RGB(0,0,0);		//背景色
	int textcolor=RGB(0,0,0);	//字体色
	const int WINx = GetSystemMetrics(SM_CXSCREEN);		//屏幕分辨率
	const int WINy = GetSystemMetrics(SM_CYSCREEN);		//屏幕分辨率
	RectAngle Cliprgn= {{0,0},{0,0}};					//绘图裁剪区
	LINESTYLE Linestyle= {0,0};							//当前画线样式
	FILLSTYLE Fillstyle= {0};							//当前填充样式
}

int getlinecolor() {
	/*这个函数用于获取当前设备画线颜色。*/
	return linecolor;
}

void setlinecolor(int color) {
	/*这个函数用于设置当前设备画线颜色。*/
	linecolor=color;
}

int gettextcolor() {
	/*这个函数用于获取当前文字颜色。*/
	return textcolor;
}

void settextcolor(int color) {
	/*这个函数用于设置当前文字颜色。*/
	textcolor=color;
}

int getfillcolor() {
	/*这个函数用于获取当前设备填充颜色。*/
	return fillcolor;
}

void setfillcolor(int color) {
	/*这个函数用于设置当前设备填充颜色。*/
	fillcolor=color;
}

int getbkcolor() {
	/*这个函数用于获取当前设备背景色。*/
	return bkcolor;
}

void setbkcolor(int color) {
	/*这个函数用于设置当前设备绘图背景色。*/
	bkcolor=color;
}

int  getbkmode() {
	/*这个函数用于获取当前设备图案填充和文字输出时的背景模式。*/
	return bkmode;
}

void setbkmode(int mode) {
	/*这个函数用于设置当前设备图案填充和文字输出时的背景模式。*/
	bkmode=mode;
}

void setcolor(int color) {
	/*这个函数用于设置当前绘图前景色。*/
	linecolor=color;
	textcolor=color;
}

int RGBtoGRAY(int rgb) {
	/*该函数用于返回与指定颜色对应的灰度值颜色。*/
	int B=rgb>>16,G=(rgb>>8)/256,R=rgb%256;
	double y=0.212671*R+0.715160*G+(double)0.072169*B;
	if(y>0.008856)return 116.0*pow(y,1.0/3)-16.0;
	else return 903.3*y;
}

void RGBtoHSL(int rgb, float &H, float &S, float &L) {
	/*该函数用于转换 RGB 颜色为 HSL 颜色。*/
	float B=rgb>>16,G=(rgb>>8)/256,R=rgb%256;
	float minn, maxn, delta,tmp;
	tmp=min(R,G);
	minn=min(tmp,B);
	tmp=max(R,G);
	maxn=max(tmp,B);
	L=maxn;
	delta=maxn-minn;
	if(maxn!=0)S=delta/maxn;
	else {
		S=0,H=0;
		return;
	}
	if(R==maxn)H=(G-B)/delta;
	else if(G==maxn)
		H=2+(B-R)/delta;
	else H=4+(R-G)/delta;
	H*=60;
	if(H<0)H+=360;
}

void RGBtoHSV(int rgb, float &H, float &S, float &V) {
	/*该函数用于转换 RGB 颜色为 HSV 颜色。*/
	float B=rgb>>16,G=(rgb>>8)/256,R=rgb%256;
	float minn, maxn, delta,tmp;
	tmp=min(R,G);
	minn=min(tmp,B);
	tmp=max(R,G);
	maxn=max(tmp,B);
	V=maxn;
	delta=maxn-minn;
	if(maxn!=0)S=delta/maxn;
	else {
		S=0,H=0;
		return;
	}
	if(R==maxn)H=(G-B)/delta;
	else if(G==maxn)
		H=2+(B-R)/delta;
	else H=4+(R-G)/delta;
	H*=60;
	if(H<0)H+=360;
}

int HSLtoRGB(float H, float S, float L) {
	/*该函数用于转换 HSL 颜色为 RGB 颜色。*/
	pass;
}

int HSVtoRGB(float H, float S, float V) {
	/*该函数用于转换 RGB 颜色为 HSV 颜色。*/
	pass;
}

int getpixel(int x, int y) {
	/*这个函数用于获取点的颜色。*/
	return EPOTWcolor[x+cshc][y+cshc];
}

void putpixel(int x, int y, int color) {
	/*这个函数用于画点。*/
	EPOTWcolor[x+cshc][y+cshc]=color;
	EPOTWtext[x+cshc][y+cshc]="  ";
	SetConsoleCursorPosition(hOut, {x<<1,y});
	if(nowbcolor^color&&!batchDraw) {
		int B=color>>16,G=(color>>8)%256,R=color%256;
		if(checkPoint(x,y))printf("\033[48;2;%d;%d;%dm  ",R,G,B);
		nowbcolor=color;
	} else if(checkPoint(x,y))printf("  ");
}

void line(int x1, int y1, int x2, int y2) {
	/*这个函数用于画直线。*/
	SetConsoleCursorPosition(hOut, {x1<<1,y1});
	if(nowbcolor^linecolor&&!batchDraw) {
		int B=linecolor>>16,G=(linecolor>>8)%256,R=linecolor%256;
		if(checkPoint(x1,y1))printf("\033[48;2;%d;%d;%dm  ",R,G,B);
		EPOTWcolor[x1+cshc][y1+cshc]=linecolor;
		EPOTWtext[x1+cshc][y1+cshc]="  ";
		nowbcolor=linecolor;
	} else if(checkPoint(x1,y1)) {
		printf("  ");
		EPOTWcolor[x1+cshc][y1+cshc]=linecolor;
		EPOTWtext[x1+cshc][y1+cshc]="  ";
	}
	x1*=100,y1*=100,x2*=100,y2*=100;
	while(x1/100!=x2/100||y1/100!=y2/100) {
		int n=round(max(1.0*abs(x2-x1),1.0*abs(y2-y1))/100);
		x1+=round(1.0*(x2-x1)/n);
		y1+=round(1.0*(y2-y1)/n);
		SetConsoleCursorPosition(hOut, {int(x1/100.0)<<1,y1/100.0});
		if(checkPoint(int(x1/100.0),int(y1/100.0)))printf("  ");
		EPOTWcolor[int(x1/100.0)+cshc][int(y1/100.0)+cshc]=linecolor;
		EPOTWtext[int(x1/100.0)+cshc][int(y1/100.0)+cshc]="  ";
	}
}

void rectangle(int left, int top, int right, int bottom) {
	/*这个函数用于画无填充的矩形。*/
	if(left>right)left^=right,right^=left,left^=right;
	if(top>bottom)top^=bottom,bottom^=top,top^=bottom;
	if(nowbcolor^linecolor&&!batchDraw) {
		int B=linecolor>>16,G=(linecolor>>8)%256,R=linecolor%256;
		printf("\033[48;2;%d;%d;%dm",R,G,B);
		nowbcolor=linecolor;
	}
	SetConsoleCursorPosition(hOut, {left<<1,top});
	for(int i=left; i<=right; i++) {
		if(checkPoint(i,top))printf("  ");
		EPOTWcolor[i+cshc][top+cshc]=linecolor;
		EPOTWtext[i+cshc][top+cshc]="  ";
	}
	SetConsoleCursorPosition(hOut, {left<<1,bottom});
	for(int i=left; i<=right; i++) {
		if(checkPoint(i,bottom))printf("  ");
		EPOTWcolor[i+cshc][bottom+cshc]=linecolor;
		EPOTWtext[i+cshc][bottom+cshc]="  ";
	}
	for(int i=top; i<=bottom; i++) {
		SetConsoleCursorPosition(hOut, {left<<1,i});
		if(checkPoint(left,i))printf("  ");
		EPOTWcolor[left+cshc][i+cshc]=linecolor;
		EPOTWtext[left+cshc][i+cshc]="  ";
	}
	for(int i=top; i<=bottom; i++) {
		SetConsoleCursorPosition(hOut, {right<<1,i});
		if(checkPoint(right,i))printf("  ");
		EPOTWcolor[right+cshc][i+cshc]=linecolor;
		EPOTWtext[right+cshc][i+cshc]="  ";
	}
}

void system_rectangle(int left, int top, int right, int bottom) {
	/*这个函数用于画无填充的矩形。（不受边界限制）*/
	if(left>right)left^=right,right^=left,left^=right;
	if(top>bottom)top^=bottom,bottom^=top,top^=bottom;
	if(nowbcolor^linecolor&&!batchDraw) {
		int B=linecolor>>16,G=(linecolor>>8)%256,R=linecolor%256;
		printf("\033[48;2;%d;%d;%dm",R,G,B);
		nowbcolor=linecolor;
	}
	SetConsoleCursorPosition(hOut, {left<<1,top});
	for(int i=left; i<=right; i++) {
		printf("  ");
		EPOTWcolor[i+cshc][top+cshc]=linecolor;
		EPOTWtext[i+cshc][top+cshc]="  ";
	}
	SetConsoleCursorPosition(hOut, {left<<1,bottom});
	for(int i=left; i<=right; i++) {
		printf("  ");
		EPOTWcolor[i+cshc][bottom+cshc]=linecolor;
		EPOTWtext[i+cshc][bottom+cshc]="  ";
	}
	for(int i=top; i<=bottom; i++) {
		SetConsoleCursorPosition(hOut, {left<<1,i});
		printf("  ");
		EPOTWcolor[left+cshc][i+cshc]=linecolor;
		EPOTWtext[left+cshc][i+cshc]="  ";
	}
	for(int i=top; i<=bottom; i++) {
		SetConsoleCursorPosition(hOut, {right<<1,i});
		printf("  ");
		EPOTWcolor[right+cshc][i+cshc]=linecolor;
		EPOTWtext[right+cshc][i+cshc]="  ";
	}
}

void fillrectangle (int left, int top, int right,int bottom) {
	/*这个函数用于画有边框的填充矩形。*/
	if(fillcolor==linecolor) {
		solidrectangle(left,top,right,bottom);
		return;
	}
	if(left>right)left^=right,right^=left,left^=right;
	if(top>bottom)top^=bottom,bottom^=top,top^=bottom;
	rectangle(left,top,right,bottom);
	if(nowbcolor^fillcolor&&!batchDraw) {
		int B=fillcolor>>16,G=(fillcolor>>8)%256,R=fillcolor%256;
		printf("\033[48;2;%d;%d;%dm",R,G,B);
		nowbcolor=fillcolor;
	}
	for(int i=top+1; i<bottom; i++) {
		SetConsoleCursorPosition(hOut, {(left<<1)+2,i});
		for(int j=left+1; j<right; j++) {
			if(checkPoint(j,i))printf("  ");
			EPOTWcolor[j+cshc][i+cshc]=fillcolor;
			EPOTWtext[j+cshc][i+cshc]="  ";
		}
	}
}

void solidrectangle(int left, int top, int right,int bottom) {
	/*这个函数用于画无边框的填充矩形。*/
	if(left>right)left^=right,right^=left,left^=right;
	if(top>bottom)top^=bottom,bottom^=top,top^=bottom;
	if(nowbcolor^fillcolor&&!batchDraw) {
		int B=fillcolor>>16,G=(fillcolor>>8)%256,R=fillcolor%256;
		printf("\033[48;2;%d;%d;%dm",R,G,B);
		nowbcolor=fillcolor;
	}
	for(int i=top; i<=bottom; i++) {
		SetConsoleCursorPosition(hOut, {left<<1,i});
		for(int j=left; j<=right; j++) {
			if(checkPoint(j,i))printf("  ");
			EPOTWcolor[j+cshc][i+cshc]=fillcolor;
			EPOTWtext[j+cshc][i+cshc]="  ";
		}
	}
}

void system_solidrectangle(int left, int top, int right,int bottom) {
	/*这个函数用于画无边框的填充矩形(不受边界限制)。*/
	if(left>right)left^=right,right^=left,left^=right;
	if(top>bottom)top^=bottom,bottom^=top,top^=bottom;
	if(nowbcolor^fillcolor&&!batchDraw) {
		int B=fillcolor>>16,G=(fillcolor>>8)%256,R=fillcolor%256;
		printf("\033[48;2;%d;%d;%dm",R,G,B);
		nowbcolor=fillcolor;
	}
	for(int i=top; i<=bottom; i++) {
		SetConsoleCursorPosition(hOut, {left<<1,i});
		for(int j=left; j<=right; j++) {
			printf("  ");
			EPOTWcolor[j+cshc][i+cshc]=fillcolor;
			EPOTWtext[j+cshc][i+cshc]="  ";
		}
	}
}

void clearrectangle(int left, int top, int right, int bottom) {
	/*这个函数用于清空矩形区域。*/
	if(left>right)left^=right,right^=left,left^=right;
	if(top>bottom)top^=bottom,bottom^=top,top^=bottom;
	if(nowbcolor^bkcolor&&!batchDraw) {
		int B=bkcolor>>16,G=(bkcolor>>8)%256,R=bkcolor%256;
		printf("\033[48;2;%d;%d;%dm",R,G,B);
		nowbcolor=bkcolor;
	}
	for(int i=top; i<=bottom; i++) {
		SetConsoleCursorPosition(hOut, {left<<1,i});
		for(int j=left; j<=right; j++) {
			if(checkPoint(j,i))printf("  ");
			EPOTWcolor[j+cshc][i+cshc]=bkcolor;
			EPOTWtext[j+cshc][i+cshc]="  ";
		}
	}
}

void circle		(int x, int y, int radius) {
	/*这个函数用于画无填充的圆。 */
	if(nowbcolor^linecolor&&!batchDraw) {
		int B=linecolor>>16,G=(linecolor>>8)%256,R=linecolor%256;
		printf("\033[48;2;%d;%d;%dm",R,G,B);
		nowbcolor=linecolor;
	}
	int sx=x-radius;
	int sy=y-radius;
	for(int i=1; i<=(radius<<1)+1; i++)
		for(int j=1; j<=(radius<<1)+1; j++) {
			int a=abs(i-(radius+1));
			int b=abs(j-(radius+1));
			double c=sqrt(a*a+b*b);
			if(round(c)==radius) {
				int px=i+x-radius-1,py=j+y-radius-1;
				SetConsoleCursorPosition(hOut, {px<<1,py});
				if(checkPoint(px,py))printf("  ");
				EPOTWcolor[px+cshc][py+cshc]=linecolor;
				EPOTWtext[px+cshc][px+cshc]="  ";
			}
		}
}

void fillcircle (int x, int y, int radius) {
	/*这个函数用于画有边框的填充圆。*/
	circle(x,y,radius);
	radius--;
	if(nowbcolor^fillcolor&&!batchDraw) {
		int B=fillcolor>>16,G=(fillcolor>>8)%256,R=fillcolor%256;
		printf("\033[48;2;%d;%d;%dm",R,G,B);
		nowbcolor=fillcolor;
	}
	int sx=x-radius;
	int sy=y-radius;
	for(int i=1; i<=(radius<<1)+1; i++)
		for(int j=1; j<=(radius<<1)+1; j++) {
			int a=abs(i-(radius+1));
			int b=abs(j-(radius+1));
			double c=sqrt(a*a+b*b);
			if(round(c)<=radius) {
				int px=i+x-radius-1,py=j+y-radius-1;
				SetConsoleCursorPosition(hOut, {px<<1,py});
				if(checkPoint(px,py))printf("  ");
				EPOTWcolor[px+cshc][py+cshc]=fillcolor;
				EPOTWtext[px+cshc][px+cshc]="  ";
			}
		}
}

void solidcircle(int x, int y, int radius) {
	/*这个函数用于画无边框的填充圆*/
	if(nowbcolor^fillcolor&&!batchDraw) {
		int B=fillcolor>>16,G=(fillcolor>>8)%256,R=fillcolor%256;
		printf("\033[48;2;%d;%d;%dm",R,G,B);
		nowbcolor=fillcolor;
	}
	int sx=x-radius;
	int sy=y-radius;
	for(int i=1; i<=(radius<<1)+1; i++)
		for(int j=1; j<=(radius<<1)+1; j++) {
			int a=abs(i-(radius+1));
			int b=abs(j-(radius+1));
			double c=sqrt(a*a+b*b);
			if(round(c)<=radius) {
				int px=i+x-radius-1,py=j+y-radius-1;
				SetConsoleCursorPosition(hOut, {px<<1,py});
				if(checkPoint(px,py))printf("  ");
				EPOTWcolor[px+cshc][py+cshc]=fillcolor;
				EPOTWtext[px+cshc][px+cshc]="  ";
			}
		}
}

void clearcircle(int x, int y, int radius) {
	/*这个函数用于清空圆形区域。*/
	if(nowbcolor^bkcolor&&!batchDraw) {
		int B=bkcolor>>16,G=(bkcolor>>8)%256,R=bkcolor%256;
		printf("\033[48;2;%d;%d;%dm",R,G,B);
		nowbcolor=bkcolor;
	}
	int sx=x-radius;
	int sy=y-radius;
	for(int i=1; i<=(radius<<1)+1; i++)
		for(int j=1; j<=(radius<<1)+1; j++) {
			int a=abs(i-(radius+1));
			int b=abs(j-(radius+1));
			double c=sqrt(a*a+b*b);
			if(round(c)<=radius) {
				int px=i+x-radius-1,py=j+y-radius-1;
				SetConsoleCursorPosition(hOut, {px<<1,py});
				if(checkPoint(px,py))printf("  ");
				EPOTWcolor[px+cshc][py+cshc]=bkcolor;
				EPOTWtext[px+cshc][px+cshc]="  ";
			}
		}
}

void ellipse	 (int left, int top, int right, int bottom) {
	/*这个函数用于画无填充的椭圆。*/
	if(left>right)left^=right,right^=left,left^=right;
	if(top>bottom)top^=bottom,bottom^=top,top^=bottom;
	int x0=(left+right)>>1,y0=(top+bottom)>>1;
	int a=(right-left)>>1,b=(bottom-top)>>1;
	int x=0,y=b;
	putpixel(x0+x,y0+y,linecolor);
	putpixel(x0-x,y0+y,linecolor);
	putpixel(x0+x,y0-y,linecolor);
	putpixel(x0-x,y0-y,linecolor);
	//上部分
	double d1=b*b+a*a*(-b+0.25);
	while(b*b*x<a*a*y) {
		if(d1<0) {
			d1+=b*b*(2*x+3);
			x++;
		} else {
			d1+=(b*b*(2*x+3)+a*a*(-2*y+2));
			x++,y--;
		}
		putpixel(x0+x,y0+y,linecolor);
		putpixel(x0-x,y0+y,linecolor);
		putpixel(x0+x,y0-y,linecolor);
		putpixel(x0-x,y0-y,linecolor);
	}
	//下部分
	double d2=sqrt((double)b*(x+0.5))+sqrt((double)a*(y-1))-sqrt((double)a*b);
	while(y>0) {
		if(d2<0) {
			d2+=b*b*(2*x+2)+a*a*(-2*y+3);
			x++,y--;
		} else {
			d2+=a*a*(-2*y+3);
			y--;
		}
		putpixel(x0+x,y0+y,linecolor);
		putpixel(x0-x,y0+y,linecolor);
		putpixel(x0+x,y0-y,linecolor);
		putpixel(x0-x,y0-y,linecolor);
	}
}

void fillellipse (int left, int top, int right, int bottom) {
	/*这个函数用于画有边框的填充椭圆。*/
	ellipse(left,top,right,bottom);
	int x=(left+right)>>1,y=(top+bottom)>>1;
	floodfill(x,y,linecolor,FLOODFILLBORDER);
}

void solidellipse(int left, int top, int right, int bottom) {
	/*这个函数用于画无边框的填充椭圆。*/
	int lc=linecolor;
	linecolor=fillcolor;
	ellipse(left,top,right,bottom);
	int x=(left+right)>>1,y=(top+bottom)>>1;
	floodfill(x,y,linecolor,FLOODFILLBORDER);
	linecolor=lc;
}

void clearellipse(int left, int top, int right, int bottom) {
	/*这个函数用于清空椭圆区域。*/
	int lc=linecolor,fc=fillcolor;
	linecolor=bkcolor;
	fillcolor=bkcolor;
	ellipse(left,top,right,bottom);
	int x=(left+right)>>1,y=(top+bottom)>>1;
	floodfill(x,y,linecolor,FLOODFILLBORDER);
	linecolor=lc;
	fillcolor=fc;
}

void roundrect     (int left, int top, int right, int bottom, int ellipsewidth, int ellipseheight) {
	/*这个函数用于画圆角矩形。*/
	int chang=(ellipsewidth+ellipseheight)/2;
	if(left>right)left^=right,right^=left,left^=right;
	if(top>bottom)top^=bottom,bottom^=top,top^=bottom;
	rectangle(left+chang,top,right-chang,top);
	rectangle(left+chang,bottom,right-chang,bottom);
	rectangle(left,top+chang,left,bottom-chang);
	rectangle(right,top+chang,right,bottom-chang);
	for(int i=1; i<=chang; i++) {
		for(int j=1; j<=chang; j++) {
			double c1;
			c1=sqrt(i*i+j*j);
			if(round(c1)==chang) {
				putpixel(right-chang+i,bottom-chang+j,linecolor);
				putpixel(left+chang-i,bottom-chang+j,linecolor);
				putpixel(right-chang+i,top+chang-j,linecolor);
				putpixel(left+chang-i,top+chang-j,linecolor);
			}
		}
	}
}

void fillroundrect (int left, int top, int right, int bottom, int ellipsewidth, int ellipseheight) {
	/*这个函数用于画有边框的填充圆角矩形。*/
	int chang=(ellipsewidth+ellipseheight)/2;
	if(left>right)left^=right,right^=left,left^=right;
	if(top>bottom)top^=bottom,bottom^=top,top^=bottom;
	fillrectangle(left+chang,top,right-chang,bottom);
	fillrectangle(left,top+chang,left+chang,bottom-chang);
	fillrectangle(right-chang,top+chang,right,bottom-chang);
	for(int i=1; i<=chang; i++) {
		for(int j=1; j<=chang; j++) {
			double c1;
			c1=sqrt(i*i+j*j);
			if(round(c1)<=chang) {
				putpixel(right-chang+i,bottom-chang+j,linecolor);
				putpixel(left+chang-i,bottom-chang+j,linecolor);
				putpixel(right-chang+i,top+chang-j,linecolor);
				putpixel(left+chang-i,top+chang-j,linecolor);
			}
		}
	}
}

void solidroundrect(int left, int top, int right, int bottom, int ellipsewidth, int ellipseheight) {
	/*这个函数用于画无边框的填充圆角矩形。*/
	int lc=linecolor;
	linecolor=fillcolor;
	fillroundrect(left,top,right,bottom,ellipsewidth,ellipseheight);
	linecolor=lc;
}

void clearroundrect(int left, int top, int right, int bottom, int ellipsewidth, int ellipseheight) {
	/*这个函数用于清空圆角矩形区域。*/
	int lc=linecolor,fc=fillcolor;
	linecolor=bkcolor;
	fillcolor=bkcolor;
	fillroundrect(left,top,right,bottom,ellipsewidth,ellipseheight);
	linecolor=lc;
	fillcolor=fc;
}

void arc	 (int left, int top, int right, int bottom, double stangle, double endangle) {
	/*这个函数用于画椭圆弧。*/
	pass;
}

void pie	 (int left, int top, int right, int bottom, double stangle, double endangle) {
	/*这个函数用于画无填充的扇形。*/
//	float R=20;//转弯半径为20米
//	std::vector<PointXYZ> trajectory;
//	PointXYZ start(5,6,0);
//	PointXYZ next_point;
//	for(float len = 0; len < R*M_PI*2; len+=0.5) {
//		float t=len/R;//归一化 将半径看成单位1
//		float d_x=sin(start.theta+t)-sin(start.theta);
//		float d_y=-cos(start.theta + t)+cos(start.theta);
//		float d_head;
//		double a=std::fmod(d_head+M_PI,2.0*M_PI);
//		if (a<0.0)a+=(2.0*M_PI);
//		d_head=a-M_PI;
//		//将半径反乘回来，变成真实值
//		next_point.x=d_x*R+start.x;
//		next_point.y=d_y*R+start.y;
//		next_point.theta = d_head;
//		trajectory.push_back(next_point);
//		putpixel(round(next_point.x),round(next_point.y),WHITE);
//	}
}

void fillpie (int left, int top, int right, int bottom, double stangle, double endangle) {
	/*这个函数用于画有边框的填充扇形。*/
	pass;
}

void solidpie(int left, int top, int right, int bottom, double stangle, double endangle) {
	/*这个函数用于画无边框的填充扇形。*/
	pass;
}

void clearpie(int left, int top, int right, int bottom, double stangle, double endangle) {
	/*这个函数用于清空扇形区域。*/
	pass;
}

void polyline	 (const POINT *points, int num) {
	/*这个函数用于画连续的多条线段。*/
	for(int i=1; i<num; i++)
		line(points[i].x,points[i].y,points[i-1].x,points[i-1].y);
}

void polygon	 (const POINT *points, int num) {
	/*这个函数用于画无填充的多边形。*/
	for(int i=1; i<num; i++)
		line(points[i].x,points[i].y,points[i-1].x,points[i-1].y);
	line(points[num-1].x,points[num-1].y,points[0].x,points[0].y);
}

void fillpolygon (const POINT *points, int num) {
	/*这个函数用于画有边框的填充多边形。*/
	for(int i=1; i<num; i++)
		line(points[i].x,points[i].y,points[i-1].x,points[i-1].y);
	line(points[num-1].x,points[num-1].y,points[0].x,points[0].y);
	bool flag=false;
	POINT pt;
	do {
		pt= {rand()%cx,rand()%cy};
		int nCross=0;
		for(int i=0; i<num; i++) {
			POINT p1;
			POINT p2;
			p1=points[i];
			p2=points[(i+1)%num];
			if(p1.y==p2.y)continue;
			if(pt.y<min(p1.y,p2.y))continue;
			if(pt.y>=max(p1.y,p2.y))continue;
			double x=(double)(pt.y-p1.y)*(double)(p2.x-p1.x)/(double)(p2.y-p1.y)+p1.x;
			if(x>pt.x)nCross++;
		}
		flag=nCross%2;
	} while(!flag);
	floodfill(pt.x,pt.y,linecolor,0);
}

void solidpolygon(const POINT *points, int num) {
	/*这个函数用于画无边框的填充多边形。*/
	int lc=linecolor;
	linecolor=fillcolor;
	for(int i=1; i<num; i++)
		line(points[i].x,points[i].y,points[i-1].x,points[i-1].y);
	line(points[num-1].x,points[num-1].y,points[0].x,points[0].y);
	linecolor=lc;
	bool flag=false;
	POINT pt;
	do {
		pt= {rand()%cx,rand()%cy};
		int nCross=0;
		for(int i=0; i<num; i++) {
			POINT p1;
			POINT p2;
			p1=points[i];
			p2=points[(i+1)%num];
			if(p1.y==p2.y)continue;
			if(pt.y<min(p1.y,p2.y))continue;
			if(pt.y>=max(p1.y,p2.y))continue;
			double x=(double)(pt.y-p1.y)*(double)(p2.x-p1.x)/(double)(p2.y-p1.y)+p1.x;
			if(x>pt.x)nCross++;
		}
		flag=nCross%2;
	} while(!flag);
	floodfill(pt.x,pt.y,fillcolor,0);
}

void clearpolygon(const POINT *points, int num) {
	/*这个函数用于清空多边形区域。*/
	int lc=linecolor,fc=fillcolor;
	linecolor=bkcolor;
	fillcolor=bkcolor;
	for(int i=1; i<num; i++)
		line(points[i].x,points[i].y,points[i-1].x,points[i-1].y);
	line(points[num-1].x,points[num-1].y,points[0].x,points[0].y);
	linecolor=lc;
	bool flag=false;
	POINT pt;
	do {
		pt= {rand()%cx,rand()%cy};
		int nCross=0;
		for(int i=0; i<num; i++) {
			POINT p1;
			POINT p2;
			p1=points[i];
			p2=points[(i+1)%num];
			if(p1.y==p2.y)continue;
			if(pt.y<min(p1.y,p2.y))continue;
			if(pt.y>=max(p1.y,p2.y))continue;
			double x=(double)(pt.y-p1.y)*(double)(p2.x-p1.x)/(double)(p2.y-p1.y)+p1.x;
			if(x>pt.x)nCross++;
		}
		flag=nCross%2;
	} while(!flag);
	floodfill(pt.x,pt.y,bkcolor,0);
	fillcolor=fc;
}

void polybezier(const POINT *points, int num) {
	/*这个函数用于画三次方贝塞尔曲线。*/
	pass;
}

void floodfill(int x, int y, int color, int filltype) {
	/*这个函数用于填充区域。*/
	queue <POINT> q;
	int dx[4]= {0,1,0,-1},dy[4]= {1,0,-1,0};
	q.push({x,y});
	if(!filltype) {
		if(EPOTWcolor[x+cshc][y+cshc]==color)return;
		putpixel(x,y,fillcolor);
		while(!q.empty()) {
			POINT p=q.front();
			q.pop();
			for(int i=0; i<4; i++) {
				int nx=p.x+dx[i],ny=p.y+dy[i];
				if(nx<0||ny<0||nx>cx||ny>cy)continue;
				if(EPOTWcolor[nx+cshc][ny+cshc]!=color&&
				        EPOTWcolor[nx+cshc][ny+cshc]!=fillcolor) {
					putpixel(nx,ny,fillcolor);
					q.push({nx,ny});
				}
			}
		}
	} else {
		if(EPOTWcolor[x+cshc][y+cshc]!=color)return;
		putpixel(x,y,fillcolor);
		while(!q.empty()) {
			POINT p=q.front();
			for(int i=0; i<4; i++) {
				int nx=p.x+dx[i],ny=p.y+dy[i];
				if(nx<0||ny<0||nx>cx||ny>cy)continue;
				if(EPOTWcolor[nx+cshc][ny+cshc]!=color) {
					putpixel(nx,ny,fillcolor);
					q.push({nx,ny});
				}
			}
		}
	}
}

void outtextxy(int x, int y, string str) {
	/*这个函数用于在指定位置输出字符串。*/
	if(str.size()&1)str+=" ";
	if(nowwcolor^textcolor&&!batchDraw) {
		int B=textcolor>>16,G=(textcolor>>8)%256,R=textcolor%256;
		printf("\033[38;2;%d;%d;%dm",R,G,B);
		nowwcolor=textcolor;
	}
	if(bkmode) {
		SetConsoleCursorPosition(hOut, {x<<1,y});
		for(int i=0; i<str.size(); i+=2) {
			int color=EPOTWcolor[x+(i>>1)+cshc][y+cshc];
			int B=color>>16,G=(color>>8)%256,R=color%256;
			if(checkPoint(x+(i>>1),y))printf("\033[48;2;%d;%d;%dm%s",R,G,B,str.substr(i,2).c_str());
			nowbcolor=RGB(R,G,B);
			EPOTWtext[x+(i>>1)+cshc][y+cshc]=str.substr(i,2);
			EPOTWwcolor[x+(i>>1)+cshc][y+cshc]=textcolor;
		}
	} else {
		SetConsoleCursorPosition(hOut, {x<<1,y});
		if(nowbcolor^bkcolor) {
			int B=bkcolor>>16,G=(bkcolor>>8)%256,R=bkcolor%256;
			printf("\033[48;2;%d;%d;%dm",R,G,B);
			nowbcolor=bkcolor;
		}
		for(int i=0; i<str.size(); i+=2) {
			if(checkPoint(x+(i>>1),y))printf("%s",str.substr(i,2).c_str());
			EPOTWtext[x+(i>>1)+cshc][y+cshc]=str.substr(i,2);
			EPOTWwcolor[x+(i>>1)+cshc][y+cshc]=textcolor;
			EPOTWcolor[x+(i>>1)+cshc][y+cshc]=bkcolor;
		}
	}
}

void system_outtextxy(int x, int y, string str) {
	/*这个函数用于在指定位置输出字符串(不受边界限制)。*/
	if(nowwcolor^textcolor&&!batchDraw) {
		int B=textcolor>>16,G=(textcolor>>8)%256,R=textcolor%256;
		printf("\033[38;2;%d;%d;%dm",R,G,B);
		nowwcolor=textcolor;
	}
	if(bkmode) {
		SetConsoleCursorPosition(hOut, {x<<1,y});
		for(int i=0; i<str.size(); i+=2) {
			int color=EPOTWcolor[x+(i>>1)+cshc][y+cshc];
			int B=color>>16,G=(color>>8)%256,R=color%256;
			printf("\033[48;2;%d;%d;%dm%s",R,G,B,str.substr(i,2).c_str());
			EPOTWtext[x+(i>>1)+cshc][y+cshc]=str.substr(i,2);
			EPOTWwcolor[x+(i>>1)+cshc][y+cshc]=textcolor;
		}
	} else {
		SetConsoleCursorPosition(hOut, {x<<1,y});
		if(nowbcolor^bkcolor) {
			int B=bkcolor>>16,G=(bkcolor>>8)%256,R=bkcolor%256;
			printf("\033[48;2;%d;%d;%dm",R,G,B);
			nowbcolor=bkcolor;
		}
		for(int i=0; i<str.size(); i+=2) {
			printf("%s",str.substr(i,2).c_str());
			EPOTWtext[x+(i>>1)+cshc][y+cshc]=str.substr(i,2);
			EPOTWwcolor[x+(i>>1)+cshc][y+cshc]=textcolor;
			EPOTWcolor[x+(i>>1)+cshc][y+cshc]=bkcolor;
		}
	}
}

int textwidth(string str) {
	/*这个函数用于获取字符串实际占用的像素宽度。*/
	return 1;
}

int textheight(string str) {
	/*这个函数用于获取字符串实际占用的像素高度。*/
	return str.size()>>1;
}

int drawtext(string str, RECT* pRect, UINT uFormat) {
	/*这个函数用于在指定区域内以指定格式输出字符串。*/
	pass;
}

void settextstyle(int nHeight, int nWidth, string lpszFace) {
	/*这个函数用于设置当前字体样式。*/
	pass;
}

void settextstyle(int nHeight, int nWidth, string lpszFace, int nEscapement,
                  int nOrientation, int nWeight, bool bItalic, bool bUnderline, bool bStrikeOut) {
	/*这个函数用于设置当前字体样式。*/
	pass;
}

void settextstyle(int nHeight, int nWidth, string lpszFace, int nEscapement,
                  int nOrientation, int nWeight, bool bItalic, bool bUnderline, bool bStrikeOut,
                  BYTE fbCharSet, BYTE fbOutPrecision, BYTE fbClipPrecision, BYTE fbQuality,
                  BYTE fbPitchAndFamily) {
	/*这个函数用于设置当前字体样式。*/
	pass;
}

void settextstyle(const LOGFONT *font) {
	/*这个函数用于设置当前字体样式。*/
	pass;
}

void gettextstyle(LOGFONT *font) {
	/*这个函数用于获取当前字体样式。*/
	pass;
}

int	getwidth() {
	/*该函数用于获取绘图区宽度*/
	return cx;
}

int	getheight() {
	/*该函数用于获取绘图区高度*/
	return cy;
}

void BeginBatchDraw() {
	/*这个函数用于开始批量绘图。
	执行后，任何绘图操作都将暂时不输出到绘图窗口上，
	直到执行 FlushBatchDraw 或 EndBatchDraw 才将之前的绘图输出。*/
	batchDraw=true;
}

void FlushBatchDraw() {
	/*这个函数用于执行未完成的绘制任务。*/
	if(!batchDraw)return;
	int wR,wG,wB,bR,bG,bB;
	SetConsoleCursorPosition(hOut, {0,0});
	for(int j=0; j<cy; j++) {
		for(int i=0; i<cx; i++) {
			if(lEPOTWcolor[i+cshc][j+cshc]^EPOTWcolor[i+cshc][j+cshc]||
			        lEPOTWwcolor[i+cshc][j+cshc]^EPOTWwcolor[i+cshc][j+cshc]||
			        lEPOTWtext[i+cshc][j+cshc]!=EPOTWtext[i+cshc][j+cshc]) {
				if(nowwcolor^EPOTWwcolor[i+cshc][j+cshc]) {
					nowwcolor=EPOTWwcolor[i+cshc][j+cshc];
					wR=EPOTWwcolor[i+cshc][j+cshc]%256;
					wG=(EPOTWwcolor[i+cshc][j+cshc]>>8)%256;
					wB=EPOTWwcolor[i+cshc][j+cshc]>>16;
					printf("\033[38;2;%d;%d;%dm",wR,wG,wB);
				}
				if(nowbcolor^EPOTWcolor[i+cshc][j+cshc]) {
					nowbcolor=EPOTWcolor[i+cshc][j+cshc];
					bR=EPOTWcolor[i+cshc][j+cshc]%256;
					bG=(EPOTWcolor[i+cshc][j+cshc]>>8)%256;
					bB=EPOTWcolor[i+cshc][j+cshc]>>16;
					printf("\033[48;2;%d;%d;%dm",bR,bG,bB);
				}
				printf("%s",EPOTWtext[i+cshc][j+cshc].c_str());
			} else SetConsoleCursorPosition(hOut, {(i<<1)+2,j});
			lEPOTWcolor[i+cshc][j+cshc]=EPOTWcolor[i+cshc][j+cshc];
			lEPOTWwcolor[i+cshc][j+cshc]=EPOTWwcolor[i+cshc][j+cshc];
			lEPOTWtext[i+cshc][j+cshc]=EPOTWtext[i+cshc][j+cshc];
		}
		if(j^(cy-1))printf("\n");
	}
}

void FlushBatchDraw(int left, int top, int right, int bottom) {
	/*[重载]执行指定区域内未完成的绘制任务*/
	if(!batchDraw)return;
	int wR,wG,wB,bR,bG,bB;
	SetConsoleCursorPosition(hOut, {0,0});
	for(int j=top; j<=bottom; j++) {
		SetConsoleCursorPosition(hOut, {left<<1,j});
		for(int i=left; i<=right; i++) {
			if(lEPOTWcolor[i+cshc][j+cshc]^EPOTWcolor[i+cshc][j+cshc]||
			        lEPOTWwcolor[i+cshc][j+cshc]^EPOTWwcolor[i+cshc][j+cshc]||
			        lEPOTWtext[i+cshc][j+cshc]!=EPOTWtext[i+cshc][j+cshc]) {
				if(nowwcolor^EPOTWwcolor[i+cshc][j+cshc]) {
					nowwcolor=EPOTWwcolor[i+cshc][j+cshc];
					wR=EPOTWwcolor[i+cshc][j+cshc]%256;
					wG=(EPOTWwcolor[i+cshc][j+cshc]>>8)%256;
					wB=EPOTWwcolor[i+cshc][j+cshc]>>16;
					printf("\033[38;2;%d;%d;%dm",wR,wG,wB);
				}
				if(nowbcolor^EPOTWcolor[i+cshc][j+cshc]) {
					nowbcolor=EPOTWcolor[i+cshc][j+cshc];
					bR=EPOTWcolor[i+cshc][j+cshc]%256;
					bG=(EPOTWcolor[i+cshc][j+cshc]>>8)%256;
					bB=EPOTWcolor[i+cshc][j+cshc]>>16;
					printf("\033[48;2;%d;%d;%dm",bR,bG,bB);
				}
				printf("%s",EPOTWtext[i+cshc][j+cshc].c_str());
			} else SetConsoleCursorPosition(hOut, {(i<<1)+2,j});
			lEPOTWcolor[i+cshc][j+cshc]=EPOTWcolor[i+cshc][j+cshc];
			lEPOTWwcolor[i+cshc][j+cshc]=EPOTWwcolor[i+cshc][j+cshc];
			lEPOTWtext[i+cshc][j+cshc]=EPOTWtext[i+cshc][j+cshc];
		}
	}
}

void EndBatchDraw() {
	/*这个函数用于结束批量绘制，并执行未完成的绘制任务。
	结束批量绘制，并执行未完成的绘制任务*/
	FlushBatchDraw();
	batchDraw=false;
}

void EndBatchDraw(int left, int top, int right, int bottom) {
	/*[重载]这个函数用于结束批量绘制，并执行未完成的绘制任务。
	结束批量绘制，并执行指定区域内未完成的绘制任务*/
	FlushBatchDraw(left,top,right,bottom);
	batchDraw=false;
}

HWND GetHWnd() {
	/*这个函数用于获取绘图窗口句柄。*/
	return hwnd;
}

string GetGGCCVer() {
	/*这个函数用于获取当前 GGCC 的版本信息。*/
	return "GGCC[2018-2021][v5.2.1] ggcc_graphics[v1.6] 持续更新~";
}

void settextsize(double sizex,double sizey) {
	/*这个函数用于设置字体宽高*/
	if(sizex<=0||sizey<=0)return;
	textX=sizex;
	textY=sizey;
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof cfi;
	cfi.nFont = 0;
	cfi.dwFontSize.X = sizex;
	cfi.dwFontSize.Y = sizey;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy(cfi.FaceName, L"Kaiti");
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

void setwindowsize(int sizex,int sizey) {
	/*这个函数用于设置窗口宽高*/
	settextsize(sizex/cx,sizey/cx);
}

void settitle(string a) {
	/*这个函数用于设置控制台标题*/
	SetConsoleTitle(a.c_str());
}

bool WinInFront() {
	/*这个函数用于判断窗口是否在最顶层*/
	return hwnd==GetConsoleWindow();
}

bool GetGameTime() {
	/*这个函数用于获取游戏时间，单位：毫秒*/
	return clock()-StartGameTime;
}

int MixColor(int color1,int color2,int percent1,int percent2) {
	/*这个函数用于混合颜色*/
	int R1,G1,B1,R2,G2,B2,mcolor;
	R1=color1%256,R2=color2%256;
	G1=(color1>>8)%256,G2=(color2>>8)%256;
	B1=color1>>16,B2=color2>>16;
	mcolor=(R1*percent1/100+R2*percent2/100)+
	       ((G1*percent1/100+G2*percent2/100)<<8)+
	       ((B1*percent1/100+B2*percent2/100)<<16);
	return mcolor;
}

int ContraryColor(int color) {
	/*这个函数用于获取该颜色的相反色*/
	int R,G,B;
	R=255-color%256,G=255-(color>>8)%256,B=255-(color>>16);
	return R+(G<<8)+(B<<16);
}

void popup(int x1,int y1,int x2,int y2,string message,string title,
           int boxcolor=WHITE) {
	/*这个函数用于绘制弹窗*/
}
