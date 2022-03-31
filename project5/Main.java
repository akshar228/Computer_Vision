import java.io.*;
import java.util.*;

class Main {

    public static int numRows, numCols, minVal, maxVal;
    public static int max = 0;
    public static int min = 999999;
    public static int[][] mirrorFramedAry; 
    public static int[][] maskRobertRightDiag = {{0,1},{-1,0}};
    public static int[][] maskRobertLeftDiag = {{1,0},{0,-1}};
    public static int[][] maskSobelRightDiag = {{2,1,0},{1,0,-1},{0,-1,-2}};
    public static int[][] maskSobelLeftDiag = {{0,1,2},{-1,0,1},{-2,-1,0}}; 
    public static int[][] RobertRightDiag, RobertLeftDiag, SobelRightDiag, SobelLeftDiag, GradiantEdge, edgeSum;
    
    static void set2DZero(int[][] Ary){
        for(int i=0; i<numRows+2 ; i++){
            for(int j=0; j<numCols+2 ; j++){
                Ary[i][j]=0;  
            }
        }
    }
    static void loadImage (int[][] Ary, Scanner file){
        for(int i=1; i<numRows+1 ; i++){
            for(int j=1 ; j<numCols+1 ; j++){
                Ary[i][j] = file.nextInt();
            }
        } 
    }
    static void mirrowFramed(int[][] Ary){
        for(int i=0; i<numRows+2; i++){
            Ary[i][0]=Ary[i][1];
            Ary[i][numCols+1]=Ary[i][numCols];    
        }
        for(int j=0; j<numCols+2; j++){
            Ary[0][j]=Ary[1][j];
            Ary[numRows+1][j]=Ary[numRows][j];
        }
    }
    static int convoluteRobert(int i, int j, int[][] Ary){
        int sum = 0, temp = j;
        for(int r = 0 ; r < 2 ; r++){
            for(int c = 0 ; c < 2 ; c++){
                sum += (Ary[r][c] * mirrorFramedAry[i][j]);
                j++;
            }
            i++;
            j = temp;
        }
        return sum;
    }
    static int convoluteSobel(int i, int j, int[][] Ary){
        int sum=0,temp = j;
        for(int r = 0 ; r < 3 ; r++){
            for(int c = 0 ; c < 3 ; c++){
                sum += (Ary[r][c] * mirrorFramedAry[i-1][j-1]);
                j++;
            }
            j = temp;
            i++;
        }
        return sum;
    } 
    static int computeGradient(int i, int j){
        int sum=0;
        int x = mirrorFramedAry[i][j], r = mirrorFramedAry[i+1][j], c = mirrorFramedAry[i][j+1];
        sum = (int) Math.sqrt(((x-r)*(x-r))+((x-c)*(x-c))); 
        return sum;
    }
    static void addTwoArys(int[][] Ary1,int[][] Ary2,int[][] Ary3){
        for(int i=1; i<numRows+1; i++){
            for(int j=1; j<numCols+1; j++){
                Ary3[i][j] = Ary2[i][j] +  Ary1[i][j];
                if(Ary3[i][j] > max){
                    max = Ary3[i][j];
                }
                if (Ary3[i][j] < min) {
                    min = Ary3[i][j];
                }
            }
        }
    }
    static void imgOut(int[][] Ary, PrintWriter file){
        int newmin = min;
        int newmax = max;
        file.println(numRows+" "+numCols+" "+newmin+" "+newmax);
        for(int i=1; i<numRows+1; i++){
            for(int j=1 ; j<numCols+1 ; j++){
                file.print(Ary[i][j]+" ");
            }
            file.println();
        }
    }
    static void prettyprint(int[][] Ary, PrintWriter file){
        for(int i=1; i<numRows+1; i++){
            for(int j=1 ; j<numCols+1 ; j++){
                file.print(Ary[i][j]+" ");
            }
            file.println();
        }
    }
    public static void main(String[] args) throws IOException {
        Scanner inFile = new Scanner(new FileInputStream(args[0]));
        PrintWriter outFile1 = new PrintWriter(new FileOutputStream(args[1]));
        PrintWriter outFile2 = new PrintWriter(new FileOutputStream(args[2]));
        PrintWriter outFile3 = new PrintWriter(new FileOutputStream(args[3]));
        PrintWriter outFile4 = new PrintWriter(new FileOutputStream(args[4]));
        numRows = inFile.nextInt();
        numCols = inFile.nextInt();
        minVal = inFile.nextInt();
        maxVal = inFile.nextInt();
        mirrorFramedAry = new int[numRows + 2][numCols + 2];
        RobertRightDiag = new int[numRows + 2][numCols + 2];
        RobertLeftDiag = new int[numRows + 2][numCols + 2];
        SobelRightDiag = new int[numRows + 2][numCols + 2];
        SobelLeftDiag = new int[numRows + 2][numCols + 2];
        GradiantEdge = new int[numRows + 2][numCols + 2];
        edgeSum = new int[numRows + 2][numCols + 2];
        loadImage(mirrorFramedAry, inFile);
        mirrowFramed(mirrorFramedAry);
        for(int i=1; i<numRows+1; i++){
            for(int j=1 ; j<numCols+1 ; j++){
                RobertRightDiag[i][j] = Math.abs(convoluteRobert(i, j, maskRobertRightDiag));
	            RobertLeftDiag[i][j] = Math.abs(convoluteRobert(i, j, maskRobertLeftDiag));
		        SobelRightDiag[i][j] = Math.abs(convoluteSobel(i, j, maskSobelRightDiag));
		        SobelLeftDiag[i][j] = Math.abs(convoluteSobel(i, j, maskSobelLeftDiag));
                GradiantEdge[i][j] = computeGradient(i,j);
                if(GradiantEdge[i][j] > max){
                    max = GradiantEdge[i][j];
                }
                if (GradiantEdge[i][j] < min) {
                    min = GradiantEdge[i][j];
                }
            }
        }
        imgOut(GradiantEdge, outFile3);
        addTwoArys(RobertRightDiag, RobertLeftDiag, edgeSum);  
        outFile4.println("-------------------- RobertLeftDiag to pretty print file --------------------"+"\n");
        prettyprint(RobertLeftDiag, outFile4);
        outFile4.println("\n"+"-------------------- RobertRightDiag to pretty print file --------------------"+"\n");
        prettyprint(RobertRightDiag, outFile4);
        imgOut(edgeSum, outFile1);
        set2DZero(edgeSum);
        min = 9999999;
        max = 0;
        addTwoArys(SobelRightDiag, SobelLeftDiag, edgeSum);
        outFile4.println("\n"+"-------------------- SobelLeftDiag to pretty print file --------------------"+"\n");
        prettyprint(SobelLeftDiag, outFile4);
        outFile4.println("\n"+"-------------------- SobelRightDiag to pretty print file --------------------"+"\n");
        prettyprint(SobelRightDiag, outFile4);
        imgOut(edgeSum, outFile2);
        outFile4.println("\n"+"-------------------- GradiantEdge to pretty print file --------------------"+"\n");
        prettyprint(GradiantEdge, outFile4);
        inFile.close();
        outFile1.close();
        outFile2.close();
        outFile3.close();
        outFile4.close();
    }
}