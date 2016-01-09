// ���� ��� �������� ������ ���������
/*
* ������ ������ � ����������� ���������� �������.
* matrix: ������������� ������� �� ����� ����� (�� ����������� �������������).
*         ������ ������� ������ ���� �� ������ ������.
* ����������: ������ ��������� ���������, �� ������ �� ������ ������ �������.
*/
#include <vector>
#include <limits>


#pragma once

using namespace std;


template <typename T>
class HungarianMatrix {
private:
	struct Node {
		T value;
		enum {ZEROSTATE, FIRSTSTATE, SECONDSTATE} state;
		Node() : state(Node::ZEROSTATE){}
		T &operator=(T &value) { this->value = value; return this->value; }
	};

	vector<vector<Node>> matrix;
	size_t height,
		width;
	vector<T> min_value_in_every_rows;
	vector<char> check_rows,
		check_column;
	int path_row0,
		path_col0;

	T min(T &first, T &second) const { return first > second ? second : first; }

	void smdir(); // substract minimum digit in rows; ������� ����������� ����� � ������ ������ � �������� �� ������� ����� � ���� ������ ��� �����
	
	bool czc(); // check for zero in each column; ��������� ������� ���� � ������ �������, ���� � ������ ������� ���� "stared zero", �� ��� ��, ��� �������
	// ��� "stared zero" � ����� ��������, ����� ��������� � ��������� ��� ���������

	void replaceZeros(); // �� ��� ������ ���������, ������ ���. � �����-�� �� ������ �������� ���� "stared zero" ��� �������������

	void findZero(int &row, int &col);

	bool starInRow(int row) const;

	void findStarInRow(int row, int &col) const;

	void setFirstState(Node &value) { value.state = Node::FIRSTSTATE; }

	void csaz(); // constract new series of alternating zeros

	void findStarInCol(int c, int &r) const;

	void findPrimeInRow(int r, int &c) const;

	void augmentPath(int &path_count, vector<vector<int>> &path);

	void clearCovers();

	void erasePrimes();

	void changeMatrix(); // ��� ������������� ���������� ��� ������������ ������� � replaceZeros()

	void findMinInMatrix(int &min) const;


public:
	HungarianMatrix(){}
	HungarianMatrix(size_t height, size_t width, vector<vector<T>> &arr): height(height), width(width) { 
		matrix.resize(height);
		min_value_in_every_rows.resize(height);
		check_rows.resize(height);
		check_column.resize(width);
		memset(&check_column.begin(), 0, sizeof(check_column));
		memset(&check_rows.begin(), 0, sizeof(check_rows));
		memset(&min_value_in_every_rows.begin(), -1, sizeof(min_value_in_every_rows));
		for (size_t i = 0; i < height; ++i) {
			matrix[i].resize(width);
			for (size_t j = 0; j < width; ++j) {
				matrix[i][j] = arr[i][j];
				min_value_in_every_rows[i] = min_value_in_every_rows[i] == -1 ? arr[i][j] : min(min_value_in_every_rows[i], arr[i][j]);
			}
		}
	}
	~HungarianMatrix(){ for (size_t i = 0; i < height; ++i) matrix[i].clear(); matrix.clear(); min_value_in_every_rows.clear(); }
	vector<pair<T,T>> startHungarianAlg();

};




