/*! \mainpage c-template
 *
 * \section Goal
 * 
 * To minimize the overhead in C development, and give a developer experience similar to Golang.
 * 
 * \section Introduction
 * 
 * `c-template` is a template repository intended to serve as a base layer for easier, and safer C development by encouraging best practices and providing well-tested components.
 * It is intended to be used when starting a new C project by cloning the repository, and starting to work in the cloned repo, although you could copy the functionality into your respective libraries.
 * It provides build management through CMake, unit testing with CMocka, dynamic analysis with Valgrind.
 * 
 * Doxygen is used for parsing your code comments turning it into docuementation with a single command.
 * This encourages writing comments, updating those comments, and having always up to date documentation.
 * 
 * In additional to this, commonly used functionality is included to reduce the need for rewriting functionality in each new library.
 * 
 * \section Reusable-Code Reusable Code
 *  - thread safe logger \link logger.h \endlink
 *    - color coded messages sent to stdout
 *    - optionally writes to a log file as well as sends to stdout
 *  - array length and size checking
 *    - note: doesn't work properly on `char*` data types
 *  - ansi color codes
 *    - includes helper functions to created colored `char*`
 *    - can write to files as well
 *  - random number and string generateion
 *  - CLI template 
 * 
 * \section  Usage
 * 
 * \subsection New-Project New Project
 * 
 * If starting a new project, the best way to use `c-template` is to clone the repo and use it as the base of your new project
 * 
 * \subsection Existing-Project Existing Project
 * 
 * If using an existing project, you'll probably want to juse copy & paste the code and configs in this repo.
 * Eventually installation via `make install` in a manner that places the code into locations like `/usr/include` will eventually be done but is not there yet.
 * 
 * \section Examples
 * 
 * For now you'll want to look at the various `*_test.c` files. Eventually dedicated examples will be written.
 * 
 * 
 *
 * etc...
 */