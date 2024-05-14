#include "Task_Referee_UI.h"

/* 1.射击辅助线 */
Graph_Data Shoot_Line[6];   // 1 3 5 7 9 中心射击线  固定不变
char String_1m[] = "1m", String_3m[] = "3m",String_5m[] = "5m",String_7m[] = "7m",String_9m[] = "9m";
String_Data Shoot_String[5];

/* 2.自瞄范围 */
Graph_Data Camera_Rectangle;//摄像头能看到的区域
Graph_Data Camera_Centre;//自瞄中心

/* 3.超级电容 */
Graph_Data SuperCap_Rectangle;//电量框
Graph_Data SuperCap_Line; //电量

/* 4.车框通过线 */
Graph_Data Chassis_Line[2];

/* 5.装甲板位姿 */
Graph_Data Armor_Circle[4];

/* 6.各机构模式 */
String_Data ChassisMode_String[4];
char String_Chassis[]  = "CHASSIS:";
char String_Follow []  = "FOLLOW";
char String_Rotate []  = "ROTATE";
char String_Lock   []  = "LOCK";

String_Data GimbalMode_String[3];
char String_Head []  = "GIMBAL:";
char String_Front[]  = "FRONT";
char String_Back []  = "BACK";

String_Data ShootMode_String[4];
char String_Shoot [] = "SHOOT:";
char String_Stop  [] = "STOP";
char String_Ready [] = "READY";
char String_Stuck [] = "STUCKING";

String_Data AimMode_String[3];
char String_Aim [] = "AIM:";
char String_Off [] = "OFF";
char String_On  [] = "ON";

Graph_Data Mode_Rectangle[5];

#if ROBOT_ID == 3
char String_Lid [] = "Lid:";
String_Data LidMode_String[3];
#endif

/* 给裁判系统发送UI任务 */
void Task_Referee_Tx(void *pvParameters)
{
    static portTickType currentTime;
    for (;;)
    {
        currentTime = xTaskGetTickCount();      // 当前系统时间
        if(Referee_State == Device_Online){
            if(Communication_Action_Rx.Key & 0x01)
                  UI_STATE = INIT;
              if(UI_STATE == INIT){
                  UI_Init();
                  UI_STATE = INITING;
              }
              else if(UI_STATE == MOVEING)
                  UI_Move();

              UI_Refresh();
        }
        vTaskDelayUntil(&currentTime, 40);
    }
}

/* 静态UI初始化函数 */
void UI_Init()
{
    /* 射击辅助线 */
    uint8_t Shoot_Char_Size = 8;    //模式字符串字号
    uint8_t Shoot_Char_Width = 2;      //模式字符串线宽
#if ROBOT_ID == 1
    Line_Draw(&Shoot_Line[0], "S1m", UI_Graph_ADD, 1, UI_Color_Yellow, 1, X_CENTRE - 30, Y_CENTRE -100, X_CENTRE + 30, Y_CENTRE -100);
    Line_Draw(&Shoot_Line[1], "S3m", UI_Graph_ADD, 1, UI_Color_Yellow, 1, X_CENTRE - 30, Y_CENTRE -150, X_CENTRE + 30, Y_CENTRE -150);
    Line_Draw(&Shoot_Line[2], "S5m", UI_Graph_ADD, 1, UI_Color_Yellow, 1, X_CENTRE - 30, Y_CENTRE -200, X_CENTRE + 30, Y_CENTRE -200);
    Line_Draw(&Shoot_Line[3], "S7m", UI_Graph_ADD, 1, UI_Color_Yellow, 1, X_CENTRE - 30, Y_CENTRE -250, X_CENTRE + 30, Y_CENTRE -250);
    Line_Draw(&Shoot_Line[4], "S9m", UI_Graph_ADD, 1, UI_Color_Yellow, 1, X_CENTRE - 30, Y_CENTRE -300, X_CENTRE + 30, Y_CENTRE -300);
    Char_Draw(&Shoot_String[0], "C1m", UI_Graph_ADD, 1, UI_Color_Yellow, Shoot_Char_Size, sizeof(String_1m), Shoot_Char_Width, X_CENTRE + 30, Y_CENTRE -100, String_1m);
    Char_Draw(&Shoot_String[1], "C3m", UI_Graph_ADD, 1, UI_Color_Yellow, Shoot_Char_Size, sizeof(String_3m), Shoot_Char_Width, X_CENTRE + 30, Y_CENTRE -150, String_3m);
    Char_Draw(&Shoot_String[2], "C5m", UI_Graph_ADD, 1, UI_Color_Yellow, Shoot_Char_Size, sizeof(String_5m), Shoot_Char_Width, X_CENTRE + 30, Y_CENTRE -200, String_5m);
    Char_Draw(&Shoot_String[3], "C7m", UI_Graph_ADD, 1, UI_Color_Yellow, Shoot_Char_Size, sizeof(String_7m), Shoot_Char_Width, X_CENTRE + 30, Y_CENTRE -250, String_7m);
    Char_Draw(&Shoot_String[4], "C9m", UI_Graph_ADD, 1, UI_Color_Yellow, Shoot_Char_Size, sizeof(String_9m), Shoot_Char_Width, X_CENTRE + 30, Y_CENTRE -300, String_9m);
    Line_Draw(&Shoot_Line[5], "CTR", UI_Graph_ADD, 1, UI_Color_Yellow, 1, X_CENTRE , Y_CENTRE , X_CENTRE, Y_CENTRE - 500 ); //中心线
    /* 可变色自瞄框 + 相机中点，多个目标选离中点最近的 */
    Rectangle_Draw(&Camera_Rectangle, "CAM", UI_Graph_ADD, 1, UI_Color_Cyan, 1, X_CENTRE -400, Y_CENTRE-300, X_CENTRE + 400, Y_CENTRE+300);
    Circle_Draw(&Camera_Centre, "CAT", UI_Graph_ADD, 1, UI_Color_Green, 2 ,X_CENTRE, Y_CENTRE, 2);
    /* 车框线 */
    Line_Draw(&Chassis_Line[0], "CKL", UI_Graph_ADD,  1, UI_Color_Yellow, 1, X_CENTRE - 200, Y_CENTRE -100, X_CENTRE - 300, Y_CENTRE -450);
    Line_Draw(&Chassis_Line[1], "CKR", UI_Graph_ADD,  1, UI_Color_Yellow, 1, X_CENTRE + 200, Y_CENTRE -100, X_CENTRE + 300, Y_CENTRE -450);
    
#elif ROBOT_ID == 3
    Line_Draw(&Shoot_Line[0], "S1m", UI_Graph_ADD, 1, UI_Color_Yellow, 1, X_CENTRE - 30, Y_CENTRE -100, X_CENTRE + 30, Y_CENTRE -100);
    Line_Draw(&Shoot_Line[1], "S3m", UI_Graph_ADD, 1, UI_Color_Yellow, 1, X_CENTRE - 30, Y_CENTRE -150, X_CENTRE + 30, Y_CENTRE -150);
    Line_Draw(&Shoot_Line[2], "S5m", UI_Graph_ADD, 1, UI_Color_Yellow, 1, X_CENTRE - 30, Y_CENTRE -200, X_CENTRE + 30, Y_CENTRE -200);
    Line_Draw(&Shoot_Line[3], "S7m", UI_Graph_ADD, 1, UI_Color_Yellow, 1, X_CENTRE - 30, Y_CENTRE -250, X_CENTRE + 30, Y_CENTRE -250);
    Line_Draw(&Shoot_Line[4], "S9m", UI_Graph_ADD, 1, UI_Color_Yellow, 1, X_CENTRE - 30, Y_CENTRE -300, X_CENTRE + 30, Y_CENTRE -300);
    Char_Draw(&Shoot_String[0], "C1m", UI_Graph_ADD, 1, UI_Color_Yellow, Shoot_Char_Size, sizeof(String_1m), Shoot_Char_Width, X_CENTRE + 30, Y_CENTRE -100, String_1m);
    Char_Draw(&Shoot_String[1], "C3m", UI_Graph_ADD, 1, UI_Color_Yellow, Shoot_Char_Size, sizeof(String_3m), Shoot_Char_Width, X_CENTRE + 30, Y_CENTRE -150, String_3m);
    Char_Draw(&Shoot_String[2], "C5m", UI_Graph_ADD, 1, UI_Color_Yellow, Shoot_Char_Size, sizeof(String_5m), Shoot_Char_Width, X_CENTRE + 30, Y_CENTRE -200, String_5m);
    Char_Draw(&Shoot_String[3], "C7m", UI_Graph_ADD, 1, UI_Color_Yellow, Shoot_Char_Size, sizeof(String_7m), Shoot_Char_Width, X_CENTRE + 30, Y_CENTRE -250, String_7m);
    Char_Draw(&Shoot_String[4], "C9m", UI_Graph_ADD, 1, UI_Color_Yellow, Shoot_Char_Size, sizeof(String_9m), Shoot_Char_Width, X_CENTRE + 30, Y_CENTRE -300, String_9m);
    Line_Draw(&Shoot_Line[5], "CTR", UI_Graph_ADD, 1, UI_Color_Yellow, 1, X_CENTRE , Y_CENTRE , X_CENTRE, Y_CENTRE - 500 ); //中心线
    /* 可变色自瞄框 + 相机中点，多个目标选离中点最近的 */
    Rectangle_Draw(&Camera_Rectangle, "CAM", UI_Graph_ADD, 1, UI_Color_Cyan, 1, X_CENTRE -400, Y_CENTRE-300, X_CENTRE + 400, Y_CENTRE+300);
    Circle_Draw(&Camera_Centre, "CAT", UI_Graph_ADD, 1, UI_Color_Green, 2 ,X_CENTRE, Y_CENTRE, 2);
    /* 车框线 */
    Line_Draw(&Chassis_Line[0], "CKL", UI_Graph_ADD,  1, UI_Color_Yellow, 1, X_CENTRE - 200, Y_CENTRE -100, X_CENTRE - 300, Y_CENTRE -450);
    Line_Draw(&Chassis_Line[1], "CKR", UI_Graph_ADD,  1, UI_Color_Yellow, 1, X_CENTRE + 200, Y_CENTRE -100, X_CENTRE + 300, Y_CENTRE -450);
    
#elif ROBOT_ID == 4
    Line_Draw(&Shoot_Line[0], "S1m", UI_Graph_ADD, 1, UI_Color_Yellow, 1, X_CENTRE - 30, Y_CENTRE -100, X_CENTRE + 30, Y_CENTRE -100);
    Line_Draw(&Shoot_Line[1], "S3m", UI_Graph_ADD, 1, UI_Color_Yellow, 1, X_CENTRE - 30, Y_CENTRE -150, X_CENTRE + 30, Y_CENTRE -150);
    Line_Draw(&Shoot_Line[2], "S5m", UI_Graph_ADD, 1, UI_Color_Yellow, 1, X_CENTRE - 30, Y_CENTRE -200, X_CENTRE + 30, Y_CENTRE -200);
    Line_Draw(&Shoot_Line[3], "S7m", UI_Graph_ADD, 1, UI_Color_Yellow, 1, X_CENTRE - 30, Y_CENTRE -250, X_CENTRE + 30, Y_CENTRE -250);
    Line_Draw(&Shoot_Line[4], "S9m", UI_Graph_ADD, 1, UI_Color_Yellow, 1, X_CENTRE - 30, Y_CENTRE -300, X_CENTRE + 30, Y_CENTRE -300);
    Char_Draw(&Shoot_String[0], "C1m", UI_Graph_ADD, 1, UI_Color_Yellow, Shoot_Char_Size, sizeof(String_1m), Shoot_Char_Width, X_CENTRE + 30, Y_CENTRE -100, String_1m);
    Char_Draw(&Shoot_String[1], "C3m", UI_Graph_ADD, 1, UI_Color_Yellow, Shoot_Char_Size, sizeof(String_3m), Shoot_Char_Width, X_CENTRE + 30, Y_CENTRE -150, String_3m);
    Char_Draw(&Shoot_String[2], "C5m", UI_Graph_ADD, 1, UI_Color_Yellow, Shoot_Char_Size, sizeof(String_5m), Shoot_Char_Width, X_CENTRE + 30, Y_CENTRE -200, String_5m);
    Char_Draw(&Shoot_String[3], "C7m", UI_Graph_ADD, 1, UI_Color_Yellow, Shoot_Char_Size, sizeof(String_7m), Shoot_Char_Width, X_CENTRE + 30, Y_CENTRE -250, String_7m);
    Char_Draw(&Shoot_String[4], "C9m", UI_Graph_ADD, 1, UI_Color_Yellow, Shoot_Char_Size, sizeof(String_9m), Shoot_Char_Width, X_CENTRE + 30, Y_CENTRE -300, String_9m);
    Line_Draw(&Shoot_Line[5], "CTR", UI_Graph_ADD, 1, UI_Color_Yellow, 1, X_CENTRE , Y_CENTRE , X_CENTRE, Y_CENTRE - 500 ); //中心线
    
    /* 可变色自瞄框 + 相机中点，多个目标选离中点最近的 */
    Rectangle_Draw(&Camera_Rectangle, "CAM", UI_Graph_ADD, 1, UI_Color_Cyan, 1, X_CENTRE -400, Y_CENTRE-300, X_CENTRE + 400, Y_CENTRE+300);
    Circle_Draw(&Camera_Centre, "CAT", UI_Graph_ADD, 1, UI_Color_Green, 2 ,X_CENTRE, Y_CENTRE, 2);
    /* 车框线 */
    Line_Draw(&Chassis_Line[0], "CKL", UI_Graph_ADD,  1, UI_Color_Yellow, 1, X_CENTRE - 200, Y_CENTRE -100, X_CENTRE - 300, Y_CENTRE -450);
    Line_Draw(&Chassis_Line[1], "CKR", UI_Graph_ADD,  1, UI_Color_Yellow, 1, X_CENTRE + 200, Y_CENTRE -100, X_CENTRE + 300, Y_CENTRE -450);
#endif
    
    /* 动态超电容量 */
    Rectangle_Draw(&SuperCap_Rectangle, "SUP", UI_Graph_ADD, 1, UI_Color_Yellow, 3, X_CENTRE - 450, Y_CENTRE - 450, X_CENTRE - 150, Y_CENTRE - 430);
    Line_Draw(&SuperCap_Line, "CAP", UI_Graph_ADD, 1, UI_Color_Green, 10,  X_CENTRE - 400, Y_CENTRE - 390, X_CENTRE - 400, Y_CENTRE - 390);

    /* 动态装甲板(圆形绕射击UI) */
    uint8_t R = 10;
    Circle_Draw(&Armor_Circle[0], "AM0", UI_Graph_ADD, 1, UI_Color_Green, 2 ,X_CENTRE + 50 * sin(Offset_Angle_Rx.Yaw),            Y_CENTRE + 50 * cos(Offset_Angle_Rx.Yaw),          R);
    Circle_Draw(&Armor_Circle[1], "AM1", UI_Graph_ADD, 1, UI_Color_Green, 2 ,X_CENTRE + 50 * sin(Offset_Angle_Rx.Yaw - PI/2),     Y_CENTRE + 50 * cos(Offset_Angle_Rx.Yaw- PI/2),    R);
    Circle_Draw(&Armor_Circle[2], "AM2", UI_Graph_ADD, 1, UI_Color_Green, 2 ,X_CENTRE + 50 * sin(Offset_Angle_Rx.Yaw - PI),       Y_CENTRE + 50 * cos(Offset_Angle_Rx.Yaw- PI),      R);
    Circle_Draw(&Armor_Circle[3], "AM3", UI_Graph_ADD, 1, UI_Color_Green, 2 ,X_CENTRE + 50 * sin(Offset_Angle_Rx.Yaw - PI/2 *3 ), Y_CENTRE + 50 * cos(Offset_Angle_Rx.Yaw- PI/2 *3), R);
    
    /* 机构模式（字符串套框）*/
    uint8_t Mode_Char_Size = 13;    //模式字符串字号
    uint8_t Mode_Char_Width = 2;      //模式字符串线宽
    Char_Draw(&ChassisMode_String[0], "CHA", UI_Graph_ADD, 1, UI_Color_Green,  Mode_Char_Size, sizeof(String_Chassis), Mode_Char_Width+1, X_CENTRE - 700, Y_CENTRE +200, String_Chassis);
    Char_Draw(&ChassisMode_String[1], "CHF", UI_Graph_ADD, 1, UI_Color_Yellow, Mode_Char_Size, sizeof(String_Follow),  Mode_Char_Width,   X_CENTRE - 700, Y_CENTRE +170, String_Follow);
    Char_Draw(&ChassisMode_String[2], "CHR", UI_Graph_ADD, 1, UI_Color_Yellow, Mode_Char_Size, sizeof(String_Rotate),  Mode_Char_Width,   X_CENTRE - 700, Y_CENTRE +140, String_Rotate);
    Char_Draw(&ChassisMode_String[3], "CHL", UI_Graph_ADD, 1, UI_Color_Yellow, Mode_Char_Size, sizeof(String_Lock),    Mode_Char_Width,   X_CENTRE - 700, Y_CENTRE +110, String_Lock);
    
    Char_Draw(&GimbalMode_String[0], "GIM", UI_Graph_ADD, 1, UI_Color_Green, Mode_Char_Size,  sizeof(String_Head),  Mode_Char_Width+1, X_CENTRE - 570, Y_CENTRE +200, String_Head);
    Char_Draw(&GimbalMode_String[1], "GIF", UI_Graph_ADD, 1, UI_Color_Yellow, Mode_Char_Size, sizeof(String_Front), Mode_Char_Width,   X_CENTRE - 570, Y_CENTRE+170,  String_Front);
    Char_Draw(&GimbalMode_String[2], "GIB", UI_Graph_ADD, 1, UI_Color_Yellow, Mode_Char_Size, sizeof(String_Back),  Mode_Char_Width,   X_CENTRE - 570, Y_CENTRE +140, String_Back);

    Char_Draw(&ShootMode_String[0], "SHO", UI_Graph_ADD, 1, UI_Color_Green,  Mode_Char_Size, sizeof(String_Shoot), Mode_Char_Width+1, X_CENTRE - 460, Y_CENTRE +200, String_Shoot);
    Char_Draw(&ShootMode_String[1], "SHS", UI_Graph_ADD, 1, UI_Color_Yellow, Mode_Char_Size, sizeof(String_Stop),  Mode_Char_Width,   X_CENTRE - 460, Y_CENTRE +170, String_Stop);
    Char_Draw(&ShootMode_String[2], "SHR", UI_Graph_ADD, 1, UI_Color_Yellow, Mode_Char_Size, sizeof(String_Ready), Mode_Char_Width,   X_CENTRE - 460, Y_CENTRE +140, String_Ready);
    Char_Draw(&ShootMode_String[3], "SHK", UI_Graph_ADD, 1, UI_Color_Yellow, Mode_Char_Size, sizeof(String_Stuck), Mode_Char_Width,   X_CENTRE - 460, Y_CENTRE +110, String_Stuck);

    Char_Draw(&AimMode_String[0], "AIM", UI_Graph_ADD, 1, UI_Color_Green,  Mode_Char_Size, sizeof(String_Aim), Mode_Char_Width+1, X_CENTRE - 350, Y_CENTRE +200, String_Aim);
    Char_Draw(&AimMode_String[1], "AMF", UI_Graph_ADD, 1, UI_Color_Yellow, Mode_Char_Size, sizeof(String_Off), Mode_Char_Width,   X_CENTRE - 350, Y_CENTRE +170, String_Off);
    Char_Draw(&AimMode_String[2], "AMN", UI_Graph_ADD, 1, UI_Color_Yellow, Mode_Char_Size, sizeof(String_On),  Mode_Char_Width,   X_CENTRE - 350, Y_CENTRE +140, String_On);
#if ROBOT_ID == 3 
    Char_Draw(&LidMode_String[0], "LID", UI_Graph_ADD, 1, UI_Color_Green,  Mode_Char_Size, sizeof(String_Lid), Mode_Char_Width+1, X_CENTRE - 280, Y_CENTRE +200, String_Lid);
    Char_Draw(&LidMode_String[1], "LDF", UI_Graph_ADD, 1, UI_Color_Yellow, Mode_Char_Size, sizeof(String_Off), Mode_Char_Width,   X_CENTRE - 280, Y_CENTRE +170, String_Off);
    Char_Draw(&LidMode_String[2], "LDN", UI_Graph_ADD, 1, UI_Color_Yellow, Mode_Char_Size, sizeof(String_On),  Mode_Char_Width,   X_CENTRE - 280, Y_CENTRE +140, String_On);
    Rectangle_Draw(&Mode_Rectangle[4], "RLD", UI_Graph_ADD, 1, UI_Color_Green, 2, X_CENTRE - 285, Y_CENTRE +200, X_CENTRE - 235, Y_CENTRE +190);  
#endif
    Rectangle_Draw(&Mode_Rectangle[0], "RCA", UI_Graph_ADD, 1, UI_Color_Green, 2, X_CENTRE - 705, Y_CENTRE +200, X_CENTRE - 605, Y_CENTRE +180);   
    Rectangle_Draw(&Mode_Rectangle[1], "RGI", UI_Graph_ADD, 1, UI_Color_Green, 2, X_CENTRE - 575, Y_CENTRE +200, X_CENTRE - 485, Y_CENTRE +190);   
    Rectangle_Draw(&Mode_Rectangle[2], "RSH", UI_Graph_ADD, 1, UI_Color_Green, 2, X_CENTRE - 465, Y_CENTRE +200, X_CENTRE - 375, Y_CENTRE +190);   
    Rectangle_Draw(&Mode_Rectangle[3], "RAM", UI_Graph_ADD, 1, UI_Color_Green, 2, X_CENTRE - 355, Y_CENTRE +200, X_CENTRE - 305, Y_CENTRE +190);
}

/* 动态UI函数 */
void UI_Move()
{
    static uint8_t twink_flag = 0; //超电关闭，容量框闪烁
    static uint8_t Armor_time = 0, Armor_flag = 0;//装甲板受打击显示1S
    /* 切换模式*/
    if(Communication_Action_Rx.ChassisAction == CHASSIS_FOLLOW)//底盘模式
        Rectangle_Draw(&Mode_Rectangle[0], "RCA", UI_Graph_Change, 1, UI_Color_Green, 2, X_CENTRE - 705, Y_CENTRE +175, X_CENTRE - 625, Y_CENTRE +150); 
     else if(Communication_Action_Rx.ChassisAction == CHASSIS_SPIN)
        Rectangle_Draw(&Mode_Rectangle[0], "RCA", UI_Graph_Change, 1, UI_Color_Green, 2, X_CENTRE - 705, Y_CENTRE +145, X_CENTRE - 625, Y_CENTRE +120); 
     else if(Communication_Action_Rx.ChassisAction == CHASSIS_NORMAL)
        Rectangle_Draw(&Mode_Rectangle[0], "RCA", UI_Graph_Change, 1, UI_Color_Green, 2, X_CENTRE - 705, Y_CENTRE +115, X_CENTRE - 645, Y_CENTRE +90); 
     
     if(Communication_Action_Rx.MidMode == FRONT)//云台朝向
        Rectangle_Draw(&Mode_Rectangle[1], "RGI", UI_Graph_Change, 1, UI_Color_Green, 2, X_CENTRE - 575, Y_CENTRE +175, X_CENTRE - 505, Y_CENTRE +150); 
     else if(Communication_Action_Rx.MidMode == BACK)
        Rectangle_Draw(&Mode_Rectangle[1], "RGI", UI_Graph_Change, 1, UI_Color_Green, 2, X_CENTRE - 575, Y_CENTRE +145, X_CENTRE - 515, Y_CENTRE +120); 
    
      if(Communication_Action_Rx.ShootAction == SHOOT_STOP)//发射机构状态
        Rectangle_Draw(&Mode_Rectangle[2], "RSH", UI_Graph_Change, 1, UI_Color_Green, 2, X_CENTRE - 465, Y_CENTRE +175, X_CENTRE - 405, Y_CENTRE +150); 
     else if(Communication_Action_Rx.ShootAction == SHOOT_READY)
        Rectangle_Draw(&Mode_Rectangle[2], "RSH", UI_Graph_Change, 1, UI_Color_Green, 2, X_CENTRE - 465, Y_CENTRE +145, X_CENTRE - 395, Y_CENTRE +120); 
     else if(Communication_Action_Rx.ShootAction == SHOOT_STUCKING)
        Rectangle_Draw(&Mode_Rectangle[2], "RSH", UI_Graph_Change, 1, UI_Color_Green, 2, X_CENTRE - 465, Y_CENTRE +115, X_CENTRE - 355, Y_CENTRE +90); 
    
    if(Communication_Action_Rx.AimAction == AIM_STOP)//自瞄状态
        Rectangle_Draw(&Mode_Rectangle[3], "RAM", UI_Graph_Change, 1, UI_Color_Green, 2, X_CENTRE - 355, Y_CENTRE +175, X_CENTRE - 315, Y_CENTRE +150); 
     else if(Communication_Action_Rx.AimAction == AIM_AUTO )
        Rectangle_Draw(&Mode_Rectangle[3], "RAM", UI_Graph_Change, 1, UI_Color_Green, 2, X_CENTRE - 355, Y_CENTRE +145, X_CENTRE - 325, Y_CENTRE +120); 

#if ROBOT_ID == 3
     if(Communication_Action_Rx.LidMode == LID_OFF)//弹仓
        Rectangle_Draw(&Mode_Rectangle[4], "RLD", UI_Graph_Change, 1, UI_Color_Green, 2, X_CENTRE - 285, Y_CENTRE +175, X_CENTRE - 245, Y_CENTRE +150); 
     else if(Communication_Action_Rx.LidMode == LID_ON )
        Rectangle_Draw(&Mode_Rectangle[4], "RLD", UI_Graph_Change, 1, UI_Color_Green, 2, X_CENTRE - 285, Y_CENTRE +145, X_CENTRE - 255, Y_CENTRE +120); 
#endif
     
     /* 机构中存在离线模块，此机构将不能使用, UI模式全框住 */
     if(Up_State == Device_Offline && Examine_Motor_State() != osOK)//底盘3个电机 CHASSIS
         Rectangle_Draw(&Mode_Rectangle[0], "RCA", UI_Graph_Change, 1, UI_Color_Purplish_red, 2, X_CENTRE - 705, Y_CENTRE +205, X_CENTRE - 605, Y_CENTRE +90); 
     if(Communication_Action_Rx.Device_State & 0x01)//云台两个电机 GIMBAL
         Rectangle_Draw(&Mode_Rectangle[1], "RGI", UI_Graph_Change, 1, UI_Color_Purplish_red, 2, X_CENTRE - 575, Y_CENTRE +205, X_CENTRE - 485, Y_CENTRE +120); 
     if(Communication_Action_Rx.Device_State & 0x02)//发射机构电机 SHOOT
        Rectangle_Draw(&Mode_Rectangle[2], "RSH", UI_Graph_Change, 1, UI_Color_Purplish_red, 2, X_CENTRE - 465, Y_CENTRE +205, X_CENTRE - 370, Y_CENTRE +90); 
     if(Communication_Action_Rx.Device_State & 0x04)//PC离线 IMU离线 AIM
        Rectangle_Draw(&Mode_Rectangle[3], "RAM", UI_Graph_Change, 1, UI_Color_Purplish_red, 2, X_CENTRE - 355, Y_CENTRE +205, X_CENTRE - 305, Y_CENTRE +120); 
     
     /* 装甲板动态受打击显示 */
        if(!Damage_status.HP_deduction_reason){
            Armor_time = 26;
            Damage_status.HP_deduction_reason = 1;
        }
        if(Armor_time > 0)
            Armor_time --;
         
        (Damage_status.armor_id & 0x01) && Armor_time ?//ID 1
         Circle_Draw(&Armor_Circle[0], "AM0", UI_Graph_Change, 1, UI_Color_Purplish_red, 2 ,X_CENTRE + 50 * sin(Offset_Angle_Rx.Yaw),         Y_CENTRE + 50 * cos(Offset_Angle_Rx.Yaw),         10) :
         Circle_Draw(&Armor_Circle[0], "AM0", UI_Graph_Change, 1, UI_Color_Green,        2 ,X_CENTRE + 50 * sin(Offset_Angle_Rx.Yaw),         Y_CENTRE + 50 * cos(Offset_Angle_Rx.Yaw),         10);
    
        (Damage_status.armor_id & 0x02) && Armor_time ?//ID 2
         Circle_Draw(&Armor_Circle[1], "AM1", UI_Graph_Change, 1, UI_Color_Purplish_red, 2 ,X_CENTRE + 50 * sin(Offset_Angle_Rx.Yaw + PI/2),  Y_CENTRE + 50 * cos(Offset_Angle_Rx.Yaw + PI/2),   10) :
         Circle_Draw(&Armor_Circle[1], "AM1", UI_Graph_Change, 1, UI_Color_Green,        2 ,X_CENTRE + 50 * sin(Offset_Angle_Rx.Yaw + PI/2),  Y_CENTRE + 50 * cos(Offset_Angle_Rx.Yaw + PI/2),   10);
    
        (Damage_status.armor_id & 0x04) && Armor_time ?//ID 3
         Circle_Draw(&Armor_Circle[2], "AM2", UI_Graph_Change, 1, UI_Color_Purplish_red, 2 ,X_CENTRE + 50 * sin(Offset_Angle_Rx.Yaw + PI),     Y_CENTRE + 50 * cos(Offset_Angle_Rx.Yaw + PI),     10) :
         Circle_Draw(&Armor_Circle[2], "AM2", UI_Graph_Change, 1, UI_Color_Green,        2 ,X_CENTRE + 50 * sin(Offset_Angle_Rx.Yaw + PI),     Y_CENTRE + 50 * cos(Offset_Angle_Rx.Yaw + PI),     10);
    
        (Damage_status.armor_id & 0x08) && Armor_time ?//ID 4
         Circle_Draw(&Armor_Circle[3], "AM3", UI_Graph_Change, 1, UI_Color_Purplish_red, 2 ,X_CENTRE + 50 * sin(Offset_Angle_Rx.Yaw + PI/2*3), Y_CENTRE + 50 * cos(Offset_Angle_Rx.Yaw + PI/2*3), 10) :
         Circle_Draw(&Armor_Circle[3], "AM3", UI_Graph_Change, 1, UI_Color_Green,        2 ,X_CENTRE + 50 * sin(Offset_Angle_Rx.Yaw + PI/2*3), Y_CENTRE + 50 * cos(Offset_Angle_Rx.Yaw + PI/2*3), 10);

     /* 超电容量及离线检测 */
     if(Cap_State == Device_Online){
        Line_Draw(&SuperCap_Line, "CAP", UI_Graph_Change, 1, UI_Color_Green, 10,  X_CENTRE - 450, Y_CENTRE - 440, X_CENTRE - (450 - SuperCap_Rx.cap_percent * 3), Y_CENTRE - 440);
         if(SuperCap_Rx.cap_percent == 0){ //超电容量为0，表示DCDC关闭，需手动恢复
             if(twink_flag <=11){
                 Rectangle_Draw(&SuperCap_Rectangle, "SUP", UI_Graph_Change, 1, UI_Color_Purplish_red, 3, X_CENTRE - 450, Y_CENTRE - 450, X_CENTRE - 150, Y_CENTRE - 430);
             } else {
                 Rectangle_Draw(&SuperCap_Rectangle, "SUP", UI_Graph_Change, 1, UI_Color_Green, 3, X_CENTRE - 450, Y_CENTRE - 450, X_CENTRE - 150, Y_CENTRE - 430);
             if(twink_flag == 20)
                twink_flag = 0;
             }
             twink_flag ++;
         } else {
             Rectangle_Draw(&SuperCap_Rectangle, "SUP", UI_Graph_Change, 1, UI_Color_Green, 3, X_CENTRE - 450, Y_CENTRE - 450, X_CENTRE - 150, Y_CENTRE - 430);             
         }
     } else {       //超电离线证明超电和下板通信can线断了
         Line_Draw(&SuperCap_Line, "CAP", UI_Graph_Change, 1, UI_Color_Purplish_red, 10,  X_CENTRE - 450, Y_CENTRE - 440, X_CENTRE - 200, Y_CENTRE - 440);
         Rectangle_Draw(&SuperCap_Rectangle, "SUP", UI_Graph_Change, 1, UI_Color_Purplish_red, 3, X_CENTRE - 450, Y_CENTRE - 450, X_CENTRE - 200, Y_CENTRE - 430);
     }
     
#if ROBOT_ID == 1
     /* 变色自瞄框 */
     Communication_Action_Rx.Device_State & 0x08 ?//瞄上了自瞄框变红色
        Rectangle_Draw(&Camera_Rectangle, "CAM", UI_Graph_Change, 1, UI_Color_Purplish_red, 1, X_CENTRE - 300, Y_CENTRE - 200, X_CENTRE + 300, Y_CENTRE + 200):
        Rectangle_Draw(&Camera_Rectangle, "CAM", UI_Graph_Change, 1, UI_Color_Cyan, 1, X_CENTRE - 400, Y_CENTRE - 200, X_CENTRE + 400, Y_CENTRE + 200);

     /* 车框线 */
        Line_Draw(&Chassis_Line[0], "CKL", UI_Graph_Change,  1, UI_Color_Yellow, 1, X_CENTRE - (100 + Offset_Angle_Rx.Pitch * 5), Y_CENTRE - (150 - Offset_Angle_Rx.Pitch * 10), X_CENTRE - (250 + Offset_Angle_Rx.Pitch * 10), Y_CENTRE -539);//X斜率Y长度
        Line_Draw(&Chassis_Line[1], "CKR", UI_Graph_Change,  1, UI_Color_Yellow, 1, X_CENTRE + (100 + Offset_Angle_Rx.Pitch * 5), Y_CENTRE - (150 - Offset_Angle_Rx.Pitch * 10), X_CENTRE + (250 + Offset_Angle_Rx.Pitch * 10), Y_CENTRE -539);
#elif ROBOT_ID == 3
     /* 变色自瞄框 */
     Communication_Action_Rx.Device_State & 0x08 ?//瞄上了自瞄框变红色
        Rectangle_Draw(&Camera_Rectangle, "CAM", UI_Graph_Change, 1, UI_Color_Purplish_red, 1, X_CENTRE - 300, Y_CENTRE - 200, X_CENTRE + 300, Y_CENTRE + 200):
        Rectangle_Draw(&Camera_Rectangle, "CAM", UI_Graph_Change, 1, UI_Color_Cyan, 1, X_CENTRE - 400, Y_CENTRE - 200, X_CENTRE + 400, Y_CENTRE + 200);

     /* 车框线 */
        Line_Draw(&Chassis_Line[0], "CKL", UI_Graph_Change,  1, UI_Color_Yellow, 1, X_CENTRE - (90 - Offset_Angle_Rx.Pitch * 5), Y_CENTRE - (150 + Offset_Angle_Rx.Pitch * 10), X_CENTRE - (375 - Offset_Angle_Rx.Pitch * 10), Y_CENTRE -539);//X斜率Y长度
        Line_Draw(&Chassis_Line[1], "CKR", UI_Graph_Change,  1, UI_Color_Yellow, 1, X_CENTRE + (90 - Offset_Angle_Rx.Pitch * 5), Y_CENTRE - (150 + Offset_Angle_Rx.Pitch * 10), X_CENTRE + (375 - Offset_Angle_Rx.Pitch * 10), Y_CENTRE -539);
#elif ROBOT_ID == 4
     /* 自瞄框 */
     Communication_Action_Rx.Device_State & 0x08 ?//瞄上了自瞄框变红色
        Rectangle_Draw(&Camera_Rectangle, "CAM", UI_Graph_Change, 1, UI_Color_Purplish_red, 1, X_CENTRE - 300, Y_CENTRE - 200, X_CENTRE + 300, Y_CENTRE + 200):
        Rectangle_Draw(&Camera_Rectangle, "CAM", UI_Graph_Change, 1, UI_Color_Cyan, 1, X_CENTRE - 400, Y_CENTRE - 200, X_CENTRE + 400, Y_CENTRE + 200);

     /* 车框线 */
        Line_Draw(&Chassis_Line[0], "CKL", UI_Graph_Change,  1, UI_Color_Yellow, 1, X_CENTRE - (90 - Offset_Angle_Rx.Pitch * 5), Y_CENTRE - (150 + Offset_Angle_Rx.Pitch * 10), X_CENTRE - (375 - Offset_Angle_Rx.Pitch * 10), Y_CENTRE -539);//X斜率Y长度
        Line_Draw(&Chassis_Line[1], "CKR", UI_Graph_Change,  1, UI_Color_Yellow, 1, X_CENTRE + (90 - Offset_Angle_Rx.Pitch * 5), Y_CENTRE - (150 + Offset_Angle_Rx.Pitch * 10), X_CENTRE + (375 - Offset_Angle_Rx.Pitch * 10), Y_CENTRE -539);
#endif
}

/* 刷新UI界面 */
void UI_Refresh()
{
    static uint8_t cnt = 0;
    if(UI_STATE == INITING){
        switch(cnt++){
            case 0: UI_Delete(UI_Data_Del_ALL, 9);break;
            case 1: UI_Delete(UI_Data_Del_ALL, 8);break;
            case 2: UI_Delete(UI_Data_Del_ALL, 7);break;
            case 3: UI_Delete(UI_Data_Del_ALL, 6);break;
            case 4: UI_Delete(UI_Data_Del_ALL, 5);break;
            case 5: UI_Delete(UI_Data_Del_ALL, 4);break;
            case 6: UI_Delete(UI_Data_Del_ALL, 3);break;
            case 7: UI_Delete(UI_Data_Del_ALL, 2);break;
            case 8: UI_Delete(UI_Data_Del_ALL, 1);break;
            case 9: UI_Delete(UI_Data_Del_ALL, 0);break;
            case 10: Graph_ReFresh(7, Shoot_Line[0], Shoot_Line[1], Shoot_Line[2], Shoot_Line[3], Shoot_Line[4], Shoot_Line[5], SuperCap_Rectangle); break;
            case 11: Graph_ReFresh(7, SuperCap_Line, Armor_Circle[0], Armor_Circle[1], Armor_Circle[2], Armor_Circle[3], Chassis_Line[0], Chassis_Line[1]); break;
            case 12: Char_ReFresh(Shoot_String[0]); break;
            case 13: Char_ReFresh(Shoot_String[1]); break;
            case 14: Char_ReFresh(Shoot_String[2]); break;
            case 15: Char_ReFresh(Shoot_String[3]); break;
            case 16: Char_ReFresh(Shoot_String[4]); break;
            case 17: Char_ReFresh(ChassisMode_String[0]); break;
            case 18: Char_ReFresh(ChassisMode_String[1]); break;
            case 19: Char_ReFresh(ChassisMode_String[2]); break;
            case 20: Char_ReFresh(ChassisMode_String[3]); break;
            case 21: Char_ReFresh(GimbalMode_String[0]); break;
            case 22: Char_ReFresh(GimbalMode_String[1]); break;
            case 23: Char_ReFresh(GimbalMode_String[2]); break;
            case 24: Char_ReFresh(ShootMode_String[0]); break;
            case 25: Char_ReFresh(ShootMode_String[1]); break;
            case 26: Char_ReFresh(ShootMode_String[2]); break;
            case 27: Char_ReFresh(ShootMode_String[3]); break;
            case 28: Char_ReFresh(AimMode_String[0]);  break;
            case 29: Char_ReFresh(AimMode_String[1]);  break;
            case 30: Char_ReFresh(AimMode_String[2]);  break;
#if ROBOT_ID == 3
            case 31: Char_ReFresh(LidMode_String[0]);  break;
            case 32: Char_ReFresh(LidMode_String[1]);  break;
            case 33: Char_ReFresh(LidMode_String[2]);  break;
            case 34: Graph_ReFresh(7, Mode_Rectangle[0], Mode_Rectangle[1], Mode_Rectangle[2], Mode_Rectangle[3], Mode_Rectangle[4], Camera_Centre, Camera_Rectangle);  break;
            case 35: UI_STATE = MOVEING; cnt=0; break;
#else
            case 31: Graph_ReFresh(5, Mode_Rectangle[0], Mode_Rectangle[1], Mode_Rectangle[2], Mode_Rectangle[3], Camera_Rectangle);  break;
            case 32: UI_STATE = MOVEING; cnt=0; break;
#endif
            default: break;
        }
    }
    if(UI_STATE == MOVEING){
        if(cnt){
#if ROBOT_ID == 3
            Graph_ReFresh(7, Mode_Rectangle[0], Mode_Rectangle[1], Mode_Rectangle[2], Mode_Rectangle[3], Mode_Rectangle[4], Chassis_Line[0], Chassis_Line[1]);
#else
            Graph_ReFresh(7, Mode_Rectangle[0], Mode_Rectangle[1], Mode_Rectangle[2], Mode_Rectangle[3], Chassis_Line[0], Chassis_Line[1], Camera_Centre);
#endif
            cnt = 0;
        } else {
            Graph_ReFresh(7, Camera_Rectangle, SuperCap_Rectangle, SuperCap_Line, Armor_Circle[0], Armor_Circle[1], Armor_Circle[2], Armor_Circle[3] );
            cnt = 1;
        }
    }
}