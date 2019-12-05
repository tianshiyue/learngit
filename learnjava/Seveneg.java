/*************************************************************************
	> File Name: Seveneg.java
	> Author: 
	> Mail: 
	> Created Time: 2019年12月05日 星期四 21时19分49秒
 ************************************************************************/
import java.util.Scanner;

abstract class Fish {
    protected String name;
    public Fish(String name) {
        this.name = name;
    }
    public String getName() {
        return name;
    }
    public abstract void swim();
}

class Anemonefish extends Fish {
    public Anemonefish (String name) {
        super(name); // 取得父类中的方法
    }
    @Override // 检查重新定义
    public void swim() {
        System.out.printf("小丑鱼%s 游泳%n", name);
    }
}

public class Seveneg {
    public static void main(String[] args) {
         Scanner scanner = new Scanner(System.in);
         String name;
         name = scanner.nextLine();
    Anemonefish anemonefish = new Anemonefish(name);
    }
}
