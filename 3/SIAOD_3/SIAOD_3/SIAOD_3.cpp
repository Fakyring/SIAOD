#include <iostream>
#include <iomanip>
#include <io.h>
#include <fcntl.h>

using namespace std;

struct BTNode
{
	int data;
	struct BTNode* left;
	struct BTNode* right;
};

BTNode* createBTNode(char data)
{
	BTNode* p = new BTNode;
	if (p != NULL)
	{
		p->data = data;
		p->left = NULL;
		p->right = NULL;
	}
	return p;
}

void createBTree(BTNode** root, int arr[], int start, int end)
{
	if (start <= end)
	{
		int mid = (start + end + 1) / 2;
		*root = createBTNode(arr[mid]);
		createBTree(&((*root)->left), arr, start, mid - 1);
		createBTree(&((*root)->right), arr, mid + 1, end);
	}
}

void printBT(const wstring& prefix, const BTNode* node, bool isRight)
{
	if (node != nullptr)
	{
		wcout << prefix;
		wcout << (isRight ? L"├───" : L"└───");
		wcout << (char)node->data << endl;
		printBT(prefix + (isRight ? L"│   " : L"    "), node->right, true);
		printBT(prefix + (isRight ? L"│   " : L"    "), node->left, false);
	}
}

void printBT(const BTNode* node)
{
	printBT(L"", node, false);
}

BTNode* getMostLeft(BTNode* p) {
	if (p->left)
	{
		getMostLeft(p->left);
	}
	else
	{
		return p;
	}
}

int findValue(BTNode* p, char needle, int steps) {
	if (p != nullptr)
	{
		if (needle == p->data)
		{
			return steps;
		}
		if (needle > p->data)
		{
			findValue(p->right, needle, ++steps);
		}
		else {
			findValue(p->left, needle, ++steps);
		}
	}
	else {
		return -1;
	}
}

char getMaxValue(BTNode* p) {
	if (p->right != nullptr)
	{
		getMaxValue(p->right);
	}
	else {
		return p->data;
	}
}

int compare(const void* x1, const void* x2)
{
	return (*(int*)x1 - *(int*)x2);
}

int main(int argc, char* argv[])
{
	_setmode(_fileno(stdout), _O_U16TEXT);
	char ch;
	int res, n = 0, a;
	wcout << "Enter array size (x >= 1, x <= 50): ";
	cin >> n;
	if (n < 0) {
		n = 1;
	}
	else if (n > 50) {
		n = 50;
	}
	int* arr = new int[n];
	srand(time(NULL));
	for (int i = 0; i < n; i++)
	{
		arr[i] = rand() % 26 + 97;
	}
	qsort(arr, n, sizeof(int), compare);
	for (int i = 0; i < n; i++)
	{
		wcout << (char)arr[i] << " ";
	}
	wcout << endl;
	BTNode* root = NULL;
	createBTree(&root, arr, 0, n - 1);
	while (true)
	{
		wcout << "1-Most left node\n2-Path length to element\n3-Get max element of the tree\n4-Print tree\nChoose: ";
		cin >> a;
		switch (a)
		{
		case 1: {
			wcout << "Most left node: " << (char)getMostLeft(root)->data << endl;
			break;
		}
		case 2: {
			wcout << "Enter char (a-z): ";
			cin >> ch;
			res = findValue(root, ch, 0);
			if (res == -1)
			{
				wcout << "No such value";
			}
			else
			{
				wcout << "There is(are) " << res << " step(s) to that value from the root";
			}
			wcout << endl;
			break;
		}
		case 3: {
			wcout << "Most value of binary tree: " << getMaxValue(root) << endl;
			break;
		}
		case 4: {
			printBT(root);
			break;
		}
		default:
			exit(0);
			break;
		}
		wcout << endl;
	}
	return 0;
}