Attribute VB_Name = "Module3"



Private Declare Function GetTickCount Lib "kernel32" () As Long




Function FileExists(directory As String, filename As String) As Boolean
Dim MyFile As String
FileExists = False
MyFile = Dir(directory & filename)
If MyFile = filename Then
FileExists = True
End If
End Function




Public Sub SlowDown(MilliSeconds As Long)

Dim lngTickStore As Long

lngTickStore = GetTickCount()

Do While lngTickStore + MilliSeconds > GetTickCount()
DoEvents
Loop

End Sub
