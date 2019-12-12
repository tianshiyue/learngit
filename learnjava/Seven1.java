/*************************************************************************
	> File Name: Seven1.java
	> Author: 
	> Mail: 
	> Created Time: 2019年12月08日 星期日 20时38分41秒
 ************************************************************************/

public class Seven1 {
    public static void main(String[] args) {
        try {
            int Number = Integer.parseInt(args[0]) ;
            System.out.println(Number++);
        }/* catch(NumberFormatException ex) {
            System.out.println("必须输入数字");
        }*/
        catch(RuntimeException  | NumberFormatException ex) {
            System.out.println("必须输入数字");
        }
    }
}
// 如果命令行没有输入 运行输出0
