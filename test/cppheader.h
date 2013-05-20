//C++ Header, this is where all the C++ functions are prototyped. 
//
     
// C++ class which holds the member function sayHello();
     
class myTest
{
public:
	void sayHello();
	myTest(){};
};
     
//Purpose of this function is to call sayHello
//This function is also, C compatible. (because we used extern "C")
extern "C" void sayHelloWrapper();
