#include <iostream>
#include <list>
#include <string>
#include <iomanip>

using namespace std;
// we use camel case for the variable and fucntion naming
// look we use each memory size in KB for the whole implementation
int allocationUnit = allocationUnit; // it uses allocationUnit KB allocation unit it may change based on the thing we want
int totalMemorySize = 10240;         // total memory capacity is 10240 KB which is 10 MB

struct MemorySegment
{
    string segmentType;
    string processId;
    int startingAddress;
    int lengthOfSegment;
};

string allocateProcess(MemorySegment process, list<MemorySegment> &memorySegments)
{
    int processTotalSpace = process.lengthOfSegment * allocationUnit;
    auto it = memorySegments.begin();

    while (it != memorySegments.end())
    {
        int holeStart = it->startingAddress;
        int holeSize = it->lengthOfSegment * allocationUnit;
        int holeEnd = holeStart + holeSize;

        if (it->segmentType == "H" &&
            holeStart <= process.startingAddress &&
            (process.startingAddress + processTotalSpace) <= holeEnd)
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
                    int beforeProcessHoleLengthOfSegment = (process.startingAddress - holeStart) / allocationUnit;
                    int afterProcessHoleLengthOfSegment = (holeEnd - (process.startingAddress + processTotalSpace)) / allocationUnit;
                    int afterProcessHoleStartingAddress = process.startingAddress + processTotalSpace;
                    it = memorySegments.insert(it, {"H", "", holeStart, beforeProcessHoleLengthOfSegment});
                    it = memorySegments.insert(it, process);
                    if (afterProcessHoleLengthOfSegment > 0)
                    {
                        memorySegments.insert(it, {"H", "", afterProcessHoleStartingAddress, afterProcessHoleLengthOfSegment});
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

string deAllocateProcess(MemorySegment deProcess, list<MemorySegment> &memorySegments)
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
            if ((hasPrev && prev(it)->segmentType == "H") && (hasNext && next(it)->segmentType == "H"))
            {
                auto leftHole = prev(it);
                auto rightHole = next(it);
                leftHole->lengthOfSegment += (deProcess.lengthOfSegment + rightHole->lengthOfSegment);
                memorySegments.erase(it);
                memorySegments.erase(rightHole);
                return "Process deleted and merged with left and right holes\n";
            }
            else if ((hasPrev && prev(it)->segmentType == "H"))
            {
                auto leftHole = prev(it);
                leftHole->lengthOfSegment += (deProcess.lengthOfSegment);
                memorySegments.erase(it);

                return "Process deleted and merged with left hole\n";
            }
            else if ((hasNext && next(it)->segmentType == "H"))
            {
                auto rightHole = next(it);
                rightHole->lengthOfSegment += (deProcess.lengthOfSegment);
                rightHole->startingAddress = deProcess.startingAddress;
                memorySegments.erase(it);
                return "Process deleted and merged with right hole\n";
            }
            else
            {
                deProcess.processId = "";
                deProcess.segmentType = "H";
                return "Process deleted without merging\n";
            }
        }
    }

    return "No process found with the given ID\n";
}

void displayMemorySegments(list<MemorySegment> &memorySegments)
{
    cout << left << setw(15) << "Type"
         << setw(15) << "Process ID"
         << setw(20) << "Start Address"
         << setw(20) << "Length" << endl;
    cout << string(70, '-') << endl;

    for (const auto &segment : memorySegments)
    {
        cout << setw(15) << (segment.segmentType == "P" ? "Process" : "Hole")
             << setw(15) << (segment.segmentType == "P" ? segment.processId : "-")
             << setw(20) << segment.startingAddress
             << setw(20) << segment.lengthOfSegment << endl;
    }
}
int main()
{

    MemorySegment startingMemory;
    startingMemory.segmentType = "H"; // which identifies as a hole
    startingMemory.processId = "";
    startingMemory.startingAddress = 0;
    startingMemory.lengthOfSegment = 10240 / allocationUnit; // this is because of each segement weights allocationUnitKB

    list<MemorySegment> memorySegments;       // this is alinked list which keep track of which location is assigned a process or it is a hole
    memorySegments.push_back(startingMemory); // we add the starting memory to the list

    cout << "=== Welcome to Memory Management Using a Linked List ===\n\n";
    cout << "Please choose an option:\n";
    cout << "1. Allocate a process\n";
    cout << "2. Deallocate a process\n";
    cout << "3. Show all memory segments with their status\n";
    cout << "0. Exit\n\n";
    cout << "Enter your choice: ";
    int choice;
    cin >> choice;
    while (choice != 0)
    {
        string result;
        MemorySegment process;
        MemorySegment deProcess;
        switch (choice)
        {
        case 1:
            // Allocate a process

            process.segmentType = "P";

            cout << "Enter the process ID: ";
            cin >> process.processId;
            cout << "Enter the starting address of the process you want to allocate: ";
            cin >> process.startingAddress;
            cout << "Enter the length of the process in segments: ";
            cin >> process.lengthOfSegment;
            result = allocateProcess(process, memorySegments);
            cout << result << "\n";
            break;
        case 2:

            deProcess.segmentType = "P";

            cout << "Enter the process ID: ";
            cin >> deProcess.processId;
            cout << "Enter the starting address of the process you want to deallocate: ";
            cin >> deProcess.startingAddress;
            cout << "Enter the length of the process in segments: ";
            cin >> deProcess.lengthOfSegment;
            result = deAllocateProcess(deProcess, memorySegments);
            cout << result << "\n";
            break;
        case 3:
            displayMemorySegments(memorySegments);
            break;
        default:
            cout << "Invalid option. Please select a valid choice from the menu.\n";
            break;
        }

        cout << "\n\nPlease choose an option:\n";
        cout << "1. Allocate a process\n";
        cout << "2. Deallocate a process\n";
        cout << "3. Show all memory segments with their status\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
    }

    return 0;
}