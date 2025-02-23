"""
    Assignment #3, due date: 2/25/2025
    Engineer: Dylan Ramdhan
    
    Focus: Understanding Concurrency - Multi-Threding & Multi-Processing
    
    Tasks:
    --> Develop a queue system where it can process requests of an API of your choosing.  For example, the API that you want to queue is to do a numpy api (e.g., numpy.matmul() )
    --> Make your queue size configurable 
    --> Test it with different call flooding (1 call every x seconds, 100 per x seconds, etc.)
    --> Implement it as:
        - Multi-threads
        - Multi-process
    --> Test it with large matrices and not small ones to stress your queue.
"""

import threading
import queue
import numpy as np
import time

# config queue size
QUEUE_SIZE = 10  
task_queue = queue.Queue(maxsize=QUEUE_SIZE)

def matrix_multiplication():
    # function to perform matrix multiplication
    size = 1000  # large matrix size for stress testing
    A = np.random.rand(size, size)
    B = np.random.rand(size, size)
    return np.matmul(A, B)

def worker():
    # worker function to process tasks from queue
    while True:
        task = task_queue.get()
        if task is None:
            break # stop signal
        result = matrix_multiplication()
        print(f"Task {task} completed")
        task_queue.task_done()


# creating threads
NUM_THREADS = 4  # config number of threads
threads = []
for _ in range(NUM_THREADS):
    t = threading.Thread(target=worker)
    t.start()
    threads.append(t)



# adding tasks to queue
NUM_TASKS = 20  # number of tasks to process
for i in range(NUM_TASKS):
    task_queue.put(i)
    time.sleep(0.1)  # simulating different call rates

task_queue.join() # waiting for all tasks to complete



# stopping threads
for _ in range(NUM_THREADS):
    task_queue.put(None)  # stopping signal

for t in threads:
    t.join()

print("Multi-threading queue processing completed.") # print end message
