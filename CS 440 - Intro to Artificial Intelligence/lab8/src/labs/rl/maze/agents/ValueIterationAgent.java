package src.labs.rl.maze.agents;


// SYSTEM IMPORTS
import edu.cwru.sepia.action.Action;
import edu.cwru.sepia.agent.Agent;
import edu.cwru.sepia.environment.model.history.History.HistoryView;
import edu.cwru.sepia.environment.model.state.Unit.UnitView;
import edu.cwru.sepia.environment.model.state.State.StateView;
import edu.cwru.sepia.util.Direction;


import java.io.InputStream;
import java.io.OutputStream;
import java.math.BigDecimal;
import java.math.RoundingMode;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;


// JAVA PROJECT IMPORTS
import edu.bu.labs.rl.maze.agents.StochasticAgent;
import edu.bu.labs.rl.maze.agents.StochasticAgent.RewardFunction;
import edu.bu.labs.rl.maze.agents.StochasticAgent.TransitionModel;
import edu.bu.labs.rl.maze.utilities.Coordinate;
import edu.bu.labs.rl.maze.utilities.Pair;
import java.util.*;



public class ValueIterationAgent
    extends StochasticAgent
{

    public static final double GAMMA = 0.1; // feel free to change this around!
    public static final double EPSILON = 1e-6; // don't change this though

    private Map<Coordinate, Double> utilities;

	public ValueIterationAgent(int playerNum)
	{
		super(playerNum);
        this.utilities = null;
	}

    public Map<Coordinate, Double> getUtilities() { return this.utilities; }
    private void setUtilities(Map<Coordinate, Double> u) { this.utilities = u; }

    public boolean isTerminalState(Coordinate c)
    {
        return c.equals(StochasticAgent.POSITIVE_TERMINAL_STATE)
            || c.equals(StochasticAgent.NEGATIVE_TERMINAL_STATE);
    }

    /**
     * A method to get an initial utility map where every coordinate is mapped to the utility 0.0
     */
    public Map<Coordinate, Double> getZeroMap(StateView state)
    {
        Map<Coordinate, Double> m = new HashMap<Coordinate, Double>();
        for(int x = 0; x < state.getXExtent(); ++x)
        {
            for(int y = 0; y < state.getYExtent(); ++y)
            {
                if(!state.isResourceAt(x, y))
                {
                    // we can go here
                    m.put(new Coordinate(x, y), 0.0);
                }
            }
        }
        return m;
    }



    public void valueIteration(StateView state)
    {
        // TODO: complete me!
        Map <Coordinate, Double> map = getZeroMap(state);
        Map <Coordinate, Double> PrimeMap = getZeroMap(state);

        boolean notPassed = true;

        double delta = 0.0;
        
        while (notPassed)
        {
            // cont. outer loop until value func changes 
            // by >= threshold across all states

            
            map = PrimeMap;

            for (int xAxis = 0; xAxis < state.getXExtent(); ++xAxis)
            {
                for (int yAxis = 0; yAxis < state.getYExtent(); ++yAxis)
                {
                    if (!state.isResourceAt(xAxis, yAxis))
                    {
                        // run bellman equations
                        if (xAxis == 3 && yAxis == 0) // set utilities of terminal states
                        {
                            PrimeMap.put(new Coordinate(xAxis, yAxis), 1.0);
                        }

                        else if (xAxis == 3 && yAxis == 1)
                        {
                            PrimeMap.put(new Coordinate(xAxis, yAxis), -1.0);
                        }

                        else // setting coordinates
                        {
                            Set<Pair<Coordinate, Double>> Westward = TransitionModel.getTransitionProbs(state, new Coordinate(xAxis, yAxis), Direction.WEST);
                            Set<Pair<Coordinate, Double>> Eastward = TransitionModel.getTransitionProbs(state, new Coordinate(xAxis, yAxis), Direction.EAST);
                            Set<Pair<Coordinate, Double>> Northward = TransitionModel.getTransitionProbs(state, new Coordinate(xAxis, yAxis), Direction.NORTH);
                            Set<Pair<Coordinate, Double>> Southward = TransitionModel.getTransitionProbs(state, new Coordinate(xAxis, yAxis), Direction.SOUTH);
                        
                            double maxValue = Math.max(Math.max(Math.max(GetSummation(Westward, map), GetSummation(Northward, map)), GetSummation(Eastward, map)), GetSummation(Southward, map));

                            double utility = RewardFunction.getReward(new Coordinate(xAxis, yAxis)) + (GAMMA * maxValue);
                            double olderUtility = map.get(new Coordinate(xAxis, yAxis));

                            PrimeMap.put(new Coordinate(xAxis, yAxis), utility);

                            if (Math.abs(utility - olderUtility) > delta)
                            {
                                delta = Math.abs(utility - olderUtility);
                            }
                        }
                    }
                }
            }

        }
        
        if (delta <= EPSILON * (1 - GAMMA) / GAMMA)
        {
            notPassed = false;
            this.setUtilities(map);
        }

    }



    private double GetSummation(Set<Pair<Coordinate, Double>> direc, Map<Coordinate, Double> map)
    {
        double summation = 0.0;
        for (Pair<Coordinate, Double> paired : direc)
        {
            Coordinate coordinate = paired.getFirst();

            double prob = paired.getSecond(); // probability
            double util = map.get(coordinate);

            double current = util * prob;
            summation += current;
        }

        return summation;
    }






    @Override
    public void computePolicy(StateView state,
                              HistoryView history)
    {
        // compute the utilities
        this.valueIteration(state);

        // compute the policy from the utilities
        Map<Coordinate, Direction> policy = new HashMap<Coordinate, Direction>();

        for(Coordinate c : this.getUtilities().keySet())
        {
            // figure out what to do when in this state
            double maxActionUtility = Double.NEGATIVE_INFINITY;
            Direction bestDirection = null;

            // go over every action
            for(Direction d : TransitionModel.CARDINAL_DIRECTIONS)
            {

                // measure how good this action is as a weighted combination of future state's utilities
                double thisActionUtility = 0.0;
                for(Pair<Coordinate, Double> transition : TransitionModel.getTransitionProbs(state, c, d))
                {
                    thisActionUtility += transition.getSecond() * this.getUtilities().get(transition.getFirst());
                }

                // keep the best one!
                if(thisActionUtility > maxActionUtility)
                {
                    maxActionUtility = thisActionUtility;
                    bestDirection = d;
                }
            }

            // policy recommends the best action for every state
            policy.put(c, bestDirection);
        }

        this.setPolicy(policy);
    }

}
