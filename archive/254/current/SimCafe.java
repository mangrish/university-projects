// class: SimCafe
// this class calls main 
public class SimCafe {

	//variable declarations
	public static int table_number;
	public static int cook_number;
	public static int sim_length;
	public static int WaitTime;
	public static int ServeTime;
	public static int NotSatTime;
	public static int NotServeTime;

	static Queue	table_queue;
	static Queue cook_queue;
	static Simulation sim;
	static Patron Temp_patron;	


	public static void main(String[] argv)
	{
		if (argv.length != 3)//if the wrong format is recieved
		{
			error_handle(); //handle the error
			System.exit(1);
		}

		else
		{
			table_number = Integer.parseInt(argv[0]);
			cook_number  = Integer.parseInt(argv[1]);
			sim_length   = Integer.parseInt(argv[2]);

			table_queue = new Queue();
			cook_queue	=	new Queue();
			//set up the simulation and set the ball rolling!
			sim = new Simulation();
			ArriveCafe arrive = new ArriveCafe(0);
			sim.enqueue(arrive);
			sim.run( sim_length );

			//create an imaginary patron to count how long each person was in a table queue for
			while (!table_queue.isEmpty() )
			{
				Temp_patron = (Patron) table_queue.dequeue();
				NotSatTime = NotSatTime + (sim_length - Temp_patron.getEnter_time() );
			}
			//create an imagainary patron to count how long a person was in the cook queue
			//for 
			while (!cook_queue.isEmpty() )
			{
				Temp_patron = (Patron) cook_queue.dequeue();
				NotServeTime = NotServeTime + (sim_length - Temp_patron.getSit_time());
			}

			//print the statistics
			print_statistics();
		}
	}
	//print_statistics prints all the required statics at the end of the simulation
	private static void print_statistics()
	{
		WaitTime = SitOrder.GetTime() ;
		ServeTime = Eat.GetTime() ;

		System.out.println("");
		System.out.println("Arrived: " +  (ArriveCafe.customer_number - 1)  );
		System.out.println("Seated: " + Patron.patrons_seated  );
		System.out.println("Served: " + Patron.patrons_served  );
		System.out.println("Wait time: " + (int) ((WaitTime+ NotSatTime)/(ArriveCafe.customer_number - 1)) );
		System.out.println("Serve time: " + ((ServeTime + (NotServeTime-1) )/(ArriveCafe.customer_number - 1))  );
	}

	//error_handle displays  a helpful error message
	private static void error_handle()
	{
		System.out.println("Usage: kaffe SimCafe #table #cooks #simulation_time");
	}

}
