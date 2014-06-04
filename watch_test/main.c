#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <string.h>

void bzero(void *s, size_t n);

void RefreshDirectory(LPTSTR);
void RefreshTree(LPTSTR);
void WatchDirectory(LPTSTR);

void _tmain(int argc, TCHAR *argv[])
{
    if(argc != 2)
    {
        _tprintf(TEXT("Usage: %s <dir>\n"), argv[0]);
        return;
    }

    WatchDirectory(argv[1]);
}

void WatchDirectory(LPTSTR lpDir)
{
   DWORD dwWaitStatus;
   HANDLE dwChangeHandles[2];
   TCHAR lpDrive[4];
   TCHAR lpFile[_MAX_FNAME];
   TCHAR lpExt[_MAX_EXT];

   //_tsplitpath_s(lpDir, lpDrive, 4, NULL, 0, lpFile, _MAX_FNAME, lpExt, _MAX_EXT);
   _splitpath(lpDir, lpDrive, NULL, lpFile, lpExt);

   lpDrive[2] = (TCHAR)'\\';
   lpDrive[3] = (TCHAR)'\0';
   printf("lpDir=%s\n", lpDir);
   printf("lpDrive=%s\n", lpDrive);
   printf("lpFile=%s\n", lpFile);
   printf("lpExt=%s\n", lpExt);




	HANDLE hDir = CreateFile(
		lpDir,
		FILE_LIST_DIRECTORY,
		FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		NULL
	);

    int nCounter = 0;
    FILE_NOTIFY_INFORMATION strFileNotifyInfo[1024];
    bzero(strFileNotifyInfo, sizeof(strFileNotifyInfo));
    DWORD dwBytesReturned = 0;

    printf("WCHAR=%d\n", sizeof(WCHAR));
    printf("FILE_NOTIFY_INFORMATION=%d\n", sizeof(FILE_NOTIFY_INFORMATION));

    while(TRUE)
    {
        if(ReadDirectoryChangesW(
        	hDir, (LPVOID)strFileNotifyInfo, sizeof(strFileNotifyInfo), TRUE,
        	FILE_NOTIFY_CHANGE_LAST_WRITE, &dwBytesReturned, NULL, NULL) == 0)
        {
            //ErrorCheck(_T("Reading Directory Change"));
            printf("Reading Directory Change");
        } else {
            //_tcout << _T("File Modified: ") << strFileNotifyInfo[0].FileName << endl;
            //_tcout << _T("Loop: ") << nCounter++ << endl;
            char buf[1024];
            memcpy(buf, strFileNotifyInfo[0].FileName, strFileNotifyInfo[0].FileNameLength);
            buf[strFileNotifyInfo[0].FileNameLength] = '\0';
            printf("File Modified: %S\n", (WCHAR*)buf);
            printf("FileNameLength: %d\n", (int) strFileNotifyInfo[0].FileNameLength);
            printf("dwBytesReturned: %d\n", (int) dwBytesReturned);
            printf("Loop: %d\n", nCounter++);
        }
    }





// Watch the directory for file creation and deletion.

   dwChangeHandles[0] = FindFirstChangeNotification(
      lpDir,                         // directory to watch
      TRUE,                         // do not watch subtree
      FILE_NOTIFY_CHANGE_LAST_WRITE); // watch file name changes

   if (dwChangeHandles[0] == INVALID_HANDLE_VALUE)
   {
     printf("\n ERROR: FindFirstChangeNotification function failed.\n");
     ExitProcess(GetLastError());
   }

// Watch the subtree for directory creation and deletion.

   dwChangeHandles[1] = FindFirstChangeNotification(
      lpDrive,                       // directory to watch
      TRUE,                          // watch the subtree
      FILE_NOTIFY_CHANGE_DIR_NAME);  // watch dir name changes

   if (dwChangeHandles[1] == INVALID_HANDLE_VALUE)
   {
     printf("\n ERROR: FindFirstChangeNotification function failed.\n");
     ExitProcess(GetLastError());
   }


// Make a final validation check on our handles.

   if ((dwChangeHandles[0] == NULL) || (dwChangeHandles[1] == NULL))
   {
     printf("\n ERROR: Unexpected NULL from FindFirstChangeNotification.\n");
     ExitProcess(GetLastError());
   }

// Change notification is set. Now wait on both notification
// handles and refresh accordingly.

   while (TRUE)
   {
   // Wait for notification.

      printf("\nWaiting for notification...\n");

      dwWaitStatus = WaitForMultipleObjects(2, dwChangeHandles,
         FALSE, INFINITE);

      switch (dwWaitStatus)
      {
         case WAIT_OBJECT_0:

         // A file was created, renamed, or deleted in the directory.
         // Refresh this directory and restart the notification.

             RefreshDirectory(lpDir);
             if ( FindNextChangeNotification(dwChangeHandles[0]) == FALSE )
             {
               printf("\n ERROR: FindNextChangeNotification function failed.\n");
               ExitProcess(GetLastError());
             }
             break;

         case WAIT_OBJECT_0 + 1:

         // A directory was created, renamed, or deleted.
         // Refresh the tree and restart the notification.

             RefreshTree(lpDrive);
             if (FindNextChangeNotification(dwChangeHandles[1]) == FALSE )
             {
               printf("\n ERROR: FindNextChangeNotification function failed.\n");
               ExitProcess(GetLastError());
             }
             break;

         case WAIT_TIMEOUT:

         // A timeout occurred, this would happen if some value other
         // than INFINITE is used in the Wait call and no changes occur.
         // In a single-threaded environment you might not want an
         // INFINITE wait.

            printf("\nNo changes in the timeout period.\n");
            break;

         default:
            printf("\n ERROR: Unhandled dwWaitStatus.\n");
            ExitProcess(GetLastError());
            break;
      }
   }
}

void RefreshDirectory(LPTSTR lpDir)
{
   // This is where you might place code to refresh your
   // directory listing, but not the subtree because it
   // would not be necessary.

   _tprintf(TEXT("Directory (%s) changed.\n"), lpDir);
}

void RefreshTree(LPTSTR lpDrive)
{
   // This is where you might place code to refresh your
   // directory listing, including the subtree.

   _tprintf(TEXT("Directory tree (%s) changed.\n"), lpDrive);
}