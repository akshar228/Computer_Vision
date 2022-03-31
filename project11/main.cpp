#include <iostream>
#include<fstream>

using namespace std;
class box{
  public:
  int minR, minC, maxR, maxC;
};
class boxNode{
  public:
  int boxType;
  box BBox;
  boxNode* next;
};

class boxQ{
  public:
  boxNode* Qfront;
  boxNode* QBack;
};
class imagePP{
public:
    int numRows, numCols, minVal, maxVal, thrVal, HPPruns, VPPruns;
    int** imgAry;
    int* HPP;
    int* VPP;
    int* HPPbin;
    int* VPPbin;
    int* HPPMorph;
    int* VPPMorph;
    string direction;
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
    void prettyPrint(ofstream& file){
        for(int i = 0 ; i < numRows + 2 ; i++){
            for(int j = 0 ; j < numCols + 2 ; j++){
                file<<imgAry[i][j]<<" "; 
            }
            file<<endl;
        }
    }
    void computeHPP(int** imAry, int* hAry){
        int sum = 0;
        for(int i = 0 ; i < numRows + 2 ; i++){
            for(int j = 0 ; j < numCols + 2 ; j++){
                if(imAry[i][j] == 1){
                    sum++;   
                }
            }
            if(sum >= thrVal){
                HPPbin[i] = 1; 
            }
            hAry[i] = sum;
            sum = 0;
        } 
    }
    void computeVPP(int** imAry, int* vAry){
        int sum = 0;
        for(int i = 0 ; i < numCols + 2 ; i++){
            for(int j = 0 ; j < numRows + 2 ; j++){
                if(imAry[j][i] == 1){
                    sum++;   
                }
            }
            if(sum >= thrVal){
                VPPbin[i] = 1; 
            }
            vAry[i] = sum;
            sum = 0;
        } 
    }
    void prettyPrinthP(ofstream& file, int* Ary){
        for(int i = 0 ; i < numRows + 2 ; i++){
            file<<Ary[i]<<" ";
        }
    }
    void prettyPrintvP(ofstream& file, int* Ary){
        for(int i = 0 ; i < numCols + 2 ; i++){
            file<<Ary[i]<<" ";
        }
    }
    void morphClosingHPP(int* bin, int* morph){
        int temp[numRows + 2];
        for (int i = 1; i < numRows + 1; i++){
            temp[i] = 0;
        }
        for (int i = 1; i < numRows + 1; i++){
            morph[i] = bin[i];
            temp[i] = bin[i];
        }
        for (int i = 1; i < numRows + 1; i++){
            if(bin[i] > 0){
                morph[i-1] = 1;
                morph[i+1] = 1;
                temp[i-1] = 1;
                temp[i+1] = 1;
            }
        }
        for (int i = 1; i < numRows + 1; i++){
            if(temp[i] > 0){
                if (temp[i-1] == 0 || temp[i+1] == 0){
                    morph[i] = 0;
                }
            }
        }
    }
    void morphClosingVPP(int* bin, int* morph){
        int temp[numCols + 2];
        for (int i = 1; i < numCols + 1; i++){
            temp[i] = 0;
        }
        for (int i = 1; i < numCols + 1; i++){
            morph[i] = bin[i];
            temp[i] = bin[i];
        }
        for (int i = 1; i < numCols + 1; i++){
            if(bin[i] > 0){
                morph[i-1] = 1;
                morph[i+1] = 1;
                temp[i-1] = 1;
                temp[i+1] = 1;
            }
        }
        for (int i = 1; i < numCols + 1; i++){
            if(temp[i] > 0){
                if (temp[i-1] == 0 || temp[i+1] == 0){
                    morph[i] = 0;
                }
            }
        }
    }
    string findDirection(int* HPPMo, int* VPPMo){
        HPPruns = 0;
        VPPruns = 0;
        countruns(HPPruns,VPPruns, HPPMo, VPPMo);
        if(HPPruns >= VPPruns){
            return "Horizontal";
        }
        else if(VPPruns >= HPPruns){
            return "Vertical";
        }
        else return "Can not determine the reading direction." ;
    }
    void countruns(int& hruns, int& vruns, int* HPPMo, int* VPPMo){
        for(int i = 1; i < numRows + 1; i++){
            if(HPPMo[i] == HPPMo[i+1]){ 
                hruns++;
            }
        }
        for (int i = 1; i < numCols + 1; i++){
            if(VPPMo[i] == VPPMo[i+1]){
                vruns++;
            }
        }
    }
};
int main(int argc, char** argv){
    imagePP l;
    string inputName = argv[1];   
    ifstream inFile;                   
    inFile.open( inputName ); 

    string outputName1 = argv[3];
    ofstream outFile1; 
    outFile1.open( outputName1 );

    string outputName2 = argv[4];
    ofstream outFile2; 
    outFile2.open( outputName2 );

    if(inFile.is_open()){
       if(outFile1.is_open() && outFile2.is_open()){ 
            inFile>>l.numRows>>l.numCols>>l.minVal>>l.maxVal;
            l.thrVal = stoi(argv[2]);
            l.imgAry = new int* [l.numRows + 2];
            l.HPP = new int [l.numRows + 2];
            l.VPP = new int [l.numCols + 2];
            l.HPPbin = new int [l.numRows + 2];
            l.VPPbin = new int [l.numCols + 2];
            l.HPPMorph = new int [l.numRows + 2];
            l.VPPMorph = new int [l.numCols + 2];
            for( int i = 0; i < l.numRows + 2; i++ ){
                l.imgAry[i] = new int[l.numCols + 2];
                l.HPP[i] = 0;
                l.HPPbin[i] = 0;
                l.HPPMorph[i] = 0;
            } 
            for(int i = 0; i < l.numCols + 2; i++){
                l.VPP[i] = 0;
                l.VPPbin[i] = 0;
                l.VPPMorph[i] = 0;
            }
            l.set2DZero(l.imgAry);
            l.loadImage(inFile, l.imgAry);
            l.computeVPP(l.imgAry, l.VPP);
            l.computeHPP(l.imgAry, l.HPP);
            outFile2<<"----------- HPP -----------"<<endl;
            l.prettyPrinthP(outFile2, l.HPP);
            outFile2<<endl<<"----------- VPP -----------"<<endl;
            l.prettyPrintvP(outFile2, l.VPP);
            outFile2<<endl<<"----------- HPPbin -----------"<<endl;
            l.prettyPrinthP(outFile2, l.HPPbin);
            outFile2<<endl<<"----------- VPPbin -----------"<<endl;
            l.prettyPrintvP(outFile2, l.VPPbin);
            l.morphClosingHPP(l.HPPbin, l.HPPMorph);
            outFile2<<endl<<"----------- HPPMorph -----------"<<endl;
            l.prettyPrinthP(outFile2, l.HPPMorph);
            l.morphClosingVPP(l.VPPbin, l.VPPMorph);
            outFile2<<endl<<"----------- VPPMorph -----------"<<endl;
            l.prettyPrintvP(outFile2, l.VPPMorph);
            l.direction = l.findDirection(l.HPPMorph,l.VPPMorph);
            outFile2<<endl<<"The reading direction is: "<<l.direction<<endl;
            inFile.close();
            outFile1.close();
            outFile2.close();
        }else{cout<<"Error!! Could NOT create output file"<<endl ;}
    }else{cout<<"Error!! Could NOT open input file"<<endl;}
}