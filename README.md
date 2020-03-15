
# ways of calculating
	recursive type all the way - serial -- Brady
	formala all the way - serial -- Nick
	multi thread formula start, recurse the rest -- for chunks larger than 2 -- Brady
	multi thread formual all of them.

# Splitting
	Different numbers of threads.

# Different ways to chunk.
	Large blocks, -- Nick
	Multiple blocks, -- Nick
	Every nth number. -- Brady

# Find a way to include bigint and bigfloat. -- Brady

	To include the boost library for large floating point numbers:
		go here and download the archive. https://www.boost.org/users/download/
		extract the archive and inside copy the first folder boost.
		paste this folder into the directory /usr/local/include
		From there you can use #include <boost/*> and all entire library is available to you.

	BigInt is just a header file already set up in the project.



# Find a way to pass bigInt through mpi system -- Brady
	maybe and char array, 
	maybe as int array.
