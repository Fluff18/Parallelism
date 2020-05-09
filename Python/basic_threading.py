
import threading 

def print_norm(n): 
	for i in range(n):
		print("RAHUL")

def print_rev(n): 
	for i in range(n):
		print("LUHAR") 

if __name__ == "__main__": 
	# creating thread 
	t1 = threading.Thread(target=print_norm, args=(10,)) 
	t2 = threading.Thread(target=print_rev, args=(10,)) 

	t1.start()  
	t2.start() 
 
	t1.join() 
	t2.join() 


