/*
 * Written By: Hari Om
 * Ex: 2.
 */
public class MyThread extends Thread {
    public MyThread(String name) {
        super(name);
    }

    @Override
    public void run() {
        System.out.println("Executing thread " + Thread.currentThread().getName());
    }
    
    public static void main(String[] args) throws InterruptedException {
            MyThread myThread = new MyThread("my thread");
            myThread.start();
    }
}
