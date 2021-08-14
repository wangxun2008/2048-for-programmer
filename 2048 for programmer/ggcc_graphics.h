//ggccͼ�ο�
//ggcc_graphics.h

/*----------------------------------------------------------------------
���ܣ�
	��ͼ�ο�
	1���ǻ��ڿ���̨��αͼ�ο⣬�����easyx, �������Ϊggcc.h��������
	2������24λ���ɫ��Ⱦͼ�������˾ɰ��4λɫ��Ⱦ����ȾЧ������
	3��ɾ���˷����ġ����ġ��ײ�ľɰ溯��������һ����ggcc��׼
	4���������������淶�Ķ��壬��ʹ���߸�������
	5��ɾ����������Ĵ��룬��Ⱦ�ٶȸ���
	6��������Dev-cpp
��Ϣ��
	�汾��v1.6
	���ߣ���ѭ
	��ʼʱ�䣺2021-7-9 17:00
-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------
������־��

|  ����	 | ������ | ������ | ����
__________________________________________________________________
2021-7-9	625��	625��	����˸��ֵײ㺯��
2021-7-10 	443��	1068��	��ʽ����ͼ����Ⱦ���֣�����˵㡢������
							���Ρ�Բ����Բ�ĸ�����Ⱦ����
2021-7-11   227��	1295��	��������ִ���������ͼ����
2021-7-12    76��   1371��  ����˶���ε���Ⱦ
2021-7-19   276��	1647��	����˿������Ļ��ƣ������˳����޸�
							��һϵ��bug
2021-7-23	 54��	1701��	�������ɫ�Ļ���뷴ת
2021-8-3	-11��	1690��	�޸���һϵ��bug
2021-8-9	 65��	1755��  �����Բ�Ǿ��εĻ��ƣ��޸�����������
							���Ρ�BFS��ˮ����bug 

-----------------------------------------------------------------------*/

#include <bits/stdc++.h>
#include <stdio.h>
#include <conio.h>
#include <cstdlib>
#include <windows.h>
#include <fstream>
#include <pthread.h>
#include <direct.h>

// ��ɫ
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

//VT����
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

// �����ʽ
#define	BDIAGONAL_FILL		0x0001				// /// ���
#define CROSS_FILL			0x0002				// +++ ���
#define DIAGCROSS_FILL		0x0004				// xxx ���
#define DOT_FILL			0x0008				// xxx ���
#define FDIAGONAL_FILL		0x0010				// \\\ ���
#define HORIZONTAL_FILL		0x0020				// === ���
#define VERTICAL_FILL		0x0040				// ||| ���

// ��䶯��
#define FLOODFILLBORDER		0x0000				// ��䶯������ɫ����Χ�ɵķ�����������
#define FLOODFILLSURFACE	0x0001				// ��䶯������ɫ����ָ����������ɫ�������

// �������ģʽ
#define OPAQUE              0x0000              // Ĭ�ϱ���ɫ 
#define TRANSPARENT         0x0001				// ͸�� 

// δ��ɱ�ʶ��
#define pass ;

// ������ɫת����
#define BGR(color) ((((color)&0xFF)<<16)|((color)&0xFF00FF00)|(((color)&0xFF0000)>>16))

// �����
#define click(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)

// ���ڻ�����
#define wbs 2000								// �������߳�
#define cshc 300								// ���ڲ���ʾ�������߳� 

using namespace std;



//----------���δ洢----------
struct RectAngle {
	POINT p1,p2;
	bool inRectangle(POINT p) {
		return (p.x>=p1.x&&p.x<=p2.x&&p.y>=p1.y&&p.y<=p2.y);
	}
};

//----------����δ洢----------
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

//----------Բ�Ĵ洢----------
struct Circle {
	POINT p;
	int r;
	bool inCircle(POINT pt) {
		int a=abs(pt.x-p.x);
		int b=abs(pt.y-p.y);
		return sqrt(a*a+b*b)<=r;
	}
};

//----------��Ĵ���----------
struct Pixel {
	POINT p;
	bool inPixel(POINT pt) {
		return (pt.x==p.x&&pt.y==p.x);
	}
};

//----------������ʽ----------
struct LINESTYLE {
	int style;				// ������ʽ(���ã�Ϊ����easyx)
	int thickness;			// �ߵĿ��
};

//----------�����ʽ----------
struct FILLSTYLE {
	int style;				// �����ʽ
};

//----------ͼ�񴢴�---------
struct IMAGE {
	//���ﻹû�ж�����
	pass;
};

//----------��ʱ�ṹ��----------
struct PointXYZ {
	PointXYZ() = default;
	PointXYZ(float x_,float y_,float theta_):x(x_),y(y_),theta(theta) {}
	float x {0.0};
	float y {0.0};
	float theta {0.0};
};

//----------���崢��----------
// ���õ�ǰ������ʽ
//		nHeight: �ַ���ƽ���߶ȣ�
//		nWidth: �ַ���ƽ�����(0 ��ʾ����Ӧ)��
//		lpszFace: �������ƣ�
//		nEscapement: �ַ�������д�Ƕ�(��λ 0.1 ��)��
//		nOrientation: ÿ���ַ�����д�Ƕ�(��λ 0.1 ��)��
//		nWeight: �ַ��ıʻ���ϸ(0 ��ʾĬ�ϴ�ϸ)��
//		bItalic: �Ƿ�б�壻
//		bUnderline: �Ƿ��»��ߣ�
//		bStrikeOut: �Ƿ�ɾ���ߣ�
//		fbCharSet: ָ���ַ�����
//		fbOutPrecision: ָ�����ֵ�������ȣ�
//		fbClipPrecision: ָ�����ֵļ������ȣ�
//		fbQuality: ָ�����ֵ����������
//		fbPitchAndFamily: ָ���Գ��淽ʽ�������������ϵ�С�



//----------���Ʊ���----------
int linecolor=RGB(0,0,0);	// ������ɫ
int fillcolor=RGB(0,0,0);	// ���ɫ
int bkcolor=RGB(0,0,0);		// ����ɫ
int textcolor=RGB(0,0,0);	// ����ɫ

int nowbcolor=RGB(0,0,0);	// ��ǰ���屳��ɫ
int nowwcolor=RGB(0,0,0);	// ��ǰ����ǰ��ɫ

bool ctrl=true;				// �Ƿ���ʾ������
int choose_ctrl_time[10];	// ����������ѡ��ʱ��

//----------ϵͳ����----------
HANDLE hIn;
HANDLE hOut;
DWORD dwInMode, dwOutMode;
HWND hwnd = GetConsoleWindow();
RECT rc;
CONSOLE_CURSOR_INFO cciCursor;
const int WINx = GetSystemMetrics(SM_CXSCREEN);		// ��Ļ�ֱ���
const int WINy = GetSystemMetrics(SM_CYSCREEN);		// ��Ļ�ֱ���
int cx = GetSystemMetrics(SM_CXSCREEN);				// ���ڿ�
int cy = GetSystemMetrics(SM_CYSCREEN);				// ���ڸ�
double textX = 8.0;									// �����
double textY = 16.0;								// �����
int win_TOP = false;								// �Ƿ��ö�
int StartGameTime=clock();							// ��ʼ��Ϸʱ��
int LastFPSTime=0;									// ��һ�μ��FPSʱ��
int FPS=0;											// ��ǰFPS
int FPSSum=0;										// ����FPS����
int LastFPS=0;										// ��һ�μ���FPS

//----------��ͼ�豸����----------
RectAngle Cliprgn= {{0,0},{0,0}};					// ��ͼ�ü���
LINESTYLE Linestyle;								// ��ǰ������ʽ
FILLSTYLE Fillstyle;								// ��ǰ�����ʽ
int EPOTWcolor[wbs+1][wbs+1];						// ������ÿ�����ص����ɫ
int EPOTWwcolor[wbs+1][wbs+1];						// ������ÿ�����ص��������ɫ
string EPOTWtext[wbs+1][wbs+1];						// ������ÿ�����ص���д����
int lEPOTWcolor[wbs+1][wbs+1];						// �ϴδ�����ÿ�����ص����ɫ
int lEPOTWwcolor[wbs+1][wbs+1];						// �ϴδ�����ÿ�����ص��������ɫ
string lEPOTWtext[wbs+1][wbs+1];					// �ϴδ�����ÿ�����ص���д����
int bkmode;											// �������ģʽ
bool batchDraw;										// �Ƿ���������ͼ
POINT mouse_deviation1= {0,0};						// ���ƫ��Ӽ�����λ������
POINT mouse_deviation2= {1.0,1.0};					// ���ƫ��˱���
int orx=0;											// ����ԭ��x
int ory=0;											// ����ԭ��y

//----------��������----------

// ��ͼģʽ��غ���

HWND initgraph(int w, int h,bool _ctrl);				// ��ʼ��ͼ�λ���
void closegraph();										// �ر�ͼ�λ���

// ��ͼ��������
void cleardevice();					// ����
void setcliprgn(RectAngle hrgn);	// ���õ�ǰ��ͼ�豸�Ĳü���
void clearcliprgn();				// ����ü�������Ļ����
void getlinestyle(LINESTYLE* pstyle);						// ��ȡ��ǰ������ʽ
void setlinestyle(const LINESTYLE* pstyle);					// ���õ�ǰ������ʽ
void setlinestyle(int style, int thickness = 1);			// ���õ�ǰ������ʽ
void getfillstyle(FILLSTYLE* pstyle);						// ��ȡ��ǰ�����ʽ
void setfillstyle(FILLSTYLE pstyle);						// ���õ�ǰ�����ʽ
int  getrop2();						// ��ȡǰ���Ķ�Ԫ��դ����ģʽ
void setrop2(int mode);				// ����ǰ���Ķ�Ԫ��դ����ģʽ
int  getpolyfillmode();				// ��ȡ��������ģʽ
void setpolyfillmode(int mode);		// ���ö�������ģʽ
void graphdefaults();				// �������л�ͼ����ΪĬ��ֵ
int getlinecolor();					// ��ȡ��ǰ������ɫ
void setlinecolor(int color);		// ���õ�ǰ������ɫ
int gettextcolor();					// ��ȡ��ǰ������ɫ
void settextcolor(int color);		// ���õ�ǰ������ɫ
int getfillcolor();					// ��ȡ��ǰ�����ɫ
void setfillcolor(int color);		// ���õ�ǰ�����ɫ
int getbkcolor();					// ��ȡ��ǰ��ͼ����ɫ
void setbkcolor(int color);			// ���õ�ǰ��ͼ����ɫ
int  getbkmode();					// ��ȡ�������ģʽ
void setbkmode(int mode);			// ���ñ������ģʽ

// ��ɫģ��ת������
int RGBtoGRAY(int rgb);
void RGBtoHSL(int rgb, float *H, float *S, float *L);
void RGBtoHSV(int rgb, float *H, float *S, float *V);
int HSLtoRGB(float H, float S, float L);
int HSVtoRGB(float H, float S, float V);


// ��ͼ����
int getpixel(int x, int y);				// ��ȡ�����ɫ
void putpixel(int x, int y, int color);	// ����
void line(int x1, int y1, int x2, int y2);		// ����
void rectangle	   (int left, int top, int right, int bottom);	// ������
void system_rectangle(int left, int top, int right, int bottom);// ������
void fillrectangle (int left, int top, int right,
                    int bottom);	// ��������(�б߿�)
void solidrectangle(int left, int top, int right,
                    int bottom);	// ��������(�ޱ߿�)
void system_solidrectangle(int left, int top, int right,
                           int bottom);	// ��������(�ޱ߿�)
void clearrectangle(int left, int top, int right, int bottom);	// ��վ�������
void circle		(int x, int y, int radius);		// ��Բ
void fillcircle (int x, int y, int radius);		// �����Բ(�б߿�)
void solidcircle(int x, int y, int radius);		// �����Բ(�ޱ߿�)
void clearcircle(int x, int y, int radius);		// ���Բ������
void ellipse	 (int left, int top, int right, int bottom);	// ����Բ
void fillellipse (int left, int top, int right,
                  int bottom);	// �������Բ(�б߿�)
void solidellipse(int left, int top, int right,
                  int bottom);	// �������Բ(�ޱ߿�)
void clearellipse(int left, int top, int right, int bottom);	// �����Բ������
void roundrect	   (int left, int top, int right, int bottom, int ellipsewidth,
                    int ellipseheight);		// ��Բ�Ǿ���
void fillroundrect (int left, int top, int right, int bottom, int ellipsewidth,
                    int ellipseheight);		// �����Բ�Ǿ���(�б߿�)
void solidroundrect(int left, int top, int right, int bottom, int ellipsewidth,
                    int ellipseheight);		// �����Բ�Ǿ���(�ޱ߿�)
void clearroundrect(int left, int top, int right, int bottom, int ellipsewidth,
                    int ellipseheight);		// ���Բ�Ǿ�������
void arc	 (int left, int top, int right, int bottom, double stangle,
              double endangle);	// ����Բ��(��ʼ�ǶȺ���ֹ�Ƕ�Ϊ������)
void pie	 (int left, int top, int right, int bottom, double stangle,
              double endangle);	// ����Բ����(��ʼ�ǶȺ���ֹ�Ƕ�Ϊ������)
void fillpie (int left, int top, int right, int bottom, double stangle,
              double endangle);	// �������Բ����(�б߿�)
void solidpie(int left, int top, int right, int bottom, double stangle,
              double endangle);	// �������Բ����(�ޱ߿�)
void clearpie(int left, int top, int right, int bottom, double stangle,
              double endangle);	// �����Բ��������
void polyline	 (const POINT *points, int num);								// ��������������
void polygon	 (const POINT *points, int num);								// �������
void fillpolygon (const POINT *points,
                  int num);								// �����Ķ����(�б߿�)
void solidpolygon(const POINT *points,
                  int num);								// �����Ķ����(�ޱ߿�)
void clearpolygon(const POINT *points, int num);								// ��ն��������
void polybezier(const POINT *points, int num);									// ������������
void floodfill(int x, int y, int color,
               int filltype = FLOODFILLBORDER);	// �������

// ������غ���
void outtextxy(int x, int y, string str);	// ��ָ��λ������ַ���
void system_outtextxy(int x, int y, string str);	// ��ָ��λ������ַ���
int textwidth(string str);					// ��ȡ�ַ���ռ�õ����ؿ�
int textheight(string str);				// ��ȡ�ַ���ռ�õ����ظ�
int drawtext(string str, RECT* pRect,
             UINT uFormat);	// ��ָ����������ָ����ʽ����ַ���
void settextstyle(int nHeight, int nWidth, string lpszFace);
void settextstyle(int nHeight, int nWidth, string lpszFace, int nEscapement,
                  int nOrientation, int nWeight, bool bItalic, bool bUnderline, bool bStrikeOut);
void settextstyle(int nHeight, int nWidth, string lpszFace, int nEscapement,
                  int nOrientation, int nWeight, bool bItalic, bool bUnderline, bool bStrikeOut,
                  BYTE fbCharSet, BYTE fbOutPrecision, BYTE fbClipPrecision, BYTE fbQuality,
                  BYTE fbPitchAndFamily);
void settextstyle(const LOGFONT *font);	// ���õ�ǰ������ʽ
void gettextstyle(LOGFONT *font);		// ��ȡ��ǰ������ʽ

// ͼ������
void loadimage(IMAGE *pDstImg, string pImgFile, int nWidth = 0,
               int nHeight = 0, bool bResize =
                   false);					// ��ͼƬ�ļ���ȡͼ��(bmp/gif/jpg/png/tif/emf/wmf/ico)
void loadimage(IMAGE *pDstImg, string pResType, string pResName,
               int nWidth = 0, int nHeight = 0,
               bool bResize = false);	// ����Դ�ļ���ȡͼ��(bmp/gif/jpg/png/tif/emf/wmf/ico)
void saveimage(string pImgFile,
               IMAGE* pImg = NULL);								// ����ͼ��(bmp/gif/jpg/png/tif)
void getimage(IMAGE *pDstImg, int srcX, int srcY, int srcWidth,
              int srcHeight);												// �ӵ�ǰ��ͼ�豸��ȡͼ��
void putimage(int dstX, int dstY, const IMAGE *pSrcImg,
              DWORD dwRop = SRCCOPY);												// ����ͼ����Ļ
void putimage(int dstX, int dstY, int dstWidth, int dstHeight,
              const IMAGE *pSrcImg, int srcX, int srcY,
              DWORD dwRop = SRCCOPY);		// ����ͼ����Ļ(ָ�����)
void rotateimage(IMAGE *dstimg, IMAGE *srcimg, double radian,
                 int bkcolor = BLACK, bool autosize = false, bool highquality = true);// ��תͼ��
void Resize(IMAGE* pImg, int width, int height);	// ������ͼ�豸�Ĵ�С
DWORD* GetImageBuffer(IMAGE* pImg = NULL);			// ��ȡ��ͼ�豸���Դ�ָ��
IMAGE* GetWorkingImage();							// ��ȡ��ǰ��ͼ�豸
void SetWorkingImage(IMAGE* pImg = NULL);			// ���õ�ǰ��ͼ�豸
HDC GetImageHDC(IMAGE* pImg = NULL);				// ��ȡ��ͼ�豸���(HDC)

// ��������
int	getwidth();			// ��ȡ��ͼ�����
int	getheight();		// ��ȡ��ͼ���߶�
void BeginBatchDraw();	// ��ʼ��������
void FlushBatchDraw();	// ִ��δ��ɵĻ�������
void FlushBatchDraw(int left, int top, int right,
                    int bottom);	// ִ��ָ��������δ��ɵĻ�������
void EndBatchDraw();	// �����������ƣ���ִ��δ��ɵĻ�������
void EndBatchDraw(int left, int top, int right,
                  int bottom);	// �����������ƣ���ִ��ָ��������δ��ɵĻ�������
HWND GetHWnd();								// ��ȡ��ͼ���ھ��(HWND)
string GetGGCCVer();						// ��ȡ GGCC ��ǰ�汾
void settextsize(double sizex,double sizey);// ���������С

//----------����-----------
string change_its(int a) {
	/*�����������ת��int��string*/
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
	/*����������ڻ�ȡ���λ��*/
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
	/*����������ڵ������ڴ�С */
	string a=change_its(a1),b=change_its(a2);
	system(("mode "+a+","+b).c_str());
}

bool inSight(int x,int y) {
	/*������������ж�����(x,y)�Ƿ��ڴ��ڿ���ʾ�ķ�Χ�� */
	return (x>=0&&y>=0&&x<cx&&y<cy);
}

bool checkPoint(int x,int y) {
	/*������������ж�����(x,y)�Ƿ����ʾ*/
	return inSight(x,y)&&!batchDraw;
}

HWND initgraph(int w, int h, bool _ctrl=false) {
	/*����������ڳ�ʼ����ͼ���ڡ�
	width
	��ͼ���ڵĿ�ȡ�
	height
	��ͼ���ڵĸ߶ȡ�*/
	cout<<"��ʼ������̨ͼ�λ�������......"<<endl<<endl;
	cout<<"ͼ�λ�����汾��Ϣ:"<<endl;
	cout<<GetGGCCVer()<<endl<<endl;
	cx=w,cy=h;
	hIn = GetStdHandle(STD_INPUT_HANDLE);
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	cout<<"[Finished]��ʼ����������������"<<endl;
	GetConsoleMode(hIn, &dwInMode);
	GetConsoleMode(hOut, &dwOutMode);
	dwInMode |= ENABLE_VIRTUAL_TERMINAL_INPUT;
	dwOutMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hIn, dwInMode);
	SetConsoleMode(hOut, dwOutMode);
	cout<<"[Finished]��ʼ��ANSI������"<<endl;

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
	cout<<"��ʼ������̨ͼ�λ�������......"<<endl<<endl;
	cout<<"ͼ�λ�����汾��Ϣ:"<<endl;
	cout<<GetGGCCVer()<<endl<<endl;
	cout<<"[Finished]��ʼ����������������"<<endl;
	cout<<"[Finished]��ʼ��ANSI������"<<endl;
	cout<<"[Finished]���ô��ڴ�С" <<endl;
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
	cout<<"[Finished]��ʼ��ͼ����Ⱦ����"<<endl;
	system("cls");
	return hwnd;
}

void setorigin(int x,int y) {
	orx=x;
	ory=y;
}

void draw_ctrl(int Ctrl=0) {
	/*����������ڻ��ƿ�����*/
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
		system_outtextxy(0,cy,"  ��GGCC��  ");
		bkmode=1;
		fillcolor=fc;
		bkmode=bkm;
		bkcolor=bkc;
		textcolor=tc;
	}
}

void update_ctrl(int Ctrl=0) {
	/*����������ڸ��¿�����*/
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
		string Button[4]= {"��","��","��","��"};
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
	/*��������������������ʾ��ʾ��Ļ*/
	POINT mp=GetMousePos();
	SetConsoleCursorPosition(hOut, {mp.x<<1,mp.y});
}

void closegraph() {
	/*����������ڹرջ�ͼ���ڡ�*/
	exit(0);
}

void cleardevice() {
	/*�������������ջ�ͼ�豸��
	����ģ����õ�ǰ����ɫ��ջ�ͼ�豸
	������ǰ������ (0, 0)��*/
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
	/*��������������õ�ǰ��ͼ�豸�Ĳü�����*/
	if(hrgn.p1.x>hrgn.p2.x)hrgn.p1.x^=hrgn.p2.x,hrgn.p2.x^=hrgn.p1.x,
		                                  hrgn.p1.x^=hrgn.p2.x;
	if(hrgn.p1.y>hrgn.p2.y)hrgn.p1.y^=hrgn.p2.y,hrgn.p2.y^=hrgn.p1.y,
		                                  hrgn.p1.y^=hrgn.p2.y;
	Cliprgn=hrgn;
}

void clearcliprgn() {
	/*�������������ղü���*/
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
	/*����������ڻ�ȡ��ǰ�豸������ʽ��*/
	pstyle=Linestyle;
}

void setlinestyle(LINESTYLE pstyle) {
	/*��������������õ�ǰ�豸������ʽ��*/
	Linestyle=pstyle;
}

void setlinestyle(int style, int thickness) {
	/*[����]��������������õ�ǰ�豸������ʽ��*/
	Linestyle.style=style;
	Linestyle.thickness=thickness;
}

void getfillstyle(FILLSTYLE &pstyle) {
	/*����������ڻ�ȡ��ǰ�豸�����ʽ��*/
	pstyle=Fillstyle;
}

void setfillstyle(FILLSTYLE pstyle) {
	/*��������������õ�ǰ�豸�����ʽ��*/
	Fillstyle=pstyle;
}

int getrop2() {
	/*����������ڻ�ȡ��ǰ�豸��Ԫ��դ����ģʽ��*/
	pass;
}
void setrop2(int mode) {
	/*��������������õ�ǰ�豸��Ԫ��դ����ģʽ��*/
	pass;
}

int getpolyfillmode() {
	/*����������ڻ�ȡ��ǰ�豸��������ģʽ��*/
	pass;
}

void setpolyfillmode(int mode) {
	/*��������������õ�ǰ�豸��������ģʽ��*/
	pass;
}

void graphdefaults() {
	/*�����������������ͼ����ǰ�㡢��ͼɫ������ɫ�����Ρ������ʽ������ΪĬ��ֵ��*/
	int linecolor=RGB(0,0,0);	//������ɫ
	int fillcolor=RGB(0,0,0);	//���ɫ
	int bkcolor=RGB(0,0,0);		//����ɫ
	int textcolor=RGB(0,0,0);	//����ɫ
	const int WINx = GetSystemMetrics(SM_CXSCREEN);		//��Ļ�ֱ���
	const int WINy = GetSystemMetrics(SM_CYSCREEN);		//��Ļ�ֱ���
	RectAngle Cliprgn= {{0,0},{0,0}};					//��ͼ�ü���
	LINESTYLE Linestyle= {0,0};							//��ǰ������ʽ
	FILLSTYLE Fillstyle= {0};							//��ǰ�����ʽ
}

int getlinecolor() {
	/*����������ڻ�ȡ��ǰ�豸������ɫ��*/
	return linecolor;
}

void setlinecolor(int color) {
	/*��������������õ�ǰ�豸������ɫ��*/
	linecolor=color;
}

int gettextcolor() {
	/*����������ڻ�ȡ��ǰ������ɫ��*/
	return textcolor;
}

void settextcolor(int color) {
	/*��������������õ�ǰ������ɫ��*/
	textcolor=color;
}

int getfillcolor() {
	/*����������ڻ�ȡ��ǰ�豸�����ɫ��*/
	return fillcolor;
}

void setfillcolor(int color) {
	/*��������������õ�ǰ�豸�����ɫ��*/
	fillcolor=color;
}

int getbkcolor() {
	/*����������ڻ�ȡ��ǰ�豸����ɫ��*/
	return bkcolor;
}

void setbkcolor(int color) {
	/*��������������õ�ǰ�豸��ͼ����ɫ��*/
	bkcolor=color;
}

int  getbkmode() {
	/*����������ڻ�ȡ��ǰ�豸ͼ�������������ʱ�ı���ģʽ��*/
	return bkmode;
}

void setbkmode(int mode) {
	/*��������������õ�ǰ�豸ͼ�������������ʱ�ı���ģʽ��*/
	bkmode=mode;
}

void setcolor(int color) {
	/*��������������õ�ǰ��ͼǰ��ɫ��*/
	linecolor=color;
	textcolor=color;
}

int RGBtoGRAY(int rgb) {
	/*�ú������ڷ�����ָ����ɫ��Ӧ�ĻҶ�ֵ��ɫ��*/
	int B=rgb>>16,G=(rgb>>8)/256,R=rgb%256;
	double y=0.212671*R+0.715160*G+(double)0.072169*B;
	if(y>0.008856)return 116.0*pow(y,1.0/3)-16.0;
	else return 903.3*y;
}

void RGBtoHSL(int rgb, float &H, float &S, float &L) {
	/*�ú�������ת�� RGB ��ɫΪ HSL ��ɫ��*/
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
	/*�ú�������ת�� RGB ��ɫΪ HSV ��ɫ��*/
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
	/*�ú�������ת�� HSL ��ɫΪ RGB ��ɫ��*/
	pass;
}

int HSVtoRGB(float H, float S, float V) {
	/*�ú�������ת�� RGB ��ɫΪ HSV ��ɫ��*/
	pass;
}

int getpixel(int x, int y) {
	/*����������ڻ�ȡ�����ɫ��*/
	return EPOTWcolor[x+cshc][y+cshc];
}

void putpixel(int x, int y, int color) {
	/*����������ڻ��㡣*/
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
	/*����������ڻ�ֱ�ߡ�*/
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
	/*����������ڻ������ľ��Ρ�*/
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
	/*����������ڻ������ľ��Ρ������ܱ߽����ƣ�*/
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
	/*����������ڻ��б߿�������Ρ�*/
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
	/*����������ڻ��ޱ߿�������Ρ�*/
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
	/*����������ڻ��ޱ߿��������(���ܱ߽�����)��*/
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
	/*�������������վ�������*/
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
	/*����������ڻ�������Բ�� */
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
	/*����������ڻ��б߿�����Բ��*/
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
	/*����������ڻ��ޱ߿�����Բ*/
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
	/*��������������Բ������*/
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
	/*����������ڻ���������Բ��*/
	if(left>right)left^=right,right^=left,left^=right;
	if(top>bottom)top^=bottom,bottom^=top,top^=bottom;
	int x0=(left+right)>>1,y0=(top+bottom)>>1;
	int a=(right-left)>>1,b=(bottom-top)>>1;
	int x=0,y=b;
	putpixel(x0+x,y0+y,linecolor);
	putpixel(x0-x,y0+y,linecolor);
	putpixel(x0+x,y0-y,linecolor);
	putpixel(x0-x,y0-y,linecolor);
	//�ϲ���
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
	//�²���
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
	/*����������ڻ��б߿�������Բ��*/
	ellipse(left,top,right,bottom);
	int x=(left+right)>>1,y=(top+bottom)>>1;
	floodfill(x,y,linecolor,FLOODFILLBORDER);
}

void solidellipse(int left, int top, int right, int bottom) {
	/*����������ڻ��ޱ߿�������Բ��*/
	int lc=linecolor;
	linecolor=fillcolor;
	ellipse(left,top,right,bottom);
	int x=(left+right)>>1,y=(top+bottom)>>1;
	floodfill(x,y,linecolor,FLOODFILLBORDER);
	linecolor=lc;
}

void clearellipse(int left, int top, int right, int bottom) {
	/*����������������Բ����*/
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
	/*����������ڻ�Բ�Ǿ��Ρ�*/
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
	/*����������ڻ��б߿�����Բ�Ǿ��Ρ�*/
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
	/*����������ڻ��ޱ߿�����Բ�Ǿ��Ρ�*/
	int lc=linecolor;
	linecolor=fillcolor;
	fillroundrect(left,top,right,bottom,ellipsewidth,ellipseheight);
	linecolor=lc;
}

void clearroundrect(int left, int top, int right, int bottom, int ellipsewidth, int ellipseheight) {
	/*��������������Բ�Ǿ�������*/
	int lc=linecolor,fc=fillcolor;
	linecolor=bkcolor;
	fillcolor=bkcolor;
	fillroundrect(left,top,right,bottom,ellipsewidth,ellipseheight);
	linecolor=lc;
	fillcolor=fc;
}

void arc	 (int left, int top, int right, int bottom, double stangle, double endangle) {
	/*����������ڻ���Բ����*/
	pass;
}

void pie	 (int left, int top, int right, int bottom, double stangle, double endangle) {
	/*����������ڻ����������Ρ�*/
//	float R=20;//ת��뾶Ϊ20��
//	std::vector<PointXYZ> trajectory;
//	PointXYZ start(5,6,0);
//	PointXYZ next_point;
//	for(float len = 0; len < R*M_PI*2; len+=0.5) {
//		float t=len/R;//��һ�� ���뾶���ɵ�λ1
//		float d_x=sin(start.theta+t)-sin(start.theta);
//		float d_y=-cos(start.theta + t)+cos(start.theta);
//		float d_head;
//		double a=std::fmod(d_head+M_PI,2.0*M_PI);
//		if (a<0.0)a+=(2.0*M_PI);
//		d_head=a-M_PI;
//		//���뾶���˻����������ʵֵ
//		next_point.x=d_x*R+start.x;
//		next_point.y=d_y*R+start.y;
//		next_point.theta = d_head;
//		trajectory.push_back(next_point);
//		putpixel(round(next_point.x),round(next_point.y),WHITE);
//	}
}

void fillpie (int left, int top, int right, int bottom, double stangle, double endangle) {
	/*����������ڻ��б߿��������Ρ�*/
	pass;
}

void solidpie(int left, int top, int right, int bottom, double stangle, double endangle) {
	/*����������ڻ��ޱ߿��������Ρ�*/
	pass;
}

void clearpie(int left, int top, int right, int bottom, double stangle, double endangle) {
	/*����������������������*/
	pass;
}

void polyline	 (const POINT *points, int num) {
	/*����������ڻ������Ķ����߶Ρ�*/
	for(int i=1; i<num; i++)
		line(points[i].x,points[i].y,points[i-1].x,points[i-1].y);
}

void polygon	 (const POINT *points, int num) {
	/*����������ڻ������Ķ���Ρ�*/
	for(int i=1; i<num; i++)
		line(points[i].x,points[i].y,points[i-1].x,points[i-1].y);
	line(points[num-1].x,points[num-1].y,points[0].x,points[0].y);
}

void fillpolygon (const POINT *points, int num) {
	/*����������ڻ��б߿��������Ρ�*/
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
	/*����������ڻ��ޱ߿��������Ρ�*/
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
	/*�������������ն��������*/
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
	/*����������ڻ����η����������ߡ�*/
	pass;
}

void floodfill(int x, int y, int color, int filltype) {
	/*������������������*/
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
	/*�������������ָ��λ������ַ�����*/
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
	/*�������������ָ��λ������ַ���(���ܱ߽�����)��*/
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
	/*����������ڻ�ȡ�ַ���ʵ��ռ�õ����ؿ�ȡ�*/
	return 1;
}

int textheight(string str) {
	/*����������ڻ�ȡ�ַ���ʵ��ռ�õ����ظ߶ȡ�*/
	return str.size()>>1;
}

int drawtext(string str, RECT* pRect, UINT uFormat) {
	/*�������������ָ����������ָ����ʽ����ַ�����*/
	pass;
}

void settextstyle(int nHeight, int nWidth, string lpszFace) {
	/*��������������õ�ǰ������ʽ��*/
	pass;
}

void settextstyle(int nHeight, int nWidth, string lpszFace, int nEscapement,
                  int nOrientation, int nWeight, bool bItalic, bool bUnderline, bool bStrikeOut) {
	/*��������������õ�ǰ������ʽ��*/
	pass;
}

void settextstyle(int nHeight, int nWidth, string lpszFace, int nEscapement,
                  int nOrientation, int nWeight, bool bItalic, bool bUnderline, bool bStrikeOut,
                  BYTE fbCharSet, BYTE fbOutPrecision, BYTE fbClipPrecision, BYTE fbQuality,
                  BYTE fbPitchAndFamily) {
	/*��������������õ�ǰ������ʽ��*/
	pass;
}

void settextstyle(const LOGFONT *font) {
	/*��������������õ�ǰ������ʽ��*/
	pass;
}

void gettextstyle(LOGFONT *font) {
	/*����������ڻ�ȡ��ǰ������ʽ��*/
	pass;
}

int	getwidth() {
	/*�ú������ڻ�ȡ��ͼ�����*/
	return cx;
}

int	getheight() {
	/*�ú������ڻ�ȡ��ͼ���߶�*/
	return cy;
}

void BeginBatchDraw() {
	/*����������ڿ�ʼ������ͼ��
	ִ�к��κλ�ͼ����������ʱ���������ͼ�����ϣ�
	ֱ��ִ�� FlushBatchDraw �� EndBatchDraw �Ž�֮ǰ�Ļ�ͼ�����*/
	batchDraw=true;
}

void FlushBatchDraw() {
	/*�����������ִ��δ��ɵĻ�������*/
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
	/*[����]ִ��ָ��������δ��ɵĻ�������*/
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
	/*����������ڽ����������ƣ���ִ��δ��ɵĻ�������
	�����������ƣ���ִ��δ��ɵĻ�������*/
	FlushBatchDraw();
	batchDraw=false;
}

void EndBatchDraw(int left, int top, int right, int bottom) {
	/*[����]����������ڽ����������ƣ���ִ��δ��ɵĻ�������
	�����������ƣ���ִ��ָ��������δ��ɵĻ�������*/
	FlushBatchDraw(left,top,right,bottom);
	batchDraw=false;
}

HWND GetHWnd() {
	/*����������ڻ�ȡ��ͼ���ھ����*/
	return hwnd;
}

string GetGGCCVer() {
	/*����������ڻ�ȡ��ǰ GGCC �İ汾��Ϣ��*/
	return "GGCC[2018-2021][v5.2.1] ggcc_graphics[v1.6] ��������~";
}

void settextsize(double sizex,double sizey) {
	/*���������������������*/
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
	/*��������������ô��ڿ��*/
	settextsize(sizex/cx,sizey/cx);
}

void settitle(string a) {
	/*��������������ÿ���̨����*/
	SetConsoleTitle(a.c_str());
}

bool WinInFront() {
	/*������������жϴ����Ƿ������*/
	return hwnd==GetConsoleWindow();
}

bool GetGameTime() {
	/*����������ڻ�ȡ��Ϸʱ�䣬��λ������*/
	return clock()-StartGameTime;
}

int MixColor(int color1,int color2,int percent1,int percent2) {
	/*����������ڻ����ɫ*/
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
	/*����������ڻ�ȡ����ɫ���෴ɫ*/
	int R,G,B;
	R=255-color%256,G=255-(color>>8)%256,B=255-(color>>16);
	return R+(G<<8)+(B<<16);
}

void popup(int x1,int y1,int x2,int y2,string message,string title,
           int boxcolor=WHITE) {
	/*����������ڻ��Ƶ���*/
}
