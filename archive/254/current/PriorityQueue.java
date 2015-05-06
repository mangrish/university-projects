public class PriorityQueue {
    
    protected Comparable[] data;
    protected int size = 0;

    public PriorityQueue() { this(16); }
    public PriorityQueue(int cap) { data = new Comparable[cap]; }

    public int getSize() { return size; }
    public boolean isEmpty() { return size == 0; }
    public int getCapacity() { return data.length; }

    public void enqueue(Comparable ob) {
	if (size == data.length) {
	    Comparable[] newdata = new Comparable[size*2];
	    for (int i=0; i<size; ++i) newdata[i] = data[i];
	    data = newdata;
	}
	floatUp(ob, size++);
    }

    public Object dequeue() { 
	if (size == 0) throw new java.util.NoSuchElementException();
	--size;				
	Object result = data[0];	
	int i = 0;
	for (int next=1; next<=size; i=next, next=2*i+1) {
	    if (next < size && data[next].compareTo(data[next+1]) > 0) ++next;
	    data[i] = data[next];
	}
	if (i < size) {
	    floatUp(data[size], i);
	}
	return result; 
    }

    protected void floatUp(Comparable ob, int i) {
	for (int next; i > 0; i=next) {
	    next = (i-1)/2;
	    if (ob.compareTo(data[next]) >= 0) break;
	    data[i] = data[next];
	}
	data[i] = ob;
    }


    private void print() {
	System.out.print("{");
	for (int i=0; i<size; ++i) {
	    if (i>0) System.out.print(", ");
	    System.out.print(data[i]);
	}
	System.out.println("}");
    }

    private static void main(String[] argv) {
	PriorityQueue q = new PriorityQueue(4); // test auto extension
	final Integer data[] = {new Integer(4), new Integer(3), 
				new Integer(6), new Integer(2), 
				new Integer(9), new Integer(5),
				new Integer(2), new Integer(1),
				new Integer(7), new Integer(4)};
	for (int i = 0; i<data.length; ++i) {
	    q.enqueue(data[i]);
	    q.print();
	}
	while (!q.isEmpty()) {
	    System.out.println(q.dequeue());
	    q.print();
	}
    }

}
