/*************************************************************************
	> File Name: test.java
	> Author: 
	> Mail: 
	> Created Time: 2019年11月26日 星期二 21时54分38秒
 ************************************************************************/
class other{
    // 在创建对象之后调用构造函数之前，若有想要执行的流程，可以使用{}定义
    {
        System.out.println("对象初始区块");
    }
    other() {
        System.out.println("other()构造函数");
    }
    other(int a) {

        // this()代表了调用另一个构造函数，至于调用哪个构造函数，则视调用this()时给的自变量类型与个数而定
        this();
        System.out.println("other(int a)构造函数");
    }
}


public class test {
    public static void main(String[] args) {
        new other(1);
    }
}
