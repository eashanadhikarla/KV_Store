#include "common.h"
#include <iostream>
#include <v8.h>
int main(int argc, char **argv) {
	// RAII v8 lock, to prevent multiple threads from entering v8 at the same time
	v8::Locker locker;
	// RAII v8 scope manager, to ensure that all handles get cleaned up
	v8::HandleScope handle_scope;
	// a definition of the global context. Anything in this goes into every
	// context built from it
	v8::Handle<v8::ObjectTemplate> globalTemplate = v8::ObjectTemplate::New();
	// create a context in which to run scripts
	v8::Handle<v8::Context> context = v8::Context::New(NULL, globalTemplate);
	// execute the script that was passed as argv[1]
	if (argc > 1) {
		load_run_script(std::string(argv[1]), context);
	} else {
		std::cout << "Error: you must provide a JavaScript file name as the first "
		<< "argument" << std::endl;
	}
}