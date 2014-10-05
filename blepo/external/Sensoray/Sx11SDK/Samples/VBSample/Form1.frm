VERSION 5.00
Begin VB.Form Form1 
   AutoRedraw      =   -1  'True
   Caption         =   "GraBiT"
   ClientHeight    =   7095
   ClientLeft      =   165
   ClientTop       =   855
   ClientWidth     =   9525
   ControlBox      =   0   'False
   Icon            =   "Form1.frx":0000
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   ScaleHeight     =   473
   ScaleMode       =   3  'Pixel
   ScaleWidth      =   635
   StartUpPosition =   3  'Windows Default
   Begin VB.PictureBox Picture1 
      AutoRedraw      =   -1  'True
      Height          =   7335
      Left            =   0
      ScaleHeight     =   485
      ScaleMode       =   3  'Pixel
      ScaleWidth      =   653
      TabIndex        =   0
      Top             =   0
      Width           =   9855
      Begin VB.Timer start_timer 
         Enabled         =   0   'False
         Interval        =   100
         Left            =   8160
         Top             =   120
      End
   End
   Begin VB.Menu File 
      Caption         =   "&File"
      Begin VB.Menu Exit 
         Caption         =   "E&xit"
      End
   End
   Begin VB.Menu Channel 
      Caption         =   "&Channel"
      Begin VB.Menu SVideo 
         Caption         =   "&S-video"
         Checked         =   -1  'True
      End
      Begin VB.Menu Video1 
         Caption         =   "Video&1"
      End
      Begin VB.Menu Video2 
         Caption         =   "Video&2"
      End
      Begin VB.Menu Video3 
         Caption         =   "Video&3"
      End
      Begin VB.Menu Video4 
         Caption         =   "Video&4"
      End
   End
   Begin VB.Menu capture 
      Caption         =   "&Capture"
      Begin VB.Menu menustart 
         Caption         =   "&Start"
      End
      Begin VB.Menu menustop 
         Caption         =   "St&op"
      End
      Begin VB.Menu menudir 
         Caption         =   "Settings"
      End
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Exit_Click()
    stopflag = True
End Sub

Private Sub Form_Load()
capture_directory = ""
capture_on_flag = False
seq_title = "frame-"
max_frames = 100
n_frame = 0

End Sub

Private Sub menudir_Click()
Form2.Show
End Sub

Private Sub menustart_Click()
n_frame = 0
menustart.Enabled = False
menudir.Enabled = False
menustop.Enabled = True
start_timer.Enabled = True
capture_on_flag = True
Form1.Picture1.Visible = False
'Form1.Picture1.AutoRedraw = False
End Sub

Private Sub menustop_Click()
capture_on_flag = False
start_timer.Enabled = False
menustart.Enabled = True
menudir.Enabled = True
menustop.Enabled = False
Form1.Picture1.Visible = True
'Form1.Picture1.AutoRedraw = True
End Sub

Private Sub start_timer_Timer()

If capture_directory = "" Then
capture_directory = "C:\Temp"
End If

Dim capture_file As String
Dim i As Integer
Dim length As Integer
capture_file = right(Str(n_frame), Len(Str(n_frame)) - 1)
length = Len(capture_file)
For i = 1 To 4 - length
capture_file = "0" & capture_file
Next i

'capture_file = Str(n_frame)

capture_file = seq_title & capture_file & ".bmp"

'If FileExists(capture_directory, capture_file) Then
'Kill capture_directory & "\" & capture_file
'End If

SavePicture Picture1.Image, capture_directory & "\" & capture_file

If n_frame >= max_frames Then
start_timer.Enabled = False
menustart.Enabled = True
menudir.Enabled = True
menustop.Enabled = False
Form1.Picture1.Visible = True
capture_on_flag = False
End If

n_frame = n_frame + 1

End Sub

Private Sub SVideo_Click()
    fgmode.input = MUX_0
    fgmode.advanced.LNOTCH = LNOTCH_OFF
    fgmode.advanced.CCOMB = CCOMB_OFF
    ecode = X11_SetMode(hfg, fgmode)
    If ecode <> 0 Then
        Call ErrorHandler(ecode)
    End If
    SVideo.Checked = True
    Video1.Checked = False
    Video2.Checked = False
    Video3.Checked = False
    Video4.Checked = False
End Sub


Private Sub Video1_Click()
    fgmode.input = MUX_1
    fgmode.advanced.LNOTCH = LNOTCH_ON
    fgmode.advanced.CCOMB = CCOMB_ON
    ecode = X11_SetMode(hfg, fgmode)
    If ecode <> 0 Then
        Call ErrorHandler(ecode)
    End If
    SVideo.Checked = False
    Video1.Checked = True
    Video2.Checked = False
    Video3.Checked = False
    Video4.Checked = False
End Sub

Private Sub Video2_Click()
    fgmode.input = MUX_2
    fgmode.advanced.LNOTCH = LNOTCH_ON
    fgmode.advanced.CCOMB = CCOMB_ON
    ecode = X11_SetMode(hfg, fgmode)
    If ecode <> 0 Then
        Call ErrorHandler(ecode)
    End If
    SVideo.Checked = False
    Video1.Checked = False
    Video2.Checked = True
    Video3.Checked = False
    Video4.Checked = False
End Sub

Private Sub Video3_Click()
    fgmode.input = MUX_3
    fgmode.advanced.LNOTCH = LNOTCH_ON
    fgmode.advanced.CCOMB = CCOMB_ON
    ecode = X11_SetMode(hfg, fgmode)
    If ecode <> 0 Then
        Call ErrorHandler(ecode)
    End If
    SVideo.Checked = False
    Video1.Checked = False
    Video2.Checked = False
    Video3.Checked = True
    Video4.Checked = False
End Sub

Private Sub Video4_Click()
    fgmode.input = MUX_4
    fgmode.advanced.LNOTCH = LNOTCH_ON
    fgmode.advanced.CCOMB = CCOMB_ON
    ecode = X11_SetMode(hfg, fgmode)
    If ecode <> 0 Then
        Call ErrorHandler(ecode)
    End If
    SVideo.Checked = False
    Video1.Checked = False
    Video2.Checked = False
    Video3.Checked = False
    Video4.Checked = True
End Sub
