import java.io.*;
import java.util.*;

class Main {

  public static int numRows, numCols, minVal, maxVal, maxHight, bestThrVal, offset, thrVal;
  public static int[] histAry, GaussAry;
  public static int[][] GaussGraph, gapGraph, histGraph;

  static int loadHist(int histAry[],Scanner inFile){
    while(inFile.hasNext()){
      histAry[inFile.nextInt()] = inFile.nextInt();
    }
    return 0;
  }

  static int getMax(int histAry[]){
    int maxValue = histAry[0];
    for(int i=1 ; i<histAry.length ; i++){
      if(histAry[i] > maxValue){
	      maxValue = histAry[i];
	    }
    }
    return maxValue;
  }

  static int plotHistGraph(int histGraph[][], int histAry[]){
    for(int i=0 ; i<maxVal+1 ; i++){
      histGraph[i][histAry[i]] = 1;
    }
    return 0;
  }

  static int prettyPrint(int histGraph[][], PrintWriter outFile){
    outFile.println();
    for(int i=0; i<maxVal+1; i++){
      for(int j=0; j<maxHight+1; j++){
        if(histGraph[i][j] == 0){
          outFile.print(" ");
        }
        else{
          outFile.print("*");
        }
      }
      outFile.println();
    }
    return 0;
  }

  static int biMeanGauss (int thrVal, PrintWriter outFile2, PrintWriter outFile3){
    double sum1 , sum2, total, minSumDiff;
    int bestThr = thrVal;
    minSumDiff = 999999.0;
    int x = maxVal-offset;
    while(thrVal < x){
      //reset array for next computation 
      set1DZero(GaussAry);
	    set2DZero(GaussGraph);
	    set2DZero(gapGraph);
      sum1 = fitGauss(0, thrVal, GaussAry, GaussGraph);
      sum2 = fitGauss(thrVal, maxVal, GaussAry, GaussGraph); 
      total = sum1 +sum2;
      if(total < minSumDiff){
	      minSumDiff = total;
		    bestThr = thrVal;
      }
      thrVal++;
      prettyPrint(GaussGraph, outFile2);
      plotGaps(histAry, GaussGraph, gapGraph);
	    prettyPrint(gapGraph, outFile3);
    }
    return bestThr;
  }

  static double fitGauss(int leftIndex,int rightIndex,int GaussAry[],int GaussGraph[][]){
    double mean, var, sum, Gval;
	  sum = 0.0;  
    mean = computeMean(leftIndex, rightIndex, maxHight);
	  var= computeVar(leftIndex, rightIndex, mean);
    int index = leftIndex;
    while(index <= rightIndex){
      Gval = modifiedGauss(index, mean, var, maxHight);
      sum += Math.abs(Gval -(double)histAry[index]);
      GaussAry[index] = (int)Gval;
      GaussGraph[index][(int)Gval] =1;
      index++;
    }
    return sum;
  }

  static double computeMean(int leftIndex, int rightIndex, int maxHeight){
    maxHeight = 0; 
    double sum = 0.0; 
    int numPixels = 0;
    int index = leftIndex;
    while (index < rightIndex){
      sum +=(histAry[index] * index);
	    numPixels += histAry[index];
      if(histAry[index] > maxHeight){
	   	  maxHeight = histAry[index];
      }
    index++;
    }
  return (double)sum / (double)numPixels;
  }

  static double computeVar(int leftIndex,int rightIndex,double mean){
    double sum =0.0;
    int numPixels = 0;
    int index = leftIndex;
    while(index < rightIndex){
      sum += (double)histAry[index] * (((double)index-mean) * ((double)index-mean));
      numPixels += histAry[index];
      index++;
    }
    return (double)sum / (double)numPixels;
  }

  static double modifiedGauss(int x,double mean,double var,double maxHeight){
    return (double) (maxHeight * Math.exp( -( ((x - mean)*(x - mean)) / (2 * var))) );
  }
  
  static void bestThrPlot(int bestThrVal){
    set1DZero(GaussAry);
    set2DZero(GaussGraph);
    set2DZero(gapGraph);
    fitGauss(0, bestThrVal, GaussAry, GaussGraph);
    fitGauss(bestThrVal, maxVal, GaussAry, GaussGraph);
    plotGaps(histAry, GaussGraph, gapGraph);
  }

  static void plotGaps(int histAry[],int GaussGraph[][],int gapGraph[][]){
    int index = minVal;
    while(index < maxVal){
      int first = Math.min(histAry[index], GaussAry[index]); 
	    int last = Math.max(histAry[index], GaussAry[index]);
      while(first < last){
        gapGraph[index][first]= 1;
        first++;
      }
      index++;
    }
  }

  static void set1DZero(int ary[]){
    for(int i=0; i<maxVal+1 ; i++){
		  ary[i]=0;
	  }
  }
  static void set2DZero(int ary[][]){
    for(int i=0; i<maxVal+1 ; i++){
      for(int j=0 ; j<maxHight+1 ; j++){
        ary[i][j]=0;
      }
	  } 
  }

  public static void main(String[] args) throws IOException {
	  
    //open infile and outfile.
    Scanner inFile = new Scanner(new FileInputStream(args[0]));
    PrintWriter outFile1 = new PrintWriter(new FileOutputStream(args[1]));
    PrintWriter outFile2 = new PrintWriter(new FileOutputStream(args[2]));
    PrintWriter outFile3 = new PrintWriter(new FileOutputStream(args[3]));
	
    //read numRows, numCols, minVal, maxVal
    numRows = inFile.nextInt();
    numCols = inFile.nextInt();
    minVal = inFile.nextInt();
    maxVal = inFile.nextInt();
	
    //set offset and thrVal
    offset = (maxVal - minVal) / 10 ;
    thrVal = offset;
	
    //set 1D array and initialized to zero 
    histAry = new int[maxVal + 1];
    GaussAry = new int[maxVal + 1];
	  set1DZero(histAry);
    set1DZero(GaussAry);
	
    //load histAry
    loadHist(histAry, inFile); 
	
    //The largest histAry[i] of the given portion of the histogram 
    maxHight = getMax(histAry); 
	
    //set 2D array and initialized to zero
    GaussGraph = new int[maxVal + 1][maxHight+1];
    gapGraph = new int[maxVal + 1][maxHight+1]; 
    histGraph = new int[maxVal + 1][maxHight+1];
    set2DZero(GaussGraph);
    set2DZero(gapGraph);
    set2DZero(histGraph); 
	
    //assign histGraph [i,histAry[i]] to 1
    plotHistGraph(histGraph, histAry);
	
    //A 2-D display of the histogram.
    outFile1.println("A) A 2-D display of the histogram.");
    prettyPrint(histGraph, outFile1);
    bestThrVal = biMeanGauss(thrVal,outFile2,outFile3);
    outFile1.println("B) The selected threshold value = "+ bestThrVal);
    bestThrPlot(bestThrVal);
    outFile1.println("C) A 2-D display of the two best-fitting Gaussians curves overlaying onto the histogram.");
    prettyPrint(GaussGraph, outFile1);
    outFile1.println("D) A 2-D display of the gaps between the two best-fitting Gaussian curves and the histogram.");
    prettyPrint(gapGraph, outFile1);
	
 	//close all file
    inFile.close();
    outFile1.close();
    outFile2.close();
    outFile3.close();
  }
}