//***********************************************************************************************************************************//
	Name: Rubaiyat Rashid
	Professor: Ioannis Stamos
	Class: CSCI 335 
	Purpose: Assignment #4
	Contents: The .zip should contain: QuadraticProbing.h, Priority.h, Dsexceptions.h, 
 		     TestInsertAndSearch.cpp, TestInsertAndDelete.cpp, TestNewInsert.cpp
	NOTE: QuadraticProbing.h was taken from https://users.cs.fiu.edu/~weiss/dsaa_c++4/code/  
	      PriorityQueue.h was originally named BinomialQueue.h and was taken from https://users.cs.fiu.edu/~weiss/dsaa_c++4/code/ 
//**********************************************************************************************************************************//

I. Parts Completed
	- All parts of this assignment were completed 

II. Bugs Encountered
	- Many bugs were encountered throughout this project.I was getting segmentation faults for part 2 because 
	  I was not changing pointers in the Hash table

III. Instructions on Running program
	- Compilation: ‘make’ or ‘make all’
	- Run:
		<> make run1a to test part 1 with 100_numbers.txt and 100_test_search.txt
		<> make run1b to test part 1 with 1000_numbers.txt and 1000_test_search.txt
		<> make run1c to test part 1 with 10000_numbers.txt and 10000_test_search.txt
		
		<> make run2a to test part 2 with 100_numbers.txt and 100_test_deletion.txt
		<> make run2b to test part 2 with 1000_numbers.txt and 1000_test_deletion.txt
		<> make run2c to test part 2 with 10000_numbers.txt and 10000_test_deletion.txt
	
		<> make run3a to test part 3 with 100_numbers.txt and 100_test_search.txt
		<> make run3b to test part 3 with 1000_numbers.txt and 1000_test_search.txt
		<> make run3c to test part 3 with 10000_numbers.txt and 10000_test_search.txt
		
	- Clean:
		<> make clean - cleans TestInsertAndSearch.o, TestInsertAndSearch,  TestInsertAndDelete.o, TestInsertAndDelete , 
			TestNewInsert.o, and TestNewInsert

IV. I/O files
	- Part 1: (i) 100_numbers.txt and 100_test_search.txt or (ii) 1000_numbers.txt and 1000_test_search.txt or 
		  (iii) 10000_numbers.txt and 10000_test_search.txt

	- Part 2: (i) 100_numbers.txt and 100_test_deletion.txt or (ii) 1000_numbers.txt and 1000_test_deletion.txt or 
		  (iii) 10000_numbers.txt and 10000_test_deletion.txt

	- Part 1: (i) 100_numbers.txt and 100_test_search.txt or (ii) 1000_numbers.txt and 1000_test_search.txt or 
		  (iii) 10000_numbers.txt and 10000_test_search.txt