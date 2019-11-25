/*************************************************************************
	> File Name: Cash.java
	> Author: 
	> Mail: 
	> Created Time: 2019年11月25日 星期一 19时46分20秒
 ************************************************************************/
import java.util.Scanner;

class CashCard {
    String num;
    int balance;
    int bonus;
    CashCard(String num, int balance, int bonus) {
        this.num = num;
        this.balance = balance;
        this.bonus = bonus;
    }
}


public class Cash {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        CashCard[] Scores = {
            new CashCard("qw", 1, 1),
            new CashCard("sa", 2, 2),
            new CashCard("ll", 3, 3)
        };
        for(CashCard card : Scores) {
            System.out.printf("%s %d %d%n", card.num, card.balance, card.bonus);
        }
        int money = scanner.nextInt();
    }
}

