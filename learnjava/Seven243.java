/*************************************************************************
	> File Name: Seven243.java
	> Author: 
	> Mail: 
	> Created Time: 2019年12月06日 星期五 20时44分45秒
 ************************************************************************/

public class Seven243 {
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
            Throwable t = ex.fillInStackTrace();
            throw (NullPointerException) t;
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
