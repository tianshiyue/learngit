/*************************************************************************
	> File Name: Main.java
	> Author: 
	> Mail: 
	> Created Time: 2019年11月21日 星期四 20时54分36秒
 ************************************************************************/
class cloth {
    int a;
    char str;
    // 构造函数是与类同名的方法
    cloth(int a, char str) {
        a = a;
        str = str;
    }
}
public class Main {
    public static void main(String[] args) {
        cloth sun1 = new cloth(1, 'b');
        cloth sun2 = new cloth(2, 'c');
        System.out.printf("sun1:(%d, %c)", sun1.a, sun1.str);
    }
}
