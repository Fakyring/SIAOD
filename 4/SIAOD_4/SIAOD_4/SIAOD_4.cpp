#include <iostream>
#include <random>
#include <string>
#include <fstream>
#include <map>
#include <chrono>
#include <regex>

using namespace std;
using namespace chrono;

int rotates, checks;
class Node {
public:
	int key;
	string data;
	Node* left;
	Node* right;
	int height;
};

int height(Node* N) {
	if (N == NULL)
		return 0;
	return N->height;
}

int max(int a, int b) {
	return (a > b) ? a : b;
}

//Создание узла
Node* newNode(int key, string data) {
	Node* node = new Node();
	node->key = key;
	node->data = data;
	node->left = NULL;
	node->right = NULL;
	node->height = 1;
	return (node);
}

//Поворот вправо
Node* rightRotate(Node* y) {
	Node* x = y->left;
	Node* T2 = x->right;
	x->right = y;
	y->left = T2;
	y->height = max(height(y->left), height(y->right)) + 1;
	x->height = max(height(x->left), height(x->right)) + 1;
	return x;
}

//Поворот влево
Node* leftRotate(Node* x) {
	Node* y = x->right;
	Node* T2 = y->left;
	y->left = x;
	x->right = T2;
	x->height = max(height(x->left), height(x->right)) + 1;
	y->height = max(height(y->left), height(y->right)) + 1;
	return y;
}

//Определить фактор сбалансированности
int getBalanceFactor(Node* N) {
	if (N == NULL)
		return 0;
	return height(N->left) - height(N->right);
}

//Добавить узел
Node* insertNode(Node* node, int key, string data, int mode) {
	if (node == NULL)
		return (newNode(key, data));
	if (key < node->key)
		node->left = insertNode(node->left, key, data, mode);
	else if (key > node->key)
		node->right = insertNode(node->right, key, data, mode);
	else
		return node;
	if (mode == 1)
	{
		node->height = 1 + max(height(node->left), height(node->right));
		int balanceFactor = getBalanceFactor(node);
		if (balanceFactor > 1) {
			if (key < node->left->key) {
				rotates++;
				return rightRotate(node);
			}
			else if (key > node->left->key) {
				rotates++;
				node->left = leftRotate(node->left);
				rotates++;
				return rightRotate(node);
			}
		}
		if (balanceFactor < -1) {
			if (key > node->right->key) {
				rotates++;
				return leftRotate(node);
			}
			else if (key < node->right->key) {
				rotates++;
				node->right = rightRotate(node->right);
				rotates++;
				return leftRotate(node);
			}
		}
	}
	return node;
}

Node* createNew(Node* root, int size, int mode) {
	int key;
	string s[5] = { "МИРЭА", "МАИ", "МЭИ", "РЭУ", "МГУ" };
	random_device rd;
	mt19937 rmt(rd());
	uniform_int_distribution<int> random(100000, 999999);
	for (int i = 0; i < size; i++)
	{
		key = random(rmt);
		root = insertNode(root, key, s[rand() % 5], mode);
	}
	return root;
}

//Найти узел с наименьшим значением
Node* nodeWithMinimumValue(Node* node) {
	Node* current = node;
	while (current->left != NULL)
		current = current->left;
	return current;
}

//Удалить узел
Node* deleteNode(Node* root, int key, int mode) {
	if (root == NULL)
		return root;
	if (key < root->key)
		root->left = deleteNode(root->left, key, mode);
	else if (key > root->key)
		root->right = deleteNode(root->right, key, mode);
	else {
		if ((root->left == NULL) || (root->right == NULL)) {
			Node* temp = root->left ? root->left : root->right;
			if (temp == NULL) {
				temp = root;
				root = NULL;
			}
			else
				*root = *temp;
			free(temp);
		}
		else {
			Node* temp = nodeWithMinimumValue(root->right);
			root->key = temp->key;
			root->data = temp->data;
			root->right = deleteNode(root->right, temp->key, mode);
		}
	}
	if (root == NULL)
		return root;
	if (mode == 1) {
		root->height = 1 + max(height(root->left), height(root->right));
		int balanceFactor = getBalanceFactor(root);
		if (balanceFactor > 1) {
			if (getBalanceFactor(root->left) >= 0) {
				return rightRotate(root);
			}
			else {
				root->left = leftRotate(root->left);
				return rightRotate(root);
			}
		}
		if (balanceFactor < -1) {
			if (getBalanceFactor(root->right) <= 0) {
				return leftRotate(root);
			}
			else {
				root->right = rightRotate(root->right);
				return leftRotate(root);
			}
		}
	}
	return root;
}

void printTree(Node* root, string indent, bool last) {
	if (root != nullptr) {
		cout << indent;
		if (last) {
			cout << "R----";
			indent += "   ";
		}
		else {
			cout << "L----";
			indent += "|  ";
		}
		cout << root->key << endl;
		printTree(root->left, indent, false);
		printTree(root->right, indent, true);
	}
}

void printBinTree(Node* root, int level, int L) {
	if (root != nullptr) {
		printBinTree(root->right, level + 1, L);
		for (int i = 0; i < level * L; i++) {
			cout << " ";
		};
		cout << root->key << endl;
		printBinTree(root->left, level + 1, L);
	}
	
}

Node* find(Node* root, int key) {
	checks++;
	if (root == nullptr || key == root->key)
	{
		return root;
	}
	else if (key < root->key) {
		find(root->left, key);
	}
	else {
		find(root->right, key);
	}
}

//Считать из файла
Node* readFile(fstream& f, Node* root) {
	map<int, string> univers;
	rotates = 0;
	string s = "";
	char* endLiner;
	int k = 0;
	while (f.good() && !f.eof())
	{
		f.read((char*)&k, sizeof(int));
		f.read((char*)&endLiner, sizeof('\n'));
		getline(f, s, '\0');
		univers.insert({ k, s });
	}
	for (auto& kv : univers)
	{
		root = insertNode(root, kv.first, kv.second, 1);
	}
	cout << "Среднее количество поворотов = " << ((double)rotates / univers.size()) << endl;
	return root;
}

//Записать в файл
void writeFile(fstream& f, Node* root) {
	if (root != nullptr)
	{
		writeFile(f, root->left);
		writeFile(f, root->right);
		f.write(reinterpret_cast<char*>(&root->key), sizeof(int));
		f.write("\0", sizeof(char));
		f.write(root->data.c_str(), root->data.size());
		f.write("\0", sizeof(char));
	}
}

//Удалить дерево
void deleteTree(Node* root)
{
	if (root == NULL) return;
	deleteTree(root->left);
	deleteTree(root->right);
	delete root;
}

//Текстовый в бинарный
void textToBin() {
	int key;
	string str, info;
	fstream txt("input.bin", ios::in);
	fstream bin("university", ios::out | ios::binary);
	while (getline(txt, info)) {
		regex rgx("\\s+");
		sregex_token_iterator iter(info.begin(), info.end(), rgx, -1);
		key = stoi(*iter);
		str = info.substr(to_string(key).size() + 1);
		bin.write(reinterpret_cast<char*>(&key), sizeof(int));
		bin.write("\0", sizeof(char));
		bin.write(str.c_str(), str.size());
		bin.write("\0", sizeof(char));
	}
	txt.close();
	bin.close();
}

int main() {
	setlocale(0, "");
	system("chcp 1251");
	Node* root = NULL, * tmp = NULL;
	string value;
	int key = 0, a = 0, count = 0, mode = 1;
	while (true)
	{
		cout << "0 - Создать БДП дерево\n";
		cout << "1 - Создать СДП дерево\n";
		cout << "2 - Добавить элемент\n";
		cout << "3 - Найти элемент\n";
		cout << "4 - Удалить элемент\n";
		cout << "5 - Вывести дерево\n";
		cout << "6 - Записать в файл\n";
		cout << "7 - Считать из файла\n";
		cout << "8 - Текстовый файл в бинарный\n";

		cout << "Введите номер операции: ";
		cin >> a;
		switch (a)
		{
		case 0: {
			mode = 0;
			cout << "Введите размер дерева: ";
			cin >> count;
			deleteTree(root);
			root = NULL;
			root = createNew(root, count, mode);
			break;
		}
		case 1: {
			mode = 1;
			cout << "Введите размер дерева: ";
			cin >> count;
			deleteTree(root);
			root = NULL;
			root = createNew(root, count, mode);
			break;
		}
		case 2: {
			cout << "Введите ключ: ";
			cin >> key;
			cout << "Введите значение: ";
			cin >> value;
			root = insertNode(root, key, value, mode);
			break;
		}
		case 3: {
			checks = 0;
			cout << "Введите ключ для поиска: ";
			cin >> key;
			auto start = high_resolution_clock::now();
			tmp = find(root, key);
			auto stop = high_resolution_clock::now();
			if (tmp != nullptr) {
				cout << "Узел " << key << " имеет адрес " << &tmp->data << endl;
			}
			cout << "Проверок: " << checks << endl;
			cout << "Время поиска: " << duration_cast<nanoseconds>(stop - start).count() << endl;
			break;
		}
		case 4: {
			cout << "Введите ключ, по которому удалить элемент: ";
			cin >> key;
			deleteNode(root, key, mode);
			break;
		}
		case 5: {
			//printTree(root, "", true);
			printBinTree(root, 0, 6);
			break;
		}
		case 6: {
			fstream f("university", ios::out | ios::binary);
			writeFile(f, root);
			f.close();
			break;
		}
		case 7: {
			deleteTree(root);
			root = NULL;
			fstream f("university", ios::in | ios::binary);
			root = readFile(f, root);
			f.close();
			break;
		}
		case 8: {
			textToBin();
			break;
		}
		default:
			exit(0);
			break;
		}
		cout << endl;
	}
}