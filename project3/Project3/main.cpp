#include <iostream>
#include<fstream>

using namespace std;

class imageProcessing {
  public:
  int numRows, numCols, minVal, maxVal, maskRows, maskCols, maskMin, maskMax;
  int** mirrorFramedAry;
  int** avgAry;
  int** medianAry; 
  int** GaussAry;
  int** maskAry;
  
  void set2DZero(int** Ary){
    for(int i=0; i<numRows+2 ; i++){
      for(int j=0; j<numCols+2 ; j++){
        Ary[i][j]=0;  
      }
    }
  }
  void set2DMaskZero(int** Ary){
    for(int i=0; i<maskRows ; i++){
      for(int j=0; j<maskCols ; j++){
        Ary[i][j]=0;  
      }
    }
  }
  void loadMask(int** Ary, ifstream& mask){
    for(int i=0; i<maskRows ; i++){
      for(int j=0; j<maskCols ; j++){
        mask>>Ary[i][j];  
      }
    }
  }
  void loadImage(int** Ary, ifstream& file){
    for(int i=1; i<numRows+1; i++){
      for(int j=1; j<numCols+1; j++){
        file>>Ary[i][j];
      }
    }
  }
  void mirrorFraming(int** Ary){
    for(int i=0; i<numRows+2; i++){
      Ary[i][0]=Ary[i][1];
      Ary[i][numCols+1]=Ary[i][numCols];    
    }
    for(int j=0; j<numCols+2; j++){
      Ary[0][j]=Ary[1][j];
      Ary[numRows+1][j]=Ary[numRows][j];
    }
  }
  void ComputeAvgImg(int** Ary, int *newMin, int *newMax){
    *newMin = 9999;
    *newMax = 0;
    for(int i=1; i<numRows+1; i++){
      for(int j=1; j<numCols+1; j++){
        avgAry[i][j] = avg3by3(i,j,avgAry);
        if(*newMin > avgAry[i][j]){
          *newMin = avgAry[i][j];
        } 
        if(*newMax < avgAry[i][j]){
          *newMax = avgAry[i][j];
        }
      }
    }
  }
  int avg3by3(int i, int j, int** Ary){
    int val;
    int sum=0;
    for(int x=i-1; x<i+2; x++){
      for(int y=j-1; y<j+2 ; y++){
        sum += mirrorFramedAry[x][y];    
      }
    }
    val = sum/9;
    return val;
  }
  void outputAryToFile(int** Ary, fstream& outfile, int x, int y){
    outfile<<numRows<<" "<<numCols<<" "<<x<<" "<<y<<endl;
    for(int i=1; i<numRows+1; i++){
      for(int j=1; j<numCols+1; j++){
        outfile<<Ary[i][j]<<" ";  
      }
      outfile<<endl;
    }  
  }
  void computeMedianImg(int** medianAry, int *newMin, int *newMax){
    int neighborAry[9]; 
    *newMin = 9999;
    *newMax = 0;
    for(int i=1; i<numRows+1; i++){
      for(int j=1; j<numCols+1; j++){
        loadNeighbors(i, j, neighborAry);
        sort(neighborAry);
        medianAry[i][j] = neighborAry[4];
        if(*newMin > medianAry[i][j]){
          *newMin = medianAry[i][j];
        }
        if(*newMax < medianAry[i][j]){
          *newMax = medianAry[i][j];
        }
      }
    }
  }
  void loadNeighbors(int i, int j, int neighborAry[]){
    int z=0;
    for(int x=i-1; x<i+2; x++){
      for(int y=j-1; y<j+2 ; y++){
      neighborAry[z] =mirrorFramedAry[x][y];    
      z++;
      }
    }
  }
  void sort(int neighborAry[]){
    for(int i = 0; i < 9 ; i++){ 
      for(int j = 0; j < 8; j++){  
        if(neighborAry[j] > neighborAry[j+1]){  
          swap(neighborAry[j], neighborAry[j+1]);
        }
      }
    }
  }
  void computeGaussImg(int** GaussAry, int *newMin, int *newMax){
    *newMin = 9999;
    *newMax = 0;
    for(int i=1; i<numRows+1; i++){
      for(int j=1; j<numCols+1; j++){
        GaussAry[i][j] = convolution(i, j, GaussAry, maskAry);
        if(*newMin > GaussAry[i][j]){
          *newMin = GaussAry[i][j];
        }
        if(*newMax < GaussAry[i][j]){
          *newMax = GaussAry[i][j];
        }
      }
    }
  }
  int convolution(int i, int j, int** GaussAry,int** maskAry){
    int sum=0, val;
    sum=(mirrorFramedAry[i-1][j-1]*maskAry[0][0])+(mirrorFramedAry[i-1][j] * maskAry[0][1])+(mirrorFramedAry[i-1][j+1] * maskAry[0][2])+
        (mirrorFramedAry[i][j-1] * maskAry[1][0])+(mirrorFramedAry[i][j] * maskAry[1][1])+(mirrorFramedAry[i][j+1] * maskAry[1][2])+
        (mirrorFramedAry[i+1][j-1] * maskAry[2][0])+(mirrorFramedAry[i+1][j] * maskAry[2][1])+(mirrorFramedAry[i+1][j+1] * maskAry[2][2]);
    val = sum/22;
    return val;
  }
};

int main(int argc, char** argv){
  imageProcessing img;
  int newMin,newMax;
  ifstream inFile;
  inFile.open(argv[1]);
  ifstream maskFile;
  maskFile.open(argv[2]);
  fstream AvgOutImg;
  AvgOutImg.open(argv[3], fstream::out);
  fstream MedianOutImg;
  MedianOutImg.open(argv[4], fstream::out);
  fstream GaussOutImg;
  GaussOutImg.open(argv[5], fstream::out);
  

  inFile>>img.numRows; 
  inFile>>img.numCols;
  inFile>>img.minVal;
  inFile>>img.maxVal;
  maskFile>>img.maskRows;
  maskFile>>img.maskCols;
  maskFile>>img.maskMin;
  maskFile>>img.maskMax;

  img.mirrorFramedAry = new int* [img.numRows + 2]; 
  for(int i=0; i<img.numRows+2; i++){
    img.mirrorFramedAry[i] = new int[img.numCols+2];
  }
  img.avgAry = new int* [img.numRows + 2];
  for(int i=0; i<img.numRows+2; i++){
    img.avgAry[i] = new int[img.numCols+2];
  }
  img.medianAry = new int* [img.numRows + 2];
  for(int i=0; i<img.numRows+2; i++){
    img.medianAry[i] = new int[img.numCols+2];
  }
  img.GaussAry = new int* [img.numRows + 2];
  for(int i=0; i<img.numRows+2; i++){
    img.GaussAry[i] = new int[img.numCols+2];
  }
  img.maskAry = new int* [img.maskRows];
  for(int i=0; i<img.maskRows; i++){
    img.maskAry[i] = new int[img.maskCols];
  }

  img.set2DZero(img.mirrorFramedAry);
  img.set2DZero(img.avgAry);
  img.set2DZero(img.medianAry);
  img.set2DZero(img.GaussAry);
  img.set2DMaskZero(img.maskAry);

  img.loadMask(img.maskAry, maskFile);
  img.loadImage(img.mirrorFramedAry, inFile);
  img.mirrorFraming(img.mirrorFramedAry);
  img.ComputeAvgImg(img.avgAry, &newMin, &newMax);
  img.outputAryToFile(img.avgAry, AvgOutImg, newMin, newMax);
  img.computeMedianImg(img.medianAry, &newMin, &newMax);
  img.outputAryToFile(img.medianAry, MedianOutImg, newMin, newMax);
  img.computeGaussImg(img.GaussAry, &newMin, &newMax);
  img.outputAryToFile(img.GaussAry, GaussOutImg, newMin, newMax);

  inFile.close();
  maskFile.close();
  AvgOutImg.close();
  MedianOutImg.close();
  GaussOutImg.close();
}