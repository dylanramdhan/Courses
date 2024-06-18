package src.labs.infexf.agents;

import java.util.*; // literally everything

import org.omg.PortableServer.IdAssignmentPolicyOperations;

// SYSTEM IMPORTS
import edu.bu.labs.infexf.agents.SpecOpsAgent;
import edu.bu.labs.infexf.distance.DistanceMetric;
import edu.bu.labs.infexf.graph.Vertex;
import edu.bu.labs.infexf.graph.Path;

import edu.cwru.sepia.environment.model.state.State.StateView;
import edu.cwru.sepia.environment.model.state.Unit.UnitView;

// JAVA PROJECT IMPORTS


public class InfilExfilAgent
    extends SpecOpsAgent
{

    public InfilExfilAgent(int playerNum)
    {
        super(playerNum);
    }

    // if you want to get attack-radius of an enemy, you can do so through the enemy unit's UnitView
    // Every unit is constructed from an xml schema for that unit's type.
    // We can lookup the "range" of the unit using the following line of code (assuming we know the id):
    //     int attackRadius = state.getUnit(enemyUnitID).getTemplateView().getRange();
    
        // completing these two functions;
    @Override
    public float getEdgeWeight(Vertex src,
                               Vertex dst,
                               StateView state)
    {
        float DANGER = 1f;

        // UID: Unit ID
        for (Integer UID: this.getOtherEnemyUnitIDs())
        {
            if(state.getUnit(UID) != null)
            {
                int AttackersRadius = state.getUnit(UID).getTemplateView().getRange();

                // Enemies X & Y Axis
                int enY = state.getUnit(UID).getYPosition();
                int enX = state.getUnit(UID).getXPosition();
                Vertex locationEnemy = new Vertex(enX, enY);

                double enDistance = DistanceMetric.chebyshevDistance(dst, locationEnemy);
                double DistanceToDeath = enDistance - AttackersRadius; 

                DANGER += 10000f / Math.pow(Math.max(1f, DistanceToDeath) , 2);
            }

            
        }
        
        return DANGER;
        // return 1f;
    }


    @Override
    public boolean shouldReplacePlan(StateView state)
    {
        // enemyUID = Enemy's Unit ID
        Integer enemyUID = this.getOtherEnemyUnitIDs().iterator().next();

        int eX = state.getUnit(enemyUID).getXPosition();
        int eY = state.getUnit(enemyUID).getYPosition();

        float ATTACKRADIUS = state.getUnit(enemyUID).getTemplateView().getRange();

        int numVerticesLookedAt = 0;

        Stack<Vertex> CurrentPlan = this.getCurrentPlan();
        Stack<Vertex> temperStack = new Stack<>();
        // intitaing this ^^ for record of not losing 
        //      vertices as its popped off stack to iterate


        while(!CurrentPlan.isEmpty() && numVerticesLookedAt < 10)
        {
            Vertex ver = CurrentP.peek();
            temperStack.push(CurrentPlan.pop()); // popping & stepping through 

            int differX = Math.abs(ver.getXCoordinate() - eX);
            int differY = Math.abs(ver.getYCoordinate() - eY);

            int dist = Math.max(differX, differY);

            if (dist <= ATTACKRADIUS + 1)
            {
                while (!temperStack.isEmpty())
                {
                    CurrentPlan.push (temperStack.pop()); // putting back
                }

                return true;
            }

            numVerticesLookedAt++;
        }
        
        // returning everything I popped off
        while (!temperStack.isEmpty())
        {
            CurrentPlan.push(temperStack.pop());
        }
        
        return true;
    }
}
