#pragma once

class ReturnMutex
{
public:
    bool WasLockedOnConstruct = false;
    bool IsLocked = false;

public:
    ReturnMutex()
    {
        if (IsLocked)
        {
            WasLockedOnConstruct = true;
        }
        else
        {
            IsLocked = true;
            WasLockedOnConstruct = false;
        };
    }

    ~ReturnMutex()
    {
        if (WasLockedOnConstruct == false)
        {
            IsLocked = false;
        }
    }

    bool ShouldReturn()
    {
        return WasLockedOnConstruct;
    }

};

class ReturnLock
{
private:
    ReturnMutex* Mutex;

public:
    ReturnLock(ReturnMutex* Mutex)
    {
        this->Mutex = Mutex;

        if (Mutex)
        {
            if (Mutex->IsLocked)
            {
                this->Mutex = nullptr;
            }
            else
            {
                Mutex->IsLocked = true;
            }
        }
    }

    ~ReturnLock()
    {
        if (Mutex)
        {
            Mutex->IsLocked = false;
        }
    }

};