#pragma once
///
///  Status of RWLock
enum RWLOCK_STATUS {
	RW_STATUS_IDLE,
	RW_STATUS_READ,
	RW_STATUS_WRITE
};

/// <summary>
/// Class RWLock - Secure read and write to memory
///	    A singleton class that secures access to memory
///     Allows multiple read access.  but only one write access after all posted
///     reads are completed.
/// </summary>
class RWLock
{
public:
	static RWLock* Instance();


	void ReadLock();
	void ReadUnlock();
	void WriteLock();
	void WriteUnlock();

protected:
	int m_iReadCount;
	enum RWLOCK_STATUS m_status;
	CRITICAL_SECTION m_cs;

	RWLock();

private:
	static RWLock* m_Instance;
};


