#include <iostream>
#include <fstream>
#include <string>
#include <Windows.h>
#include <conio.h>

using namespace std;

struct Contract
{
	int id=0;
	string f;
	string i;
	string o;
	string theme;
	double cost=0;
};


bool Avtorizator()
{
	int p=5;
	setlocale(0, "");
	bool cont = false;
	char database[10][20] = { "User", "123" };
	char login[20], password[20];
	while (p > 0)
	{
		p--;
		system("cls");
		cout << "Введите логин: ";
		cin >> login;
		cout << "\nВведите пароль: ";
		cin >> password;
		for (int i = 0; database[i][0]; i += 2)
		{
			if (!strcmp(login, database[i]))
			{
				if (!strcmp(password, database[i + 1]))
					cont = true;
			}
		}
		if (!cout) cout << "Логин или пароль неверны.\nКоличество попыток: " << p;
		else break;
		system("pause > nul");
	}
	if (cont) {
		system("cls");
		cout << "Вход выполнен успешно." << endl << endl;
		return true;
	}
	else cout << "\nВы исчерпали лимит попыток!";
	system("pause > nul");

	return false;
}

void PrintContract(Contract& c)
{
	cout << "Номер договора: " << c.id << endl;
	cout << "Фамилия: " << c.f << endl;
	cout << "Имя: " << c.i << endl;
	cout << "Отчество: " << c.o << endl;
	cout << "Предмет договора: " << c.theme << endl;
	cout << "Цена доровора: " << c.cost << " руб." << endl;

}

bool ReadBlock(istream& f, Contract& contract)
{
	string s;
	getline(f, s);
	if (f.fail())return false;
	contract.id = stoi(s);
	getline(f, contract.f);
	getline(f, contract.i);
	getline(f, contract.o);
	getline(f, contract.theme);
	getline(f, s);
	contract.cost = stod(s);
	return !f.fail();
}

bool DataInitialization()
{
	bool fileExist = false;
	if (ifstream("Buffer.txt"))
		fileExist = true;
	else {
		ofstream _buf("Buffer.txt");
		if (!_buf)
			cout << "Ошибка создания буферного файла!" << endl;
		_buf.close();
	}
	return fileExist;
}

void DataEntry()
{
	int _number;
	string _surname;
	string _Name;
	string _patronymic;
	string _subject;
	double _price;
	int n;
	cout << "Введите количество данных: ";
	cin >> n;
	ofstream record("Buffer.txt", ios::app);

	if (record)
	{
		record << n << endl;

		for (int i = 0; i < n; i++)
		{
			cout << "Номер договора: ";
			cin >> _number;
			cout << "Фамилия: ";
			cin >> _surname;
			_surname = _surname.substr(0, 15);
			cout << "Имя: ";
			cin >> _Name;
			_Name = _Name.substr(0, 15);
			cout << "Отчество: ";
			cin >> _patronymic;
			_patronymic = _patronymic.substr(0, 15);
			cout << "Предмет договора: ";
			cin >> _subject;
			_subject = _subject.substr(0, 100);
			cout << "Цена договора: ";
			cin >> _price;
			record << _number << endl;
			record << _surname << endl;
			record << _Name << endl;
			record << _patronymic << endl;
			record << _subject << endl;
			if (i < n - 1)
				record << _price << endl;
			else
				record << _price;
			cout << "_____________" << endl;
		}
	}
	else
		cout << "Ошибка открытия файла" << endl;
	record.close();
}

void findNumber()
{
	char n[400];
	cout << "Введите число: ";
	cin >> n;
	ifstream reading("Buffer.txt"); // Считывает данные с файла
	char findNumber[400]; 
	int recordsCount;
	reading.getline(findNumber, 200); 
	recordsCount = atoi(findNumber); // Удаляет пробелы??? и преобразует в int
	int index = 5;
	do {
		index++;
		reading.getline(findNumber, 400); // Проверяет каждую строку		
		if (index % 6 == 0 and strcmp(findNumber, n) == 0) // Если без остатка делится на 0 и одинаков с числом n
		{
			cout << findNumber << endl; //Проверка номера строки
			break;
		}
	} while (!reading.eof()); // Пока файл не закончится

	if (strcmp(findNumber, n) == 0)
	{
		ifstream oleg("Buffer.txt");
		int m = 0; //номер строки
		string line;
		while (getline(oleg, line)) 
		{
			m += 1;
			if (findNumber == line) 
			{
				cout << "Найдено в строке #" << m << endl;
				string str;
			
				int i = 0;
				int l = m + 6;
				while (getline(oleg, str))
				{
					if (i >= 0 and i <= 4)
					{
						cout << str << endl;
					}
					i++;
				}
				oleg.close();
				break; 
			}
		}
	}
	else
		cout << "Ничего не найдено." << endl;
	return;
}

Contract findBySurname(string fileName, int from = 0)
{
	
	Contract contract;
	ifstream f(fileName);
	
	if (f)
	{
		string s;
		string surName;
		cout << "Введите фамилию: ";
		cin >> surName;
		surName = surName.substr(0, 15);
		getline(f, s);
		int blockCount = stoi(s);
		for (int i = 0; i < blockCount; i++)
		{
			bool res = ReadBlock(f, contract);
			auto state = f.rdstate();
			if (i >= from && res && contract.f == surName)
			{
				f.close();
				return contract;
			}
		}
		contract = { -1, "", "", "", "", 0 };
		
		f.close();
		return contract;
	}
	cout << "Ошибка выполнения";
	return contract;
}

Contract findPrice(string fileName, double min, double max, int &from)
{

	Contract contract;
	ifstream cost(fileName);

	if (cost)
	{
		string s;
		getline(cost, s);
		int blockCount = stod(s);
		for (int i = 0; i < blockCount; i++)
		{
			bool res = ReadBlock(cost, contract);
			auto state = cost.rdstate();
			if (i >= from && res && (contract.cost >= min && contract.cost <= max))
			{
				cost.close();
				from = i;
				return contract;
			}
		}
		contract = { -1, "", "", "", "", 0 };

		cost.close();
		from = -1;
		return contract;
	}
	//cout << "Ошибка выполнения";
	from = -1;
	return contract;
}

void DataReading(string fileName)
{
	ifstream reading(fileName);
	ofstream record("Buffer.txt", ios::out);
	if (reading)
	{
		if (record)
		{
			int _number;
			string _surname;
			string _Name;
			string _patronymic;
			string _subject;
			double _price;
			int n;
			reading >> n;
			record << n << endl;
			for (int i = 0; i < n; i++)
			{
				reading >> _number;
				reading >> _surname;
				reading >> _Name;
				reading >> _patronymic;
				reading >> _subject;
				reading >> _price;
				record << _number << endl;
				record << _surname << endl;
				record << _Name << endl;
				record << _patronymic << endl;
				record << _subject << endl;
				if (i < n - 1)
					record << _price << endl;
				else
					record << _price;
			}
			cout << "Данные считаны" << endl;
		}
		else
			cout << "Ошибка открытия буфера!" << endl;
	}
	else
		cout << "Ошибка открытия файла!" << endl;
	reading.close();
	record.close();
}

void Print()
{
	ifstream reading("Buffer.txt");
	if (reading)
	{
		int _number;
		string _surname;
		string _Name;
		string _patronymic;
		string _subject;
		double _price;
		int n = 0;
		reading >> n;
		cout << "Количество данных: " << n << endl << endl;
		for (int i = 0; i < n; i++)
		{
			cout << "Документ №" << i + 1 << endl;
			reading >> _number;
			cout << "Номер договора: " << _number << endl;
			reading >> _surname;
			cout << "Фамилия: " << _surname << endl;
			reading >> _Name;
			cout << "Имя: " << _Name << endl;
			reading >> _patronymic;
			cout << "Отчество: " << _patronymic << endl;
			reading >> _subject;
			cout << "Предмет договора: " << _subject << endl;
			reading >> _price;
			cout << "Цена договора: " << _price << endl;
			cout << "___________________________" << endl;
		}
	}
	else
		cout << "Ошибка открытия файла" << endl;
	reading.close();
}

void Copy()
{
	ifstream reading("Buffer.txt");
	ofstream record("Buffer_.txt", ios::out);
	if (reading)
	{
		if (record)
		{
			int _number;
			string _surname;
			string _Name;
			string _patronymic;
			string _subject;
			double _price;
			int n;
			reading >> n;
			record << n << endl;
			for (int i = 0; i < n; i++)
			{
				reading >> _number;
				record << _number << endl;
				reading >> _surname;
				record << _surname << endl;
				reading >> _Name;
				record << _Name << endl;
				reading >> _patronymic;
				record << _patronymic << endl;
				reading >> _subject;
				record << _subject << endl;
				reading >> _price;
				if (i < n - 1)
					record << _price << endl;
				else
					record << _price;
			}
		}
		else
			cout << "Ошибка открытия файла!" << endl;
	}
	else
		cout << "Ошибка открытия буферного файла!" << endl;
	record.close();
	reading.close();
}

bool DataCleaning()
{
	bool clear = false;
	fstream _buf("Buffer.txt", ios::out);
	if (!_buf)
		cout << "Ошибка открытия буферного файла!" << endl;
	_buf.clear();
	if (_buf.peek() == EOF)
		clear = true;
	else
		clear = false;
	_buf.close();
	return clear;
}

int AmountOfData()
{
	ifstream _buf("Buffer.txt");
	int n=0;
	if (_buf)
	{
		_buf >> n;
	}
	else
		cout << "Ошибка открытия буферного файла!" << endl;
	_buf.close();
	return n;
}

void DeleteData()
{
	Copy();
	ifstream reading("Buffer_.txt");
	ofstream record("Buffer.txt", ios::out);
	if (reading)
	{
		if (record)
		{
			int _number;
			string _surname;
			string _Name;
			string _patronymic;
			string _subject;
			double _price;
			int n, _n;
			reading >> n;
			int b = n - 1;
			cout << "Выберите номер удаляемого элемента (от 1 до " << n << "): ";
			cin >> _n;
			_n--;
			system("cls");
			record << b << endl;
			if (_n >= 0 && _n < n)
			{
				for (int i = 0; i < n; i++)
				{
					if (i != _n)
					{
						reading >> _number;
						record << _number << endl;
						reading >> _surname;
						record << _surname << endl;
						reading >> _Name;
						record << _Name << endl;
						reading >> _patronymic;
						record << _patronymic << endl;
						reading >> _subject;
						record << _subject << endl;
						reading >> _price;
						if (i < n - 1)
							record << _price << endl;
						else
							record << _price;
					}
					else
					{
						reading >> _number;
						reading >> _surname;
						reading >> _Name;
						reading >> _patronymic;
						reading >> _subject;
						reading >> _price;
					}
				}
				cout << "Данные удалены!" << endl;
			}
			else
				cout << "Номер введен не верно!" << endl;
		}
		else
			cout << "Ошибка открытия файла!" << endl;
	}
	else
		cout << "Ошибка открытия буферного файла!" << endl;
	record.close();
	reading.close();
	remove("Buffer_.txt");
}

void AddData()
{
	int _number;
	string _surname;
	string _Name;
	string _patronymic;
	string _subject;
	double _price;
	int n = AmountOfData() + 1;
	ofstream record("Buffer.txt", ios::app);
	ofstream record_("Buffer.txt", ios::out | ios::in);
	if (record_)
	{
		record_ << n << endl;
	}
	else
		cout << "Ошибка открытия буферного файла!" << endl;
	if (record)
	{
		record << endl;
		cout << "Введите данные №1 (Номер договора): ";
		cin >> _number;
		cout << "Введите данные №2 (Фамилия): ";
		cin >> _surname;
		_surname = _surname.substr(0, 15);
		cout << "Введите данные №3 (Имя): ";
		cin >> _Name;
		_Name = _Name.substr(0, 15);
		cout << "Введите данные №4 (Отчество): ";
		cin >> _patronymic;
		_patronymic = _patronymic.substr(0, 15);
		cout << "Введите данные №5 (Предмет договора): ";
		cin >> _subject;
		_subject = _subject.substr(0, 100);
		cout << "Введите данные №6 (Цена договора): ";
		cin >> _price;
		record << _number << endl;
		record << _surname << endl;
		record << _Name << endl;
		record << _patronymic << endl;
		record << _subject << endl;
		record << _price << endl;
	}
	else
		cout << "Ошибка открытия файла!" << endl;
	record.close();
	record_.close();
}

void SaveData(string fileName)
{
	ifstream reading("Buffer.txt");
	ofstream record(fileName, ios::out);
	if (reading)
	{
		if (record)
		{
			int _number;
			string _surname;
			string _Name;
			string _patronymic;
			string _subject;
			double _price;
			int n;
			reading >> n;
			record << n << endl;
			for (int i = 0; i < n; i++)
			{
				reading >> _number;
				record << _number << endl;
				reading >> _surname;
				record << _surname << endl;
				reading >> _Name;
				record << _Name << endl;
				reading >> _patronymic;
				record << _patronymic << endl;
				reading >> _subject;
				record << _subject << endl;
				reading >> _price;
				if (i < n - 1)
					record << _price << endl;
				else
					record << _price;
			}
			cout << "Данные сохранены в файле " << fileName << endl;
		}
		else
			cout << "Ошбика открытия буферного файла!" << endl;
	}
	else
		cout << "Ошибка отыкрытия файла!" << endl;
	record.close();
	reading.close();
}

void FindByPriceRange()
{
	system("cls");
	Contract contract;
	int from = 0;
	double min, max;
	cout << "Введите нижний диапазон цены: ";
	cin >> min;
	cout << "Введите верхний диапазон цены: ";
	cin >> max;
	cin.get();

	contract = findPrice("Buffer.txt", min, max, from);
	if (from < 0)
	{
		// not found
		cout << "Товар не найден\n";
		_getch();
		system("cls");
		return;
	}
	do
	{
		PrintContract(contract);
		contract = findPrice("Buffer.txt", min, max, ++from);
	} while (from >= 0);

	system("pause");
	system("cls");
}

int _stateMenu;

void Menu()
{

	cout << "Выберите действие:" << endl
		<< "(0) Выход из программы." << endl
		<< "(1) Ввод данных." << endl
		<< "(2) Вывод данных." << endl
		<< "(3) Удаление данных." << endl
		<< "(4) Добавление данных." << endl
		<< "(5) Копирование данных в другой файл." << endl
		<< "(6) Поиск по номеру договора." << endl
		<< "(7) Поиск по фамилии." << endl
		<< "(8) Поиск по диапазону цен договора." << endl
		<< "(10) Очистка данных." << endl
		<< "Ваш выбор: ";
	cin >> _stateMenu;
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	DataInitialization();

	if (Avtorizator()) // Если комментить, то ставить true
	{

		int _actions;
		string fileName;
		Contract c; 
		_stateMenu = -1;
		while (_stateMenu != 0)
		{
			Menu();
			switch (_stateMenu)
			{
			case 0:
				cout << "Работа завершена." << endl;
				return 0;
			case 1:
				system("cls");

				cout << "Ввод вручную или из файла?: ";
				cin >> _actions;

				system("cls");

				if (_actions == 1)
				{
					DataEntry();
				}
				else
				{
					cout << "Введите название файла: ";
					cin >> fileName;

					DataReading(fileName);
				}
				system("pause");
				system("cls");
				break;
			case 2:
				system("cls");
				Print();
				system("pause");
				system("cls");
				break;
			case 3:
				system("cls");
				DeleteData();
				system("pause");
				system("cls");
				break;
			case 4:
				system("cls");
				AddData();
				system("pause");
				system("cls");
				break;
			case 5:
				system("cls");
				cout << "Введите название файла: ";
				cin >> fileName;
				system("cls");
				SaveData(fileName);
				system("pause");
				system("cls");
				break;
			case 6:
				system("cls");
				findNumber();
				system("pause");
				system("cls");
				break;
			case 7:
				system("cls");	
				c = findBySurname("Buffer.txt");
				if (c.id != -1) PrintContract(c);
				system("pause");
				system("cls");
				break;
			case 8:
				FindByPriceRange();
				break;
			case 10:
				system("cls");
				DataCleaning();
				if (DataCleaning())
					cout << "Данные очищены!" << endl;
				else
					cout << "Данные НЕ очищены!" << endl;
				system("pause");
				system("cls");
				break;
			default:
				cout << "Неправильно выбран пункт меню!" << endl;
				system("pause");
				system("cls");
			}
		}
		system("pause");
	}
}