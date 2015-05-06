// class: ArriveCafe
// creates a new customer and handles when a new customer arrives
import java.util.*;
public class ArriveCafe extends Event{

	//random number generator
	static Random random = new java.util.Random(12345678);

	//final ints for working out times
	public static final int nextpatron_min = 30;
	public static final int nextpatron_max = 300;
	public static final int cook_min = 60; 
	public static final int cook_max = 600;
	public static final int eat_min = 300;
	public static final int eat_max = 3600;

	// variables
	public int nextpatron_time;
	public int order_time;
	public int eat_time;
	Patron patron;
	public static int customer_number = 0;
	public int cust_num;

	//constructer: creates instances of random numbers and makes a new patron
	//it also assigns a patron a number and increments the total number of patrons
	public ArriveCafe(int t) 
	{
		super(t);
		nextpatron_time = (int) (nextpatron_min + random.nextFloat()*(nextpatron_max - nextpatron_min));
		order_time = (int) (cook_min + random.nextFloat()*(cook_max - cook_min));
		eat_time	 = (int) (eat_min + random.nextFloat()*(eat_max - eat_min));
		customer_number++;
		cust_num = customer_number;
		patron = new Patron(time,order_time, eat_time, cust_num);
	}

	public void handle()
	{
		//print message for arrive
		System.out.println( time + " " + patron.getPatron_num() + " arrive" );

		if (SimCafe.table_number > 0) //if number of tables it greater than 0
		{
			SitOrder sitorder = new SitOrder(time, patron);// schedule a sit event 	
			enqueue(sitorder);
		}
		else 
		{
			SimCafe.table_queue.enqueue(patron);//enqueue the patron in the table queue
		}
		//schedule when the next customer is arriving
		ArriveCafe arrivecafe = new ArriveCafe(time+nextpatron_time);
 	  enqueue(arrivecafe); 
	}

}
