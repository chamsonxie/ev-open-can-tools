# DBC CAN ID → 信号映射表

由 tesla_can.dbc + tesla_powertrain.dbc 生成

## 0x003 (dec: 3) — STW_ANGL_STAT  
> 方向盘角度 + 角速度
- DLC: 8  |  发送节点: STW  |  来源: `tesla_can.dbc`
  - `StW_Angl`: 位5, 长14, Motorola, unsigned, 比例=0.5, 偏移=-2048.0 NEO ✅ 有枚举值
  - `StW_AnglSpd`: 位21, 长14, Motorola, unsigned, 比例=0.5, 偏移=-2048.0 NEO ✅ 有枚举值
  - `StW_AnglSens_Stat`: 位33, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `StW_AnglSens_Id`: 位35, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `MC_STW_ANGL_STAT`: 位55, 长4, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `CRC_STW_ANGL_STAT`: 位63, 长8, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO

## 0x00E (dec: 14) — STW_ANGLHP_STAT  
> 方向盘角度高精度
- DLC: 8  |  发送节点: STW  |  来源: `tesla_can.dbc`
  - `StW_AnglHP`: 位5, 长14, Motorola, unsigned, 比例=0.1, 偏移=-819.2 NEO ✅ 有枚举值
  - `StW_AnglHP_Spd`: 位21, 长14, Motorola, unsigned, 比例=0.5, 偏移=-4096.0 NEO ✅ 有枚举值
  - `StW_AnglHP_Sens_Stat`: 位33, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `StW_AnglHP_Sens_Id`: 位35, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `MC_STW_ANGLHP_STAT`: 位55, 长4, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `CRC_STW_ANGLHP_STAT`: 位63, 长8, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO

## 0x045 (dec: 69) — STW_ACTN_RQ  
> 方向盘按键、ACC拨杆、雨刮
- DLC: 8  |  发送节点: STW  |  来源: `tesla_can.dbc`
  - `SpdCtrlLvr_Stat`: 位0, 长6, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `VSL_Enbl_Rq`: 位6, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `SpdCtrlLvrStat_Inv`: 位7, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `DTR_Dist_Rq`: 位8, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `TurnIndLvr_Stat`: 位16, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `HiBmLvr_Stat`: 位18, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `WprWashSw_Psd`: 位20, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `WprWash_R_Sw_Posn_V2`: 位22, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `StW_Lvr_Stat`: 位24, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `StW_Cond_Flt`: 位27, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `StW_Cond_Psd`: 位28, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `HrnSw_Psd`: 位30, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `StW_Sw00_Psd`: 位32, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `StW_Sw01_Psd`: 位33, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `StW_Sw02_Psd`: 位34, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `StW_Sw03_Psd`: 位35, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `StW_Sw04_Psd`: 位36, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `StW_Sw05_Psd`: 位37, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `StW_Sw06_Psd`: 位38, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `StW_Sw07_Psd`: 位39, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `StW_Sw08_Psd`: 位40, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `StW_Sw09_Psd`: 位41, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `StW_Sw10_Psd`: 位42, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `StW_Sw11_Psd`: 位43, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `StW_Sw12_Psd`: 位44, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `StW_Sw13_Psd`: 位45, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `StW_Sw14_Psd`: 位46, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `StW_Sw15_Psd`: 位47, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `WprSw6Posn`: 位48, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `MC_STW_ACTN_RQ`: 位52, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `CRC_STW_ACTN_RQ`: 位56, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO

## 0x068 (dec: 104) — MCU_locationStatus2
- DLC: 8  |  发送节点: MCU  |  来源: `tesla_can.dbc`
  - `MCU_elevation`: 位0, 长32, Intel, signed, 比例=0.1, 偏移=0.0 GTW
  - `MCU_navigonExpectedSpeed`: 位32, 长7, Intel, unsigned, 比例=1.0, 偏移=0.0 GTW

## 0x06D (dec: 109) — SBW_RQ_SCCM
- DLC: 4  |  发送节点: STW  |  来源: `tesla_can.dbc`
  - `StW_Sw_Stat3`: 位0, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `MsgTxmtId`: 位6, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `TSL_RND_Posn_StW`: 位8, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `TSL_P_Psd_StW`: 位12, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `MC_SBW_RQ_SCCM`: 位20, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `CRC_SBW_RQ_SCCM`: 位24, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO

## 0x101 (dec: 257) — GTW_epasControl  
> EPAS转向控制 (GTW请求)
- DLC: 3  |  发送节点: NEO  |  来源: `tesla_can.dbc`
  - `GTW_epasTuneRequest`: 位2, 长3, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `GTW_epasPowerMode`: 位6, 长4, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `GTW_epasEmergencyOn`: 位7, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `GTW_epasControlCounter`: 位11, 长4, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_epasLDWEnabled`: 位12, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `GTW_epasControlType`: 位15, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `GTW_epasControlChecksum`: 位23, 长8, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO

## 0x106 (dec: 262) — DI_torque1  
> 电机扭矩、转速、油门 (powertrain DBC)
- DLC: 8  |  发送节点: DI  |  来源: `tesla_powertrain.dbc`
  - `DI_torqueDriver`: 位0, 长13, Intel, signed, 比例=0.25, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_torque1Counter`: 位13, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `DI_torqueMotor`: 位16, 长13, Intel, signed, 比例=0.25, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_soptState`: 位29, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_motorRPM`: 位32, 长16, Intel, signed, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_pedalPos`: 位48, 长8, Intel, unsigned, 比例=0.4, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_torque1Checksum`: 位56, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO

## 0x108 (dec: 264) — DI_torque1  
> 电机扭矩、转速、油门踏板位置
- DLC: 8  |  发送节点: DI  |  来源: `tesla_can.dbc`
  - `DI_torqueDriver`: 位0, 长13, Intel, signed, 比例=0.25, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_torque1Counter`: 位13, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `DI_torqueMotor`: 位16, 长13, Intel, signed, 比例=0.25, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_soptState`: 位29, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_motorRPM`: 位32, 长16, Intel, signed, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_pedalPos`: 位48, 长8, Intel, unsigned, 比例=0.4, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_torque1Checksum`: 位56, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO

## 0x116 (dec: 278) — DI_torque2  
> 档位、车速、刹车 (powertrain DBC)
- DLC: 6  |  发送节点: DI  |  来源: `tesla_powertrain.dbc`
  - `DI_torqueEstimate`: 位0, 长12, Intel, signed, 比例=0.5, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_gear`: 位12, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_brakePedal`: 位15, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_vehicleSpeed`: 位16, 长12, Intel, unsigned, 比例=0.05, 偏移=-25.0 NEO ✅ 有枚举值
  - `DI_gearRequest`: 位28, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_torqueInterfaceFailure`: 位31, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_torque2Counter`: 位32, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `DI_brakePedalState`: 位36, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_epbParkRequest`: 位38, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_epbInterfaceReady`: 位39, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_torque2Checksum`: 位40, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO

## 0x118 (dec: 280) — DI_torque2  
> 档位 (P/R/N/D)、车速、刹车踏板
- DLC: 6  |  发送节点: DI  |  来源: `tesla_can.dbc`
  - `DI_torqueEstimate`: 位0, 长12, Intel, signed, 比例=0.5, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_gear`: 位12, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_brakePedal`: 位15, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_vehicleSpeed`: 位16, 长12, Intel, unsigned, 比例=0.05, 偏移=-25.0 NEO ✅ 有枚举值
  - `DI_gearRequest`: 位28, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_torqueInterfaceFailure`: 位31, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_torque2Counter`: 位32, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `DI_brakePedalState`: 位36, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_epbParkRequest`: 位38, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_epbInterfaceReady`: 位39, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_torque2Checksum`: 位40, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO

## 0x131 (dec: 305) — EPAS3P_sysStatus  
> EPAS3P转向状态
- DLC: 8  |  发送节点: NEO  |  来源: `tesla_can.dbc`
  - `EPAS_steeringRackForce`: 位1, 长10, Motorola, unsigned, 比例=50.0, 偏移=-25575.0 NEO
  - `EPAS_steeringFault`: 位2, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `EPAS_steeringReduced`: 位3, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `EPAS_currentTuneMode`: 位7, 长4, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `EPAS_torsionBarTorque`: 位19, 长12, Motorola, unsigned, 比例=0.01, 偏移=-20.5 NEO
  - `EPAS_eacErrorCode`: 位23, 长4, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `EPAS_internalSAS`: 位37, 长14, Motorola, unsigned, 比例=0.1, 偏移=-819.2 NEO
  - `EPAS_handsOnLevel`: 位39, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `EPAS_sysStatusCounter`: 位51, 长4, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `EPAS_eacStatus`: 位55, 长3, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `EPAS_sysStatusChecksum`: 位63, 长8, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO

## 0x135 (dec: 309) — ESP_135h  
> ESP刹车、ABS、车身稳定控制
- DLC: 5  |  发送节点: ESP  |  来源: `tesla_can.dbc`
  - `ESP_hillStartAssistActive`: 位1, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `ESP_absBrakeEvent`: 位2, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `ESP_brakeLamp`: 位3, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `ESP_brakeDiscWipingActive`: 位4, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `ESP_tcLampFlash`: 位5, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `ESP_espFaultLamp`: 位6, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `ESP_espLampFlash`: 位7, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `ESP_135hCounter`: 位11, 长4, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `ESP_stabilityControlSts`: 位14, 长3, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `ESP_tcOffLamp`: 位15, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `ESP_135hChecksum`: 位23, 长8, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `ESP_messagePumpService`: 位24, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `ESP_messagePumpFailure`: 位25, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `ESP_messageEBDFailure`: 位26, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `ESP_absFaultLamp`: 位27, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `ESP_tcDisabledByFault`: 位28, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `ESP_messageDynoModeActive`: 位29, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `ESP_hydraulicBoostEnabled`: 位30, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `ESP_espOffLamp`: 位31, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值

## 0x155 (dec: 341) — ESP_B  
> ESP车轮脉冲计数、车速
- DLC: 8  |  发送节点: ESP  |  来源: `tesla_can.dbc`
  - `ESP_wheelPulseCountFrL`: 位7, 长8, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO,EPAS
  - `ESP_wheelPulseCountFrR`: 位15, 长8, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO,EPAS
  - `ESP_wheelPulseCountReL`: 位23, 长8, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO,EPAS
  - `ESP_wheelPulseCountReR`: 位31, 长8, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO,EPAS
  - `ESP_BChecksum`: 位39, 长8, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO,EPAS
  - `ESP_vehicleSpeed`: 位47, 长16, Motorola, unsigned, 比例=0.00999999978, 偏移=0.0 NEO,EPAS
  - `ESP_vehicleSpeedQF`: 位57, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO,EPAS
  - `ESP_BCounter`: 位62, 长4, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO,EPAS

## 0x1F8 (dec: 504) — BrakeMessage  
> 刹车踏板状态 (powertrain DBC)
- DLC: 8  |  发送节点: XXX  |  来源: `tesla_powertrain.dbc`
  - `driverBrakeStatus`: 位2, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值

## 0x201 (dec: 513) — SDM1  
> 安全带状态 (驾驶员 + 乘客)
- DLC: 5  |  发送节点: GTW  |  来源: `tesla_can.dbc`
  - `SDM_bcklPassStatus`: 位3, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `SDM_bcklDrivStatus`: 位5, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO

## 0x209 (dec: 521) — DAS_longControl  
> DAS纵向控制 (急动度、加速度、速度限制)
- DLC: 8  |  发送节点: NEO  |  来源: `tesla_can.dbc`
  - `DAS_locMode`: 位0, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DAS_locState`: 位2, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DAS_locRequest`: 位5, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DAS_locJerkMin`: 位8, 长8, Intel, unsigned, 比例=0.034, 偏移=-8.67 NEO ✅ 有枚举值
  - `DAS_locJerkMax`: 位16, 长8, Intel, unsigned, 比例=0.034, 偏移=0.0 NEO ✅ 有枚举值
  - `DAS_locSpeed`: 位24, 长11, Intel, unsigned, 比例=0.1, 偏移=0.0 NEO ✅ 有枚举值
  - `DAS_locAccelMin`: 位35, 长9, Intel, unsigned, 比例=0.04, 偏移=-15.0 NEO ✅ 有枚举值
  - `DAS_locAccelMax`: 位44, 长9, Intel, unsigned, 比例=0.04, 偏移=-15.0 NEO ✅ 有枚举值
  - `DAS_longControlCounter`: 位53, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `DAS_longControlChecksum`: 位56, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO

## 0x20A (dec: 522) — BrakeMessage  
> 刹车踏板状态
- DLC: 8  |  发送节点: XXX  |  来源: `tesla_can.dbc`
  - `driverBrakeStatus`: 位2, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值

## 0x214 (dec: 532) — EPB_epasControl  
> EPB → EPAS控制 (EAC允许)
- DLC: 3  |  发送节点: EPB  |  来源: `tesla_can.dbc`
  - `EPB_epasEACAllow`: 位2, 长3, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO,EPAS
  - `EPB_epasControlCounter`: 位11, 长4, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO,EPAS
  - `EPB_epasControlChecksum`: 位23, 长8, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO,EPAS

## 0x218 (dec: 536) — MCU_chassisControl  
> MCU底盘控制 (FCW/AEB/LDW/变道等配置)
- DLC: 8  |  发送节点: GTW  |  来源: `tesla_can.dbc`
  - `MCU_dasDebugEnable`: 位0, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_parkBrakeRequest`: 位1, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_trailerModeCH`: 位3, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_fcwSensitivity`: 位4, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_fcwEnable`: 位6, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_latControlEnable`: 位8, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_accOvertakeEnable`: 位10, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_ldwEnable`: 位12, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_aebEnable`: 位14, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_bsdEnable`: 位16, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_ahlbEnable`: 位18, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_parkSetting`: 位20, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_pedalSafetyEnable`: 位22, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_frontDefrostReq_das`: 位24, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_autoParkRequest`: 位26, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_redLightStopSignEnable`: 位30, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_enableCreepTorqueCH`: 位32, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_narrowGarages`: 位33, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_rebootAutopilot`: 位34, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_enableAutowipers`: 位35, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_overPaintedUSS`: 位38, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_selfParkTune`: 位40, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_towModeEnable`: 位44, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_zeroSpeedConfirmed`: 位46, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_aesEnable`: 位48, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_autoLaneChangeEnable`: 位50, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_chassisControlCounter`: 位52, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_chassisControlChecksum`: 位56, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO

## 0x238 (dec: 568) — UI_driverAssistRoadSign  
> 道路标志 / 停止线 / 红绿灯 / 车队速度
- DLC: 8  |  发送节点: GTW  |  来源: `tesla_can.dbc / tesla_powertrain.dbc`
- **复用信号**: `UI_roadSign`

  **公共信号:**
  - `UI_roadSign`: 位0, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_splineLocConfidence`: 位40, 长7, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_splineID`: 位48, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 Vector__XXX
  - `UI_roadSignCounter`: 位52, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 Vector__XXX
  - `UI_roadSignChecksum`: 位56, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 Vector__XXX

  **Mux 0:**
  - `UI_dummyData`: 位8, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 Vector__XXX

  **Mux 1:**
  - `UI_stopSignStopLineDist`: 位8, 长10, Intel, unsigned, 比例=0.25, 偏移=-8.0 Vector__XXX
  - `UI_stopSignStopLineConf`: 位18, 长7, Intel, unsigned, 比例=1.0, 偏移=0.0 Vector__XXX

  **Mux 2:**
  - `UI_trafficLightStopLineDist`: 位8, 长10, Intel, unsigned, 比例=0.25, 偏移=-8.0 Vector__XXX
  - `UI_trafficLightStopLineConf`: 位18, 长7, Intel, unsigned, 比例=1.0, 偏移=0.0 Vector__XXX

  **Mux 3:**
  - `UI_baseMapSpeedLimitMPS`: 位8, 长8, Intel, unsigned, 比例=0.25, 偏移=0.0 DAS
  - `UI_bottomQrtlFleetSpeedMPS`: 位16, 长8, Intel, unsigned, 比例=0.25, 偏移=0.0 DAS
  - `UI_topQrtlFleetSpeedMPS`: 位24, 长8, Intel, unsigned, 比例=0.25, 偏移=0.0 DAS

  **Mux 4:**
  - `UI_meanFleetSplineSpeedMPS`: 位8, 长8, Intel, unsigned, 比例=0.25, 偏移=0.0 DAS
  - `UI_medianFleetSpeedMPS`: 位16, 长8, Intel, unsigned, 比例=0.25, 偏移=0.0 DAS
  - `UI_meanFleetSplineAccelMPS2`: 位24, 长8, Intel, unsigned, 比例=0.05, 偏移=-6.35 DAS
  - `UI_rampType`: 位32, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS

  **Mux 5:**
  - `UI_currSplineIdFull`: 位8, 长32, Intel, unsigned, 比例=1.0, 偏移=0.0 APP

## 0x239 (dec: 569) — DAS_lanes  
> 车道信息: 曲率、宽度、岔路、线使用情况
- DLC: 8  |  发送节点: NEO  |  来源: `tesla_can.dbc`
  - `DAS_leftLaneExists`: 位0, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `DAS_rightLaneExists`: 位1, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `DAS_virtualLaneWidth`: 位4, 长4, Intel, unsigned, 比例=0.3125, 偏移=2.0 NEO
  - `DAS_virtualLaneViewRange`: 位8, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `DAS_virtualLaneC0`: 位16, 长8, Intel, unsigned, 比例=0.035, 偏移=-3.5 NEO
  - `DAS_virtualLaneC1`: 位24, 长8, Intel, unsigned, 比例=0.0016, 偏移=-0.2 NEO
  - `DAS_virtualLaneC2`: 位32, 长8, Intel, unsigned, 比例=2e-05, 偏移=-0.0025 NEO
  - `DAS_virtualLaneC3`: 位40, 长8, Intel, unsigned, 比例=2.4e-07, 偏移=-3e-05 NEO
  - `DAS_leftLineUsage`: 位48, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DAS_rightLineUsage`: 位50, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DAS_leftFork`: 位52, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DAS_rightFork`: 位54, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DAS_lanesCounter`: 位60, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO

## 0x246 (dec: 582) — UI_solarData  
> 太阳位置 (方位角、仰角)
- DLC: 5  |  发送节点: GTW  |  来源: `tesla_can.dbc`
  - `UI_solarAzimuthAngle`: 位0, 长16, Intel, signed, 比例=1.0, 偏移=0.0 APP
  - `UI_solarAzimuthAngleCarRef`: 位16, 长9, Intel, signed, 比例=1.0, 偏移=0.0 APP
  - `UI_isSunUp`: 位25, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 Vector__XXX
  - `UI_solarElevationAngle`: 位32, 长8, Intel, signed, 比例=1.0, 偏移=0.0 APP

## 0x256 (dec: 598) — DI_state  
> DI系统状态: 巡航、速度、防盗 (powertrain DBC)
- DLC: 8  |  发送节点: DI  |  来源: `tesla_powertrain.dbc`
  - `DI_systemState`: 位0, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_vehicleHoldState`: 位3, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_proximity`: 位6, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `DI_driveReady`: 位7, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `DI_regenLight`: 位8, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `DI_state`: 位9, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_cruiseState`: 位12, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_analogSpeed`: 位16, 长12, Intel, unsigned, 比例=0.1, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_immobilizerState`: 位28, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_speedUnits`: 位31, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_cruiseSet`: 位32, 长9, Intel, unsigned, 比例=0.5, 偏移=0.0 NEO
  - `DI_aebState`: 位41, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_stateCounter`: 位44, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `DI_digitalSpeed`: 位48, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_stateChecksum`: 位56, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO

## 0x283 (dec: 643) — BODY_R1  
> 车身: 门锁、灯光、雨刮、温度、安全带
- DLC: 8  |  发送节点: GTW  |  来源: `tesla_can.dbc`
  - `DrRLtch_FL_Stat`: 位1, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DrRLtch_FR_Stat`: 位3, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DrRLtch_RL_Stat`: 位5, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DrRLtch_RR_Stat`: 位7, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DL_RLtch_Stat`: 位9, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `EngHd_Stat`: 位11, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `VTA_Alm_Actv`: 位13, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `LgtSens_Twlgt`: 位18, 长3, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `LgtSens_SNA`: 位19, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `LgtSens_Tunnel`: 位20, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `LgtSens_Flt`: 位21, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `LgtSens_Night`: 位22, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `ADL_LoBm_On_Rq`: 位23, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `Trlr_Stat`: 位25, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `Hrn_On`: 位26, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `Term54_Actv`: 位27, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MPkBrk_Stat`: 位28, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `WprOutsdPkPosn`: 位29, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `LoBm_On_Rq`: 位32, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `HiBm_On`: 位33, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `IrLmp_D_Lt_Flt`: 位34, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `IrLmp_P_Rt_Flt`: 位35, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `LoBm_D_Lt_Flt`: 位36, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `LoBm_P_Rt_Flt`: 位37, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `RevGr_Engg`: 位39, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `AirTemp_Insd`: 位47, 长8, Motorola, unsigned, 比例=0.25, 偏移=0.0 NEO ✅ 有枚举值
  - `Bckl_Sw_RL_Stat_SAM_R`: 位49, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `Bckl_Sw_RR_Stat_SAM_R`: 位51, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `Bckl_Sw_RM_Stat_SAM_R`: 位53, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `StW_Cond_Stat`: 位55, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `AirTemp_Outsd`: 位63, 长8, Motorola, unsigned, 比例=0.5, 偏移=-40.0 NEO ✅ 有枚举值

## 0x2B8 (dec: 696) — UI_radarMapData  
> 雷达目标地图数据
- DLC: 8  |  发送节点: GTW  |  来源: `tesla_can.dbc`
  - `UI_radarTargetDx`: 位0, 长8, Intel, unsigned, 比例=1.0, 偏移=-95.0 DAS
  - `UI_radarTargetDxEnd`: 位8, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_radarTargetTrustMap`: 位16, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_radarEnableBraking`: 位17, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_radarMapDataCounter`: 位52, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_radarMapDataChecksum`: 位56, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS

## 0x2B9 (dec: 697) — DAS_control  
> DAS控制 (设定速度、ACC状态、AEB、急动度/加速度限制)
- DLC: 8  |  发送节点: NEO  |  来源: `tesla_can.dbc`
  - `DAS_setSpeed`: 位0, 长12, Intel, unsigned, 比例=0.1, 偏移=0.0 NEO ✅ 有枚举值
  - `DAS_accState`: 位12, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DAS_aebEvent`: 位16, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DAS_jerkMin`: 位18, 长9, Intel, unsigned, 比例=0.03, 偏移=-15.232 NEO ✅ 有枚举值
  - `DAS_jerkMax`: 位27, 长8, Intel, unsigned, 比例=0.059, 偏移=0.0 NEO ✅ 有枚举值
  - `DAS_accelMin`: 位35, 长9, Intel, unsigned, 比例=0.04, 偏移=-15.0 NEO ✅ 有枚举值
  - `DAS_accelMax`: 位44, 长9, Intel, unsigned, 比例=0.04, 偏移=-15.0 NEO ✅ 有枚举值
  - `DAS_controlCounter`: 位53, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `DAS_controlChecksum`: 位56, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO

## 0x2BF (dec: 703) — DAS_control  
> DAS控制 (powertrain DBC)
- DLC: 8  |  发送节点: GTW  |  来源: `tesla_powertrain.dbc`
  - `DAS_setSpeed`: 位0, 长12, Intel, unsigned, 比例=0.1, 偏移=0.0 DI,PM,APS ✅ 有枚举值
  - `DAS_accState`: 位12, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 DI,PM,APS ✅ 有枚举值
  - `DAS_aebEvent`: 位16, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 DI,PM,APS ✅ 有枚举值
  - `DAS_jerkMin`: 位18, 长9, Intel, unsigned, 比例=0.03, 偏移=-15.232 DI,PM,APS ✅ 有枚举值
  - `DAS_jerkMax`: 位27, 长8, Intel, unsigned, 比例=0.059, 偏移=0.0 DI,PM,APS ✅ 有枚举值
  - `DAS_accelMin`: 位35, 长9, Intel, unsigned, 比例=0.04, 偏移=-15.0 DI,PM,APS ✅ 有枚举值
  - `DAS_accelMax`: 位44, 长9, Intel, unsigned, 比例=0.04, 偏移=-15.0 DI,PM,APS ✅ 有枚举值
  - `DAS_controlCounter`: 位53, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 DI,PM,APS
  - `DAS_controlChecksum`: 位56, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 DI,PM,APS

## 0x2C8 (dec: 712) — UI_roadCurvature  
> 道路曲率 (C0-C3 多项式系数)
- DLC: 8  |  发送节点: GTW  |  来源: `tesla_can.dbc`
  - `UI_roadCurvC0`: 位0, 长11, Intel, signed, 比例=0.02, 偏移=0.0 DAS
  - `UI_roadCurvC1`: 位11, 长10, Intel, signed, 比例=0.00075, 偏移=0.0 DAS
  - `UI_roadCurvC2`: 位21, 长14, Intel, signed, 比例=7.5e-06, 偏移=0.0 DAS
  - `UI_roadCurvC3`: 位35, 长13, Intel, signed, 比例=3e-08, 偏移=0.0 DAS
  - `UI_roadCurvRange`: 位48, 长6, Intel, unsigned, 比例=4.0, 偏移=0.0 DAS
  - `UI_roadCurvHealth`: 位54, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_roadCurvChecksum`: 位56, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 Vector__XXX

## 0x2D8 (dec: 728) — UI_csaOfframpCurvature  
> 匝道出口曲率
- DLC: 8  |  发送节点: GTW  |  来源: `tesla_can.dbc`
  - `UI_csaOfframpCurvC2`: 位0, 长16, Intel, signed, 比例=1e-06, 偏移=0.0 DAS
  - `UI_csaOfframpCurvC3`: 位16, 长16, Intel, signed, 比例=4e-09, 偏移=0.0 DAS
  - `UI_csaOfframpCurvRange`: 位32, 长8, Intel, unsigned, 比例=2.0, 偏移=0.0 DAS
  - `UI_csaOfframpCurvCounter`: 位40, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 Vector__XXX
  - `UI_csaOfframpCurvUsingTspline`: 位48, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_csaOfframpCurvReserved`: 位49, 长7, Intel, unsigned, 比例=1.0, 偏移=0.0 Vector__XXX
  - `UI_csaOfframpCurvChecksum`: 位56, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 Vector__XXX

## 0x2E8 (dec: 744) — UI_csaRoadCurvature  
> 道路曲率 (CSA)
- DLC: 8  |  发送节点: GTW  |  来源: `tesla_can.dbc`
  - `UI_csaRoadCurvC2`: 位0, 长16, Intel, signed, 比例=1e-06, 偏移=0.0 DAS
  - `UI_csaRoadCurvC3`: 位16, 长16, Intel, signed, 比例=4e-09, 偏移=0.0 DAS
  - `UI_csaRoadCurvRange`: 位32, 长8, Intel, unsigned, 比例=2.0, 偏移=0.0 DAS
  - `UI_csaRoadCurvCounter`: 位40, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 Vector__XXX
  - `UI_csaRoadCurvUsingTspline`: 位48, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_csaRoadCurvReserved`: 位49, 长7, Intel, unsigned, 比例=1.0, 偏移=0.0 Vector__XXX
  - `UI_csaRoadCurvChecksum`: 位56, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 Vector__XXX

## 0x2F8 (dec: 760) — UI_gpsVehicleSpeed  
> GPS车速、航向、地图限速
- DLC: 8  |  发送节点: GTW  |  来源: `tesla_can.dbc`
  - `UI_gpsHDOP`: 位0, 长8, Intel, unsigned, 比例=0.1, 偏移=0.0 DAS
  - `UI_gpsVehicleHeading`: 位8, 长16, Intel, unsigned, 比例=0.0078125, 偏移=0.0 DAS
  - `UI_gpsVehicleSpeed`: 位24, 长16, Intel, unsigned, 比例=0.00390625, 偏移=0.0 Vector__XXX
  - `UI_userSpeedOffset`: 位40, 长6, Intel, unsigned, 比例=1.0, 偏移=-30.0 DAS
  - `UI_mapSpeedLimitUnits`: 位46, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS ✅ 有枚举值
  - `UI_userSpeedOffsetUnits`: 位47, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS ✅ 有枚举值
  - `UI_mppSpeedLimit`: 位48, 长5, Intel, unsigned, 比例=5.0, 偏移=0.0 DAS
  - `UI_gpsNmeaMIA`: 位53, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS

## 0x30C (dec: 780) — DriverSeat  
> 驾驶员座椅占用 + 安全带
- DLC: 8  |  发送节点: XXX  |  来源: `tesla_can.dbc`
  - `occupancyStatus`: 位16, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `buckleStatus`: 位19, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值

## 0x318 (dec: 792) — GTW_carState  
> GTW车辆状态: 车门、时间、启动状态、灯光
- DLC: 8  |  发送节点: GTW  |  来源: `tesla_can.dbc`
  - `YEAR`: 位0, 长7, Intel, unsigned, 比例=1.0, 偏移=2000.0 NEO ✅ 有枚举值
  - `CERRD`: 位7, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `MONTH`: 位8, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DOOR_STATE_FL`: 位12, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DOOR_STATE_FR`: 位14, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `SECOND`: 位16, 长6, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DOOR_STATE_RL`: 位22, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `Hour`: 位24, 长5, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DOOR_STATE_RR`: 位29, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DAY`: 位32, 长5, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `MINUTE`: 位40, 长6, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `BOOT_STATE`: 位46, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `GTW_updateInProgress`: 位48, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DOOR_STATE_FrontTrunk`: 位50, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `MCU_factoryMode`: 位52, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `MCU_transportModeOn`: 位53, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `BC_headLightLStatus`: 位55, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `BC_headLightRStatus`: 位57, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `BC_indicatorLStatus`: 位59, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `BC_indicatorRStatus`: 位61, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO

## 0x338 (dec: 824) — UI_status  
> UI状态: 触屏、音频、蜂窝、WiFi、摄像头、温度
- DLC: 8  |  发送节点: GTW  |  来源: `tesla_can.dbc`
  - `UI_touchActive`: 位0, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 IC
  - `UI_audioActive`: 位1, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 IC
  - `UI_bluetoothActive`: 位2, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 IC
  - `UI_cellActive`: 位3, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 IC
  - `UI_displayReady`: 位4, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 IC
  - `UI_gpsActive`: 位5, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 IC
  - `UI_wifiConnected`: 位6, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 IC,APP
  - `UI_systemActive`: 位7, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 IC
  - `UI_xmActive`: 位8, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 IC
  - `UI_displayOn`: 位9, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 IC,APP
  - `UI_readyForDrive`: 位10, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 IC
  - `UI_cellConnected`: 位11, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 IC,APP
  - `UI_vpnActive`: 位12, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 IC,APP
  - `UI_wifiActive`: 位13, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 IC
  - `UI_cameraActive`: 位14, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 IC,APP
  - `UI_usbActive`: 位15, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 IC
  - `UI_screenshotActive`: 位16, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 IC,APP
  - `UI_monitorModemPower`: 位17, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 Vector__XXX
  - `UI_factoryReset`: 位18, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 Vector__XXX
  - `UI_cellNetworkTechnology`: 位20, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 APP
  - `UI_tegraCoreTemperature`: 位24, 长8, Intel, unsigned, 比例=1.0, 偏移=-64.0 IC
  - `UI_tegraAmbientTemperature`: 位32, 长8, Intel, unsigned, 比例=1.0, 偏移=-64.0 IC
  - `UI_googleWifiUsages`: 位40, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 Vector__XXX
  - `UI_autopilotTrial`: 位48, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 APP
  - `UI_cellSignalBars`: 位50, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 APP
  - `UI_hardwareType`: 位53, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 APP
  - `UI_developmentCar`: 位55, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 Vector__XXX
  - `UI_cellReceiverPower`: 位56, 长8, Intel, unsigned, 比例=1.0, 偏移=-128.0 APP

## 0x348 (dec: 840) — GTW_status  
> GTW状态: 驱动就绪、刹车、驾驶员在位
- DLC: 8  |  发送节点: GTW  |  来源: `tesla_can.dbc`
  - `GTW_driveRailReq`: 位0, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_brakePressed`: 位1, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_driverPresent`: 位2, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_notEnough12VForDrive`: 位3, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_icPowerOff`: 位4, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_driverIsLeaving`: 位5, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_accGoingDown`: 位6, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_driveGoingDown`: 位7, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_accRailReq`: 位8, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_hvacRailReq`: 位9, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_preconditionRequest`: 位10, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_hvacGoingDown`: 位11, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_statusCounter`: 位51, 长4, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_statusChecksum`: 位63, 长8, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO

## 0x368 (dec: 872) — DI_state  
> DI状态: 巡航、速度、防盗、AEB
- DLC: 8  |  发送节点: DI  |  来源: `tesla_can.dbc`
  - `DI_systemState`: 位0, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_vehicleHoldState`: 位3, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_proximity`: 位6, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `DI_driveReady`: 位7, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `DI_regenLight`: 位8, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `DI_state`: 位9, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_cruiseState`: 位12, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_analogSpeed`: 位16, 长12, Intel, unsigned, 比例=0.1, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_immobilizerState`: 位28, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_speedUnits`: 位31, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_cruiseSet`: 位32, 长9, Intel, unsigned, 比例=0.5, 偏移=0.0 NEO
  - `DI_aebState`: 位41, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_stateCounter`: 位44, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `DI_digitalSpeed`: 位48, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `DI_stateChecksum`: 位56, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO

## 0x370 (dec: 880) — EPAS_sysStatus  
> EPAS转向: 扭矩、手握检测、调校模式、故障
- DLC: 8  |  发送节点: EPAS  |  来源: `tesla_can.dbc`
  - `EPAS_steeringRackForce`: 位1, 长10, Motorola, unsigned, 比例=50.0, 偏移=-25575.0 NEO ✅ 有枚举值
  - `EPAS_steeringFault`: 位2, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `EPAS_steeringReduced`: 位3, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `EPAS_currentTuneMode`: 位7, 长4, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `EPAS_torsionBarTorque`: 位19, 长12, Motorola, unsigned, 比例=0.01, 偏移=-20.5 NEO ✅ 有枚举值
  - `EPAS_eacErrorCode`: 位23, 长4, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `EPAS_internalSAS`: 位37, 长14, Motorola, unsigned, 比例=0.1, 偏移=-819.200012 NEO
  - `EPAS_handsOnLevel`: 位39, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `EPAS_sysStatusCounter`: 位51, 长4, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `EPAS_eacStatus`: 位55, 长3, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `EPAS_sysStatusChecksum`: 位63, 长8, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO

## 0x388 (dec: 904) — MCU_clusterBacklightRequest  
> 仪表盘背光 + 就绪状态
- DLC: 3  |  发送节点: NEO  |  来源: `tesla_can.dbc`
  - `MCU_clusterReadyForPowerOff`: 位5, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_clusterReadyForDrive`: 位6, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO ✅ 有枚举值
  - `MCU_clusterBacklightOn`: 位7, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `MCU_clusterBrightnessLevel`: 位8, 长8, Intel, unsigned, 比例=0.5, 偏移=0.0 NEO

## 0x389 (dec: 905) — DAS_status2  
> DAS状态2: ACC限速、PMM、LSS、碰撞预警
- DLC: 8  |  发送节点: XXX  |  来源: `tesla_can.dbc`
  - `DAS_accSpeedLimit`: 位0, 长10, Intel, unsigned, 比例=0.2, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_pmmObstacleSeverity`: 位10, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_pmmLoggingRequest`: 位13, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_activationFailureStatus`: 位14, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_pmmUltrasonicsFaultReason`: 位16, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_pmmRadarFaultReason`: 位19, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_pmmSysFaultReason`: 位21, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_pmmCameraFaultReason`: 位24, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_ACC_report`: 位26, 长5, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_lssState`: 位31, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_radarTelemetry`: 位34, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_robState`: 位36, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_driverInteractionLevel`: 位38, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_ppOffsetDesiredRamp`: 位40, 长8, Intel, unsigned, 比例=0.01, 偏移=-1.28 XXX ✅ 有枚举值
  - `DAS_longCollisionWarning`: 位48, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_status2Counter`: 位52, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX
  - `DAS_status2Checksum`: 位56, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX

## 0x398 (dec: 920) — GTW_carConfig  
> GTW车辆配置: HW级别、四驱、雷达、AP类型
- DLC: 8  |  发送节点: GTW  |  来源: `tesla_can.dbc`
  - `GTW_performanceConfig`: 位2, 长3, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_fourWheelDrive`: 位4, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_unknown1`: 位5, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_dasHw`: 位7, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_parkAssistInstalled`: 位9, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_forwardRadarHw`: 位11, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_airSuspensionInstalled`: 位14, 长3, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_unknown2`: 位15, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_country`: 位23, 长16, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_parkSensorGeometryType`: 位33, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_rhd`: 位34, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_bodyControlsType`: 位35, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_radarPosition`: 位39, 长4, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_rearCornerRadarHw`: 位41, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_frontCornerRadarHw`: 位43, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_epasType`: 位45, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_chassisType`: 位47, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_wheelType`: 位52, 长5, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_rearSeatControllerMask`: 位55, 长3, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_euVehicle`: 位56, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_foldingMirrorsInstalled`: 位57, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_brakeHwType`: 位59, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_autopilot`: 位61, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO
  - `GTW_unknown3`: 位63, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 NEO

## 0x399 (dec: 921) — AutopilotStatus  
> 自动驾驶状态: 激活状态、手握检测、变道、召唤
- DLC: 8  |  发送节点: XXX  |  来源: `tesla_can.dbc`
  - `autopilotStatus`: 位0, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_blindSpotRearLeft`: 位4, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_blindSpotRearRight`: 位6, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_fusedSpeedLimit`: 位8, 长5, Intel, unsigned, 比例=5.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_suppressSpeedWarning`: 位13, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_summonObstacle`: 位14, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX
  - `DAS_summonClearedGate`: 位15, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX
  - `DAS_visionOnlySpeedLimit`: 位16, 长5, Intel, unsigned, 比例=5.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_heaterState`: 位21, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_forwardCollisionWarning`: 位22, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_autoparkReady`: 位24, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_autoParked`: 位25, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX
  - `DAS_autoparkWaitingForBrake`: 位26, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX
  - `DAS_summonFwdLeashReached`: 位28, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX
  - `DAS_summonRvsLeashReached`: 位29, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX
  - `DAS_sideCollisionAvoid`: 位30, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_sideCollisionWarning`: 位32, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_sideCollisionInhibit`: 位34, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_csaState`: 位35, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_laneDepartureWarning`: 位37, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_fleetSpeedState`: 位40, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_autopilotHandsOnState`: 位42, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_autoLaneChangeState`: 位46, 长5, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_summonAvailable`: 位51, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX
  - `DAS_statusCounter`: 位52, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX
  - `DAS_statusChecksum`: 位56, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX

## 0x3C8 (dec: 968) — UI_driverAssistMapData  
> 地图数据: 限速、国家代码、导航路线、自动泊车
- DLC: 8  |  发送节点: GTW  |  来源: `tesla_can.dbc`
  - `UI_mapSpeedLimitDependency`: 位0, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_roadClass`: 位3, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_inSuperchargerGeofence`: 位6, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_mapSpeedUnits`: 位7, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_mapSpeedLimit`: 位8, 长5, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_mapSpeedLimitType`: 位13, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_countryCode`: 位16, 长10, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_streetCount`: 位26, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_gpsRoadMatch`: 位28, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_navRouteActive`: 位29, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_parallelAutoparkEnabled`: 位30, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_perpendicularAutoparkEnabled`: 位31, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_nextBranchDist`: 位32, 长5, Intel, unsigned, 比例=10.0, 偏移=0.0 DAS
  - `UI_controlledAccess`: 位37, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_nextBranchLeftOffRamp`: 位38, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_nextBranchRightOffRamp`: 位39, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_rejectLeftLane`: 位40, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_rejectRightLane`: 位41, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_rejectHPP`: 位42, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_rejectNav`: 位43, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_rejectLeftFreeSpace`: 位44, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_rejectRightFreeSpace`: 位45, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_rejectAutosteer`: 位46, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_rejectHandsOn`: 位47, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_acceptBottsDots`: 位48, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_autosteerRestricted`: 位49, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_pmmEnabled`: 位50, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_scaEnabled`: 位51, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_mapDataCounter`: 位52, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_mapDataChecksum`: 位56, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS

## 0x3D8 (dec: 984) — MCU_locationStatus  
> GPS经纬度坐标
- DLC: 8  |  发送节点: MCU  |  来源: `tesla_can.dbc`
  - `MCU_latitude`: 位0, 长28, Intel, signed, 比例=1e-06, 偏移=0.0 NEO
  - `MCU_longitude`: 位28, 长29, Intel, signed, 比例=1e-06, 偏移=0.0 NEO
  - `MCU_gpsAccuracy`: 位57, 长7, Intel, unsigned, 比例=0.2, 偏移=0.0 NEO

## 0x3E8 (dec: 1000) — UI_driverAssistControl  
> UI自动驾驶控制: 召唤、AP开关、脱手禁用
- DLC: 8  |  发送节点: GTW  |  来源: `tesla_can.dbc`
  - `UI_autopilotControlRequest`: 位0, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_ulcStalkConfirm`: 位1, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_summonHeartbeat`: 位2, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_curvSpeedAdaptDisable`: 位4, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_dasDeveloper`: 位5, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_enableVinAssociation`: 位6, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_lssLkaEnabled`: 位7, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_lssLdwEnabled`: 位8, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_autoSummonEnable`: 位10, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_exceptionListEnable`: 位11, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 APP
  - `UI_roadCheckDisable`: 位12, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_driveOnMapsEnable`: 位13, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_handsOnRequirementDisable`: 位14, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_forksEnable`: 位15, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_fuseLanesDisable`: 位16, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_fuseHPPDisable`: 位17, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_fuseVehiclesDisable`: 位18, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_enableNextGenACC`: 位19, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 APP
  - `UI_visionSpeedType`: 位20, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 APP
  - `UI_curvatureDatabaseOnly`: 位22, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_lssElkEnabled`: 位23, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_summonExitType`: 位24, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_summonEntryType`: 位26, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_selfParkRequest`: 位28, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS,PARK
  - `UI_summonReverseDist`: 位32, 长6, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_undertakeAssistEnable`: 位38, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_adaptiveSetSpeedEnable`: 位39, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_drivingSide`: 位40, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_enableClipTelemetry`: 位42, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 APP
  - `UI_enableTripTelemetry`: 位43, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 APP
  - `UI_enableRoadSegmentTelemetry`: 位44, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 APP
  - `UI_followNavRouteEnable`: 位46, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 APP
  - `UI_ulcSpeedConfig`: 位48, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 APP
  - `UI_ulcBlindSpotConfig`: 位50, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 APP
  - `UI_autopilotAlwaysOn`: 位52, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 APP
  - `UI_accFromZero`: 位53, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 APP
  - `UI_alcOffHighwayEnable`: 位54, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 APP
  - `UI_validationLoop`: 位55, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 APP
  - `UI_ulcOffHighway`: 位56, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 APP
  - `UI_enableNavRouteCSA`: 位57, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 APP
  - `UI_enableCutinExperiments`: 位58, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 APP
  - `UI_source3D`: 位60, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 APP
  - `UI_enableVisionOnlyStops`: 位63, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 APP

## 0x3E9 (dec: 1001) — DAS_bodyControls  
> DAS车身控制: 灯光、雨刮、转向灯
- DLC: 8  |  发送节点: XXX  |  来源: `tesla_can.dbc`
  - `DAS_headlightRequest`: 位0, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_hazardLightRequest`: 位2, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_wiperSpeed`: 位4, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_turnIndicatorRequest`: 位8, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_highLowBeamDecision`: 位10, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_highLowBeamOffReason`: 位12, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_turnIndicatorRequestReason`: 位16, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX ✅ 有枚举值
  - `DAS_bodyControlsCounter`: 位52, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX
  - `DAS_bodyControlsChecksum`: 位56, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 XXX

## 0x3EE (dec: 1006) — UI_autopilotControl  
> UI自动驾驶控制 (mux0: HOV/ALC配置, mux1: 摄像头遮挡)
- DLC: 8  |  发送节点: GTW  |  来源: `tesla_can.dbc`
- **复用信号**: `UI_autopilotControlIndex`

  **公共信号:**
  - `UI_autopilotControlIndex`: 位0, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 APP,APS

  **Mux 0:**
  - `UI_hovEnabled`: 位3, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 APP,APS
  - `UI_donDisableAutoWiperDuration`: 位4, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 APP,APS
  - `UI_donDisableOnAutoWiperSpeed`: 位7, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 APP,APS
  - `UI_blindspotMinSpeed`: 位11, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 APP,APS
  - `UI_blindspotDistance`: 位15, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 APP,APS
  - `UI_blindspotTTC`: 位18, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 APP,APS
  - `UI_donStopEndOfRampBuffer`: 位21, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 APP,APS
  - `UI_donDisableCutin`: 位24, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 APP,APS
  - `UI_donMinGoreWidthForAbortMap`: 位25, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 APP,APS
  - `UI_donAlcProgGoreAbortThres`: 位29, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 APP,APS
  - `UI_donMinGoreWidthForAbortNotMap`: 位33, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 APP,APS
  - `UI_alcDisableUltrasonicCheck`: 位37, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 APP,APS
  - `UI_alcUltrasonicDistance`: 位38, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 APP,APS
  - `UI_alcUltrasonicWaitTime`: 位42, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 APP,APS
  - `UI_alcEgoLeadingReactionAccel`: 位48, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 APP,APS
  - `UI_alcMergIntervalRearDHyst`: 位50, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 APP,APS
  - `UI_alcMergingIntervalHeadwayHyst`: 位52, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 APP,APS
  - `UI_alcAssertivenessRate`: 位54, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 APP,APS
  - `UI_alcViewRangeSensitivity`: 位56, 长2, Intel, unsigned, 比例=1.0, 偏移=0.0 APP,APS

  **Mux 1:**
  - `UI_camBlockLaneCheckDisable`: 位3, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 APP,APS
  - `UI_camBlockLaneCheckThreshold`: 位4, 长6, Intel, unsigned, 比例=0.01587, 偏移=0.0 APP,APS
  - `UI_camBlockBlurDisable`: 位10, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 APP,APS
  - `UI_camBlockBlurThreshold`: 位11, 长6, Intel, unsigned, 比例=0.01587, 偏移=0.0 APP,APS

## 0x428 (dec: 1064) — UI_telemetryControl  
> 遥测控制 (TCR配置)
- DLC: 8  |  发送节点: GTW  |  来源: `tesla_can.dbc`
  - `UI_TCR_enable`: 位0, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_TCR_moveStateStanding`: 位1, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_TCR_moveStateStopped`: 位2, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_TCR_moveStateMoving`: 位3, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_TCR_moveStateIndeterm`: 位4, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_TCR_classConstElem`: 位5, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_TCR_classMovingPed`: 位6, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_TCR_classMovingTwoWheel`: 位7, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_TCR_classMovingFourWheel`: 位8, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_TCR_classUnknown`: 位9, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_TCR_downSampleFactor`: 位16, 长4, Intel, unsigned, 比例=1.0, 偏移=0.0 Vector__XXX
  - `UI_TCR_wExist`: 位24, 长5, Intel, unsigned, 比例=1.0, 偏移=0.0 Vector__XXX
  - `UI_TCR_vehSpeed`: 位32, 长8, Intel, unsigned, 比例=1.0, 偏移=0.0 Vector__XXX
  - `UI_TCR_minRCS`: 位40, 长8, Intel, unsigned, 比例=0.25, 偏移=-14.0 Vector__XXX
  - `UI_TCR_maxDy`: 位48, 长5, Intel, unsigned, 比例=0.5, 偏移=0.0 Vector__XXX
  - `UI_TCR_maxObjects`: 位56, 长5, Intel, unsigned, 比例=1.0, 偏移=0.0 Vector__XXX
  - `UI_TCR_maxRoadClass`: 位61, 长3, Intel, unsigned, 比例=1.0, 偏移=0.0 Vector__XXX

## 0x438 (dec: 1080) — UI_driverAssistAnonDebugParams  
> 匿名调试参数 (视觉速度滑块)
- DLC: 8  |  发送节点: GTW  |  来源: `tesla_can.dbc`
  - `UI_anonDebugParam1`: 位0, 长7, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_anonDebugFlag1`: 位7, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_anonDebugParam2`: 位8, 长7, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_anonDebugFlag2`: 位15, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_anonDebugParam3`: 位16, 长7, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_anonDebugFlag3`: 位23, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_anonDebugParam4`: 位24, 长7, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_anonDebugFlag4`: 位31, 长1, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_anonDebugParam5`: 位32, 长7, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_anonDebugParam6`: 位40, 长7, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_anonDebugParam7`: 位48, 长7, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS
  - `UI_visionSpeedSlider`: 位56, 长7, Intel, unsigned, 比例=1.0, 偏移=0.0 DAS

## 0x488 (dec: 1160) — DAS_steeringControl  
> DAS转向控制 (角度请求)
- DLC: 4  |  发送节点: NEO  |  来源: `tesla_can.dbc`
  - `DAS_steeringAngleRequest`: 位6, 长15, Motorola, unsigned, 比例=0.1, 偏移=-1638.35 EPAS ✅ 有枚举值
  - `DAS_steeringHapticRequest`: 位7, 长1, Motorola, unsigned, 比例=1.0, 偏移=0.0 EPAS ✅ 有枚举值
  - `DAS_steeringControlCounter`: 位19, 长4, Motorola, unsigned, 比例=1.0, 偏移=0.0 EPAS
  - `DAS_steeringControlType`: 位23, 长2, Motorola, unsigned, 比例=1.0, 偏移=0.0 EPAS ✅ 有枚举值
  - `DAS_steeringControlChecksum`: 位31, 长8, Motorola, unsigned, 比例=1.0, 偏移=0.0 EPAS

---
### 图例
- DLC: 数据长度 (字节)
- BO: 字节序 (Intel = LSB在前, Motorola = MSB在前)
- 比例/偏移: 物理值 = 原始值 × 比例 + 偏移
- ✅ 有枚举值: 信号有命名值映射 (来自 DBC 的 VAL_ 表)
