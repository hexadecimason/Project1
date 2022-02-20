#include <iostream>
using namespace std;

// A CRM matrix represents the relationships between a set of (n) users, as determined by their tweets and retweets of each other.
class CRM {
    private:
        // These are to keep track of how far each array has been filled: for a cost of a few bytes each we can continually call our add methods in the given for() loop until the arrays have been filled, without changing the pointer.
        int rCounterArray; // Tracks along the indecies of rowPos to add the indicies that separate each user in values[]. 'rCounter' variables work together to track both rowPos and corresponding entries in *values.
        int rCounterVals; // Increments with each input to keep track of indecies of values[] that corresponds to each user.
        int cCounter;
        int vCounter;

        static const int EMPTY_ROW = -1; // Default value for an empty row in the CRM, gets stored in rowPos.

		int influenceScore(int user); // Calculates the Influence score for an individual user.
		int activityScore(int user); // Calculates the Activity score for an individual user.
        int* userSort(int* scores); // Takes in a pointer to an array of scores and returns a pointer to another array, this one sorted highest-scoring index to lowest-scoring. This is used to calculate lists for both metrics used in this class: Influence and Activity.
		
    protected:
        int n; // Number of rows in the CRM: this serves as the reference value representing the number of users.
        int m; // Number of columns in the CRM.
        int nonZeros; // Number of non-zeros in the CRM. 

        int* values; // Points to an array containing all non-zero values, in order of appearance in the matrix (read left -> right, top -> bottom).
        int* rowPos; // Points to an array where each index represents a user and the entry is the index in 'values[]' where that user's reweets begin.
        int* colPos; // Contains column numbers of the entries listed in 'values:' there is a 1:1 relationship between each index of colPos and of values.

    public:
        CRM(); // Default contructor
        CRM(int rows, int cols, int NumZeros); // Primary constructor requires dimensions and number of values to store.

        int getNumRows(); // Getter method for 'n'
        void addValue(int value); // Add value to 'values' array.
        void addRow(int row); // Add an index to the rowPos array for a given row: this index added represents the index in *values at which that user's non-zero values begin.
        void addColumn(int col); // Add a column number to the colPos array. 

        void display(); // Prints the three CRM arrays.
        int mostInfluentialUser(); // Identifies the user with the highest Influence score.
        int mostActiveUser(); // Identifies the user with the highest Activity score.

        int* influentialUsers(); // Prints a list of user indecies sorted by Influence score, high to low.
        int* activeUsers(); // Prints a list of user indecies sorted by Activity score, high to low.
        
        ~CRM(); // Default destructor is the only destructor.
};

CRM::CRM(){}

CRM::CRM(int rows, int cols, int numNonZeros){
    n = rows;
    m = cols;
    nonZeros = numNonZeros;
    values = new int[nonZeros];
    rowPos = new int[n];
    colPos = new int[nonZeros];

    rCounterArray = 0;
    cCounter = 0;
    vCounter = 0;
	rCounterVals = 0;
}

int CRM::getNumRows(){
    return n;
}

void CRM::addValue(int val){
    values[vCounter] = val;
    vCounter ++; // Next index to fill.
}

// Adds a new entry to rowPos, but only under certain conditions: new rows are only added if the user has changed, as this array only stores entries for each unique user rather than entries for each value.
void CRM::addRow(int row){

    // This condition means we have skipped adding a row and need to fill an EMPTY_ROW.
    if(row > rCounterArray){ 
        int skippedRows = row - rCounterArray;
        while(skippedRows > 0){
            rowPos[rCounterArray] = EMPTY_ROW;
            rCounterArray ++; // next index to fill.
            skippedRows --;
        }
    }

    // Only executes if input stream switches rows/users.
    if(row != rCounterArray - 1){
        for(int i = 0; i < nonZeros; i++){
            if(values[i] == values[rCounterVals]) {rowPos[rCounterArray] = i;}
        }
        rCounterArray++; // moves to next index of rowPos
    }   

    rCounterVals++;
}

void CRM::addColumn(int col){
    colPos[cCounter] = col;
    cCounter ++; // Next index to fill.
}

// Printed output of the CRM structure and its values.
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
    cout << endl << endl;
}

int CRM::mostInfluentialUser(){
	int currentUser = 0;
	for(int i = 1; i < n; i ++){
		if(influenceScore(i) > influenceScore(currentUser))
			currentUser = i;
	}
	return currentUser;
}

// For the given user, searches colPos to identify indecies in *values that represent people retweeting them (their influence); adds the value to a running sum. 
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

// For a given user, this method identifies the range of indicies in *values that must be summed to find the users activity.
int CRM::activityScore(int user){
    

	int activityScore = 0;
	int firstIndex = rowPos[user];
	int lastIndex = firstIndex;

    // If -1, calculating score is simple.
	if(rowPos[user] == EMPTY_ROW){return activityScore;}

    // executes if "user" is the last entry: simpler case, no need to check next index for missing value.
    if(user == n-1){
        for(int i = rowPos[n-1]; i < nonZeros; i ++)
            activityScore = activityScore + values[i];
        
        return activityScore;
    }

	// A loop is used to anticipate potential -1 entries we must pass over.
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

int* CRM::userSort(int* scores){
// takes an array of scores and returns an array of indecies (representing the users in our CRM), ordered by score.
// (1) Duplicates values to new array (2) Sorts the new array (3) Matches values of sorted array to their index in the unsorted array to identify which user had which score.

    int* userList = new int[n];
    int temp;

    // (1) Duplicate
    int* sorted = new int[n];
    for(int i = 0; i < n; i++){
        sorted[i] = scores[i];
    }

    // (2) Sort
    for(int i = 0; i < n; i ++){
        for(int k = i+1; k < n; k++){
            if(sorted[k] > sorted[i]){
                temp = sorted[i];
                sorted[i] = sorted[k];
                sorted[k] = temp;
            }
        }
    }

    // (3) Match sorted score to index in unsorted array.
    bool alreadyAdded;
    for(int i = 0; i < n; i ++){
        alreadyAdded = false;
        
        // 'i' represents the rank of scores; 'k' represents the user.
        for(int k = 0; k < n; k ++){

            // checks to see if 'k' already appears in the list of users.
            if(i > 0 && userList[i-1] == k){
                alreadyAdded = true;
            }

            // When an equal score is found, its unsorted index (representing user #) is added to the list of users.
            if(sorted[i] == scores[k] && !alreadyAdded){
                userList[i] = k;

                // Equal-score indecies should be in numerical order.
                if(i > 0 && scores[userList[i-1]] == scores[userList[i]] && userList[i-1] > userList[i]){
                    temp = userList[i-1];
                    userList[i-1] = userList[i];
                    userList[i] = temp;
                }
                break;
            }
            alreadyAdded = false;
        }
    }
    return userList;
}

CRM::~CRM(){
    if (values != NULL) delete[] values;
	if (rowPos != NULL) delete[] rowPos;
	if (colPos != NULL) delete[] colPos;

    cout << "CRM Object Destroyed!!" << endl;

    n = m = 0;
    nonZeros = 0;
    rCounterArray = vCounter = cCounter = rCounterVals = 0;
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
        (*A).addValue (value);
        (*A).addRow (row);
        (*A).addColumn (col);
        
    }
    (*A).display ( );

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
	cout << "Users ranked by Influentiality: ";
    for (int i=0; i < (*A).getNumRows ( ); i++) 
	    cout << influentialityVector [i] << " ";
    cout << endl;

    // Rank users by how active they are
    cout << "Users ranked by Activity: ";
    int* activityVector = (*A).activeUsers();
    for (int i=0; i < (*A).getNumRows ( ); i++) 
		cout << activityVector[i] << " ";
    cout << endl;

    // CALL DESTRUCTORS
    delete A;
    delete [] influentialityVector;
    delete [] activityVector;

    return 0;
}

