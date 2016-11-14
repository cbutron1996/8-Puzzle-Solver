// Christian Butron
// CISC 3410
// Program #1

// Breadth-First Search
// Depth-First Search
// A* Search with Manhattan Distance Heuristic

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <stack>
#include <algorithm>
#include <time.h>
using namespace std;
const int goal[3][3] =  {{0,1,2},
                        {3,4,5},
                        {6,7,8}};

struct Node
{
    Node* left;
    Node* up;
    Node* down;
    Node* right;
    Node* parent;

    int state[3][3];
    int rIndex;
    int cIndex;
    int cost;
    int depth;
    int manDist;
    int op;

    Node();
    Node(Node *);
    Node(int state[3][3]);

    bool isGoal();
    void printNode();

    bool moveLeft();
    bool moveUp();
    bool moveDown();
    bool moveRight();
};

class Tree
{
    private:
		int initial[3][3];
		int nodeCount;
		int maxDepth;
        Node *root;

    public:
        Tree() { nodeCount = 0; }
        int getNodeCount() const { return nodeCount; }
        int getMaxDepth() const { return maxDepth; }

		void plantTree(int);
		void read();
		void print(Node *);

		void BFS();
		void DFS();
		void ASTAR();
};

int getManDist(Node *);

Node::Node()
{
    this->left = NULL;
    this->up = NULL;
    this->down = NULL;
    this->right = NULL;
    this->parent = NULL;
}

Node::Node(Node *temp)
{
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            this->state[i][j] = temp->state[i][j];
            if(temp->state[i][j] == 0)
            {
                this->rIndex = i;
                this->cIndex = j;
            }
        }
    }
    this->left = NULL;
    this->up = NULL;
    this->down = NULL;
    this->right = NULL;
    this->parent = NULL;
}

Node::Node(int state[3][3])
{
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            this->state[i][j] = state[i][j];
            if(state[i][j] == 0)
            {
                this->rIndex = i;
                this->cIndex = j;
            }
        }
    }
    this->left = NULL;
    this->up = NULL;
    this->down = NULL;
    this->right = NULL;
    this->parent = NULL;
}

bool Node::isGoal()
{
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            if(this->state[i][j] != goal[i][j])
                return false;
        }
    }
    return true;
}

bool Node::moveLeft()
{
    Node *temp = new Node(this);
	temp->parent = this;
	temp->cost = this->cost + 1;
	temp->depth = this->depth + 1;
    int r = temp->rIndex;
    int c = temp->cIndex;
    if(c > 0 && this->op != 4)
    {
        swap(temp->state[r][c], temp->state[r][c-1]);
        temp->op = 1;
        temp->manDist = getManDist(temp);
        this->left = temp;
        return true;
    }
    delete temp;
    return false;
}

bool Node::moveUp()
{
    Node *temp = new Node(this);
	temp->parent = this;
	temp->cost = this->cost + 1;
	temp->depth = this->depth + 1;
    int r = temp->rIndex;
    int c = temp->cIndex;
    if(r > 0 && this->op != 3)
    {
        swap(temp->state[r][c], temp->state[r-1][c]);
        temp->op = 2;
        temp->manDist = getManDist(temp);
        this->up = temp;
        return true;
    }
    delete temp;
    return false;
}

bool Node::moveDown()
{
    Node *temp = new Node(this);
	temp->parent = this;
	temp->cost = this->cost + 1;
	temp->depth = this->depth + 1;
    int r = temp->rIndex;
    int c = temp->cIndex;
    if(r < 2 && this->op != 2)
    {
        swap(temp->state[r][c], temp->state[r+1][c]);
        temp->op = 3;
        temp->manDist = getManDist(temp);
        this->down = temp;
        return true;
    }
    delete temp;
    return false;
}

bool Node::moveRight()
{
    Node *temp = new Node(this);
	temp->parent = this;
	temp->cost = this->cost + 1;
	temp->depth = this->depth + 1;
    int r = temp->rIndex;
    int c = temp->cIndex;
    if(c < 2 && this->op != 1)
    {
        swap(temp->state[r][c], temp->state[r][c-1]);
        temp->op = 4;
        temp->manDist = getManDist(temp);
        this->right = temp;
        return true;
    }
    delete temp;
    return false;
}

void Node::printNode()
{
    switch(this->op)
    {
        case 0:
        {
            cout<<"Root"<<endl;
            break;
        }
        case 1:
        {
            cout<<"Left"<<endl;
            break;
        }
        case 2:
        {
            cout<<"Up"<<endl;
            break;
        }
        case 3:
        {
            cout<<"Down"<<endl;
            break;
        }
        case 4:
        {
            cout<<"Right"<<endl;
            break;
        }
    }
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            cout << this->state[i][j] << " ";
        }
        cout << endl;
    }
}

void Tree::plantTree(int n)
{
	read();
	Node *temp = new Node(initial);
	temp->depth = 0;
	temp->cost = 0;
	temp->op = 0;
	temp->manDist = getManDist(temp);

	if(root == NULL)
		root = temp;

	if(n == 0)
        BFS();
    else if(n == 1)
        DFS();
    else if(n == 2)
        ASTAR();
}

void Tree::read()
{
	ifstream infile ("puzzle.txt");
    string line, item;
    int c = 0;
    int temp[9];
    while(getline(infile, line))
    {
		istringstream linestream(line);
		while(getline(linestream, item, ' '))
        {
            temp[c] = atoi(item.c_str());
            c++;
        }
    }
    int k = 0;
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            initial[i][j] = temp[k];
            k++;
        }
    }
}

void Tree::print(Node* temp)
{
	vector<Node*> v;
	while(temp != NULL)
    {
		v.push_back(temp);
		temp = temp->parent;
	}
	cout << "Path Cost: " << v[0]->cost << endl;
	for(int j = v.size() - 1; j >= 0; j--)
		v[j]->printNode();
	cout << "Goal State" << endl;
	for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            cout << goal[i][j] << " ";
        }
        cout << endl;
    }
}

void Tree::BFS()
{
    queue<Node *> Q;
    Q.push(root);
    maxDepth = 0;
    while(!Q.empty())
    {
        nodeCount++;
        Node *temp = Q.front();
        Q.pop();
        if(temp->depth > maxDepth)
            maxDepth = temp->depth;
        if(temp->isGoal())
        {
            cout << "Breadth-First Search" << endl;
            print(temp);
            return;
        }
        if(temp->moveLeft()) Q.push(temp->left);
        if(temp->moveUp()) Q.push(temp->up);
        if(temp->moveDown()) Q.push(temp->down);
        if(temp->moveRight()) Q.push(temp->right);
    }
}

void Tree::DFS()
{
    stack<Node *> S;
    S.push(root);
    maxDepth = 0;
    while(!S.empty())
    {
        nodeCount++;
        Node *temp = S.top();
        S.pop();
        if(temp->depth > maxDepth)
            maxDepth = temp->depth;
        if(temp->isGoal())
        {
            cout << "Depth-First Search" << endl;
            print(temp);
            return;
        }
        if(temp->moveLeft() && temp->depth < 10) S.push(temp->left);
        if(temp->moveUp() && temp->depth < 10) S.push(temp->up);
        if(temp->moveDown() && temp->depth < 10) S.push(temp->down);
        if(temp->moveRight() && temp->depth < 10) S.push(temp->right);
    }
}

void Tree::ASTAR()
{
    queue<Node *> Q;
    Q.push(root);
    maxDepth = 0;
    while(!Q.empty())
    {
        nodeCount++;
        Node *temp = Q.front();
        Q.pop();
        if(temp->depth > maxDepth)
            maxDepth = temp->depth;
        if(temp->isGoal())
        {
            cout << "A* Search" << endl;
            print(temp);
            return;
        }
        if(temp->moveLeft())
        {
            if(temp->left->manDist < temp->manDist)
                Q.push(temp->left);
            else
                delete temp->left;
        }
        if(temp->moveUp())
        {
            if(temp->up->manDist < temp->manDist)
                Q.push(temp->up);
            else
                delete temp->up;
        }
        if(temp->moveDown())
        {
            if(temp->down->manDist < temp->manDist)
                Q.push(temp->down);
            else
                delete temp->down;
        }
        if(temp->moveRight())
        {
            if(temp->right->manDist < temp->manDist)
                Q.push(temp->right);
            else
                delete temp->right;
        }
    }
}

int getManDist(Node *curr)
{
    int manDist = 0, rDist, cDist;
    for(int i=0; i<3; i++)
    {
        for(int j=0; j<3; j++)
        {
            int rDist, cDist;
            for(int a = 0; a < 3; a++)
            {
                for(int b = 0; b < 3; b++)
                {
                    if(curr->state[i][j] == goal[a][b])
                    {
                        rDist = abs(a - i);
                        cDist = abs(b - j);
                    }
                }
            }
            manDist +=  rDist + cDist;
        }
    }
    return manDist;
}

int main()
{
    char choice;
    cout << "Which search do you want to perform?" << endl;
    cout << "Enter B for Breadth-First Search" << endl;
    cout << "Enter D for Depth-First Search" << endl;
    cout << "Enter A for A* Search with Manhattan Distance heuristic" << endl;
    cout << "Enter anything else to exit" << endl;
    cout << endl;
    while(cin >> choice)
    {
        cout << endl;
        if(choice == 'B')
        {
            clock_t start, finish, total;
            start = clock();

            Tree t;
            t.plantTree(0);

            finish = clock();
            total = finish - start;
            cout << "Time: " << total << " milliseconds" << endl;
            cout << "Expanded Node Count: " << t.getNodeCount() << endl;
            cout << "Size of Memory: " << sizeof(Node*) * t.getNodeCount() << " bytes" << endl;
            cout << "Max Stack/Queue Depth: " << t.getMaxDepth() << endl;
            cout << endl;
        }
        else if(choice == 'D')
        {
            clock_t start, finish, total;
            start = clock();

            Tree t;
            t.plantTree(1);

            finish = clock();
            total = finish - start;
            cout << "Time: " << total << " milliseconds" << endl;
            cout << "Expanded Node Count: " << t.getNodeCount() << endl;
            cout << "Size of Memory: " << sizeof(Node*) * t.getNodeCount() << " bytes" << endl;
            cout << "Max Stack/Queue Depth: " << t.getMaxDepth() << endl;
            cout << endl;
        }
        else if(choice == 'A')
        {
            clock_t start, finish, total;
            start = clock();

            Tree t;
            t.plantTree(2);

            finish = clock();
            total = finish - start;
            cout << "Time: " << total << " milliseconds" << endl;
            cout << "Expanded Node Count: " << t.getNodeCount() << endl;
            cout << "Size of Memory: " << sizeof(Node*) * t.getNodeCount() << " bytes" << endl;
            cout << "Max Stack/Queue Depth: " << t.getMaxDepth() << endl;
            cout << endl;
        }
        else
            break;
    }
	return 0;
}
