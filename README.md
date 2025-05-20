Sliding Puzzle Solver by Luis Huiza

Tested on CIMS using: linserv1.cims.nyu.edu

argument structure:

-a [algorithm (either astar or id)] (optionally -v) -w [width of the puzzle (single number)] [num of tiles in the puzzle (must include an empty space)]

ex: ./tiles -a astar -v -w 2 1 3 2 _

will output a file with the text:

1 3 
2 _ 

1 _ 
2 3 

_ 1 
2 3 

A* path length=3

using A*, and will output the number of moves to solve along with the moves that need to be done to solve it

if no algorithm is given, an error will occur

if a non integer is given after -w, an error will occur

if [num of tiles in the puzzle] is not divisible by [width of the puzzle], there will be an error

For the algorithm used for ID, I used the example in the contents section on Brightspace

def _DfsImpl(s, A, T, G, visited):

  if G(s):

    return True

  if visited[s]:

    return False

  visited[s] = True

  for a in A(s):

    if _DfsImpl(T(s,a), A, T, G):

    return True
    
  return False

def DFS(S, A, T, G):
  return _DfsImpl(s, A, T, G, set())

along with the example here: https://www.geeksforgeeks.org/depth-first-search-or-dfs-for-a-graph/ as a basis

In my implementation, void solve_by_id is the equivalent to def DFS, it sets up the initial and goal states, along with the dimensions of the puzzle, and uses a while loop along with a defined limit to start IDS

bool DFS then is the counterpart to DfsImpl, it handles the recursion calls to search each branching choice until we find a state that matches our goal state.

It first checks if it the current states matches the goal state, if it does, it ends the recursion and returns the results of the search

If not, it checks if the current state matches a previous one to avoid looping

then, we use a for loop to check if up, left, down, right are valid moves, and if they are, we recurse one depth deeper with the new states we get.

This continues until either we go past the depth limit, in which case we go back, and then increase the depth limit by 1, or we find the answer to the puzzle.

For the algorithm used for A*, I used the example in the contents section on Brightspace

def AStar(S, A, T, G, C, h):

  # assuming something called path which tracks the path and can provide g+h

  q = queue(path(S, h)) # min priority q which can handle g+h

  while q:

    p = q.pop() # by definition, the shortest path

    if G(p):

    return p # shorted path includes goal, return it

    for a in A(p.end):

    q.add(p.add(a, T(p.end, a))) # add each newly reachable path, queue sorts

For Astar, I made a struct State to handle the values of each state during the search

State contains:

The puzzle state

the value of g (number of moves from the initial state to this state)

the value of h (in this case, the manhattan distance of all the tiles current position to their end position)

a stack to keep track of all the moves needed to get to this state (for if -v is called)

we then use a priority queue, and use similar code to the one in ID to get new states based on all the permutations of the current state, and then continue down the best f = g + h value in the State queue until we find our goal state, or we go through all the States in the priority queue without finding the goal state, in which case it prints "No Solution Found"

For calculating the value of h in a State for A*, I used Manhattan distance

We look for the position of each space in the current board and the goal state, then calculate the difference with |x1 - x2| + |y1 - y2|, and the cumulative of all those operations is the h value of the current state.

References for the code, primarily data structures and a sorter for building the goal state 

https://en.cppreference.com/w/cpp/algorithm/swap
https://en.cppreference.com/w/cpp/container/unordered_set
https://en.cppreference.com/w/cpp/container/stack
https://cplusplus.com/reference/sstream/istringstream/istringstream/
https://www.geeksforgeeks.org/priority-queue-of-pairs-in-c-with-ordering-by-first-and-second-element/
https://www.geeksforgeeks.org/cpp-program-for-quicksort/