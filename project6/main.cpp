#include <iostream>
#include<fstream>

using namespace std;

class CCLabel{ 
    public:
    int numRows, numCols, minVal, maxVal, newMin = 9999, newMax = 0, newLabel = 0, trueNumCC, numNb = 5, Connectness;
    int* NonZeroNeighborAry;
    int* EQAry;
    int** zeroFramedAry;
    struct  Property{
        public:
        int label, numpixels, upperLftR, upperLftC, lowerRgtR, lowerRgtC;
    };
    Property* CCproperty; 
    void set2DZero(int** Ary){
        for(int i = 0 ; i < numRows + 2 ; i++){
            for(int j = 0 ; j < numCols + 2 ; j++){
                Ary[i][j] = 0;  
            }
        }
    }
    void loadImage(ifstream& file, int** Ary){
        for(int i = 1 ; i < numRows + 1 ; i++){
            for(int j = 1 ; j < numCols + 1 ; j++){
                file>>Ary[i][j];
            }
        }
    }
    int connectPass1(int connectness, int** Ary, int* nonZeroAry){
        int newLabel = 0, nonZeroCount;
        bool diffFlag;
        for(int i = 1 ; i < numRows + 1 ; i++){
            for(int j = 1 ; j < numCols + 1 ; j++){
                if(Ary[i][j] > 0){
                    int minLabel = loadNonZeroPass1(Ary, Connectness, i, j, nonZeroAry, diffFlag, nonZeroCount); 
                    if(minLabel == 0){ 
			            newLabel++;
                        Ary[i][j] = newLabel;
                    }
                    else if(minLabel > 0){ 
                        Ary[i][j] = minLabel;
                        if (diffFlag == true){
                            updateEQ(EQAry, nonZeroAry, minLabel, nonZeroCount);
                        }
                    }
                }
            }
        }
        return newLabel;
    }
    int loadNonZeroPass1(int** Ary, int connnectness, int i, int j, int* nonZeroAry,bool &diffFlag,int &nonZeroCount){
        minus1D(nonZeroAry);
        nonZeroCount = 0;
        if(Ary[i-1][j] > 0){		
            nonZeroAry[nonZeroCount] = Ary[i-1][j];
		    nonZeroCount++;
        }
        if(Ary[i][j-1] > 0){ 
		    nonZeroAry [nonZeroCount] = Ary[i][j-1];
		    nonZeroCount++;
        }
        if(Connectness == 8){  
		    if(Ary[i-1][j-1] > 0){
			    nonZeroAry[nonZeroCount] = Ary[i-1][j-1];
			    nonZeroCount++;
            }
            if(Ary[i-1][j+1] > 0){
			    nonZeroAry[nonZeroCount] = Ary[i-1][j+1];
			    nonZeroCount++;
            }
        }
        if(nonZeroCount <= 0){ 
            return 0;
        }
        int minLabel = nonZeroAry[0];
	    diffFlag = false;
        int index = 1;
        while(index < nonZeroCount){
            if(minLabel != nonZeroAry[index]){
                diffFlag = true;    
            }
            if(minLabel > nonZeroAry[index]){		
                minLabel = nonZeroAry[index];
            }
            index++;
        }
        return minLabel;
    }
    void minus1D(int* Ary){
        for(int i = 0; i < numNb ; i++ ){
            Ary[i] = -1;
        }
    }
    void updateEQ(int* Ary, int* nonZeroAry, int minLabel, int nonZeroCount){
        int index = 0;
        while(index < nonZeroCount ){
            EQAry[nonZeroAry[index]] = minLabel;
            index++;
        }
    }
    void prettyPrint(ofstream& file){
        for(int i = 0 ; i < numRows + 2 ; i++){
            for(int j = 0 ; j < numCols + 2 ; j++){
                if(zeroFramedAry[i][j] > 0){
                   file<<zeroFramedAry[i][j]<<" "; 
                }
                else{
                    file<<"  ";
                }
            }
            file<<endl;
        }
    }
    void printEQAry(int i, ofstream& file){
        int index = 0;
        while(index <= i){
            file<<EQAry[index]<<" ";
            index++;
        }
    }
    void connectPass2(int Connectness, int** Ary, int* nonZeroAry){
        int nonZeroCount;
        bool diffFlag;
        for(int i = numRows ; i > 0 ; i--){
            for(int j = numCols ; j > 0 ; j--){
                if(Ary[i][j] > 0){
                    int minLabel = loadNonZeroPass2(Ary, Connectness, i, j, nonZeroAry, diffFlag, nonZeroCount);
                    if(minLabel != Ary[i][j]){ 
                        Ary[i][j] = minLabel;
                    }
                    if(diffFlag == true){
                        updateEQ(EQAry, nonZeroAry, minLabel, nonZeroCount);
                    }
                }
            }
        }
    }
    int loadNonZeroPass2(int** Ary, int Connectness, int i, int j, int* nonZeroAry, bool &diffFlag, int &nonZeroCount){
        minus1D(nonZeroAry);
        nonZeroCount = 0;
        nonZeroAry[nonZeroCount] = Ary[i][j];
        nonZeroCount++;  
        if(Ary[i+1][j] > 0){
		    nonZeroAry[nonZeroCount] = Ary[i+1][j];
		    nonZeroCount++;
        }
        if(Ary[i][j+1] > 0){ 
		    nonZeroAry[nonZeroCount] = Ary[i][j+1];
		    nonZeroCount++;
        }
        if(Connectness == 8){
		    if(Ary[i+1][j-1] > 0){
			    nonZeroAry[nonZeroCount] = Ary[i+1][j-1];
			    nonZeroCount++;
            }
            if(Ary[i+1][j+1] > 0){
			    nonZeroAry[nonZeroCount] = Ary[i+1][j+1];
			    nonZeroCount++;
            }
        }
        int minLabel = nonZeroAry[0]; 
	    diffFlag = false;
        int index = 1;
        while(index < nonZeroCount){
            if(minLabel != nonZeroAry[index]){
                diffFlag = true;    
            }
            if(minLabel > nonZeroAry[index]){  
		        minLabel = nonZeroAry[index];
            }
            index++;
        }
        return minLabel;		
    }
    int manageEQAry(int* EQAry, int newLabel){
        int index = 1;
        trueNumCC = 0;
        while (index <= newLabel){
            if(index != EQAry[index]){
                EQAry[index]=EQAry[EQAry[index]];
            }
            else{
                trueNumCC++;
                EQAry[index] = trueNumCC;
            }
            index++;
        }
        return trueNumCC;
    }
    void connectPass3(int* EQAry,int** Ary){
        for(int i = 1 ; i < numRows + 1 ; i++){
            for(int j = 1 ; j < numCols + 1 ; j++){
                if(Ary[i][j]>0){
                    Ary[i][j] = EQAry[Ary[i][j]];
                    CCproperty[Ary[i][j]].numpixels++;
					if(CCproperty[Ary[i][j]].upperLftR == 0 || CCproperty[Ary[i][j]].upperLftR > i) 
                        CCproperty[Ary[i][j]].upperLftR = i;
					if(CCproperty[Ary[i][j]].lowerRgtR < i) 
                        CCproperty[Ary[i][j]].lowerRgtR = i;
					if(CCproperty[Ary[i][j]].upperLftC == 0 || CCproperty[Ary[i][j]].upperLftC > j) 
                        CCproperty[Ary[i][j]].upperLftC = j;
					if(CCproperty[Ary[i][j]].lowerRgtC < j) 
                        CCproperty[Ary[i][j]].lowerRgtC = j; 
                }
                if(newMin > Ary[i][j]){
                    newMin = Ary[i][j];
                }
                if(newMax < Ary[i][j]){
                    newMax = Ary[i][j];
                }
            }
        }
    }
    void printImg(ofstream& file){
        for(int r = 1; r < numRows + 1; r++){
            for(int c =1; c < numCols + 1; c++){
                file<<zeroFramedAry[r][c]<<" ";
            }
            file<<endl;
        }
    }
    void printCCproperty(ofstream& file){
        file<<numRows<<" "<<numCols<<" "<<newMin<<" "<<newMax<<endl;
		file<<trueNumCC<<endl;
        file<<"-- -- -- -- -- -- -- --"<<endl;
		int index = 1;
		while(index < trueNumCC + 1){
            file<<index<<endl;
			file<<CCproperty[index].numpixels<<endl;
			file<<CCproperty[index].upperLftR<<" "<<CCproperty[index].upperLftC<<endl;
			file<<CCproperty[index].lowerRgtR<<" "<<CCproperty[index].lowerRgtC<<endl;
			file<<"-- -- -- -- -- -- -- --"<<endl;
			index++;
		}
    }
    void drawBoxes(int** Ary,Property* CCproperty){
        int index =1;
        while(index <= trueNumCC){
            int minRow = CCproperty[index].upperLftR;
		    int maxRow = CCproperty[index].lowerRgtR;
		    int minCol = CCproperty[index].upperLftC;
		    int maxCol = CCproperty[index].lowerRgtC;
            int label = CCproperty[index].label;
				for (int i = minRow; i < maxRow + 1; i++) {
					Ary[i][maxCol] = index;
                    Ary[i][minCol] = index;
				}
				for (int j = minCol; j < maxCol + 1; j++) {
					Ary[minRow][j] = index;
					Ary[maxRow][j] = index;
				}
            index++;
        }
    }
};
int main(int argc, char** argv){
    CCLabel l;
    string inputName = argv[1];   
    ifstream inFile;                   
    inFile.open( inputName ); 

    string outputName1 = argv[3];
    ofstream prettyPrintFile; 
    prettyPrintFile.open( outputName1 );
    
    string outputName2 = argv[4];
    ofstream labelFile; 
    labelFile.open( outputName2 );

    string outputName3 = argv[5];
    ofstream propertyFile; 
    propertyFile.open( outputName3 );

    if(inFile.is_open()){
       if(prettyPrintFile.is_open() && labelFile.is_open() && propertyFile.is_open() ){ 
            inFile>>l.numRows>>l.numCols>>l.minVal>>l.maxVal;
            l.zeroFramedAry = new int* [l.numRows + 2];
            for( int i = 0; i < l.numRows + 2; i++ ){
                l.zeroFramedAry[i] = new int[l.numCols + 2];
            }
            l.set2DZero(l.zeroFramedAry);
            l.loadImage(inFile, l.zeroFramedAry);
            l.Connectness = stoi(argv[2]);
            l.NonZeroNeighborAry = new int[l.numNb];
            l.minus1D(l.NonZeroNeighborAry);
            l.EQAry = new int[(l.numRows * l.numCols) / 2];
            for(int i = 0; i< ((l.numRows * l.numCols) / 2) + 1; i++){
                l.EQAry[i] = i; 
            }
            l.newLabel = l.connectPass1(l.Connectness, l.zeroFramedAry,l.NonZeroNeighborAry); 
            l.prettyPrint(prettyPrintFile);
            prettyPrintFile<<"EQAry up to newLable after pass 1: "<<endl;
            l.printEQAry(l.newLabel, prettyPrintFile);
            l.connectPass2(l.Connectness, l.zeroFramedAry, l.NonZeroNeighborAry);
            l.prettyPrint(prettyPrintFile);
            prettyPrintFile<<"EQAry up to newLable after pass 2: "<<endl;
	        l.printEQAry(l.newLabel, prettyPrintFile);
            l.trueNumCC = l.manageEQAry(l.EQAry, l.newLabel);
            l.CCproperty = new CCLabel::Property[l.trueNumCC + 1];
	        for (int i = 0; i < l.trueNumCC + 1; i++) {
		        l.CCproperty[i].label = 0;
		        l.CCproperty[i].lowerRgtR = 0;
		        l.CCproperty[i].lowerRgtC = 0;
		        l.CCproperty[i].numpixels = 0;
		        l.CCproperty[i].upperLftR = 0;
		        l.CCproperty[i].upperLftC = 0;
	        }
            prettyPrintFile<<endl<<"EQAry up to newLable after manageEQAry: "<<endl;
            l.printEQAry(l.newLabel, prettyPrintFile); 
            l.connectPass3(l.EQAry, l.zeroFramedAry);
            l.prettyPrint(prettyPrintFile);
            prettyPrintFile<<endl<<"EQAry up to newLable after pass3: "<<endl;
            l.printEQAry(l.newLabel, prettyPrintFile);
            labelFile<<l.numRows<<" "<<l.numCols<<" "<<l.newMin<<" "<<l.newMax<<endl;
            l.printImg(labelFile);
            l.printCCproperty(propertyFile);
            l.drawBoxes(l.zeroFramedAry, l.CCproperty);
            l.prettyPrint(prettyPrintFile);  
            inFile.close();
            prettyPrintFile.close();
            labelFile.close();
            propertyFile.close();
        }else{cout<<"Error!! Could NOT create output file"<<endl ;}
    }else{cout<<"Error!! Could NOT open input file"<<endl;}

}