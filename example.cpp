#include <boost/heap/fibonacci_heap.hpp>
#include <sstream>
#include <iostream>

using namespace std;

struct node
{
    int id;
int notid;
    node(int i)
      : id(i)
    { }
};

struct compare_node
{
    bool operator()(const node& n1, const node& n2) const
    {
        return n1.id > n2.id;
    }
};

int main()
{
    boost::heap::fibonacci_heap<node, boost::heap::compare<compare_node>> heap;
    boost::heap::fibonacci_heap < node, boost::heap::compare<compare_node>> ::handle_type handle;
    heap.push(node(3));
    node a(2);
    handle=heap.push(a);
    heap.push(node(1));

	stringstream ss;
	ss.str("Example String\n");
	string s;
	s=ss.str();
	cout << s;

    for(const node& n : heap) {
        std::cout << n.id << "\n";
    }

	heap.update(handle,5);

    struct node temp(0);
	while (!heap.empty())
	{
	    temp = heap.top();
	    std::cout << temp.id << "\n";
	    heap.pop();
	}



	std::cout <<"\n" << INFINITY;
}
