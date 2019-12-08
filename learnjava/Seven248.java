/*************************************************************************
	> File Name: Seven248.java
	> Author: 
	> Mail: 
	> Created Time: 2019年12月06日 星期五 21时38分21秒
 ************************************************************************/


import java.io.*;
import java.util.Scanner;

public class Seven248 {
    public static void main(String[] args) throws FileNotFoundException {
        Scanner scanner = new Scanner(System.in);
        System.out.print("please input the filename:");
        String name = scanner.next();
        readFile(name);
    }
    public static String readFile(String name) throws FileNotFoundException {
        StringBuilder text = new StringBuilder();
        Scanner console = null;
        try {
            console = new Scanner(new FileInputStream(name));
            while(console.hasNext()) {
                text.append(console.nextLine());
                text.append('\n');
            }
        } finally {
            if(console != null) {
                console.close();
            }
        }
        return text.toString();
    }    
}
