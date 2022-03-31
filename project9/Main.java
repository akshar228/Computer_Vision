import java.io.*;
import java.util.*;

class Main {
	public static int numRows, numCols, minVal, maxVal;
    public static int[][] imgAry;
    static void loadImage(int[][] Ary,Scanner file){
        for(int i = 0 ; i < numRows ; i++){
            for(int j = 0 ; j < numCols ; j++){
                Ary[i][j] = file.nextInt();
            }
        }
    }
    public static void main(String[] args) throws IOException{
        HoughTransform HT = new HoughTransform();
        Scanner inFile = new Scanner(new FileInputStream(args[0]));
        PrintWriter outFile1 = new PrintWriter(new FileOutputStream(args[1]));
		PrintWriter outFile2 = new PrintWriter(new FileOutputStream(args[2]));
	    numRows = inFile.nextInt();
        numCols = inFile.nextInt();
        minVal = inFile.nextInt();
        maxVal = inFile.nextInt();
        int diagonal = (int) Math.sqrt((numRows * numRows) + (numCols * numCols));
        HT.HoughAngle = 180;
        HT.HoughDist = 2 * diagonal;
        imgAry = new int[numRows][numCols];
        HT.HoughAry = new int[HT.HoughDist][HT.HoughAngle];
        for(int i = 0 ; i < HT.HoughDist ; i++){
            for(int j = 0 ; j < HT.HoughAngle ; j++){
                HT.HoughAry[i][j] = 0;  
            }
        } 
        loadImage(imgAry, inFile);
        HT.buildHoughSpace(imgAry, numRows, numCols);
        HT.prettyPrint(HT.HoughAry, outFile1);
        HT.determineMinMax(HT.HoughAry);
        outFile2.println(HT.HoughDist + " " + HT.HoughAngle + " " + HT.HoughMinVal + " " + HT.HoughMaxVal);
        HT.ary2File(HT.HoughAry, outFile2);
        inFile.close();
        outFile1.close();
        outFile2.close();
    }   
}
class HoughTransform{ 
    private static class xyCoord{
        int x;
        int y;
        xyCoord(){
            this.x = x;
            this.y = y;
        }
    }
    public static int angleInDegree; 
	public static double angleInRadians;
	public static int HoughDist;
    public static int HoughAngle;
    public static int HoughMinVal = 99999;
    public static int HoughMaxVal = 0;
    public static int[][] HoughAry; 
    static void buildHoughSpace(int[][] Ary, int row, int col){
        xyCoord point = new xyCoord();
        for(int r = 0 ; r < row ; r++){
            for(int c = 0 ; c < col ; c++){
                if(Ary[r][c] > 0){
                    point.x = c;
                    point.y = r;
                    angleInDegree = 0;
                    while(angleInDegree <= 179){
                        angleInRadians = angleInDegree / 180.00 * Math.PI;
                        double dist = computeDistance(point, angleInRadians);
                        int distInt = (int) dist;
                        HoughAry[distInt][angleInDegree]++;
                        angleInDegree++;
                    }
                }
            }
        }
    }
    static double computeDistance(xyCoord point,double angleRadians){
        double dis = 0.00, offset = HoughDist/2;
        double x = point.x;
        double y = point.y;
        double t = angleRadians - Math.atan(y/x) - 90.00;
        dis = Math.sqrt((x * x)+(y * y)) * Math.cos(t) + offset; 
        return dis;
    }
    public static void determineMinMax(int[][] Ary){
        for(int i = 0 ; i < HoughDist; i++){
            for(int j = 0 ; j < HoughAngle ; j++){
                if(Ary[i][j] > HoughMaxVal){
                    HoughMaxVal = Ary[i][j];
                }
                if (Ary[i][j] < HoughMinVal) {
                    HoughMinVal = Ary[i][j];
                }
            }
        }
    }
    public static void prettyPrint(int[][] Ary, PrintWriter file){
        for(int i = 0 ; i < HoughDist; i++){
            for(int j = 0 ; j < HoughAngle ; j++){
                if(Ary[i][j] == 0){
					file.print("  ");
				}
				else{
					file.print(Ary[i][j] + " ");
				}
            }
            file.println();
        }
    }
    public static void ary2File(int[][] Ary, PrintWriter file){
        for(int i = 0 ; i < HoughDist ; i++){
            for(int j = 0 ; j < HoughAngle ; j++){
				if(Ary[i][j] >=1 ){
					file.print(Ary[i][j] + " ");
				}
				else{
					file.print(0 + " ");
				}
            }
            file.println();
        }
    }
}