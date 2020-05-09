import threading
import queue
import random
class multiple:
    def __init__(self):
        self.queue = queue.Queue()
        self.lock = threading.Lock()

    def get(self):
        if (not self.empty()):
            self.lock.acquire()
            i= self.queue.get()
            print(threading.currentThread().getName(),"Get",i)
            self.lock.release()
            return i
        return None

    def add(self,item):
        self.lock.acquire()
        i = self.queue.put(item)
        print(threading.currentThread().getName(),"Add",item)
        self.lock.release()
        return i

    def empty(self):
        self.lock.acquire()
        i = self.queue.empty()
        self.lock.release()
        return i
    
mq = multiple()
l = []
for i in range(1,10):
    if (i %2):
        l.append(threading.Thread(None, mq.add,i,(random.randint(1,100),)))
    else:
        l.append(threading.Thread(None,mq.get,i))

for i in l:
    i.start()

for i in l:
    i.join()