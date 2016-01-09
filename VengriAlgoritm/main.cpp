#include <iostream>
#include <fstream> // ��� ������ � �������
#include "vengr.h" // ��� ����
#include <assert.h> // ��� ������� assert() ������� ���������� ��������� �����
#include <ctype.h> // ����� ��� isdigit(char c), ������� ��������� -- �������� �� ������ ������ ������.

#pragma once // ��������� ����������� ���� ����� �������� (�� ��������, ����� ���� ����� vector.h �� ����������� ������)




int main(int argc, char *argv[]) {
	setlocale(LC_ALL, "rus"); // ����� ������� �������
	
	ofstream output_file("out.txt", ios_base::trunc | ios_base::out); // trunc -- ������� ����������, out -- ������� ��� ������
	
	if (!output_file.is_open()) // ��������: ������ �� ����. ���� ��� -- ������ ��������
		assert(!output_file.is_open());
	
	
	if (argc >= 2) { // ���� ���������� ����� ���������� ���� �� ����� 2 (��� ������ ���������� ������ ��� �����), �� ����, ��� ����� ������������ � out.txt
		
		if (argc == 3) { // ���� �� ���-�� ���������� 3 -- �� ��������, ��� �� �������� ��� �������� ���� � ���������
			output_file.close(); // ��������� ��� out.txt
			output_file.open(argv[2], ios_base::trunc | ios_base::out); // ��������� ����� �������� ����, ������� �� ���������
			
			if (!output_file.is_open()) // ��������: ������ �� ��� ����
				assert(!output_file.is_open()); // ������ ��������, ���� �� ������
		}
		ifstream input_file(argv[1], ios_base::in); // �������������� ��� ������� ����
		//ifstream input_file("input.txt", ios_base::in); // ��� ��� ������ ���� 
		if (!input_file.is_open()) // ���� �� ������ -- ������ ��������
			assert(!input_file.is_open());
		
		size_t height = 0, // ������ ����� �������
			width = 0; // ������
		const size_t buffer_size = 16; // ���� ���-�� �������� � ���� ����� � ������ ������ ������� ��������� ( �� ���� ������������� -- ���� �������� � ����� �� 
		// �������� ������/������ ������� ����� 14

		char buffer[buffer_size]; //  ����� ��� ������ ������ �������� �����
		input_file.getline(buffer, buffer_size); // ��������� ������ ������
		
		bool way = false; // �������� ���������� ��� ���������
		
		
		for (size_t i = 0; buffer[i] && i < buffer_size; ++i) {
			if (isdigit(buffer[i])) { // ��������� -- �������� �� ��������� ������ ��������
				if (!way) {
					height = height * 10 + (buffer[i] - '0'); // ���������� � ������
				}
				else {
					width = width * 10 + (buffer[i] - '0'); // ���������� � ������
				}
			}
			else if (buffer[i] == ' ') // ���� ���������� ������, �� ��������� � ������ ����� ��������� � �����
				if (width)
					break;
				way = true;
		}

		vector<vector<int>> arr(height, vector<int>(width)); // ������� �������
		size_t i = 0,
			j = 0;
		char c;
		input_file.get(c); //��������� ������ ���������
		while (c != EOF && i != height) { //���� �� ����� ����� � ������ �������
			if (isdigit(c)) { // �������?
				arr[i][j] = arr[i][j] * 10 + (c - '0'); // ���������� �������
			}
			else if ((c == ' ' || c == '\n') && arr[i][j]) { // ���� ������ ��� ������� ������
				j++;
				if (j == width) {
					j = 0;
					i++;
				}
			}
			input_file.get(c); //��������� ������
		}

		vector<pair<int,int>> ans(width); //������� ������ ���. ������ ������� ������� �������� ��� ��������
		HungarianMatrix<int> matrix(height, width, arr);
		ans = matrix.startHungarianAlg(); // ��������� ��������, ��������� ���������� � ����� ������ ���

		for (size_t i = 0; i < width; ++i) {
			output_file << ans[i].second << "-" << ans[i].first << endl; // ���������� � �������� ���� ���
		}
		input_file.close(); //��������� �����
		output_file.close();
	}
	//system("pause");
	return 0;
}