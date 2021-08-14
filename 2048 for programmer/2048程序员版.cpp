#include <ggcc_graphics.h>

const int size=4,winsize=size*5+10;
int Map[10][10],Map2[10][10];
int mode=0,score=0,best=0;
map <int,string> nte;
map <int,string> nte2;

void EvaluationStatus(int x,int y,int a,int mod) {
	string str;
	if(mode==0) {
		str=nte[a];
		setbkmode(1),settextcolor(WHITE);
		if(str=="RI ")setfillcolor(RGB(120,120,120));
		else if(str=="CE ")setfillcolor(RGB(253,208,0));
		else if(str=="UKE")setfillcolor(RGB(13,49,66));
		else if(str=="TLE")setfillcolor(RGB(27,62,143));
		else if(str=="MLE")setfillcolor(RGB(27,62,143));
		else if(str=="OLE")setfillcolor(RGB(27,62,143));
		else if(str=="RE ")setfillcolor(RGB(138,0,138));
		else if(str=="WA ")setfillcolor(RED);
		else if(str=="PC ")setfillcolor(RGB(236,118,0));
		else if(str=="AC ")setfillcolor(GREEN);
		else if(str=="AK ")setfillcolor(BLUE);
		else setfillcolor(RGB(56,56,56));
	} else if(mode==1) {
		str=nte2[a];
		setbkmode(1),settextcolor(WHITE);
		if(str=="商")setfillcolor(RGB(236,227,226)),settextcolor(BLACK);
		else if(str=="周")setfillcolor(RGB(233,219,178)),settextcolor(BLACK);
		else if(str=="秦")setfillcolor(RGB(238,171,130));
		else if(str=="汉")setfillcolor(RGB(250,147,83));
		else if(str=="唐")setfillcolor(RGB(250,114,83));
		else if(str=="宋")setfillcolor(RGB(243,82,46));
		else if(str=="元")setfillcolor(RGB(247,218,138));
		else if(str=="明")setfillcolor(RGB(244,205,102));
		else if(str=="清")setfillcolor(RGB(249,203,80));
		else if(str=="民")setfillcolor(RGB(249,201,72));
		else if(str=="now")setfillcolor(RGB(249,198,63));
		else setfillcolor(RGB(204,190,179));
	} else if(mode==2) {
		str=change_its(a);
		setbkmode(1),settextcolor(WHITE);
		if(str=="2")setfillcolor(RGB(236,227,226)),settextcolor(BLACK);
		else if(str=="4")setfillcolor(RGB(233,219,178)),settextcolor(BLACK);
		else if(str=="8")setfillcolor(RGB(238,171,130));
		else if(str=="16")setfillcolor(RGB(250,147,83));
		else if(str=="32")setfillcolor(RGB(250,114,83));
		else if(str=="64")setfillcolor(RGB(243,82,46));
		else if(str=="128")setfillcolor(RGB(247,218,138));
		else if(str=="256")setfillcolor(RGB(244,205,102));
		else if(str=="512")setfillcolor(RGB(249,203,80));
		else if(str=="1024")setfillcolor(RGB(249,201,72));
		else if(str=="2048")setfillcolor(RGB(249,198,63));
		else setfillcolor(RGB(204,190,179));
	}
	solidrectangle(x,y,x+4,y+4);
	if(str!="  "&&str!="0")outtextxy(x+2-(str.size()/4),y+2,str);
}

void init() {
	nte[0]="   ",nte[2]="RI ",nte[4]="CE ",nte[8]="UKE";
	nte[16]="TLE",nte[32]="MLE",nte[64]="OLE",nte[128]="RE ";
	nte[256]="WA ",nte[512]="PC ",nte[1024]="AC ",nte[2048]="AK ";
	nte2[0]="   ",nte2[2]="商",nte2[4]="周",nte2[8]="秦";
	nte2[16]="汉",nte2[32]="唐",nte2[64]="宋",nte2[128]="元";
	nte2[256]="明",nte2[512]="清",nte2[1024]="民",nte2[2048]="now";
	initgraph(size*5+10,size*5+10,false),srand(time(0));
}

int check() {
	int s=0;
	for(int i=1; i<=size; i++)for(int j=1; j<=size; j++)if(Map[i][j]==2048)return -1;
	for(int i=1; i<=size; i++)for(int j=1; j<=size; j++)
			if(Map[i][j]==Map[i-1][j]||Map[i][j]==Map[i+1][j]
			        ||Map[i][j]==Map[i][j-1]||Map[i][j]==Map[i][j+1])return 1;
			else if(Map[i][j])s++;
	return s!=size*size;
}

void op_input(char x) {
	if(x!='w'&&x!='s'&&x!='a'&&x!='d')return;
	queue <int> q[10],ans[10];
	bool move=false;
	if(x=='w')for(int i=1; i<=size; i++)for(int j=1; j<=size; j++)if(Map[j][i])q[i].push(Map[j][i]);
	if(x=='s')for(int i=1; i<=size; i++)for(int j=size; j>=1; j--)if(Map[j][i])q[i].push(Map[j][i]);
	if(x=='a')for(int i=1; i<=size; i++)for(int j=1; j<=size; j++)if(Map[i][j])q[i].push(Map[i][j]);
	if(x=='d')for(int i=1; i<=size; i++)for(int j=size; j>=1; j--)if(Map[i][j])q[i].push(Map[i][j]);
	for(int i=1; i<=size; i++)for(int j=1; j<=size; j++)Map2[i][j]=Map[i][j];
	memset(Map,0,sizeof(Map));
	for(int i=1; i<=size; i++) {
		int top1,top2,pa;
		if(!q[i].empty()) {
			top1=q[i].front(),q[i].pop();
			while(!q[i].empty()) {
				top2=q[i].front(),q[i].pop();
				if(top1==top2) {
					score+=top1+top2;
					best=max(best,score);
					ans[i].push(top1*2),top1=0;
					if(!q[i].empty())top1=q[i].front(),q[i].pop();
					else break;
				} else ans[i].push(top1),top1=top2;
			}
			ans[i].push(top1);
		}
		if(x=='w'||x=='a')pa=0;
		else pa=size+1;
		if(x=='w')while(!ans[i].empty())Map[++pa][i]=ans[i].front(),ans[i].pop();
		if(x=='s')while(!ans[i].empty())Map[--pa][i]=ans[i].front(),ans[i].pop();
		if(x=='a')while(!ans[i].empty())Map[i][++pa]=ans[i].front(),ans[i].pop();
		if(x=='d')while(!ans[i].empty())Map[i][--pa]=ans[i].front(),ans[i].pop();
	}
	for(int i=1; i<=size&&!move; i++)for(int j=1; j<=size; j++)
			if(Map[i][j]!=Map2[i][j]) {
				move=true;
				break;
			}
	if(move) {
		int x2=rand()%size+1,y=rand()%size+1;
		while(Map[x2][y])x2=rand()%size+1,y=rand()%size+1;
		Map[x2][y]=2;
	}
}

void print_game() {
	if(mode==0)setbkcolor(BLACK);
	else if(mode==1)setbkcolor(RGB(248,244,234));
	else if(mode==2)setbkcolor(RGB(248,244,234));
	cleardevice();
	if(mode==0)setfillcolor(RGB(30,30,30)),settextcolor(RGB(150,150,150));
	if(mode==1)setfillcolor(RGB(173,156,135)),settextcolor(RGB(203,194,173));
	if(mode==2)setfillcolor(RGB(173,156,135)),settextcolor(RGB(203,194,173));
	solidrectangle(winsize-10,2,winsize-6,4);
	solidrectangle(winsize-16,2,winsize-12,4);
	solidrectangle(winsize-22,2,winsize-18,4);
	setbkmode(1);
	outtextxy(winsize-9,2," score");
	outtextxy(winsize-15,2," best");
	outtextxy(winsize-21,2," mode");
	outtextxy(winsize/2-10,winsize-1,"Space 更改模式  Use Space to change mode");
}

int main() {
	ifstream fr;
	ofstream fw;
	init();
	Map[rand()%size+1][rand()%size+1]=2;
	memset(Map2,-1,sizeof(Map2));
	settitle("2048 For Programmer");
	print_game();
	fr.open("2048saves.txt");
	fr>>best;
	fr.close();
	do {
		settextcolor(WHITE);
		string s=change_its(score),b=change_its(best),m;
		if(mode==0)m="程序员";
		else if(mode==1)m="朝代版";
		else if(mode==2)m="数字版";
		outtextxy(winsize-8-(s.size()/4),4,s+' ');
		outtextxy(winsize-14-(b.size()/4),4,b+' ');
		outtextxy(winsize-21,4,m);
		for(int i=1; i<=size; i++)for(int j=1; j<=size; j++)
				if(Map[i][j]!=Map2[i][j])EvaluationStatus(j*5,i*5+2,Map[i][j],mode);
		char get=getch();
		if(get==' ') {
			mode=(mode+1)%3;
			print_game();
			memset(Map2,-1,sizeof(Map2));
		} else op_input(tolower(get));
	} while(check()==1);
	settextcolor(WHITE);
	string s=change_its(score),b=change_its(best),m;
	if(mode==0)m="程序员";
	else if(mode==1)m="朝代版";
	else if(mode==2)m="数字版";
	outtextxy(winsize-8-(s.size()/4),4,s+' ');
	outtextxy(winsize-14-(b.size()/4),4,b+' ');
	outtextxy(winsize-21,4,m);
	for(int i=1; i<=size; i++)for(int j=1; j<=size; j++)
			EvaluationStatus(j*5,i*5+2,Map[i][j],mode);
	setbkmode(0),setbkcolor(BLUE);
	fw.open("2048saves.txt");
	fw<<best;
	fw.close();
	if(check()==-1)outtextxy(12,15,"You win!"),Sleep(1000000);
	else outtextxy(10,15,"Emmm.You failed..."),Sleep(1000000);
}
