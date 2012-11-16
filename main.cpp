#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include <stdio.h>
#include <limits.h>

using namespace std;

const int data_length=361;
const double r_to_d = 180/3.14159265359;
const int m = 500;

struct angle
{
	int x;
	int y;
	double an;
	double or;
};

struct robot_map	//тестовая карта
{
	unsigned char points[m][m];
};

struct rf_data
{
	int arr[data_length];	//показания дальномера 
};

int init(robot_map& map)	//начальное заполнение карты
{
	for(int i = 0; i < m; ++i)
		for(int j = 0; j < m; ++j)
		{
			map.points[i][j] = 0;
		}
	for(int i = 145; i <= 355; ++i)
		for(int j = 145; j <= 355; ++j)
		{
			map.points[i][j] = 254;
			//cout << map[i][j] << ' ';
		}

	for(int i = 150; i <= 350; ++i)
		for(int j = 150; j <= 350; ++j)
		{
			map.points[i][j] = 0;
			//cout << map[i][j] << ' ';
		}
	return 0;
	}

list<angle> calc_angles_on_map(robot_map& map)
{
	list<angle> ang_list;
	ang_list.clear();
	angle ang[8];	//тестовые углы
	
	ang[0].x = 145;
	ang[0].y = 145;
	ang[0].an = 270.0;
	ang[0].or = 270.0;

	ang[1].x = 355;
	ang[1].y = 145;
	ang[1].an = 270.0;
	ang[1].or = 180.0;

	ang[2].x = 150;
	ang[2].y = 150;
	ang[2].an = 90.0;
	ang[2].or = 0.0;

	ang[3].x = 350;
	ang[3].y = 150;
	ang[3].an = 90.0;
	ang[3].or = 270.0;

	ang[4].x = 150;
	ang[4].y = 350;
	ang[4].an = 90.0;
	ang[4].or = 90.0;

	ang[5].x = 350;
	ang[5].y = 350;
	ang[5].an = 90.0;
	ang[5].or = 180.0;

	ang[6].x = 145;
	ang[6].y = 355;
	ang[6].an = 270.0;
	ang[6].or = 0.0;

	ang[7].x = 355;
	ang[7].y = 355;
	ang[7].an = 270.0;
	ang[7].or = 90.0;

	for(int i = 0; i < 8; ++i)
	{
		ang_list.push_back(ang[i]);
	}
	return ang_list;
}

int get_rf_data(int* arr) //загрузка данных с дальномера
{
	FILE *f1 = fopen ("d://res.txt", "rt");
	if(f1 == NULL)
	{
		cout<< "File not found" << endl;
		return -1;
	}
	for(int i = 0; i < data_length; ++i)
	{
		fscanf(f1, "%d", & (arr[i]));
		//arr[i]=10000*sin(2*i*3.14159265359/180);
		cout<<arr[i] << ' ';
	}
	fclose(f1);
	return 0;
}

list<angle> calc_angels_rf(int* arr)	//вычисление углов с дальномера
{
	list<angle> ang_list;
	ang_list.clear();
	
	int ex[data_length];	//точки экстремумы
//	int extype[data_length];	//тип вершины
	double angl[data_length];	//угол между точкой отсчёта и экстремумом
	double anglex[data_length];	//угол между прямыми
	int anglx[data_length];	//координата х экстремумов
	int angly[data_length];	//координата у экстремумов
	int allx[data_length];	//координаты x всех точек
	int ally[data_length];	//координаты y всех точек
	double k[data_length];	//параметр k из уравнения прямой
	double b[data_length];	//параметр b из уравнения прямой
	double bh[data_length];
	double bl[data_length];
	double angl2[data_length];	//угол между роботом и углом

		for(int i = 0; i < data_length; ++i)
	{
		ex[i] = 0;
//		extype[i] = 0;
		angl[i] = 0;
		anglex[i] = 0;
		angl2[i] = 0;
		anglx[i] = 0;
		angly[i] = 0;
		allx[i] = 0;
		ally[i] = 0;
		k[i] = 0;
		b[i] = 0;
		bh[i] = 0;
		bl[i] = 0;

	}

	//заполнение вектора экстремумов
	for(int i = 0; i < data_length - 1; ++i)
	{
		if(arr[i] == arr[i + 1]){

			int s = 0;
			do
			++s;
			while(arr[i] == arr[i + s]);
			if(arr[i - 1] > arr[i] && arr[i + s + 1] > arr[i + s] || arr[i - 1] < arr[i] && arr[i + s + 1] < arr[i + s])
				ex[i + s/2] = arr[i];
		}

		if(arr[i] > arr[i+1] && arr[i] > arr[i-1] || arr[i] < arr[i+1] && arr[i] < arr[i-1])
		{
			ex[i] = arr[i];
			//cout << '[' << ex[i] << ' ' << i/2 << ']' << '_';	//вывод значения экстремума и номера ячейки экстремума
		}
	}
	
	//нахождение угла экстремумов
	int n = 0;
//	int an = 0;
	for(int i = 0; i < data_length; i++)
	{
		if(ex[i])
		{
			angl[i] = i/2;
		}
	}

	//нахождение координат X и Y
	int t = 0;
	for(int i = 0; i < data_length; ++i)
	{
		if(ex[i])
		{
			angly[i] = arr[i]*sin((angl[i])/r_to_d);
			anglx[i] = arr[i]*cos((angl[i])/r_to_d);
			//cout << '(' << anglx[t] << ',' << angly[t] << ')' << ' ';
		}
	}

	//нахождение координат всех точек
	for(int i = 0; i < data_length; ++i)
	{
		ally[i] = arr[i]*sin(i/2/r_to_d);
		allx[i] = arr[i]*cos(i/2/r_to_d);
		//cout << '(' << allx[i] << ',' << ally[i] << ')' << ' ';
	}

	//определение уравнений прямых рядом с экстремумами
	for(int i = 0; i < data_length - 2; ++i)
	{
		if(ex[i])
		{
			int s = 1;
			do{
				s++;
				if((allx[i] - allx[i - s]) == 0)
					k[i - 1] = INT_MAX;
				else
					k[i - 1] = (double)(ally[i] - ally[i - s]) / (allx[i] - allx[i - s]);
				if((allx[i] - allx[i - s]) == 0)
					b[i - 1] = INT_MAX;
				else
					b[i - 1] = (double)(ally[i - s] - (((ally[i] - ally[i - s]) / 
					(allx[i] - allx[i - s]))*allx[i - s]));
				if(cos(atan(k[i - 1])) == 0)
				{
					b[i - 3] = INT_MAX;
					b[i - 2] = INT_MAX;
				}
				else
				{
					b[i - 3] = (double)(b[i - 1] - 3/cos(atan(k[i - 1])));
					b[i - 2] = (double)(b[i - 1] + 3/cos(atan(k[i - 1])));
				}
			}while((double)(k[i - 1]*allx[i] - ally[i] + b[i - 3] ) < 0 
				&& (double)(k[i - 1]*allx[i] - ally[i] + b[i - 2]) > 0 
				&& (i + s) > 0 && (i + s) < data_length);
			s = 1;
			do{
				s++;
				if((allx[i + 2] - allx[i]) == 0)
					k[i + 1] = INT_MAX;
				else
					k[i + 1] = (double)(ally[i + 2] - ally[i]) / 
					(allx[i + 2] - allx[i]);
				if((allx[i + 2] - allx[i]) == 0)
					b[i + 1] = INT_MAX;
				else
					b[i + 1] = (double)(ally[i] - (((ally[i + 2] - ally[i]) / 
					(allx[i + 2] - allx[i]))*allx[i]));
				if(cos(atan(k[i - 1])) == 0)
				{
					b[i + 3] = INT_MAX;
					b[i + 2] = INT_MAX;
				}
				else
					b[i + 3] = (double)(b[i - 1] + 3/cos(atan(k[i - 1])));
					b[i + 2] = (double)(b[i - 1] - 3/cos(atan(k[i - 1])));
			}while((double)(k[i + 1]*allx[i] - ally[i] + b[i + 2] ) < 0 
				&& (double)(k[i + 1]*allx[i] - ally[i] + b[i + 3]) > 0 
				&& (i + s) > 0 && (i + s) < data_length);
			

		}
	}
	/*	//определение уравнений прямых рядом с экстремумами
	for(int i = 0; i < data_length - 2; ++i)
	{
		if(ex[i])
		{
			if((allx[i] - allx[i - 2]) == 0)
				k[i - 1] = INT_MAX;
			else
				k[i - 1] = (double)(ally[i] - ally[i - 2]) / (allx[i] - allx[i - 2]);
			
			if((allx[i + 2] - allx[i]) == 0)
				k[i + 1] = INT_MAX;
			else
				k[i + 1] = (double)(ally[i + 2] - ally[i]) / (allx[i + 2] - allx[i]);
			
			if((allx[i] - allx[i - 2]) == 0)
				b[i - 1] = INT_MAX;
			else
				b[i - 1] = (double)(ally[i - 2] - (((ally[i] - ally[i - 2]) / (allx[i] - allx[i - 2]))*allx[i - 2]));
			
			if((allx[i + 2] - allx[i]) == 0)
				b[i + 1] = INT_MAX;
			else
				b[i + 1] = (double)(ally[i] - (((ally[i + 2] - ally[i]) / (allx[i + 2] - allx[i]))*allx[i]));
		}
	}
*/

/*	//определение типа вершины
	for(int i = 0; i < data_length - 1; ++i)
	{
		if(arr[i] > arr[i+1] && arr[i] > arr[i-1] || arr[i] < arr[i+1] && arr[i] < arr[i-1])
		{
			if(arr[i] > arr[i+1] && arr[i] > arr[i-1])
				extype[i] = 1;
			else extype[i] = 2;
		}
	}
*/
	//определение угла между прямыми экстремума
	for(int i = 0; i < data_length - 1; ++i)
	{
		if(ex[i]){
			if(ex[i] < 360/2)
				anglex[i] = r_to_d*atan((double)(k[i + 1] - k[i - 1]) / (1 + k[i - 1] * k[i + 1]));
			else
				anglex[i] = r_to_d*atan((double)(k[i - 1] - k[i + 1]) / (1 + k[i - 1] * k[i + 1]));
		}
			/*			if(!(anglex[i] > 45 || anglex[i] < -45))
			{
				ex[i] = 0;
				anglex[i] = 0;
			}
*/		
	}
		//определение угла между роботом и углом
	for(int i = 0; i < data_length; ++i)
	{
		if(ex[i])
		{
			angl2[i] = (180 - angl[i]) + (r_to_d*atan((double)k[i - 1]));
		}
	}

//Заполнение списка
	for(int i = 0; i < data_length - 1; ++i)
	{
		angle tmp;
		if(anglex[i]){
			tmp.x = anglx[i];
			tmp.y = angly[i];
			tmp.an = anglex[i];
			tmp.or = angl2[i];
			ang_list.push_back(tmp);
		}
	}

	return ang_list;
}

int avrg()
{
	return 0;
}

//вывод экстремумов
int output(list<angle>& anglex)
{
	list<angle>::iterator it;
	for(it=anglex.begin(); it!= anglex.end(); ++it)
		cout << (*it).x<<' ' <<(*it).y<<' '<<(*it).an<<' '<<(*it).or << endl;
	return 0;
}


//функция принимает карту, углы с карты, углы с дальномера
//для каждого угла с дальномера сравнить по величине со всеми углами 
//с карты, если совпадает - 
/*list<angle> compare(robot_map& map, )
	for(int i = 0; i < data_length; ++i)
		if(angle)
*/

int main()
{
	//создание и загрузка карты
	robot_map map;
	//init(map);
	
	//расчёт углов на карте
	list<angle> an_list_map = calc_angles_on_map(map);

	int arr[data_length];	//показания дальномера
	get_rf_data(arr);

	list<angle> an_list_rf = calc_angels_rf(arr);
	cout<<endl;
	output(an_list_rf);
	cout<<endl;
	//output(an_list_map);

	
	//int avrg[data_length];	//скользящая средняя




/*	for(int i = 0; i < data_length; ++i)
	{
		ex[i] = 0;
		extype[i] = 0;
//		avrg[i] = 0;
		angl[i] = 0;
		anglex[i] = 0;
		angl2[i] = 0;
		anglx[i] = 0;
		angly[i] = 0;
		allx[i] = 0;
		ally[i] = 0;
		k[i] = 0;
		b[i] = 0;
	}
*/
	

/*	//вычисление скользящей средней
	int a = 12;	//значение усреднения(только чётное),  при 0 - исходные значения
	int y = 0;
	for(int i = a/2; i < data_length - a/2; i++)
	{
		for(int s = 0; s <= a; s++)
			y += arr[i - (a/2) + s];

		avrg[i] = y/(a+1);
		y = 0;
	}
*/

/*	//вывод экстремумов
	for(int i = 0; i < data_length - 1; ++i)
		if(anglex[i])
			cout << anglex[i] << endl;
*/	


//заполнение матрицы данными

/*	for(int i = 0; i <= 500; ++i)
		for(int j = 0; j <= 500; ++j)
		{
			cout << map[i][j] << ' ';
		}*/

	//list < int > int_list;
}

/*	//определение уравнений прямых рядом с экстремумами
	for(int i = 0; i < data_length - 2; ++i)
	{
		if(ex[i])
		{
			if((allx[i] - allx[i - 2]) == 0)
				k[i - 1] = INT_MAX;
			else
				k[i - 1] = (double)(ally[i] - ally[i - 2]) / (allx[i] - allx[i - 2]);
			
			if((allx[i + 2] - allx[i]) == 0)
				k[i + 1] = INT_MAX;
			else
				k[i + 1] = (double)(ally[i + 2] - ally[i]) / (allx[i + 2] - allx[i]);
			
			if((allx[i] - allx[i - 2]) == 0)
				b[i - 1] = INT_MAX;
			else
				b[i - 1] = (double)(ally[i - 2] - (((ally[i] - ally[i - 2]) / (allx[i] - allx[i - 2]))*allx[i - 2]));
			
			if((allx[i + 2] - allx[i]) == 0)
				b[i + 1] = INT_MAX;
			else
				b[i + 1] = (double)(ally[i] - (((ally[i + 2] - ally[i]) / (allx[i + 2] - allx[i]))*allx[i]));
		}
	}
*/