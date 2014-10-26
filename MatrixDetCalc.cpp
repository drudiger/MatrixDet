#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <vector>

using namespace std;


long findDet(vector<vector<int>> m, int n);

int main() {
	int size = 0;
	cout << "Welcome to the Matrix Determinant Calculator.\n";

	while (true) {									//Get size of matrix, and store in stringstream
		cout << "Please enter the size \"n\" of an n by n matrix: ";
		string inSize;
		getline(cin, inSize);
		stringstream s(inSize);
		if (s >> size && size >= 0)
			break;
		cout << "Invalid input. Please enter a positive integer.\n";
	}

	vector<vector<int>> matrix;
	int temp;
	vector<int> tempVec;

	string inRow;
	matrix.resize(size);							//Create matrix of size n
	for (int i = 0; i < size; i++) {
		retry:
		tempVec.clear();
		cout << "Please enter row " << i + 1 << " of your matrix with each number separated by a space: ";
		getline(cin, inRow);
		stringstream t(inRow);
		for (int j = 0; j < size; j++){
			if (!(t >> temp)){						//Fail if input can't be stored in a stringstream
				cout << "Invalid input. Please try again.\n";
				t.clear();
				t.str("");
				goto retry;
			}
			else
				tempVec.push_back(temp);			//Store each number in tempVec
		}
		matrix[i] = tempVec;						//tempVec should now be a row of numbers; store it in matrix
	}
	string yn;
	string num;
	do{
		int max = 0;
		for (int i = 0; i < size; i++)				//Calculate the longest number in the matrix, so the matrix can be displayed without overlap
			for (int j = 0; j < size; j++)
				if (abs(matrix[i][j]) > max)
					max = abs(matrix[i][j]);

		cout << "The matrix you entered is:";
		for (int i = 0; i < size; i++) {
			cout << endl;
			for (int j = 0; j < size; j++)			//Displays matrix the user entered, and ensures that there's at least 3 spaces between each number
				cout << setw((streamsize)(log(max)/log(10)+3)) << matrix[i][j];
		}
		
		cout << "\nIs this correct? y/n: ";			//Confirmation that matrix is correct
		getline(cin, yn);
		while (yn != "n" && yn != "y" && yn != "N" && yn != "Y"){
			cout << "Please enter \"y\" or \"n\": ";
			getline(cin, yn);
		}
		
		if (yn == "n" || yn == "N"){				//Allows user to change specific matrix entries that may have been entered incorrectly
			cout << "Enter the position you would like to change in <row> <column> format, e.g., enter \"1 2\" to specify the number in row 1, column 2: ";
			take2:
			getline(cin, num);
			stringstream r(num);
			int row, col, val;
			if (!(r >> row && r >> col)){
				cout << "Incorrect format. Please try again: ";
				r.clear();
				r.str("");
				num = "";
				goto take2;
			}
			if (row < 1 || col < 1 || row > size || col > size){
				cout << "Specified location is outside of the matrix. Please try again: ";
				r.clear();
				r.str("");
				num = "";
				goto take2;
			}
			cout << "Now enter the new value: ";
			dumbuserhere:
			r.clear();
			r.str("");
			num = "";
			getline(cin, num);
			r << num;
			if (!(r >> val)){
				cout << "Invalid input. Please enter the new value: ";
				goto dumbuserhere;
			}
			matrix[row-1][col-1] = val;
		}
	} while (yn == "n" || yn == "N");

	long ans;
	if (size == 1)
		ans = matrix[0][0];							//Trivial answer for size 1 matrix
	else
		ans = findDet(matrix, size);				//Calculate determinant using recursion

	cout << "The determinant is " << ans << ".\n";
	return 0;
}

long findDet(vector<vector<int>> m, int n){
	int cur;
	long total = 0;
													//Initialize a 3D vector, which is used to store the multiple matrices created by the solving process.
	vector<vector<vector<int>>> matrices(n, vector<vector<int>>(n-1 , vector<int>(n-1, m[1][1])));

	if (n == 2)										//Base case
		return m[0][0] * m[1][1] - m[1][0] * m[0][1];
	else {
		for (int c = 0; c < n; c++) {
			cur = m[0][c];							//cur points to each number in the first row sequentially, c is the horizontal index of that number
			for (int i = 1; i < n; i++) {			//i refers to each row
				for (int j = 0, k = 0; j < n; j++, k++) {
													//j and k are both for columns, but both are required since the submatrix has to skip the column associated with c
					if (j == c)						//Skips the column associated with c
						k++;
					if (k == n)						//In case column skipped was the last column	
						break;	
					matrices[c][i-1][j] = m[i][k];	//Adds current number to current submatrix (specified by c)
				}
			}
			if ((c - 1) % 2)
													//If the index number is odd, add the resulting determinant of the current number multiplied by the corresponding submatrix to the total
				total += cur*findDet(matrices[c], n - 1);
			else									//Otherwise, subtract it from the total
				total -= cur*findDet(matrices[c], n - 1);
		}
	}
	return total;
}