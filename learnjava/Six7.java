/*************************************************************************
	> File Name: Six7.java
	> Author: 
	> Mail: 
	> Created Time: 2019年12月03日 星期二 20时41分43秒
 ************************************************************************/


class Some {
    protected int x;
    Some(int x) {
        this.x = x;
    }
    Some() {  } 
}

class Other extends Some {
    // 子类没有调用父类中的构造方法 所以默认调用父类中无参数的构造方法，所以手动添加无参数的构造方法
    Other(int x) {
        super.x = x;
    }
}
public class Six7 {
    public static void main() {
        Other other = new Other(10);
    }
}
