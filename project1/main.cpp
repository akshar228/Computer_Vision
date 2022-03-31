#include <iostream>
#include<fstream>

using namespace std;

class image{ 
  public:
    int numRows, numCols, minVal, maxVal;
    int * histAry;
    int computeHist(ifstream &inFile,int * histAry){
      int pixel_val;
      while(!inFile.eof()){
        inFile>>pixel_val;
        histAry[pixel_val]++;
      }
      return 0;
    };

    int printHist(int * histAry,fstream &outFile1){
      for(int i = 0;i<maxVal+1; i++){
        outFile1<<i<<"  "<<histAry[i]<<endl;
      }
      return 0;
    };

    int thresholdOp(ifstream& inFile, 
      int thresholdValue,fstream&outFile2){
      inFile>>numRows; 
      inFile>>numCols;
      inFile>>minVal;
      inFile>>maxVal;
      outFile2<<numRows<<" ";
      outFile2<<numCols<<" ";
      outFile2<<"0 ";
      outFile2<<"1 "<<endl;
      int pixel_val;
      for(int i=0;i<numRows;i++){
        for(int j=0;j<numCols;j++){
          inFile>>pixel_val;
          if(pixel_val>=thresholdValue){outFile2<<"1 ";}
          else{outFile2<<"0 ";}
        }
        outFile2<<endl;
      }
      return 0;
    };

    int prettyPrint(fstream& outFile2,fstream& outFile3){
      outFile2>>numRows;
	    outFile2>>numCols;
	    outFile2>>minVal;
	    outFile2>>maxVal;
      int pixel_val;
      for(int i=0; i<numRows;i++){
		    for(int j=0; j<numCols;j++){
			    outFile2>>pixel_val;
			    if (pixel_val>0){outFile3<<"1 ";}
          else {outFile3<<"  ";}
		    }
		    outFile3<<endl;
	    }
      return 0;
    }; 
};

 int main(int argc, char** argv){
  image img;  
  ifstream inFile;
  inFile.open(argv[1]);
  fstream outFile1;
  outFile1.open(argv [2],fstream::out);
  fstream outFile2;
  outFile2.open(argv [3],fstream::out);
  fstream outFile3;
  outFile3.open(argv [4],fstream::out);
  int thresholdValue = stoi(argv [5]);
  inFile>>img.numRows; 
  inFile>>img.numCols;
  inFile>>img.minVal;
  inFile>>img.maxVal;
  outFile1<<img.numRows<<" ";
  outFile1<<img.numCols<<" ";
  outFile1<<img.minVal<<" ";
  outFile1<<img.maxVal<<" "<<endl;
  img.histAry = new int[img.maxVal+1];
  for(int i=0; i<img.maxVal+1; i++){
    img.histAry[i] = 0;
  }
  img.computeHist(inFile, img.histAry);
  img.printHist(img.histAry, outFile1);
  inFile.close();
  inFile.open(argv[1]); 
  img.thresholdOp(inFile,thresholdValue, outFile2);
  outFile2.close();
  outFile2.open(argv[3]);
  img.prettyPrint(outFile2, outFile3); 
  inFile.close();
  outFile1.close();
  outFile2.close();
  outFile3.close();
  return 0;
}
