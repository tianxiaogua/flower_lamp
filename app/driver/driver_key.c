// https://blog.csdn.net/qq_40102829/article/details/108927767

#include "driver_key.h"


 /*******************************************************************************
  * @brief  按键处理函数
  * @param  无
  * @retval 无
  ******************************************************************************/
void key_Process(KEY_PROCESS_TypeDef *key)
{
	switch(key->flag.key_state){
	    //【按键按下】
		case KEY_STATE_PRESS :
			//在按键按下时从0开始计时，直到超时
			if(key->time_continus < KEY_TIME_OUT){key->time_continus++;}

			//发生长按事件
			if(key->time_continus > KEY_TIME_CONTINUS){
				if(key->event_current_type != EVENT_NONE_CLICK){ //识别长按前的按键事件
					if(key->press_cnt > 1){key->press_cnt--;}
					key->flag.once_event = 1;
				}
				else{
					key->flag.press_time = 1;					//【0：短按/1：长按】识别此次为长按
					key->flag.key_state = KEY_STATE_IDLE;		//主动结束按下动作，进入无动作状态，保证在长按的按下过程中就识别出长按事件
				
					key->event_current_type = EVENT_LONG_CLICK;	//分配当前按键事件类型
					key->flag.once_event = 1;					//产生按键事件
					key->press_cnt = 1;
					key->time_idle = KEY_TIME_OUT;				//按键空闲时间超时
				}
			}
            
			//按下时进行一次判断
			if(key->flag.check){
				key->flag.check = 0;
				if(!key->flag.press_time){					//判断上一次按键类型
					//判断上一次按键动作空闲时间
					if(key->time_idle < KEY_TIME_IDLE){		//若上一次按键动作后的空闲时间在规定时间内，说明发生了连击事件，一次完整的按键事件还未结束
						key->press_cnt++;
					}else{key->press_cnt = 1;}
				}
				key->flag.press_time = 0;					//【0：短按/1：长按】此次为短按
			}
		break;
			
	    //【按键松开】，若是长按，不会进入该判断
		case KEY_STATE_RELEASE: 
			//在按键按下时从0开始计时，直到超时
			if(key->time_idle < KEY_TIME_OUT){key->time_idle++;}
			
			//松开时进行一次判断
			if(key->flag.check){
				key->flag.check = 0;
				
				//判断此次按键动作
				if(!key->flag.press_time){								///长按会屏蔽短按
					if(key->press_cnt > 1){								//连击事件
						key->event_current_type = EVENT_DOUBLE_CLICK;	//分配按键事件类型
					}
					else{												//单击事件
						key->event_current_type = EVENT_SHORT_CLICK;		//分配按键事件类型
						key->press_cnt = 1;								//连击次数置1
					}
				}
			}
			
			//按键松开后判断此次按键动作后的空闲时间，从而判断此次动作是否结束
			if(key->time_idle > KEY_TIME_IDLE){			//空闲时间超时，认为一次完整的按键事件结束
				if(!key->flag.press_time){				//松开前是短按标志，则产生按键事件，这里是为了屏蔽长按后的松手动作
					key->flag.once_event = 1;			//产生按键事件
					key->flag.key_state = KEY_STATE_IDLE;//进入无动作状态
				}
			}
		break;
	//【按键无动作】
		default : 
		break;
	}

}


 /*******************************************************************************
  * @brief  按键配置初始化函数，在主函数调用
  * @param  无
  * @retval 无
  ******************************************************************************/
void key_config_init(KEY_PROCESS_TypeDef *key)
{
	//初始化
	key->flag.check = 0;
	key->flag.key_state = KEY_STATE_IDLE;
	key->flag.once_event = 0;
	key->flag.press_time = 0;
	
	key->event_current_type = EVENT_NONE_CLICK;
	key->event_previous_type = EVENT_NONE_CLICK;
	key->press_cnt = 1;

	key->time_continus = 0;
	key->time_idle = KEY_TIME_OUT;
}


 /*******************************************************************************
  * @brief  使用模板，按键事件判断例子，在主函数循环调用该函数 或 通过定时器定时查询
			将printf替换成需要处理的函数即可使用单击、连击、长按等按键功能
  * @param  无
  * @retval 无
  ******************************************************************************/
uint8 key_scan(KEY_PROCESS_TypeDef *key)
{
    uint8 key_status = 0;
	if(key->flag.once_event){
		key->flag.once_event = 0;
		
		switch(key->event_current_type){
			case EVENT_SHORT_CLICK : key_status = EVENT_SHORT_CLICK, GUA_LOGI("单击\r\n");break;
			case EVENT_DOUBLE_CLICK : key_status = EVENT_DOUBLE_CLICK, GUA_LOGI("%d连击\r\n",key->press_cnt);break;
			case EVENT_LONG_CLICK : key_status = EVENT_LONG_CLICK, GUA_LOGI("长按\r\n");break;
			default: GUA_LOGI("none\r\n");break;
		}
		//事件处理完需更新前态和现态
		key->event_previous_type = key->event_current_type;
		key->event_current_type = EVENT_NONE_CLICK;
	}
    return key_status;
}

