// class : Eat
// handles the event where a patron is seated and has ordered and is now eating

public class Eat extends Event{

	Patron patron;	// a patron
	Patron nextpatron;	// incase next person comes a new paron is required
	private int Wait_time; //the time the patron waited to be served
	private static int Wait_total;//the total time accumulated for waiting

	//the constructer for eat, which takes the current time, and the current patron
	public Eat (int time, Patron p)
	{
		super(time); //adjust the time in Event
		patron = p;
		SimCafe.cook_number--;// a cook is being used	
	}

	public void handle()
	{
		patron.patrons_served++;//add to the number of patrons served
		SimCafe.cook_number++;// a cook is now free

		//the wait time for the patron to get their order
		Wait_time = time -  patron.getSit_time()-patron.getEnter_time();
		Wait_total = Wait_total +Wait_time ;//the accumulated time for wait time
		System.out.println( time + " " + patron.getPatron_num() + " eat "+ Wait_time  );
		//a depart is made and sheduled as soon as the person is finished eating
		DepartCafe departcafe = new DepartCafe(time+patron.getEat_time(), patron );
		enqueue(departcafe);

		if (!SimCafe.cook_queue.isEmpty() )//is the cook queue is not empty
		{
			//dequeue the customer from the queue and schedule an eat event for them
			Patron nextpatron = (Patron) SimCafe.cook_queue.dequeue();
			Eat eat =  new Eat(time+nextpatron.getOrder_time(), nextpatron);
			enqueue(eat);
		}
	
	}
	// GetTime returns the accumulated time patrons had to wait to be served
	public static int GetTime()
	{
		return Wait_total;
	}

}
