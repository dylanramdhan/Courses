"""
    Assignment #3, due date: 2/25/2025
    Engineer: Dylan Ramdhan
    
    ** This file is the unit testing for the implementation of multi-threading version w/queue system
"""


import unittest
import threading
import queue
import time
from main import task_queue, worker, NUM_TASKS, NUM_THREADS # importing main.py


class TestMainQueueProcessing(unittest.TestCase):
    
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
    
    
    
if __name__ == "__main__":
    unittest.main()
