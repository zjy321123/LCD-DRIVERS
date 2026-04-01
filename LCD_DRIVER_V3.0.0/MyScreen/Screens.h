#ifndef SCREENS_H
#define SCREENS_H
#include "lvgl.h"

#define SCREEN_AutoLoadtime 3000
#define SCREEN_6AutoLoadtime SCREEN_AutoLoadtime
#define SCREEN_13AutoLoadtime SCREEN_AutoLoadtime
#define SCREEN_14AutoLoadtime SCREEN_AutoLoadtime
#define SCREEN_16AutoLoadtime SCREEN_AutoLoadtime
#define SCREEN_22AutoLoadtime SCREEN_AutoLoadtime
#define SCREEN_24AutoLoadtime SCREEN_AutoLoadtime
#define SCREEN_26AutoLoadtime SCREEN_AutoLoadtime
#define SCREEN_28AutoLoadtime SCREEN_AutoLoadtime
#define SCREEN_29AutoLoadtime 300
#define SCREEN_30AutoLoadtime SCREEN_AutoLoadtime
#define SCREEN_32AutoLoadtime SCREEN_AutoLoadtime
#define SCREEN_35AutoLoadtime SCREEN_AutoLoadtime
#define SCREEN_37AutoLoadtime SCREEN_AutoLoadtime
#define SCREEN_40AutoLoadtime SCREEN_AutoLoadtime
#define SCREEN_43AutoLoadtime SCREEN_AutoLoadtime
#define SCREEN_46AutoLoadtime SCREEN_AutoLoadtime
#define SCREEN_49AutoLoadtime SCREEN_AutoLoadtime
#define SCREEN_52AutoLoadtime SCREEN_AutoLoadtime
#define SCREEN_54AutoLoadtime SCREEN_AutoLoadtime
#define SCREEN_74AutoLoadtime 10000
#define SCREEN_76AutoLoadtime SCREEN_AutoLoadtime
#define SCREEN_77AutoLoadtime SCREEN_AutoLoadtime
#define SCREEN_79AutoLoadtime SCREEN_AutoLoadtime
#define SCREEN_56AutoLoadtime 500

typedef enum
{
    SCREEN,
    SCREEN_1,
    SCREEN_2,
    SCREEN_3,
    SCREEN_4,
    SCREEN_5,
    SCREEN_6,
    SCREEN_7,
    SCREEN_8,
    SCREEN_9,
    SCREEN_10,
    SCREEN_11,
    SCREEN_12,
    SCREEN_13,
    SCREEN_14,
    SCREEN_15,
    SCREEN_16,
    SCREEN_17,
    SCREEN_18,
    SCREEN_19,
    SCREEN_20,
    SCREEN_21,
    SCREEN_22,
    SCREEN_23,
    SCREEN_24,
    SCREEN_25,
    SCREEN_26,
    SCREEN_27,
    SCREEN_28,
    SCREEN_29,
    SCREEN_30,
    SCREEN_31,
    SCREEN_32,
    SCREEN_33,
    SCREEN_34,
    SCREEN_35,
    SCREEN_36,
    SCREEN_37,
    SCREEN_38,
    SCREEN_39,
    SCREEN_40,
    SCREEN_41,
    SCREEN_42,
    SCREEN_43,
    SCREEN_44,
    SCREEN_45,
    SCREEN_46,
    SCREEN_47,
    SCREEN_48,
    SCREEN_49,
    SCREEN_50,
    SCREEN_51,
    SCREEN_52,
    SCREEN_53,
    SCREEN_54,
    SCREEN_55,
    SCREEN_56,
    SCREEN_57,
    SCREEN_58,
    SCREEN_59,
    SCREEN_60,
    SCREEN_61,
    SCREEN_62,
    SCREEN_63,
    SCREEN_64,
    SCREEN_65,
    SCREEN_66,
    SCREEN_67,
    SCREEN_68,
    SCREEN_69,
    SCREEN_70,
    SCREEN_71,
    SCREEN_72,
    SCREEN_73,
    SCREEN_74,
    SCREEN_75,
    SCREEN_76,
    SCREEN_77,
    SCREEN_78,
    SCREEN_79,
    SCREEN_80,
    SCREEN_81,
    SCREEN_82,
    SCREEN_83,
    SCREEN_FACTORY_TEST,
    SCREEN_LAB_TEST,
    SCREEN_NUll,
} current_screen_t;

// 全局UI结构体定义
typedef struct
{
    lv_obj_t *top_layer; // 独立于屏幕的顶层容器
    lv_obj_t *screen_line_7;
    lv_obj_t *screen_line_6;
    lv_obj_t *screen_line_5;
    lv_obj_t *screen_bar_10;
    // 点数组作为结构体成员
    lv_point_t screen_line_7_points[2];
    lv_point_t screen_line_6_points[2];
    lv_point_t screen_line_5_points[2];
} Power_ui_t;
extern Power_ui_t Power_ui;

extern void Screen_init(void);
extern void Screens_KeysProcess(unsigned int EventID);
extern void KeyEnterevent_task(void);
extern void Screen_Movie(unsigned int Keyid);
extern void Screen_1_Movie(unsigned int Keyid);
extern void Screen_2_Movie(unsigned int Keyid);
extern void Screen_3_Movie(unsigned int Keyid);
extern void Screen_4_Movie(unsigned int Keyid);
extern void Screen_5_Movie(unsigned int Keyid);
extern void Screen_6_Movie(unsigned int Keyid);
extern void Screen_7_Movie(unsigned int Keyid);
extern void Screen_8_Movie(unsigned int Keyid);
extern void Screen_9_Movie(unsigned int Keyid);
extern void Screen_10_Movie(unsigned int Keyid);
extern void Screen_11_Movie(unsigned int Keyid);
extern void Screen_12_Movie(unsigned int Keyid);
extern void Screen_13_Movie(unsigned int Keyid);
extern void Screen_14_Movie(unsigned int Keyid);
extern void Screen_15_Movie(unsigned int Keyid);
extern void Screen_16_Movie(unsigned int Keyid);
extern void Screen_17_Movie(unsigned int Keyid);
extern void Screen_18_Movie(unsigned int Keyid);
extern void Screen_19_Movie(unsigned int Keyid);
extern void Screen_20_Movie(unsigned int Keyid);
extern void Screen_21_Movie(unsigned int Keyid);
extern void Screen_22_Movie(unsigned int Keyid);
extern void Screen_23_Movie(unsigned int Keyid);
extern void Screen_24_Movie(unsigned int Keyid);
extern void Screen_25_Movie(unsigned int Keyid);
extern void Screen_26_Movie(unsigned int Keyid);
extern void Screen_27_Movie(unsigned int Keyid);
extern void Screen_28_Movie(unsigned int Keyid);
extern void Screen_29_Movie(unsigned int Keyid);
extern void Screen_30_Movie(unsigned int Keyid);
extern void Screen_31_Movie(unsigned int Keyid);
extern void Screen_32_Movie(unsigned int Keyid);
extern void Screen_33_Movie(unsigned int Keyid);
extern void Screen_34_Movie(unsigned int Keyid);
extern void Screen_35_Movie(unsigned int Keyid);
extern void Screen_36_Movie(unsigned int Keyid);
extern void Screen_37_Movie(unsigned int Keyid);
extern void Screen_38_Movie(unsigned int Keyid);
extern void Screen_39_Movie(unsigned int Keyid);
extern void Screen_40_Movie(unsigned int Keyid);
extern void Screen_41_Movie(unsigned int Keyid);
extern void Screen_42_Movie(unsigned int Keyid);
extern void Screen_43_Movie(unsigned int Keyid);
extern void Screen_44_Movie(unsigned int Keyid);
extern void Screen_45_Movie(unsigned int Keyid);
extern void Screen_46_Movie(unsigned int Keyid);
extern void Screen_47_Movie(unsigned int Keyid);
extern void Screen_48_Movie(unsigned int Keyid);
extern void Screen_49_Movie(unsigned int Keyid);
extern void Screen_50_Movie(unsigned int Keyid);
extern void Screen_51_Movie(unsigned int Keyid);
extern void Screen_52_Movie(unsigned int Keyid);
extern void Screen_53_Movie(unsigned int Keyid);
extern void Screen_54_Movie(unsigned int Keyid);
extern void Screen_55_Movie(unsigned int Keyid);
extern void Screen_56_Movie(unsigned int Keyid);
extern void Screen_57_Movie(unsigned int Keyid);
extern void Screen_58_Movie(unsigned int Keyid);
extern void Screen_59_Movie(unsigned int Keyid);
extern void Screen_60_Movie(unsigned int Keyid);
extern void Screen_61_Movie(unsigned int Keyid);
extern void Screen_62_Movie(unsigned int Keyid);
extern void Screen_63_Movie(unsigned int Keyid);
extern void Screen_64_Movie(unsigned int Keyid);
extern void Screen_65_Movie(unsigned int Keyid);
extern void Screen_66_Movie(unsigned int Keyid);
extern void Screen_67_Movie(unsigned int Keyid);
extern void Screen_68_Movie(unsigned int Keyid);
extern void Screen_69_Movie(unsigned int Keyid);
extern void Screen_70_Movie(unsigned int Keyid);
extern void Screen_71_Movie(unsigned int Keyid);
extern void Screen_72_Movie(unsigned int Keyid);
extern void Screen_73_Movie(unsigned int Keyid);
extern void Screen_74_Movie(unsigned int Keyid);
extern void Screen_75_Movie(unsigned int Keyid);
extern void Screen_76_Movie(unsigned int Keyid);
extern void Screen_77_Movie(unsigned int Keyid);
extern void Screen_78_Movie(unsigned int Keyid);
extern void Screen_79_Movie(unsigned int Keyid);
extern void Screen_80_Movie(unsigned int Keyid);
extern void Screen_81_Movie(unsigned int Keyid);
extern void Screen_FactoryTest_Movie(unsigned int Keyid);
extern void Screen_LabTest_Movie(unsigned int Keyid);

extern void Screens_KeysPress(unsigned int EventID);
extern void Screen_KeyRelease(unsigned int Keyid);
extern void Screen_56_KeyRelease(unsigned int Keyid);
extern void Screen_11_KeyRelease(unsigned int Keyid);

extern void Screendate_preloading(void);
extern void Screen1date_preloading(void);
extern void Screen2date_preloading(void);
extern void Screen4date_preloading(void);
extern void Screen5date_preloading(void);
extern void Screen6date_preloading(void);
extern void Screen8date_preloading(void);
extern void Screen9date_preloading(void);
extern void Screen10date_preloading(void);
extern void Screen12date_preloading(void);
extern void Screen13date_preloading(void);
extern void Screen15date_preloading(void);
extern void Screen17date_preloading(void);
extern void Screen18date_preloading(void);
extern void Screen19date_preloading(void);
extern void Screen20date_preloading(void);
extern void Screen21date_preloading(void);
extern void Screen23date_preloading(void);
extern void Screen25date_preloading(void);
extern void Screen27date_preloading(void);
extern void Screen28date_preloading(void);
extern void Screen29date_preloading(void);
extern void Screen30date_preloading(void);
extern void Screen31date_preloading(void);
extern void Screen33date_preloading(void);
extern void Screen35date_preloading(void);
extern void Screen36date_preloading(void);
extern void Screen38date_preloading(void);
extern void Screen40date_preloading(void);
extern void Screen41date_preloading(void);
extern void Screen43date_preloading(void);
extern void Screen44date_preloading(void);
extern void Screen46date_preloading(void);
extern void Screen47date_preloading(void);
extern void Screen49date_preloading(void);
extern void Screen50date_preloading(void);
extern void Screen52date_preloading(void);
extern void Screen53date_preloading(void);
extern void Screen54date_preloading(void);
extern void Screen55date_preloading(void);
extern void Screen56date_preloading(void);
extern void Screen57date_preloading(void);
extern void Screen58date_preloading(void);
extern void Screen59date_preloading(void);
extern void Screen60date_preloading(void);
extern void Screen61date_preloading(void);
extern void Screen62date_preloading(void);
extern void Screen63date_preloading(void);
extern void Screen64date_preloading(void);
extern void Screen65date_preloading(void);
extern void Screen66date_preloading(void);
extern void Screen67date_preloading(void);
extern void Screen68date_preloading(void);
extern void Screen69date_preloading(void);
extern void Screen70date_preloading(void);
extern void Screen71date_preloading(void);
extern void Screen72date_preloading(void);
extern void Screen74date_preloading(void);
extern void Screen77date_preloading(void);
extern void Screen79date_preloading(void);
extern void Screen81date_preloading(void);
extern void Screen_FactoryTest_preloading(void);
extern void Screen_LabTest_preloading(void);

extern void Screens_AutoLoadProcess(void);
extern void Screen_AutoLoadProcess(void);
extern void Screen6_AutoLoadProcess(void);
extern void Screen13_AutoLoadProcess(void);
extern void Screen16_AutoLoadProcess(void);
extern void Screen22_AutoLoadProcess(void);
extern void Screen24_AutoLoadProcess(void);
extern void Screen26_AutoLoadProcess(void);
extern void Screen29_AutoLoadProcess(void);
extern void Screen30_AutoLoadProcess(void);
extern void Screen32_AutoLoadProcess(void);
extern void Screen35_AutoLoadProcess(void);
extern void Screen37_AutoLoadProcess(void);
extern void Screen40_AutoLoadProcess(void);
extern void Screen43_AutoLoadProcess(void);
extern void Screen46_AutoLoadProcess(void);
extern void Screen49_AutoLoadProcess(void);
extern void Screen52_AutoLoadProcess(void);
extern void Screen54_AutoLoadProcess(void);
extern void Screen56_AutoLoadProcess(void);
extern void Screen74_AutoLoadProcess(void);
extern void Screen77_AutoLoadProcess(void);
extern void Screen79_AutoLoadProcess(void);
extern void ScreenLabTest_AutoLoadProcess(void);
/*******************新增函数************************/
extern void GuiguiderInit(void);
extern void Power_Movie(void);
extern void Power_HideChargeUI(void);
extern void Power_SetBatteryColor(uint8_t battery_percent);
extern void Power_ui_init(void);
extern void Screens_printf(void);

extern void Screens_PowerShow(void);
extern void Screens_RFModeShow(void);
#endif
