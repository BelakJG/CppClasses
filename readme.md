# CPP Classes

This is just a repo for me to store custom classes I might use e.g. an array class where I create it with size known during runtime so it already handles memory management

RuntimeArray: Lets you create an array with a fixed size at runtime. Useful for when you want to create the array based on a user input or by a variable that is unknown at
    compile and you want it to stay fixed. Automatically handles memory so you don't have to clear raw pointers.

ThreadPool: Creates a pool of threads and will build a queue of functions with any return type and process them automatically. Has functionality to pause the main thread to
    keep everything in sync as well as graceful termination of threads and resizing.