#include "Screens.h"
#include "Syscfg.h"
#include "gui_guider.h"
#include "events_init.h"
#include <stdio.h>
#include <stdlib.h>
#include "lvgl.h"
#include "keyboard.h"
#include "Calendar.h"
extern lv_indev_t *indev_keypad;
extern lv_ui guider_ui; // 声明 界面对象
current_screen_t current_screen, Pre_screen, Mem_Screen, Mem_Screen2;
uint16_t Autotick = 0;

void GuiguiderInit(void)
{
    extern uint8_t System_FactoryTestFlag;

    if (System_FactoryTestFlag == 0xFF)
    {
        current_screen = SCREEN_FACTORY_TEST;
        init_scr_del_flag(&guider_ui);       // 初始化 *_del 标志
        setup_scr_Factory_Test(&guider_ui);  // 生成 Factory_Test 界面
        lv_scr_load(guider_ui.Factory_Test); // 加载该界面
        events_init(&guider_ui);
        Screen_FactoryTest_preloading();
    }
    else
    {
        setup_ui(&guider_ui); // 初始化主界面
        events_init(&guider_ui);
        Screen_init();
    }
}
void KeyEnterevent_task(void)
{
    unsigned int EventID = NULL;
    EventID = g_Keyboard.KeyCmd;
    g_Keyboard.KeyCmd = 0;
    switch (EventID)
    {
    case KEYENTER_TRG:
        Screens_KeysProcess(EventID);
        break;
    case KEYFORWARD_TRG:
        Screens_KeysProcess(EventID);
        break;
    case KEYBEHIND_TRG:
        Screens_KeysProcess(EventID);
        break;
    case KEYLEFT_TRG:
        Screens_KeysProcess(EventID);
        break;
    case KEYRIGHT_TRG:
        Screens_KeysProcess(EventID);
        break;
    case KEYRETURN_TRG:
        Screens_KeysProcess(EventID);
        break;
    case KEYMENU_TRG:
        Screens_KeysProcess(EventID);
        break;
    case KEYFORWARD_2S:
        Screens_KeysProcess(EventID);
        break;
    case KEYBEHIND_2S:
        Screens_KeysProcess(EventID);
        break;
    case KEYENTER_2S:
        Screens_KeysProcess(EventID);
        break;
    case RETURN_2S:
        Screens_KeysProcess(EventID);
        break;
    case KEYENTER_6S:
        Screens_KeysProcess(EventID);
        break;
    default:
        break;
    }
}
void Screens_KeysProcess(unsigned int EventID)
{

    switch (current_screen)
    {
    case SCREEN:
        Screen_Movie(EventID);
        break;
    case SCREEN_1:
        Screen_1_Movie(EventID);
        break;
    case SCREEN_2:
        Screen_2_Movie(EventID);
        break;
    case SCREEN_3:
        Screen_3_Movie(EventID);
        break;
    case SCREEN_4:
        Screen_4_Movie(EventID);
        break;
    case SCREEN_5:
        Screen_5_Movie(EventID);
        break;
    case SCREEN_6:
        Screen_6_Movie(EventID);
        break;
    case SCREEN_7:
        Screen_7_Movie(EventID);
        break;
    case SCREEN_8:
        Screen_8_Movie(EventID);
        break;
    case SCREEN_9:
        Screen_9_Movie(EventID);
        break;
    case SCREEN_10:
        Screen_10_Movie(EventID);
        break;
    case SCREEN_11:
        Screen_11_Movie(EventID);
        break;
    case SCREEN_12:
        Screen_12_Movie(EventID);
        break;
    case SCREEN_13:
        Screen_13_Movie(EventID);
        break;
    case SCREEN_14:
        Screen_14_Movie(EventID);
        break;
    case SCREEN_15:
        Screen_15_Movie(EventID);
        break;
    case SCREEN_16:
        Screen_16_Movie(EventID);
        break;
    case SCREEN_17:
        Screen_17_Movie(EventID);
        break;
    case SCREEN_18:
        Screen_18_Movie(EventID);
        break;
    case SCREEN_19:
        Screen_19_Movie(EventID);
        break;
    case SCREEN_20:
        Screen_20_Movie(EventID);
        break;
    case SCREEN_21:
        Screen_21_Movie(EventID);
        break;
    case SCREEN_22:
        Screen_22_Movie(EventID);
        break;
    case SCREEN_23:
        Screen_23_Movie(EventID);
        break;
    case SCREEN_24:
        Screen_24_Movie(EventID);
        break;
    case SCREEN_25:
        Screen_25_Movie(EventID);
        break;
    case SCREEN_26:
        Screen_26_Movie(EventID);
        break;
    case SCREEN_27:
        Screen_27_Movie(EventID);
        break;
    case SCREEN_28:
        Screen_28_Movie(EventID);
        break;
    case SCREEN_29:
        Screen_29_Movie(EventID);
        break;
    case SCREEN_30:
        Screen_30_Movie(EventID);
        break;
    case SCREEN_31:
        Screen_31_Movie(EventID);
        break;
    case SCREEN_32:
        Screen_32_Movie(EventID);
        break;
    case SCREEN_33:
        Screen_33_Movie(EventID);
    case SCREEN_34:
        Screen_34_Movie(EventID);
        break;
    case SCREEN_35:
        Screen_35_Movie(EventID);
        break;
    case SCREEN_36:
        Screen_36_Movie(EventID);
        break;
    case SCREEN_37:
        Screen_37_Movie(EventID);
        break;
    case SCREEN_38:
        Screen_38_Movie(EventID);
        break;
    case SCREEN_39:
        Screen_39_Movie(EventID);
        break;
    case SCREEN_40:
        Screen_40_Movie(EventID);
        break;
    case SCREEN_41:
        Screen_41_Movie(EventID);
        break;
    case SCREEN_42:
        Screen_42_Movie(EventID);
        break;
    case SCREEN_43:
        Screen_43_Movie(EventID);
        break;
    case SCREEN_44:
        Screen_44_Movie(EventID);
        break;
    case SCREEN_45:
        Screen_45_Movie(EventID);
        break;
    case SCREEN_46:
        Screen_46_Movie(EventID);
        break;
    case SCREEN_47:
        Screen_47_Movie(EventID);
        break;
    case SCREEN_48:
        Screen_48_Movie(EventID);
        break;
    case SCREEN_49:
        Screen_49_Movie(EventID);
        break;
    case SCREEN_50:
        Screen_50_Movie(EventID);
        break;
    case SCREEN_51:
        Screen_51_Movie(EventID);
        break;
    case SCREEN_52:
        Screen_52_Movie(EventID);
        break;
    case SCREEN_53:
        Screen_53_Movie(EventID);
        break;
    case SCREEN_54:
        Screen_54_Movie(EventID);
        break;
    case SCREEN_55:
        Screen_55_Movie(EventID);
        break;
    case SCREEN_56:
        Screen_56_Movie(EventID);
        break;
    case SCREEN_57:
        Screen_57_Movie(EventID);
        break;
    case SCREEN_58:
        Screen_58_Movie(EventID);
        break;
    case SCREEN_59:
        Screen_59_Movie(EventID);
        break;
    case SCREEN_60:
        Screen_60_Movie(EventID);
        break;
    case SCREEN_61:
        Screen_61_Movie(EventID);
        break;
    case SCREEN_62:
        Screen_62_Movie(EventID);
        break;
    case SCREEN_63:
        Screen_63_Movie(EventID);
        break;
    case SCREEN_64:
        Screen_64_Movie(EventID);
        break;
    case SCREEN_65:
        Screen_65_Movie(EventID);
        break;
    case SCREEN_66:
        Screen_66_Movie(EventID);
        break;
    case SCREEN_67:
        Screen_67_Movie(EventID);
        break;
    case SCREEN_68:
        Screen_68_Movie(EventID);
        break;
    case SCREEN_69:
        Screen_69_Movie(EventID);
        break;
    case SCREEN_70:
        Screen_70_Movie(EventID);
        break;
    case SCREEN_71:
        Screen_71_Movie(EventID);
        break;
    case SCREEN_72:
        Screen_72_Movie(EventID);
        break;
    case SCREEN_73:
        Screen_73_Movie(EventID);
        break;
    case SCREEN_74:
        Screen_74_Movie(EventID);
        break;
    case SCREEN_75:
        Screen_75_Movie(EventID);
        break;
    case SCREEN_76:
        Screen_76_Movie(EventID);
        break;
    case SCREEN_77:
        Screen_77_Movie(EventID);
        break;
    case SCREEN_78:
        Screen_78_Movie(EventID);
        break;
    case SCREEN_79:
        Screen_79_Movie(EventID);
        break;
    case SCREEN_80:
        Screen_80_Movie(EventID);
        break;
    case SCREEN_81:
        Screen_81_Movie(EventID);
        break;
    case SCREEN_FACTORY_TEST:
        Screen_FactoryTest_Movie(EventID);
        break;
    #ifdef LAB_TEST
    case SCREEN_LAB_TEST:
        Screen_LabTest_Movie(EventID);
        break;
    #endif
    default:
        break;
    }
}

void Screens_AutoLoadProcess(void)
{
    Screen_AutoLoadProcess();
    Screen6_AutoLoadProcess();
    Screen13_AutoLoadProcess();
    Screen22_AutoLoadProcess();
    Screen24_AutoLoadProcess();
    Screen26_AutoLoadProcess();
    Screen29_AutoLoadProcess();
    Screen32_AutoLoadProcess();
    Screen43_AutoLoadProcess();
    Screen46_AutoLoadProcess();
    Screen49_AutoLoadProcess();
    Screen52_AutoLoadProcess();
    Screen54_AutoLoadProcess();
    Screen56_AutoLoadProcess();
    Screen74_AutoLoadProcess();
    Screen77_AutoLoadProcess();
    Screen79_AutoLoadProcess();
    #ifdef LAB_TEST
    ScreenLabTest_AutoLoadProcess();
    #endif
}
void Screens_KeysPress(unsigned int EventID)
{

    switch (current_screen)
    {
    case SCREEN:
        Screen_KeyRelease(EventID);
        break;
    // case SCREEN_6:
    //     Screen_6_KeyRelease(EventID);
    //     break;
    case SCREEN_11:
        // Screen_11_KeyRelease(EventID);
        Screen_KeyRelease(EventID);
        break;
    case SCREEN_37:
        Screen_KeyRelease(EventID);
        break;
    case SCREEN_56:
        Screen_56_KeyRelease(EventID);
        break;
    case SCREEN_76:
        Screen_11_KeyRelease(EventID); // 借用
    case SCREEN_78:
        Screen_11_KeyRelease(EventID); // 借用
        break;
    default:
        break;
    }
}
void Screens_printf(void)
{
    lv_obj_t *curcreeens = lv_scr_act();
    if (curcreeens == NULL)
    {
        printf("Current screen is NULL!\n");
        return;
    }
    if (curcreeens == guider_ui.screen)
        printf("Current screen is screen!\n");

    else if (curcreeens == guider_ui.screen_1)
        printf("Current screen is screen_1!\n");
    else if (curcreeens == guider_ui.screen_2)
        printf("Current screen is screen_2!\n");
}
