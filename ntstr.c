#define STRICT
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>
#pragma warning(disable : 4255 4820)
#include <winternl.h>
#pragma warning(default : 4255 4820)

#define STATUS_SUCCESS (0x00000000)
#define MILLISECONDS_FROM_100NANOSECONDS(durationNanoS) ((durationNanoS) / (1000.0 * 10.0))

NTSYSAPI
NTSTATUS
NTAPI
NtSetTimerResolution(
  ULONG   DesiredResolution,
  BOOLEAN SetResolution,
  PULONG  CurrentResolution
);

int
  _tmain(
    int      argc, 
    _TCHAR** argv)
{
  ULONG _;
  ULONG ulMinimumResolution;
  ULONG ulMaximumResolution;
  ULONG ulCurrentResolution;

  RtlSecureZeroMemory(&_, sizeof(ULONG));
  RtlSecureZeroMemory(&ulMinimumResolution, sizeof(ULONG));
  RtlSecureZeroMemory(&ulMaximumResolution, sizeof(ULONG));
  RtlSecureZeroMemory(&ulCurrentResolution, sizeof(ULONG));

  if(STATUS_SUCCESS == NtQueryTimerResolution(
    &ulMinimumResolution,
    &ulMaximumResolution,
    &ulCurrentResolution))
    _tprintf(
      _T("[%.3lf ms, %.3lf ms, %.3lf ms]"),
      MILLISECONDS_FROM_100NANOSECONDS(ulMaximumResolution),
      MILLISECONDS_FROM_100NANOSECONDS(ulCurrentResolution),
      MILLISECONDS_FROM_100NANOSECONDS(ulMinimumResolution)
    );
  if(argc == 2)
    if(STATUS_SUCCESS == NtSetTimerResolution(
      (ULONG)_ttoi(argv[1]), 
      (BOOLEAN)!!TRUE, 
      &_))
    {
      _tprintf(
        _T(" %.3lf ms => %.3lf ms"),
        MILLISECONDS_FROM_100NANOSECONDS(ulCurrentResolution),
        MILLISECONDS_FROM_100NANOSECONDS((ULONG)_ttoi(argv[1]))
      );
      SuspendThread(GetCurrentThread());
    }
  ExitProcess(0);
}
