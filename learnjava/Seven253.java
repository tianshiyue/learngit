/*************************************************************************
	> File Name: Seven253.java
	> Author: 
	> Mail: 
	> Created Time: 2019年12月08日 星期日 20时27分18秒
 ************************************************************************/

public class Seven253 {
    public static void main(String[] args) {
        try(Resource res = new Resource()) {
            res.doSome();
        } catch(Exception ex) {
            ex.printStackTrace();
        }
    }
}

// 类要操作接口
class Resource implements AutoCloseable {
    void doSome() {
        System.out.println("doSome");
    }
    @Override
    public void close() throws Exception {
        System.out.println("资源被关闭");
    }
}
