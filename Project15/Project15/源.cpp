#include<conio.h>
#include<graphics.h>
#include<easyx.h>
#include<time.h>
using namespace std;


#define ENEMY_NUM 10

/*
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
*/
#define SUCCESS 0
#define FAILURE 1

enum DIRECTION {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

/*坦克*/
struct tank_s {
	int x;
	int y;
	DIRECTION direction;
	int live;
};

/*子弹*/
struct bullet_s {
	int pos_x;
	int pos_y;
	DIRECTION direction;
	int status;/*状态*/
};

void menu();//菜单绘制
void init_map();//地图绘制
void set_prop_map(int x, int y, int val);//地图状态改变
void tank_walk(tank_s* tank, DIRECTION directon, IMAGE* img);//坦克移动控制
int do_tank_walk(tank_s* tank, DIRECTION direction, IMAGE* img, int step);//坦克移动执行
DIRECTION enemy_direction(tank_s* tank, int x, int y);//敌方坦克移动方向控制
void tank_fire(tank_s* tank, bullet_s* bullet, int need_sound, int friend_nummber);//坦克开火控制
int bullet_action(bullet_s* bullet, tank_s* enemy_tank, tank_s* my_tank);//坦克子弹运行
void play();//总游玩控制

int map[26][26] =
{
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 },
	{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 },
	{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 },
	{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 },
	{ 0,0,1,1,0,0,1,1,0,0,1,1,2,2,1,1,0,0,1,1,0,0,1,1,0,0 },
	{ 0,0,1,1,0,0,1,1,0,0,1,1,2,2,1,1,0,0,1,1,0,0,1,1,0,0 },
	{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 },
	{ 0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0 },
	{ 0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0 },
	{ 1,1,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,1,1 },
	{ 2,2,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,2,2 },
	{ 0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,1,1,0,0,1,1,0,0,1,1,1,1,1,1,0,0,1,1,0,0,1,1,0,0 },
	{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 },
	{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 },
	{ 0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0,1,1,0,0 },
	{ 0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0 },
	{ 0,0,1,1,0,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,0,0 },
	{ 0,0,1,1,0,0,1,1,0,0,0,1,1,1,1,0,0,0,1,1,0,0,1,1,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,1,3,3,1,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,1,3,3,1,0,0,0,0,0,0,0,0,0,0,0 } };

/*菜单*/

void set_prop_map(int x, int y, int val) {
	map[y][x] = val;
	map[y + 1][x] = val;
	map[y][x + 1] = val;
	map[y + 1][x + 1] = val;
}

void menu() {
	IMAGE logo_img;
	loadimage(&logo_img, _T("logo.bmp"), 433, 147);
	putimage(110, 20, &logo_img);
	//导航
	setlinecolor(WHITE);
	setfillcolor(BLACK);

	fillrectangle(230, 200, 310, 240);
	settextstyle(25, 0, _T("宋体"));
	outtextxy(240, 210, _T("说 明"));

	fillrectangle(350, 200, 430, 240);
	outtextxy(360, 210, _T("开 始"));

	MOUSEMSG mouse;
	IMAGE illustrate_img;
	loadimage(&illustrate_img, _T("illustrate.jpg"), 300, 300);

	int finished = 0;

	while (!finished) {
		mouse = GetMouseMsg();

		switch (mouse.uMsg) {
		case WM_MOUSEMOVE:
			if (mouse.x > 230 && mouse.x < 310 && mouse.y>200 && mouse.y < 240) {
				putimage(150, 250, &illustrate_img);
			}
			else {
				solidrectangle(150, 250, 450, 550);
			}
		case WM_LBUTTONDOWN:
			if (mouse.x > 350 && mouse.x < 430 && mouse.y>200 && mouse.y < 240) {
				cleardevice();
				return;
			}
		}
	}
}
/*地图*/ /*墙1和2相反*/
void init_map() {
	IMAGE img_home1, img_wall1, img_wall2;
	loadimage(&img_home1, _T("home1.jpg"), 50, 50);
	loadimage(&img_wall1, _T("wall1.jpg"), 25, 25);
	loadimage(&img_wall2, _T("wall2.jpg"), 25, 25);

	for (int i = 0;i <= 25;i++) {
		for (int j = 0;j <= 25;j++) {
			if (map[i][j] == 1) {
				putimage(25 * j, 25 * i, &img_wall2);
			}
			else {
				if (map[i][j] == 2) {
					putimage(25 * j, 25 * i, &img_wall1);
				}
				else {
					if (map[i][j] == 3) {
						putimage(25 * j, 25 * i, &img_home1);

						set_prop_map(j, i, 4);
					}
				}
			}
		}
	}
}

void init_map_2(int* map, int rows, int cols);


int main(void) {
	initgraph(666, 666);

	menu();

	init_map();

	system("pause");
}