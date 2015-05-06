public class Queue extends java.util.Vector {
    public Queue(int cap, int inc) { super(cap, inc); }
    public Queue(int cap) { super(cap); }
    public Queue() { super(); }

    public void enqueue(Object ob) { addElement(ob); }

    public Object dequeue() { return remove(0); }

}
