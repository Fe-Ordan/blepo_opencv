VERSION 5.00
Begin VB.Form Form2 
   BorderStyle     =   3  'Fixed Dialog
   Caption         =   "Choose Output Directory"
   ClientHeight    =   4800
   ClientLeft      =   45
   ClientTop       =   435
   ClientWidth     =   6000
   ControlBox      =   0   'False
   LinkTopic       =   "Form2"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4800
   ScaleWidth      =   6000
   ShowInTaskbar   =   0   'False
   StartUpPosition =   3  'Windows Default
   Begin VB.TextBox Text4 
      Height          =   285
      Left            =   4920
      TabIndex        =   8
      Top             =   1440
      Width           =   615
   End
   Begin VB.TextBox Text3 
      Height          =   285
      Left            =   4080
      TabIndex        =   7
      Top             =   2640
      Width           =   1455
   End
   Begin VB.TextBox Text2 
      Height          =   285
      Left            =   4920
      TabIndex        =   4
      Top             =   840
      Width           =   615
   End
   Begin VB.DirListBox Dir1 
      Height          =   3690
      Left            =   120
      TabIndex        =   3
      Top             =   240
      Width           =   3615
   End
   Begin VB.CommandButton Command3 
      Caption         =   "Cancel"
      Height          =   495
      Left            =   3960
      TabIndex        =   2
      Top             =   3960
      Width           =   1575
   End
   Begin VB.CommandButton Command2 
      Caption         =   "OK"
      Height          =   495
      Left            =   3960
      TabIndex        =   1
      Top             =   3360
      Width           =   1575
   End
   Begin VB.TextBox Text1 
      Height          =   375
      Left            =   120
      Locked          =   -1  'True
      TabIndex        =   0
      TabStop         =   0   'False
      Top             =   4080
      Width           =   3615
   End
   Begin VB.Label Label3 
      Caption         =   "Max Frames"
      Height          =   255
      Left            =   3960
      TabIndex        =   9
      Top             =   1440
      Width           =   855
   End
   Begin VB.Label Label2 
      Caption         =   "Sequence Title"
      Height          =   255
      Left            =   4080
      TabIndex        =   6
      Top             =   2400
      Width           =   1455
   End
   Begin VB.Label Label1 
      Caption         =   "Delay (ms)"
      Height          =   255
      Left            =   4080
      TabIndex        =   5
      Top             =   840
      Width           =   855
   End
End
Attribute VB_Name = "Form2"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub Command2_Click()
capture_directory = Dir1.Path

If Val(Text2.Text) > 0 Then
Form1.start_timer.Interval = Val(Text2.Text)
End If

If Len(Text3.Text) > 0 Then
seq_title = Text3.Text
End If

If Val(Text4.Text) > 0 Then
max_frames = Val(Text4.Text)
End If

Form2.Hide
End Sub

Private Sub Command3_Click()
Form2.Hide
End Sub

Private Sub Dir1_Change()
Text1.Text = Dir1.Path
End Sub

Private Sub Form_Load()
Dir1.Path = "C:\"
Dir1.Refresh
Text2.Text = Str(Form1.start_timer.Interval)
Text3.Text = seq_title
Text4.Text = Str(max_frames)
End Sub

