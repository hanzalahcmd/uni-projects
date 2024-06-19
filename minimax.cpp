#include<iostream>
#include<Windows.h>
#include<vector>
#include<ctime>
using namespace std;
int d = 3;
bool upmove(vector<vector<int>> &g,int&k)
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
						g[li][ri] *= 2; k+=g[li][ri];
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
				else li++;
			}
		}
	}
	return did;
}
bool downmove(vector<vector<int>>& g,int&k)
{
	bool did(false);
	int i, j, li, ri;
	for (j = 0; j < 4; j++)
	{
		li = 3, ri = j;
		for (i = 2; i >=0; i--)
		{
			if (g[i][j] != 0)
			{
				int temp = i +1;
				if (g[temp][j] == 0 || g[temp][j] == g[i][j])
				{
					if (g[li][ri] == g[i][j])
					{
						g[li][ri] *= 2; k+=g[li][ri];
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
				else li--;
			}
		}
	}
	return did;
}
bool rightmove(vector<vector<int>>& g,int&k)
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
						g[li][ri] *= 2; k+=g[li][ri];
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
				else ri--;
			}
		}
	}
	return did;
}
bool leftmove(vector<vector<int>>& g,int&k)
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
						g[li][ri] *= 2; k+=g[li][ri];
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
				else ri++;
			}
		}
	}
	return did;
}
void dis(vector<vector<int>> a)
{
	for(int i=0;i<4;++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			cout << a[i][j]<<"\t";
		}cout << endl;
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
	static int depth;
	vector<vector<int>> state;
	vector<losemove*> children;
	losemove* favchild;
	winmove(vector<vector<int>> a,int mo,int b=0,int ch=1)
	{
		mov = mo;
		state = a; favchild = nullptr;
		score = b; bool comeback=false;
		makechildren(ch); 
		heuristic();
	    largest();
	}
	void makechildren(int de);
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
	vector<winmove*> children;
	winmove* favchild;
	losemove(vector<vector<int>> a,int mo,int b=0,int ch=1)
	{
		mov = mo;
		state = a;
		favchild = nullptr; 
		score = b;
		makechildren(ch);
		heuristic();
		smallest();
	}
	void makechildren(int de)
	{
		if (de >= 0)
		{
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					if (state[i][j] == 0)
					{
						vector<vector<int>> temp = state;
						temp[i][j] = 4;
						children.push_back(new winmove(temp,mov, score,de-1));
						temp[i][j] = 2;
						children.push_back(new winmove(temp,mov,score,de-1));
					}
				}
			}
		}
	}
	void smallest()
	{
		int min = 655536;
		for (int i = 0; i < children.size(); ++i)
		{
			if (children[i]->score < min)
			{
				min = children[i]->score;
				favchild = children[i];
			}
			if(favchild!=nullptr)
			mov = favchild->mov;
		}
	}
	void display()
	{
		dis(state);
		cout << "\n" << score << " children " << children.size() << " losemove " << mov <<"\n ";// <<"  " << favchild->score << "\n";
	}
	void heuristic()
	{
		int maxblock = -1; int x(-1), y(-1);
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				if (state[i][j] > maxblock)
				{
					maxblock = state[i][j];
					x = i; y = j;
				}

			}
		}
		if (x == 0 && y == 0 || x == 0 && y == 3 || x == 3 && y == 0 || x == 3 && y == 3)
		{
			score += maxblock;
		}
	}
};
void winmove::makechildren(int de)
{
	int max = -1;
	if (de >= 0)
	{
		vector<vector<int>> temp = state; int a = score;
		if (upmove(temp, a))
		{
			children.push_back(new losemove(temp, 0, a, de));
			if (a > max)
			{
				max = a;
				favchild = children.back();
			}
		}
		temp = state; a = score;
		if (downmove(temp, a))
		{
			children.push_back(new losemove(temp, 1, a, de));
			if (a > max)
			{
				max = a;
				favchild = children.back();
			}
		}
		temp = state; a = score;
		if (rightmove(temp, a))
		{
			children.push_back(new losemove(temp, 2, a, de));
			if (a > max)
			{
				max = a;
				favchild = children.back();
			}
		}
		temp = state; a = score;
		if (leftmove(temp, a))
		{
			children.push_back(new losemove(temp, 3, a, de));
			if (a > max)
			{
				max = a;
				favchild = children.back();
			}
		}
	}
	//score = 0;
}
void winmove::largest()
{
	int max = -1;
	for (int i = 0; i < children.size(); ++i)
	{
		
			if (children[i]->score > max)
			{
				favchild = children[i]; max = children[i]->score;
			}
		
	}
}
void winmove::display()
{
	dis(state);
	cout << "\n" << score << " children " << children.size() << " winmove " << mov;// << favchild->score;
}
void winmove::heuristic()
{
	int maxblock = -1; int x(-1), y(-1);
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			if (state[i][j] > maxblock)
			{
				maxblock = state[i][j];
				x = i; y = j;
			}

		}
	}
	if (x == 0 && y == 0 || x == 0 && y == 3 || x == 3 && y == 0 || x == 3 && y == 3)
	{
		score += maxblock;
	}
}
int trav = 0;
void traversal(void *tr,char bari='w')//no use just for checking the tree
{
	cout << "\n" << ++trav << endl;
	if (bari == 'w')
	{
		winmove* h = reinterpret_cast<winmove*>(tr);
		h->display();
		for (int i = 0; i < h->children.size(); ++i)
		{
			if(h->children.size()!=0)
			traversal(reinterpret_cast<void*>(h->children[i]), 'l');
		}
	}
	else if (bari == 'l')
	{
		losemove* h = reinterpret_cast<losemove*>(tr);
		h->display();
		for (int i = 0; i < h->children.size(); ++i)
		{
			if(h->children.size()!=0)
			traversal(reinterpret_cast<void*>(h->children[i]), 'w');
		}
	}

}

int main()
{
	srand(time(0));
	vector<vector<int>> grid(4,vector<int>(4,0));
	vector<int> tiles(10, 2);
	tiles[rand() % 10] = 4;
	int i1(0), i2(0), i3(0), i4(0);
	do
	{
		i1 = rand() % 4;
		i2 = rand() % 4;
		i3 = rand() % 4;
		i4 = rand() % 4;
	} while (i1==i3&&i2==i4);
	grid[i1][i2] = tiles[rand() % 10]; 
	grid[i3][i4] = tiles[rand() % 10];
	dis(grid); cout << endl;
	/*winmove test(grid, -1);
	void* ptr = &test;
	traversal(ptr);*/
	//cout<<"\n"<<test.mov;
	int score(0);
	while (1)
	{
		winmove test(grid, -1);
		int coco(0);
		if (test.favchild != nullptr)
		{
			coco = test.favchild->mov;
		}
		else
		{
			coco = rand() % 4;//there comes a point when tree is not genrated because very few 
		}
		switch (coco)
		{
			case 0:
			{
				cout << "\nu\n";
				if(upmove(grid,score))
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
		system("cls");
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
				 //cout << "\nh\n";
			} while (grid[i1][i2]!=0&&checkempty(grid));
			grid[i1][i2] = tiles[rand() % 10];
			Sleep(1000);
			dis(grid); cout << endl;
			Sleep(1000);
		}
		if (checkover(grid))
		{
			cout << "gameover\n";
			Sleep(7000);
			break;
		}
	}
	
	//dis(grid);
	cout << "popo";
}