import java.util.Random;

class AccountBalance_sync {

    private static int balance = 0;
    public AccountBalance_sync(int bal) {
        balance = bal;
    }
    
    public double getBalance() {
        return balance;
    }
    public void deposit(int value) {
        balance = balance+value;
        System.out.println("After Deposit: " + balance);
    }

    public void withdraw(int value) {
        if(value <= balance)
            balance = balance - value;
        else
            System.out.println("Not enough balance");
        System.out.println("After Withdraw: " + balance);
     }
}

class Depositer_sync extends Thread {
    private AccountBalance_sync acc;
    public Depositer_sync(AccountBalance_sync acc) {
        this.acc = acc;
    }

    public void run() {
        Random rand = new Random();
        for(int i = 0; i < 5; i++)
            synchronized(acc) {
                //acc.deposit(rand.nextInt(1000));
                acc.deposit(100);
                //Signaling that money has been deposited.
                acc.notify();
            }
    }
}

class Withdrawer_sync extends Thread {
    private AccountBalance_sync acc;
    public Withdrawer_sync(AccountBalance_sync acc) {
        this.acc = acc;
    }

    public void run() {
        Random rand = new Random();
        for(int i = 0; i < 5; i++)
           synchronized(acc) {
               while(acc.getBalance() < 100) {
                   //Waiting for depositors to deposit.
                   try {
                       acc.wait();
                   } catch (InterruptedException err) {
                       System.err.println(err.getMessage());
                   }
               }
               // Commented out for testing. We should get final
               // balance as 1000 every time, as we deposit 100 5 times and withdraw
               // 5 times.
               //acc.withdraw(rand.nextInt(1000));
               acc.withdraw(100);
           }
    }
}

public class Balance_sync {
    public static void main(String[] args) {
        int numThreads = 10;
        
        AccountBalance_sync acc = new AccountBalance_sync(1000);
        
        Thread[] mythreads = new Thread[numThreads];

        for(int i = 0; i < numThreads; i++) {
            if(i % 2 == 0)
                mythreads[i] = new Depositer_sync(acc);
            else
                mythreads[i] = new Withdrawer_sync(acc);
        }

        for(int i = 0; i < numThreads; i++)
            mythreads[i].start();

        for(int i = 0; i < numThreads; i++) {
            try {
                mythreads[i].join();
            } catch (InterruptedException err) {
                System.err.println(err.getMessage());
            }
        }
        System.out.println("Closing Balance =" + acc.getBalance());
    }
}