#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

class Morphology{
    public:
    int numImgRows, numImgCols, imgMin, imgMax, numStructRows, numStructCols, structMin, structMax, rowOrigin, colOrigin;
	int rowFrameSize, colFrameSize ,extraRows , extraCols; 
    int** zeroFramedAry;
    int** morphAry;
    int** structAry;

    void set2DZero(int** Ary){
        for(int i = 0 ; i < numImgRows + extraRows ; i++){
            for(int j = 0 ; j < numImgCols + extraCols ; j++){
                Ary[i][j] = 0;  
            }
        }
    }
    void set2DStructZero(int** Ary){
        for(int i = 0 ; i < numStructRows ; i++){
            for(int j = 0 ; j < numStructCols ; j++){
                Ary[i][j] = 0;  
            }
        }
    }
    void loadImg(ifstream& file, int** Ary){
        for(int i = rowOrigin ; i < numImgRows + rowOrigin  ; i++){
            for(int j = colOrigin ; j < numImgCols + colOrigin ; j++){
                file>>Ary[i][j];
            }
        }
    }
    void prettyPrint(int** Ary, ofstream& file){
        for(int i = rowOrigin ; i < numImgRows + rowOrigin ; i++){
            for(int j = colOrigin ; j < numImgCols + colOrigin ; j++){
                if(Ary[i][j]==0){
                    file<<". ";
                }
                else{file<<Ary[i][j]<<" ";}
            }
            file<<endl;
        }
        file<<"---------------------------------------------------------------------"<<endl;
    }
    void pretty1Print(int** Ary, ofstream& file){
        file<<"structure element is:"<<endl;
        for(int i=0; i<numStructRows  ; i++){
            for(int j=0; j<numStructCols ; j++){
                file<<Ary[i][j]<<" ";
            }
            file<<endl;
        }
        file<<"---------------------------------------------------------------------"<<endl;
    }
    void loadstruct(ifstream& file, int** Ary){
        for(int i = 0; i < numStructRows ; i++){
            for(int j = 0; j < numStructCols ; j++){
                file>>Ary[i][j]; 
            }
        }
    }
    void ComputeDilation(int** Ary){
        for(int i = rowOrigin; i < numImgRows + rowOrigin ; i++){
            for(int j = colOrigin; j < numImgCols + colOrigin ; j++){
                if(Ary[i][j] > 0){
                    dilation(i,j);
                }
            }
        }
    }
    void dilation(int i, int j){
        int roffset = i-rowOrigin, coffset= j-colOrigin;
        for(int r = 0 ; r < numStructRows ; r++){
            for(int c = 0 ; c < numStructCols ; c++){
                if(structAry[r][c]==1){
                    morphAry[r+roffset][c+coffset] = 1;                
                }
            }            
        }
    }
    void ComputeErosion(int** Ary){
        for(int i = rowOrigin; i < numImgRows + rowOrigin ; i++){
            for(int j = colOrigin; j < numImgCols + colOrigin ; j++){
                if(Ary[i][j] > 0){
                    erosion(i,j, Ary);
                }
            }
        }
    }
    void erosion(int i, int j, int** Ary){
        int roffset = i-rowOrigin, coffset= j-colOrigin;
        bool match = true;
        for(int r = 0 ; r < numStructRows ; r++){
            for(int c = 0 ; c < numStructCols ; c++){
                if(structAry[r][c]>0 && Ary[r+roffset][c+coffset]==0){
                    match = false;
                }
            }
        }
        if(match==true){
            morphAry[i][j]=1;
        }
    }
    void ComputeOpening(int** Ary){
        int** tempAry = new int* [numImgRows + extraRows];
        for(int i=0; i<numImgRows + extraRows; i++){
            tempAry[i] = new int[numImgCols + extraCols];
        }
        ComputeErosion(Ary);
        for(int i = rowOrigin; i < numImgRows + rowOrigin ; i++){
            for(int j = colOrigin; j < numImgCols + colOrigin ; j++){
                tempAry[i][j] = morphAry[i][j];
            }
        }
        set2DZero(morphAry);
        ComputeDilation(tempAry);
    }
    void ComputeClosing(int** Ary){
       int** tempAry = new int* [numImgRows + extraRows];
        for(int i=0; i<numImgRows + extraRows; i++){
            tempAry[i] = new int[numImgCols + extraCols];
        }
        ComputeDilation(Ary);
        for(int i = rowOrigin; i < numImgRows + rowOrigin ; i++){
            for(int j = colOrigin; j < numImgCols + colOrigin ; j++){
                tempAry[i][j] = morphAry[i][j];
            }
        }
        set2DZero(morphAry);
        ComputeErosion(tempAry);
    }
    void AryToFile(int** Ary, ofstream& file){
        file<<numImgRows<<" "<<numImgCols<<" "<<imgMin<<" "<<imgMax<<endl;
        for(int i = rowOrigin ; i < numImgRows + rowOrigin ; i++){
            for(int j = colOrigin ; j < numImgCols + colOrigin ; j++){
                file<<Ary[i][j]<<" ";
            }
            file<<endl;
        }
    }
};

int main(int argc, const char * argv[]){
    Morphology mp;
    string inputName1 = argv[1];   
    ifstream imgFile;                   
    imgFile.open( inputName1 ); 

    string inputName2 = argv[2];   
    ifstream structFile;                   
    structFile.open( inputName2 ); 

    string outputName1 = argv[3];
    ofstream dilateOutFile; 
    dilateOutFile.open( outputName1 );
    
    string outputName2 = argv[4];
    ofstream erodeOutFile; 
    erodeOutFile.open( outputName2 );

    string outputName3 = argv[5];
    ofstream closingOutFile; 
    closingOutFile.open( outputName3 );

    string outputName4 = argv[6];
    ofstream openingOutFile; 
    openingOutFile.open( outputName4 );

    string outputName5 = argv[7];
    ofstream prettyPrintFile; 
    prettyPrintFile.open( outputName5 );

    if( imgFile.is_open() && structFile.is_open() ){
       if(dilateOutFile.is_open() && erodeOutFile.is_open() &&
          openingOutFile.is_open() && closingOutFile.is_open() && prettyPrintFile.is_open() ){
              
            imgFile>>mp.numImgRows>>mp.numImgCols>>mp.imgMin>>mp.imgMax;
            structFile>>mp.numStructRows>>mp.numStructCols>>mp.structMin>>mp.structMax;
            structFile>>mp.rowOrigin>>mp.colOrigin;
            
            mp.rowFrameSize = (mp.numStructRows / 2);
	        mp.colFrameSize = (mp.numStructCols / 2);
	        mp.extraRows = (mp.rowFrameSize * 2); 
	        mp.extraCols = (mp.colFrameSize * 2); 

            mp.zeroFramedAry = new int* [mp.numImgRows + mp.extraRows]; 
            mp.morphAry = new int* [mp.numImgRows + mp.extraRows];
            for(int i=0; i<mp.numImgRows + mp.extraRows; i++){
                mp.zeroFramedAry[i] = new int[mp.numImgCols + mp.extraCols];
                mp.morphAry[i] = new int[mp.numImgCols + mp.extraCols];
            }
            mp.structAry = new int * [mp.numStructRows];
            for(int i = 0 ; i < mp.numStructRows ; i++){
                mp.structAry[i] = new int[mp.numStructCols]; 
            }

            mp.set2DZero(mp.zeroFramedAry);
            mp.loadImg(imgFile, mp.zeroFramedAry);
            mp.prettyPrint(mp.zeroFramedAry, prettyPrintFile);
            mp.set2DStructZero(mp.structAry);
            mp.loadstruct(structFile, mp.structAry);
            mp.pretty1Print(mp.structAry, prettyPrintFile);      
            mp.set2DZero(mp.morphAry); 
            mp.ComputeDilation(mp.zeroFramedAry);
            mp.AryToFile(mp.morphAry, dilateOutFile);
            mp.prettyPrint(mp.morphAry, prettyPrintFile);
            mp.set2DZero(mp.morphAry);
            mp.ComputeErosion(mp.zeroFramedAry);
            mp.AryToFile(mp.morphAry, erodeOutFile);
            mp.prettyPrint(mp.morphAry, prettyPrintFile);
            mp.set2DZero(mp.morphAry);
            mp.ComputeOpening(mp.zeroFramedAry);
            mp.AryToFile(mp.morphAry, openingOutFile);
            mp.prettyPrint(mp.morphAry, prettyPrintFile);
            mp.set2DZero(mp.morphAry);
            mp.ComputeClosing(mp.zeroFramedAry);
            mp.AryToFile(mp.morphAry, closingOutFile);
            mp.prettyPrint(mp.morphAry, prettyPrintFile);
            
            imgFile.close();
            structFile.close();
            dilateOutFile.close();
            erodeOutFile.close();
            openingOutFile.close();
            closingOutFile.close();
            prettyPrintFile.close();
       }else{cout<<"Error!! Could NOT create output file"<<endl ;}
    }else{cout<<"Error!! Could NOT open input file"<<endl;}
}