#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <cstring>
#include <unordered_set>
#include <queue>
#include <cmath>
#include <sstream> 

struct State
{
    //struct for a State object, holding a puzzle state, g and h values, and a stack to store the moves to reach to the puzzle state

    std::vector<std::vector<int>> puzzle_state;
    int g;
    int h;
    std::stack<std::string> movesToState;

    State(std::vector<std::vector<int>> puzzl_st, int _g, int _h, std::stack<std::string> moves)
    {
        puzzle_state = puzzl_st;
        g = _g;
        h = _h;
        movesToState = moves;
    }

    //operator to help sort the States in the queue

    bool operator<(State const &other) const
    {
        return (g + h) > (other.g + other.h);
    }
};

int manhattan_distance(std::vector<std::vector<int>> current, int width, int height)
{
    //calculate how far every space on the board is from their final position using |x1 - x2| + |y1 - y2|, giving us a h value for a state

    int distance = 0;
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            int value = current[j][i];

            if (value != 0) 
            {
                int x_2 = (value - 1) / width;

                int y_2 = (value - 1) % width;

                distance += abs(i - x_2) + abs(j - y_2);
            }
        }
    }
    return distance;
}

void print_grid(std::string state, int width)
{
    std::istringstream iss(state);
    std::string tile;

    int total_tiles = 0;

    while(iss >> tile)
    {
        if(tile == "0")
        {
            std::cout << "_";
        }
        else
        {
            std::cout << tile;
        }

        total_tiles++;

        if(total_tiles == width)
        {
            std::cout << std::endl;

            total_tiles = 0;
        }
        else
        {
            std::cout << " ";
        }
    }

    std::cout << std::endl;
}

std::string convert_to_string(std::vector<std::vector<int>> states, int width, int height)
{
    //turns the 2d grid into a string for easy storage

    std::string result;
    for (int i = 0; i < height; i++) 
    {
        for (int j = 0; j < width; j++) 
        {
            result += std::to_string(states[j][i]) + " ";
        }
    }
    return result;
}

std::vector<int> make_solution(std::vector<int> puzzle, int tile_count)
{
    //takes the string made from the arguments after "-w" and sorts them to their positions when the puzzle is solved

    std::vector<int> solution = puzzle;
    for(int i = 1; i < tile_count; i++)
    {
        int key = solution[i];
        int j = i-1;

        while(j >= 0 && solution[j] > key)
        {
            solution[j + 1] = solution[j];
            j = j-1;
        }

        solution[j + 1] = key;
    }
    return solution;
}

std::vector<std::vector<int>> swap_tiles(std::vector<std::vector<int>> current, int empty_x, int empty_y, int dir = 0)
{
    //swaps the position of two tiles
    //dir 0 = up, 1 = left, 2 = down, 3 = right

    std::vector<std::vector<int>> new_current = current;
    if(dir == 0)
    {
        std::swap(new_current[empty_x][empty_y], new_current[empty_x][empty_y - 1]);

        return new_current;
    }
    else if(dir == 1)
    {
        std::swap(new_current[empty_x][empty_y], new_current[empty_x - 1][empty_y]);

        return new_current;
    }
    else if(dir == 2)
    {
        std::swap(new_current[empty_x][empty_y], new_current[empty_x][empty_y + 1]);

        return new_current;
    }
    else
    {
        std::swap(new_current[empty_x][empty_y], new_current[empty_x + 1][empty_y]);
        
        return new_current;
    }
}

void print_stack(std::stack<std::string>& stack, int width)
{
    //recursively print all the states from the initial state to the final state, showing the solution to the puzzle

    if (stack.empty())
    {
        return;
    }
    std::string state = stack.top();
    stack.pop();
    print_stack(stack, width);
    print_grid(state, width);
    return;
}

int get_empty_coord(std::vector<std::vector<int>> state, int dim, int width, int height)
{
    //searches for the "_" space in the puzzle so it can get its coordinates

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(state[j][i] == 0)
            {
                if(dim == 0)
                {
                    //returns X
                    return j;
                }

                if(dim == 1)
                {
                    //returns Y
                    return i;
                }
                
            }
        }
    }
    std::cout << "Error: no _ space in the puzzle" << std::endl;
    exit(EXIT_FAILURE);
}

bool is_int(std::string str) 
{
    //used to ensure the arguments for the puzzle are not non-integers

    int char_count = str.length();
    for (int i = 0; i < char_count; i++)
    {
        if (!isdigit(str[i]))
        {
            return false;
        }    
    }
    return true;
}

bool is_goal(std::vector<std::vector<int>> goal, std::vector<std::vector<int>> current, int width, int height)
{
    //check if the current state of the DFS is the same as the goal state
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(current[j][i] != goal[j][i])
            {
                return false;
            }
        }
    }
    return true;
}

bool is_valid_move(int x, int y, int width, int height, int dir)
{
    //check in which directions can a swap be made
    //dir 0 = up, 1 = left, 2 = down, 3 = right

    if(dir == 0)
    {
        if(y > 0)
        {
            return true;
        }
    }
    if(dir == 1)
    {
        if(x > 0)
        {
            return true;
        }
    }
    if(dir == 2)
    {
        if(y < height - 1)
        {
            return true;
        }
    }
    if(dir == 3)
    {
        if(x < width - 1)
        {
            return true;
        }
    }
    return false;
}

bool DFS(std::vector<std::vector<int>>& goal, std::vector<std::vector<int>>& current, int depth, int limit, int width, int height, bool verbose, std::unordered_set<std::string>& visited, std::stack<std::string> moves)
{
    //check if the current state matches the goal

    if(is_goal(goal, current, width, height))
    {
        if(verbose)
        {
            print_stack(moves, width);
        }
        std::cout << "ID path length=" << depth + 1 << std::endl;
        return true;
    }

    //backtrack if the depth limit is reached

    if(depth > limit)
    {
        return false;
    }

    //serialize the grid state into a string, compare it to the others in the visited set, if a match is found the loop continues to the next step

    std::string current_str = convert_to_string(current, width, height);
    if (visited.find(current_str) != visited.end())
    {
        return false;
    }
    visited.insert(current_str);

    //get the position of the empty spot on the puzzle

    int empty_x = get_empty_coord(current, 0, width, height);
    int empty_y = get_empty_coord(current, 1, width, height);

    //test all possible directions for new moves, if the move is valid we dive one depth level deeper with the changed state of the puzzle

    for(int i = 0; i < 4; i++)
    {
        if(is_valid_move(empty_x,empty_y, width, height, i))
        {
            std::vector<std::vector<int>> new_state = swap_tiles(current, empty_x, empty_y, i);
            moves.push(convert_to_string(new_state, width, height));
            if(DFS(goal, new_state, depth + 1, limit, width, height, verbose, visited, moves))
            {
                return true;
            }
            moves.pop();
        }        
    }

    //clean the visited set while backtracking
    visited.erase(current_str);
    return false;    
}

void solve_by_ID(std::vector<std::vector<int>> goal, std::vector<std::vector<int>> initial, bool verbose, int width, int height)
{
    //setup the depth limit, initial state, the initial state for the IDS search, and the set of visited states

    int limit = 0;
    std::vector<std::vector<int>> current (width, std::vector<int>(height));

    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            current[j][i] = initial[j][i];
        }
    }

    while(true)
    {
        std::unordered_set<std::string> visited;
        std::stack<std::string> moves;
        moves.push(convert_to_string(current, width, height));
        if(DFS(goal, current, 0, limit, width, height, verbose, visited, moves))
        {
            break;
        }
        limit++;
    }
}

void solve_by_Astar(std::vector<std::vector<int>> goal, std::vector<std::vector<int>> initial, bool verbose, int width, int height)
{
    //setup initial state, the queue for the A* pathing, and the set of visited states

    std::unordered_set<std::string> visited;
    std::priority_queue<State> _queue;
    int start_h = manhattan_distance(initial, width, height);
    _queue.push(State(initial, 0, start_h, {}));

    //while loop will explore every state on the queue until the solution is found or the queue is completely empty, giving out a "No Solution Found"

    while(!_queue.empty())
    {
        State current = _queue.top();
        _queue.pop();

        //check if the current state matches the goal

        if(is_goal(goal, current.puzzle_state, width, height))
        {
            if(verbose)
            {
                current.movesToState.push(convert_to_string(current.puzzle_state, width, height));
                print_stack(current.movesToState, width);
            }
            std::cout << "A* path length=" << current.g + 1 << std::endl;
            exit(0);
        }

        //serialize the grid state into a string, compare it to the others in the visited set, if a match is found the loop continues to the next step

        std::string current_string = convert_to_string(current.puzzle_state, width, height);
        if (visited.find(current_string) != visited.end())
        {
            continue;
        }
        visited.insert(current_string);

        //get the position of the empty spot on the puzzle

        int empty_x = get_empty_coord(current.puzzle_state, 0, width, height);
        int empty_y = get_empty_coord(current.puzzle_state, 1, width, height);

        //test all possible directions for new moves, if the move is valid a new State is made and added to the queue

        for(int i = 0; i < 4; i++)
        {
            if(is_valid_move(empty_x,empty_y, width, height, i))
            {
                std::vector<std::vector<int>> new_puzzle = current.puzzle_state;
                new_puzzle = swap_tiles(new_puzzle, empty_x, empty_y, i);
                int new_g = current.g + 1;
                int new_h = manhattan_distance(new_puzzle, width, height);
                std::stack<std::string> new_movesToState = current.movesToState;
                new_movesToState.push(convert_to_string(current.puzzle_state, width, height));

                _queue.push(State(new_puzzle, new_g, new_h, new_movesToState));
            }        
        }
    }

    std::cout << "No Solution Found" << std::endl;
    exit(0);
}

int main(int argc, char** argv)
{
    //booleans to ensure all the arguments are read properly
    bool astar = false;
    bool id = false;
    bool verbose = false;
    bool puzzle = false;
    bool grid_build = false;
    bool got_width = false;

    int tile_count = 0;
    int width = 0;
    int height = 0;

    //storage for the puzzle start and solution states
    std::vector<int> puzzle_tiles;
    std::vector<int> puzzle_goal;
    std::string arg_string;

    if(argc < 2)
    {
        std::cout << "Error: no arguments given" << std::endl;
        return 1;
    }

    //turn argv into strings, then parse them as needed

    for(int i = 1; i < argc; i++)
    {
        arg_string += argv[i];
        arg_string += " ";
    }

    std::istringstream iss(arg_string);
    std::string arg;

    while(iss >> arg)
    {
        if(arg == "-a")
        {
            puzzle = true;
            continue;
        }

        if(puzzle)
        {
            if(arg == "astar")
            {
                astar = true;
                puzzle = false;
                continue;
            }
            else if(arg == "id")
            {
                id = true;
                puzzle = false;
                continue;
            }
            else
            {
                std::cout << "Error: non-valid option given for search algorithm" << std::endl;
                return 1;
            }
        }

        if(arg == "-v")
        {
            verbose = true;
            continue;
        }

        if(arg == "-w")
        {
            grid_build = true;
            continue;
        }

        if(grid_build && !got_width)
        {
            if(is_int(arg))
            {
                width = std::stoi(arg);
                got_width = true;
                continue;
            }

            std::cout << "Error: puzzle width cannot be a non-integer" << std::endl;
            return 1;
        }

        if(grid_build && got_width)
        {
            if(arg == "_")
            {
                puzzle_tiles.push_back(0);
                tile_count++;
                continue;
            }
            else if(is_int(arg))
            {
                puzzle_tiles.push_back(std::stoi(arg));
                tile_count++;
                continue;
            }
            else
            {
                std::cout << "Error: puzzle cannot contain non-integers" << std::endl;
                return 1;
            }
        }
    }

    //final set of error checks before proceeding to build the puzzle grids

    if(!id && !astar)
    {
        std::cout << "Error: No algorithm chosen" << std::endl;
        return 1;
    }

    if(tile_count < width)
    {
        std::cout << "Error: Not enough tiles for a solvable puzzle" << std::endl;
        return 1;
    }

    if(tile_count % width != 0)
    {
        std::cout << "Error: the amount of tiles given is not divisible by the width" << std::endl;
        return 1;
    }

    if(tile_count/width == 1)
    {
        std::cout << "Error: not enough height for a valid puzzle" << std::endl;
        return 1;
    }

    //setting up the 2d vector arrays for the inital and goal states of the puzzle

    height = tile_count/width;
    puzzle_goal = make_solution(puzzle_tiles, tile_count);
    std::vector<std::vector<int>> problem (width, std::vector<int>(height));
    std::vector<std::vector<int>> goal (width, std::vector<int>(height));
    int tile_index = 0;

    for (int i = 0; i < height; i++) 
    {
        for (int j = 0; j < width; j++) 
        {
            if (tile_index < tile_count) 
            {
                problem[j][i] = puzzle_tiles[tile_index];
                goal[j][i] = puzzle_goal[tile_index];
                tile_index++;
            }
        }
    }

    //braching out to the corresponding algorithm for solving the puzzle

    if(id && !astar)
    {
        solve_by_ID(goal,problem,verbose,width,height);
    }

    if(astar && !id)
    {
        solve_by_Astar(goal,problem,verbose,width,height);
    }

    return(0);
}