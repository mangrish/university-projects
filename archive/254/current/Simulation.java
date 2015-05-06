public class Simulation extends PriorityQueue {
    private int currTime = 0;

    public int getTime() { return currTime; }

    public void enqueue(Event e) {
	if (e.getTime() < currTime) 
	    throw new IllegalArgumentException("Event scheduled for the past");
	e.setSimulation(this);
	super.enqueue(e);
    }

    public void run(int duration) {
	Event e;

	while (!isEmpty()
	       && (e=(Event)dequeue()).getTime() < duration) {
	    currTime = e.getTime();
//	    System.out.println(currTime + " handling " + e);
	    e.handle();
			
	}
    }
}
