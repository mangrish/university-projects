public abstract class Event implements Comparable {
    protected int time;
    protected Simulation simulation;

    public Event()
		{}
    public Event(int t)
		{ 
			time = t; 
		}

    public int getTime() 
		{ 
			return time; 
		}
    public void setTime(int t) 
		{ 
			time = t; 
		}

    public Simulation getSimulation() 
		{ 
			return simulation; 
		}
    public void setSimulation(Simulation sim) 
		{ 
			simulation = sim; 
		}

    public void enqueue(Event e) 
		{ 
			simulation.enqueue(e); 
		}

    public abstract void handle();

    public int compareTo(Object o) 
		{
			return time - ((Event)o).time;
    }
}
