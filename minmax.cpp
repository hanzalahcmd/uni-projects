#include <iostream>
#include <Windows.h>
#include <vector>
#include <chrono>
#include <memory>
#include<algorithm>
using namespace std;
int cunt = 0;
int d = 3;
bool upmove(vector<vector<int>>& g, int& k)
{
	bool did(false);
	int i, j, li, ri;
	for (j = 0; j < 4; j++)
	{
		li = 0, ri = j;
		for (i = 1; i < 4; i++)
		{
			if (g[i][j] != 0)
			{
				int temp = i - 1;
				if (g[temp][j] == 0 || g[temp][j] == g[i][j])
				{
					if (g[li][ri] == g[i][j])
					{
						g[li][ri] *= 2;
						k += g[li][ri];
						g[i][j] = 0;
					}
					else
					{
						if (g[li][ri] == 0)
						{
							g[li][ri] = g[i][j];
							g[i][j] = 0;
						}
						else
						{
							g[++li][ri] = g[i][j];
							g[i][j] = 0;
						}
					}
					did = true;
				}
				else
					li++;
			}
		}
	}
	return did;
}
bool downmove(vector<vector<int>>& g, int& k)
{
	bool did(false);
	int i, j, li, ri;
	for (j = 0; j < 4; j++)
	{
		li = 3, ri = j;
		for (i = 2; i >= 0; i--)
		{
			if (g[i][j] != 0)
			{
				int temp = i + 1;
				if (g[temp][j] == 0 || g[temp][j] == g[i][j])
				{
					if (g[li][ri] == g[i][j])
					{
						g[li][ri] *= 2;
						k += g[li][ri];
						g[i][j] = 0;
					}
					else
					{
						if (g[li][ri] == 0)
						{
							g[li][ri] = g[i][j];
							g[i][j] = 0;
						}
						else
						{
							g[--li][ri] = g[i][j];
							g[i][j] = 0;
						}
					}
					did = true;
				}
				else
					li--;
			}
		}
	}
	return did;
}
bool rightmove(vector<vector<int>>& g, int& k)
{
	bool did(false);
	int i, j, li, ri;
	for (i = 0; i < 4; i++)
	{
		li = i, ri = 3;
		for (j = 2; j >= 0; j--)
		{
			if (g[i][j] != 0)
			{
				int temp = j + 1;
				if (g[i][temp] == 0 || g[i][temp] == g[i][j])
				{
					if (g[li][ri] == g[i][j])
					{
						g[li][ri] *= 2;
						k += g[li][ri];
						g[i][j] = 0;
					}
					else
					{
						if (g[li][ri] == 0)
						{
							g[li][ri] = g[i][j];
							g[i][j] = 0;
						}
						else
						{
							g[li][--ri] = g[i][j];
							g[i][j] = 0;
						}
					}
					did = true;
				}
				else
					ri--;
			}
		}
	}
	return did;
}
bool leftmove(vector<vector<int>>& g, int& k)
{
	bool did(false);
	int i, j, li, ri;
	for (i = 0; i < 4; i++)
	{
		li = i, ri = 0;
		for (j = 1; j < 4; j++)
		{
			if (g[i][j] != 0)
			{
				int temp = j - 1;
				if (g[i][temp] == 0 || g[i][temp] == g[i][j])
				{
					if (g[li][ri] == g[i][j])
					{
						g[li][ri] *= 2;
						k += g[li][ri];
						g[i][j] = 0;
					}
					else
					{
						if (g[li][ri] == 0)
						{
							g[li][ri] = g[i][j];
							g[i][j] = 0;
						}
						else
						{
							g[li][++ri] = g[i][j];
							g[i][j] = 0;
						}
					}
					did = true;
				}
				else
					ri++;
			}
		}
	}
	return did;
}
void dis(vector<vector<int>> a)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			cout << a[i][j] << "\t";
		}
		cout << endl;
	}
}
bool checkover(vector<vector<int>> g)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (g[i][j] == 0)
				return false;
			if (i - 1 >= 0 && i - 1 < 4)
			{
				int t = i - 1;
				if (g[i][j] == g[t][j])
					return false;
			}
			if (i + 1 >= 0 && i + 1 < 4)
			{
				int t = i + 1;
				if (g[i][j] == g[t][j])
					return false;
			}
			if (j - 1 >= 0 && j - 1 < 4)
			{
				int t = j - 1;
				if (g[i][j] == g[i][t])
					return false;
			}
			if (j + 1 >= 0 && j + 1 < 4)
			{
				int t = j + 1;
				if (g[i][j] == g[i][t])
					return false;
			}
		}
	}
	return true;
}
bool checkempty(vector<vector<int>> g)
{
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (g[i][j] == 0)
				return true;
		}
	}
	return false;
}

class losemove;

class winmove
{
public:
	int mov;
	int score;
	vector<vector<int>> state;
	vector<unique_ptr<losemove>> children;
	unique_ptr<losemove> favchild;

	winmove(vector<vector<int>> a, int mo, int b = 0, int ch = d)
	{
		++cunt;
		mov = mo;
		state = a;
		score = b;
		makechildren(ch);
		heuristic();
		largest();
	}

	void makechildren(int de, int alpha = INT_MIN, int beta = INT_MAX);
	void largest();
	void display();
	void heuristic();
};

class losemove
{
public:
	int score;
	int mov;
	vector<vector<int>> state;
	vector<unique_ptr<winmove>> children;
	unique_ptr<winmove> favchild;

	losemove(vector<vector<int>> a, int mo, int b = 0, int ch = d)
	{
		++cunt;
		mov = mo;
		state = a;
		score = b;
		makechildren(ch);
		heuristic();
		smallest();
	}

	void makechildren(int de, int alpha = INT_MIN, int beta = INT_MAX)
	{
		if (de >= 0)
		{
			int min_ = INT_MAX; 

			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					if (state[i][j] == 0)
					{
						vector<vector<int>> temp = state;

						// 4 move
						temp[i][j] = 4;
						auto child = make_unique<winmove>(temp, mov, score, de - 1);
						min_ = min(min_, child->score);

						if (min_ < beta) {
							beta = min_;
							favchild = std::move(child); 
						}

						if (alpha >= beta)
							return;

						children.push_back(std::move(favchild));

						temp[i][j] = 2;
						child = make_unique<winmove>(temp, mov, score, de - 1);
						min_ = min(min_, child->score);

						if (min_ < beta) {
							beta = min_;
							favchild = std::move(child); 
						}

						if (alpha >= beta)
							return;

						children.push_back(std::move(favchild));
					}
				}
			}
		}
	}


	void smallest()
	{
		int min = 655536;
		for (auto& child : children)
		{
			if (child!=nullptr&&child->score < min)
			{
				min = child->score;
				favchild = std::move(child);
			}
			if (favchild != nullptr)
				mov = favchild->mov;
		}
	}

	void display()
	{
		dis(state);
		cout << "\n"
			<< score << " children " << children.size() << " losemove " << mov << "\n ";
	}

	void heuristic()
	{
		int maxblock = -1;
		int x(-1), y(-1);
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				if (state[i][j] > maxblock)
				{
					maxblock = state[i][j];
					x = i;
					y = j;
				}
			}
		}
		if ((x == 0 && y == 0) || (x == 0 && y == 3) || (x == 3 && y == 0) || (x == 3 && y == 3))
		{
			score -= maxblock;
		}
	}
};

void winmove::makechildren(int de, int alpha, int beta)
{
	int max_ = INT_MIN;
	if (de >= 0)
	{
		vector<vector<int>> temp = state;
		int a = score;

		// Up move
		if (upmove(temp, a))
		{
			auto child = make_unique<losemove>(temp, 0, a, de);
			max_ = max(max_, child->score);
			if (max_ > alpha) {
				alpha = max_;
				favchild = std::move(child); 
			}
			if (alpha >= beta) 
				return;
			children.push_back(std::move(favchild)); 
		}

		// Down move
		temp = state;
		a = score;
		if (downmove(temp, a))
		{
			auto child = make_unique<losemove>(temp, 1, a, de);
			max_ = max(max_, child->score);
			if (max_ > alpha) {
				alpha = max_;
				favchild = std::move(child); 
			}
			if (alpha >= beta)
				return;
			children.push_back(std::move(favchild)); 
		}

		// Right move
		temp = state;
		a = score;
		if (rightmove(temp, a))
		{
			auto child = make_unique<losemove>(temp, 2, a, de);
			max_ = max(max_, child->score);
			if (max_ > alpha) {
				alpha = max_;
				favchild = std::move(child); 
			}
			if (alpha >= beta)
				return;
			children.push_back(std::move(favchild)); 
		}

		// Left move
		temp = state;
		a = score;
		if (leftmove(temp, a))
		{
			auto child = make_unique<losemove>(temp, 3, a, de);
			max_ = max(max_, child->score);
			if (max_ > alpha) {
				alpha = max_;
				favchild = std::move(child); 
			}
			if (alpha >= beta)
				return;
			children.push_back(std::move(favchild)); 
		}
	}
}



void winmove::largest()
{
	int max = -1;
	for (auto& child : children)
	{
		if (child != nullptr && child->score > max)
		{
			max = child->score;
			favchild = move(child);
		}
	}
}

void winmove::display()
{
	dis(state);
	cout << "\n"
		<< score << " children " << children.size() << " winmove " << mov;
}

void winmove::heuristic()
{
	int maxblock = -1;
	int x(-1), y(-1);
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (state[i][j] > maxblock)
			{
				maxblock = state[i][j];
				x = i;
				y = j;
			}
		}
	}
	if ((x == 0 && y == 0) || (x == 0 && y == 3) || (x == 3 && y == 0) || (x == 3 && y == 3))
	{
		score += maxblock;
	}
}

int trav = 0;
// void traversal(void *tr, char bari = 'w') // no use just for checking the tree//this code is commented because it gives compilation issue and was just for the debuggin purpose
// {
// 	cout << "\n"
// 		 << ++trav << endl;
// 	if (bari == 'w')
// 	{
// 		winmove *h = reinterpret_cast<winmove *>(tr);
// 		h->display();
// 		for (int i = 0; i < h->children.size(); ++i)
// 		{
// 			if (h->children.size() != 0)
// 				traversal(reinterpret_cast<void *>(h->children[i]), 'l');
// 		}
// 	}
// 	else if (bari == 'l')
// 	{
// 		losemove *h = reinterpret_cast<losemove *>(tr);
// 		h->display();
// 		for (int i = 0; i < h->children.size(); ++i)
// 		{
// 			if (h->children.size() != 0)
// 				traversal(reinterpret_cast<void *>(h->children[i]), 'w');
// 		}
// 	}
// }

int main()
{
	srand(time(0));
	vector<vector<int>> grid(4, vector<int>(4, 0));
	vector<int> tiles(10, 2);
	tiles[rand() % 10] = 4;
	int i1(0), i2(0), i3(0), i4(0);
	do
	{
		i1 = rand() % 4;
		i2 = rand() % 4;
		i3 = rand() % 4;
		i4 = rand() % 4;
	} while (i1 == i3 && i2 == i4);
	grid[i1][i2] = tiles[rand() % 10];
	grid[i3][i4] = tiles[rand() % 10];
	dis(grid);
	system("cls");
	/*winmove test(grid, -1);
	void* ptr = &test;
	traversal(ptr);*/
	// cout<<"\n"<<test.mov;
	int score(0);
	int movecount = 0;
	chrono::high_resolution_clock::duration average{ 0 };
	while (1)
	{

		auto start = chrono::high_resolution_clock::now();
		std::unique_ptr<winmove> test = std::make_unique<winmove>(grid, -1);
		auto end = chrono::high_resolution_clock::now();
		int coco(0);
		if (test->favchild != nullptr)
		{
			coco = test->favchild->mov;
		}
		else
		{
			coco = rand() % 4; 
		}
		switch (coco)
		{
		case 0:
		{
			cout << "\nu\n";
			if (upmove(grid, score))
			{
				cout << "\nup\n";
			}
			break;
		}
		case 1:
		{
			cout << "\nd\n";
			if (downmove(grid, score))
			{
				cout << "\ndown\n";
			}
			break;
		}
		case 2:
		{
			cout << "\nr\n";
			if (rightmove(grid, score))
			{
				cout << "\nright\n";
			}
			break;
		}
		case 3:
		{
			cout << "\nl\n";
			if (leftmove(grid, score))
			{
				cout << "\nleft\n";
			}
			break;
		}
		}

		if (checkover(grid))
		{
			cout << "gameover\n";
			Sleep(7000);
			break;
		}
		else
		{
			int i1(0), i2(0);
			do
			{
				i1 = rand() % 4;
				i2 = rand() % 4;
				// cout << "\nh\n";
			} while (grid[i1][i2] != 0 && checkempty(grid));
			grid[i1][i2] = tiles[rand() % 10];
			//Sleep(500);
			dis(grid);
			cout << endl;
			chrono::duration<float> decisiontime = end - start;
			average += end - start;
			cout << "time taken to make the decision " << decisiontime.count()<<" seconds";
			//Sleep(500);
			cout << endl<<cunt<<"\nis the number of states processed\n";

			system("cls");
		}
		++movecount;
	}
	average /= movecount;
	system("cls");
	cout << "average decision time " << chrono::duration_cast<chrono::milliseconds>(average).count()<<"milliseconds\n";
	cout << "the last state of the game is \n";
	dis(grid);
	cout << "the number of moves made by the program "<<movecount<<endl;
}