#include <iostream>
#include<fstream>

using namespace std;

class ThinningSkeleton{
public:
      int numRows, numCols, minVal, maxVal, changeFlag , cycleCount;
      int** firstAry;
      int** secondAry;
      
      void zeroFrame(int** Ary){
            for(int i = 0 ; i < numRows + 2 ; i++){
                  for(int j = 0 ; j < numCols + 2 ; j++){
                        Ary[i][j] = 0;  
                  }
            }
      }
      void prettyPrint(int** Ary, ofstream& file){
            for(int i = 0 ; i < numRows + 2 ; i++){
                  for(int j = 0 ; j < numCols + 2 ; j++){
                        if(Ary[i][j] > 0){
                              file<<Ary[i][j]<<" "; 
                        }
                        else{
                              file<<"  ";
                        }
                  }
                  file<<endl;
            }
      }
      void loadImage(ifstream& file, int** Ary){
            for(int i = 1 ; i < numRows + 1 ; i++){
                  for(int j = 1 ; j < numCols + 1 ; j++){
                        file>>Ary[i][j];
                  }
            }
      }
      void doThinning(int** firstAry,int** secondAry,int &changeFlag){
            northThinning(firstAry, secondAry, changeFlag);
            copyArys(firstAry, secondAry);
            southThinning(firstAry, secondAry, changeFlag);
            copyArys(firstAry, secondAry);
            westThinning(firstAry, secondAry, changeFlag);
            copyArys(firstAry, secondAry);
            EastThinning(firstAry, secondAry, changeFlag);
            copyArys(firstAry, secondAry);
      }
      void northThinning(int** firstAry, int** secondAry, int &changeFlag){
            for(int i = 1 ; i < numRows + 1 ; i++){
                  for(int j = 1 ; j < numCols + 1 ; j++){
                        secondAry[i][j] = firstAry[i][j];
                        if(firstAry[i][j]  > 0 && firstAry[i-1][j] <= 0){
                              if(check3n4Conditions(firstAry, i, j, "north") == true){
                                    secondAry[i][j]  = 0; 
                                    changeFlag++;
                              }
                        }
                  }
            }
      }
      void southThinning(int** firstAry, int** secondAry, int &changeFlag){
            for(int i = 1 ; i < numRows + 1 ; i++){
                  for(int j = 1 ; j < numCols + 1 ; j++){
                        secondAry[i][j] = firstAry[i][j];
                        if(firstAry[i][j]  > 0 && firstAry[i+1][j] <= 0){
                              if(check3n4Conditions(firstAry, i, j, "south") == true){
                                    secondAry[i][j]  = 0; 
                                    changeFlag++;
                              }
                        }
                  }
            }
      }
      void westThinning(int** firstAry, int** secondAry, int &changeFlag){
            for(int i = 1 ; i < numRows + 1 ; i++){
                  for(int j = 1 ; j < numCols + 1 ; j++){
                        secondAry[i][j] = firstAry[i][j];
                        if(firstAry[i][j]  > 0 && firstAry[i][j-1] <= 0){
                              if(check3n4Conditions(firstAry, i, j, "west") == true){
                                    secondAry[i][j]  = 0; 
                                    changeFlag++;
                              }
                        }
                  }
            }
      }
      void EastThinning(int** firstAry,int** secondAry,int &changeFlag){
            for(int i = 1 ; i < numRows + 1 ; i++){
                  for(int j = 1 ; j < numCols + 1 ; j++){
                        secondAry[i][j] = firstAry[i][j];
                        if(firstAry[i][j]  > 0 && firstAry[i][j+1] <= 0){
                              if(check3n4Conditions(firstAry, i, j, "east") == true){
                                    secondAry[i][j]  = 0; 
                                    changeFlag++;
                              }
                        }
                  }
            }
      }
      void copyArys(int** firstAry,int** secondAry){
            for(int i = 1 ; i < numRows + 1 ; i++){
                  for(int j = 1 ; j < numCols + 1 ; j++){
                        firstAry[i][j] = secondAry[i][j];
                  }
            }
      }
      bool check3n4Conditions(int** firstAry, int i, int j, string whichside){
            int a = firstAry[i-1][j-1], b = firstAry[i-1][j], c = firstAry[i-1][j+1], d = firstAry[i][j-1];
            int e = firstAry[i][j+1], f = firstAry[i+1][j-1], g = firstAry[i+1][j],  h = firstAry[i+1][j+1]; 
            if(whichside == "north" && g == 1 && (a + c + d + e + f + g + h) >= 4){
                  if(e == 1 && c == 1){
                      return true;
                  }
                  if(f == 1 && d == 1){
                      return true;
                  }
                  if(h == 1 && e == 1){
                      return true;
                  }
                  if(d == 1 && a == 1){
                      return true;
                  }
            }
            if(whichside == "south" && b == 1 && (a + b + c + d + e + f + h) >= 4){
                  if(e == 1 && h == 1){
                      return true;
                  }
                  if(f == 1 && d == 1){
                      return true;
                  }
                  if(a == 1 && d == 1){
                      return true;
                  }
                  if(c == 1 && e == 1){
                      return true;
                  }
            }
            if(whichside == "west" && e == 1 && (a + b + c + e + f + g + h) >= 3){
                  if(a==0 && g==0 && h==0){
                        return false;
                  }
                  if((a + b + c + e + f + g + h) == 4 && (b==0 && c==0 && g==0) ){
                        return false;  
                  }
                  if(b==0 && c==0 && g==0 && h==0){
                        return false;
                  }
                  if(b==0 && c==0 && f==0 && h==0){
                        return false;
                  }
                  return true;
            }
            if(whichside == "east" && d == 1 && (a + b + c + d + f + g + h) >= 3){
                  if(b==0 && a==0 && g==0 && f==0){
                        return false;
                  }
                  if((a + b + c + d + f + g + h) == 4 && (b==0 && a==0 && g==0) ){
                        return false;  
                  }
                  if(c==0 && a==0 && g==0 && f==0){
                        return false;
                  }
                  if(b==0 && c==0 && f==0 && g==0){
                        return false;
                  }
                  return true;
            }
            return false;
      }
}; 

int main(int argc, char** argv){
      ThinningSkeleton TS;
      string inputName = argv[1];   
      ifstream inFile;                   
      inFile.open(inputName); 
      string outputName1 = argv[2];
      ofstream outFile1; 
      outFile1.open(outputName1);
      string outputName2 = argv[3];
      ofstream outFile2; 
      outFile2.open(outputName2);

       if(inFile.is_open()){
            if(outFile1.is_open() && outFile2.is_open()){
                  inFile>>TS.numRows>>TS.numCols>>TS.minVal>>TS.maxVal;
                  outFile1<<TS.numRows<<" "<<TS.numCols<<" "<<TS.minVal<<" "<<TS.maxVal<<endl;
                  TS.firstAry = new int* [TS.numRows + 2];
                  TS.secondAry = new int* [TS.numRows + 2];
                  for( int i = 0; i < TS.numRows + 2; i++ ){
                        TS.firstAry[i] = new int[TS.numCols + 2];
                        TS.secondAry[i] = new int[TS.numCols + 2];
                  }
                  TS.zeroFrame(TS.firstAry);
                  TS.zeroFrame(TS.secondAry);
                  TS.loadImage(inFile, TS.firstAry);
                  outFile2<<"----------This print is before thinning----------"<<endl;
                  TS.prettyPrint(TS.firstAry, outFile2);
                  int i = 1;
                  while(TS.changeFlag > 0){
                        TS.changeFlag = 0;
                        TS.doThinning(TS.firstAry, TS.secondAry, TS.changeFlag);
                        outFile2<<"----------This print is after pass: "<<i<<" ----------"<<endl;
                        TS.prettyPrint(TS.firstAry, outFile2);
                        i++;
                  }
                  for(int i = 1 ; i < TS.numRows + 1 ; i++){
                        for(int j = 1 ; j < TS.numCols + 1 ; j++){
                              outFile1<<TS.firstAry[i][j]<<" ";
                        }
                        outFile1<<endl;

                  }
                  inFile.close();
                  outFile1.close();
                  outFile2.close();
            }else{cout<<"Error!! Could NOT create output file"<<endl ;}
      }else{cout<<"Error!! Could NOT open input file"<<endl;}
}