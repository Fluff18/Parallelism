
class AccountBalance {

    private static double balance = 0;
    public AccountBalance(double bal) {
        balance = bal;
    }
    
    public double getBalance() {
        return balance;
    }
    public void deposit(double value) {
        System.out.println("Depositing");
        balance = balance+value;
    }

    public void withdraw(double value) {
        System.out.println("Withdrawing");
        if(value <= balance)
            balance = balance - value;
        else
            System.out.println("Not enough balance");
     }
}

class Depositer extends Thread {
    private AccountBalance_sync acc;
    public Depositer(AccountBalance_sync acc) {
        this.acc = acc;
    }

    public void run() {
        for(int i = 0; i < 5; i++)
            acc.deposit(100);
    }
}

class Withdrawer extends Thread {
    private AccountBalance_sync acc;
    public Withdrawer(AccountBalance_sync acc) {
        this.acc = acc;
    }

    public void run() {
        for(int i = 0; i < 4; i++)
            acc.withdraw(50);
    }
}

public class Balance {
    public static void main(String[] args) {
        int numThreads = 10;
        
        AccountBalance_sync acc = new AccountBalance_sync(10000);
        
        Thread[] mythreads = new Thread[numThreads];

        for(int i = 0; i < numThreads; i++) {
            if(i % 2 == 0)
                mythreads[i] = new Depositer(acc);
            else
                mythreads[i] = new Withdrawer(acc);
        }

        for(int i = 0; i < numThreads; i++)
            mythreads[i].start();

        for(int i = 0; i < numThreads; i++) {
            try {
                mythreads[i].join();
            } catch (InterruptedException err) {
                System.err.println(err.getMessage());
        }
        System.out.println("Closing Balance =" + acc.getBalance());
    }
}}