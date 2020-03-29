#include <iostream>
#include <fstream>
using namespace std;
char sodu[9][9];
bool possibility[9][9][9];
char hashArray[82][2];
void EnterAndInitial() {
	ifstream input("sodu.txt");
	for (int cnt1 = 0; cnt1 < 9; cnt1++) for (int cnt2 = 0; cnt2 < 9; cnt2++) {
		input >> sodu[cnt1][cnt2];
		sodu[cnt1][cnt2] -= '0';
		if (sodu[cnt1][cnt2]) {
			for (int cnt3 = 0; cnt3 < 9; cnt3++) possibility[cnt1][cnt2][cnt3] = false;
			possibility[cnt1][cnt2][sodu[cnt1][cnt2] - 1] = true;
		}
		else
			for (int cnt3 = 0; cnt3 < 9; cnt3++) possibility[cnt1][cnt2][cnt3] = true;
	}
}
void GetUniquity() {
	int temp1, temp2, temp3, x, y;
	for (int cnt1 = 0; cnt1 < 9; cnt1++) for (int cnt2 = 0; cnt2 < 9; cnt2++) {
		temp1 = 0;
		for (int cnt3 = 0; cnt3 < 9; cnt3++) if (possibility[cnt1][cnt3][cnt2]) { temp1++; temp2 = cnt3; }
		if (temp1 == 1) {
			sodu[cnt1][temp2] = cnt2 + 1;
			for (int cnt3 = 0; cnt3 < 9; cnt3++) possibility[cnt1][temp2][cnt3] = false;
			possibility[cnt1][temp2][cnt2] = true;
		}
	}
	for (int cnt1 = 0; cnt1 < 9; cnt1++) for (int cnt2 = 0; cnt2 < 9; cnt2++) {
		temp1 = 0;
		for (int cnt3 = 0; cnt3 < 9; cnt3++) if (possibility[cnt3][cnt1][cnt2]) { temp1++; temp2 = cnt3; }
		if (temp1 == 1) {
			sodu[temp2][cnt1] = cnt2 + 1;
			for (int cnt3 = 0; cnt3 < 9; cnt3++) possibility[temp2][cnt1][cnt3] = false;
			possibility[temp2][cnt1][cnt2] = true;
		}
	}
	for (x = 0; x < 9; x += 3) for (y = 0; y < 9; y += 3) {
		for (int cnt1 = 0; cnt1 < 9; cnt1++) {
			temp1 = 0;
			for (int cnt2 = 0; cnt2 < 3; cnt2++) for (int cnt3 = 0; cnt3 < 3; cnt3++) if (possibility[cnt2 + y][cnt3 + x][cnt1]) { temp1++; temp2 = cnt2; temp3 = cnt3; }
			if (temp1 == 1) {
				sodu[temp2 + y][temp3 + x] = cnt1 + 1;
				for (int cnt3 = 0; cnt3 < 9; cnt3++) possibility[temp2 + y][temp3 + x][cnt3] = false;
				possibility[temp2 + y][temp3 + x][cnt1] = true;
			}
		}
	}
}
void DiminishPossibility() {
	int x, y;
	for (int cnt1 = 0; cnt1 < 9; cnt1++) for (int cnt2 = 0; cnt2 < 9; cnt2++) {
		if (sodu[cnt1][cnt2]) {
			if (cnt1 < 3) y = 0; else if (cnt1 < 6) y = 3; else y = 6;
			if (cnt2 < 3) x = 0; else if (cnt2 < 6) x = 3; else x = 6;
			for (int cnt3 = 0; cnt3 < 9; cnt3++) {
				possibility[cnt1][cnt3][sodu[cnt1][cnt2] - 1] = false;
				possibility[cnt3][cnt2][sodu[cnt1][cnt2] - 1] = false;
			}
			for (int cnt3 = 0; cnt3 < 3; cnt3++) for (int cnt4 = 0; cnt4 < 3; cnt4++) possibility[cnt3 + y][cnt4 + x][sodu[cnt1][cnt2] - 1] = false;
		}
	}
}
int NextState() {
	int amountOfPossibility;
	int temp1;
	int rest = 81;
	for (int cnt1 = 0; cnt1 < 9; cnt1++) for (int cnt2 = 0; cnt2 < 9; cnt2++) {
		amountOfPossibility = 0;
		for (int cnt3 = 0; cnt3 < 9; cnt3++) {
			if (possibility[cnt1][cnt2][cnt3]) {
				amountOfPossibility++;
				temp1 = cnt3 + 1;
			}
		}
		if (amountOfPossibility == 1) sodu[cnt1][cnt2] = temp1;
		if (sodu[cnt1][cnt2]) rest--;
	}
	return rest;
}
bool IsCalculatable() {
	int ptr = 0;
	char rest = 81, lastStepRest = 81;
	while (rest > 0) {
		GetUniquity();
		DiminishPossibility();
		rest = NextState();
		if (lastStepRest != rest) lastStepRest = rest;
		else {
			for (int cnt1 = 0; cnt1 < 9; cnt1++) for (int cnt2 = 0; cnt2 < 9; cnt2++) if (!sodu[cnt1][cnt2]) { hashArray[ptr][0] = cnt1; hashArray[ptr++][1] = cnt2; }
			hashArray[ptr][0] = -1; hashArray[ptr][1] = -1;
			return false;
		}
	}
	return true;
}
bool IsRational() {
	int temp, x, y;
	for (int cnt1 = 0; cnt1 < 9; cnt1++) for (int cnt2 = 1; cnt2 <= 9; cnt2++) for (int cnt3 = 0, temp = 0; cnt3 < 9; cnt3++) {
		if (sodu[cnt1][cnt3] == cnt2) temp++;
		if (temp > 1) return false;
	}
	for (int cnt1 = 0; cnt1 < 9; cnt1++) for (int cnt2 = 1; cnt2 <= 9; cnt2++) for (int cnt3 = 0, temp = 0; cnt3 < 9; cnt3++) {
		if (sodu[cnt3][cnt1] == cnt2) temp++;
		if (temp > 1) return false;
	}
	for (x = 0; x < 9; x += 3) for (y = 0; y < 9; y += 3)
		for (int cnt1 = 1; cnt1 <= 9; cnt1++) {
			temp = 0;
			for (int cnt2 = 0; cnt2 < 3; cnt2++) for (int cnt3 = 0; cnt3 < 3; cnt3++) {
				if (sodu[cnt2 + x][cnt3 + y] == cnt1) temp++;
				if (temp > 1) return false;
			}
		}
	return true;
}
bool DFS(char depth) {
	if (hashArray[depth][0] == -1) return true;
	else {
		for (int cnt1 = 1; cnt1 <= 9; cnt1++) {
			if (possibility[hashArray[depth][0]][hashArray[depth][1]][cnt1 - 1]) {
				sodu[hashArray[depth][0]][hashArray[depth][1]] = cnt1;
				if (IsRational() && DFS(depth + 1)) return true;
				else sodu[hashArray[depth][0]][hashArray[depth][1]] = 0;
			}
		}
		return false;
	}
}
bool Solve() {
	if (IsRational())
		if (IsCalculatable() || DFS(0)) return true; else return false;
	else return false;
}
void Output(bool solvable) {
	if (solvable) {
		for (int cnt1 = 0; cnt1 < 9; cnt1++) {
			for (int cnt2 = 0; cnt2 < 9; cnt2++) {
				sodu[cnt1][cnt2] += '0';
				cout << sodu[cnt1][cnt2] << ' ';
			}
			cout << "\n";
		}
	}
	else cout << "unsolvable!";
}
int main() {
	EnterAndInitial();
	Output(Solve());
	return 0;
}