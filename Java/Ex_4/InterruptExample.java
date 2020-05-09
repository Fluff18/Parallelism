/*
    Written By: Hari
    Ex: 4.
*/
public class InterruptExample implements Runnable {
    public void run() {
        try {
            Thread.sleep(Long.MAX_VALUE);
        } catch (InterruptedException e) {
            System.out.println("["+Thread.currentThread().getName()+"] Interrupted by exception!");
        }
        while(!Thread.interrupted()) {
        }
        System.out.println("["+Thread.currentThread().getName()+"] Interrupted for the second time.");
    }
    public static void main(String[] args) throws InterruptedException {
        Thread myThread = new Thread(new InterruptExample(), "myThread");
        myThread.start();
        
        System.out.println("["+Thread.currentThread().getName()+"] Sleeping in main thread for 2s...");
        Thread.sleep(2000);
        
        System.out.println("["+Thread.currentThread().getName()+"] Interrupting myThread");
        myThread.interrupt();
    }
}