/*************************************************************************
	> File Name: Six9.java
	> Author: 
	> Mail: 
	> Created Time: 2019年12月03日 星期二 21时02分00秒
 ************************************************************************/

class Some {
    Some() {
        this(10);
        System.out.println("some()");
    }
    Some(int x) {
        System.out.println("some(int x)");
    }
}

class Other extends Some {
    Other() {
        super(10);// 调用父类中的方法
        System.out.println("Other()");
    }
    Other(int y) {
        System.out.println("Other(int y)");
    }
}


public class Six9 {
    public static void main(String[] args) {
        //Other other = new Other(10);
        Some some = new Some(); // 当子类中没有调用父类的时，子类默认调用父类中无参数构造函数
    }
}
