# This program generates a randomized maze using the DFS algorithm
from tkinter import mainloop
import turtle

# constants
size = 32
maze_size = 8
offset = 16

# Get the wall data from encoded number given in input
def parse_walls(maze, x, y) -> list:
        walls = []
        val = maze[y][x]

        for i in range(4):
            walls.insert(0, val%2) # This bit is 1 if there is no wall in that direction
            val >>= 1

        return walls

class MazeDrawer(turtle.Pen):
           
    def draw_walls(self, maze) -> None:
        for y in range(maze_size):
            for x in range(maze_size):
                self.pu()
                self.setpos((x-(maze_size/2))*size, ((maze_size/2)-y)*size)
                self.seth(0)
                walls = parse_walls(maze,x,y)

                for wall in walls:
                    if wall==1:
                        self.pu()
                    else:
                        self.pd()
                    self.forward(size)
                    self.right(90)
        self.pu()

class Micromouse(turtle.Turtle):
    def __init__(self, start, target, raw_maze):
        super().__init__()
        self.hideturtle()
        self.shape("circle")
        self.speed(1)

        self.set_route(start, target)
        self.raw_maze = raw_maze
        self.sensed_maze = [[0 for x in range(maze_size)] for y in range(maze_size)]
        
    def set_route(self, start, target):
        # Tuples containing the x,y coordinates of the start and the target cell
        self.start = start
        self.target = target    

        x,y = start
        self.pu()
        self.setpos((x-(maze_size/2))*size + offset, ((maze_size/2)-y)*size - offset)
        self.showturtle()

    def FloodFill(self):
        # Distances assuming there are no walls
        self.maze = [[abs(x-self.target[0]) + abs(y-self.target[1]) 
                        for x in range(maze_size)] 
                        for y in range(maze_size)]
        self.visited = [[0 for i in range(maze_size)] for j in range(maze_size)]
    
        # Go to each cell and update its distance until the goal is reached
        cell = self.start
        node_stack = [cell]

        # Recalculate the costs
        while len(node_stack): # set visited to start is true
            # Move to the cell
            next_cell = node_stack.pop()
            self.goto_cell(next_cell, cell)
            cell=next_cell
            self.visited[cell[1]][cell[0]]=2
            # Get sensor data
            self.sensed_maze[cell[1]][cell[0]] = self.raw_maze[cell[1]][cell[0]]
            # Update the costs
            self.update_cells(cell)

            # Go to the minimum neighbour
            neighbours = self.get_open_neighbours(cell)
            unvisited = list(filter(lambda x : not self.visited[x[1]][x[0]], neighbours))
            node_stack.extend(unvisited)

        # Go to the end
        self.goto_cell(self.start)

    def update_cells(self, cell):
        node_queue = [cell]

        while len(node_queue):
            curr_cell = node_queue.pop()

            # Check if we reached the target
            if curr_cell==self.target:
                continue

            neighbours = self.get_open_neighbours(curr_cell)
            if(neighbours):
                min_value = min(neighbours, key=lambda coords : self.maze[coords[1]][coords[0]])

                # Check if the value of this cell is less than the minimum + 1
                if self.maze[curr_cell[1]][curr_cell[0]] <= self.maze[min_value[1]][min_value[0]]:

                    self.maze[curr_cell[1]][curr_cell[0]] = self.maze[min_value[1]][min_value[0]]+1
                    # Add all accessible neighbours to the queue
                    for neighbour in neighbours:
                        node_queue.insert(0, neighbour)
    
    def goto_cell(self, coords, cell):
        # A* algorithm to go the cell
        while cell!=coords:
            # Move the turtle to that node
            self.setpos((cell[0]-(maze_size/2))*size+offset, 
                        ((maze_size/2) - cell[1])*size-offset)
            
            # Go to the minimum neighbour
            neighbours = self.get_open_neighbours(cell)
            cell = min(neighbours, key= lambda x:abs(coords[0]-x[0])+abs(coords[1]-x[1]))

        # Move the turtle to that node
        self.setpos((cell[0]-(maze_size/2))*size+offset, 
                ((maze_size/2) - cell[1])*size-offset)
    
    def get_open_neighbours(self,coords):
        neighbours = []
        x,y = coords
        
        # Get the walls of the cell
        walls = parse_walls(self.sensed_maze, x, y)

        # Get the surrounding neighbours
        if maze_size>x>=0 and maze_size>y-1>=0 and walls[0]:
            neighbours.append((x,y-1))
        if maze_size>x+1>=0 and maze_size>y>=0 and walls[1]:
            neighbours.append((x+1, y))
        if maze_size>x>=0 and maze_size>y+1>=0 and walls[2]:
            neighbours.append((x,y+1))
        if maze_size>x-1>=0 and maze_size>y>=0 and walls[3]:
            neighbours.append((x-1,y))

        return neighbours

def draw(filename : str, start, target) -> None:
    # read the maze and create the maze
    with open(filename) as f:
        data = f.readlines()
    
    maze = []
    for line in data:
        l = [int(x) for x in line.split(",")]
        maze.append(l)
        

    global maze_size
    maze_size = len(maze)

    # create the drawer and draw the maze
    drawer = MazeDrawer()
    drawer.speed(0)
    drawer.hideturtle()
    drawer.draw_walls(maze)

    # create the micromouse and solve the maze
    mouse = Micromouse(start,target, maze)
    # Traverse the maze
    mouse.FloodFill()

if __name__=="__main__":
    start = tuple([int(x) for x in input("Start: ").split(',')])
    target = tuple([int(x) for x in input("Target: ").split(',')])
    draw('maze2.txt', start, target)
    mainloop()

