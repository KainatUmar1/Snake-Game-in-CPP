#include<windows.h>
#include<conio.h>
#include<iostream>
#include<string>

using namespace std;

void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOuput, scrn);
}
void SetClr(int clr)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), clr);
}
void SetClr2(int tcl, int bcl)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (tcl + (bcl * 16)));
}

enum direction{UP,DOWN,LEFT,RIGHT};

struct position
{
	int ri, ci;
};

struct snake
{
	position* ps{};
	int size=0,score=0;
	int leftKey=0, rightKey=0, upKey=0, downKey=0;
	char sym=' ';
	string p_name;
	direction dir=RIGHT;
};

struct food
{
	position fp;
	bool isAlive;
	int score;
	char sym;
};

bool isLegalCoordinate(position p, snake& s)
{
	for (int i = 0; i < s.size; i++)
	{
		if (s.ps[i].ri == p.ri && s.ps[i].ci == p.ci)
			return false;
	}
	return true;
}

void generateFood(food& f, snake& s, int h, int w)
{
	do
	{
		f.fp.ri = rand() % h + 1;
		f.fp.ci = rand() % w + 1;
	} while (!isLegalCoordinate(f.fp, s));
	f.isAlive = true;
	f.score = 1;
}

void Box(int h, int w)
{
	char sym = -37;
	SetClr(3);
	for (int i = 0; i < h; h--)
	{
		gotoRowCol(h, i);
		cout << sym;
	}
	for (int i = 0; i < w; i++)
	{
		gotoRowCol(w,i+1);
		cout << sym;
	}
	for (int i = 0; i < w; i++)
	{
		gotoRowCol(i, w);
		cout << sym;
	}
	for (int i = 0; i < w; w--)
	{
		gotoRowCol(i,w-1);
		cout << sym;
	}
}

void Box1(int row, int col, int para, int n)
{
	char sym = -37;
	for (int i = 0; i < para * n; i++)
	{
		gotoRowCol(row, col++);
		cout << sym;
	}

	for (int i = 0; i < para; i++)
	{
		gotoRowCol(row++, col);
		cout << sym;
	}

	for (int i = 0; i < para * n; i++)
	{
		gotoRowCol(row, col--);
		cout << sym;
	}

	for (int i = 0; i < para; i++)
	{
		gotoRowCol(row--, col);
		cout << sym;
	}
}

void Box2(int h, int w)
{
	char ch = -37;
	Box(h, w);
	for (int i = 0; i <= h / 2; i++,h++)
	{
		gotoRowCol(h / 3, w / 2);
		cout << ch;
	}
	SetClr(15);
}

void Box3(int h, int w)
{
	int r = h;
	char ch = -37;
	Box(h, w);
	Box2(h, w);
	for (int i = 0; i <= w / 2; i++,w++)
	{
		gotoRowCol(h / 2, w / 3);
		cout << ch;
	}
	SetClr(15);
}

void table(int w)
{
	SetClr(3);
	for (int r = 2; r < 5; r++)
	{
		Box1(r * 4, w + 8, 8, 2);
		Box1(r * 4, w + 24, 8, 1);
	}
	SetClr(15);
}

void keyTable(snake& s, snake& t, snake& u, snake& v, int w)
{
	SetClr(3);
	for (int i = 1, r = 32; i < 4; i++, r += 4)
	{
		Box1(r, w, 8, 2);
		Box1(r, w + 16, 8, 1);
	}
	SetClr(15);
	gotoRowCol(30, w + 8);
	cout << "KEY TABLE";
	gotoRowCol(34, w + 3);
	cout << s.p_name;
	gotoRowCol(34, w + 18);
	cout << "ARROWS";
	gotoRowCol(38, w + 3);
	cout << t.p_name;
	gotoRowCol(38, w + 17);
	cout << "A-S-D-W";
	gotoRowCol(42, w + 3);
	cout << u.p_name;
	gotoRowCol(42, w + 17);
	cout << "J-K-L-I";
	gotoRowCol(46, w + 3);
	cout << v.p_name;
	gotoRowCol(46, w + 17);
	cout << "C-V-B-F";
}

void init(snake& s, snake& t, snake& u, snake& v, food& f, int h, int w)
{
	system("cls");
	s.ps = new position[3];
	for (int i = 0; i <= 2; i++)
	{
		s.ps[i].ri = 10;
		s.ps[i].ci = 10;
	}

	t.ps = new position[3];
	for (int i = 0; i <= 2; i++)
	{
		t.ps[i].ri = 10;
		t.ps[i].ci = w - 10;
	}

	u.ps = new position[3];
	for (int i = 0; i <= 2; i++)
	{
		u.ps[i].ri = h - 10 ;
		u.ps[i].ci = w - 10 ;
	}

	v.ps = new position[3];
	for (int i = 0; i <= 2; i++)
	{
		v.ps[i].ri = h - 10 ;
		v.ps[i].ci = 10;
	}

	s.size = 3, t.size = 3, u.size = 3, v.size = 3;
	s.score = 0, t.score = 0, u.score = 0, v.score = 0;
	s.sym = 'S', t.sym = 'T', u.sym = 'U', v.sym = 'V';

	s.rightKey = 77, s.leftKey = 75, s.upKey = 72, s.downKey = 80;
	t.rightKey = 100, t.leftKey = 97, t.upKey = 119, t.downKey = 115;
	u.rightKey = 108, u.leftKey = 106, u.upKey = 105, u.downKey = 107;
	v.rightKey = 98, v.leftKey = 99, v.upKey = 102, v.downKey = 118;

	s.dir = DOWN, t.dir = LEFT, u.dir = UP, v.dir = RIGHT;

	table(w);

	gotoRowCol(5, w + 13);
	cout << "Players";
	gotoRowCol(10, w + 10);
	cin >> s.p_name;
	gotoRowCol(14, w + 10);
	cin >> t.p_name;
	gotoRowCol(18, w + 10);
	cin >> u.p_name;
	gotoRowCol(22, w + 10);
	cin >> v.p_name;

	generateFood(f, s, h, w);
	f.sym = -37;
}

void displaySnake(const snake& s, const snake& t, const snake& u, const snake& v)
{
	for (int i = 0; i< s.size; i++)
	{
		SetClr(4);
		gotoRowCol(s.ps[i].ri, s.ps[i].ci);
		cout << s.sym;
	}

	for (int i = 0; i < t.size; i++)
	{
		SetClr(3);
		gotoRowCol(t.ps[i].ri, t.ps[i].ci);
		cout << t.sym;
	}

	for (int i = 0; i < u.size; i++)
	{
		SetClr(6);
		gotoRowCol(u.ps[i].ri, u.ps[i].ci);
		cout << u.sym;
	}

	for (int i = 0; i < v.size; i++)
	{
		SetClr(10);
		gotoRowCol(v.ps[i].ri, v.ps[i].ci);
		cout << v.sym;
	}
	SetClr(15);
}

void eraseSnake(const snake& s, const snake& t, const snake& u, const snake& v)
{
	for (int i = 0; i< s.size; i++)
	{
		gotoRowCol(s.ps[i].ri, s.ps[i].ci);
		cout << " ";
	}

	for (int i = 0; i < t.size; i++)
	{
		gotoRowCol(t.ps[i].ri, t.ps[i].ci);
		cout << " ";
	}

	for (int i = 0; i < u.size; i++)
	{
		gotoRowCol(u.ps[i].ri, u.ps[i].ci);
		cout << " ";
	}

	for (int i = 0; i < v.size; i++)
	{
		gotoRowCol(v.ps[i].ri, v.ps[i].ci);
		cout << " ";
	}
}

void moveSnake(snake& s, snake& t, snake& u, snake& v,int h,int w)
{
	for (int i = s.size - 1; i > 0; i--)
		s.ps[i] = s.ps[i - 1];

	for (int i = t.size - 1; i > 0; i--)
		t.ps[i] = t.ps[i - 1];

	for (int i = u.size - 1; i > 0; i--)
		u.ps[i] = u.ps[i - 1];

	for (int i = v.size - 1; i > 0; i--)
		v.ps[i] = v.ps[i - 1];

	switch (s.dir)
	{
	case UP:
		s.ps[0].ri--;
		if (s.ps[0].ri == 0)
			s.ps[0].ri = h -1;
		break;
	case DOWN:
		s.ps[0].ri++;
		if (s.ps[0].ri == h)
			s.ps[0].ri = 1;
		break;
	case LEFT:
		s.ps[0].ci--;
		if (s.ps[0].ci == 0)
			s.ps[0].ci = w-1;
		break;
	case RIGHT:
		s.ps[0].ci++;
		if (s.ps[0].ci == w)
			s.ps[0].ci = 1;
		break;
	}

	switch (t.dir)
	{
	case UP:
		t.ps[0].ri--;
		if (t.ps[0].ri == 0)
			t.ps[0].ri = h - 1;
		break;
	case DOWN:
		t.ps[0].ri++;
		if (t.ps[0].ri == h)
			t.ps[0].ri = 1;
		break;
	case LEFT:
		t.ps[0].ci--;
		if (t.ps[0].ci == 0)
			t.ps[0].ci = w - 1;
		break;
	case RIGHT:
		t.ps[0].ci++;
		if (t.ps[0].ci == w)
			t.ps[0].ci = 1;
		break;
	}

	switch (u.dir)
	{
	case UP:
		u.ps[0].ri--;
		if (u.ps[0].ri == 0)
			u.ps[0].ri = h - 1;
		break;
	case DOWN:
		u.ps[0].ri++;
		if (u.ps[0].ri == h)
			u.ps[0].ri = 1;
		break;
	case LEFT:
		u.ps[0].ci--;
		if (u.ps[0].ci == 0)
			u.ps[0].ci = w - 1;
		break;
	case RIGHT:
		u.ps[0].ci++;
		if (u.ps[0].ci == w)
			u.ps[0].ci = 1;
		break;
	}

	switch (v.dir)
	{
	case UP:
		v.ps[0].ri--;
		if (v.ps[0].ri == 0)
			v.ps[0].ri = h - 1;
		break;
	case DOWN:
		v.ps[0].ri++;
		if (v.ps[0].ri == h)
			v.ps[0].ri = 1;
		break;
	case LEFT:
		v.ps[0].ci--;
		if (v.ps[0].ci == 0)
			v.ps[0].ci = w - 1;
		break;
	case RIGHT:
		v.ps[0].ci++;
		if (v.ps[0].ci == w)
			v.ps[0].ci = 1;
		break;
	}
}

void changeDirection(snake& a, char key)
{
	if (key == a.leftKey)
		if (a.dir != RIGHT)
			a.dir = LEFT;
	if (key == a.rightKey)
		if (a.dir != LEFT)
			a.dir = RIGHT;
	if (key == a.upKey)
		if (a.dir != DOWN)
			a.dir = UP;
	if (key == a.downKey)
		if (a.dir != UP)
			a.dir = DOWN;	
}

void snakeCheck(snake& s, snake& t, snake& u, snake& v, char ch)
{
	if (ch == 97 || ch == 100 || ch == 115 || ch == 119)
		changeDirection(t, ch);
	if (ch == 72 || ch == 75 || ch == 77 || ch == 80)
		changeDirection(s, ch);
	if (ch == 105 || ch == 106 || ch == 107 || ch == 108)
		changeDirection(u, ch);
	if (ch == 98 || ch == 102 || ch == 118 || ch == 99)
		changeDirection(v, ch);
}

void displayFood(food f)
{
	SetClr(10);
	gotoRowCol(f.fp.ri, f.fp.ci);
	cout << f.sym;
	SetClr(15);
}

bool foodIsEaten(snake& s, food f)
{
	if (s.ps[0].ri == f.fp.ri && s.ps[0].ci == f.fp.ci)
		return true;
	return false;
}

void expandSnake(snake& s )
{
	position* hp = new position[s.size + 1];
	for (int i = 0; i < s.size; i++)
		hp[i] = s.ps[i];
	hp[s.size] = s.ps[s.size - 1];
	s.size++;
	delete[]s.ps;
	s.ps = hp;
}

void eatingFood(snake& s, snake& t, snake& u, snake& v, food& f, int h, int w)
{
	if (foodIsEaten(s, f))
	{
		generateFood(f, s, h, w);
		displayFood(f);
		expandSnake(s);
		s.score++;
	}

	if (foodIsEaten(t, f))
	{
		generateFood(f, t, h, w);
		displayFood(f);
		expandSnake(t);
		t.score++;
	}

	if (foodIsEaten(u, f))
	{
		generateFood(f, u, h, w);
		displayFood(f);
		expandSnake(u);
		u.score++;
	}

	if (foodIsEaten(v, f))
	{
		generateFood(f, v, h, w);
		displayFood(f);
		expandSnake(v);
		v.score++;
	}
}

void displayScore(snake &s, snake& t, snake& u, snake& v,int w)
{
	gotoRowCol(5, w);
	cout << " SCORES";

	gotoRowCol(10, w + 4);
	cout << s.score;

	gotoRowCol(14, w + 4);
	cout << t.score;

	gotoRowCol(18, w + 4);
	cout << u.score;

	gotoRowCol(22, w + 4);
	cout << v.score;
}

bool gameover(snake& s, int h, int w,int lvl)
{
	if (lvl == 2 && (s.ps[0].ci == w / 2) && (s.ps[0].ri >= h / 3 && s.ps[0].ri < 2 * h / 3))
		return true;
	if (lvl == 3 && (s.ps[0].ri == h / 2) && (s.ps[0].ci >= w / 3 && s.ps[0].ci <= 2 * w / 3))
		return true;
	
	switch (s.dir)
	{
	case UP:
		if (s.ps[0].ri == 1)
			return true;
	case DOWN:
		if (s.ps[0].ri == h - 1)
			return true;
	case LEFT:
		if (s.ps[0].ci == 0)
			return true;
	case RIGHT:
		if (s.ps[0].ci == w - 1)
			return true;
	}
	return false;
}

void endGame(int h,int w)
{
	gotoRowCol(h / 2, w / 2 -5);
	SetClr2(0, 15);
	cout << "GAME OVER...!!!";
	gotoRowCol(h + 10, w + 10);
	exit(1);
}

void check(snake& s, snake& t, snake& u, snake& v, int h, int w,int lvl)
{
	if (gameover(s, h, w,lvl))
	{
		s.size = 0;
		gotoRowCol(59, w + 18);
		SetClr(4);
		cout << s.p_name << " OUT!";
	}

	if (gameover(t, h, w,lvl))
	{
		t.size = 0;
		gotoRowCol(61, w + 18);
		SetClr(3);
		cout << t.p_name << " OUT!";
	}

	if (gameover(u, h, w,lvl))
	{
		u.size = 0;
		gotoRowCol(63, w + 18);
		SetClr(6);
		cout << u.p_name << " OUT!";
	}

	if (gameover(v, h, w,lvl))
	{
		v.size = 0;
		gotoRowCol(65, w + 18);
		SetClr(10);
		cout << v.p_name << " OUT!";
	}
	if (s.size == 0 && t.size == 0 && u.size == 0 && v.size == 0)
		endGame(h, w);
	SetClr(15);
}

void level(snake& s, snake& t, snake& u, snake& v, int h, int w,int &lvl)
{
	if (s.score == 5 || t.score == 5 || u.score == 5 || v.score == 5)
	{
		Box2(h, w);
		lvl = 2;
	}
	if (s.score == 10 || t.score == 10 || u.score == 10 || v.score == 10)
	{
		Box3(h, w);
		lvl = 3;
	}
}

int main()
{
	sndPlaySound(TEXT("game_sound2.wav"), SND_ASYNC);
	char ch;
	int height = 80, width = 80, lvl = 0, n = 0;
	snake s, t, u, v;
	food f;
	gotoRowCol(2, 2);
	cout << "Which game do you want to play?";
	gotoRowCol(4, 2);
	cout << "1- Boundary\t\t2- Duniya Gol Hai";
	gotoRowCol(5, 2);
	cin >> n;
	if (n == 1)
		lvl = 1;
	if (n == 2)
		lvl = 0;
	init(s, t, u, v, f, height, width);
	Box(height, width);
	displayFood(f);
	keyTable(s, t, u, v, width + 8);
	while (true)
	{
		if (_kbhit())
		{
			ch = _getch();
			snakeCheck(s, t, u, v, ch);
		}
		displayScore(s, t, u, v, width + 24);
		displaySnake(s, t, u, v);
		Sleep(100);
		eraseSnake(s,t,u,v);
		moveSnake(s, t, u, v, height, width);
		eatingFood(s, t, u, v, f, height - 1, width - 1);
		if (lvl != 0)
			check(s, t, u, v, height, width, lvl);
		level(s, t, u, v, height, width, lvl);
	}
	return 0;
}
