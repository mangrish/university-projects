// class SitOrder
// this handles when a customer has sat down and is about to order
class SitOrder extends Event{

	Patron patron; //a patron
	private int Wait_time; //the amount of time the patron waited
	private static int Wait_total;//accumulator of patron wait times
	
	//constructer for SitOrder which takes the current time and the current patron
	public SitOrder( int time, Patron p)
	{
		super(time); //adjust the time in Event
		patron = p;
	}


	public void handle()
	{
		patron.patrons_seated++; //a customer has been seated
		SimCafe.table_number--; //a table has been taken
		patron.setSit_time(time); //remebr what time the individual sat at
		Wait_time = time - patron.getEnter_time();
		Wait_total = Wait_total + Wait_time;
		System.out.println(time + " " + patron.getPatron_num() + " sit " + Wait_time);

		if (SimCafe.cook_number > 0) //if the number of cooks is > 0
		{
			//shedule a eat event
			Eat eat = new Eat(time+patron.getOrder_time(), patron);
			enqueue(eat);
		}
		else
		{
			SimCafe.cook_queue.enqueue(patron); //enquue the patron in the cook queue
		}
	}

	// returns the accumulated wait for seating time
	public static int GetTime()
	{
		return Wait_total;
	}




}

