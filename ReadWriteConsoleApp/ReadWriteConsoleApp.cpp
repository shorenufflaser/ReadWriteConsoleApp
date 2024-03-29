// ReadWriteConsoleApp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <math.h>
#include <time.h>  
#include <iostream>

#include "RWLock.h"

using namespace std;

typedef struct tgThreadInfo {
	int iThreadNumber;
	int iRand;
} THREADINFO_TYPE;


#define MAX_THREADS 10

RWLock * g_pLock;
RWLock * g_pLockII;
HANDLE g_hExit;

/// <summary>
/// RWLockThreadFunction - Thread to either read or write to memory
///     The data passed in is the thread ID number and a number to 
///     determine the quantity of writes or reads to perform in immediate succession
/// </summary>
/// <param name = "lpParam">a pointer to a THREADINFO_TYPE </param>
/// <returns> the quantity if reads or writes in succession</returns>
DWORD WINAPI RWLockThreadFunction(LPVOID lpParam)
{
	THREADINFO_TYPE* piThreadNumber = (THREADINFO_TYPE *) lpParam;
	int iCount;
	
	iCount = 10 + piThreadNumber->iRand;
	do {
		if (iCount % 2 > 0) {
			for (int i = 0; i < iCount; i++) {
				g_pLock->WriteLock();
				cout << "thread [" << piThreadNumber->iThreadNumber << "]WriteLock in->" << endl;
				Sleep(iCount);
				g_pLock->WriteUnlock();
				cout << "<- thread WriteUnlock out" << endl;
			}
		}
		else {
			for (int i = 0; i < iCount; i++) {
				cout << "                              thread [" << piThreadNumber->iThreadNumber << "]ReadLock in->" << endl;
				g_pLock->ReadLock();
				Sleep(iCount);
				g_pLock->ReadUnlock();
				cout << "                              thread ReadUnlock in->" << endl;
			}
		}
	} while (WaitForSingleObject(g_hExit, 0) != WAIT_OBJECT_0);
	ResetEvent(g_hExit);
	return (DWORD)iCount;
}


int main()
{
	int i;
	HANDLE  hThreadArray[MAX_THREADS];
	DWORD   dwThreadIdArray[MAX_THREADS];
	THREADINFO_TYPE		threadNumber[MAX_THREADS];

	cout << "Hello" << endl;

	g_hExit = CreateEvent(nullptr, TRUE, FALSE, nullptr);

	g_pLock = RWLock::Instance();
	g_pLockII = RWLock::Instance();

	_ASSERT(g_pLock == g_pLockII);				// Test for singleton
// Only across threads for the application
// Multiple instances of this app would create
// Multiple instances of RWLock
// To insure a single instance accross the OS
// use a global named mutex

	srand((unsigned)time(NULL));

	// Create Threads to test the RWLock functionality.
	// This is a non-robust, very simple test to show the read and write lock / unlock functions work
	// 
	for (int i = 0; i < MAX_THREADS; i++) {
		threadNumber[i].iThreadNumber = i;
		threadNumber[i].iRand = rand() % 10;
		hThreadArray[i] = CreateThread(
			NULL,                   // default security attributes
			0,                      // use default stack size  
			RWLockThreadFunction,       // thread function name
			&threadNumber[i],          // argument to thread function 
			0,                      // use default creation flags 
			&dwThreadIdArray[i]);   // returns the thread identifier 

		if (hThreadArray[i] == NULL) {
			cout << "Error creating threads" << endl;
			cin >> i;
			exit(3);
		}
	}

	cin >> i;
	SetEvent(g_hExit);

    return 0;
}

