#include <iostream>
using namespace std;

class CRM {
    private:
        int rCounter; // these are to keep track of how far each array has been filled.
        int cCounter;
        int vCounter;
		int newRowCount; // allows us to track where the new rows begin; as addRow() gets repeatedly called and cleared from the stack, we need to keep track of which value marks a new row.
        static const int EMPTY_ROW = -1; // default value for an empty row.

		int influenceScore(int user);
		int activityScore(int user);
        int* userSort(int* scores);
		
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

int CRM::getNumRows(){
    return n;
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
        if(values[i] == values[newRowCount]) {rowPos[rCounter] = i;}
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
	// most influential: the user who has been retweeted the most.
	int currentUser = 0;
	for(int i = 1; i < n; i ++){
		if(influenceScore(i) > influenceScore(currentUser))
			currentUser = i;
	}

	return currentUser;
}

int CRM::influenceScore(int user){
	int userScore = 0;
	for(int i = 0; i < nonZeros; i++){
		if(colPos[i] == user){
			userScore = userScore + values[i];
		}
	}

	return userScore;
}

int CRM::mostActiveUser(){
	int currentUser = 0;

	for(int i = 0; i < n; i++){

		if(activityScore(i) > activityScore(currentUser))
			currentUser = i;
	}

    return currentUser;
}

int CRM::activityScore(int user){

	int activityScore = 0;
	int firstIndex = rowPos[user];
	int lastIndex = firstIndex;

    // If -1, calculating score is simple.
	if(rowPos[user] == EMPTY_ROW){return activityScore;}

    // executes if "user" is the last entry: simpler case.
    if(user == n-1){
        for(int i = rowPos[n-1]; i < nonZeros; i ++)
            activityScore = activityScore + values[i];
        
        return activityScore;
    }

	// executes for all user IDs other than the last entry: in these cases, a different loop is needed to anticipate potential -1 entries we must pass over.
	for(int i = 1; i < n; i ++){
		if(rowPos[user+i] != EMPTY_ROW){
			lastIndex = rowPos[user+i] - 1;
			break;
		}
	}

    // calculation of actual score.
	for(int i = firstIndex; i <= lastIndex; i ++) {
		activityScore = activityScore + values[i];
	}

	return activityScore;
}

int* CRM::influentialUsers(){
    int* scores = new int[n];
    int userScore;

    for(int i = 0; i < n; i ++){
        scores[i] = influenceScore(i);
    }

    return userSort(scores);
}

int* CRM::activeUsers(){
    int* scores = new int[n];
    int userScore;

    for(int i = 0; i < n; i ++){
        scores[i] = activityScore(i);
    }

    return userSort(scores);
}

// takes an array of scores and returns a list of it's indicies in order based on the score.
int* CRM::userSort(int* scores){
    int* userList = new int[n];

    int user;
    int nextHighest;
    int score;
    bool isDuplicate;

    int largestScore = 0;

    // we will aquire the highest score so that we can reference n-1 to easily access the "next highest."
    for(int i = 0; i < n; i ++){
            if(scores[i] > largestScore){
                largestScore = scores[i];
                userList[0] = i;
        }
    }

    for(int i = 1; i < n; i ++){
        
        nextHighest = scores[userList[i-1]];

        /*
        // mark as duplicate value if duplicated AND not already added.
        for(int k = 0; k < n; k ++){
            if(scores[i] == scores[k] && i != k){
                isDuplicate = true;
            }
            else{isDuplicate = false;}
    
        }
        */
       
        largestScore = 0;
        for(int k = 0; k < n; k ++){

            if(scores[i] == scores[k] && i != k){
                isDuplicate = true;

                for(int p = 0; p <= i; p++){
                    if(k == p) {isDuplicate = false;}
                }
            }
            else{
                isDuplicate = false;
            }

            if((scores[k] > largestScore && scores[k] < nextHighest) || isDuplicate){
                largestScore = scores[k];
                user = k;
            }
        }

        userList[i] = user;
    }

    /*
    for(int i = 0; i < n; i ++){
        cout << "user i: " << i << " | " << "score: " << scores[i] << endl;
    }
    */

    return userList;
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
	int mostInf = (*A).mostInfluentialUser ();
	cout << "Most influential user: " << mostInf << endl;
	cout << endl;

    // Find most active user
	int mostAct = (*A).mostActiveUser();
	cout << "Most active user: " << mostAct << endl;
	cout << endl;

    // Rank users by their influence
    int* influentialityVector = (*A).influentialUsers ();
	cout << "Users ranked by Influentiality: " << endl;
    for (int i=0; i < (*A).getNumRows ( ); i++) 
	    cout << influentialityVector [i] << " ";
    cout << endl << endl;

    // Rank users by how active they are
    cout << "Users ranked by Activity: " << endl;
    int* activityVector = (*A).activeUsers();
    for (int i=0; i < (*A).getNumRows ( ); i++) 
		cout << activityVector[i] << " ";
    cout << endl;

    // CALL DESTRUCTORS
    delete A;
    delete [] influentialityVector;
    delete [] activityVector;


    return 1;
}

