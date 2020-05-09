import threading
import random

bal = 0
lock = threading.Lock()

def add(amount):
    global bal
    lock.acquire()
    print("Adding "+str(amount)+" to "+str(bal))
    bal=bal+amount
    print(bal)
    lock.release()

def subtract(amount):
    global bal
    lock.acquire()
    print("Subtracting " + str(amount)+" from "+str(bal))
    if(bal-amount > 0):
        bal = bal - amount
        print(bal)
    else:
        print("Low balance");
    lock.release()


if __name__ == '__main__':
    l = []
    for i in range(1,10):
        if(i%2):
            l.append(threading.Thread(target = add, args=(random.randint(1,500),)))
        else:
            l.append(threading.Thread(target = subtract, args=(random.randint(1,500),)))

    for i in l:
        i.start()

    for i in l:
        i.join()
