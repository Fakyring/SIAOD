#include <iostream>

using namespace std;

void toBin(unsigned int x)
{
	int n = sizeof(int) * 8;
	unsigned  mask = (1 << (n - 1));

	for (int i = 1; i <= n; i++)
	{
		cout << ((x & mask) >> (n - i));
		mask = mask >> 1;
	}
}
int logOr(int x, int mask)
{
	return x | mask;
}
int logAnd(int x, int mask)
{
	return x & mask;
}
int logMult(int x)
{
	return x << 6;
}
int logDiv(int x)
{
	return x >> 6;
}
int main()
{
	setlocale(0, "");
	cout << "1) Превратить в 1 1-ый, 15-ый и 17-ый биты статичного числа" << endl;
	cout << "2) Ввести с клавиатуры число, превратить в 0 1-ый, 15-ый и 17-ый биты введенного числа" << endl;
	cout << "3) Умножить введенное с клавиатуры число на 64" << endl;
	cout << "4) Разделить введенное с клавиатуры число на 64" << endl;
	cout << "5) Ввести число с клавиатуры и номер бита, который нужно обнулить в 1(0), используя маску с 1 в старшем разряде" << endl;
	cout << "Выберите задание: ";
	int a;
	int n, mask, b;
	cin >> a;
	switch (a)
	{
	case 1: {
		n = 4;
		mask = 0x28002;
		cout << "До обработки" << endl;
		cout << "Число в 10-чной системе: " << n << endl;
		cout << "Число в 2-чной системе: ";
		toBin(n);
		cout << endl;
		cout << "Маска в 10-чной системе: " << mask << endl;
		cout << "Маска в 2-чной системе: ";
		toBin(mask);
		cout << endl;
		cout << "После обработки" << endl;
		cout << "Число в 10-чной системе: " << logOr(n, mask) << endl;
		cout << "Число в 2-чной системе: ";
		toBin(logOr(n, mask));
		cout << endl;
		break;
	}
	case 2: {
		cout << "Введите число в 10-чной системе: ";
		cin >> n;
		mask = 0xFFFFFFC7;
		cout << "До обработки" << endl;
		cout << "Число в 10-чной системе: " << n << endl;
		cout << "Число в 2-чной системе: ";
		toBin(n);
		cout << endl;
		cout << "Маска в 10-чной системе: " << mask << endl;
		cout << "Маска в 2-чной системе: ";
		toBin(mask);
		cout << endl;
		cout << "После обработки" << endl;
		cout << "Число в 10-чной системе: " << logAnd(n, mask) << endl;
		cout << "Число в 2-чной системе: ";
		toBin(logAnd(n, mask));
		cout << endl;
		break;
	}
	case 3: {
		cout << "Введите число в 10-чной системе: ";
		cin >> n;
		cout << "До обработки" << endl;
		cout << "Число в 10-чной системе: " << n << endl;
		cout << "Число в 2-чной системе: ";
		toBin(n);
		cout << endl;
		cout << "После обработки" << endl;
		cout << "Число в 10-чной системе: " << logMult(n) << endl;
		cout << "Число в 2-чной системе: ";
		toBin(logMult(n));
		cout << endl;
		break;
	}
	case 4: {
		cout << "Введите число в 10-чной системе: ";
		cin >> n;
		cout << "До обработки" << endl;
		cout << "Число в 10-чной системе: " << n << endl;
		cout << "Число в 2-чной системе: ";
		toBin(n);
		cout << endl;
		cout << "После обработки" << endl;
		cout << "Число в 10-чной системе: " << logDiv(n) << endl;
		cout << "Число в 2-чной системе: ";
		toBin(logDiv(n));
		cout << endl;
		break;
	}
	case 5: {
		cout << "Введите число в 10-чной системе: ";
		cin >> n;
		cout << "Введите номер бита для изменения: ";
		cin >> b;
		mask = 0x80000000;
		mask = ~abs(mask >> 31 - b);
		cout << "До обработки" << endl;
		cout << "Число в 10-чной системе: " << n << endl;
		cout << "Число в 2-чной системе: ";
		toBin(n);
		cout << endl;
		cout << "Маска в 10-чной системе: " << mask << endl;
		cout << "Маска в 2-чной системе: ";
		toBin(mask);
		cout << endl;
		cout << "После обработки" << endl;
		cout << "Число в 10-чной системе: " << logAnd(n, mask) << endl;
		cout << "Число в 2-чной системе: ";
		toBin(logAnd(n, mask));
		cout << endl;
		break;
	}
	default: {
		cout << "Неправильный номер" << endl;
		system("pause");
		exit(0);
		break;
	}
	}
	system("pause");
}