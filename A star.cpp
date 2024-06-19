#include <iostream>
#include<set>
#include<cmath>
#include<Windows.h>
#include<cstdlib>
using namespace std;
#define MAXC 10
#define MAXR 10

class box
{
protected:
    int x; int y; double g; double h; double f; bool goal; bool ishtart; bool occupied; int px;int py;
public:
    box() {}
    bool isgoal() { return goal; }
    box(int a, int b, bool c)
    {
        x = a; y = b; goal = 1; g = 0; h = 0; f = 0; ishtart = 0; occupied = 0; px = -1; py = -1;
    }
    box(int a, int b);
    void print1()const
    {
        cout << x << " " << y<<"f "<<f<<"g "<<g<<"\n";
        
    }
    void calg(int a, int b)
    {
        g = sqrt(pow(abs(x - a), 2) + pow(abs(y - b), 2));
    }
    void calf()
    {
        f = g + h; 
    }
    double calh(box &target)
    {

        h = sqrt(pow(abs(target.getx() - x), 2) + pow(abs(target.gety() - y), 2)); 
        return h;
    }
    int getpx() { return px; }
    int getpy() { return py; }
    int getx()const { return x; }
    int gety() const{ return y; }
    double getg()const { return g; }
    double getf ()const { return f; }
    friend bool operator <(const box& r, const box& l);
    friend void gstart();
    void printp()
    {
        cout << px << " px and py " << py << "\n";
    }
    void setp(box &parent)
    {
        px = parent.getx(); py = parent.gety(); calg(px, py); g += parent.getg(); calf();
    }
};
bool operator <(const box& r, const box& l)
{
	if(r.x==l.x&&r.y==l.y&&r.f==l.f){return r.g<l.g;}
    if (r.x == l.x && r.y == l.y) { return r.f < l.f; }
    else if (r.x == l.x) { return r.y < l.y; }
    else  { return r.x < l.x; };
}
box* start = NULL;
void gstart()
{
    cout << "enter the coordinates of your location\n"; int a(0), b(0); cin >> a >> b;
    start = new box(a, b); start->ishtart = 1; start->f = 0; start->g = 0; start->h = 0;
    
}
box target(9,9,1);
box::box(int a, int b)
{
    x = a; y = b; goal = 0; g = 0; h = calh(target); calf(); ishtart = 0; occupied = 0; px = -1; py = -1;
    
}
int main()
{
    char dis[MAXR][MAXC];
    for (int a = 0; a < MAXR; ++a)
    {
        for (int b = 0; b < MAXC; ++b)
        {
            dis[a][b] = ' ';
        }
    }
    gstart();
    system("CLS");
    dis[start->getx()][start->gety()] = '0'; dis[target.getx()][target.gety()] = '0';
    box** field = new box * [MAXC];
    for (int i = 0; i < MAXC; ++i)
    {
        field[i] = new box[MAXR];

        for (int j = 0; j < MAXC; ++j)
        {
            if (i == target.getx() && j == target.gety())
            {
                field[i][j] = target; continue;
            }
            if (i == start->getx() && j == start->gety())
            {
                field[i][j] = *start; continue;
            }
            field[i][j] = box(i, j);

        }


    }
    set<box>open; set<box> close;
    open.insert(*start);
    while (!open.empty())
    {
    	int found(0);
        set<box>::iterator i; int c = 0; int f(0); double min = 100;
        for (i = open.begin(); i != open.end(); ++i, ++c)
        {
              double temp(0);
            temp = i->getf(); 
            if (temp < min)
            {
                min = temp; f = c;
            }
        }c = 0; int X(0), Y(0);
        for (i = open.begin(); i != open.end(); ++i, ++c)
        {
            if (c == f)
            {
                X = i->getx(); Y = i->gety();i->print1(); dis[X][Y] = '0';
                cout << X << " x and y " << Y << "\n";
//                system("CLS");
//                for (int a=0;a<MAXC;++a)
//                {
//                    for (int b=0;b<MAXR;++b)
//                    {
//                        cout << dis[a][b];
//                    }
//                    cout << "\n";
//                }
                open.erase(i);close.insert(field[X][Y]);
                break;
            }
        }
        for(i=close.begin();i!=close.end();++i)
        {
        	if(i->getx()==X-1&&i->gety()==Y)
        	{
        		found=1;
			}
		}
        if (X-1>=0&&X-1<MAXR&&Y>=0&&Y< MAXC&&X-1!=field[X][Y].getpx()&&Y!=field[X][Y].getpy()&&found==0)
        {
        	
            field[X - 1][Y].setp(field[X][Y]); cout << X - 1 << " x and y " << Y << " "; 
			field[X - 1][Y].printp();
            int pre1(0),pre2(0);
            if (field[X - 1][Y].getx()==target.getx()&& field[X - 1][Y].gety()==target.gety())
            {
                break;
            }
            for (i=open.begin();i!=open.end();++i)
            {
                if (i->getx()==X-1&&i->gety()==Y&&field[X-1][Y].getf()>i->getf())
                {
                    pre1 = 1;
                }
            }
            for (i = close.begin(); i != close.end(); ++i)
            {
                if (i->getx() == X - 1 && i->gety() == Y && field[X - 1][Y].getf() > i->getf())
                {
                    pre2 = 1;
                }
            }
            if (pre1 == 0 && pre2 == 0)
            {
                open.insert(field[X - 1][Y]);
            }
        }found=0;
        for(i=close.begin();i!=close.end();++i)
        {
        	if(i->getx()==X+1&&i->gety()==Y)
        	{
        		found=1;
			}
		}
        if (X + 1 >= 0 && X + 1 < MAXR && Y >= 0 && Y < MAXC&&X+1!=field[X][Y].getpx()&&Y!=field[X][Y].getpy()&&found==0)
        {
            field[X + 1][Y].setp(field[X][Y]); cout << X + 1 << " x and y " << Y << " "; 
			field[X + 1][Y].printp();
            int pre1(0),pre2(0);
            if (field[X + 1][Y].getx() == target.getx() && field[X + 1][Y].gety() == target.gety())
            {
                break;
            }
            for (i = open.begin(); i != open.end(); ++i)
            {
                if (i->getx() == X +1 && i->gety() == Y && field[X + 1][Y].getf() > i->getf())
                {
                    pre1= 1;
                }
            }
            for (i = close.begin(); i != close.end(); ++i)
            {
                if (i->getx() == X + 1 && i->gety() == Y && field[X + 1][Y].getf() > i->getf())
                {
                    pre2 = 1;
                }
            }
            if (pre1 == 0 && pre2 == 0)
            {
                open.insert(field[X + 1][Y]);
            }
        }found=0;
        for(i=close.begin();i!=close.end();++i)
        {
        	if(i->getx()==X&&i->gety()==Y-1)
        	{
        		found=1;
			}
		}
        if (X  >= 0 && X < MAXR && Y -1>= 0 && Y-1 < MAXC&&X!=field[X][Y].getpx()&&Y-1!=field[X][Y].getpy()&&found==0)
        {
            field[X][Y - 1].setp(field[X][Y]); cout << X << " x and y " << Y-1 << " ";
			 field[X ][Y-1].printp();
            int pre1(0), pre2(0);
            if (field[X][Y - 1].getx() == target.getx() && field[X][Y - 1].gety() == target.gety())
            {
                break;
            }
            for (i = open.begin(); i != open.end(); ++i)
            {
                if (i->getx() == X  && i->gety() == Y-1 && field[X ][Y-1].getf() > i->getf())
                {
                    pre1 = 1;
                }
            }
            for (i = close.begin(); i != close.end(); ++i)
            {
                if (i->getx() == X && i->gety() == Y - 1 && field[X][Y - 1].getf() > i->getf())
                {
                    pre2 = 1;
                }
            }
            if (pre1 == 0 && pre2 == 0)
            {
                open.insert(field[X][Y - 1]);

            }
        }found=0;
        for(i=close.begin();i!=close.end();++i)
        {
        	if(i->getx()==X&&i->gety()==Y+1)
        	{
        		found=1;
			}
		}
        if (X  >= 0 && X < MAXR && Y+1 >= 0 && Y +1< MAXC&&X!=field[X][Y].getpx()&&Y+1!=field[X][Y].getpy()&&found==0)
        {
            field[X][Y + 1].setp(field[X][Y]); cout << X  << " x and y " << Y+1 << " "; 
			field[X ][Y+1].printp();
            int pre1(0), pre2(0);
            if (field[X][Y + 1].getx() == target.getx() && field[X][Y + 1].gety() == target.gety())
            {
                break;
            }
            for (i = open.begin(); i != open.end(); ++i)
            {
                if (i->getx() == X  && i->gety() == Y +1&& field[X ][Y+1].getf() > i->getf())
                {
                    pre1 = 1;
                }
            }
            for (i = close.begin(); i != close.end(); ++i)
            {
                if (i->getx() == X && i->gety() == Y + 1 && field[X][Y + 1].getf() > i->getf())
                {
                    pre2 = 1;
                }
            }
            if (pre1==0&&pre2==0)
            {
                open.insert(field[X][Y + 1]);

            }
        }found=0;
        for(i=close.begin();i!=close.end();++i)
        {
        	if(i->getx()==X-1&&i->gety()==Y+1)
        	{
        		found=1;
			}
		}
        if (X - 1 >= 0 && X - 1 < MAXR && Y+1 >= 0 && Y+1 < MAXC&&X-1!=field[X][Y].getpx()&&Y+1!=field[X][Y].getpy()&&found==0)
        {
            field[X-1][Y+1].setp(field[X][Y]); cout << X - 1 << " x and y " << Y+1 << " "; 
			field[X - 1][Y+1].printp();
            int pre1(0), pre2(0);
            if (field[X - 1][Y + 1].getx() == target.getx() && field[X - 1][Y + 1].gety() == target.gety())
            {
                break;
            }
            for (i = open.begin(); i != open.end(); ++i)
            {
                if (i->getx() == X - 1 && i->gety() == Y+1 && field[X - 1][Y+1].getf() > i->getf())
                {
                    pre1 = 1;
                }
            }
            for (i = close.begin(); i != close.end(); ++i)
            {
                if (i->getx() == X - 1 && i->gety() == Y + 1 && field[X - 1][Y + 1].getf() > i->getf())
                {
                    pre2 = 1;
                }
            }
            if (pre1==0&&pre2==0)
            {
                open.insert(field[X - 1][Y + 1]);
            }
        }found=0;
        for(i=close.begin();i!=close.end();++i)
        {
        	if(i->getx()==X-1&&i->gety()==Y-1)
        	{
        		found=1;
			}
		}
        if (X - 1 >= 0 && X - 1 < MAXR && Y -1>= 0 && Y-1 < MAXC&&X-1!=field[X][Y].getpx()&&Y-1!=field[X][Y].getpy()&&found==0)
        {
            field[X-1][Y-1].setp(field[X][Y]); cout << X - 1 << " x and y " << Y-1 << " "; 
			field[X - 1][Y-1].printp();
            int pre1(0), pre2(0);
            if (field[X - 1][Y - 1].getx() == target.getx() && field[X - 1][Y - 1].gety() == target.gety())
            {
                break;
            }
            for (i = open.begin(); i != open.end(); ++i)
            {
                if (i->getx() == X - 1 && i->gety() == Y-1 && field[X - 1][Y-1].getf() > i->getf())
                {
                    pre1 = 1;
                }
            }
            for (i = close.begin(); i != close.end(); ++i)
            {
                if (i->getx() == X - 1 && i->gety() == Y - 1 && field[X - 1][Y - 1].getf() > i->getf())
                {
                    pre2 = 1;
                }
            }
            if (pre1==0&&pre2==0)
            {
                open.insert(field[X - 1][Y - 1]);
            }
        }found=0;
        for(i=close.begin();i!=close.end();++i)
        {
        	if(i->getx()==X+1&&i->gety()==Y-1)
        	{
        		found=1;
			}
		}
        if (X + 1 >= 0 && X + 1 < MAXR && Y-1 >= 0 && Y-1 < MAXC&&X+1!=field[X][Y].getpx()&&Y-1!=field[X][Y].getpy()&&found==0)
        {
            field[X+1][Y-1].setp(field[X][Y]);cout << X + 1 << " x and y " << Y-1 << " "; field[X + 1][Y-1].printp();
            int pre1(0), pre2(0);
            if (field[X + 1][Y - 1].getx() == target.getx() && field[X + 1][Y - 1].gety() == target.gety())
            {
                break;
            }
            for (i = open.begin(); i != open.end(); ++i)
            {
                if (i->getx() == X + 1 && i->gety() == Y-1 && field[X - 1][Y-1].getf() > i->getf())
                {
                    pre1 = 1;

                }
            }
            for (i = close.begin(); i != close.end(); ++i)
            {
                if (i->getx() == X + 1 && i->gety() == Y - 1 && field[X - 1][Y - 1].getf() > i->getf())
                {
                    pre2 = 1;
                }
            }
            if (pre1==0&&pre2==0)
            {
                open.insert(field[X + 1][Y - 1]);
            }
        }found=0;
        for(i=close.begin();i!=close.end();++i)
        {
        	if(i->getx()==X+1&&i->gety()==Y+1)
        	{
        		found=1;
			}
		}
        if (X +1 >= 0 && X + 1 < MAXR && Y+1 >= 0 && Y+1 < MAXC&&X+1!=field[X][Y].getpx()&&Y+1!=field[X][Y].getpy()&&found==0)
        {
            field[X+1][Y+1].setp(field[X][Y]); cout << X + 1 << " x and y " << Y+1 << " "; field[X + 1][Y+1].printp();
            int pre1(0), pre2(0);
            if (field[X +1][Y + 1].getx() == target.getx() && field[X + 1][Y + 1].gety() == target.gety())
            {
                break;
            }
            for (i = open.begin(); i != open.end(); ++i)
            {
                if (i->getx() == X + 1 && i->gety() == Y+1 && field[X + 1][Y+1].getf() > i->getf())
                {
                    pre1 = 1;
                }
            }
            for (i = close.begin(); i != close.end(); ++i)
            {
                if (i->getx() == X + 1 && i->gety() == Y + 1 && field[X + 1][Y + 1].getf() > i->getf())
                {
                    pre2 = 1;
                }
            }
            if (pre1==0&&pre2==0)
            {
                open.insert(field[X + 1][Y + 1]);
            }
        }
        close.insert(field[X][Y]);
    }int x(-1), y(-1), Px(-1), Py(-1);
    set<box>::iterator i;
    cout<<"output\n";
    for (i=close.begin();i!=close.end();++i)
    {
        i->print1();
    }
    x = target.getx(); y = target.gety();
    while (Px != start->getx() && Py != start->gety())
    {
        cout << x << " x and y " << y << "\n";
        Sleep(1000);
        Px = field[x][y].getpx(); Py = field[x][y].getpy();
        x = Px; y = Py;
    }
    for(int i=0;i<MAXR;++i)
    {
    	for(int j=0;j<MAXC;++j)
    	{
    		cout<<dis[i][j];
		}
		cout<<"\n";
	}
}
//#include <iostream>
#include <vector>
#include <queue>
#include <cmath>

using namespace std;

// Define a cell struct to represent each cell in the grid
struct Cell {
    int x, y;
    double f, g, h;
    Cell* parent;

    Cell(int _x, int _y) : x(_x), y(_y), f(0), g(0), h(0), parent(nullptr) {}

    // Overload the '<' operator for priority queue
    bool operator<(const Cell& other) const {
        return f > other.f;
    }
};

// Define a function to calculate the Euclidean distance heuristic between two cells
double euclidean_distance(Cell* a, Cell* b) {
    return sqrt(pow(a->x - b->x, 2) + pow(a->y - b->y, 2));
}

// Define a function to check if a cell is inside the grid
bool is_inside_grid(int x, int y, int rows, int cols) {
    return (x >= 0 && x < rows && y >= 0 && y < cols);
}

// Define the A* algorithm function
vector<Cell*> astar(Cell* start, Cell* goal, vector<vector<int>>& grid) {
    // Define the open and closed lists
    priority_queue<Cell*> open_list;
    vector<Cell*> closed_list;

    // Add the start cell to the open list
    start->h = euclidean_distance(start, goal);
    start->f = start->g + start->h;
    open_list.push(start);

    // Loop until the open list is empty or the goal is found
    while (!open_list.empty()) {
        // Get the cell with the lowest f value from the open list
        Cell* current = open_list.top();
        open_list.pop();

        // Check if the current cell is the goal
        if (current->x == goal->x && current->y == goal->y) {
            // Trace the path from the goal back to the start
            vector<Cell*> path;
            while (current != nullptr) {
                path.push_back(current);
                current = current->parent;
            }
            return path;
        }

        // Add the current cell to the closed list
        closed_list.push_back(current);

        // Check the neighbors of the current cell
        for (int i = -1; i <= 1; i++) {
            for (int j = -1; j <= 1; j++) {
                // Skip the current cell and the diagonals
                if ((i == 0 && j == 0) || (i != 0 && j != 0)) {
                    continue;
                }

                // Calculate the coordinates of the neighbor cell
                int x = current->x + i;
                int y = current->y + j;

                // Check if the neighbor cell is inside the grid
                if (!is_inside_grid(x, y, grid.size(), grid[0].size())) {
                    continue;
                }

                // Check if the neighbor cell is an obstacle
                if (grid[x][y] == 1) {
                    continue;
                }

                // Create the neighbor cell
                Cell* neighbor = new Cell(x, y);
                neighbor->g = current->g + 1;
                neighbor->h = euclidean_distance(neighbor, goal);
                neighbor->f = neighbor->g + neighbor->h;
                neighbor->parent = current;

                // Check if the neighbor cell is already in the closed list
                bool

