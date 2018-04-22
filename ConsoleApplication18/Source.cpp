#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>
#include <conio.h>
#include <locale.h>

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

enum usT
{administrator, diller,	buyer};

struct product
{
	int code;
	char desc[150];
	int rPrice;
	int oPrice;
	int count;
	char gar[10];
};

struct bask
{
	int vp;
	int code;
	char desc[150];
	int Price;
	int kol;
	int sum;
};

struct user
{
	int id;
	char login[30];
	char password[30];
	usT Type;
};

void yellow();
void white();
void red();
void green();
void GotoXY(int X, int Y);
user *creatStructUser(int *r);
void Hello();
int avtor();
int menuB(int a);
int menuA(int a);
int menuKT(int a);
void parsStr(int*code, char*desc, int*rP, int*oP, char*gar, int*count, char *str);
product *creatStructProd(int *r, int vp);
void viewProd(product*pr, int r, int vp, int b);
void sortPr(product*pr, int r);
void strD(char*str, int s);
void Basket(int b);
void viewHist(int b);
void sortPrC(product*pr, int r);
void viewProdA(product*pr, int r, int vp);

void main()
{
	setlocale(LC_ALL, "Ru");
	Hello();
	int r;// количество user-ов	
	int b=avtor();// id user	
	user *users1 = creatStructUser(&r);
	int m,k; //пункт меню
	for (int i = 0; i < r; i++)
	{
		if(users1[i].id==b&&users1[i].Type==buyer) m=menuB(b);
		else if (users1[i].id == b&&users1[i].Type == diller) m = menuB(b);
		else if (users1[i].id == b&&users1[i].Type == administrator) m = menuA(b);
	}
	while (true)
	{
		if (m == 1)
		{
			k = menuKT(b);
			product*prod = creatStructProd(&r, k);
			if (users1[b-1].Type != administrator)
				viewProd(prod, r, k, b);
			else
				viewProdA(prod, r, k);			
		}
		else if(m==2)
		{
			FILE*f;
			system("cls");
			char p[200] = "HISTORY\\";
			char b1[10];
			char tmp[200];
			itoa(b, b1, 10);
			strcat(p, b1);
			if ((f = fopen(p, "r")) != NULL)
			{
				viewHist(b);
				getch();
				m = menuB(b);
			}
			else
			{
				printf("Вы еще не совершали заказов\n");
				getch();
				m = menuB(b);
			}
		}		
	}	
}

void yellow()
{
	SetConsoleTextAttribute(hConsole, 14);
}

void white()
{
	SetConsoleTextAttribute(hConsole, 15);
}

void red()
{
	SetConsoleTextAttribute(hConsole, 12);
}

void green()
{
	SetConsoleTextAttribute(hConsole, 11);
}

void GotoXY(int X, int Y)
{
	COORD coord = { X, Y };
	SetConsoleCursorPosition(hConsole, coord);
}

user *creatStructUser(int *r)
{
	FILE *f;
	user *us;
	int count = 0, razm;
	char tmp[100];
	if ((f = fopen("users", "r")) == NULL) perror("Ошибка открытия файла: ");
	else
	{
		while (!feof(f))
		{
			fgets(tmp, 100, f);
			count++;
		}
		fclose(f);
	}
	razm = count / 4;
	*r = razm;
	us = (user*)calloc(razm, sizeof(user));
	char t;
	if ((f = fopen("users", "r")) == NULL) perror("Ошибка открытия файла: ");
	else
	{
		for (int i = 0; i < razm; i++)
		{
			fscanf(f, "%d", &us[i].id);
			fscanf(f, "%s", &us[i].login);
			fscanf(f, "%s", &us[i].password);
			getc(f);
			t = getc(f);
			getc(f);
			if (t == '0') us[i].Type = administrator;
			else if (t == '1') us[i].Type = diller;
			else if (t == '2') us[i].Type = buyer;
		}
		fclose(f);
	}
	return us;
}

void Hello()
{
	yellow(); GotoXY(10, 5);
	printf("-----------------------------------------\n");
	yellow(); GotoXY(10, 6);
	printf("Добро пожаловать в наш магазин!");
	yellow(); GotoXY(10, 7);
	printf("Для продолжения нажмите на любую клавишу");
	yellow(); GotoXY(10, 8);
	printf("-----------------------------------------\n");
	getch();
}

int avtor()
{
	FILE *file;	
	int d;
	user*users = creatStructUser(&d);
	int a;
	char login[30], password[30];
	while (true)
	{
		system("cls");
		green(); GotoXY(10, 5);
		printf("----------------------------------------------------\n");
		green(); GotoXY(10, 6);
		printf("Если вы зарегистрированны нажмите клавишу 1 и Enter,");
		green(); GotoXY(10, 7);
		printf("для регистрации нажмите клавишу 2 и Enter");
		green(); GotoXY(10, 8);
		printf("----------------------------------------------------\n");
		GotoXY(54, 7);
		scanf("%d", &a);
		getchar();
		if (a == 1 || a == 2)break;
		else
		{
			system("cls");
			red(); GotoXY(10, 6);
			printf("Ошибка: нажмите на любую клавишу и введите еще раз");
			getchar();
		}
	}
	int r = 0;
	if (a == 1)
	{
		while (r == 0)
		{
			system("cls");
			green(); GotoXY(10, 5);
			printf("------------------------------------------------\n");
			green(); GotoXY(10, 6);
			printf("Введите логин: ");
			green(); GotoXY(10, 7);
			printf("------------------------------------------------\n");
			GotoXY(26, 6);
			scanf("%s", &login);
			green(); GotoXY(10, 8);
			printf("Введите пароль: ");
			green(); GotoXY(10, 9);
			printf("------------------------------------------------\n");
			GotoXY(26, 8);
			scanf("%s", &password);
			for (int i = 0; i < d; i++)
			{
				if (strcmp(users[i].login, login) == 0 && strcmp(users[i].password, password) == 0)
				{
					return users[i].id;
					r = 1;
				}
			}
			if (r == 0)
			{
				system("cls");
				red(); GotoXY(10, 5);
				printf("------------------------------------------------\n");
				red(); GotoXY(10, 6);
				printf("Login или пароль не верны");
				red(); GotoXY(10, 7);
				printf("------------------------------------------------\n");
				getchar();
				getchar();
			}
		}
	}
	else
	{
		system("cls");
		green(); GotoXY(10, 5);
		printf("------------------------------------------------\n");
		green(); GotoXY(10, 6);
		printf("Введите новый логин: ");
		green(); GotoXY(10, 7);
		printf("------------------------------------------------\n");
		GotoXY(32, 6);
		scanf("%s", &login);
		green(); GotoXY(10, 8);
		printf("Введите новый пароль: ");
		green(); GotoXY(10, 9);
		printf("------------------------------------------------\n");
		GotoXY(32, 8);
		scanf("%s", &password);
		if ((file = fopen("users", "a")) == NULL) perror("Ошибка открытия файла: ");
		else
		{
			fprintf(file, "%d\n", users[d - 1].id + 1);
			fprintf(file, "%s\n", login);
			fprintf(file, "%s\n", password);
			fprintf(file, "%c\n", '2');
		}
		fclose(file);
		yellow(); GotoXY(10, 10);
		printf("Поздравляем Вы зарегистрированны!!!\n");
		green(); GotoXY(10, 11);
		printf("------------------------------------------------\n");
		return users[d - 1].id + 1;
	}	
}

int menuB(int a)
{
	system("cls");
	int r, m = 1,k,code;
	user *users1 = creatStructUser(&r);
	for (int i = 0; i < r; i++)
	{
		if (users1[i].id == a)
		{
			green(); GotoXY(10, 5);
			printf("----------------------------------------------\n");
			green(); GotoXY(10, 6);
			printf("%s, добро пожаловать в наш магазин!\n", users1[i].login);
			green(); GotoXY(10, 7);
			printf("----------------------------------------------\n");
			getchar();
		}
	}
	yellow(); GotoXY(10, 9);
	printf("Выберите пункт меню");
	yellow(); GotoXY(11, 10);
	printf("->  Магазин");
	white(); GotoXY(11, 11);
	printf("    История заказов");
	do
	{
		k = 0;
		code = getch();
		if (code == 224 || code == 0)
			code = getch();
		if (code == 80)
		{
			if(m!=2) m += 1;
			else m = 1;
			if (m == 1)
			{
				yellow(); GotoXY(11, 10); printf("->  Магазин");
				white(); GotoXY(11, 11);  printf("    История заказов");				
			}
			else if (m == 2)
			{
				white(); GotoXY(11, 10); printf("    Магазин");
				yellow(); GotoXY(11, 11);  printf("->  История заказов");				
			}					
		}
		else if (code == 72)
		{
			if (m != 1) m -= 1;
			else m = 2;
			if (m == 1)
			{
				yellow(); GotoXY(11, 10); printf("->  Магазин");
				white(); GotoXY(11, 11);  printf("    История заказов");				
			}
			else if (m == 2)
			{
				white(); GotoXY(11, 10); printf("    Магазин");
				yellow(); GotoXY(11, 11);  printf("->  История заказов");				
			}			
		}
		else if (code == 13)
		{
			return m;
			k = 1;
		}
	} while (k==0);
}

int menuA(int a)
{
	system("cls");
	int r, m = 1, k, code;
	user *users1 = creatStructUser(&r);
	for (int i = 0; i < r; i++)
	{
		if (users1[i].id == a)
		{
			green(); GotoXY(10, 5);
			printf("----------------------------------------------\n");
			green(); GotoXY(10, 6);
			printf("%s, добро пожаловать в наш магазин!\n", users1[i].login);
			green(); GotoXY(10, 7);
			printf("----------------------------------------------\n");
			getchar();
		}
	}
	yellow(); GotoXY(10, 9);
	printf("Выберите пункт меню");
	yellow(); GotoXY(11, 10);
	printf("->  Товары");
	return 1;	
}

int menuKT(int a)
{
	int r, m = 1, m1 = 1, k, code;
	user *users1 = creatStructUser(&r);
	while (true)
	{
		system("cls");
		for (int i = 0; i < r; i++)
		{
			if (users1[i].id == a)
			{
				green(); GotoXY(10, 5);
				printf("----------------------------------------------\n");
				green(); GotoXY(10, 6);
				printf("%s, добро пожаловать в наш магазин!\n", users1[i].login);
				green(); GotoXY(10, 7);
				printf("----------------------------------------------\n");
				/*getchar();*/
			}
		}
		yellow(); GotoXY(10, 9);
		printf("Выберите категорию товара");
		yellow(); GotoXY(11, 10); printf("->  Ноутбуки");
		white(); GotoXY(11, 11); printf("    Планшеты");
		white(); GotoXY(11, 12); printf("    Смартфоны");
		white(); GotoXY(11, 13); printf("    Моноблоки, неттопы");
		white(); GotoXY(11, 14); printf("    Персональные компьютеры");
		white(); GotoXY(11, 15); printf("    Мониторы");
		white(); GotoXY(11, 16); printf("    Принтеры, МФУ");
		m = 1;
		do
		{
			k = 0;
			code = getch();
			if (code == 224 || code == 0)
				code = getch();
			if (code == 80)
			{
				if (m != 7) m += 1;
				else m = 1;
				if (m == 1)
				{
					yellow(); GotoXY(11, 10); printf("->  Ноутбуки");
					white(); GotoXY(11, 11); printf("    Планшеты");
					white(); GotoXY(11, 12); printf("    Смартфоны");
					white(); GotoXY(11, 13); printf("    Моноблоки, неттопы");
					white(); GotoXY(11, 14); printf("    Персональные компьютеры");
					white(); GotoXY(11, 15); printf("    Мониторы");
					white(); GotoXY(11, 16); printf("    Принтеры, МФУ");
				}
				else if (m == 2)
				{
					white(); GotoXY(11, 10); printf("    Ноутбуки");
					yellow(); GotoXY(11, 11); printf("->  Планшеты");
					white(); GotoXY(11, 12); printf("    Смартфоны");
					white(); GotoXY(11, 13); printf("    Моноблоки, неттопы");
					white(); GotoXY(11, 14); printf("    Персональные компьютеры");
					white(); GotoXY(11, 15); printf("    Мониторы");
					white(); GotoXY(11, 16); printf("    Принтеры, МФУ");
				}
				else if (m == 3)
				{
					white(); GotoXY(11, 10); printf("    Ноутбуки");
					white(); GotoXY(11, 11); printf("    Планшеты");
					yellow(); GotoXY(11, 12); printf("->  Смартфоны");
					white(); GotoXY(11, 13); printf("    Моноблоки, неттопы");
					white(); GotoXY(11, 14); printf("    Персональные компьютеры");
					white(); GotoXY(11, 15); printf("    Мониторы");
					white(); GotoXY(11, 16); printf("    Принтеры, МФУ");
				}
				else if (m == 4)
				{
					white(); GotoXY(11, 10); printf("    Ноутбуки");
					white(); GotoXY(11, 11); printf("    Планшеты");
					white(); GotoXY(11, 12); printf("    Смартфоны");
					yellow(); GotoXY(11, 13); printf("->  Моноблоки, неттопы");
					white(); GotoXY(11, 14); printf("    Персональные компьютеры");
					white(); GotoXY(11, 15); printf("    Мониторы");
					white(); GotoXY(11, 16); printf("    Принтеры, МФУ");
				}
				else if (m == 5)
				{
					white(); GotoXY(11, 10); printf("    Ноутбуки");
					white(); GotoXY(11, 11); printf("    Планшеты");
					white(); GotoXY(11, 12); printf("    Смартфоны");
					white(); GotoXY(11, 13); printf("    Моноблоки, неттопы");
					yellow(); GotoXY(11, 14); printf("->  Персональные компьютеры");
					white(); GotoXY(11, 15); printf("    Мониторы");
					white(); GotoXY(11, 16); printf("    Принтеры, МФУ");
				}
				else if (m == 6)
				{
					white(); GotoXY(11, 10); printf("    Ноутбуки");
					white(); GotoXY(11, 11); printf("    Планшеты");
					white(); GotoXY(11, 12); printf("    Смартфоны");
					white(); GotoXY(11, 13); printf("    Моноблоки, неттопы");
					white(); GotoXY(11, 14); printf("    Персональные компьютеры");
					yellow(); GotoXY(11, 15); printf("->  Мониторы");
					white(); GotoXY(11, 16); printf("    Принтеры, МФУ");
				}
				else if (m == 7)
				{
					white(); GotoXY(11, 10); printf("    Ноутбуки");
					white(); GotoXY(11, 11); printf("    Планшеты");
					white(); GotoXY(11, 12); printf("    Смартфоны");
					white(); GotoXY(11, 13); printf("    Моноблоки, неттопы");
					white(); GotoXY(11, 14); printf("    Персональные компьютеры");
					white(); GotoXY(11, 15); printf("    Мониторы");
					yellow(); GotoXY(11, 16); printf("->  Принтеры, МФУ");
				}
			}
			else if (code == 72)
			{
				if (m != 1) m -= 1;
				else m = 7;
				if (m == 1)
				{
					yellow(); GotoXY(11, 10); printf("->  Ноутбуки");
					white(); GotoXY(11, 11); printf("    Планшеты");
					white(); GotoXY(11, 12); printf("    Смартфоны");
					white(); GotoXY(11, 13); printf("    Моноблоки, неттопы");
					white(); GotoXY(11, 14); printf("    Персональные компьютеры");
					white(); GotoXY(11, 15); printf("    Мониторы");
					white(); GotoXY(11, 16); printf("    Принтеры, МФУ");
				}
				else if (m == 2)
				{
					white(); GotoXY(11, 10); printf("    Ноутбуки");
					yellow(); GotoXY(11, 11); printf("->  Планшеты");
					white(); GotoXY(11, 12); printf("    Смартфоны");
					white(); GotoXY(11, 13); printf("    Моноблоки, неттопы");
					white(); GotoXY(11, 14); printf("    Персональные компьютеры");
					white(); GotoXY(11, 15); printf("    Мониторы");
					white(); GotoXY(11, 16); printf("    Принтеры, МФУ");
				}
				else if (m == 3)
				{
					white(); GotoXY(11, 10); printf("    Ноутбуки");
					white(); GotoXY(11, 11); printf("    Планшеты");
					yellow(); GotoXY(11, 12); printf("->  Смартфоны");
					white(); GotoXY(11, 13); printf("    Моноблоки, неттопы");
					white(); GotoXY(11, 14); printf("    Персональные компьютеры");
					white(); GotoXY(11, 15); printf("    Мониторы");
					white(); GotoXY(11, 16); printf("    Принтеры, МФУ");
				}
				else if (m == 4)
				{
					white(); GotoXY(11, 10); printf("    Ноутбуки");
					white(); GotoXY(11, 11); printf("    Планшеты");
					white(); GotoXY(11, 12); printf("    Смартфоны");
					yellow(); GotoXY(11, 13); printf("->  Моноблоки, неттопы");
					white(); GotoXY(11, 14); printf("    Персональные компьютеры");
					white(); GotoXY(11, 15); printf("    Мониторы");
					white(); GotoXY(11, 16); printf("    Принтеры, МФУ");
				}
				else if (m == 5)
				{
					white(); GotoXY(11, 10); printf("    Ноутбуки");
					white(); GotoXY(11, 11); printf("    Планшеты");
					white(); GotoXY(11, 12); printf("    Смартфоны");
					white(); GotoXY(11, 13); printf("    Моноблоки, неттопы");
					yellow(); GotoXY(11, 14); printf("->  Персональные компьютеры");
					white(); GotoXY(11, 15); printf("    Мониторы");
					white(); GotoXY(11, 16); printf("    Принтеры, МФУ");
				}
				else if (m == 6)
				{
					white(); GotoXY(11, 10); printf("    Ноутбуки");
					white(); GotoXY(11, 11); printf("    Планшеты");
					white(); GotoXY(11, 12); printf("    Смартфоны");
					white(); GotoXY(11, 13); printf("    Моноблоки, неттопы");
					white(); GotoXY(11, 14); printf("    Персональные компьютеры");
					yellow(); GotoXY(11, 15); printf("->  Мониторы");
					white(); GotoXY(11, 16); printf("    Принтеры, МФУ");
				}
				else if (m == 7)
				{
					white(); GotoXY(11, 10); printf("    Ноутбуки");
					white(); GotoXY(11, 11); printf("    Планшеты");
					white(); GotoXY(11, 12); printf("    Смартфоны");
					white(); GotoXY(11, 13); printf("    Моноблоки, неттопы");
					white(); GotoXY(11, 14); printf("    Персональные компьютеры");
					white(); GotoXY(11, 15); printf("    Мониторы");
					yellow(); GotoXY(11, 16); printf("->  Принтеры, МФУ");
				}
			}
			else if (code == 13)
			{
				k = 1;
			}
		} while (k == 0);
		if (m == 1)
		{
			system("cls");
			for (int i = 0; i < r; i++)
			{
				if (users1[i].id == a)
				{
					green(); GotoXY(10, 5);
					printf("----------------------------------------------\n");
					green(); GotoXY(10, 6);
					printf("%s, добро пожаловать в наш магазин!\n", users1[i].login);
					green(); GotoXY(10, 7);
					printf("----------------------------------------------\n");
				}
			}
			yellow(); GotoXY(10, 9);
			printf("Выберите производителя ноутбуков");
			yellow(); GotoXY(11, 10); printf("->  Acer");
			white(); GotoXY(11, 11); printf("    Asus");
			white(); GotoXY(11, 12); printf("    Lenovo");
			m1 = 1;
			do
			{
				k = 0;
				code = getch();
				if (code == 224 || code == 0)
					code = getch();
				if (code == 80)
				{
					if (m1 != 3) m1 += 1;
					else m1 = 1;
					if (m1 == 1)
					{
						yellow(); GotoXY(11, 10); printf("->  Acer");
						white(); GotoXY(11, 11); printf("    Asus");
						white(); GotoXY(11, 12); printf("    Lenovo");
					}
					else if (m1 == 2)
					{
						white(); GotoXY(11, 10); printf("    Acer");
						yellow(); GotoXY(11, 11); printf("->  Asus");
						white(); GotoXY(11, 12); printf("    Lenovo");
					}
					else if (m1 == 3)
					{
						white(); GotoXY(11, 10); printf("    Acer");
						white(); GotoXY(11, 11); printf("    Asus");
						yellow(); GotoXY(11, 12); printf("->  Lenovo");
					}
				}
				else if (code == 72)
				{
					if (m1 != 1) m1 -= 1;
					else m1 = 3;
					if (m1 == 1)
					{
						yellow(); GotoXY(11, 10); printf("->  Acer");
						white(); GotoXY(11, 11); printf("    Asus");
						white(); GotoXY(11, 12); printf("    Lenovo");
					}
					else if (m1 == 2)
					{
						white(); GotoXY(11, 10); printf("    Acer");
						yellow(); GotoXY(11, 11); printf("->  Asus");
						white(); GotoXY(11, 12); printf("    Lenovo");
					}
					else if (m1 == 3)
					{
						white(); GotoXY(11, 10); printf("    Acer");
						white(); GotoXY(11, 11); printf("    Asus");
						yellow(); GotoXY(11, 12); printf("->  Lenovo");
					}
				}
				else if (code == 13)
				{
					return m1;
					k = 1;
					break;
				}
				else if (code == 27)
				{
					k = 1;
				}
			} while (k == 0);
		}
		else if (m == 2)
		{
			system("cls");
			for (int i = 0; i < r; i++)
			{
				if (users1[i].id == a)
				{
					green(); GotoXY(10, 5);
					printf("----------------------------------------------\n");
					green(); GotoXY(10, 6);
					printf("%s, добро пожаловать в наш магазин!\n", users1[i].login);
					green(); GotoXY(10, 7);
					printf("----------------------------------------------\n");
				}
			}
			yellow(); GotoXY(10, 9);
			printf("Выберите производителя планшетов");
			yellow(); GotoXY(11, 10); printf("->  Lenovo");
			white(); GotoXY(11, 11); printf("    Samsung");
			m1 = 4;
			do
			{
				k = 0;
				code = getch();
				if (code == 224 || code == 0)
					code = getch();
				if (code == 80)
				{
					if (m1 != 5) m1 += 1;
					else m1 = 4;
					if (m1 == 4)
					{
						yellow(); GotoXY(11, 10); printf("->  Lenovo");
						white(); GotoXY(11, 11); printf("    Samsung");
					}
					else if (m1 == 5)
					{
						white(); GotoXY(11, 10); printf("    Lenovo");
						yellow(); GotoXY(11, 11); printf("->  Samsung");
					}
				}
				else if (code == 72)
				{
					if (m1 != 4) m1 -= 1;
					else m1 = 5;
					if (m1 == 4)
					{
						yellow(); GotoXY(11, 10); printf("->  Lenovo");
						white(); GotoXY(11, 11); printf("    Samsung");
					}
					else if (m1 == 5)
					{
						white(); GotoXY(11, 10); printf("    Lenovo");
						yellow(); GotoXY(11, 11); printf("->  Samsung");
					}
				}
				else if (code == 13)
				{
					return m1;
					k = 1;
					break;
				}
				else if (code == 27)
				{
					k = 1;
				}
			} while (k == 0);
		}
		else if (m == 3)
		{
			system("cls");
			for (int i = 0; i < r; i++)
			{
				if (users1[i].id == a)
				{
					green(); GotoXY(10, 5);
					printf("----------------------------------------------\n");
					green(); GotoXY(10, 6);
					printf("%s, добро пожаловать в наш магазин!\n", users1[i].login);
					green(); GotoXY(10, 7);
					printf("----------------------------------------------\n");
				}
			}
			yellow(); GotoXY(10, 9);
			printf("Выберите производителя смартфонов");
			yellow(); GotoXY(11, 10); printf("->  Huawei");
			white(); GotoXY(11, 11); printf("    MEIZU");
			white(); GotoXY(11, 12); printf("    Samsung");
			m1 = 6;
			do
			{
				k = 0;
				code = getch();
				if (code == 224 || code == 0)
					code = getch();
				if (code == 80)
				{
					if (m1 != 8) m1 += 1;
					else m1 = 6;
					if (m1 == 6)
					{
						yellow(); GotoXY(11, 10); printf("->  Huawei");
						white(); GotoXY(11, 11); printf("    MEIZU");
						white(); GotoXY(11, 12); printf("    Samsung");
					}
					else if (m1 == 7)
					{
						white(); GotoXY(11, 10); printf("    Huawei");
						yellow(); GotoXY(11, 11); printf("->  MEIZU");
						white(); GotoXY(11, 12); printf("    Samsung");
					}
					else if (m1 == 8)
					{
						white(); GotoXY(11, 10); printf("    Huawei");
						white(); GotoXY(11, 11); printf("    MEIZU");
						yellow(); GotoXY(11, 12); printf("->  Samsung");
					}
				}
				else if (code == 72)
				{
					if (m1 != 6) m1 -= 1;
					else m1 = 8;
					if (m1 == 6)
					{
						yellow(); GotoXY(11, 10); printf("->  Huawei");
						white(); GotoXY(11, 11); printf("    MEIZU");
						white(); GotoXY(11, 12); printf("    Samsung");
					}
					else if (m1 == 7)
					{
						white(); GotoXY(11, 10); printf("    Huawei");
						yellow(); GotoXY(11, 11); printf("->  MEIZU");
						white(); GotoXY(11, 12); printf("    Samsung");
					}
					else if (m1 == 8)
					{
						white(); GotoXY(11, 10); printf("    Huawei");
						white(); GotoXY(11, 11); printf("    MEIZU");
						yellow(); GotoXY(11, 12); printf("->  Samsung");
					}
				}
				else if (code == 13)
				{
					return m1;
					k = 1;
					break;
				}
				else if (code == 27)
				{
					k = 1;
				}
			} while (k == 0);
		}
		else if (m == 4)
		{
			system("cls");
			for (int i = 0; i < r; i++)
			{
				if (users1[i].id == a)
				{
					green(); GotoXY(10, 5);
					printf("----------------------------------------------\n");
					green(); GotoXY(10, 6);
					printf("%s, добро пожаловать в наш магазин!\n", users1[i].login);
					green(); GotoXY(10, 7);
					printf("----------------------------------------------\n");
				}
			}
			yellow(); GotoXY(10, 9);
			printf("Выберите производителя моноблоков");
			yellow(); GotoXY(11, 10); printf("->  Acer");
			white(); GotoXY(11, 11); printf("    Asus");
			white(); GotoXY(11, 12); printf("    Gigabyte");
			m1 = 9;
			do
			{
				k = 0;
				code = getch();
				if (code == 224 || code == 0)
					code = getch();
				if (code == 80)
				{
					if (m1 != 11) m1 += 1;
					else m1 = 9;
					if (m1 == 9)
					{
						yellow(); GotoXY(11, 10); printf("->  Acer");
						white(); GotoXY(11, 11); printf("    Asus");
						white(); GotoXY(11, 12); printf("    Gigabyte");
					}
					else if (m1 == 10)
					{
						white(); GotoXY(11, 10); printf("    Acer");
						yellow(); GotoXY(11, 11); printf("->  Asus");
						white(); GotoXY(11, 12); printf("    Gigabyte");
					}
					else if (m1 == 11)
					{
						white(); GotoXY(11, 10); printf("    Acer");
						white(); GotoXY(11, 11); printf("    Asus");
						yellow(); GotoXY(11, 12); printf("->  Gigabyte");
					}
				}
				else if (code == 72)
				{
					if (m1 != 9) m1 -= 1;
					else m1 = 11;
					if (m1 == 9)
					{
						yellow(); GotoXY(11, 10); printf("->  Acer");
						white(); GotoXY(11, 11); printf("    Asus");
						white(); GotoXY(11, 12); printf("    Gigabyte");
					}
					else if (m1 == 10)
					{
						white(); GotoXY(11, 10); printf("    Acer");
						yellow(); GotoXY(11, 11); printf("->  Asus");
						white(); GotoXY(11, 12); printf("    Gigabyte");
					}
					else if (m1 == 11)
					{
						white(); GotoXY(11, 10); printf("    Acer");
						white(); GotoXY(11, 11); printf("    Asus");
						yellow(); GotoXY(11, 12); printf("->  Gigabyte");
					}
				}
				else if (code == 13)
				{
					return m1;
					k = 1;
					break;
				}
				else if (code == 27)
				{
					k = 1;
				}
			} while (k == 0);
		}
		else if (m == 5)
		{
			system("cls");
			for (int i = 0; i < r; i++)
			{
				if (users1[i].id == a)
				{
					green(); GotoXY(10, 5);
					printf("----------------------------------------------\n");
					green(); GotoXY(10, 6);
					printf("%s, добро пожаловать в наш магазин!\n", users1[i].login);
					green(); GotoXY(10, 7);
					printf("----------------------------------------------\n");
				}
			}
			yellow(); GotoXY(10, 9);
			printf("Выберите категорию ПК");
			yellow(); GotoXY(11, 10); printf("->  Eco");
			white(); GotoXY(11, 11); printf("    Standard");
			white(); GotoXY(11, 12); printf("    Advanced");
			white(); GotoXY(11, 13); printf("    Extreme");
			m1 = 12;
			do
			{
				k = 0;
				code = getch();
				if (code == 224 || code == 0)
					code = getch();
				if (code == 80)
				{
					if (m1 != 15) m1 += 1;
					else m1 = 12;
					if (m1 == 12)
					{
						yellow(); GotoXY(11, 10); printf("->  Eco");
						white(); GotoXY(11, 11); printf("    Standard");
						white(); GotoXY(11, 12); printf("    Advanced");
						white(); GotoXY(11, 13); printf("    Extreme");
					}
					else if (m1 == 13)
					{
						white(); GotoXY(11, 10); printf("    Eco");
						yellow(); GotoXY(11, 11); printf("->  Standard");
						white(); GotoXY(11, 12); printf("    Advanced");
						white(); GotoXY(11, 13); printf("    Extreme");
					}
					else if (m1 == 14)
					{
						white(); GotoXY(11, 10); printf("    Eco");
						white(); GotoXY(11, 11); printf("    Standard");
						yellow(); GotoXY(11, 12); printf("->  Advanced");
						white(); GotoXY(11, 13); printf("    Extreme");
					}
					else if (m1 == 15)
					{
						white(); GotoXY(11, 10); printf("    Eco");
						white(); GotoXY(11, 11); printf("    Standard");
						white(); GotoXY(11, 12); printf("    Advanced");
						yellow(); GotoXY(11, 13); printf("->  Extreme");
					}
				}
				else if (code == 72)
				{
					if (m1 != 12) m1 -= 1;
					else m1 = 15;
					if (m1 == 12)
					{
						yellow(); GotoXY(11, 10); printf("->  Eco");
						white(); GotoXY(11, 11); printf("    Standard");
						white(); GotoXY(11, 12); printf("    Advanced");
						white(); GotoXY(11, 13); printf("    Extreme");
					}
					else if (m1 == 13)
					{
						white(); GotoXY(11, 10); printf("    Eco");
						yellow(); GotoXY(11, 11); printf("->  Standard");
						white(); GotoXY(11, 12); printf("    Advanced");
						white(); GotoXY(11, 13); printf("    Extreme");
					}
					else if (m1 == 14)
					{
						white(); GotoXY(11, 10); printf("    Eco");
						white(); GotoXY(11, 11); printf("    Standard");
						yellow(); GotoXY(11, 12); printf("->  Advanced");
						white(); GotoXY(11, 13); printf("    Extreme");
					}
					else if (m1 == 15)
					{
						white(); GotoXY(11, 10); printf("    Eco");
						white(); GotoXY(11, 11); printf("    Standard");
						white(); GotoXY(11, 12); printf("    Advanced");
						yellow(); GotoXY(11, 13); printf("->  Extreme");
					}
				}
				else if (code == 13)
				{
					return m1;
					k = 1;
					break;
				}
				else if (code == 27)
				{
					k = 1;
				}
			} while (k == 0);
		}
		else if (m == 6)
		{
			system("cls");
			for (int i = 0; i < r; i++)
			{
				if (users1[i].id == a)
				{
					green(); GotoXY(10, 5);
					printf("----------------------------------------------\n");
					green(); GotoXY(10, 6);
					printf("%s, добро пожаловать в наш магазин!\n", users1[i].login);
					green(); GotoXY(10, 7);
					printf("----------------------------------------------\n");
				}
			}
			yellow(); GotoXY(10, 9);
			printf("Выберите производителя мониторов");
			yellow(); GotoXY(11, 10); printf("->  Dell");
			white(); GotoXY(11, 11); printf("    Philips");
			white(); GotoXY(11, 12); printf("    ViewSonic");
			m1 = 16;
			do
			{
				k = 0;
				code = getch();
				if (code == 224 || code == 0)
					code = getch();
				if (code == 80)
				{
					if (m1 != 18) m1 += 1;
					else m1 = 16;
					if (m1 == 16)
					{
						yellow(); GotoXY(11, 10); printf("->  Dell");
						white(); GotoXY(11, 11); printf("    Philips");
						white(); GotoXY(11, 12); printf("    ViewSonic");
					}
					else if (m1 == 17)
					{
						white(); GotoXY(11, 10); printf("    Dell");
						yellow(); GotoXY(11, 11); printf("->  Philips");
						white(); GotoXY(11, 12); printf("    ViewSonic");
					}
					else if (m1 == 18)
					{
						white(); GotoXY(11, 10); printf("    Dell");
						white(); GotoXY(11, 11); printf("    Philips");
						yellow(); GotoXY(11, 12); printf("->  ViewSonic");
					}
				}
				else if (code == 72)
				{
					if (m1 != 16) m1 -= 1;
					else m1 = 18;
					if (m1 == 16)
					{
						yellow(); GotoXY(11, 10); printf("->  Dell");
						white(); GotoXY(11, 11); printf("    Philips");
						white(); GotoXY(11, 12); printf("    ViewSonic");
					}
					else if (m1 == 17)
					{
						white(); GotoXY(11, 10); printf("    Dell");
						yellow(); GotoXY(11, 11); printf("->  Philips");
						white(); GotoXY(11, 12); printf("    ViewSonic");
					}
					else if (m1 == 18)
					{
						white(); GotoXY(11, 10); printf("    Dell");
						white(); GotoXY(11, 11); printf("    Philips");
						yellow(); GotoXY(11, 12); printf("->  ViewSonic");
					}
				}
				else if (code == 13)
				{
					return m1;
					k = 1;
					break;
				}
				else if (code == 27)
				{
					k = 1;
				}
			} while (k == 0);
		}
		else if (m == 7)
		{
			system("cls");
			for (int i = 0; i < r; i++)
			{
				if (users1[i].id == a)
				{
					green(); GotoXY(10, 5);
					printf("----------------------------------------------\n");
					green(); GotoXY(10, 6);
					printf("%s, добро пожаловать в наш магазин!\n", users1[i].login);
					green(); GotoXY(10, 7);
					printf("----------------------------------------------\n");
				}
			}
			yellow(); GotoXY(10, 9);
			printf("Выберите производителя принтеров");
			yellow(); GotoXY(11, 10); printf("->  HP");
			m1 = 19;
			do
			{
				k = 0;
				code = getch();
				if (code == 224 || code == 0)
					code = getch();
				else if (code == 13)
				{
					return m1;
					k = 1;
					break;
				}
				else if (code == 27)
				{
					k = 1;
				}
			} while (k == 0);
		}
	}
}

void parsStr(int*code, char*desc, int*rP, int*oP, char*gar, int*count, char *str)
{
	int nach=0, kon;
	char tmp[150];
	int i = nach;
	while (true)
	{
		if (str[i] != '\t')i++;
		else
		{
			kon = i;
			break;
		}
	}
	for (int j = nach; j < kon; j++)
	{
		tmp[j] = str[j];
	}
	tmp[kon] = '\0';
	*code = atoi(tmp);	

	nach = kon + 1;
	i = nach;
	while (true)
	{
		if (str[i] != '\t')i++;
		else
		{
			kon = i;
			break;
		}
	}
	int b = 0;
	tmp[0] = '\0';
	for (int j = nach; j < kon; j++)
	{
		tmp[b] = str[j];
		b++;
	}
	tmp[b] = '\0';
	desc[0] = '\0';
	strcat(desc, tmp);

	nach = kon + 1;
	i = nach;
	while (true)
	{
		if (str[i] != '\t')i++;
		else
		{
			kon = i;
			break;
		}
	}
	b = 0;
	tmp[0] = '\0';
	for (int j = nach; j < kon; j++)
	{
		tmp[b] = str[j];
		b++;
	}
	tmp[b] = '\0';
	*rP = atoi(tmp);	
		
	nach = kon + 1;
	i = nach;
	while (true)
	{
		if (str[i] != '\t')i++;
		else
		{
			kon = i;
			break;
		}
	}
	b = 0;
	tmp[0] = '\0';
	for (int j = nach; j < kon; j++)
	{
		tmp[b] = str[j];
		b++;
	}
	tmp[b] = '\0';
	*oP = atoi(tmp);	

	nach = kon + 1;
	i = nach;
	while (true)
	{
		if (str[i] != '\t')i++;
		else
		{
			kon = i;
			break;
		}
	}
	b = 0;
	tmp[0] = '\0';
	for (int j = nach; j < kon; j++)
	{
		tmp[b] = str[j];
		b++;
	}
	tmp[b] = '\0';
	gar[0] = '\0';
	strcat(gar, tmp);
	
	nach = kon + 1;
	i = nach;
	while (true)
	{
		if (str[i] != '\n')i++;
		else
		{
			kon = i;
			break;
		}
	}
	b = 0;
	tmp[0] = '\0';
	for (int j = nach; j < kon; j++)
	{
		tmp[b] = str[j];
		b++;
	}
	tmp[b] = '\0';
	*count = atoi(tmp);
}
	
product *creatStructProd(int *r, int vp)
{
	FILE *f;
	product*prod;
	int razm=0;
	char tmp[200];	
	char *str;	
	if (vp == 1) str = "Notebook Acer";
	else if (vp == 2) str = "Notebook Asus";
	else if (vp == 3) str = "Notebook Lenovo";
	else if (vp == 4) str = "Tablet Lenovo";
	else if (vp == 5) str = "Tablet Samsung";
	else if (vp == 6) str = "Smartphon Huawei";
	else if (vp == 7) str = "Smartphon Meizu";
	else if (vp == 8) str = "Smartphon Sumsung";
	else if (vp == 9) str = "Monoblock Aser";
	else if (vp == 10) str = "Monoblock Asus";
	else if (vp == 11) str = "Monoblock Gigabyte";
	else if (vp == 12) str = "Eco";
	else if (vp == 13) str = "Standard";
	else if (vp == 14) str = "Advanced";
	else if (vp == 15) str = "Extreme";
	else if (vp == 16) str = "Monitor Dell";
	else if (vp == 17) str = "Monitor Philips";
	else if (vp == 18) str = "Monitor ViewSonic";
	else if (vp == 19) str = "Print HP";
	if ((f = fopen(str, "r")) == NULL) perror("Ошибка открытия файла: ");
	else
	{
		while (!feof(f))
		{
			fgets(tmp, 200, f);
			razm++;
		}
		fclose(f);
	}	
	*r = razm;
	prod = (product*)calloc(razm, sizeof(product));
	char t;
	if ((f = fopen(str, "r")) == NULL) perror("Ошибка открытия файла: ");
	else
	{
		for (int i = 0; i < razm; i++)
		{
			fgets(tmp, 200, f);
			parsStr(&prod[i].code, prod[i].desc, &prod[i].rPrice, &prod[i].oPrice, prod[i].gar, &prod[i].count, tmp);
			/*printf("%d - %s, %d, %d, %s, %d\n", prod[i].code, prod[i].desc, prod[i].rPrice, prod[i].oPrice, prod[i].gar, prod[i].count);*/
		}
		fclose(f);
	}
	return prod;
}

void viewProd(product*pr, int r, int vp, int b)
{
	FILE *f;
	system("cls");
	int rr;
	user *users1 = creatStructUser(&rr);
	yellow(); GotoXY(4, 2);
	if (vp == 1) printf("Ноутбуки Acer");
	else if (vp == 2) printf("Ноутбуки Asus");
	else if (vp == 3) printf("Ноутбуки Lenovo");
	else if (vp == 4) printf("Планшеты Lenovo");
	else if (vp == 5) printf("Планшеты Samsung");
	else if (vp == 6) printf("Смартфоны Huawei");
	else if (vp == 7) printf("Смартфоны Meizu");
	else if (vp == 8) printf("Смартфоны Sumsung");
	else if (vp == 9) printf("Моноблоки Aser");
	else if (vp == 10) printf("Моноблоки Asus");
	else if (vp == 11) printf("Моноблоки Gigabyte");
	else if (vp == 12) printf("Компьютеры Eco");
	else if (vp == 13) printf("Компьютеры Standard");
	else if (vp == 14) printf("Компьютеры Advanced");
	else if (vp == 15) printf("Компьютеры Extreme");
	else if (vp == 16) printf("Мониторы Dell");
	else if (vp == 17) printf("Мониторы Philips");
	else if (vp == 18) printf("Мониторы ViewSonic");
	else if (vp == 19) printf("Принтерыы HP");
	white(); GotoXY(4, 3);
	for (int i = 0; i < 110; i++)printf("-");
	yellow(); GotoXY(4, 4);
	printf("Код    | Описание"); for (int i = 0; i < 65; i++)printf(" ");
	printf(" | Цена   | Гарант.| Наличие");
	white(); GotoXY(4, 5);
	for (int i = 0; i < 110; i++)printf("-");
	int stroch = 6;
	for (int i = 0; i < r; i++)
	{
			white(); GotoXY(4, stroch);
			printf("       | "); for (int j = 0; j < 74; j++)printf(" "); printf("|        |        | ");
			white(); GotoXY(4, stroch);
			printf("%d", pr[i].code);
			white(); GotoXY(12, stroch);
			strD(pr[i].desc, stroch);
			white(); GotoXY(89, stroch);
			if(users1[b-1].Type==buyer)
				printf("%d", pr[i].rPrice);
			else if (users1[b-1].Type == diller)
				printf("%d", pr[i].oPrice);
			white(); GotoXY(98, stroch);
			printf("%s", pr[i].gar);
			white(); GotoXY(107, stroch);
			printf("%d", pr[i].count);
			white(); GotoXY(4, stroch + 2);
			for (int j = 0; j < 110; j++)printf("-");
			stroch = stroch + 3;			
	}
	stroch = stroch + 2;
	yellow(); GotoXY(4, stroch);		
	printf("Выберите указателем, продукцию для добавления в корзину\n");
	yellow(); GotoXY(4, stroch+1);
	printf("Нажмите клавишу S, для сортировки по цене");
	int p = 0, k, code;
	stroch = 6;
	yellow(); GotoXY(1, stroch); printf("->");
	do
	{
		k = 0;
		code = getch();
		if (code == 224 || code == 0)
			code = getch();
		if (code == 80)
		{
			if (p < r-1)
			{
				p += 1;
				yellow(); GotoXY(1, stroch); printf("  ");
				stroch = stroch + 3;
				yellow(); GotoXY(1, stroch); printf("->");
			}
			else
			{
				p = 0;
				yellow(); GotoXY(1, stroch); printf("  ");
				stroch = 6;
				yellow(); GotoXY(1, stroch); printf("->");
			}
		}
		else if (code == 72)
		{
			if (p > 0)
			{
				p -= 1;
				yellow(); GotoXY(1, stroch); printf("  ");
				stroch = stroch - 3;
				yellow(); GotoXY(1, stroch); printf("->");
			}
			else
			{
				p = r-1;
				yellow(); GotoXY(1, stroch); printf("  ");
				stroch = 6+((r-1)*3);
				yellow(); GotoXY(1, stroch); printf("->");
			}
		}
		else if (code == 83)
		{
			sortPr(pr, r);
			yellow(); GotoXY(4, 4);
			printf("Код    | Описание"); for (int i = 0; i < 65; i++)printf(" ");
			printf(" | Цена   | Гарант.| Наличие");
			white(); GotoXY(4, 5);
			for (int i = 0; i < 110; i++)printf("-");
			int stroch = 6;
			for (int i = 0; i < r; i++)
			{
				white(); GotoXY(4, stroch);
				printf("       | "); for (int j = 0; j < 74; j++)printf(" "); printf("|        |        | ");
				white(); GotoXY(4, stroch);
				printf("%d", pr[i].code);
				white(); GotoXY(12, stroch);
				strD(pr[i].desc, stroch);
				white(); GotoXY(89, stroch);
				if (users1[b-1].Type == buyer)
					printf("%d", pr[i].rPrice);
				else if (users1[b-1].Type == diller)
					printf("%d", pr[i].oPrice);
				white(); GotoXY(98, stroch);
				printf("%s", pr[i].gar);
				white(); GotoXY(107, stroch);
				printf("%d", pr[i].count);
				white(); GotoXY(4, stroch + 2);
				for (int j = 0; j < 110; j++)printf("-");
				stroch = stroch + 3;
			}
			stroch = stroch + 2;
			yellow(); GotoXY(4, stroch);
			printf("Выберите указателем, продукцию для добавления в корзину\n");
		}
		else if (code == 13)
		{			
			k = 1;
		}
	} while (k==0);
	
	system("cls");
	if (pr[p].count == 0)
	{
		red(); GotoXY(4, 2);
		printf("Данный товар у нас закончился");
	}
	else
	{
		yellow(); GotoXY(4, 2);
		printf("Вы выбрали:");
		white(); GotoXY(4, 3);
		for (int i = 0; i < 110; i++)printf("-");
		yellow(); GotoXY(4, 4);
		printf("Код    | Описание"); for (int i = 0; i < 65; i++)printf(" ");
		printf(" | Цена   | Гарант.| Наличие");
		white(); GotoXY(4, 5);
		for (int i = 0; i < 110; i++)printf("-");
		stroch = 6;
		white(); GotoXY(4, stroch);
		printf("       | "); for (int j = 0; j < 74; j++)printf(" "); printf("|        |        | ");
		white(); GotoXY(4, stroch);
		printf("%d", pr[p].code);
		white(); GotoXY(12, stroch);
		strD(pr[p].desc, stroch);
		white(); GotoXY(89, stroch);
		if (users1[b-1].Type == buyer)
			printf("%d", pr[p].rPrice);
		else if (users1[b-1].Type == diller)
			printf("%d", pr[p].oPrice);
		white(); GotoXY(98, stroch);
		printf("%s", pr[p].gar);
		white(); GotoXY(107, stroch);
		printf("%d", pr[p].count);
		white(); GotoXY(4, stroch + 2);
		for (int j = 0; j < 110; j++)printf("-");
		stroch = stroch + 5;
		int kol;
		while (true)
		{
			white(); GotoXY(4, stroch);
			printf("Введите количество: "); scanf("%d", &kol);
			if (kol <= pr[p].count&&kol > 0)break;
			else if (kol <= 0)
			{
				red(); GotoXY(4, stroch + 1);
				printf("Вы не ыерно указали количество, введите еще раз");
				white(); GotoXY(4, stroch);
				printf("Введите количество:      ");
			}
			else if (kol > pr[p].count)
			{
				red(); GotoXY(4, stroch + 1);
				printf("Такого количества у нас нет, введите еще раз");
				white(); GotoXY(4, stroch);
				printf("Введите количество:      ");
			}
		}
		stroch = stroch + 2;
		yellow(); GotoXY(4, stroch);
		if (users1[b-1].Type == buyer)
			printf("Итого: %d*%d=%d", pr[p].rPrice, kol, pr[p].rPrice*kol);
		else if (users1[b-1].Type == diller)
			printf("Итого: %d*%d=%d", pr[p].oPrice, kol, pr[p].oPrice*kol);		
		stroch = stroch + 2;
		int t;
		while (true)
		{
			yellow(); GotoXY(4, stroch);
			printf("Добавить в корзину? 1-Да, 2-Вернутся в меню "); scanf("%d", &t);
			if (t > 0 && t < 3)break;
			else
			{
				red(); GotoXY(4, stroch + 1);
				printf("Вы ввели не ыерно, введите еще раз");
				white(); GotoXY(4, stroch);
				printf("Добавить в корзину? 1-Да, 2-Вернутся в меню            ");
			}
		}
		if (t == 1)
		{
			if ((f = fopen("basket", "a+")) == NULL) perror("Ошибка открытия файла: ");
			else
			{
				fprintf(f, "%d\n", vp);
				fprintf(f, "%d\n", pr[p].code);
				fprintf(f, "%s\n", pr[p].desc);
				if(users1[b-1].Type==buyer)
				fprintf(f, "%d\n", pr[p].rPrice);
				if (users1[b-1].Type == diller)
					fprintf(f, "%d\n", pr[p].oPrice);
				fprintf(f, "%d\n", kol);
				if (users1[b-1].Type == buyer)
				fprintf(f, "%d\n", pr[p].rPrice*kol);
				if (users1[b-1].Type == diller)
					fprintf(f, "%d\n", pr[p].oPrice*kol);
			}
			fclose(f);
			while (true)
			{
				yellow(); GotoXY(4, stroch + 2);
				printf("Перейти в корзину? 1-Да, 2-Вернутся в меню "); scanf("%d", &t);
				if (t > 0 && t < 3)break;
				else
				{
					red(); GotoXY(4, stroch + 1);
					printf("Вы ввели не ыерно, введите еще раз");
					white(); GotoXY(4, stroch);
					printf("Перейти в корзину? 1-Да, 2-Вернутся в меню            ");
				}
			}
			if (t == 1) Basket(b);
		}
	}
}

void sortPr(product*pr, int r)
{
	product tmp;
	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < r - 1; j++)
		{
			if (pr[j].rPrice > pr[j + 1].rPrice)
			{
				tmp = pr[j];
				pr[j] = pr[j + 1];
				pr[j + 1] = tmp;
			}
		}
	}
}

void strD(char*str,int s)
{
	char st1[75];
	st1[0] = '\0';
	char st2[75];
	st2[0] = '\0';
	if (strlen(str) > 74)
	{
		for (int i = 0; i < 74; i++)
		{
			st1[i] = str[i];
		}
		st1[74] = '\0';
		printf("%s", st1);
		GotoXY(4, s+1);
		printf("       | "); for (int i = 0; i < 74; i++)printf(" "); printf("|        |        | ");
		GotoXY(12, s+1);
		for (int i = 0; i < strlen(str) - 74; i++)
		{
			st2[i] = str[i + 74];
		}
		st2[strlen(str)-74] = '\0';
		printf("%s", st2);
	}
	else
	{
		for (int i = 0; i < 74; i++)
		{
			st1[i] = str[i];
		}
		st1[74] = '\0';
		printf("%s", st1);
		GotoXY(4, s + 1);
		printf("       | "); for (int i = 0; i < 74; i++)printf(" "); printf("|        |        | ");
	}
}

void Basket(int b)
{
	FILE*f;
	system("cls");
	int rr;
	user *users1 = creatStructUser(&rr);
	int razm=0;
	char tmp[150];
	bask * bask1;
	if ((f = fopen("basket", "r")) == NULL) perror("Ошибка открытия файла: ");
	else
	{
		while (!feof(f))
		{
			fgets(tmp, 150, f);
			razm++;
		}		
	}
	fclose(f);
	razm = razm / 6;
	bask1 = (bask*)calloc(razm, sizeof(bask));
	yellow(); GotoXY(4, 1);
	printf("Корзина");
	white(); GotoXY(4, 3);
	for (int i = 0; i < 110; i++)printf("-");
	yellow(); GotoXY(4, 4);
	printf("Код    | Описание"); for (int i = 0; i < 65; i++)printf(" ");
	printf(" | Цена   | Кол-во  | Сумма");
	white(); GotoXY(4, 5);
	for (int i = 0; i < 110; i++)printf("-");
	int stroch = 6;
	tmp[0] = '\0';
	if ((f = fopen("basket", "r")) == NULL) perror("Ошибка открытия файла: ");
	else
	{
		for (int i = 0; i < razm; i++)
		{
			fgets(tmp, 9, f);
			bask1[i].vp = atoi(tmp);
			tmp[0] = '\0';
			fgets(tmp, 9, f);
			bask1[i].code = atoi(tmp);
			tmp[0] = '\0';
			fgets(bask1[i].desc, 150, f);			
			fgets(tmp, 9, f);			
			bask1[i].Price = atoi(tmp);			
			tmp[0] = '\0';
			fgets(tmp, 9, f);
			bask1[i].kol = atoi(tmp);
			tmp[0] = '\0';
			fgets(tmp, 9, f);
			bask1[i].sum = atoi(tmp);
			tmp[0] = '\0';			
		}
	}
	fclose(f);
	for (int i = 0; i < razm; i++)
	{
		white(); GotoXY(4, stroch);
		printf("       | "); for (int j = 0; j < 74; j++)printf(" "); printf("|        |        | ");
		white(); GotoXY(4, stroch);
		printf("%d", bask1[i].code);
		white(); GotoXY(12, stroch);
		strD(bask1[i].desc, stroch);
		white(); GotoXY(89, stroch);
		printf("%d", bask1[i].Price);
		white(); GotoXY(98, stroch);
		printf("%d", bask1[i].kol);
		white(); GotoXY(107, stroch);
		printf("%d", bask1[i].sum);
		white(); GotoXY(4, stroch + 2);
		for (int j = 0; j < 110; j++)printf("-");
		stroch = stroch + 3;
	}
	stroch = stroch + 1;
	yellow(); GotoXY(12, stroch);
	printf("Итого:");
	yellow(); GotoXY(107, stroch);
	int gs=0;
	for (int i = 0; i < razm; i++)
	{
		gs += bask1[i].sum;
	}
	printf("%d", gs);	
	stroch = stroch + 2;
	yellow(); GotoXY(4, stroch);
	int t1;
	while (true)
	{
		yellow(); GotoXY(4, stroch);
		printf("Оформить заказ? 1-Да, 2-Вернутся в меню "); scanf("%d", &t1);
		if (t1 > 0 && t1 < 3)break;
		else
		{
			red(); GotoXY(4, stroch + 1);
			printf("Вы ввели не ыерно, введите еще раз");
			white(); GotoXY(4, stroch);
			printf("Оформить заказ? 1-Да, 2-Вернутся в меню            ");
		}
	}
	if (t1 == 1)
	{
		char *str;
		for (int i = 0; i < razm; i++)
		{			
			if (bask1[i].vp == 1) str = "Notebook Acer";
			else if (bask1[i].vp == 2) str = "Notebook Asus";
			else if (bask1[i].vp == 3) str = "Notebook Lenovo";
			else if (bask1[i].vp == 4) str = "Tablet Lenovo";
			else if (bask1[i].vp == 5) str = "Tablet Samsung";
			else if (bask1[i].vp == 6) str = "Smartphon Huawei";
			else if (bask1[i].vp == 7) str = "Smartphon Meizu";
			else if (bask1[i].vp == 8) str = "Smartphon Sumsung";
			else if (bask1[i].vp == 9) str = "Monoblock Aser";
			else if (bask1[i].vp == 10) str = "Monoblock Asus";
			else if (bask1[i].vp == 11) str = "Monoblock Gigabyte";
			else if (bask1[i].vp == 12) str = "Eco";
			else if (bask1[i].vp == 13) str = "Standard";
			else if (bask1[i].vp == 14) str = "Advanced";
			else if (bask1[i].vp == 15) str = "Extreme";
			else if (bask1[i].vp == 16) str = "Monitor Dell";
			else if (bask1[i].vp == 17) str = "Monitor Philips";
			else if (bask1[i].vp == 18) str = "Monitor ViewSonic";
			else if (bask1[i].vp == 19) str = "Print HP";
			int r = 0;
			product*prod;
			if ((f = fopen(str, "r")) == NULL) perror("Ошибка открытия файла: ");
			else
			{
				while (!feof(f))
				{
					fgets(tmp, 200, f);
					r++;
				}
				fclose(f);
			}
			prod = (product*)calloc(r, sizeof(product));
			char t;
			if ((f = fopen(str, "r")) == NULL) perror("Ошибка открытия файла: ");
			else
			{
				for (int i = 0; i < r; i++)
				{
					fgets(tmp, 200, f);
					parsStr(&prod[i].code, prod[i].desc, &prod[i].rPrice, &prod[i].oPrice, prod[i].gar, &prod[i].count, tmp);					
				}
				fclose(f);
			}
			for (int j = 0; j < r; j++)
			{
				if (prod[j].code == bask1[i].code)
				{
					prod[j].count -= bask1[i].kol;
				}
			}
			if ((f = fopen(str, "w+")) == NULL) perror("Ошибка открытия файла: ");
			else
			{
				for (int i = 0; i < r; i++)
				{
					fprintf(f, "%d\t", prod[i].code);
					fprintf(f, "%s\t", prod[i].desc);
					fprintf(f, "%d\t", prod[i].rPrice);
					fprintf(f, "%d\t", prod[i].oPrice);
					fprintf(f, "%s\t", prod[i].gar);
					if(i!=r-1)
						fprintf(f, "%d\n", prod[i].count);
					else
						fprintf(f, "%d", prod[i].count);
				}
				fclose(f);
			}
		}
		char p[200]="HISTORY\\";
		char b1[10];
		itoa(b, b1, 10);
		strcat(p, b1);		
			if ((f = fopen(p, "a+")) == NULL) perror("Ошибка открытия файла: ");
			else
			{
				for (int i = 0; i < razm; i++)
				{			
					fprintf(f, "%d\n", bask1[i].vp);
					fprintf(f, "%d\n", bask1[i].code);
					fprintf(f, "%s", bask1[i].desc);
					fprintf(f, "%d\n", bask1[i].Price);
					fprintf(f, "%d\n", bask1[i].kol);
					fprintf(f, "%d\n", bask1[i].sum);
				}
				fclose(f);
			}
			if ((f = fopen("basket", "w")) == NULL) perror("Ошибка открытия файла: ");
			else
			{
				fprintf(f, "");
				fclose(f);
			}		
	}
}

void viewHist(int b)
{
	FILE*f;
	system("cls");
	bask *bask1;
	int razm = 0;
	char p[200] = "HISTORY\\";
	char b1[10];
	char tmp[200];
	itoa(b, b1, 10);
	strcat(p, b1);
	if ((f = fopen(p, "r")) == NULL) perror("Ошибка ");		
	else
	{
		while (!feof(f))
		{
			fgets(tmp, 200, f);
			razm++;
		}
		fclose(f);
	}
	razm = razm / 6;
	bask1 = (bask*)calloc(razm, sizeof(bask));
	yellow(); GotoXY(4, 1);
	printf("Ваши заказы:");
	white(); GotoXY(4, 3);
	for (int i = 0; i < 110; i++)printf("-");
	yellow(); GotoXY(4, 4);
	printf("Код    | Описание"); for (int i = 0; i < 65; i++)printf(" ");
	printf(" | Цена   | Кол-во  | Сумма");
	white(); GotoXY(4, 5);
	for (int i = 0; i < 110; i++)printf("-");
	int stroch = 6;
	tmp[0] = '\0';
	if ((f = fopen(p, "r")) == NULL) perror("Ошибка ");
	else
	{
		for (int i = 0; i < razm; i++)
		{
			fgets(tmp, 9, f);
			bask1[i].vp = atoi(tmp);
			tmp[0] = '\0';
			fgets(tmp, 9, f);
			bask1[i].code = atoi(tmp);
			tmp[0] = '\0';
			fgets(bask1[i].desc, 150, f);
			fgets(tmp, 9, f);
			bask1[i].Price = atoi(tmp);
			tmp[0] = '\0';
			fgets(tmp, 9, f);
			bask1[i].kol = atoi(tmp);
			tmp[0] = '\0';
			fgets(tmp, 9, f);
			bask1[i].sum = atoi(tmp);
			tmp[0] = '\0';
		}
	}
	fclose(f);
	for (int i = 0; i < razm; i++)
	{
		white(); GotoXY(4, stroch);
		printf("       | "); for (int j = 0; j < 74; j++)printf(" "); printf("|        |        | ");
		white(); GotoXY(4, stroch);
		printf("%d", bask1[i].code);
		white(); GotoXY(12, stroch);
		strD(bask1[i].desc, stroch);
		white(); GotoXY(89, stroch);
		printf("%d", bask1[i].Price);
		white(); GotoXY(98, stroch);
		printf("%d", bask1[i].kol);
		white(); GotoXY(107, stroch);
		printf("%d", bask1[i].sum);
		white(); GotoXY(4, stroch + 2);
		for (int j = 0; j < 110; j++)printf("-");
		stroch = stroch + 3;
	}
	stroch = stroch + 1;
	yellow(); GotoXY(12, stroch);
	printf("Итого:");
	yellow(); GotoXY(107, stroch);
	int gs = 0;
	for (int i = 0; i < razm; i++)
	{
		gs += bask1[i].sum;
	}
	printf("%d", gs);
	stroch = stroch + 2;
}

void sortPrC(product*pr, int r)
{
	product tmp;
	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < r - 1; j++)
		{
			if (pr[j].count > pr[j + 1].count)
			{
				tmp = pr[j];
				pr[j] = pr[j + 1];
				pr[j + 1] = tmp;
			}
		}
	}
}

void viewProdA(product*pr, int r, int vp)
{
	FILE *f;
	system("cls");
	int rr;
	user *users1 = creatStructUser(&rr);
	yellow(); GotoXY(4, 2);
	if (vp == 1) printf("Ноутбуки Acer");
	else if (vp == 2) printf("Ноутбуки Asus");
	else if (vp == 3) printf("Ноутбуки Lenovo");
	else if (vp == 4) printf("Планшеты Lenovo");
	else if (vp == 5) printf("Планшеты Samsung");
	else if (vp == 6) printf("Смартфоны Huawei");
	else if (vp == 7) printf("Смартфоны Meizu");
	else if (vp == 8) printf("Смартфоны Sumsung");
	else if (vp == 9) printf("Моноблоки Aser");
	else if (vp == 10) printf("Моноблоки Asus");
	else if (vp == 11) printf("Моноблоки Gigabyte");
	else if (vp == 12) printf("Компьютеры Eco");
	else if (vp == 13) printf("Компьютеры Standard");
	else if (vp == 14) printf("Компьютеры Advanced");
	else if (vp == 15) printf("Компьютеры Extreme");
	else if (vp == 16) printf("Мониторы Dell");
	else if (vp == 17) printf("Мониторы Philips");
	else if (vp == 18) printf("Мониторы ViewSonic");
	else if (vp == 19) printf("Принтерыы HP");
	int v = 0;
	do
	{
		white(); GotoXY(4, 3);
		for (int i = 0; i < 110; i++)printf("-");
		yellow(); GotoXY(4, 4);
		printf("Код    | Описание"); for (int i = 0; i < 65; i++)printf(" ");
		printf(" | Цена   | Гарант.| Наличие");
		white(); GotoXY(4, 5);
		for (int i = 0; i < 110; i++)printf("-");
		int stroch = 6;
		for (int i = 0; i < r; i++)
		{
			if (pr[i].count > 3)
			{
				white(); GotoXY(4, stroch);
				printf("       | "); for (int j = 0; j < 74; j++)printf(" "); printf("|        |        | ");
				white(); GotoXY(4, stroch);
				printf("%d", pr[i].code);
				white(); GotoXY(12, stroch);
				strD(pr[i].desc, stroch);
				white(); GotoXY(89, stroch);
				printf("%d", pr[i].rPrice);
				white(); GotoXY(98, stroch);
				printf("%s", pr[i].gar);
				white(); GotoXY(107, stroch);
				printf("%d", pr[i].count);
				white(); GotoXY(4, stroch + 2);
				for (int j = 0; j < 110; j++)printf("-");
				stroch = stroch + 3;
			}
			else
			{
				white(); GotoXY(4, stroch);
				printf("       | "); for (int j = 0; j < 74; j++)printf(" "); printf("|        |        | ");
				red(); GotoXY(4, stroch);
				printf("%d", pr[i].code);
				red(); GotoXY(12, stroch);
				strD(pr[i].desc, stroch);
				red(); GotoXY(89, stroch);
				printf("%d", pr[i].rPrice);
				red(); GotoXY(98, stroch);
				printf("%s", pr[i].gar);
				red(); GotoXY(107, stroch);
				printf("%d", pr[i].count);
				white(); GotoXY(4, stroch + 2);
				for (int j = 0; j < 110; j++)printf("-");
				stroch = stroch + 3;
			}
		}
		stroch = stroch + 2;
		yellow(); GotoXY(4, stroch);
		printf("Выберите указателем, продукцию для заказывания");
		yellow(); GotoXY(4, stroch + 1);
		printf("Нажмите клавишу S, для сортировки по количеству товара");
		yellow(); GotoXY(4, stroch + 2);
		printf("Нажмите клавишу Esc, для перемещения в верхнее меню");
		int p = 0, k, code;
		stroch = 6;
		yellow(); GotoXY(1, stroch); printf("->");
		do
		{
			k = 0;
			code = getch();
			if (code == 224 || code == 0)
				code = getch();
			if (code == 80)
			{
				if (p < r - 1)
				{
					p += 1;
					yellow(); GotoXY(1, stroch); printf("  ");
					stroch = stroch + 3;
					yellow(); GotoXY(1, stroch); printf("->");
				}
				else
				{
					p = 0;
					yellow(); GotoXY(1, stroch); printf("  ");
					stroch = 6;
					yellow(); GotoXY(1, stroch); printf("->");
				}
			}
			else if (code == 72)
			{
				if (p > 0)
				{
					p -= 1;
					yellow(); GotoXY(1, stroch); printf("  ");
					stroch = stroch - 3;
					yellow(); GotoXY(1, stroch); printf("->");
				}
				else
				{
					p = r - 1;
					yellow(); GotoXY(1, stroch); printf("  ");
					stroch = 6 + ((r - 1) * 3);
					yellow(); GotoXY(1, stroch); printf("->");
				}
			}
			else if (code == 83)
			{
				sortPrC(pr, r);
				yellow(); GotoXY(4, 4);
				printf("Код    | Описание"); for (int i = 0; i < 65; i++)printf(" ");
				printf(" | Цена   | Гарант.| Наличие");
				white(); GotoXY(4, 5);
				for (int i = 0; i < 110; i++)printf("-");
				int stroch = 6;
				for (int i = 0; i < r; i++)
				{
					if (pr[i].count > 3)
					{
						white(); GotoXY(4, stroch);
						printf("       | "); for (int j = 0; j < 74; j++)printf(" "); printf("|        |        | ");
						white(); GotoXY(4, stroch);
						printf("%d", pr[i].code);
						white(); GotoXY(12, stroch);
						strD(pr[i].desc, stroch);
						white(); GotoXY(89, stroch);
						printf("%d", pr[i].rPrice);
						white(); GotoXY(98, stroch);
						printf("%s", pr[i].gar);
						white(); GotoXY(107, stroch);
						printf("%d", pr[i].count);
						white(); GotoXY(4, stroch + 2);
						for (int j = 0; j < 110; j++)printf("-");
						stroch = stroch + 3;
					}
					else
					{
						white(); GotoXY(4, stroch);
						printf("       | "); for (int j = 0; j < 74; j++)printf(" "); printf("|        |        | ");
						red(); GotoXY(4, stroch);
						printf("%d", pr[i].code);
						red(); GotoXY(12, stroch);
						strD(pr[i].desc, stroch);
						red(); GotoXY(89, stroch);
						printf("%d", pr[i].rPrice);
						red(); GotoXY(98, stroch);
						printf("%s", pr[i].gar);
						red(); GotoXY(107, stroch);
						printf("%d", pr[i].count);
						white(); GotoXY(4, stroch + 2);
						for (int j = 0; j < 110; j++)printf("-");
						stroch = stroch + 3;
					}
				}
				stroch = stroch + 2;
				yellow(); GotoXY(4, stroch);
				printf("Выберите указателем, продукцию для заказывания");
			}
			else if (code == 27)
			{
				v = 1;
				break;
			}
			else if (code == 13)
			{
				k = 1;
			}
		} while (k == 0);

		if (v != 1)
		{
			system("cls");
			yellow(); GotoXY(4, 2);
			printf("Вы выбрали:");
			white(); GotoXY(4, 3);
			for (int i = 0; i < 110; i++)printf("-");
			yellow(); GotoXY(4, 4);
			printf("Код    | Описание"); for (int i = 0; i < 65; i++)printf(" ");
			printf(" | Цена   | Гарант.| Наличие");
			white(); GotoXY(4, 5);
			for (int i = 0; i < 110; i++)printf("-");
			stroch = 6;
			white(); GotoXY(4, stroch);
			printf("       | "); for (int j = 0; j < 74; j++)printf(" "); printf("|        |        | ");
			white(); GotoXY(4, stroch);
			printf("%d", pr[p].code);
			white(); GotoXY(12, stroch);
			strD(pr[p].desc, stroch);
			white(); GotoXY(89, stroch);
			printf("%d", pr[p].rPrice);
			white(); GotoXY(98, stroch);
			printf("%s", pr[p].gar);
			white(); GotoXY(107, stroch);
			printf("%d", pr[p].count);
			white(); GotoXY(4, stroch + 2);
			for (int j = 0; j < 110; j++)printf("-");
			stroch = stroch + 5;
			int kol;
			white(); GotoXY(4, stroch);
			printf("Введите количество товара для заказа: "); scanf("%d", &kol);
			pr[p].count += kol;
		}

	} while (v == 0);
	char *str;
	for (int i = 0; i < r; i++)
	{
		if (vp == 1) str = "Notebook Acer";
		else if (vp == 2) str = "Notebook Asus";
		else if (vp == 3) str = "Notebook Lenovo";
		else if (vp == 4) str = "Tablet Lenovo";
		else if (vp == 5) str = "Tablet Samsung";
		else if (vp == 6) str = "Smartphon Huawei";
		else if (vp == 7) str = "Smartphon Meizu";
		else if (vp == 8) str = "Smartphon Sumsung";
		else if (vp == 9) str = "Monoblock Aser";
		else if (vp == 10) str = "Monoblock Asus";
		else if (vp == 11) str = "Monoblock Gigabyte";
		else if (vp == 12) str = "Eco";
		else if (vp == 13) str = "Standard";
		else if (vp == 14) str = "Advanced";
		else if (vp == 15) str = "Extreme";
		else if (vp == 16) str = "Monitor Dell";
		else if (vp == 17) str = "Monitor Philips";
		else if (vp == 18) str = "Monitor ViewSonic";
		else if (vp == 19) str = "Print HP";
		if ((f = fopen(str, "w+")) == NULL) perror("Ошибка открытия файла: ");
		else
		{
			for (int i = 0; i < r; i++)
			{
				fprintf(f, "%d\t", pr[i].code);
				fprintf(f, "%s\t", pr[i].desc);
				fprintf(f, "%d\t", pr[i].rPrice);
				fprintf(f, "%d\t", pr[i].oPrice);
				fprintf(f, "%s\t", pr[i].gar);
				if (i != r - 1)
					fprintf(f, "%d\n", pr[i].count);
				else
					fprintf(f, "%d", pr[i].count);
			}
			fclose(f);
		}
	}
}

