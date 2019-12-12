/*************************************************************************
	> File Name: Seven6.java
	> Author: 
	> Mail: 
	> Created Time: 2019年12月08日 星期日 21时16分27秒
 ************************************************************************/

public class Seven6 {
    class Resource {
        void deservice() throws IOException {
            System.out.println("Resource");    
        }
    }
    class Some extends Resource {
        @Override
        void deservice() throws Throwable {
            System.out.println("Some");
        }
    }
    public static void main(String[] args) {
        System.out.println("1111");
    }
}
