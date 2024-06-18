package src.pas.battleship.agents;


// SYSTEM IMPORTS


// JAVA PROJECT IMPORTS
import edu.bu.battleship.agents.Agent;
import edu.bu.battleship.game.Game.GameView;
import edu.bu.battleship.game.EnemyBoard.Outcome;
import edu.bu.battleship.utils.Coordinate;


public class ProbabilisticAgent
    extends Agent
{

    public ProbabilisticAgent(String name)
    {
        super(name);
        System.out.println("[INFO] ProbabilisticAgent.ProbabilisticAgent: constructed agent");
    }

    @Override
    public Coordinate makeMove(final GameView game)
    {

        return null;
    }

    @Override
    public void afterGameEnds(final GameView game) {}

}
