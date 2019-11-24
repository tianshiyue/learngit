/*************************************************************************
	> File Name: DeepCopy.java
	> Author: 
	> Mail: 
	> Created Time: 2019年11月24日 星期日 16时50分24秒
 ************************************************************************/

class clothes {
    String color;
    char size;
    clothes(String color, char size) {
        this.color = color;
        this.size = size;
    }
}
public class DeepCopy {
    public static void main(String[] args) {
        clothes[] c1 = {new clothes("red", 'L'), new clothes("blue", 'M')};
        clothes[] c2 = new clothes[c1.length];
        for (int i = 0; i < c1.length; i++) {
            clothes c = new clothes(c1[i].color, c1[i].size);
            c2[i] = c;
        }
        c1[0].color = "yellow";
        System.out.printf("%s%n", c2[0].color);
    }
}
