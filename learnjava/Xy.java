/*************************************************************************
	> File Name: XY.java
	> Author: 
	> Mail: 
	> Created Time: 2019年11月24日 星期日 15时39分40秒
 ************************************************************************/

public class XY {
    public static void main(String[] args) {
        int[][] a = {
            {1, 2, 3},
            {4, 5, 6}
        };
        for(int i = 0; i < a.length; i++) {
            for(int j = 0; j < a[i].length; j++) {
                System.out.printf("%d ", a[i][j]);
            }
            System.out.println();
        }
    }
}
