#include <iostream>
#include <random>
#include <vector>
#include <string.h>
#include <sstream>
#include <fstream>
#include <chrono>


using namespace std;

struct InputData {
    int bufferSize;
    int matrixWidth, matrixHeight;
    vector<vector<string>> matrix;
    int numSequence;
    vector<vector<string>> sequences;
    vector<int> points;
};

struct ProcessNest {
    int maxPoint;
    vector<int> indices;
    vector<string> currentBuffer;
    vector<string> resultBuffer;
    vector<pair<int,int>> currentCoordinates;
    vector<pair<int,int>> resultCoordinates;
};




void printProcessNest(const ProcessNest& processNest) {
    cout << "Max Point: " << processNest.maxPoint << endl;

    cout << "Indices: " << endl;
    for (const auto& index : processNest.indices) {
        cout << index << " ";
    }
    cout << endl;

    cout << "Current Buffer: " << endl;
    for (const auto& buffer : processNest.currentBuffer) {
        cout << buffer << " ";
    }
    cout << endl;

    cout << "Current Coordinates: " << endl;
    for (const auto& coordinate : processNest.currentCoordinates) {
        cout << "(" << coordinate.first << ", " << coordinate.second << ") ";
    }
    cout << endl;

    cout << "Result Buffer: " << endl;
    for (const auto& buffer : processNest.resultBuffer) {
        cout << buffer << " ";
    }
    cout << endl;

    cout << "Result Coordinates: " << endl;
    for (const auto& coordinate : processNest.resultCoordinates) {
        cout << "(" << coordinate.first << ", " << coordinate.second << ") ";
    }
    cout << endl;
}



bool isSubString(vector<string> & stringBesar, vector<string>& stringKecil){
    int sizeKecil = stringKecil.size();
    int sizeBesar = stringBesar.size();

    for(int i = 0; i<=sizeBesar-sizeKecil;i++){
        int j;
        for(j = 0; j<sizeKecil; j++){
            if(stringBesar[i+j] != stringKecil[j]){
                break;
            }
        }
        if(j == sizeKecil){
            return true;
        }
    }
    return false;
}

int checkPointOfBuffer(vector<string> & buffer, vector<vector<string>> & sequences,vector<int> &points){

    int point = 0;
    for(int i = 0;i<sequences.size();i++){
        if(isSubString(buffer,sequences[i])){
            point+=points[i];
        }
    }
    return point;
}



void nestedLoop(int depth, int checker, InputData & inputData, ProcessNest & processNest){
    // rekursi untuk specify berapa nest loopnya
    if(depth>0){
        int i;
        int iterateCount;
        if(checker == -1){
            iterateCount = inputData.matrixHeight;
        }
        else{
            iterateCount = inputData.matrixWidth;
        }
        checker*= -1; 
        for(i=0;i<iterateCount;i++){
            

            int row,col;
            int indicesSize = processNest.indices.size();

            
            if(indicesSize!=0){
                if(indicesSize%2==1){
                    col = processNest.indices[indicesSize-1];
                    row = i;
                }
                else{
                    col = i;
                    row = processNest.indices[indicesSize-1];
                }
            }
            else{
                row = 0;
                col = i;
            }

            // mengecek apakah titik tersebut sudah disinggahi sebelumnya
            bool flagCoordinate = false;
            for(int j = 0;j<processNest.currentCoordinates.size();j++){
                if(processNest.currentCoordinates[j].first== col+1 && processNest.currentCoordinates[j].second== row + 1){
                    flagCoordinate = true;
                    break;
                }
            }
            if(flagCoordinate){
                continue;
            }

            processNest.currentBuffer.push_back(inputData.matrix[row][col]);

            // mengecek apakah titik pertama dan kedua merupakan starting sequence (optimasi)
            if(indicesSize==1){
                bool flag = true;
                for(int j = 0;j<inputData.sequences.size();j++){
                    if((processNest.currentBuffer[0]==inputData.sequences[j][0] && processNest.currentBuffer[1]==inputData.sequences[j][1]) || processNest.currentBuffer[1]==inputData.sequences[j][0]){
                        flag = false;
                        break;
                    }
                }
                if(flag){
                    processNest.currentBuffer.pop_back();
                    continue;
                }
            }

            int currentPoint = 0;
            
            processNest.currentCoordinates.push_back(make_pair(col+1,row+1));
               
            currentPoint = checkPointOfBuffer(processNest.currentBuffer,inputData.sequences,inputData.points);
            
      
            if(currentPoint<processNest.maxPoint){
                // do nothing
            }

            else if(currentPoint>processNest.maxPoint || currentPoint == processNest.maxPoint && (processNest.currentBuffer.size() < processNest.resultBuffer.size())){
                processNest.maxPoint = currentPoint;
                processNest.resultBuffer = processNest.currentBuffer;
                processNest.resultCoordinates = processNest.currentCoordinates;
            }
         

            processNest.indices.push_back(i);
   
            nestedLoop(depth-1,checker,inputData,processNest);
            processNest.indices.pop_back();
            processNest.currentBuffer.pop_back();
            processNest.currentCoordinates.pop_back();
           
        }
    }
    else{   

    }
}

void readFromTxt(InputData & inputData){
    fstream file;
    string line;
    string basepath = "../test/input/";
    string filename;
    cout << "Enter file name: \n";
    cin >> filename;
    file.open(basepath + filename,ios::in);
    while(!file.is_open()){
        cout << "\nFile not found!\nMasukin nama yang bener !!!!\nMake sure file is in the folder test/input\n";
        cout << "Enter file name: \n";
        cin >> filename;
        file.open(basepath + filename,ios::in);
    }
    if(file.is_open()){
        getline(file,line);
        inputData.bufferSize = stoi(line);
        
        getline(file,line);
        istringstream iss;
        iss.str(line);
        iss >> inputData.matrixWidth >> inputData.matrixHeight;
        iss.clear();
        for(int i = 0 ; i < inputData.matrixHeight; i++){
            getline(file,line);
            iss.str(line);
            vector <string> row;
            for(int j = 0 ; j < inputData.matrixWidth; j++){
                string value;
                iss >> value;
                row.push_back(value);
            }
            inputData.matrix.push_back(row);
            iss.clear();
        }

        getline(file,line);
        iss.str(line);
        iss >> inputData.numSequence;
        iss.clear();

        for(int i = 0 ; i < inputData.numSequence ; i++){
            getline(file, line);
            iss.str(line);
            string word;
            vector<string> row;
            while(iss >> word){
                row.push_back(word);
            }
            inputData.sequences.push_back(row);
            iss.clear();

            getline(file,line);
            iss.str(line);
            int val;
            iss >> val;
            inputData.points.push_back(val);
            iss.clear();
        }
    }
   
    file.close();
}




void printInputData(const InputData& inputData) {
    cout << "Buffer Size: " << inputData.bufferSize << endl;
    cout << "Matrix Width: " << inputData.matrixWidth << endl;
    cout << "Matrix Height: " << inputData.matrixHeight << endl;

    cout << "Matrix: " << endl;
    for(int i = 0 ; i<inputData.matrix.size();i++){
        for(int j = 0 ; j<inputData.matrix[i].size();j++){
            cout << inputData.matrix[i][j] << " ";
        }
        cout << endl;
    }

    cout << "Number of Sequences: " << inputData.numSequence << endl;

    cout << "Sequences: " << endl;

    for(int i = 0;i<inputData.sequences.size();i++){
        for(int j = 0;j<inputData.sequences[i].size();j++){
            cout << inputData.sequences[i][j] << " ";
        }
        cout << endl;
        cout << inputData.points[i] << endl;
    }
    
}


vector<string> generateSequence(int len,vector<string> & uniqueToken){
    vector<string> res;
    for(int i = 0; i<len ; i++){
        int RandNum = rand()%uniqueToken.size();
        res.push_back(uniqueToken[RandNum]);
    }
    return res;
}
bool isSeqExist(vector<vector<string>> & sequences, vector<string> & sequence){
    for(int i = 0 ; i < sequences.size();i++){
        if(sequences[i] == sequence){
            return true;
        }
    }
    return false;
}

void generateSequences(int numSequence, int ukuranMaxSeq, vector<vector<string>> & sequences,vector<string> &  uniqueToken){
    for(int i = 0 ;i<numSequence;i++){
        vector <string> temp;
        int randUkuran = rand()%(ukuranMaxSeq)+1;
        temp = generateSequence(randUkuran,uniqueToken);
        while(isSeqExist(sequences,temp)){
            int randUkuran = rand()%(ukuranMaxSeq)+1;
            temp = generateSequence(randUkuran,uniqueToken);
        }

        sequences.push_back(temp);
    }
}

void generateMatrix(int width,int height, vector<vector<string>> & matrix, int numTokenUnik, vector<string> & uniqueToken){

    for(int i = 0; i<height;i++){
        vector <string> row;
        for(int j = 0; j < width;j++){

            int randNum = rand()%(numTokenUnik);
            row.push_back(uniqueToken[randNum]);


        }
        matrix.push_back(row);
    }
}

void generatePoints(vector<int> & points,int len){
    for(int i = 0 ; i < len ; i++){
        int randNum = rand()%(100-(-100)+1) + (-100);
        // random between -100 and 100
        points.push_back(randNum);
    }
}

void generateRandom(InputData & inputData){
    int numTokenUnik;
    int numBuffer;
    int width,height;
    int numSequence;
    int ukuranMaxSeq;
    istringstream iss;
    cout << "masukkan jumlah token unik: \n";
    cin >> numTokenUnik;
    cout << "Masukkan token unik: \n";
    string inp;
    vector<string> uniqueToken;
    for(int i = 0 ; i < numTokenUnik;i++){
        cin >> inp;
        uniqueToken.push_back(inp);
    }

    cout << "Masukkan size buffer: \n";
    cin >> numBuffer;
    inputData.bufferSize = numBuffer;
    cout << "Masukkan width dan height matrix: \n";
    cin >> width >> height;
    
    inputData.matrixWidth = width;
    inputData.matrixHeight = height;

   

    generateMatrix(width,height,inputData.matrix,numTokenUnik,uniqueToken);
    cout << "Masukkan jumlah sequence: \n";
    cin >> numSequence;
    inputData.numSequence = numSequence;
    cout << "Masukkan ukuran maksimal per sequence: \n";
    cin >> ukuranMaxSeq;
    while(ukuranMaxSeq<1){
        cout << "Ukuran tidak valid!\nMasukkan ulang: ";
        cin >> ukuranMaxSeq;
    }
    generateSequences(numSequence,ukuranMaxSeq,inputData.sequences,uniqueToken);
   
    generatePoints(inputData.points,numSequence);
    cout << endl;

}

void writeToTxt(ProcessNest & processNest,double time){
    string basepath = "../test/output/";
    string filename;
    cout << "Enter file name: \n";
    cin >> filename;
    ofstream file(basepath+filename);
    if(file.is_open()){
        file << processNest.maxPoint << endl;
        for(int i = 0;i<processNest.resultBuffer.size();i++){
            file << processNest.resultBuffer[i] << " ";
        }
        file << endl;

        for(int i = 0; i < processNest.resultCoordinates.size();i++){
            file << processNest.resultCoordinates[i].first << ", " << processNest.resultCoordinates[i].second << endl;
        }
        file << endl;
        file << time << " ms" <<endl;
        file.close();
    }
}

void writeToCLI(ProcessNest & processNest, double time){
    cout << processNest.maxPoint << endl;
    
    for(int i = 0 ; i < processNest.resultBuffer.size();i++){
        cout << processNest.resultBuffer[i] << " ";
    }
    cout << endl;
    
    for(int i = 0 ; i < processNest.resultCoordinates.size(); i++){
        cout << (processNest.resultCoordinates[i].first) << ", " << (processNest.resultCoordinates[i].second) << endl;
    }
    cout << endl;
    cout << time << " ms" << endl;
}



int main(){
    cout << "WELCOME DI PERMAIN MERETAS ALA PUNK SIBER (versi CLI) !\n";
    InputData inputData;
    cout << "read dari file (1) atau generate random (2) ???\n";
    string inp;
    cin >> inp;
    while(inp!="1" && inp!="2"){
        cout << "masukkan antara 1 dan 2 !!!!" << endl;
        cin >> inp;
    }
    if(inp=="1"){
        readFromTxt(inputData);
    }
    else if(inp=="2"){
        generateRandom(inputData);
    }
    cout << endl;
    printInputData(inputData);

    
    ProcessNest processNest;
    auto start = chrono::high_resolution_clock::now();


    int depth = inputData.bufferSize;
    int checker = 1;
    processNest.maxPoint = 0;

    nestedLoop(depth,1,inputData,processNest);

    
    auto end = chrono::high_resolution_clock::now();
    double duration = chrono::duration<double>(end - start).count();
    cout << endl << "result : " << endl;
    writeToCLI(processNest,duration);
   
    cout << "Apakah ingin menyimpan solusi? (y/n)\n";
    string check;
    cin >> check;
    if(check == "y" || check =="Y"){
        writeToTxt(processNest,duration);
    }

}