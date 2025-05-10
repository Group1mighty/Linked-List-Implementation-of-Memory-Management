#include <iostream>
#include <list>
#include <string>

using namespace std;
// we use camel case for the variable and fucntion naming
// look we use each memory size in KB for the whole implementation
int alocationUnit = 5;       // it uses 5 KB allocation unit it may change based on the thing we want
int totalMemorySize = 10240; // total memory capacity is 10240 KB which is 10 MB

struct MemorySegment
{
    string segmentType;
    string processId;
    int startingAdress;
    int lengthOfSegment;
};

int main()
{

    MemorySegment startingMemory;
    startingMemory.segmentType = "H"; // which identifies as a hole
    startingMemory.processId = "";
    startingMemory.startingAdress = 0;
    startingMemory.lengthOfSegment = 10240 / 5; // this is because of each segement weights 5KB

    list<MemorySegment> memorySegments; // this is alinked list which keep track of which location is assigned a process or it is a hole

    cout << "=== Welcome to Memory Management Using a Linked List ===\n\n";
    cout << "Please choose an option:\n";
    cout << "1. Allocate a process\n";
    cout << "2. Deallocate a process\n";
    cout << "3. Show all memory segments with their status\n\n";
}
