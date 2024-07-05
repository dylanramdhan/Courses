package src.labs.pitfall.agents;


// SYSTEM IMPORTS
import edu.cwru.sepia.action.Action;
import edu.cwru.sepia.agent.Agent;
import edu.cwru.sepia.environment.model.history.History.HistoryView;
import edu.cwru.sepia.environment.model.state.State.StateView;
import edu.cwru.sepia.environment.model.state.Unit.UnitView;


import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.Set;


// JAVA PROJECT IMPORTS
import edu.bu.labs.pitfall.Difficulty;
import edu.bu.labs.pitfall.Synchronizer;
import edu.bu.labs.pitfall.utilities.Coordinate;



public class BayesianAgent
    extends Agent
{

    public static class PitfallBayesianNetwork
        extends Object
    {
        private Map<Coordinate, Boolean>    knownBreezeCoordinates;
        private Set<Coordinate>             frontierPitCoordinates;
        private Set<Coordinate>             otherPitCoordinates;
        private final double                pitProb;

        public PitfallBayesianNetwork(Difficulty difficulty)
        {
            this.knownBreezeCoordinates = new HashMap<Coordinate, Boolean>();

            this.frontierPitCoordinates = new HashSet<Coordinate>();
            this.otherPitCoordinates = new HashSet<Coordinate>();

            this.pitProb = Difficulty.getPitProbability(difficulty);
        }

        public Map<Coordinate, Boolean> getKnownBreezeCoordinates() { return this.knownBreezeCoordinates; }
        public Set<Coordinate> getFrontierPitCoordinates() { return this.frontierPitCoordinates; }
        public Set<Coordinate> getOtherPitCoordinates() { return this.otherPitCoordinates; }
        public final double getPitProb() { return this.pitProb; }


        /**
         *  TODO: please replace this code. The code here will pick a **random** frontier square to explore next,
         *        which may be a pit! You should do the following steps:
         *          1) for each frontier square X, calculate the query Pr[Pit_X = true | evidence]
         *             we typically expand this to say:
         *                         Pr[Pit_X = true | evidence] = alpha * Pr[Pit_X = true && evidence]
         *             however you don't need to calculate alpha explicitly.
         *             If you calculate Pr[Pit_X = true && evidence] for every X, you can convert the values into
         *             probabilities by adding up all Pr[Pit_X = true && evidence] values and dividing each
         *             Pr[Pit_X = true && evidence] value by the sum.
         *
         *          2) pick the pit that is the least likely to have a pit in it to explore next!
         *
         *          As an aside here, you can certainly choose to calculate Pr[Pit_X = false | evidence] values
         *          instead (and then pick the coordinate with the highest prob), its up to you!
         **/
        public Coordinate getNextCoordinateToExplore()
        {
            // System.out.println(this.getFrontierPitCoordinates());

            Coordinate toExplore = null;
            if(this.getFrontierPitCoordinates().size() > 0)
            {
                List<Coordinate> choices = new ArrayList<Coordinate>(this.getFrontierPitCoordinates());
                Collections.shuffle(choices);
                toExplore = choices.get(0);
            }
            return toExplore;
        }
    }

    private int                     myUnitID;
    private int                     enemyPlayerNumber;
    private Set<Coordinate>         gameCoordinates;
    private Set<Coordinate>         unexploredCoordinates;
    private Coordinate              coordinateIJustAttacked;
    private Coordinate              srcCoordinate;
    private Coordinate              dstCoordinate;
    private PitfallBayesianNetwork  bayesianNetwork;

    private final Difficulty        difficulty;

	public BayesianAgent(int playerNum, String[] args)
	{
        super(playerNum);

        if(args.length != 3)
		{
			System.err.println("[ERROR] BayesianAgent.BayesianAgent: need to provide args <playerID> <seed> <difficulty>");
		}

        this.myUnitID = -1;
        this.enemyPlayerNumber = -1;
        this.gameCoordinates = new HashSet<Coordinate>();
        this.unexploredCoordinates = new HashSet<Coordinate>();
        this.coordinateIJustAttacked = null;
        this.srcCoordinate = null;
        this.dstCoordinate = null;
        this.bayesianNetwork = null;

        this.difficulty = Difficulty.valueOf(args[2].toUpperCase());
	}

	public int getMyUnitID() { return this.myUnitID; }
    public int getEnemyPlayerNumber() { return this.enemyPlayerNumber; }
    public Set<Coordinate> getGameCoordinates() { return this.gameCoordinates; }
    public Set<Coordinate> getUnexploredCoordinates() { return this.unexploredCoordinates; }
    public final Coordinate getCoordinateIJustAttacked() { return this.coordinateIJustAttacked; }
    public final Coordinate getSrcCoordinate() { return this.srcCoordinate; }
    public final Coordinate getDstCoordinate() { return this.dstCoordinate; }
    public PitfallBayesianNetwork getBayesianNetwork() { return this.bayesianNetwork; }
    public final Difficulty getDifficulty() { return this.difficulty; }

    private void setMyUnitID(int i) { this.myUnitID = i; }
    private void setEnemyPlayerNumber(int i) { this.enemyPlayerNumber = i; }
    private void setCoordinateIJustAttacked(Coordinate c) { this.coordinateIJustAttacked = c; }
    private void setSrcCoordinate(Coordinate c) { this.srcCoordinate = c; }
    private void setDstCoordinate(Coordinate c) { this.dstCoordinate = c; }
    private void setBayesianNetwork(PitfallBayesianNetwork n) { this.bayesianNetwork = n; }

	@Override
	public Map<Integer, Action> initialStep(StateView state,
                                            HistoryView history)
	{

		// locate enemy and friendly units
        Set<Integer> myUnitIDs = new HashSet<Integer>();
		for(Integer unitID : state.getUnitIds(this.getPlayerNumber()))
        {
            myUnitIDs.add(unitID);
        }

        if(myUnitIDs.size() != 1)
        {
            System.err.println("[ERROR] PitfallAgent.initialStep: should only have 1 unit but found "
                + myUnitIDs.size());
            System.exit(-1);
        }

		// check that all units are archers units
	    if(!state.getUnit(myUnitIDs.iterator().next()).getTemplateView().getName().toLowerCase().equals("archer"))
	    {
		    System.err.println("[ERROR] PitfallAgent.initialStep: should only control archers!");
		    System.exit(1);
	    }

        // get the other player
		Integer[] playerNumbers = state.getPlayerNumbers();
		if(playerNumbers.length != 2)
		{
			System.err.println("ERROR: Should only be two players in the game");
			System.exit(-1);
		}
		Integer enemyPlayerNumber = null;
		if(playerNumbers[0] != this.getPlayerNumber())
		{
			enemyPlayerNumber = playerNumbers[0];
		} else
		{
			enemyPlayerNumber = playerNumbers[1];
		}

        // check enemy units
        Set<Integer> enemyUnitIDs = new HashSet<Integer>();
        for(Integer unitID : state.getUnitIds(enemyPlayerNumber))
        {
            if(!state.getUnit(unitID).getTemplateView().getName().toLowerCase().equals("hiddensquare"))
		    {
			    System.err.println("ERROR [BayesianAgent.initialStep]: Enemy should start off with HiddenSquare units!");
			        System.exit(-1);
		    }
            enemyUnitIDs.add(unitID);
        }


        // initially everything is unknown
        Coordinate coord = null;
        for(Integer unitID : enemyUnitIDs)
        {
            coord = new Coordinate(state.getUnit(unitID).getXPosition(),
                                   state.getUnit(unitID).getYPosition());
            this.getUnexploredCoordinates().add(coord);
            this.getGameCoordinates().add(coord);
        }

        this.setMyUnitID(myUnitIDs.iterator().next());
        this.setEnemyPlayerNumber(enemyPlayerNumber);
        this.setSrcCoordinate(new Coordinate(1, state.getYExtent() - 2));
        this.setDstCoordinate(new Coordinate(state.getXExtent() - 2, 1));
        this.setBayesianNetwork(new PitfallBayesianNetwork(this.getDifficulty()));

        Map<Integer, Action> initialActions = new HashMap<Integer, Action>();
        initialActions.put(
            this.getMyUnitID(),
            Action.createPrimitiveAttack(
                this.getMyUnitID(),
                state.unitAt(this.getSrcCoordinate().getXCoordinate(), this.getSrcCoordinate().getYCoordinate())
            )
        );
        this.getUnexploredCoordinates().remove(this.getSrcCoordinate());
		return initialActions;
	}

    public boolean isFrontierCoordiante(Coordinate src,
                                        StateView state)
    {
        int dirs[][] = new int[][]{{-1, 0}, {+1, 0}, {0, -1}, {0, +1}};
        for(int dir[] : dirs)
        {
            int x = src.getXCoordinate() + dir[0];
            int y = src.getYCoordinate() + dir[1];

            if(x >= 1 && x <= state.getXExtent() - 2 &&
               y >= 1 && y <= state.getYExtent() - 2 &&
               (!state.isUnitAt(x, y) ||
                !state.getUnit(state.unitAt(x, y)).getTemplateView().getName().toLowerCase().equals("hiddensquare")))
            {
                return true;
            }
        }
        return false;
    }

    public void makeObservations(StateView state,
                                 HistoryView history)
    {
        this.getBayesianNetwork().getKnownBreezeCoordinates().clear();
        this.getBayesianNetwork().getFrontierPitCoordinates().clear();
        this.getBayesianNetwork().getOtherPitCoordinates().clear();

        Set<Coordinate> exploredCoordinates = new HashSet<Coordinate>();
        for(Integer enemyUnitID : state.getUnitIds(this.getEnemyPlayerNumber()))
        {
            UnitView enemyUnitView = state.getUnit(enemyUnitID);
            if(enemyUnitView.getTemplateView().getName().toLowerCase().equals("breezesquare"))
            {
                this.getBayesianNetwork().getKnownBreezeCoordinates().put(
                    new Coordinate(enemyUnitView.getXPosition(),
                                   enemyUnitView.getYPosition()),
                    true
                );
            } else if(enemyUnitView.getTemplateView().getName().toLowerCase().equals("safesquare"))
            {
                this.getBayesianNetwork().getKnownBreezeCoordinates().put(
                    new Coordinate(enemyUnitView.getXPosition(),
                                   enemyUnitView.getYPosition()),
                    false
                );
            } else if(enemyUnitView.getTemplateView().getName().toLowerCase().equals("hiddensquare"))
            {
                this.getBayesianNetwork().getOtherPitCoordinates().add(
                    new Coordinate(enemyUnitView.getXPosition(),
                                   enemyUnitView.getYPosition())
                );
            }

            // now separate out the frontier from the "other" ones
            for(Coordinate unknownCoordinate : this.getBayesianNetwork().getOtherPitCoordinates())
            {
                if(this.isFrontierCoordiante(unknownCoordinate, state))
                {
                    this.getBayesianNetwork().getFrontierPitCoordinates().add(unknownCoordinate);
                }
            }
            this.getBayesianNetwork().getOtherPitCoordinates().removeAll(
                this.getBayesianNetwork().getFrontierPitCoordinates()
            );
        }
    }

	@Override
	public Map<Integer, Action> middleStep(StateView state,
                                           HistoryView history) {
		Map<Integer, Action> actions = new HashMap<Integer, Action>();

        if(Synchronizer.isMyTurn(this.getPlayerNumber(), state))
        {

            // get the observation from the past
            if(state.getTurnNumber() > 0)
            {
                this.makeObservations(state, history);
            }

            Coordinate coordinateOfUnitToAttack = this.getBayesianNetwork().getNextCoordinateToExplore();

            // could have won the game (and waiting for enemy units to die)
            // or we have a coordinate to attack
            // we need to check that the unit at that coordinate is a hidden square (not allowed to attack other units)
            if(coordinateOfUnitToAttack != null)
            {
                Integer unitID = state.unitAt(coordinateOfUnitToAttack.getXCoordinate(),
                                              coordinateOfUnitToAttack.getYCoordinate());
                if(unitID == null)
                {
                    System.err.println("ERROR: BayesianAgent.middleStep: deciding to attack unit at " +
                        coordinateOfUnitToAttack + " but no unit was found there!");
                    System.exit(-1);
                }

                String unitTemplateName = state.getUnit(unitID).getTemplateView().getName();
                if(!unitTemplateName.toLowerCase().equals("hiddensquare"))
                {
                    // can't attack non hidden-squares!
                    System.err.println("ERROR: BayesianAgent.middleStep: deciding to attack unit at " +
                        coordinateOfUnitToAttack + " but unit at that square is [" + unitTemplateName + "] " +
                        "and should be a HiddenSquare unit!");
                    System.exit(-1);
                }
                this.setCoordinateIJustAttacked(coordinateOfUnitToAttack);

                actions.put(
                    this.getMyUnitID(),
                    Action.createPrimitiveAttack(
                        this.getMyUnitID(),
                        unitID)
                );
                this.getUnexploredCoordinates().remove(coordinateOfUnitToAttack);
            }

        }

		return actions;
	}

    @Override
	public void terminalStep(StateView state, HistoryView history) {}

    @Override
	public void loadPlayerData(InputStream arg0) {}

	@Override
	public void savePlayerData(OutputStream arg0) {}

}

