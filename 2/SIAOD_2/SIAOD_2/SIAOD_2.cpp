#include <iostream>
#include <list>
#include <regex>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <random>

using namespace std;
using namespace chrono;

struct HashElement {
	bool empty;
	string name;
	int key;
	HashElement(bool empty, int key, string name) {
		this->empty = empty;
		this->key = key;
		this->name = name;
	}
	HashElement() {}
};
class Hash {
	int BUCKET;
	int count = 0;
	list <HashElement>* table;
public:
	Hash(int V);
	void createNew(Hash& hash_table, int size);
	void readFile(Hash& hash_table);
	void writeFile();
	void insertItem(Hash& hash_table, int key, string content);
	void deleteItem(int key);
	int hashFunction(int x) {
		return (x % BUCKET);
	}
	void rehash(Hash& old_hash);
	void displayHash();
	HashElement find(int key);
};
Hash::Hash(int b) {
	this->BUCKET = b;
	table = new list <HashElement>[BUCKET];
}
void Hash::createNew(Hash& hash_table, int size) {
	int key;
	string s[5] = { "МИРЭА", "МАИ", "МЭИ", "РЭУ", "МГУ" };
	random_device rd;
	mt19937 rmt(rd());
	uniform_int_distribution<int> random(100000, 999999);
	for (int i = 0; i < size; i++)
	{
		key = random(rmt);
		insertItem(hash_table, key, s[rand() % 5]);
	}
}
//Считать из файла
void Hash::readFile(Hash& hash_table) {
	string str;
	int key;
	fstream f("university.bin", ios::in | ios::binary);
	while (getline(f, str))
	{
		regex rgx("\\s+");
		sregex_token_iterator iter(str.begin(), str.end(), rgx, -1);
		key = stoi(*iter);
		insertItem(hash_table, key, str.substr(to_string(key).size() + 1));
	}
	f.close();
}
//Записать в файл
void Hash::writeFile() {
	string str;
	fstream f("university.bin", ios::out | ios::binary);
	for (int i = 0; i < BUCKET; i++)
	{
		for (HashElement x : table[i])
		{
			if (!x.empty)
			{
				str = to_string(x.key) + " " + x.name;
				str += "\n";
				f.write(str.c_str(), size(str));
			}
		}
	}
}
//Добавить элемент
void Hash::insertItem(Hash& hash_table, int key, string content) {
	int index = hashFunction(key);
	HashElement* he = new HashElement(false, key, content);
	list<HashElement>::iterator iter;
	for (iter = table[index].begin(); iter != table[index].end(); iter++) {
		if (((HashElement)*iter).empty)
		{
			*iter = *he;
			break;
		}
	}
	if (iter == table[index].end()) {
		table[index].push_front(*he);
	}
	count++;
	if (count >= BUCKET * 0.95)
	{
		rehash(hash_table);
	}
}
//Удалить элемент
void Hash::deleteItem(int key) {
	int index = hashFunction(key);
	for (list<HashElement>::iterator iter = table[index].begin(); iter != table[index].end(); iter++) {
		if (((HashElement)*iter).key == key) {
			HashElement& x(*iter);
			x.empty = true;
			count--;
			writeFile();
			cout << "Запись удалена" << endl;
			return;
		}
	}
	cout << "Запись не найдена" << endl;
}
//Рехеширование
void Hash::rehash(Hash& old_hash) {
	BUCKET = BUCKET + 10;
	Hash new_hash(BUCKET);
	for (int i = 0; i < BUCKET - 10; i++) {
		for (HashElement x : table[i]) {
			if (!x.empty) {
				new_hash.table[hashFunction(x.key)].push_back(x);
				new_hash.count++;
			}
		}
	}
	old_hash = new_hash;
}
//Поиск элемента
HashElement Hash::find(int key) {
	int index = hashFunction(key);
	for (HashElement x : table[index]) {
		if (x.key == key) {
			return x;
		}
	}
	return HashElement();
}
//Вывод таблицы
void Hash::displayHash() {
	for (int i = 0; i < BUCKET; i++) {
		cout << i;
		for (HashElement x : table[i])
			cout << " --> " << x.key << " " << x.name << " " << (x.empty == 1 ? "true" : "false");
		cout << endl;
	}
	cout << endl;
}
int main() {
	system("chcp 1251");
	setlocale(0, "");
	int a, key, size = 0;
	string content, str;
	Hash hash_table(10);
	while (true)
	{
		cout << "0 - Выход\n";
		cout << "1 - Создать новую таблицу\n";
		cout << "2 - Добавить элемент\n";
		cout << "3 - Удалить элемент\n";
		cout << "4 - Считать файл\n";
		cout << "5 - Записать в файл\n";
		cout << "6 - Найти запись\n";
		cout << "7 - Рехешировать\n";
		cout << "Выберите действие: ";
		cin >> a;
		switch (a)
		{
		case 1: {
			cout << "Введите размер новой таблицы: ";
			cin >> size;
			hash_table = Hash(10);
			hash_table.createNew(hash_table, size);
			hash_table.displayHash();
			break;
		}
		case 2: {
			cout << "Введите код специализации (999999): ";
			cin >> key;
			if (key < 100000 || key > 999999)
			{
				cout << "Вы не справились с вводом кода специализации.\nДо свидания!";
				exit(0);
			}
			cout << "Введите название ВУЗа: ";
			cin >> content;
			hash_table.insertItem(hash_table, key, content);
			hash_table.displayHash();
			break;
		}
		case 3: {
			hash_table.displayHash();
			cout << "Введите код специализации: ";
			cin >> key;
			hash_table.deleteItem(key);
			hash_table.displayHash();
			break;
		}
		case 4: {
			hash_table = Hash(10);
			hash_table.readFile(hash_table);
			hash_table.displayHash();
			break;
		}
		case 5: {
			hash_table.writeFile();
			break;
		}
		case 6: {
			cout << "Введите ключ для поиска: ";
			cin >> key;
			auto start = high_resolution_clock::now();
			HashElement he = hash_table.find(key);
			auto stop = high_resolution_clock::now();
			if (he.key >= 0)
			{
				cout << "Ваша запись: " << hash_table.hashFunction(he.key) << " " << he.key << " " << he.name << endl;
				cout << "Время поиска: " << duration_cast<nanoseconds>(stop - start).count() << endl;
			}
			break;
		}
		case 7: {
			hash_table.rehash(hash_table);
			hash_table.displayHash();
			break;
		}
		default:
			cout << "До свидания";
			exit(0);
			break;
		}
	}
	return 0;
}