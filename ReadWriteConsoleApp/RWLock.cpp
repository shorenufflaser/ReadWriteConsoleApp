#include "stdafx.h"
#include "RWLock.h"


// This is a simple design Using Critical Sections to protect state and counters
// It uses Sleep to allow thread processing to continue for other threads.
// A more robust system would use events to pause threads not granted immediate access for writes



RWLock* RWLock::m_Instance = nullptr;

RWLock::RWLock()
{
	InitializeCriticalSection(&m_cs);
	EnterCriticalSection(&m_cs);
	m_iReadCount = 0;
	m_status = RW_STATUS_IDLE;
	LeaveCriticalSection(&m_cs);
}

RWLock* RWLock::Instance() {
	if (m_Instance == nullptr)
		m_Instance = new RWLock;
	return m_Instance;
}


/// <summary>
/// ReadLock - Ask for read access.
///    If writing wait till write complete.
///    If reading, grant access immediately
/// </summary>
/// <returns> void</returns>
void RWLock::ReadLock()
{
	EnterCriticalSection(&m_cs);
	while (m_status == RW_STATUS_WRITE) {
		LeaveCriticalSection(&m_cs);
		Sleep(1);
		EnterCriticalSection(&m_cs);
	}
	if (m_status == RW_STATUS_IDLE || m_status == RW_STATUS_READ) {
		m_status = RW_STATUS_READ;
		m_iReadCount++;
	}
	LeaveCriticalSection(&m_cs);
}

/// <summary>
/// ReadUnlock - Release read access
/// </summary>
/// <returns> void</returns>
void RWLock::ReadUnlock()
{
	EnterCriticalSection(&m_cs);
	while (m_status == RW_STATUS_WRITE) {
		LeaveCriticalSection(&m_cs);
		Sleep(1);
		EnterCriticalSection(&m_cs);
	}
	m_iReadCount--;
	if (m_iReadCount <= 0)
		m_status = RW_STATUS_IDLE;
	LeaveCriticalSection(&m_cs);
}

/// <summary>
/// WriteLock - Ask for write access
///     If reading or writing wait until all reads or a write is complete
/// </summary>
/// <returns> void</returns>
void RWLock::WriteLock()
{
	EnterCriticalSection(&m_cs);
	while (m_status != RW_STATUS_IDLE)
	{
		LeaveCriticalSection(&m_cs);
		Sleep(1);
		EnterCriticalSection(&m_cs);
	}
	m_status = RW_STATUS_WRITE;
	LeaveCriticalSection(&m_cs);
}


/// <summary>
/// WriteUnlock - Release write access
/// </summary>
/// <returns> void</returns>
void RWLock::WriteUnlock()
{
	EnterCriticalSection(&m_cs);
	m_status = RW_STATUS_IDLE;
	LeaveCriticalSection(&m_cs);
}

