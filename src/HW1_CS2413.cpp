#include <iostream>
using namespace std;

class CRM {
    private:
        int rCounter; // these are to keep track of how far each array has been filled.
        int cCounter;
        int vCounter;
		int newRowCount; // allows us to track where the new rows begin; as addRow() gets repeatedly called and cleared from the stack, we need to keep track of which value marks a new row.
        static const int EMPTY_ROW = -1; // default value for an empty row.
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
        void addRow(int row); // add a row number to the rowPos array
        void addColumn(int col); // add a column number to the colPos array

        void display(); // prints the three CRM arrays

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

    rCounter = 0;
    cCounter = 0;
    vCounter = 0;
	newRowCount = 0;
}

void CRM::addValue(int val){
    values[vCounter] = val;
    vCounter ++;
}

void CRM::addRow(int row){

if(row > rCounter){ // we've skipped a row and need to fill an EMPTY ROW.
    int skippedRows = row - rCounter;
    while(skippedRows > 0){
        rowPos[rCounter] = -1;
		rCounter ++;
        skippedRows --;
    }
}

if(row != rCounter - 1){ // checks to see if this is the same as previous row
    for(int i = 0; i < nonZeros; i++){
        if(values[i] == values[newRowCount]){
            rowPos[rCounter] = i;
        }
     }
    rCounter++;
}   

newRowCount++;

}

void CRM::addColumn(int col){
    colPos[cCounter] = col;
    cCounter ++;
}

void CRM::display(){
    // values
    cout << "values:";
    for(int i = 0; i < nonZeros; i++){
        cout << " " << values[i];
    }
    cout << endl;

    // rowPos
    cout << "rowPos:";
    for(int i = 0; i < n; i++){
        cout << " " << rowPos[i];
    }
    cout << endl;

    // colPos
    cout << "colPos:";
    for(int i = 0; i < nonZeros; i++){
        cout << " " << colPos[i];
    }
    cout << endl;
}

int CRM::mostInfluentialUser(){
    return 0;
}

int* CRM::influentialUsers(){
    return 0;
}

CRM::~CRM(){
    if (values != NULL) delete[] values;
	if (rowPos != NULL) delete[] rowPos;
	if (colPos != NULL) delete[] colPos;

    cout << "CRM object succesfully destroyed: memory leak avoided (probably)" << endl;

    // this was in the template, but why? Same reason as deleting arrays?
    n = m = 0;
    nonZeros = 0;
    rCounter = vCounter = cCounter = 0;

}




int main(){

    CRM* A;
    int numRows, numCols, numNonZeros;
    int row, col, value;

    cin >> numRows >> numCols;
    cin >> numNonZeros;

    A = new CRM(numRows, numCols, numNonZeros);

    // with numRows, numCols, and numNonZeros already read we just need each row of users, of which there are numRows.
    for(int i = 0; i < numNonZeros; i ++){
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

