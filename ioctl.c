#include "ioctl.h"
#include "memory.h"
#include "shared.h"

VOID EvtIoDeviceControl(
    WDFQUEUE Queue,
    WDFREQUEST Request,
    size_t OutputBufferLength,
    size_t InputBufferLength,
    ULONG IoControlCode)
{
    UNREFERENCED_PARAMETER(Queue);

    NTSTATUS status = STATUS_SUCCESS;
    size_t bytesReturned = 0;

    if (IoControlCode == IOCTL_PING)
    {
        DbgPrint("Driver ready\n");
    }
    else if (IoControlCode == IOCTL_READ_PHYS)
    {
        PPHYS_READ_REQUEST req;
        PVOID outBuffer;

        status = WdfRequestRetrieveInputBuffer(Request, sizeof(PHYS_READ_REQUEST), &req, NULL);
        if (!NT_SUCCESS(status)) goto end;

        status = WdfRequestRetrieveOutputBuffer(Request, req->Size, &outBuffer, NULL);
        if (!NT_SUCCESS(status)) goto end;

        status = ReadPhysicalMemory(req->PhysicalAddress, outBuffer, req->Size);
        if (NT_SUCCESS(status))
            bytesReturned = req->Size;
    }

end:
    WdfRequestCompleteWithInformation(Request, status, bytesReturned);
}
