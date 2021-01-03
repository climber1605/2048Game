#include <iostream>
#include <iomanip>
#include <windows.h>
#include <ctime>
#include <cstdlib>
using namespace std;

const int rows = 4;
const int cols = 4;
const int target = 2048;

int board[rows][cols];
int score = 0;

// 刷新局面
void display() {
	system("cls");
	HANDLE consolehwnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consolehwnd, FOREGROUND_INTENSITY|FOREGROUND_GREEN|FOREGROUND_BLUE);
	cout << "SCORE: " << score << endl << endl;
	
	for (int i = 0; i < rows; i++) {
		SetConsoleTextAttribute(consolehwnd, FOREGROUND_INTENSITY|FOREGROUND_GREEN);
		cout << "+------+------+------+------+" << endl << "| ";
		for (int j = 0; j < 4; j++) {
			SetConsoleTextAttribute(consolehwnd, FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN);
			if (!board[i][j]) cout << setw( 4 ) << " ";
			else cout << setw( 4 ) << board[i][j];
			SetConsoleTextAttribute(consolehwnd, FOREGROUND_INTENSITY|FOREGROUND_GREEN);
			cout << " | ";
		}
		cout << endl;
	}
	
	SetConsoleTextAttribute(consolehwnd, FOREGROUND_INTENSITY|FOREGROUND_GREEN);
	cout << "+------+------+------+------+" << endl << endl;	
}

// 随机在空白位置生成一个新方块，以0.1的概率生成2，以0.9的概率生成4
void addTail() {
	int spaces[rows * cols][2];
	int cnt = 0;
	
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			if (!board[i][j]) {
				spaces[cnt][0] = i;
				spaces[cnt][1] = j;
				cnt++;
			}
			
	if (cnt == 0)  return;   
	int idx = rand() % cnt;
	int r = spaces[idx][0], c = spaces[idx][1];
	int s = rand() % 100;
	if (s < 90) 
		board[r][c] = 2;
	else
		board[r][c] = 4;
}

// 开局随机生成两个方块
void initialize() {
	addTail();
	addTail();
}

// 判断(r, c)位置的方块值是否为val，以此判断两方块能否合并
bool canMerge(int r, int c, int val) {
	return (r >= 0 && r < rows && c >= 0 && c < cols && board[r][c] == val);
}

// 将所有方块向上平移，不考虑合并
bool moveUpOnce() {
	bool moved = false;
	for (int j = 0; j < cols; j++) {
		for (int i = 1; i < rows; i++) {
			if (!board[i][j])
				continue;
			int k = i;
			for (; k > 0 && !board[k - 1][j]; k--)  ;
			if (k != i) {
				board[k][j] = board[i][j];
				board[i][j] = 0;
				moved = true;
			}
		}
	}
	return moved;
}

// 将所有方块向上合并
bool mergeUpOnce() {
	bool merged = false;
	for (int j = 0; j < cols; j++) {
		for (int i = 1; i < rows; i++) {
			if (board[i][j] && canMerge(i - 1, j, board[i][j])) {
				score += board[i][j];
				board[i - 1][j] += board[i][j];
				board[i][j] = 0;
				merged = true;
			}
		}
	}
	return merged;
}

// 将所有方块向上移动并合并
bool moveUp() { 
	bool moved = moveUpOnce();
	bool merged = mergeUpOnce();
	if (merged)  moveUpOnce();
	return moved || merged;
}

bool moveDownOnce() {
	bool moved = false;
	for (int j = 0; j < cols; j++) {
		for (int i = rows - 2; i >= 0; i--) {
			if (!board[i][j])
				continue;
			int k = i;
			for (; k + 1 < rows && !board[k + 1][j]; k++)  ;
			if (k != i) {
				board[k][j] = board[i][j];
				board[i][j] = 0;
				moved = true;
			}
		}
	}
	return moved;
}

bool mergeDownOnce() {
	bool merged = false;
	for (int j = 0; j < cols; j++) {
		for (int i = rows - 2; i >= 0; i--) {
			if (board[i][j] && canMerge(i + 1, j, board[i][j])) {
				score += board[i][j];
				board[i + 1][j] += board[i][j];
				board[i][j] = 0;
				merged = true;
			}
		}
	}
	return merged;
}

bool moveDown() { 
	bool moved = moveDownOnce();
	bool merged = mergeDownOnce();
	if (merged)  moveDownOnce();
	return moved || merged;
}

bool moveLeftOnce() {
	bool moved = false;
	for (int i = 0; i < rows; i++) {
		for (int j = 1; j < cols; j++) {
			if (!board[i][j])
				continue;
			int k = j;
			for (; k > 0 && !board[i][k - 1]; k--)  ;
			if (k != j) {
				board[i][k] = board[i][j];
				board[i][j] = 0;
				moved = true;
			}
		}
	}
	return moved;
}

bool mergeLeftOnce() {
	bool merged = false;
	for (int i = 0; i < rows; i++) {
		for (int j = 1; j < cols; j++) {
			if (board[i][j] && canMerge(i, j - 1, board[i][j])) {
				score += board[i][j];
				board[i][j - 1] += board[i][j];
				board[i][j] = 0;
				merged = true;
			}
		}
	}
	return merged;
}

bool moveLeft() { 
	bool moved = moveLeftOnce();
	bool merged = mergeLeftOnce();
	if (merged)  moveLeftOnce();
	return moved || merged;
}

bool moveRightOnce() {
	bool moved = false;
	for (int i = 0; i < rows; i++) {
		for (int j = cols - 2; j >= 0; j--) {
			if (!board[i][j])
				continue;
			int k = j;
			for (; k + 1 < cols && !board[i][k + 1]; k++)  ;
			if (k != j) {
				board[i][k] = board[i][j];
				board[i][j] = 0;
				moved = true;
			}
		}
	}
	return moved;
}

bool mergeRightOnce() {
	bool merged = false;
	for (int i = 0; i < rows; i++) {
		for (int j = cols - 2; j >= 0; j--) {
			if (board[i][j] && canMerge(i, j + 1, board[i][j])) {
				score += board[i][j];
				board[i][j + 1] += board[i][j];
				board[i][j] = 0;
				merged = true;
			}
		}
	}
	return merged;
}

bool moveRight() { 
	bool moved = moveRightOnce();
	bool merged = mergeRightOnce();
	if (merged)  moveRightOnce();
	return moved || merged;
}

// 处理玩家的一次按键输入，若发生移动或合并，返回true
bool processInput() {
	HANDLE consolehwnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consolehwnd, FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
	cout << "(W)Up (S)Down (A)Left (D)Right "; 
	
	char c; 
	cin >> c; 
	c &= 0x5F;  // 将小写字母转为大写
	bool changed = false;
	
	switch( c ) {
		case 'W': changed = moveUp(); break;
		case 'S': changed = moveDown(); break;
		case 'A': changed = moveLeft(); break;
		case 'D': changed = moveRight();
	}
	return changed;
}

// 判断游戏是否获胜，即是否存在2048方块
bool judgeWin() {
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			if (board[i][j] == target)
				return true;
	return false;
}

// 判断游戏是否失败，即上下左右都无法移动或合并
bool judgeLose() {
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
			if (!board[i][j] || canMerge(i - 1, j, board[i][j]) || canMerge(i, j - 1, board[i][j]))
				return false;
	return true;		
}

int main() {
	srand((unsigned)time(NULL));
	initialize();
	display();
	string info = "";
	
	while (true) {
		bool changed = processInput();
		// 只有发生了移动或合并才生成一个新方块
		if (changed)
			addTail();
		display();
		if (judgeWin()) {
		    info = "You win!";
			break;
		}
		if (judgeLose()) {
			info = "Game over";
			break;
		}
	}
	
	HANDLE consolehwnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(consolehwnd, FOREGROUND_INTENSITY|FOREGROUND_RED);
	cout << info << endl;
	return 0;
}