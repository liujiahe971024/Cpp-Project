1.  
    1. Initialize all vertices as not visited
    2. For every vertex 'V':
         if 'V' is not visited before:  1. call DFS(V)
                                        2. Print new line character

    DFS(V):
        1. Mark 'V' as visited
        2. Print 'V'
        3. For every adjacent 'A' of 'V', if 'A' is not visited,
           Recursively call DFS(A)

    If there is only one line priented out, it is connected undirected graph. 
    Otherwise, it is not a connected undirected graph.
    The runtime of this algorithm is O(V+E).


2.
   Use the same algoritm as above, count how many lines in the output, each 
   line is a different connected components. The runtime is also O(V+E).

3.
    1. Initialize all vertices as not visited and distance as INT_MAX
    2. Initialize the distance of the vertice of "Kevin Bacon" as 0
    3. Set a priority queue with two arguments, first is an integer of the 
        distance, second is the vertice 'V'. The vertice which have a smaller
         distance have higher priority.
    4. Push the vertice of "Kevin Bacon" with 0 distance to the priority queue
    5. While the priority queue is not empty:
        1. Copy the top vertice "V" from the priority queue
        2. pop the priority queue
        3. If 'V' is not visited before:
            1. Mark 'V' as visited
            2. For every adjacent vertice 'A' of 'V':
                1. if 'A' distance = INT_MAX:
                    1. Set 'A' distance as ('V' distance + 1)
                    2. if 'A' distance larger or equal to 7, print "False"
                    3. Push ('A' distance, vertice 'A') into the priority queue
 
    If nothing print out after run this algorithm, it shows "Six Degrees of 
    Kevin Bacon" is true. If you see some "False" printed out, it shows 
    there are some actors have more than 6 edges relationship between them and
    Kevin Bacon. The algorithm runtime is O(V+E)

