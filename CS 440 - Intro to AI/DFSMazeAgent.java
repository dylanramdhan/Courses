package src.labs.stealth.agents;

// SYSTEM IMPORTS
import edu.bu.labs.stealth.agents.MazeAgent;
import edu.bu.labs.stealth.graph.Vertex;
import edu.bu.labs.stealth.graph.Path;


import edu.cwru.sepia.environment.model.state.State.StateView;


import java.util.HashSet;       // will need for bfs
import java.util.Stack;         // will need for dfs
import java.util.LinkedList;    // will need for bfs
import java.util.Set;           // will need for bfs


// JAVA PROJECT IMPORTS


public class DFSMazeAgent
    extends MazeAgent
{

    public DFSMazeAgent(int playerNum)
    {
        super(playerNum);
    }

    @Override
    public Path search(Vertex src,
                       Vertex goal,
                       StateView state)
    {
        Set<Vertex> visited = new HashSet<>(); // HashSet to keep track of visited nodes
        Stack<Path> queue = new Stack<>(); // Queue to manage the nodes to visit
        
        Path begin = new Path(src);
        Path gold = new Path(goal);

        stack.add(begin);
        visited.add(src);
    
        
            while (!stack.isEmpty()) 
            {
                Path node = stack.pop(); // Queue = start
                Vertex currentAt = node.getDestination();

                //for(int i = 0; i > 10 ; i++) // tried putting coordinates in linked list
                //{
                    //Vertex currentAt = add(begin);
                    //Path currentPath = queue.poll();
                    
                    // Angeled Directions
                    Vertex southEast = new Vertex(currentAt.getXCoordinate() + 1, currentAt.getYCoordinate() + 1);
                    Vertex southWest = new Vertex(currentAt.getXCoordinate() - 1, currentAt.getYCoordinate() + 1);
                    Vertex northEast = new Vertex(currentAt.getXCoordinate() + 1, currentAt.getYCoordinate() - 1);
                    Vertex northWest = new Vertex(currentAt.getXCoordinate() - 1, currentAt.getYCoordinate() - 1);

                    // North, East, South, West
                    Vertex north = new Vertex(currentAt.getXCoordinate(), currentAt.getYCoordinate() - 1);
                    Vertex east = new Vertex(currentAt.getXCoordinate() + 1, currentAt.getYCoordinate()); 
                    Vertex south = new Vertex(currentAt.getXCoordinate(), currentAt.getYCoordinate() + 1);
                    Vertex west = new Vertex(currentAt.getXCoordinate() - 1, currentAt.getYCoordinate());
                

                ////////////// Now Finding The Gold ///////////////////
                   // Must create a Linked List:
                    Vertex NearTheGold[] = {southEast, southWest, northEast, northWest, north, east, west, south};

                   
                // Check for gold
                   for (Vertex nearby : NearTheGold)
                    {
                        if (NearTheGold(nearby, gold))
                        {
                            return node;
                        }
                    } 
                   
                   
                   for (Vertex nearby : NearTheGold) // graph.getNeighbors(node)) 
                    { 
                        if (!visited.contains(nearby) && Valid(nearby, state)) 
                        {
                            visited.add(nearby); // Mark the neighbor as visited
                            stack.add(new Path(nearby, 1f, node)); // Add the neighbor to the queue for future processing
                        }
                    }
                
                //}
                

            }
            
            return null; 
    } 
        // Function helping to check if its near the goal
        public boolean NearTheGold(Vertex point, Path ending)
        {
            if (point.equals(ending.getDestination()))
            {
                return true;
            }
            else 
            {
                return false;
            }
        }


        public boolean Valid(Vertex point, StateView state)
        {
            if (state.inBounds(point.getXCoordinate(), point.getYCoordinate()) 
                                && (!state.isResourceAt(point.getXCoordinate(), point.getYCoordinate())))
            {
                return true;
            }
            else
            {
                return false;
            }
        }



        @Override
        public boolean shouldReplacePlan(StateView TheState) // returns true if current plan is now INVALID
        {
            return false;
        }

}
