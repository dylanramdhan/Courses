"""
    Assignment #3, due date: 2/25/2025
    Engineer: Dylan Ramdhan
    
    ** This file is the unit testing of Multithreading Queues w/Callbacks
"""


import unittest
import threading
import multiprocessing
import queue
import numpy as np
import time
from main import task_queue, worker, NUM_TASKS, NUM_THREADS  # ensure main.py is accessible
from mp import worker as mp_worker # importing worker from mp.py


class TestQueueProcessing(unittest.TestCase):
    
    def test_queue_initialization(self):
        # testing if queue initializes correctly
        self.assertIsInstance(task_queue, queue.Queue)
        self.assertEqual(task_queue.qsize(), 0)
    
    
    def test_thread_creation(self):
        # testing if threads are created correctly
        threads = []
        for _ in range(NUM_THREADS):
            t = threading.Thread(target=worker)
            t.start()
            threads.append(t)
        
        for t in threads:
            t.join()
        
        self.assertEqual(len(threads), NUM_THREADS)
    
    
    def test_task_processing(self):
        # testing if tasks are processed correctly
        for i in range(NUM_TASKS):
            task_queue.put(i)
            time.sleep(0.05)
        
        time.sleep(1)  # allowing some processing time in case of delays
        self.assertTrue(task_queue.empty())
    
    
    def test_multi_processing_queue(self):
        # testing if muliprocessing queue processes correctly
        mp_queue = multiprocessing.Queue()
        num_workers = 5
        processes = []
        
        for _ in range(num_workers):
            p = multiprocessing.Process(target=mp_worker, args=(mp_queue,))
            p.start()
            processes.append(p)
        
        
        for _ in range(5):  # Simulate 5 tasks
            A = np.random.rand(100, 100)
            B = np.random.rand(100, 100)
            mp_queue.put((A, B))
            time.sleep(0.1)
        
        
        for _ in range(num_workers):
            mp_queue.put(None)
        
        
        for p in processes:
            p.join()
        
        
        self.assertTrue(mp_queue.empty())
    
    
    
if __name__ == "__main__":
    unittest.main()
