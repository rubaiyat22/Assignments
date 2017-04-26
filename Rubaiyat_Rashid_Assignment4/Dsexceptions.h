#ifndef DS_EXCEPTIONS_H
#define DS_EXCEPTIONS_H

//#include <string>

class UnderflowException {
public:
	UnderflowException( const std::string & message)
	{
		std::cout << message << std::endl;
		abort();
	}

};
class IllegalArgumentException { };
class ArrayIndexOutOfBoundsException { };
class IteratorOutOfBoundsException { };
class IteratorMismatchException { };
class IteratorUninitializedException { };

#endif