#include<conio.h>
#include<graphics.h>
#include<easyx.h>
#include<time.h>
using namespace std;

/*******************
* �������ݿ����޸�
********************/
/*��������Ƿ�������1������0�ر�*/

#define ENEMY_NUM 10//�з�������ȫ������ʤ����������3��
int tank_arrive_time = 500;//�з�̹�˳���ʱ����
int my_tanks_live = 1;//����̹������ֵ
int enemy_tanks_live = 1;//�з�̹������ֵ
int home_defence = 0;//�����Ƿ��޵�
int tk = 0;//�Ƿ�������

/*������
#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3
*/

enum DIRECTION {
	UP,
	DOWN,
	LEFT,
	RIGHT
};

/*******************
* ��ʹ�õĽṹ��
********************/

/*̹��*/
struct tank_s {
	int x;
	int y;
	DIRECTION direction;
	int live;
};

/*�ӵ�*/
struct bullet_s {
	int pos_x;
	int pos_y;
	DIRECTION direction;
	int status;/*״̬*/
	int friend_number;//����ʶ��1Ϊ������0Ϊ�з�
};

/*******************
* ��ʹ�õĺ���
********************/

void menu();//�˵�����
void init_map();//��ͼ����
void set_prop_map(int x, int y, int val);//��ͼ״̬�ı�
void tank_walk(tank_s* tank, DIRECTION directon, IMAGE* img);//̹���ƶ�����
int do_tank_walk(tank_s* tank, DIRECTION direction, IMAGE* img, int step);//̹���ƶ�ִ��
DIRECTION enemy_direction(tank_s* tank, int x, int y);//�з�̹���ƶ��������
void tank_fire(tank_s* tank, bullet_s* bullet, int need_sound, int friend_nummber);//̹�˿������
int bullet_action(bullet_s* bullet, tank_s* enemy_tank, tank_s* my_tank);//̹���ӵ�����
void play();//���������

/*******************
* ��ͼ
********************/
int h = 1 + home_defence;
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
	{ 0,0,1,1,0,0,1,1,0,0,0,h,h,h,h,0,0,0,1,1,0,0,1,1,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,h,3,3,h,0,0,0,0,0,0,0,0,0,0,0 },
	{ 0,0,0,0,0,0,0,0,0,0,0,h,3,3,h,0,0,0,0,0,0,0,0,0,0,0 } };

/*******************
* ���庯��
********************/

/*�����з�����*/
DIRECTION enemy_direction(tank_s* tank, int x, int y) {
	int r = rand() % 100;

	if (tank->x > x) {
		if (tank->y > y) {//Ŀ�������Ϸ�
			if (r <= 50)return UP;
			else return LEFT;
		}
		else {//Ŀ�������·�
			if (r <= 50)return DOWN;
			else return LEFT;
		}
	}
	else {
		if (tank->y > y) {//Ŀ�������Ϸ�
			if (r <= 50)return UP;
			else return RIGHT;
		}
		else {//Ŀ�������·�
			if (r <= 50)return DOWN;
			else return RIGHT;
		}
	}
}



/*�˵�*/

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
	//����
	setlinecolor(WHITE);
	setfillcolor(BLACK);

	fillrectangle(230, 200, 310, 240);
	settextstyle(25, 0, _T("����"));
	outtextxy(240, 210, _T("˵ ��"));

	fillrectangle(350, 200, 430, 240);
	outtextxy(360, 210, _T("�� ʼ"));

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
/*��ͼ*/ /*ͼ���ļ���ǽ1��2�෴*/
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

/*������ƣ���Ч��������0����Ч��������1*/
int do_tank_walk(tank_s* tank, DIRECTION direction, IMAGE* img, int step) {
	int new_x = tank->x;
	int new_y = tank->y;

	int old_prop = map[tank->y][tank->x];

	if (step == 1) {
		if (direction == UP) {
			new_y -= 1;
		}
		else if (direction == DOWN) {
			new_y += 1;
		}
		else if (direction == LEFT) {
			new_x -= 1;
		}
		else if (direction == RIGHT) {
			new_x += 1;
		}
		else {
			return 0;
		}
		set_prop_map(tank->x, tank->y, 0);
	}

	setfillcolor(BLACK);
	solidrectangle(tank->x * 25, tank->y * 25, tank->x * 25 + 50, tank->y * 25 + 50);

	if (step == 1) {
		set_prop_map(new_x, new_y, old_prop);

		tank->x = new_x;
		tank->y = new_y;
	}

	putimage(tank->x * 25, tank->y * 25, img);
	return 1;
}

/*�ƶ�����*/
void tank_walk(tank_s* tank, DIRECTION directon, IMAGE* img) {
	switch (directon) {
	case LEFT:
		if (tank->direction == LEFT && tank->x - 1 >= 0 && map[tank->y][tank->x - 1] == 0 && map[tank->y + 1][tank->x - 1] == 0) {
			do_tank_walk(tank, LEFT, img, 1);
		}
		else if (tank->direction != LEFT) {
			tank->direction = LEFT;
			do_tank_walk(tank, LEFT, img, 0);
		}
		break;
	case UP:
		if (tank->direction == UP && tank->y - 1 >= 0 && map[tank->y - 1][tank->x] == 0 && map[tank->y - 1][tank->x + 1] == 0) {
			do_tank_walk(tank, UP, img, 1);
		}
		else if (tank->direction != UP) {
			tank->direction = UP;
			do_tank_walk(tank, UP, img, 0);
		}
		break;
	case DOWN:
		if (tank->direction == DOWN && tank->y + 2 <= 25 && map[tank->y + 2][tank->x] == 0 && map[tank->y + 2][tank->x + 1] == 0) {
			do_tank_walk(tank, DOWN, img, 1);
		}
		else if (tank->direction != DOWN) {
			tank->direction = DOWN;
			do_tank_walk(tank, DOWN, img, 0);
		}
		break;
	case RIGHT:
		if (tank->direction == RIGHT && tank->x + 2 <= 25 && map[tank->y][tank->x + 2] == 0 && map[tank->y + 1][tank->x + 2] == 0) {
			do_tank_walk(tank, RIGHT, img, 1);
		}
		else if (tank->direction != RIGHT) {
			tank->direction = RIGHT;
			do_tank_walk(tank, RIGHT, img, 0);
		}
		break;
	}
}

int bullet_action(bullet_s* bullet, tank_s* enemy_tank, tank_s* my_tank) {
	int x, y, x1, y1;

	x = bullet->pos_x / 25;
	y = bullet->pos_y / 25;

	/*�����һ�λ��Ƶ��ӵ�*/
	setfillcolor(BLACK);
	solidrectangle(bullet->pos_x, bullet->pos_y, bullet->pos_x + 3, bullet->pos_y + 3);

	if (bullet->direction == UP) {
		bullet->pos_y -= 2;
		x1 = x + 1;
		y1 = y;
	}
	else if (bullet->direction == DOWN) {
		bullet->pos_y += 2;
		x1 = x + 1;
		y1 = y;
	}
	else if (bullet->direction == LEFT) {
		bullet->pos_x -= 2;
		x1 = x;
		y1 = y + 1;
	}
	else if (bullet->direction == RIGHT) {
		bullet->pos_x += 2;
		x1 = x;
		y1 = y + 1;
	}
	else {
		return 0;
	}

	if (bullet->pos_x < 0 || bullet->pos_x > 650 || bullet->pos_y < 0 || bullet->pos_y > 650) {
		bullet->status = 0;
		return 0;
	}

	/*��ײ���*/
	if (map[y][x] == 4 || map[y1][x1] == 4) {//�з����л���
		bullet->status = 0;
		if (bullet->friend_number == 0 || tk == 1) {
			return 1;
		}
	}

	if (map[y][x] == 200 || map[y1][x1] == 200) {//�з����м���̹��
		if (bullet->friend_number == 0 || tk == 1) {
			my_tank->live--;
		}
		bullet->status = 0;
		if (my_tank->live == 0) {
			return 1;
		}
	}

	//�������ез�̹��
	if (map[y][x] >= 100 && map[y][x] <= 199 || map[y1][x1] >= 100 && map[y1][x1] <= 199) {
		tank_s* tank = NULL;
		bullet->status = 0;

		if (bullet->friend_number == 1 || tk == 1) {
			if (map[y][x] >= 100 && map[y][x] <= 199) {
				tank = enemy_tank + (map[y][x] - 100);
			}
			else tank = enemy_tank + (map[y][x] - 100);

			tank->live--;
			if (tank->live == 0) {
				set_prop_map(tank->x, tank->y, 0);
				setfillcolor(BLACK);
				solidrectangle(tank->x * 25, tank->y * 25, tank->x * 25 + 50, tank->y * 25 + 50);
			}
		}
	}

	//����ǽ
	if (map[y][x] == 1) {
		map[y][x] = 0;
		bullet->status = 0;
		setfillcolor(BLACK);
		solidrectangle(x * 25, y * 25, x * 25 + 25, y * 25 + 25);
	}
	else if (map[y][x] == 2) {
		bullet->status = 0;
	}
	if (map[y1][x1] == 1) {
		map[y1][x1] = 0;
		bullet->status = 0;
		setfillcolor(BLACK);
		solidrectangle(x1 * 25, y1 * 25, x1 * 25 + 25, y1 * 25 + 25);
	}
	else if (map[y1][x1] == 2) {
		bullet->status = 0;
	}

	//���»����ӵ�
	if (bullet->status == 1) {
		setfillcolor(WHITE);
		solidrectangle(bullet->pos_x, bullet->pos_y, bullet->pos_x + 3, bullet->pos_y + 3);
	}
}

void tank_fire(tank_s* tank, bullet_s* bullet, int need_sound, int friend_number) {
	if (bullet->status == 0) {
		if (tank->direction == UP) {
			bullet->pos_x = tank->x * 25 + 23;
			bullet->pos_y = tank->y * 25 - 3;
		}
		else if (tank->direction == LEFT) {
			bullet->pos_x = tank->x * 25 - 3;
			bullet->pos_y = tank->y * 25 + 23;
		}
		else if (tank->direction == DOWN) {
			bullet->pos_x = tank->x * 25 + 23;
			bullet->pos_y = tank->y * 25 + 50;
		}
		else if (tank->direction == RIGHT) {
			bullet->pos_x = tank->x * 25 + 50;
			bullet->pos_y = tank->y * 25 + 23;
		}
		bullet->direction = tank->direction;
		bullet->status = 1;
		bullet->friend_number = friend_number;
	}
}

/*����*//*ʤ������1��ʧ�ܷ���0*/
void play() {
	IMAGE img;

	tank_s my_tank;
	bullet_s my_bullet;/*����*/

	tank_s enemy_tank[ENEMY_NUM];
	bullet_s enemy_bullet[ENEMY_NUM];/*�з�*/

	IMAGE my_tank_img[4];
	IMAGE enemy_tank_img[4];
	int key;
	int times = 0;/*��¼��ǰ�������ߴ�����ÿ��10ms*/
	int enemy_total = 0;

	loadimage(&my_tank_img[UP], _T("tank_up.jpg"), 50, 50);
	loadimage(&my_tank_img[DOWN], _T("tank_down.jpg"), 50, 50);
	loadimage(&my_tank_img[LEFT], _T("tank_left.jpg"), 50, 50);
	loadimage(&my_tank_img[RIGHT], _T("tank_right.jpg"), 50, 50);

	loadimage(&enemy_tank_img[UP], _T("it1_up.jpg"), 50, 50);
	loadimage(&enemy_tank_img[DOWN], _T("it1_down.jpg"), 50, 50);
	loadimage(&enemy_tank_img[LEFT], _T("it1_left.jpg"), 50, 50);
	loadimage(&enemy_tank_img[RIGHT], _T("it1_right.jpg"), 50, 50);

	my_bullet.status = 0;

	//��ʼ�����������
	srand(time(NULL));

	/*����λ��*/
	my_tank.x = 8;
	my_tank.y = 24;
	my_tank.live = my_tanks_live;
	my_tank.direction = UP;
	set_prop_map(my_tank.x, my_tank.y, 200);

	/*�з�λ��*/
	for (int i = 0;i < ENEMY_NUM;i++) {
		if (i % 3 == 0) {
			enemy_tank[i].x = 0;
		}
		else if (i % 3 == 1) {
			enemy_tank[i].x = 12;
		}
		else if (i % 3 == 2) {
			enemy_tank[i].x = 24;
		}
		enemy_tank[i].y = 0;
		enemy_tank[i].live = enemy_tanks_live;
		set_prop_map(enemy_tank[i].x, enemy_tank[i].y, 100 + i);
		enemy_bullet[i].status = 0;
		enemy_tank[i].direction = DOWN;
	}

	do_tank_walk(&enemy_tank[0], DOWN, &enemy_tank_img[DOWN], 0);
	set_prop_map(enemy_tank[0].x, enemy_tank[0].y, 100);
	do_tank_walk(&enemy_tank[1], DOWN, &enemy_tank_img[DOWN], 0);
	set_prop_map(enemy_tank[1].x, enemy_tank[1].y, 101);
	do_tank_walk(&enemy_tank[2], DOWN, &enemy_tank_img[DOWN], 0);
	set_prop_map(enemy_tank[2].x, enemy_tank[2].y, 102);
	enemy_total = 3;

	putimage(my_tank.x * 25, my_tank.y * 25, &my_tank_img[my_tank.direction]);

	while (1) {
		if (times > 0 && times % tank_arrive_time == 0 && enemy_total < ENEMY_NUM) {
			//�жϳ���λ���Ƿ���̹�˴���
			set_prop_map(enemy_tank[enemy_total].x, enemy_tank[enemy_total].y, 100 + enemy_total);
			enemy_total++;
		}
		/*�з��ƶ�*/
		if (times % 200 == 0) {
			for (int i = 0;i < enemy_total;i++) {
				if (enemy_tank[i].live == 0)continue;
				if (i % 2 == 0) {//Ŀ�꼺������
					DIRECTION d = enemy_direction(&enemy_tank[i], 12, 24);
					tank_walk(&enemy_tank[i], d, &enemy_tank_img[d]);
				}
				else {//Ŀ�꼺��̹��
					DIRECTION d = enemy_direction(&enemy_tank[i], my_tank.x, my_tank.y);
					tank_walk(&enemy_tank[i], d, &enemy_tank_img[d]);
				}

				tank_fire(&enemy_tank[i], &enemy_bullet[i], 0, 0);
			}
		}
		else if (times % 50 == 0) {
			for (int i = 0;i < enemy_total;i++) {
				if (enemy_tank[i].live) {
					tank_walk(&enemy_tank[i], enemy_tank[i].direction, &enemy_tank_img[enemy_tank[i].direction]);
				}
			}
		}

		/*�����ƶ�*/
		if (_kbhit()) {
			key = _getch();

			switch (key) {
			case'a':
				tank_walk(&my_tank, LEFT, &my_tank_img[LEFT]);
				break;
			case'w':
				tank_walk(&my_tank, UP, &my_tank_img[UP]);
				break;
			case's':
				tank_walk(&my_tank, DOWN, &my_tank_img[DOWN]);
				break;
			case'd':
				tank_walk(&my_tank, RIGHT, &my_tank_img[RIGHT]);
				break;
			case'j':
				tank_fire(&my_tank, &my_bullet, 1, 1);
				break;
			case'p':
				system("pause");
				break;
			default:
				break;
			}
		}
		if (my_bullet.status == 1) {
			if (bullet_action(&my_bullet, enemy_tank, &my_tank) == 1) {
				cleardevice();
				loadimage(&img, _T("over.gif"), 500, 250);
				putimage(80, 200, &img);
				return;
			}
		}
		for (int i = 0;i < ENEMY_NUM;i++) {
			if (enemy_bullet[i].status == 1) {
				if (bullet_action(&enemy_bullet[i], enemy_tank, &my_tank) == 1) {
					cleardevice();
					loadimage(&img, _T("over.gif"), 500, 250);
					putimage(80, 200, &img);
					return;
				}
			}
		}
		//�з��Ƿ�ȫ��������
		int isWin = 0;
		for (int i = 0;i < ENEMY_NUM;i++) {
			if (enemy_tank[i].live == 0)isWin++;
		}
		if (isWin == ENEMY_NUM) {
			cleardevice();
			loadimage(&img, _T("pass.jpg"), 500, 250);
			putimage(80, 200, &img);
			return;
		}

		Sleep(10);
		times++;
	}
}

/*******************
* ����
********************/

int main(void) {
	initgraph(666, 666);

	menu();

	init_map();
	play();

	system("pause");
}