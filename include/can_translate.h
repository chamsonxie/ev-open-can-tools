#pragma once
#include "can_frame_types.h"
#include "can_helpers.h"
#include <cstdio>
#include <cstring>
inline bool formatCanTranslation(uint32_t id, const uint8_t* data, uint8_t dlc, char* out, size_t outLen) {
  if (!out || outLen == 0) return false;
  out[0] = '\0';
  auto app = [&](const char* s) { strncat(out, s, outLen - strlen(out) - 1); };
  if (id == 280 || id == 0x118) {
    if (dlc < 3) return false;
    uint8_t b2 = data[2], b4 = data[4], b6 = data[6];
    uint8_t gr = (b2 >> 5) & 0x07;
    const char* g = (gr==1?"P":gr==2?"R":gr==3?"N":gr==4?"D":gr==7?"SNA":gr==0?"无效":"未知");
    bool parked = (gr==0 || gr==1 || gr==7);
    float acc = b4 * 0.4f;
    uint8_t br = (b2 >> 3) & 0x03;
    const char* bs = (br==0?"OFF":br==1?"ON":"INVALID");
    bool aca = (b6 & 0x04) != 0;
    char tmp[128];
    snprintf(tmp, sizeof(tmp), "车速：(录制中未包含)，档位：%s（%s），油门：%.1f%%，刹车：%s，ACA：%s", g, parked?"驻车":"非驻车", acc, bs, aca?"是":"否");
    strlcpy(out, tmp, outLen);
    return true;
  }
  if (id == 1016 || id == 0x3F8) {
    if (dlc < 6) return false;
    uint8_t spr = (data[3] >> 4) & 0x0F;
    char tmp[80]; snprintf(tmp, sizeof(tmp), "召唤请求：%s（spr=%u）", spr?"是":"否", spr);
    strlcpy(out, tmp, outLen);
    return true;
  }
  if (id == 1021 || id == 0x3FD) {
    if (dlc < 1) return false;
    uint8_t mux = data[0] & 0x07;
    char tmp[32]; snprintf(tmp, sizeof(tmp), "UI_selfPark mux=%u", mux);
    strlcpy(out, tmp, outLen);
    return true;
  }
  if (id == 2047 || id == 0x7FF) {
    if (dlc < 6) return false;
    uint8_t mux = data[0] & 0x07;
    if (mux == 2) {
      uint8_t ap = (data[5] >> 2) & 0x07;
      const char* apn = (ap==0?"NONE":ap==1?"HIGHWAY":ap==2?"ENHANCED":ap==3?"SELF_DRIVING":ap==4?"BASIC":"未知");
      char tmp[80]; snprintf(tmp, sizeof(tmp), "AP状态：%s", apn);
      strlcpy(out, tmp, outLen);
      return true;
    }
    char tmp[32]; snprintf(tmp, sizeof(tmp), "GTW mux=%u", mux);
    strlcpy(out, tmp, outLen);
    return true;
  }
  if (id == 0x116) {
    if (dlc < 6) return false;
    uint16_t raw = ((uint16_t)data[2] << 4) | (data[3] >> 4);
    float mph = raw * 0.05f - 25.0f;
    float kph = mph * 1.60934f;
    uint8_t gr = (data[1] >> 4) & 0x07;
    const char* g = (gr==1?"P":gr==2?"R":gr==3?"N":gr==4?"D":gr==7?"SNA":gr==0?"无效":"未知");
    char tmp[96];
    if (mph > -10 && mph < 200) {
      snprintf(tmp, sizeof(tmp), "车速：%.1f km/h (%.1f mph)，档位：%s", kph, mph, g);
    } else {
      snprintf(tmp, sizeof(tmp), "车速：(无效)，档位：%s", g);
    }
    strlcpy(out, tmp, outLen);
    return true;
  }
  return false;
}
