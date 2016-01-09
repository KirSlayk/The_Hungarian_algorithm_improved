#include <iostream>
#include <fstream> // для рабоыт с файлами
#include "vengr.h" // наш файл
#include <assert.h> // для функции assert() которая выкидывает остановку проги
#include <ctype.h> // нужна для isdigit(char c), которая проверяет -- является ли данный символ числом.

#pragma once // дирекитва подключения всех файло единожды (от коллизий, кароч типа чтобы vector.h не подключался дважды)




int main(int argc, char *argv[]) {
	setlocale(LC_ALL, "rus"); // чтобы русский понимал
	
	ofstream output_file("out.txt", ios_base::trunc | ios_base::out); // trunc -- удалить содержимое, out -- открыть для записи
	
	if (!output_file.is_open()) // проверям: открыт ли файл. Если нет -- кидаем останову
		assert(!output_file.is_open());
	
	
	if (argc >= 2) { // если количество наших аргументов хотя бы равно 2 (под первым аргументом всегда имя проги), то норм, рез будет записываться в out.txt
		
		if (argc == 3) { // если же кол-во аргументов 3 -- то вероятно, что мы написали еще выходной файл в аргументы
			output_file.close(); // закрываем наш out.txt
			output_file.open(argv[2], ios_base::trunc | ios_base::out); // открываем новый выходной файл, который мы прописали
			
			if (!output_file.is_open()) // проверям: открыт ли наш файл
				assert(!output_file.is_open()); // кидаем останову, если не открыт
		}
		ifstream input_file(argv[1], ios_base::in); // инициализируем наш входной файл
		//ifstream input_file("input.txt", ios_base::in); // это для дебага было 
		if (!input_file.is_open()) // если не открыт -- кидаем останову
			assert(!input_file.is_open());
		
		size_t height = 0, // высота нашей матрицы
			width = 0; // ширана
		const size_t buffer_size = 16; // макс кол-во символов в моей проге в первой строке входной программы ( то есть гипотетически -- макс разрядов в одном из 
		// значений высоты/ширины матрицы равно 14

		char buffer[buffer_size]; //  буфер для первой строки входного файла
		input_file.getline(buffer, buffer_size); // считываем первую строку
		
		bool way = false; // побочная переменная для ветвления
		
		
		for (size_t i = 0; buffer[i] && i < buffer_size; ++i) {
			if (isdigit(buffer[i])) { // проверяем -- является ли очередной символ цифоргой
				if (!way) {
					height = height * 10 + (buffer[i] - '0'); // записываем в высоту
				}
				else {
					width = width * 10 + (buffer[i] - '0'); // записываем в ширину
				}
			}
			else if (buffer[i] == ' ') // если встретился пробел, то переходим в другую ветку ветвления в итоге
				if (width)
					break;
				way = true;
		}

		vector<vector<int>> arr(height, vector<int>(width)); // создаем матрицу
		size_t i = 0,
			j = 0;
		char c;
		input_file.get(c); //считываем символ очередной
		while (c != EOF && i != height) { //пока не конец файла и второе условие
			if (isdigit(c)) { // цифорга?
				arr[i][j] = arr[i][j] * 10 + (c - '0'); // записываем элемент
			}
			else if ((c == ' ' || c == '\n') && arr[i][j]) { // если пробел или перенос строки
				j++;
				if (j == width) {
					j = 0;
					i++;
				}
			}
			input_file.get(c); //очередной символ
		}

		vector<pair<int,int>> ans(width); //создаем вектор пар. Каждый элемент вектора содержит два значения
		HungarianMatrix<int> matrix(height, width, arr);
		ans = matrix.startHungarianAlg(); // выполняем алгоритм, результат записываем в новый вектор пар

		for (size_t i = 0; i < width; ++i) {
			output_file << ans[i].second << "-" << ans[i].first << endl; // записываем в выходной файл все
		}
		input_file.close(); //закрываем файлы
		output_file.close();
	}
	//system("pause");
	return 0;
}