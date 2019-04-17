# Simple Find Shortest Route

#### Author: Jacob Wilkins

#### Language: C++

#### Description
	The program computes a route between the source and destination city, and prints out both length of the route and a list of all the cities on the route.

#### Structure (Refer to comments): 
	1. Node and successor array are defined as structs
	2. Expanding fringe function is defined as SuccFunc
	3. 3 functions are defined and work together to sort the fringe queue: MinIndex, InsertToEnd, and SortFringe
	4. 2 additional functions are defined to sort the queue with heuristics: MinIndexHeur, SortFringeHeur
	5. In main,
		1. First, I declare most of the needed variables and initialize the root node
		2. Next, I counted the number of edges in the graph
		3. Using the number of edges, I created the successor array
		4. I then performed a graph search using a children vector, closed vector, and fringe queue. The solution was saved into a node pointer called success. After each loop, the fringe queue is sorted (Uses heuristics sort if heuristic text file is provided)
		5. Next, I printed the nodes expanded and distance
		6. Using the success node pointer, I calculated the route and saved it to a successor array called route
		7. Using this successor array, I printed the route

#### Running Code: 
	1. Compile: g++ find_route.cpp -o find
	2. Run with no heuristic: ./find input1.txt origin_city destination_city
	3. Run with heuristics: ./find input1.txt origin_city destination_city h_kassel.txt
