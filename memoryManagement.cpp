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
    int startingAddress;
    int lengthOfSegment;
};

string allocateProcess(MemorySegment process, list<MemorySegment> memorySegments)
{
    int processTotalSpace = process.lengthOfSegment * 5;
    auto it = memorySegments.begin();
    int insertPos = -1; // used to track the most appropriate postion to insert the process
    while (it != memorySegments.end())
    {
        int holeStart = it->startingAddress;
        int holeSize = it->lengthOfSegment * 5;
        int holeEnd = holeStart + holeSize;

        if (it->segmentType == "H" &&
            holeStart <= process.startingAddress &&
            process.startingAddress + (process.lengthOfSegment * 5) <= holeEnd)
        {
            if (process.startingAddress == holeStart)
            {
                // Shrink the hole from the start
                it->startingAddress += processTotalSpace;
                it->lengthOfSegment -= process.lengthOfSegment;

                memorySegments.insert(it, process);
                return "Process allocated at beginning of hole.";
            }
            else
            {
                if ((holeEnd - process.startingAddress) >= processTotalSpace)
                {
                    int beforeProcessHoleLengthOfSegment = (process.startingAddress - holeStart) / 5;
                    int afterProcessHoleLengthOfSegemnt = (holeEnd - (process.startingAddress + processTotalSpace)) / 5;
                    int afterProcessHoleStartingAddres = process.startingAddress + processTotalSpace;
                    it = memorySegments.insert(it, {"H", "", holeStart, beforeProcessHoleLengthOfSegment});
                    it = memorySegments.insert(it, process);
                    if (afterProcessHoleLengthOfSegemnt > 0)
                    {
                        memorySegments.insert(it, {{"H", "", afterProcessHoleStartingAddres, beforeProcessHoleLengthOfSegment}});
                    }
                    return "Process allocated by splitting the hole.";
                }
                else
                {
                    return "No suitable hole found.";
                }
            }
        }

        ++it;
    }
    return "No suitable hole found.";
}

string deAllocateProcess(MemorySegment deProcess, list<MemorySegment> memorySegments)
{

    bool isFound = false;
    auto start = memorySegments.begin();
    auto end = memorySegments.end();
    for (auto it = memorySegments.begin(); it != memorySegments.end(); ++it)
    {
        if (it->processId == deProcess.processId)
        {
            isFound = true;
            bool hasPrev = it != memorySegments.begin();
            bool hasNext = std::next(it) != memorySegments.end();
        }
    }

    return "";
}
int main()
{

    MemorySegment startingMemory;
    startingMemory.segmentType = "H"; // which identifies as a hole
    startingMemory.processId = "";
    startingMemory.startingAddress = 0;
    startingMemory.lengthOfSegment = 10240 / 5; // this is because of each segement weights 5KB

    list<MemorySegment> memorySegments; // this is alinked list which keep track of which location is assigned a process or it is a hole

    cout << "=== Welcome to Memory Management Using a Linked List ===\n\n";
    cout << "Please choose an option:\n";
    cout << "1. Allocate a process\n";
    cout << "2. Deallocate a process\n";
    cout << "3. Show all memory segments with their status\n\n";
    cout << "Enter your choice: ";
    int choice;
    cin >> choice;
    while (choice != 0)
    {
        switch (choice)
        {
        case 1:
            // Allocate a process
            MemorySegment process;
            process.segmentType = "P";

            cout << "Enter the process ID: ";
            cin >> process.processId;
            cout << "Enter the starting address of the process you want to allocate: ";
            cin >> process.startingAddress;
            cout << "Enter the length of the process in segments: ";
            cin >> process.lengthOfSegment;
            string result = allocateProcess(process, memorySegments);
            cout << result << "\n";
            break;
        case 2:
            MemorySegment deProcess;
            deProcess.segmentType = "P";

            cout << "Enter the process ID: ";
            cin >> deProcess.processId;
            cout << "Enter the starting address of the process you want to deallocate: ";
            cin >> deProcess.startingAddress;
            cout << "Enter the length of the process in segments: ";
            cin >> deProcess.lengthOfSegment;
            string result = deAllocateProcess(deProcess, memorySegments);
            cout << result << "\n";
            break;
        }
    }
}