/*************************************************************************
	> File Name: Some.java
	> Author: 
	> Mail: 
	> Created Time: 2019年12月03日 星期二 19时00分43秒
 ************************************************************************/

abstract class Some {
    abstract void doService();
}

class Other extends Some {
    @Override // 检查重定义
    void doService() {
        System.out.println("other service");
    }
}

public class Qw {
    public static void main( String[] args ) {
        //Other other = new Other();
        //other.doService();
        Some some = new Other();
        some.doService();
    }
}


