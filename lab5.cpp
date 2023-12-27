#include <Windows.h>
#include <iostream>
#include <fstream>
#define filename "out.sce"
#include <cmath>
using std::ofstream;
using std::cout;
#define D0 0.35
#define B 0.28
//вариант 5

//нелинейность D 
double D(double T)
{
	if (T != 0.0)
		return D0 * pow(T, B);
	else	
		return D0;
}

//вычисление максимума D на слое
double D_i_max(double* T, int N)
{
	double max = 0;
	for (int i = 0; i < N; i++)
	{
		if (D(T[i]) > max)
			max = D(T[i]);
	}
	return max;
}

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	double xleft = 0, xright = 1; //расчётная область x
	double k = 0.5; //k < 1 коэфициент
	double h = 0.01, t = 0; //шаг t изначально 0
	double time = 0, tmax = 4.0; //счетчик времени и предел
	int N = (xright - xleft) / h + 1.0; //вычисление количества шагов x

	double* T1 = new double[N];	//массивы слоёв
	double* T2 = new double[N];

	cout<<"Шагов N = "<<N<<"\n";
	cout<<"Шаг h = "<<h<<"\n";
	cout<<"Шаг t = "<<t<<"\n";
	cout<<"Временных слоёв Nt = вычисляется...\n\n";

	int Nu = 0; //начальное условие
	int GuL = -2;	//граничные условия T
	int GuR = 1;
	
	for (int i = 1; i < N - 1; i++)	//заполнение начального слоя
		T1[1] = Nu;
	
	t = k * h * h / (2 * D_i_max(T1, N)); //пересчёт шага
	time += t;
	int layersdone = 1; //счётчик для вывода
	int z = 1;


	ofstream fout(filename);    //файл для построения графика
		if (!fout) fout << "File failure\n";
	
	while (time <= tmax) //главный цикл
	{
		for (int i = 1; i < N - 1; i++)	//вычисление слоя2
		{
			T2[i] = (t / (h * h)) * (D((T1[i + 1] + T1[i]) / 2.0) * (T1[i + 1] - T1[i]) - 
				D((T1[i] + T1[i - 1]) / 2.0) * (T1[i] - T1[i - 1])) + T1[i];
		}
		T2[0] = T2[1] - (h * GuL); //левое 2 рода
		T2[N - 1] = GuR;	//правое 1 рода

		t = k * h * h / (2 * D_i_max(T1, N)); //пересчёт шага

		for (int i = 0; i < N; i++) //обновление слоя
			T1[i] = T2[i];

		if (((time >= tmax / 4 ) && (z == 1)) || 
			((time >= tmax / 4 * 2) && (z == 2)) || 
			((time >= tmax / 4 * 3) && (z == 3)) || 
			((time >= tmax - 2 * t) && (z == 4)))
		{
			cout<<"Вывод слоя "<<layersdone<<"\t t = "<<time<<"\n";
			fout<<"U"<<z<<"=[";
			for (int i=0; i<N; i++)
			{
				fout<<T2[i];
				if (i<N-1)
					fout<<",";
			}
			fout<<"]\n";
			z++;
		}
		time += t;
		layersdone++;
	}

	//вывод в файл всех x
	fout<<"x=[";
	for (int i=0; i<N; i++)
	{
		fout<<xleft + i*h;
		if (i < N - 1)
			fout<<",";
	}
	fout<<"]\n";

	//вывод в файл нулевого слоя
	fout<<"U0=[";
	for (int i=0; i<N; i++)
	{
		fout<<Nu;
		if (i<N-1)
			fout<<",";
	}
	fout<<"]\n";

	cout<<z<<" линий\n";
	cout<<layersdone<<" слоёв\n";
	fout<<"plot2d(x,U0,style=color(\"blue\"))\n"; // t = 0
	fout<<"plot2d(x,U1,style=color(\"cyan\"))\n";
	fout<<"plot2d(x,U2,style=color(\"pink\"))\n";
	fout<<"plot2d(x,U3,style=color(\"green\"))\n";
	fout<<"plot2d(x,U4,style=color(\"magenta\"))\n";

	fout.close();

	cout << "Построение графика\n";
	system("\"\\Program Files\\scilab-6.1.0\\bin\\WScilex.exe\" -f out.sce");
	system("pause");
	delete[] T1;
	delete[] T2;
	return 0;
}