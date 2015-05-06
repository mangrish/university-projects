// class: DepartCafe
// does the handling for the event where a patron is leaving the cafe

public class DepartCafe extends Event{

	Patron patron; // the patron being passed in
	Patron nextpatron; //if the table queue is dequeued, than a next patron is needed

	//the constructor for departcafe, which takes the current time and the patron
	public DepartCafe ( int time, Patron p)
	{
		super(time);//adjust time in Event
		patron = p;
	}

	public void handle()
	{
		//print out for depart and calculation for depart
		System.out.println(time +" "  + patron.getPatron_num()+ " depart " + (time - patron.getSit_time()- patron.getEnter_time()) ); 
		SimCafe.table_number++;//a table is now free 
		if (!SimCafe.table_queue.isEmpty() )//if the table queue is not empty
		{
			Patron nextpatron = (Patron) SimCafe.table_queue.dequeue();//get the next patron from the queue
			//dont need 2 increment as table will b occupied again
			SitOrder sitorder = new SitOrder(time, nextpatron);//create a new sit event	
			sitorder.handle();//send it to SitOrder to handle the scheduling
			
		}

	}
}
