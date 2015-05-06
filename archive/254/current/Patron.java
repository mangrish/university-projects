//class: Patron
// this holds details for each individual customer about when they arrived
// ate etc... , and hold details of the total number of people seated etc..
public class Patron{

	//private variables for the patron
	private int enter_time;
	private int sit_time;
	private int cust_num;
	private int order_time;
	private int eat_time;
	//public variables to keep track of the total number of patrons seated and served
	public static int patrons_seated = 0;
	public static int patrons_served = 0;

	//constructer which assigns values to the private variables
	public Patron(int currTime, int o_time, int e_time, int customer_num)  
	{
		cust_num = customer_num;
		enter_time = currTime;
		order_time = o_time;
		eat_time = e_time;
	}

	//returns the customer number of a patron	
	public int getPatron_num()
	{
		return cust_num;
	}

	// returns how long a customer had to wait for their order
	public int getOrder_time()
	{
		return order_time;
	}

	// sets the time a sit occured
	public void  setSit_time(int time)
	{
		sit_time = time - enter_time;	
	}

	// returns the customers sit time
	public int getSit_time()
	{
		return sit_time;
	}

	//returns the customers entry to the cafe time
	public int getEnter_time()
	{
		return enter_time;
	}
	
	//returns how long a customer ate for
	public int getEat_time()
	{
		return eat_time;
	}

	
}
