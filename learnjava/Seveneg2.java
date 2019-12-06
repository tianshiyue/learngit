/*************************************************************************
	> File Name: Seveneg2.java
	> Author: 
	> Mail: 
	> Created Time: 2019年12月05日 星期四 22时15分40秒
 ************************************************************************/

import java.util.Scanner;

// 使用(try)捕捉(catch)代表错误的对象
public class Seveneg2 {
    public static void main(String[] args) {
        try {
            int i = 0;
            double sum = 0;
            Scanner scanner = new Scanner(System.in);
            while(true) {
                int num = scanner.nextInt();
                if(num == 0) {
                break;
                }
                sum += num;
                i++;
            }
            System.out.printf("%.2f%n", sum/i);
        } catch(InputMismatchException ) {
            System.out.println("必须输入整数");
        }
    }
}
