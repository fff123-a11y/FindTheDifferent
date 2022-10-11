#include <stdio.h>
#include <easyx.h>
#include <conio.h>
#pragma comment (lib, "winmm.lib")

RECT diffRects[3]{
	{120, 150, 150 + 120, 150 + 130 },
	{490, 160, 490 + 40, 160 + 125 },
	{495, 360, 495 + 70, 360 + 65 },
};

bool marks[3] = { false };

//绘制叉叉的相关数据
int fx;
int fy;
bool isShow = false;

IMAGE img_title;
IMAGE img_diff[3];
IMAGE img_fork[2];

void loadResource() {
	//加载图片
	loadimage(&img_title, "res/images/title.jpg");
	loadimage(img_diff + 0, "res/images/diff0.jpg");
	loadimage(img_diff + 1, "res/images/diff1.jpg");
	loadimage(img_diff + 2, "res/images/diff2.jpg");
	
	loadimage(img_fork + 0, "res/images/cross_mask.jpg", 50, 50);
	loadimage(img_fork + 1, "res/images/cross_src.jpg", 50, 50);
}

void draw() {
	//输出图片
	putimage(0, 0, &img_title);
	putimage(0, 100, &img_diff[3] + 0);

	//设置线条颜色
	setlinecolor(BLUE);
	//设置线条的样式
	setlinestyle(PS_SOLID, 2);
	//绘制矩形
	for (int i = 0; i < 3; i++) {
		if (marks[i] == false) 
			continue;
		//左边
		rectangle(diffRects[i].left, diffRects[i].top + 100, diffRects[i].right, diffRects[i].bottom + 100);
		//右边
		rectangle(diffRects[i].left + 600, diffRects[i].top + 100, diffRects[i].right + 600, diffRects[i].bottom + 100);
	}

	//绘制点错的叉叉
	if (isShow) {
		putimage(fx, fy, img_fork + 0, SRCPAINT);
		putimage(fx, fy, img_fork + 1, SRCAND);
	}
}

//判断鼠标是否在某个区域内
bool mouseInRect(int x, int y, RECT* rect) {
	//判断某个点在矩形里面
	if (x > rect->left && x < rect->right && y >rect->top && y < rect->bottom ||
		(x > rect -> left +600 && x < rect->right + 600 && y >rect->top&& y < rect->bottom)) {
		return true;
	}
	return false;
}

bool clickRight(int mx, int my) {
	for (int i = 0; i < 3; i++) {
		if (mouseInRect(mx, my, diffRects + i)) {
			return true;
		}
	}
	return false;
}
void onMouseLButtonDown(ExMessage* msg) {
	RECT r = { 120,150,120 + 150,150 + 130 };
	//判断点击的位置是否在不同点上
	if (mouseInRect(msg->x, msg->y, &r)) {
		printf("you good\n");
		isShow = false;
		PlaySound("res/music/chenggong.wav", NULL, SND_ASYNC | SND_FILENAME);
	}
	else {
		isShow = true;
		fx = msg->x;
		fy = msg->y;
		PlaySound("res/music/baocuo.wav", NULL, SND_ASYNC | SND_FILENAME);
	}
}

int main() {
	//创建窗口
	initgraph(1200, 600, EW_SHOWCONSOLE);
	loadResource();

	//游戏是死循环
	while (true) {
		
		BeginBatchDraw();
		
		draw();

		//处理鼠标消息
		ExMessage msg; //结构体变量
		//抓消息
		while (peekmessage(&msg, EM_MOUSE)) {
			//判断一下是不是鼠标左键点击
			if (msg.message == WM_LBUTTONDOWN) {
				onMouseLButtonDown(&msg);
			}
		}
	}

	getchar();
	return 0;
}
