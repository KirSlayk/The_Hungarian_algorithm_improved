// реализация алгоритма
#include "vengr.h"


template <typename T>
void HungarianMatrix<T>::smdir() { // step 1 substract min degit in rows
	bool way;
	for (size_t i = 0; i < height; ++i) {
		way = true;
		for (size_t j = 0; j < width; ++j) {
			matrix[i][j].value -= min_value_in_every_rows[i];
			if (!matrix[i][j].value && way) {
				matrix[i][j].state = Node::FIRSTSTATE; // step 2
				way = false;
			}
		}
	}
}

template <typename T>
bool HungarianMatrix<T>::czc() { // step 3 check for zero in each column
	for (size_t i = 0; i < height; ++i) 
		for (size_t j = 0; j < width; ++j) 
			if (matrix[i][j].state == Node::FIRSTSTATE) 
				check_column[j] = 1;
	size_t count = 0;
	for (size_t i = 0; i < width; ++i) 
		count += check_column[i];
	if (count >= width || count >= height)
		return true;
	else return false;
	
}

template <typename T>
void HungarianMatrix<T>::replaceZeros() { // step 4
	int row = -1,
		col = -1;
	bool done = false;
	while (!done) {
		findZero(row, col);
		if (row = -1) {
			done = true;
			changeMatrix();
		}
		else {
			matrix[row][col].state = Node::SECONDSTATE;
			if (starInRow(row)) {
				findStarInRow(row, col);
				check_rows[row] = 1;
				check_column[col] = 0;
			}
			else {
				done = true;
				path_col0 = col;
				path_row0 = row;
				csaz();
			}
		}
	}
}

template <typename T>
void HungarianMatrix<T>::findZero(int &row, int &col) { // find some new zero
	size_t r = 0,
		c = 0;
	bool done = false;
	row = -1;
	col = -1;
	while (!done) {
		c = 0;
		while (c < width && !done) {
			if (matrix[r][c].value == 0 && check_column[c] == 0 && check_rows[r] == 0) {
				row = r;
				col = c++;
				done = true;
			}
		}
		r += 1;
		if (r >= height)
			done = true;
	}
}

template <typename T>
bool HungarianMatrix<T>::starInRow(int row) const {
	for (size_t c = 0; c < width; ++c)
		if (matrix[row][c].state == Node::FIRSTSTATE)
			return true;
	return false;
}

template <typename T>
void HungarianMatrix<T>::findStarInRow(int row, int &col) const {
	col = -1;
	for (size_t c = 0; c < width; ++c)
		if (matrix[row][c].state == Node::FIRSTSTATE)
			col = c;
}

template <typename T>
void HungarianMatrix<T>::csaz() { // step 5; constact a series of alternating zeros;
	bool done = false;
	int r = -1,
		c = -1,
		path_count = 1;
	vector<vector<int>>path(height, vector<int>(width));
	for (size_t i = 0; i < height; ++i) {
	//	path[i].resize(width);
		memset(&path[i].begin(), 0, sizeof(path[i]));
	}
	path[path_count - 1][0] = path_row0;
	path[path_count - 1][1] = path_col0;
	while (!done) {
		findStarInCol(path[path_count - 1][1], r);
		if (r > -1) {
			path_count++;
			path[path_count - 1][0] = r;
			path[path_count - 1][1] = path[path_count - 2][1];
		}
		else {
			done = true;
		}
		if (!done) {
			findPrimeInRow(path[path_count - 1][0], c);
			path_count++;
			path[path_count - 1][0] = path[path_count - 2][0];
			path[path_count - 1][1] = c;
		}
	}
	augmentPath(path_count, path);
	clearCovers();
	erasePrimes();
	for (size_t i = 0; i < height; ++i)
		path[i].clear();
	path.clear();
	czc();
}

template <typename T>
void HungarianMatrix<T>::findStarInCol(int c, int &r) const {
	r = -1;
	for (size_t i = 0; i < height; ++i)
		if (matrix[i][c].state == Node::FIRSTSTATE)
			r = i;
}

template <typename T>
void HungarianMatrix<T>::findPrimeInRow(int r, int &c) const {
	for (size_t i = 0; i < width; ++i)
		if (matrix[r][i].state == Node::SECONDSTATE)
			c = i;
}

template <typename T>
void HungarianMatrix<T>::augmentPath(int &path_count, vector<vector<int>> &path) {
	for (int i = 0; i < path_count; ++i) {
		if (matrix[path[i][0]][path[i][1]].state == Node::FIRSTSTATE)
			matrix[path[i][0]][path[i][1]].state = Node::ZEROSTATE;
		else
			matrix[path[i][0]][path[i][1]].state = Node::FIRSTSTATE;
	}
}

template <typename T>
void HungarianMatrix<T>::clearCovers() {
	for (size_t i = 0; i < height; ++i)
		check_rows[i] = 0;
	for (size_t i = 0; i < width; ++i)
		check_column[i] = 0;
}

template <typename T>
void HungarianMatrix<T>::erasePrimes() {
	for (size_t i = 0; i < height; ++i)
		for (size_t j = 0; j < width; ++j)
			if (matrix[i][j].state == Node::SECONDSTATE)
				matrix[i][j].state = Node::ZEROSTATE;
}

template <typename T>
void HungarianMatrix<T>::changeMatrix() { // step 6
	int min = matrix[0][0].value;
	findMinInMatrix(min);
	for (size_t i = 0; i < height; ++i)
		for (size_t j = 0; j < width; ++j) {
			if (check_rows[i] == 1)
				matrix[i][j].value += min;
			if (check_column[j] == 0)
				matrix[i][j].value -= min;
		}
	replaceZeros();
}

template <typename T>
void HungarianMatrix<T>::findMinInMatrix(int &min) const {
	for (size_t i = 0; i < height; ++i)
		for (size_t j = 0; j < width; ++j)
			if (check_column[j] == 0 && check_rows[i] == 0 && min > matrix[i][j].value)
				min = matrix[i][j].value;
}



template <typename T>
vector<pair<T,T>> HungarianMatrix<T>::startHungarianAlg() {
	vector<pair<T,T>> ans(width);
	smdir(); // substract min element of every element in rows and star zero
	if (!czc()) // check for zero in every column
		replaceZeros();
	else {
		for (size_t i = 0; i < width; ++i)
			for (size_t j = 0; j < height; ++j)
				if (matrix[i][j].state == Node::FIRSTSTATE) {
					ans[i].first = i;
					ans[i].second = j;
				}
		return ans;
	}
	return ans;

}