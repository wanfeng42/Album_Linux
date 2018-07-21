#include "heads.h"

/** 
 * 返回屏幕操作；
**/
int getDirection()
{
	int x1 = -1, y1 = -1;
	int x2, y2;

	int fd = open(TOUCH_DEV, O_RDONLY);
	if (-1 == fd)
	{
		perror("open touch device fail");
		return -1;
	}
	while (1)
	{
		struct input_event ev;
		int r = read(fd, &ev, sizeof(ev));
		if (r != sizeof(ev))
			continue;
		if (EV_ABS == ev.type && ABS_X == ev.code) //发过来的是X坐标
		{
			if (-1 == x1)
				x1 = ev.value;
			x2 = ev.value;
			//printf("x1 = %d, y1=%d\n",x1,y1);
			//fprintf(stderr, "tauch : Line %d x1 = %d x2 = %d\n", __LINE__, x1, x2);
		}
		if (EV_ABS == ev.type && ABS_Y == ev.code)
		{
			if (-1 == y1)
				y1 = ev.value;
			y2 = ev.value;
			//fprintf(stderr, "tauch : Line %d  y1= %d y2 = %d\n", __LINE__, y1, y2);
		}
		//if( ( EV_ABS == ev.type ) && ( ABS_PRESSURE == ev.code ) && ( 0==ev.value) )
		if ((EV_KEY == ev.type) && (0x14a == ev.code) && (0 == ev.value))
		{
			int deltx = abs(x2 - x1);
			int delty = abs(y2 - y1) > 480 ? 80 : abs(y2 - y1);
			//fprintf(stderr, "tauch : Line %d deltx = %d delay = %d\n", __LINE__, deltx, delty);
			if (deltx > delty) //这种情况下只返回左或者右
			{
				if (deltx > 90) //90是判定是否滑动的阈值
				{
					close(fd);
					if (x2 > x1)
						return MOVE_RIGHT;
					else
						return MOVE_LEFT;
				}
				else
				{
					x1 = y1 = -1;
					if (x2 > 0 && x2 < 267 && y2 > 160 && y2 < 320)
					{
						return GO_LEFT;
					}
					if (x2 > 534 && x2 < 800 && y2 > 160 && y2 < 320)
					{
						return GO_RIGHT;
					}
					if (x2 > 267 && x2 < 534 && y2 > 0 && y2 < 160)
					{
						return GO_UP;
					}
					if (x2 > 267 && x2 < 534 && y2 > 320 && y2 < 480)
					{
						return GO_DOWN;
					}
				}
			}
			else
			{ // 只返回上或者下
				if (delty > 90)
				{
					close(fd);
					if (y2 > y1)
						return MOVE_DOWN;
					else
						return MOVE_UP;
				}
				else
				{
					x1 = y1 = -1;
					if (x2 > 0 && x2 < 267 && y2 > 160 && y2 < 320)
					{
						return GO_LEFT;
					}
					if (x2 > 534 && x2 < 800 && y2 > 160 && y2 < 320)
					{
						return GO_RIGHT;
					}
					if (x2 > 267 && x2 < 534 && y2 > 0 && y2 < 160)
					{
						return GO_UP;
					}
					if (x2 > 267 && x2 < 534 && y2 > 320 && y2 < 480)
					{
						return GO_DOWN;
					}
				}
			}
		}
	}
}