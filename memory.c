#include "memory.h"

NTSTATUS ReadPhysicalMemory(ULONGLONG physAddr, PVOID buffer, SIZE_T size)
{
    PHYSICAL_ADDRESS pa;
    pa.QuadPart = physAddr;

    PVOID mapped = MmMapIoSpace(pa, size, MmNonCached);
    if (!mapped)
        return STATUS_UNSUCCESSFUL;

    __try
    {
        RtlCopyMemory(buffer, mapped, size);
    }
    __except (EXCEPTION_EXECUTE_HANDLER)
    {
        MmUnmapIoSpace(mapped, size);
        return STATUS_ACCESS_VIOLATION;
    }

    MmUnmapIoSpace(mapped, size);
    return STATUS_SUCCESS;
}
