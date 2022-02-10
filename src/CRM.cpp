#include <iostream>
using namespace std;

class CRM {
    protected:
        int n; // number of rows in CRM
        int m; // number of columns in CRM
        int nonZeros; // number of non-zeros in CRM

        int* values; // Points to an array containing all non-zero values, in order
        int* rowPos; // Points to an array where each index represents a user and the entry is the index in 'values[]' where that user's reweets begin.
        int* colPos; // Contains column numbers of the entries listed in 'values:' there is a 1:1 relationship between each index of colPos and of values.

    public:
        CRM();
        CRM(int rows, int cols, int NumZeros);

        int getNumRows(); // getter method for 'n'
        void addValue(int value); // add value to 'values' array
        void addRow(int row);
        void addColumn(int col);

        void display(); // prints the three CRM

        int mostInfluentialUser();
        int mostActiveUser();

        int* influentialUsers();
        int* activeUsers();
        
        ~CRM();



};

CRM::CRM(){

}

CRM::CRM(int rows, int cols, int numNonZeros){
    n = rows;
    m = cols;
    nonZeros = numNonZeros;
    values = new int[nonZeros];
    rowPos = new int[n];
    colPos = new int[nonZeros];
}

int CRM::mostInfluentialUser(){

}

int* CRM::influentialUsers(){

}

CRM::~CRM(){
    if (values != NULL) delete[] values;
	if (rowPos != NULL) delete[] rowPos;
	if (colPos != NULL) delete[] colPos;

    cout << "CRM object succesfully destroyed: memory leak avoided (probably)" << endl;

    // this was in the template, but why? Same reason as deleting arrays?
    n = 0;
    m = 0;
    nonZeros = 0;
}




int main(){

    CRM* A;
    int numRows, numCols, numNonZeros;
    int row, col, value;

    cin >> numRows >> numCols;
    cin >> numNonZeros;

    A = new CRM(numRows, numCols, numNonZeros);

    // with numRows, numCols, and numNonZeros already read we just need each row of users, of which there are numRows.
    for(int i = 0; i < numRows; i ++){
        cin >> row >> col >> value;

        (*A).addValue(value);
        (*A).addRow(row);
        (*A).addColumn(col);
    }

    (*A).display();

    // Find most influential user

    // Find most active user

    // Rank users by their influence

    // Rank users by how active they are

    // CALL DESTRUCTORS
    delete A;

    return 1;
}

