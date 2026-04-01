1. I think J0 will stay Q0 for 5ms, be in Q1 for 0ms. J1 will be in Q0 for 10ms  and in Q1 for 3ms. (My predictions were correct!)
2. 
- `python3 ./mlfq.py -n 3 -j 1 -l 0,200,0 -c`
- `python3 ./mlfq.py -n 3 -j 2 -l 0,200,0:100,10,0 -c`
- `python3 ./mlfq.py -n 3 -j 2 -l 0,200,0:100,10,1 -c`
3. The scheduler will behave like a round-robin if it just has 1 queue. `python3 ./mlfq.py -n 1 -j 2 -l 0,200,0:100,10,0 -c`
4. `python3 ./mlfq.py -n 3 -j 2 -l 0,200,9:100,10,0 -c`
5. To get 5% of the CPU you can boost is back Q/0.05, where Q is the quantum. In this case you would need to boost it back 200 times.
6. Using `-I` puts the I/O outputs at the front or the back of the queue depending on how you use it.