/*************************************************************************
	> File Name: Score.java
	> Author: 
	> Mail: 
	> Created Time: 2019年11月24日 星期日 15时29分07秒
 ************************************************************************/

public class Score {
    public static void main(String [] agrs) {
        int[] Scores = {1,2,3,4,5,6};
        for(int i = 0; i < Scores.length; i++) {
            System.out.printf("%d ", Scores[i]);
        }
        System.out.println();
        for(int t : Scores) {
            System.out.printf("%d ", t);
        }
    }
}
