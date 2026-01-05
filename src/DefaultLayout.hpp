#pragma once
const char* defaultLayout = R"(
[Window][WindowOverViewport_11111111]
Pos=0,0
Size=1728,972
Collapsed=0

[Window][Debug##Default]
Pos=60,60
Size=400,400
Collapsed=0

[Window][Connection]
Pos=0,0
Size=615,430
Collapsed=0
DockId=0x00000003,0

[Window][Game Logs]
Pos=0,0
Size=615,430
Collapsed=0
DockId=0x00000003,1

[Window][Console]
Pos=0,0
Size=615,430
Collapsed=0
DockId=0x00000003,2

[Window][SecondAID Status]
Pos=0,0
Size=615,430
Collapsed=0
DockId=0x00000003,3

[Window][Script Browser]
Pos=1440,519
Size=288,453
Collapsed=0
DockId=0x0000000A,0

[Window][ScriptEditorID]
Pos=552,0
Size=876,868
Collapsed=0
DockId=0x00000002,0

[Window][Controls]
Pos=1440,0
Size=288,517
Collapsed=0
DockId=0x00000009,0

[Window][Script Editor]
Pos=617,0
Size=821,781
Collapsed=0
DockId=0x00000011,0

[Window][Static Analysis Overview]
Pos=617,0
Size=821,781
Collapsed=0
DockId=0x00000011,1

[Window][Active File Analysis]
Pos=617,783
Size=821,189
Collapsed=0
DockId=0x00000012,0

[Window][Breakpoints]
Pos=0,432
Size=615,208
Collapsed=0
DockId=0x0000000B,0

[Window][Lua Errors]
Pos=0,799
Size=615,173
Collapsed=0
DockId=0x0000000E,0

[Window][Watches]
Pos=0,642
Size=615,155
Collapsed=0
DockId=0x0000000D,0

[Table][0x687B9A6B,5]
RefScale=13

[Table][0xBEB68690,3]
RefScale=13
Column 0  Weight=1.0000
Column 1  Width=50
Column 2  Width=100

[Table][0xB72AC151,4]
RefScale=13
Column 0  Weight=1.0000
Column 1  Weight=1.0000
Column 2  Width=40
Column 3  Width=30

[Table][0xC55F7D5F,5]
RefScale=13
Column 0  Sort=0v

[Docking][Data]
DockSpace             ID=0x08BD597D Window=0x1BBC0F80 Pos=0,0 Size=1728,972 Split=X
  DockNode            ID=0x00000005 Parent=0x08BD597D SizeRef=1438,864 Split=X
    DockNode          ID=0x00000007 Parent=0x00000005 SizeRef=615,864 Split=X
      DockNode        ID=0x00000001 Parent=0x00000007 SizeRef=658,864 Split=Y
        DockNode      ID=0x00000003 Parent=0x00000001 SizeRef=640,430 Selected=0x44C22849
        DockNode      ID=0x00000004 Parent=0x00000001 SizeRef=640,432 Split=Y Selected=0xEA83D666
          DockNode    ID=0x0000000B Parent=0x00000004 SizeRef=527,208 Selected=0x16AF887D
          DockNode    ID=0x0000000C Parent=0x00000004 SizeRef=527,330 Split=Y Selected=0xA0C159B7
            DockNode  ID=0x0000000D Parent=0x0000000C SizeRef=497,155 CentralNode=1 Selected=0x3EF237F2
            DockNode  ID=0x0000000E Parent=0x0000000C SizeRef=497,173 Selected=0x2B4E6928
      DockNode        ID=0x00000002 Parent=0x00000007 SizeRef=876,864 Selected=0x924E2E89
    DockNode          ID=0x00000008 Parent=0x00000005 SizeRef=821,864 Split=Y Selected=0xE89E0DC0
      DockNode        ID=0x0000000F Parent=0x00000008 SizeRef=821,745 Split=Y Selected=0xE89E0DC0
        DockNode      ID=0x00000011 Parent=0x0000000F SizeRef=821,781 Selected=0xE89E0DC0
        DockNode      ID=0x00000012 Parent=0x0000000F SizeRef=821,189 Selected=0x7786E6F2
      DockNode        ID=0x00000010 Parent=0x00000008 SizeRef=821,225 Selected=0x4369D7FA
  DockNode            ID=0x00000006 Parent=0x08BD597D SizeRef=288,864 Split=Y Selected=0x4C37AC16
    DockNode          ID=0x00000009 Parent=0x00000006 SizeRef=334,517 Selected=0xA0C159B7
    DockNode          ID=0x0000000A Parent=0x00000006 SizeRef=334,453 Selected=0x4C37AC16


)";
