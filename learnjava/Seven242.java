/*************************************************************************
	> File Name: Seven242.java
	> Author: 
	> Mail: 
	> Created Time: 2019年12月06日 星期五 20时35分28秒
 ************************************************************************/

public class Seven242 {
    public static void main(String[] args) {
        try {
            c();
        } catch(NullPointerException ex) {
            ex.printStackTrace();
        }
    }
    static void c() {
        try {
            b();
        } catch(NullPointerException ex) {
            ex.printStackTrace();
            throw ex;
        }
    }
    static void b() {
        a();
    }
    static String a() {
        String text = null;
        return text.toUpperCase();
    }
}
