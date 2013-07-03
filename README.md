surge
=====

Surge Project. Regarding shortest time path in a Railway Network

I intend to make a code to output the path requiring minimum time in a Railway Network given train time table as an input.
The app wil include an option for the user to place an upper limit to the monetary cost.
I will be using Dijkstra's algorithm and Yen's algorithm for this purpose. For Dijkstra in Fibonacci Heap I am using Boost library.

Compilation command is : g++ -I $BOOSTROOT -o railway.o railway.cpp dijkstra.cpp preprocess.cpp -g -std=c++0x

-g can be omitted. It is only for using GNU debugger.
