import java.io.*;
import java.util.*;

class Main {
	public static int numRows, numCols, minVal, maxVal;
	public static int newmin = 99999, newmax = 0;
	public static int[][] zeroFramedAry, skeletonAry;
	static void setZero(int[][] Ary){
        for(int i = 0 ; i < numRows + 2 ; i++){
            for(int j = 0 ; j < numCols + 2 ; j++){
                Ary[i][j] = 0;  
            }
        }
	}
	static void loadImage (Scanner file, int[][] Ary){
        for(int i = 1 ; i < numRows + 1 ; i++){
            for(int j = 1 ; j < numCols + 1 ; j++){
                Ary[i][j] = file.nextInt();
            }
        } 
	}
	static void prettyprint(int[][] Ary, PrintWriter file){
        for(int i = 1 ; i < numRows + 1 ; i++){
            for(int j = 1 ; j < numCols + 1 ; j++){
                if(Ary[i][j] == 0){
					file.print("  ");
				}
				else{
					if(Ary[i][j] > 9){
						file.print(Ary[i][j]);
					}
					else{
						file.print(Ary[i][j] + " ");
					}
				}
            }
            file.println();
        }
	}
	static void compute8Distance(int[][] Ary, PrintWriter file){
		fistPass_8Distance(Ary);
		file.println("-----1st pass distance transform-----");
		prettyprint(Ary, file);
		secondPass8Distance(Ary); 
		file.println("\n"+"-----2nd pass distance transform-----");
		prettyprint(Ary, file);
	}
	static void fistPass_8Distance(int[][] Ary){
		for(int i = 1 ; i < numRows + 1 ; i++){
            for(int j = 1 ; j < numCols + 1 ; j++){
				if(Ary[i][j] > 0){
					int a = Ary[i-1][j-1], b = Ary[i-1][j], c = Ary[i-1][j+1], d = Ary[i][j-1];
					Ary[i][j] = Math.min(Math.min(a, b), Math.min(c, d)) + 1;
				}
			}
		}
	}
	static void secondPass8Distance(int[][] Ary){
		for(int i = numRows ; i > 0 ; i--){
            for(int j = numCols ; j > 0 ; j--){
				if(Ary[i][j] > 0){
					int e = Ary[i][j+1], f = Ary[i+1][j-1], g = Ary[i+1][j], h = Ary[i+1][j+1], x = Ary[i][j]; 
					Ary[i][j] = Math.min(x, 1 + Math.min(Math.min(e, f), Math.min(g, h)));
				}
				if(newmin > Ary[i][j]){
                    newmin = Ary[i][j];
                }
                if(newmax < Ary[i][j]){
                    newmax = Ary[i][j];
                }
			}
		}
	}
	static void skeletonExtraction(int[][] Ary, int[][] skAry, PrintWriter skfile, PrintWriter file){
		computeLocalMaxima(Ary, skAry);
		file.println("\n"+"-----Local maxima-----");
		prettyprint(skAry, file);
		extractLocalMaxima(skAry, skfile);
		skfile.close();
	}
	static void computeLocalMaxima(int[][] Ary, int[][] skAry){
		for(int i = 1 ; i < numRows + 1 ; i++){
            for(int j = 1 ; j < numCols + 1 ; j++){
				if(Ary[i][j] > 0 && isLocalMaxima(Ary, i, j) != 0){ 	
					skAry[i][j] = Ary[i][j];				
				}
			 	else{
					 skAry[i][j] = 0;
				}
			}
		}
	}
	static int isLocalMaxima(int[][] Ary, int i, int j){
		int a = Ary[i-1][j-1], b = Ary[i-1][j], c = Ary[i-1][j+1], d = Ary[i][j-1];
		int e = Ary[i][j+1], f = Ary[i+1][j-1], g = Ary[i+1][j], h = Ary[i+1][j+1];
		int x = Ary[i][j];
		if(x >= a && x >= b && x >= c && x >= d && x >= e && x >= f && x >= g && x >= h){
			return 1;
		}
		else{
			return 0;
		}
	}
	static void extractLocalMaxima(int[][] skAry, PrintWriter skfile){
		skfile.println(numRows + " " + numCols + " " +  newmin + " " + newmax);
		for(int i = 1 ; i < numRows + 1 ; i++){
            for(int j = 1 ; j < numCols + 1 ; j++){
				if(skAry[i][j] > 0){
					if(i < 10 ){
						skfile.println("0"+ i + " " + j + " " + skAry[i][j]); 
					}
					else if(j < 10){
						skfile.println(i + " " + "0"+ j + " " + skAry[i][j]);
					}
					else{
						skfile.println(i + " " + j + " " + skAry[i][j]);
					}
				}
			}
		}
	}
	static void skeletonExpansion(int[][] Ary, Scanner skfile, PrintWriter file){
		setZero(Ary);
		load(skfile, Ary);
		firstPassExpension(Ary);
		file.println("-----1st pass Expansion-----");
		prettyprint(Ary, file);
		secondPassExpension(Ary);
		file.println("\n"+"-----2nd pass Expansion-----");
		prettyprint(Ary, file);
	}
	static void load(Scanner file, int[][] Ary){
		file.nextLine();
		while(file.hasNextInt()){ 
			Ary[file.nextInt()][file.nextInt()] = file.nextInt();
		}
	}
	static void firstPassExpension(int[][] Ary){
		for(int i = 1 ; i < numRows + 1 ; i++){
            for(int j = 1 ; j < numCols + 1 ; j++){
				if(Ary[i][j] == 0){
					int x = max_neighbors(i, j, Ary) - 1;
					if(Ary[i][j] < x){
						Ary[i][j] = x;
					}
				}
			}
		}
	}
	static void secondPassExpension(int[][] Ary){
		for(int i = numRows ; i > 0 ; i--){
            for(int j = numCols ; j > 0 ; j--){
				int x = max_neighbors(i, j, Ary);
				if(Ary[i][j] < x){
					Ary[i][j] = x - 1;	
				}
			}
		}	
	}
	static int max_neighbors(int i, int j, int[][] Ary){
		int max = 0;
    	for(int x = i - 1 ; x < i + 2 ; x++){
      		for(int y = j - 1 ; y < j + 2 ; y++){
				if (Ary[x][y] > max) {
                    max = Ary[x][y];
				}    
     	 	}
		}
		return max;
	}
	static void ary2File(int[][] Ary, PrintWriter file){
		file.println(numRows+" "+numCols+" "+minVal+" "+maxVal);
		for(int i = 1 ; i < numRows + 1 ; i++){
            for(int j = 1 ; j < numCols + 1 ; j++){
				if(Ary[i][j] >=1 ){
					file.print(1 + " ");
				}
				else{
					file.print(0 + " ");
				}
            }
            file.println();
        }
    }
	public static void main(String[] args) throws IOException {
		Scanner inFile = new Scanner(new FileInputStream(args[0]));
        PrintWriter outFile1 = new PrintWriter(new FileOutputStream(args[1]));
		PrintWriter outFile2 = new PrintWriter(new FileOutputStream(args[2]));
		numRows = inFile.nextInt();
        numCols = inFile.nextInt();
        minVal = inFile.nextInt();
		maxVal = inFile.nextInt();
		zeroFramedAry = new int[numRows + 2][numCols + 2];
		skeletonAry = new int[numRows + 2][numCols + 2];
		String skeletonFileName = args[0];
		PrintWriter skeletonFile = new PrintWriter(skeletonFileName.substring(0, skeletonFileName.indexOf(".")) + "_skeleton"+ ".txt");
		String decompressedFileName = args[0];
		PrintWriter decompressFile  = new PrintWriter(decompressedFileName.substring(0, decompressedFileName.indexOf(".")) +"_decompressed"+ ".txt");
		setZero(zeroFramedAry);
		setZero(skeletonAry);
		loadImage(inFile, zeroFramedAry);
		compute8Distance(zeroFramedAry, outFile1);
		skeletonExtraction(zeroFramedAry, skeletonAry, skeletonFile, outFile1);
		File skefile = new File(skeletonFileName.substring(0, skeletonFileName.indexOf(".")) + "_skeleton"+ ".txt");
		Scanner skeletonFile_2 = new Scanner(skefile);
		skeletonExpansion(zeroFramedAry, skeletonFile_2, outFile2);
		ary2File(zeroFramedAry, decompressFile);
		inFile.close();
        outFile1.close();
		outFile2.close();
		skeletonFile_2.close();
		decompressFile.close();	
	}
}