#pragma once
#ifdef ESP_PLATFORM
#include "platform/espidf_runtime.h"
#else
#include <Arduino.h>
#endif
#include <stddef.h>
#include <stdint.h>

#ifndef ESP_PLATFORM
static const char DASH_HTML[] PROGMEM = R"HTML(
<!DOCTYPE html>
<html lang="zh-CN" data-theme="dark">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1,user-scalable=no">
<title>EV开源CAN工具</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
[data-theme="dark"]{
  --bg:#0d0d0d;--bg2:var(--bg);--card:#161616;--card2:#1e1e1e;
  --bd:#2a2a2a;--bd2:#333;
  --tx:#f0f0f0;--tx2:#999;--tx3:#555;
  --acc:#5b8fff;--accBg:rgba(91,143,255,.1);--accBd:rgba(91,143,255,.25);
  --ok:#3dba72;--okBg:rgba(61,186,114,.1);
  --err:#ff4f4f;--errBg:rgba(255,79,79,.08);--errBd:rgba(255,79,79,.2);
  --warn:#f5a623;
}
[data-theme="light"]{
  --bg:#f5f5f5;--bg2:var(--bg);--card:#fff;--card2:#f0f0f0;
  --bd:#e0e0e0;--bd2:#ccc;
  --tx:#111;--tx2:#555;--tx3:#999;
  --acc:#2563eb;--accBg:rgba(37,99,235,.08);--accBd:rgba(37,99,235,.2);
  --ok:#16a34a;--okBg:rgba(22,163,74,.08);
  --err:#dc2626;--errBg:rgba(220,38,38,.06);--errBd:rgba(220,38,38,.18);
  --warn:#d97706;
}
html{scroll-behavior:smooth}
body{background:var(--bg);color:var(--tx);font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',sans-serif;
  min-height:100vh;max-width:480px;margin:0 auto;font-size:14px;line-height:1.5;
  transition:background .2s,color .2s}

/* 头部 */
.hdr{padding:20px 16px 0;display:flex;flex-direction:column;gap:4px}
.hdr-top{display:flex;align-items:center;justify-content:space-between}
.hdr-left{display:flex;align-items:center;gap:8px;flex-wrap:wrap;min-width:0}
.hdr-title{font-size:20px;font-weight:700;color:var(--tx)}
.hw-badge{padding:3px 8px;border-radius:5px;font-size:11px;font-weight:600;
  background:var(--accBg);border:1px solid var(--accBd);color:var(--acc)}
.gtw-badge{padding:3px 8px;border-radius:5px;font-size:11px;font-weight:600;
  background:var(--card);border:1px solid var(--bd2);color:var(--tx2)}
.gtw-badge.known{color:var(--ok);border-color:rgba(61,186,114,.25);background:var(--okBg)}
.theme-btn{padding:6px 10px;border:1px solid var(--bd2);border-radius:8px;
  background:var(--card);color:var(--tx2);font-size:12px;cursor:pointer;
  display:flex;align-items:center;gap:4px;transition:all .2s}
.theme-btn:hover{border-color:var(--acc);color:var(--acc)}
.hdr-status{display:flex;align-items:center;gap:6px;font-size:12px;color:var(--tx2)}
.sdot{width:7px;height:7px;border-radius:50%;flex-shrink:0;transition:all .4s}
.dot-on{background:var(--ok);box-shadow:0 0 8px var(--ok)}
.dot-off{background:var(--err)}
.dot-warn{background:var(--warn)}

/* FPS栏 */
.fps-bar{margin:14px 16px 0;height:3px;background:var(--bd);border-radius:2px;overflow:hidden}
.fps-fill{height:100%;background:var(--acc);border-radius:2px;transition:width .5s;width:0%}

/* 状态网格 */
.stat-grid{display:grid;grid-template-columns:1fr 1fr 1fr;gap:8px;margin:14px 16px 0}
.stat{background:var(--card);border:1px solid var(--bd);border-radius:10px;padding:10px 12px}
.stat-lbl{font-size:10px;color:var(--tx3);text-transform:uppercase;letter-spacing:.8px;margin-bottom:3px}
.stat-val{font-size:14px;font-weight:600;color:var(--tx)}
.v-ok{color:var(--ok)}.v-err{color:var(--err)}.v-acc{color:var(--acc)}.v-dim{color:var(--tx3)}.v-warn{color:var(--warn)}
.stat-wide{grid-column:span 3}

/* 分隔线 */
hr{border:none;border-top:1px solid var(--bd);margin:16px}

/* 卡片 */
.card{background:var(--card);border:1px solid var(--bd);border-radius:12px;padding:16px;margin:0 16px 12px;overflow:hidden}
.card-hdr{display:grid;grid-template-columns:minmax(0,1fr) auto auto;align-items:center;column-gap:8px;margin-bottom:14px}
.card-title{font-size:13px;font-weight:600;color:var(--tx);text-transform:uppercase;letter-spacing:.5px;min-width:0}
.card-meta{font-size:11px;color:var(--tx3);justify-self:end;text-align:right;min-width:0}
.card-min-btn{padding:4px 8px;font-size:10px;justify-self:end}
.card.collapsed{padding-bottom:12px}
.card.collapsed .card-hdr{margin-bottom:0}
.card.collapsed>:not(.card-hdr){display:none !important}
.subsec{margin-top:14px;padding:12px 24px;border-top:1px solid var(--bd)}
.subsec:first-child{margin-top:0;padding-top:0;border-top:none}
.subsec-head{display:grid;grid-template-columns:minmax(0,1fr) auto auto;align-items:center;column-gap:8px;margin-bottom:8px}
.subsec-title{font-size:13px;font-weight:600;color:var(--tx);min-width:0}
.title-help{display:inline-flex;align-items:center;justify-content:center;width:16px;height:16px;margin-left:6px;border:1px solid var(--bd2);border-radius:50%;font-size:10px;font-weight:700;color:var(--tx3);cursor:pointer;vertical-align:middle;line-height:1;background:transparent}
.title-help:hover{border-color:var(--accBd);color:var(--acc);background:var(--accBg)}
.info-box{margin-bottom:10px;padding:10px 12px;background:var(--bg2);border:1px solid var(--bd);border-radius:9px;font-size:12px;color:var(--tx3);line-height:1.6}
.info-box a{color:var(--acc);text-decoration:none}
.inline-help-panel{display:none;margin:8px 0 0;padding:10px 12px;background:var(--bg2);border:1px solid var(--bd);border-radius:9px;font-size:12px;color:var(--tx3);line-height:1.6}
.inline-help-panel.show{display:block}
.subsec-meta{font-size:11px;color:var(--tx3);justify-self:end;text-align:right;min-width:0}
.subsec-btn{padding:4px 8px;font-size:10px;justify-self:end}
.subsec.collapsed .subsec-head{margin-bottom:0}
.subsec.collapsed .subsec-body{display:none}



/* 速度选项 */
.pills{display:flex;gap:6px;flex-wrap:wrap}
/* 设置行 */
.setting-row{display:flex;align-items:center;justify-content:space-between;
  padding:12px 0;border-bottom:1px solid var(--bd)}
.setting-row:last-of-type{border-bottom:none;padding-bottom:0}
.setting-row:first-of-type{padding-top:0}
.setting-info{flex:1;min-width:0}
.setting-name{font-size:13px;font-weight:500;color:var(--tx)}
.setting-desc{font-size:11px;color:var(--tx3);margin-top:2px}
.hw4-only.hidden{display:none}

/* 开关 */
.tgl{position:relative;width:44px;height:24px;flex-shrink:0;margin-left:12px}
.tgl input{opacity:0;width:0;height:0;position:absolute}
.tgl-track{position:absolute;inset:0;background:var(--bd2);border-radius:24px;cursor:pointer;transition:all .22s}
.tgl-thumb{position:absolute;top:3px;left:3px;width:18px;height:18px;background:#fff;
  border-radius:50%;transition:all .22s;box-shadow:0 1px 3px rgba(0,0,0,.3)}
.tgl input:checked~.tgl-track{background:var(--acc)}
.tgl input:checked~.tgl-track .tgl-thumb{transform:translateX(20px)}
.tgl input:disabled~.tgl-track{opacity:.35;cursor:not-allowed}

/* 嗅探器 */
.sniff-ctrl{display:flex;gap:6px;margin-bottom:8px}
.sniff-input{flex:1;background:var(--bg);border:1px solid var(--bd);border-radius:8px;
  padding:7px 10px;color:var(--tx);font-size:12px;font-family:inherit;transition:border .2s}
.sniff-input{width:100%;min-width:0;box-sizing:border-box;}
.sniff-input:focus{outline:none;border-color:var(--acc)}
.sniff-input::placeholder{color:var(--tx3)}
.sniff-btn{padding:7px 12px;background:transparent;border:1px solid var(--bd);border-radius:8px;
  color:var(--tx2);font-size:11px;font-weight:600;cursor:pointer;transition:all .18s;font-family:inherit}
.sniff-btn.paused{border-color:var(--warn);color:var(--warn)}
.sniff-btn:hover:not(.paused){border-color:var(--bd2);color:var(--tx)}
.sniff-box{background:var(--bg);border:1px solid var(--bd);border-radius:9px;
  max-height:250px;overflow-y:auto;font-family:'SF Mono','Courier New',monospace}
.sniff-box::-webkit-scrollbar{width:4px}
.sniff-box::-webkit-scrollbar-thumb{background:var(--bd2);border-radius:4px}
.sniff-row{display:grid;grid-template-columns:38px 72px 1fr;gap:8px;
  padding:6px 10px;border-bottom:1px solid var(--bd);font-size:11px;align-items:start}
.sniff-row:last-child{border-bottom:none}
.sniff-row.hi{border-left:2px solid var(--acc);padding-left:8px}
.s-ts{color:var(--tx3);font-size:10px;padding-top:1px}
.s-id{color:var(--acc);font-weight:700}
.s-data{color:var(--tx2);word-break:break-all}
.s-name{color:var(--ok);font-size:10px;margin-top:2px}

/* 错误标志 */
.eflg-row{display:flex;flex-wrap:wrap;gap:5px;margin-top:10px}
.eflg-pill{padding:3px 8px;border-radius:5px;font-size:10px;font-weight:600;letter-spacing:.3px}
.eflg-ok{background:var(--okBg);color:var(--ok)}
.eflg-warn{background:rgba(245,166,35,.1);color:var(--warn)}
.eflg-err{background:var(--errBg);color:var(--err)}

/* 多路复用表 */
.mux-tbl{width:100%;border-collapse:collapse;font-size:12px;margin-top:10px}
.mux-tbl th{color:var(--tx3);font-size:10px;text-transform:uppercase;letter-spacing:.8px;
  text-align:left;padding:4px 8px;border-bottom:1px solid var(--bd);font-weight:500}
.mux-tbl td{padding:5px 8px;color:var(--tx2);border-bottom:1px solid var(--bd)}
.mux-tbl tr:last-child td{border-bottom:none}
.mux-tbl td:first-child{color:var(--acc);font-weight:600}

/* 上次写入检查 */
.probe-status{font-size:13px;font-weight:600}
.probe-note{font-size:11px;color:var(--tx3);line-height:1.6;margin-top:10px}
.probe-block{margin-top:12px;padding-top:12px;border-top:1px solid var(--bd)}
.probe-meta{font-size:11px;color:var(--tx3);margin-bottom:4px}
.probe-label{font-size:10px;color:var(--tx3);text-transform:uppercase;letter-spacing:.8px;margin-bottom:6px}
.probe-hex{font-family:'SF Mono','Courier New',monospace;font-size:12px;color:var(--tx2);word-break:break-all}

/* 按钮 */
.btn-row{display:flex;gap:8px;margin-top:14px}
.btn{flex:1;padding:10px;border:1px solid;border-radius:9px;background:transparent;
  font-family:inherit;font-size:12px;font-weight:600;cursor:pointer;transition:all .18s;letter-spacing:.3px}
.btn-stop{border-color:var(--errBd);color:var(--err)}
.btn-stop:hover{background:var(--errBg)}
.btn-reboot{border-color:var(--bd2);color:var(--tx2)}
.btn-reboot:hover{border-color:var(--acc);color:var(--acc)}

/* 确认弹窗 */
.modal-backdrop{position:fixed;inset:0;display:none;align-items:center;justify-content:center;
  padding:16px;background:rgba(0,0,0,.55);z-index:9999}
.modal-card{width:min(100%,360px);background:var(--card);border:1px solid var(--bd2);
  border-radius:12px;padding:16px;box-shadow:0 16px 40px rgba(0,0,0,.35)}
.modal-title{font-size:14px;font-weight:700;color:var(--tx)}
.modal-msg{margin-top:8px;font-size:12px;color:var(--tx2);line-height:1.6;white-space:pre-wrap}
.modal-actions{display:flex;justify-content:flex-end;gap:8px;margin-top:14px}
.modal-btn-primary{background:var(--accBg);border-color:var(--accBd);color:var(--acc)}
.modal-btn-primary:hover{background:var(--acc);color:#fff}

/* OTA上传 */
.ota-drop{border:2px dashed var(--bd2);border-radius:10px;padding:24px 16px;
  text-align:center;cursor:pointer;transition:all .2s;background:var(--bg)}
.ota-drop:hover,.ota-drop.drag{border-color:var(--acc);background:var(--accBg)}
.ota-drop input{display:none}
.ota-icon{font-size:24px;margin-bottom:8px}
.ota-text{font-size:13px;font-weight:500;color:var(--tx2);margin-bottom:3px}
.ota-sub{font-size:11px;color:var(--tx3)}
.ota-progress{margin-top:12px;display:none}
.ota-bar{height:4px;background:var(--bd);border-radius:2px;overflow:hidden;margin-bottom:6px}
.ota-fill{height:100%;background:var(--acc);border-radius:2px;transition:width .3s;width:0%}
.ota-status{font-size:11px;color:var(--acc);text-align:center}
.ota-btn{width:100%;margin-top:10px;padding:10px;border:1px solid var(--accBd);border-radius:9px;
  background:var(--accBg);color:var(--acc);font-family:inherit;font-size:13px;font-weight:600;
  cursor:pointer;transition:all .2s;display:none}
.ota-btn:hover{background:var(--acc);color:#fff}

/* 日志 */
.log-box{background:var(--bg);border:1px solid var(--bd);border-radius:9px;padding:10px 12px;
  font-family:'SF Mono','Courier New',monospace;font-size:11px;color:var(--tx2);
  max-height:180px;overflow-y:auto;line-height:1.9;white-space:pre-wrap;word-break:break-all}
.log-box::-webkit-scrollbar{width:4px}
.log-box::-webkit-scrollbar-thumb{background:var(--bd2);border-radius:4px}
.lf{color:var(--ok)}.lh{color:var(--acc)}.le{color:var(--err)}.lc{color:var(--warn)}.lo{color:var(--tx2)}

/* 记录器 */
.rec-bar{height:4px;background:var(--bd);border-radius:2px;overflow:hidden;margin-bottom:6px}
.rec-fill{height:100%;background:var(--err);border-radius:2px;transition:width .3s;width:0%}
.rec-info{display:flex;justify-content:space-between;font-size:11px;color:var(--tx3);margin-bottom:10px}

/* 警告 */
.warn-bar{margin:0 16px 14px;padding:10px 14px;border-radius:9px;
  background:var(--errBg);border:1px solid var(--errBd);font-size:11px;color:var(--err);line-height:1.7}
.foot{text-align:center;padding:8px 16px 20px;font-size:11px;color:var(--tx3)}
.footer-actions{display:flex;justify-content:center;padding:4px 16px 24px}
.footer-support-btn{flex:0 1 280px}
</style>
</head>
<body>

<div class="hdr">
  <div class="hdr-top">
    <div class="hdr-left">
      <div class="hdr-title">EV开源CAN工具</div>
      <span class="hw-badge" id="hw-badge">HW3</span>
      <span class="gtw-badge" id="gtw-badge" title="GTW_autopilot">GTW &mdash;</span>
    </div>
    <button class="theme-btn" onclick="toggleTheme()" id="theme-btn">&#9788; 浅色</button>
  </div>
  <div class="hdr-status">
    <span class="sdot dot-off" id="dot"></span>
    <span id="hdr-desc">等待CAN帧</span>
  </div>
</div>

<div class="fps-bar"><div class="fps-fill" id="fps-fill"></div></div>

<div class="stat-grid">
  <div class="stat"><div class="stat-lbl">CAN总线</div><div class="stat-val" id="s-can">离线</div></div>
    <div class="stat"><div class="stat-lbl">帧率</div><div class="stat-val v-dim" id="s-fps">0.0赫兹</div></div>
  <div class="stat"><div class="stat-lbl">接收</div><div class="stat-val v-acc" id="s-rx">0</div></div>
      <div class="stat"><div class="stat-lbl">跟车距离</div><div class="stat-val v-dim" id="s-fd">—</div></div>
  <div class="stat"><div class="stat-lbl">速度配置</div><div class="stat-val v-dim" id="s-prof">—</div></div>
  <div class="stat"><div class="stat-lbl">运行时间</div><div class="stat-val v-dim" id="s-up">0秒</div></div>
      <button class="btn btn-reboot" onclick="reboot()">重启</button>
</div>

<div style="height:12px" ></div>



  <div class="subsec" data-subkey="config-wifi-hotspot">
    <div class="subsec-head">
      <div class="subsec-title">WiFi热点 <span class="title-help" onclick="return toggleHelp(this,event)" data-help-target="ap-info" title="配置设备热点名称、密码和可见性。存储在NVS中。">?</span></div>
      <div class="subsec-meta"><span id="ap-stored" style="margin-right:8px"></span><span id="ap-clients">0个客户端</span></div>
    </div>
    <div class="subsec-body">
      <div id="ap-info" class="info-box" style="display:none">
        存储在NVS（非易失性存储）中。SSID和密码在固件更新和重启后仍然保留。只有通过USB进行完全出厂擦除才能清除。
      </div>
      <div class="setting-desc" style="margin-bottom:8px">更改WiFi热点名称和密码</div>
      <div style="display:flex;gap:6px;margin-bottom:6px">
        <input class="sniff-input" id="ap-ssid" placeholder="热点名称" style="flex:1">
        <input class="sniff-input" id="ap-pass" placeholder="新密码（最少8位）" type="password" style="flex:1">
      </div>
      <div class="setting-row" style="padding:8px 0">
        <div class="setting-info">
          <div class="setting-name">隐藏SSID</div>
          <div class="setting-desc">不广播热点名称 &mdash; 客户端必须手动输入</div>
        </div>
        <label class="tgl"><input type="checkbox" id="ap-hidden"><div class="tgl-track"><div class="tgl-thumb"></div></div></label>
      </div>
      <div style="display:flex;gap:6px;align-items:center">
        <button class="sniff-btn" onclick="saveAP()">保存</button>
        <span style="font-size:11px;color:var(--tx3)" id="ap-status"></span>
      </div>
      <div style="font-size:10px;color:var(--tx3);margin-top:6px">重启后生效。密码留空则保持当前密码。</div>
    </div>
  </div>

  <div class="subsec" data-subkey="config-wifi-internet">
    <div class="subsec-head">
      <div class="subsec-title">WiFi网络 <span class="title-help" onclick="return toggleHelp(this,event)" title="最多保存4个网络。设备会依次尝试连接。">?</span></div>
      <div class="subsec-meta"><span id="wifi-status">未配置</span></div>
    </div>
    <div class="subsec-body">
      <div class="setting-desc" style="margin-bottom:8px">保存最多4个网络（例如家里+手机热点）。设备会依次尝试连接。存储在NVS中 &mdash; 固件更新后仍然保留。</div>
      <div id="wifi-saved-list" style="margin-bottom:8px"></div>
      <div id="wifi-add-wrap">
        <div class="setting-desc" style="margin-bottom:6px"><b>添加网络</b> <span id="wifi-slot-count" style="color:var(--tx3)">(0/4)</span></div>
        <div style="display:flex;gap:6px;margin-bottom:6px">
          <input class="sniff-input" id="wifi-ssid" placeholder="WiFi 名称" style="flex:1">
          <button class="sniff-btn" onclick="scanWifi()" id="scan-btn">扫描</button>
        </div>
        <div id="wifi-nets" style="display:none;margin-bottom:6px;max-height:140px;overflow-y:auto;border:1px solid var(--bd);border-radius:6px;background:var(--bg2)"></div>
        <div style="display:flex;gap:6px;margin-bottom:6px">
          <input class="sniff-input" id="wifi-pass" placeholder="密码" type="password" style="flex:1">
          <button class="sniff-btn" onclick="saveWifi()" id="wifi-save-btn">保存并连接</button>
        </div>
        <details style="margin-top:4px">
          <summary style="font-size:11px;color:var(--acc);cursor:pointer;user-select:none">静态IP（可选） <span class="title-help" onclick="return toggleHelp(this,event)" title="设置固定IP地址而不是使用DHCP。">?</span></summary>
          <div style="margin-top:6px">
            <label style="font-size:11px;color:var(--tx3);display:flex;align-items:center;gap:6px;margin-bottom:6px">
              <input type="checkbox" id="wifi-static" onchange="toggleStaticIP()"> 使用静态IP
            </label>
            <div id="static-fields" style="display:none">
              <div style="display:grid;grid-template-columns:1fr 1fr;gap:4px">
                <input class="sniff-input" id="wifi-ip" placeholder="IP（例如192.168.1.100）">
                <input class="sniff-input" id="wifi-gw" placeholder="网关（例如192.168.1.1）">
                <input class="sniff-input" id="wifi-mask" placeholder="掩码（255.255.255.0）" value="255.255.255.0">
                <input class="sniff-input" id="wifi-dns" placeholder="DNS（例如8.8.8.8）">
              </div>
            </div>
          </div>
        </details>
        <input type="hidden" id="wifi-edit-idx" value="-1">
      </div>
    </div>
  </div>

  <div class="subsec" data-subkey="config-can-pins">
    <div class="subsec-head">
      <div class="subsec-title">CAN引脚 <span class="title-help" onclick="return toggleHelp(this,event)" title="设置用于CAN收发器的ESP32 GPIO引脚。错误的引脚值可能导致CAN无法工作。">?</span></div>
      <div class="subsec-meta" id="can-pins-status">默认</div>
    </div>
    <div class="subsec-body">
      <div style="display:flex;gap:6px;align-items:center">
        <input class="sniff-input" id="can-tx" type="number" min="0" max="39" placeholder="TX 引脚" style="flex:1">
        <input class="sniff-input" id="can-rx" type="number" min="0" max="39" placeholder="RX 引脚" style="flex:1">
        <button class="sniff-btn" onclick="saveCanPins()">保存</button>
      </div>
      <div style="font-size:11px;color:var(--tx3);margin-top:6px" id="can-pins-hint">更改后需重启</div>
    </div>
  </div>

  <div class="subsec" data-subkey="config-dashboard-log" style="margin-top:14px">
    <div class="subsec-head">
      <div class="subsec-title">仪表盘日志 <span class="title-help" onclick="return toggleHelp(this,event)" title="显示最近的仪表盘和固件日志。这是仪表盘日志输出，不是CAN嗅探器。">?</span></div>
      <div class="subsec-meta">最近的仪表盘输出</div>
    </div>
    <div class="subsec-body">
      <div class="setting-row" style="padding-top:0">
        <div class="setting-info">
          <div class="setting-name">仪表盘日志记录 <span class="title-help" onclick="return toggleHelp(this,event)" title="打开或关闭仪表盘日志输出。">?</span></div>
          <div class="setting-desc">切换仪表盘日志输出</div>
        </div>
        <label class="tgl"><input type="checkbox" id="tgl-eprn" checked onchange="pushLogging()">
          <div class="tgl-track"><div class="tgl-thumb"></div></div></label>
      </div>
      <div class="log-box" id="log">等待中...</div>
    </div>
  </div>
</div>

<div class="card">
  <div class="card-hdr">
    <div class="card-title">固件更新 <span class="title-help" onclick="return toggleHelp(this,event)" title="检查更新、启用测试版并手动上传固件。">?</span></div>
    <div class="card-meta" id="fw-ver">版本信息</div>
  </div>
  <div style="margin-bottom:10px">
    <div class="setting-row">
      <div class="setting-info">
        <div class="setting-name">测试版通道 <span class="title-help" onclick="return toggleHelp(this,event)" title="有可用时显示预发布固件版本。">?</span></div>
        <div class="setting-desc">包含预发布/测试版固件版本</div>
      </div>
      <label class="tgl"><input type="checkbox" id="beta-tgl" onchange="toggleBeta()"><div class="tgl-track"><div class="tgl-thumb"></div></div></label>
    </div>
    <div class="setting-row">
      <div class="setting-info">
        <div class="setting-name">开机自动更新 <span class="title-help" onclick="return toggleHelp(this,event)" title="WiFi连接后自动检查固件更新。">?</span></div>
        <div class="setting-desc">WiFi连接后约15秒自动检查并安装更新</div>
      </div>
      <label class="tgl"><input type="checkbox" id="auto-upd-tgl" onchange="toggleAutoUpdate()"><div class="tgl-track"><div class="tgl-thumb"></div></div></label>
    </div>
  </div>
  <div style="display:flex;gap:6px;align-items:center">
    <button class="sniff-btn" onclick="checkUpdate()" id="upd-check-btn">检查更新</button>
    <span style="font-size:11px;color:var(--tx3)" id="upd-status"></span>
  </div>
  <div id="upd-info" class="info-box" style="display:none;margin-top:10px">
    <div style="display:flex;justify-content:space-between;align-items:center">
      <div>
        <div style="font-size:13px;font-weight:600" id="upd-ver"></div>
        <div style="font-size:11px;color:var(--tx3)" id="upd-detail"></div>
      </div>
      <button class="sniff-btn" onclick="installUpdate()" id="upd-install-btn" style="background:var(--ok);color:#fff;border-color:var(--ok)">安装</button>
    </div>
  </div>

  <details style="margin-top:14px;padding-top:12px;border-top:1px solid var(--bd)">
    <summary style="font-size:12px;color:var(--acc);cursor:pointer;user-select:none">手动固件上传(.bin) <span class="title-help" onclick="return toggleHelp(this,event)" title="直接将本地固件.bin文件上传到设备。">?</span></summary>
    <div style="margin-top:10px">
      <div class="ota-drop" id="ota-drop" onclick="$('ota-file').click()" ondragover="event.preventDefault();this.classList.add('drag')" ondragleave="this.classList.remove('drag')" ondrop="handleDrop(event)">
        <input type="file" id="ota-file" accept=".bin" onchange="fileSelected(this.files[0])">
        <div class="ota-icon">&#8679;</div>
        <div class="ota-text">点击选择固件.bin文件</div>
        <div class="ota-sub">或拖放文件到此</div>
      </div>
      <div class="ota-progress" id="ota-progress">
        <div class="ota-bar"><div class="ota-fill" id="ota-fill"></div></div>
        <div class="ota-status" id="ota-status">上传中...</div>
      </div>
      <button class="ota-btn" id="ota-upload-btn" onclick="uploadFirmware()">刷写固件</button>
      <button class="sniff-btn" id="ota-reset-btn" onclick="resetOtaCredentials()" style="width:100%;margin-top:6px">重置OTA凭据</button>
      <div style="margin-top:10px;font-size:11px;color:var(--tx3);line-height:1.7">
        在PlatformIO中编译.bin文件： <span style="color:var(--acc);font-family:monospace">Ctrl+Alt+B</span><br>
        文件位置： <span style="color:var(--acc);font-family:monospace">.pio/build/esp32_ext_mcp2515/firmware.bin</span>
      </div>
    </div>
  </details>
</div>
<div class="card">
  <div class="card-hdr"><div class="card-title">CAN <span class="title-help" onclick="return toggleHelp(this,event)" title="实时CAN工具：嗅探、记录、控制器状态和上次写入检查。">?</span></div><div class="card-meta">嗅探器、记录器和总线状态</div></div>

  <div class="subsec" data-subkey="can-sniffer">
    <div class="subsec-head">
      <div class="subsec-title">CAN嗅探器 <span class="title-help" onclick="return toggleHelp(this,event)" title="实时显示最新的30个CAN帧。可以按ID或名称过滤，在线路ID和DBC ID之间切换，以及暂停视图。">?</span></div>
      <div class="subsec-meta" id="sniff-count">0帧</div>
    </div>
    <div class="subsec-body">
      <div class="sniff-ctrl">
        <input class="sniff-input" id="sniff-filter" placeholder="按ID或名称过滤" oninput="renderSnifferAndTrans()">
        <button class="sniff-btn" id="sniff-id-btn" onclick="toggleSniffIdMode()">线路ID</button>
        <button class="sniff-btn" id="sniff-pause-btn" onclick="togglePause()">暂停</button>
      </div>
      <div class="sniff-box" id="sniffer">
        <div style="padding:20px;color:var(--tx3);text-align:center;font-size:12px">等待CAN帧</div>
      </div>
    </div>
  </div>

  <div class="subsec" data-subkey="can-translated">
    <div class="subsec-head">
      <div class="subsec-title">实时信号翻译 <span class="title-help" onclick="return toggleHelp(this,event)" title="根据固件已知bit布局，对嗅探到的帧进行简洁中文翻译（车速、档位、油门、AP状态、召唤等）。与上方嗅探器一一对应，同一顺序。">?</span></div>
      <div class="subsec-meta" id="trans-count">0帧</div>
    </div>
    <div class="subsec-body">
      <div class="sniff-box" id="translated" style="max-height:260px"></div>
    </div>
  </div>

  <div class="subsec" data-subkey="can-recorder">
    <div class="subsec-head">
      <div class="subsec-title">CAN记录器 <span class="title-help" onclick="return toggleHelp(this,event)" title="实时记录CAN流量，达到帧数限制后可以下载为CSV文件。">?</span></div>
      <div class="subsec-meta" id="rec-meta">空闲</div>
    </div>
    <div class="subsec-body">
      <div class="rec-bar"><div class="rec-fill" id="rec-fill"></div></div>
      <div class="rec-info">
        <span id="rec-count">0/2000帧</span>
        <span id="rec-status">就绪</span>
      </div>
      <div class="btn-row">
        <button class="btn" id="rec-btn" onclick="toggleRec()">开始录制</button>
        <a class="btn" id="rec-dl" href="/rec_download" download="can_recording.csv" style="display:none;text-align:center;text-decoration:none;padding:10px;border:1px solid var(--bd2);color:var(--tx2)">下载CSV</a>
      </div>
    </div>
  </div>

  <div class="subsec" data-subkey="can-controller">
    <div class="subsec-head">
      <div class="subsec-title">CAN控制器 <span class="title-help" onclick="return toggleHelp(this,event)" title="显示CAN控制器健康状态、错误标志以及每个mux的接收、发送和错误计数器。">?</span></div>
      <div class="subsec-meta" style="display:flex;align-items:center;gap:8px">
        <button onclick="resetStats()" style="font-size:10px;padding:2px 8px;border:1px solid var(--bd2);border-radius:5px;background:transparent;color:var(--tx3);cursor:pointer;font-family:inherit">重置</button>
      </div>
    </div>
    <div class="subsec-body">
      <div class="eflg-row" id="eflg-row"><span class="eflg-pill eflg-ok">OK</span></div>
      <table class="mux-tbl">
        <tr><th>Mux</th><th>接收</th><th>发送</th><th>错误</th></tr>
        <tr><td>0</td><td id="m0rx">0</td><td id="m0tx">0</td><td id="m0err">0</td></tr>
        <tr><td>1</td><td id="m1rx">0</td><td id="m1tx">0</td><td id="m1err">0</td></tr>
        <tr><td>2</td><td id="m2rx">0</td><td id="m2tx">0</td><td id="m2err">0</td></tr>
      </table>
    </div>
  </div>

  <div class="subsec" data-subkey="can-last-write-check">
    <div class="subsec-head">
      <div class="subsec-title">上次写入检查 <span class="title-help" onclick="return toggleHelp(this,event)" title="比较最后注入的帧与总线上具有相同CAN ID和mux的最新帧。有助于发现覆盖问题，但不能证明模块接受了更改。">?</span></div>
    </div>
    <div class="subsec-body">
      <div class="probe-status v-dim" id="probe-status">暂无注入帧</div>
      <div class="probe-block">
        <div class="probe-label">已发送</div>
        <div class="probe-meta" id="probe-tx-meta">—</div>
        <div class="probe-hex" id="probe-tx">—</div>
      </div>
      <div class="probe-block">
        <div class="probe-label">总线</div>
        <div class="probe-meta" id="probe-rx-meta">—</div>
        <div class="probe-hex" id="probe-rx">—</div>
      </div>
    </div>
  </div>
</div>


<div class="card">
  <div class="card-hdr"><div class="card-title">ESP-NOW <span class="title-help" onclick="return toggleHelp(this,event)" title="通过ESP-NOW广播CAN信号（油门、刹车、转向灯、车速）到附近的ESP32设备。扫描并连接接收端设备后自动广播。">?</span></div><div class="card-meta" id="espnow-status">未初始化</div></div>

  <div class="subsec" data-subkey="espnow-scan">
    <div class="subsec-head">
      <div class="subsec-title">设备扫描 <span class="title-help" onclick="return toggleHelp(this,event)" title="开始扫描后，本设备将监听包含暗号"chamsonxie"的广播包。接收端设备需发送此暗号进行配对。">?</span></div>
      <div class="subsec-meta" id="espnow-scan-meta">未扫描</div>
    </div>
    <div class="subsec-body">
      <div style="display:flex;gap:6px;margin-bottom:8px;align-items:center">
        <button class="sniff-btn" id="espnow-scan-btn" onclick="toggleEspNowScan()">开始扫描</button>
        <span style="font-size:11px;color:var(--tx3)" id="espnow-scan-status"></span>
      </div>
      <div id="espnow-devices" style="margin-top:4px">
        <div style="font-size:12px;color:var(--tx3);text-align:center;padding:12px">点击"开始扫描"查找附近的接收端设备</div>
      </div>
      <div id="espnow-paired" style="margin-top:8px;display:none">
        <div class="setting-row">
          <div class="setting-info">
            <div class="setting-name" id="espnow-paired-name">已配对设备</div>
            <div class="setting-desc" id="espnow-paired-mac"></div>
          </div>
          <button class="sniff-btn" onclick="unpairEspNow()" style="background:var(--errBg);border-color:var(--errBd);color:var(--err)">断开</button>
        </div>
      </div>
      <div style="font-size:11px;color:var(--tx3);margin-top:8px;line-height:1.6">
        暗号: <code style="background:var(--bg);padding:2px 6px;border-radius:4px">chamsonxie</code><br>
        接收端设备需广播此暗号才能被本设备发现。
      </div>
    </div>
  </div>
</div>

<div class="warn-bar">CAN总线写入会影响车辆行为。如果出现意外行为，请立即移除设备。与任何车辆制造商无关。</div>

<div class="modal-backdrop" id="confirm-modal" onclick="dashConfirmBackdrop(event)">
  <div class="modal-card" role="dialog" aria-modal="true" aria-labelledby="confirm-title">
    <div class="modal-title" id="confirm-title">确认</div>
    <div class="modal-msg" id="confirm-msg"></div>
    <div class="modal-actions">
      <button class="sniff-btn" id="confirm-cancel" onclick="dashConfirmResolve(false)">取消</button>
      <button class="sniff-btn modal-btn-primary" id="confirm-ok" onclick="dashConfirmResolve(true)">继续</button>
    </div>
  </div>
</div>

<div class="modal-backdrop" id="support-modal" onclick="supportBackdrop(event)">
  <div class="modal-card" role="dialog" aria-modal="true" aria-labelledby="support-title" style="width:min(100%,560px)">
    <div class="modal-title" id="support-title">支持</div>
    <div class="modal-msg" style="margin-top:10px">
      <textarea id="support-body" readonly style="width:100%;min-height:260px;resize:vertical;border:1px solid var(--bd2);border-radius:8px;background:var(--bg);color:var(--tx);padding:10px;font:inherit;line-height:1.5"></textarea>
    </div>
    <div class="modal-actions" style="justify-content:space-between;align-items:center">
      <span id="support-status" style="font-size:11px;color:var(--tx3)"></span>
      <div style="display:flex;gap:8px;flex-wrap:wrap;justify-content:flex-end">
        <button class="sniff-btn" onclick="copySupport()">复制</button>
        <button class="sniff-btn modal-btn-primary" onclick="openSupportIssue()">打开GitHub Issue</button>
        <button class="sniff-btn" onclick="closeSupport()">关闭</button>
      </div>
    </div>
  </div>
</div>

<div class="foot" id="dash-foot">EV开源CAN工具 &bull; 加载中...</div>
<div class="foot" style="margin-top:4px">
  <a href="https://github.com/ev-open-can-tools/ev-open-can-tools" target="_blank" rel="noopener" style="color:var(--acc);text-decoration:none">GitHub</a>
  &bull;
  <a href="https://discord.gg/ZTQKAUTd2F" target="_blank" rel="noopener" style="color:var(--acc);text-decoration:none">Discord</a>
</div>
<div class="foot" style="margin-top:8px;font-size:10px">
  <div style="margin-bottom:4px">用Monero打赏</div>
  <div style="word-break:break-all;color:var(--tx2)">46CJEjnN74N83AZHHYKX3mD9kkV6UJYVjN58PTWvQ6VU8Vvn3tmyExkaC2kq9asD6SZY9weaZqx5o9nf1MxkKbmTKWLUeRD</div>
</div>
<div class="foot footer-actions">
  <button type="button" class="btn btn-reboot footer-support-btn" onclick="openSupport()">支持</button>
</div>

<script>
const HW=['Legacy','HW3','HW4'];
const SP3=['舒适','标准','快速'];
const SP4=['舒适','标准','快速','极速','慢速'];
const $=id=>document.getElementById(id);
const setText=(id,value)=>{const el=$(id);if(el)el.textContent=value;};
const setClass=(id,value)=>{const el=$(id);if(el)el.className=value;};
function profileNamesForHw(hw){return hw===2?SP4:SP3;}
function profileDisplayName(hw,sp,auto){
  const name=(profileNamesForHw(hw)||[])[clampProfileForHw(hw,sp)]||'—';
  return auto?'Auto ('+name+')':name;
}
function gtwAutopilotName(v){
  return ['无','高速','增强','完全自动驾驶','基础'][v]||'未知';
}
function gtwAutopilotBadge(v){
  if(v<0)return 'GTW —';
  if(v===3)return 'GTW 自动驾驶';
  return 'GTW '+gtwAutopilotName(v);
}
function updateGtwBadge(v){
  const el=$('gtw-badge');if(!el)return;
  v=Number(v);
  const known=!isNaN(v)&&v>=0;
  el.textContent=gtwAutopilotBadge(known?v:-1);
  el.className='gtw-badge '+(known?'known':'');
  el.title=known?('GTW_autopilot: '+gtwAutopilotName(v)+' ('+v+')'):'GTW_autopilot: 尚未收到';
}
let state={hw:1,can:true,apGate:false,sp:0,spAuto:true,plgr:1,plgrmax:20,gate:null};
let sniffPaused=false,sniffFrames=[];
let sniffShowDbcIds=localStorage.getItem('sniffIdMode')==='dbc';
let otaFile=null;
let otaUser=localStorage.getItem('otaU')||'',otaPass=localStorage.getItem('otaP')||'';
let logSince=0;
let installedPlugins=[];
let pluginMax=0;
const peMaxOps=16;
let peLoadedPluginName='';
let peTestPollTimer=null;
let pluginDetailOpen={};
let dashConfirmState=null;
let supportIssueUrl='https://github.com/ev-open-can-tools/ev-open-can-tools/issues/new?template=issue.yml';
let supportBodyText='';
let dashboardPollTimers=[];
let dashboardPollFailures=0;
let dashboardStatusOk=false;
let dashboardInitialLoaded=false;
let dashboardPollStopped=false;
let dashboardStaIp='';
const pollLocks={};

function stopDashboardPolling(){
  if(dashboardPollStopped)return;
  dashboardPollStopped=true;
  dashboardPollTimers.forEach(clearInterval);
  dashboardPollTimers=[];
  $('dot').className='sdot dot-off';
  $('hdr-desc').textContent='仪表盘已断开';
  let msg='与'+location.hostname+'的连接已断开。重新连接后请刷新页面。';
  if(dashboardStaIp&&dashboardStaIp!==location.hostname)msg='与'+location.hostname+'的连接已断开。切换到您的普通WiFi并打开http://'+dashboardStaIp;
  $('wifi-status').textContent=msg;
  $('wifi-status').style.color='var(--err)';
}

function noteDashboardPoll(ok){
  if(ok){dashboardPollFailures=0;dashboardStatusOk=true;return;}
  if(dashboardPollStopped)return;
  dashboardStatusOk=false;
  dashboardPollFailures++;
  $('dot').className='sdot dot-off';
  $('hdr-desc').textContent='仪表盘重连中';
}

async function fetchPollJson(url,timeoutMs,trackConnection){
  const ctrl=new AbortController();
  const timer=setTimeout(()=>ctrl.abort(),timeoutMs||2500);
  try{
    const r=await fetch(url,{signal:ctrl.signal});
    if(!r.ok)throw new Error('HTTP '+r.status);
    const d=await r.json();
    if(trackConnection)noteDashboardPoll(true);
    return d;
  }catch(e){
    if(trackConnection)noteDashboardPoll(false);
    throw e;
  }finally{
    clearTimeout(timer);
  }
}

async function runPoll(name,fn){
  if(dashboardPollStopped||pollLocks[name])return;
  pollLocks[name]=true;
  try{return await fn();}finally{pollLocks[name]=false;}
}

function waitMs(ms){return new Promise(resolve=>setTimeout(resolve,ms));}

function initCardMinimizers(){
  document.querySelectorAll('.card').forEach((card,i)=>{
    const hdr=card.querySelector('.card-hdr');if(!hdr||hdr.querySelector('.card-min-btn'))return;
    const title=card.querySelector('.card-title');
    const key='cardCollapse:'+i+':'+((title?title.textContent:'card').trim().toLowerCase().replace(/[^a-z0-9]+/g,'-'));
    card.dataset.collapseKey=key;
    const btn=document.createElement('button');
    btn.type='button';
    btn.className='sniff-btn card-min-btn';
    btn.onclick=()=>{
      const collapsed=!card.classList.contains('collapsed');
      card.classList.toggle('collapsed',collapsed);
      localStorage.setItem(key,collapsed?'1':'0');
      btn.textContent=collapsed?'展开':'收起';
    };
    hdr.appendChild(btn);
    const collapsed=localStorage.getItem(key)==='1';
    card.classList.toggle('collapsed',collapsed);
    btn.textContent=collapsed?'展开':'收起';
  });
}
function initSubsectionMinimizers(){
  document.querySelectorAll('.subsec').forEach((sec,i)=>{
    const hdr=sec.querySelector('.subsec-head');if(!hdr||hdr.querySelector('.subsec-btn'))return;
    const explicitKey=sec.dataset.subkey||'';
    const title=sec.querySelector('.subsec-title');
    const safe=((title?title.textContent:'section').trim().toLowerCase().replace(/[^a-z0-9]+/g,'-'));
    const key='subCollapse:'+(explicitKey||i+':'+safe);
    sec.dataset.collapseKey=key;
    const btn=document.createElement('button');
    btn.type='button';
    btn.className='sniff-btn subsec-btn';
    btn.onclick=()=>{
      const collapsed=!sec.classList.contains('collapsed');
      sec.classList.toggle('collapsed',collapsed);
      localStorage.setItem(key,collapsed?'1':'0');
      btn.textContent=collapsed?'展开':'收起';
    };
    hdr.appendChild(btn);
    const collapsed=localStorage.getItem(key)==='1';
    sec.classList.toggle('collapsed',collapsed);
    btn.textContent=collapsed?'展开':'收起';
  });
}

function syncSniffPauseButton(){
  const b=$('sniff-pause-btn');if(!b)return;
  b.textContent=sniffPaused?'继续':'暂停';
  b.classList.toggle('paused',sniffPaused);
}

function actionErrorMessage(e,fallback){
  if(!e)return fallback;
  if(e.name==='AbortError'||e.name==='SyntaxError'||e.message==='Failed to fetch'||e.message==='Empty response')return fallback;
  return e.message||fallback;
}

async function fetchJsonWithTimeout(url,options,timeoutMs){
  const ctrl=new AbortController();
  const timer=setTimeout(()=>ctrl.abort(),timeoutMs||2500);
  try{
    const opts=Object.assign({},options||{});
    opts.signal=ctrl.signal;
    const r=await fetch(url,opts);
    const text=await r.text();
    if(!text||!text.trim())throw new Error(r.ok?'Empty response':('HTTP '+r.status));
    const d=JSON.parse(text);
    if(!r.ok)throw new Error(d.error||('HTTP '+r.status));
    return d;
  }finally{
    clearTimeout(timer);
  }
}

function dashConfirmResolve(ok){
  if(!dashConfirmState)return;
  const resolve=dashConfirmState.resolve;
  dashConfirmState=null;
  $('confirm-modal').style.display='none';
  document.body.style.overflow='';
  resolve(!!ok);
}

function dashConfirmBackdrop(ev){
  if(ev.target===$('confirm-modal'))dashConfirmResolve(false);
}

function supportBackdrop(ev){
  if(ev.target===$('support-modal'))closeSupport();
}

function dashConfirm(message,title,okText,cancelText){
  if(dashConfirmState)dashConfirmResolve(false);
  return new Promise(resolve=>{
    dashConfirmState={resolve};
    $('confirm-title').textContent=title||'确认';
    $('confirm-msg').textContent=message||'';
    $('confirm-ok').textContent=okText||'继续';
    $('confirm-cancel').textContent=cancelText||'取消';
    $('confirm-modal').style.display='flex';
    document.body.style.overflow='hidden';
    setTimeout(()=>{$('confirm-ok').focus();},0);
  });
}

function supportPluginSummary(){
  return (installedPlugins||[]).filter(p=>p&&p.enabled).map(function(p){
    const rules=(p.details||[]).map(function(r,idx){
      const d=r.diag||{};
      return '  rule '+(idx+1)+' CAN '+(r.hex||r.id)+(r.mux>=0?' mux '+r.mux:'')+
        ': match='+((d.match_count||0))+
        ' changed='+((d.changed_count||0))+
        ' tx_ok='+((d.send_ok_count||0))+
        ' tx_fail='+((d.send_fail_count||0))+
        ' last_original='+(d.last_original||'')+
        ' last_modified='+(d.last_modified||'');
    }).join('\n');
    return '#'+p.priority+' '+p.name+(p.rules?' ('+p.rules+' rules)':'')+(rules?'\n'+rules:'');
  }).join('\n')||'none';
}

function supportSettingsSummary(){
  return [
    'Hardware: '+(HW[state.hw]||'?'),
    'Speed profile: '+profileDisplayName(state.hw,state.sp,state.spAuto),
    'CAN status: '+($('s-can')?$('s-can').textContent:'—'),
    'Injection: '+($('s-inj')?$('s-inj').textContent:'—'),
    'AD: '+($('s-AD')?$('s-AD').textContent:'—'),
    'CAN pins: '+($('can-pins-status')?$('can-pins-status').textContent:'—'),
    'Firmware: '+($('fw-ver')?$('fw-ver').textContent:'—'),
    'Beta channel: '+($('beta-tgl')&&$('beta-tgl').checked?'enabled':'disabled'),
    'Auto-update: '+($('auto-upd-tgl')&&$('auto-upd-tgl').checked?'enabled':'disabled'),
    'Plugin replay: '+(state.plgr||1)+'x',
    'Dashboard logging: '+($('tgl-eprn')&&$('tgl-eprn').checked?'enabled':'disabled')
  ].join('\n');
}

function supportBool(v){return v?'yes':'no';}
function supportAge(v){return typeof v==='number'&&v>=0?(v+' ms ago'):'never';}
function supportHexByte(v){return typeof v==='number'&&v>=0?'0x'+toHex(v&255,2):'unavailable';}
function supportGateDiagnostics(){
  const g=state.gate;
  if(!g)return 'AP Gate Diagnostics: unavailable';
  const plugins=Array.isArray(g.plugins)&&g.plugins.length
    ?g.plugins.map(p=>'#'+p.priority+' '+p.name).join(', ')
    :'none';
  return [
    'AP Gate Diagnostics',
    'Enabled: '+supportBool(!!g.enabled),
    'CAN active: '+supportBool(!!g.canActive),
    'Injection allowed: '+supportBool(!!g.allowed),
    'Reason: '+(g.reason||'unknown'),
    'AP active: '+supportBool(!!g.ap),
    'Parked: '+supportBool(!!g.parked),
    'Summoning: '+supportBool(!!g.summoning),
    'AP stable: '+(typeof g.apStableMs==='number'?g.apStableMs:0)+' ms',
    'Last 921: '+supportAge(g.last921AgeMs),
    'Last 280: '+supportAge(g.last280AgeMs),
    'Last 390: '+supportAge(g.last390AgeMs),
    'Last 1016: '+supportAge(g.last1016AgeMs),
    'Last 1021: '+supportAge(g.last1021AgeMs),
    'DAS_autopilotStatus: '+supportHexByte(g.dasAutopilotStatus),
    'Hardware: '+(HW[typeof g.hardware==='number'?g.hardware:state.hw]||'?'),
    'Enabled plugins: '+plugins
  ].join('\n');
}

function buildSupportBody(){
  const enabled=supportPluginSummary();
  const gate=supportGateDiagnostics();
  const body=[
    'ev-open-can-tools support report',
    '',
    'Device',
    'Hardware: '+(HW[state.hw]||'?'),
    'Speed profile: '+profileDisplayName(state.hw,state.sp,state.spAuto),
    'CAN status: '+($('s-can')?$('s-can').textContent:'—'),
    'Injection: '+($('s-inj')?$('s-inj').textContent:'—'),
    'AD: '+($('s-AD')?$('s-AD').textContent:'—'),
    'CAN pins: '+($('can-pins-status')?$('can-pins-status').textContent:'—'),
    'Firmware: '+($('fw-ver')?$('fw-ver').textContent:'—'),
    '',
    'Settings',
    'Beta channel: '+($('beta-tgl')&&$('beta-tgl').checked?'enabled':'disabled'),
    'Auto-update: '+($('auto-upd-tgl')&&$('auto-upd-tgl').checked?'enabled':'disabled'),
    'Plugin replay: '+(state.plgr||1)+'x',
    'Dashboard logging: '+($('tgl-eprn')&&$('tgl-eprn').checked?'enabled':'disabled'),
    '',
    gate,
    '',
    'Enabled plugins',
    enabled,
    '',
    'Notes',
    ''
  ].join('\n');
  supportBodyText=body;
  return body;
}

function openSupport(){
  const el=$('support-body');
  if(el)el.value=buildSupportBody();
  const st=$('support-status');
  if(st){st.textContent='复制此文本，然后打开GitHub Issue表单。';st.style.color='var(--tx3)';}
  $('support-modal').style.display='flex';
  document.body.style.overflow='hidden';
  setTimeout(()=>{if(el)el.focus();el&&el.setSelectionRange(0,0);},0);
}

function closeSupport(){
  $('support-modal').style.display='none';
  document.body.style.overflow='';
}

function copySupportText(text,el){
  if(el){
    el.focus();
    el.select();
    el.setSelectionRange(0,text.length);
    if(document.execCommand&&document.execCommand('copy'))return true;
  }
  if(navigator.clipboard&&navigator.clipboard.writeText){
    navigator.clipboard.writeText(text).catch(()=>{});
    return true;
  }
  return false;
}

function copySupport(){
  const el=$('support-body');
  const text=el?el.value:buildSupportBody();
  if(copySupportText(text,el)){
    const st=$('support-status');if(st){st.textContent='已复制到剪贴板';st.style.color='var(--ok)';}
    return true;
  }
  const st=$('support-status');if(st){st.textContent='复制失败';st.style.color='var(--err)';}
  return false;
}

function openSupportIssue(){
  const url='https://github.com/ev-open-can-tools/ev-open-can-tools/issues/new?template=issue.yml';
  const copied=copySupport();
  supportIssueUrl=url;
  window.open(url,'_blank','noopener');
  const st=$('support-status');if(st&&copied){st.textContent='已复制支持信息。请粘贴到支持问题中。';st.style.color='var(--ok)';}
  closeSupport();
}

document.addEventListener('keydown',e=>{
  if(e.key==='Escape'){
    if(dashConfirmState)dashConfirmResolve(false);
    closeHelpPanels(document);
  }
});
document.addEventListener('click',e=>{
  if(!e.target.closest('.title-help')&&!e.target.closest('.inline-help-panel')){
    closeHelpPanels(document);
  }
});

function toggleTheme(){
  const html=document.documentElement;
  const isDark=html.getAttribute('data-theme')==='dark';
  html.setAttribute('data-theme',isDark?'light':'dark');
  $('theme-btn').innerHTML=isDark?'&#9790; 深色':'&#9788; 浅色';
  localStorage.setItem('theme',isDark?'light':'dark');
}
(function(){
  const t=localStorage.getItem('theme')||'dark';
  document.documentElement.setAttribute('data-theme',t);
  // 将在DOM就绪后更新
  window.addEventListener('DOMContentLoaded',()=>{
    $('theme-btn').innerHTML=t==='dark'?'&#9788; 浅色':'&#9790; 深色';
  });
})();



function updateInjectButtons(active){
  const stop=$('btn-stop'),resume=$('btn-resume');
  if(stop)stop.style.display=active?'':'none';
  if(resume)resume.style.display=active?'none':'';
}

function sniffBusPrefix(){return state.hw===0?0x0800:0x1000;}
function sniffBusLabel(){return state.hw===0?'PARTY':'CH';}
function sniffWireId(id){return id&0x7FF;}
function sniffDbcId(id){return sniffWireId(id)|sniffBusPrefix();}
function sniffDisplayId(id){return sniffShowDbcIds?sniffDbcId(id):sniffWireId(id);}
function updateSniffIdToggle(){
  const b=$('sniff-id-btn'),bus=sniffBusLabel();
  b.textContent=sniffShowDbcIds?('DBC '+bus):'线路ID';
  b.title=sniffShowDbcIds?('显示带有'+bus+'前缀的DBC JSON ID'):('显示线路上的11位CAN ID');
  $('sniff-filter').placeholder='按线路/DBC ID或名称过滤';
}
function toggleSniffIdMode(){
  sniffShowDbcIds=!sniffShowDbcIds;
  localStorage.setItem('sniffIdMode',sniffShowDbcIds?'dbc':'wire');
  updateSniffIdToggle();
  renderSnifferAndTrans();
}



function updatePluginReplayControl(count,max){
  count=Math.max(1,parseInt(count,10)||1);
  max=Math.max(count,parseInt(max,10)||20);
  state.plgr=count;state.plgrmax=max;
  const input=$('plugin-replay');
  if(input){input.max=max;if(document.activeElement!==input)input.value=count;}
  const meta=$('plugin-replay-meta');if(meta)meta.textContent=count+'x';
}
async function savePluginReplay(){
  const input=$('plugin-replay'),st=$('plugin-replay-status'),max=state.plgrmax||20;
  let v=parseInt(input.value,10);
  if(isNaN(v)||v<1||v>max){st.textContent='请输入1-'+max;st.style.color='var(--err)';return;}
  st.textContent='保存中...';st.style.color='var(--tx3)';
  try{
    const r=await fetch('/config',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:'plgr='+v});
    const d=await r.json();
    if(!d.ok)throw new Error();
    updatePluginReplayControl(v,max);
    st.textContent='已保存';st.style.color='var(--ok)';
    poll();
  }catch(e){st.textContent='保存失败';st.style.color='var(--err)';}
}



function updateApGateControl(d){
  const enabled=!!d.apGate;
  state.apGate=enabled;
  const tgl=$('ap-gate-tgl');if(tgl)tgl.checked=enabled;
  setText('ap-gate-meta',enabled?'开启':'关闭');
}
async function saveApGate(){
  const tgl=$('ap-gate-tgl'),st=$('ap-gate-status');
  const enabled=tgl.checked?'1':'0';
  st.textContent='保存中...';st.style.color='var(--tx3)';
  try{
    const r=await fetch('/config',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:'apg='+enabled});
    const d=await r.json();
    if(!d.ok)throw new Error();
    state.apGate=enabled==='1';
    setText('ap-gate-meta',state.apGate?'开启':'关闭');
    st.textContent='已保存';st.style.color='var(--ok)';
    poll();
  }catch(e){st.textContent='保存失败';st.style.color='var(--err)';}
}

async function pushLogging(){
  const body='eprn='+($('tgl-eprn').checked?'1':'0');
  try{await fetch('/logging',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body});}catch(e){}
  if($('tgl-eprn').checked)pollLog();
  poll();
}

async function emergencyStop(){if(!await dashConfirm('停止注入？重启后仍保持禁用，直到您按下"恢复注入"。','停止注入','停止'))return;try{await fetch('/disable',{method:'POST'});}catch(e){}poll();}
async function resumeInj(){try{await fetch('/config',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:'hw='+state.hw+'&sp='+state.sp+'&spa='+(state.spAuto?'1':'0')+'&can=1'});}catch(e){}poll();}
async function reboot(){if(!await dashConfirm('重启设备？','重启','重启'))return;try{await fetch('/reboot',{method:'POST'});}catch(e){}}

function fmtUp(s){
  if(s<60)return s+'s';
  if(s<3600)return Math.floor(s/60)+'m '+String(s%60).padStart(2,'0')+'s';
  return Math.floor(s/3600)+'h '+Math.floor((s%3600)/60)+'m';
}
function fmtAgeMs(ms){
  if(ms<1000)return ms+' ms';
  if(ms<10000)return (ms/1000).toFixed(1)+' s';
  if(ms<60000)return Math.round(ms/1000)+' s';
  return fmtUp(Math.floor(ms/1000));
}
function toHex(n,p){return n.toString(16).toUpperCase().padStart(p,'0')}
function fmtProbeData(data,dlc){
  if(!Array.isArray(data)||!dlc)return '—';
  return data.slice(0,dlc).map(v=>toHex((v||0)&255,2)).join(' ');
}
function renderWriteProbe(p){
  const status=$('probe-status');
  if(!p||!p.active){
    status.textContent='暂无注入帧';
    status.className='probe-status v-dim';
    $('probe-tx-meta').textContent='—';
    $('probe-tx').textContent='—';
    $('probe-rx-meta').textContent='—';
    $('probe-rx').textContent='—';
    return;
  }
  const id='CAN 0x'+toHex((p.id||0)&0x7FF,3)+(p.mux>=0?' · mux '+p.mux:'');
  $('probe-tx-meta').textContent=id+' · '+fmtAgeMs(p.txa||0)+'前';
  $('probe-tx').textContent=fmtProbeData(p.tx,p.txdlc);
  if(p.rxa!==undefined){
    $('probe-rx-meta').textContent=id+' · '+fmtAgeMs(p.rxa||0)+'前';
    $('probe-rx').textContent=fmtProbeData(p.rx,p.rxdlc);
  }else{
    $('probe-rx-meta').textContent='尚未看到匹配的RX帧';
    $('probe-rx').textContent='—';
  }
  let text='等待下一个匹配的总线帧';
  let cls='probe-status v-acc';
  if(p.state===2){text='总线上看到匹配帧';cls='probe-status v-ok';}
  else if(p.state===3){text='最新总线帧与注入帧不同';cls='probe-status v-warn';}
  else if(p.state===4){text='驱动发送失败';cls='probe-status v-err';}
  status.textContent=text;
  status.className=cls;
}

function renderEflg(e){
  const el=$('eflg-row');
  if(!e){el.innerHTML='<span class="eflg-pill eflg-ok">正常</span>';return;}
  let h='';
  if(e&0x20)h+='<span class="eflg-pill eflg-err">Bus-Off</span>';
  if(e&0x10)h+='<span class="eflg-pill eflg-warn">TX Passive</span>';
  if(e&0x08)h+='<span class="eflg-pill eflg-warn">RX Passive</span>';
  if(e&0x04)h+='<span class="eflg-pill eflg-warn">TX Warn</span>';
  if(e&0x02)h+='<span class="eflg-pill eflg-warn">RX Warn</span>';
  if(e&0xC0)h+='<span class="eflg-pill eflg-err">RX Overflow</span>';
  el.innerHTML=h||'<span class="eflg-pill eflg-ok">正常</span>';
}

function togglePause(){
  sniffPaused=!sniffPaused;
  syncSniffPauseButton();
  renderSnifferAndTrans();
}

function renderSniffer(){
  updateSniffIdToggle();
  const filter=$('sniff-filter').value.trim().toLowerCase();
  const el=$('sniffer');
  let frames=sniffFrames;
  if(filter){
    const fid=parseInt(filter);
    if(!isNaN(fid))frames=frames.filter(f=>sniffWireId(f.id)===fid||sniffDbcId(f.id)===fid);
    else frames=frames.filter(f=>f.name&&f.name.toLowerCase().includes(filter));
  }
  $('sniff-count').textContent=frames.length+' 帧';
  if(!frames.length){
    el.innerHTML='<div style="padding:20px;color:var(--tx3);text-align:center;font-size:12px">'+(sniffPaused?'嗅探器已暂停':'暂无帧')+'</div>';
    return;
  }
  const ADIds=new Set([1021,1016,921]);
  el.innerHTML=frames.slice(-30).reverse().map(f=>{
    const hex=Array.from({length:f.dlc},(_,i)=>toHex(f.data[i],2)).join(' ');
    const wireId=sniffWireId(f.id),dbcId=sniffDbcId(f.id),displayId=sniffDisplayId(f.id);
    const altId=sniffShowDbcIds?('Wire 0x'+toHex(wireId,3)):('DBC '+sniffBusLabel()+' 0x'+toHex(dbcId,3));
    return`<div class="sniff-row${ADIds.has(f.id)?' hi':''}">
      <span class="s-ts">${(f.ts/1000).toFixed(1)}s</span>
      <span class="s-id" title="${altId}">0x${toHex(displayId,3)}</span>
      <div><div class="s-data">${hex}</div>${f.name?`<div class="s-name">${f.name}</div>`:''}</div>
    </div>`;
  }).join('');
}

function renderTranslated(){
  const el=$('translated');
  if(!el)return;
  let frames=sniffFrames||[];
  const filter=$('sniff-filter')?$('sniff-filter').value.trim().toLowerCase():'';
  if(filter){
    const fid=parseInt(filter);
    if(!isNaN(fid))frames=frames.filter(f=>sniffWireId(f.id)===fid||sniffDbcId(f.id)===fid);
    else frames=frames.filter(f=>(f.name&&f.name.toLowerCase().includes(filter))||(f.trans&&f.trans.toLowerCase().includes(filter)));
  }
  $('trans-count').textContent=frames.length+' 帧';
  if(!frames.length){
    el.innerHTML='<div style="padding:20px;color:var(--tx3);text-align:center;font-size:12px">'+(sniffPaused?'已暂停':'暂无翻译')+'</div>';
    return;
  }
  const ADIds=new Set([1021,1016,921]);
  el.innerHTML=frames.slice(-30).reverse().map(f=>{
    const wireId=sniffWireId(f.id),dbcId=sniffDbcId(f.id),displayId=sniffDisplayId(f.id);
    const altId=sniffShowDbcIds?('Wire 0x'+toHex(wireId,3)):('DBC '+sniffBusLabel()+' 0x'+toHex(dbcId,3));
    const tr=(f.trans&&f.trans.length)?f.trans:'';
    return`<div class="sniff-row${ADIds.has(f.id)?' hi':''}">
      <span class="s-ts">${(f.ts/1000).toFixed(1)}s</span>
      <span class="s-id" title="${altId}">0x${toHex(displayId,3)}</span>
      <div><div class="s-data" style="font-size:11px;opacity:0.85">${tr?tr:'<span style="color:var(--tx3)">（无已知翻译）</span>'}</div></div>
    </div>`;
  }).join('');
}

function renderSnifferAndTrans(){ renderSniffer(); renderTranslated(); }

async function pollSniffer(){
  return runPoll('frames',async()=>{
    if(sniffPaused||!dashboardStatusOk)return;
    try{const d=await fetchPollJson('/frames',2500);sniffFrames=d.frames||[];renderSnifferAndTrans();}catch(e){}
  });
}

// OTA上传
function fileSelected(file){
  if(!file)return;
  otaFile=file;
  const drop=$('ota-drop');
  drop.querySelector('.ota-text').textContent=file.name;
  drop.querySelector('.ota-sub').textContent=(file.size/1024).toFixed(0)+' KB';
  $('ota-upload-btn').style.display='block';
}

function handleDrop(e){
  e.preventDefault();
  $('ota-drop').classList.remove('drag');
  const file=e.dataTransfer.files[0];
  if(file&&file.name.endsWith('.bin'))fileSelected(file);
}

function resetOtaCredentials(){
  localStorage.removeItem('otaU');
  localStorage.removeItem('otaP');
  otaUser='';
  otaPass='';
  const btn=$('ota-reset-btn');
  if(btn){
    btn.textContent='OTA凭据已重置';
    setTimeout(()=>{btn.textContent='重置OTA凭据';},1500);
  }
}

async function uploadFirmware(){
  if(!otaFile)return;
  if(!otaUser){otaUser=prompt('OTA用户名:')||'';localStorage.setItem('otaU',otaUser);}
  if(!otaPass){otaPass=prompt('OTA密码:')||'';localStorage.setItem('otaP',otaPass);}
  if(!otaUser||!otaPass)return;
  const prog=$('ota-progress');
  const fill=$('ota-fill');
  const status=$('ota-status');
  prog.style.display='block';
  $('ota-upload-btn').disabled=true;
  $('ota-upload-btn').textContent='刷写中...';

  const xhr=new XMLHttpRequest();
  xhr.upload.onprogress=e=>{
    if(e.lengthComputable){
      const pct=Math.round(e.loaded/e.total*100);
      fill.style.width=pct+'%';
      status.textContent='上传中... '+pct+'%';
    }
  };
  xhr.onload=()=>{
    if(xhr.status===200){
      status.textContent='完成！设备正在重启...';
      fill.style.width='100%';
      setTimeout(()=>window.location.reload(),5000);
    } else {
      status.textContent='上传失败: '+xhr.status;
      status.style.color='var(--err)';
    }
    $('ota-upload-btn').disabled=false;
    $('ota-upload-btn').textContent='刷写固件';
  };
  xhr.onerror=()=>{
    status.textContent='连接错误';
    status.style.color='var(--err)';
    $('ota-upload-btn').disabled=false;
  };
  xhr.open('POST','/update',true,otaUser,otaPass);
  xhr.setRequestHeader('X-File-Name',otaFile.name);
  xhr.setRequestHeader('X-File-Size',otaFile.size);
  const form=new FormData();
  form.append('firmware',otaFile);
  xhr.send(form);
}

async function poll(){
  return runPoll('status',async()=>{
    try{
      const d=await fetchPollJson('/status',5000,true);
    const on=!!d.can,armed=!!d.ci,injecting=typeof d.ia==='undefined'?armed:!!d.ia,fpsVal=Number(d.fps||0);
    state.hw=d.hw;state.sp=d.sp;state.spAuto=typeof d.spAuto==='undefined'?state.spAuto:!!d.spAuto;state.can=armed;
    state.gate=d.gate||null;
    if(typeof d.plgr!=='undefined')updatePluginReplayControl(d.plgr,d.plgrmax);
    if(typeof d.apGate!=='undefined')updateApGateControl(d);
    setClass('dot','sdot '+(d.txerr>5?'dot-warn':on?'dot-on':'dot-off'));
    setText('hdr-desc',on?(injecting?(d.AD?'AD激活 — 注入中':'CAN激活 — 注入中'):(armed&&d.apGate?'等待AP — 注入就绪':'CAN激活 — 监控中')):'等待CAN帧');
    updateInjectButtons(armed);

    setText('s-can',on?'在线':'离线');
    setClass('s-can','stat-val '+(on?'v-ok':'v-err'));
    setText('s-inj',injecting?'活跃':(armed&&d.apGate?'等待AP':'已阻止'));
    setClass('s-inj','stat-val '+(injecting?'v-ok':(armed&&d.apGate?'v-warn':'v-err')));
    setText('s-AD',d.AD?'活跃':'未激活');
    setClass('s-AD','stat-val '+(d.AD?'v-ok':'v-dim'));
    setText('s-fps',fpsVal.toFixed(1)+' Hz');
    setClass('s-fps','stat-val '+(fpsVal>5?'v-acc':'v-dim'));
    setText('s-rx',d.rx);
    setText('s-tx',d.tx);
    setText('s-txerr',d.txerr);
    setClass('s-txerr','stat-val '+(d.txerr>0?'v-warn':'v-dim'));
    setText('s-fd',d.fd||'—');
    setText('s-prof',profileDisplayName(d.hw,state.sp,state.spAuto));
    setText('s-up',fmtUp(d.up));
    setText('s-mcp-raw','EFLG: 0x'+toHex(d.eflg,2));
    const fpsFill=$('fps-fill');if(fpsFill)fpsFill.style.width=Math.min(fpsVal/20*100,100)+'%';
    setText('hw-badge',HW[d.hw]||'?');
    updateGtwBadge(d.gtwap);
    try{renderEflg(d.eflg);}catch(e){}
    try{renderWriteProbe(d.probe);}catch(e){}
    if(d.mux){for(let i=0;i<3;i++){setText('m'+i+'rx',d.mux[i].rx);setText('m'+i+'tx',d.mux[i].tx);const e=$('m'+i+'err');if(e){e.textContent=d.mux[i].err;e.style.color=d.mux[i].err>0?'var(--err)':'';}}}
    updateSniffIdToggle();
    const eprn=$('tgl-eprn');if(eprn&&typeof d.eprn!=='undefined')eprn.checked=d.eprn;
    if(!dashboardInitialLoaded){
      dashboardInitialLoaded=true;
      pollLog();pollSniffer();pollPlugins();loadWifiNetworks();loadWifiStatus();loadApStatus();loadUpdateInfo();loadCanPins();
    }
    }catch(e){}
  });
}

function colorLog(l){
  if(l.includes('AD=ON')||l.includes('AD active'))return'<span class="lf">'+l+'</span>';
  if(l.match(/\[HW[34]\]|\[LEGACY\]|\[HW3\]/))return'<span class="lh">'+l+'</span>';
  if(l.includes('ERR')||l.includes('FAIL'))return'<span class="le">'+l+'</span>';
  if(l.includes('[CFG]'))return'<span class="lc">'+l+'</span>';
  if(l.includes('[OK]')||l.includes('[BOOT]'))return'<span class="lf">'+l+'</span>';
  if(l.includes('[OTA]'))return'<span class="lo">'+l+'</span>';
  return l;
}
async function pollLog(){
  return runPoll('log',async()=>{
    if(!$('tgl-eprn').checked||!dashboardStatusOk)return;
    try{
      const d=await fetchPollJson('/log?since='+logSince,2000);
    if(d.seq)logSince=d.seq;
    if(!d.lines.length)return;
    const el=$('log');
    const newHtml=d.lines.map(colorLog).join('\n');
    if(el.textContent==='等待中...')el.innerHTML=newHtml;
    else el.innerHTML+='\n'+newHtml;
    // 修剪为100行
    const lines=el.innerHTML.split('\n');
    if(lines.length>100)el.innerHTML=lines.slice(-100).join('\n');
    el.scrollTop=el.scrollHeight;
    }catch(e){}
  });
}

async function resetStats(){try{await fetch('/reset_stats',{method:'POST'});}catch(e){}poll();}

let recIsActive=false,recInterval=null;
async function toggleRec(){recIsActive?await stopRec():await startRec();}
async function startRec(){
  try{
    await fetch('/rec_start',{method:'POST'});
    recIsActive=true;
    const b=$('rec-btn');
    b.textContent='停止录制';
    b.style.borderColor='var(--err)';b.style.color='var(--err)';
    $('rec-dl').style.display='none';
    recInterval=setInterval(pollRec,800);
  }catch(e){}
}
async function stopRec(){
  clearInterval(recInterval);recIsActive=false;
  try{await fetch('/rec_stop',{method:'POST'});}catch(e){}
  const b=$('rec-btn');
  b.textContent='开始录制';b.style.borderColor='';b.style.color='';
  await pollRec();
}
async function pollRec(){
  try{
    const d=await(await fetch('/rec_status')).json();
    const pct=Math.min(d.count/d.cap*100,100);
    $('rec-fill').style.width=pct+'%';
    $('rec-count').textContent=d.count+' / '+d.cap+' 帧';
    if(d.active){
      $('rec-status').textContent='录制中...';$('rec-status').style.color='var(--err)';
      $('rec-meta').textContent='录制中...';
    } else {
      $('rec-meta').textContent=d.saved?d.count+' 帧已保存':'空闲';
      $('rec-status').textContent=d.saved?'已保存':'就绪';
      $('rec-status').style.color=d.saved?'var(--ok)':'';
      $('rec-dl').style.display=d.saved?'':'none';
      if(recIsActive){recIsActive=false;clearInterval(recInterval);const b=$('rec-btn');b.textContent='开始录制';b.style.borderColor='';b.style.color='';}
    }
  }catch(e){}
}

// ── AP热点管理 ──
async function saveAP(){
  const ssid=$('ap-ssid').value,pass=$('ap-pass').value,hidden=$('ap-hidden').checked?'1':'0';
  if(!ssid){$('ap-status').textContent='请输入热点名称';$('ap-status').style.color='var(--err)';return;}
  if(pass&&pass.length<8){$('ap-status').textContent='密码至少8个字符';$('ap-status').style.color='var(--err)';return;}
  try{const r=await fetch('/ap_config',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:'ssid='+encodeURIComponent(ssid)+'&pass='+encodeURIComponent(pass)+'&hidden='+hidden});
    const d=await r.json();
    if(d.ok){$('ap-status').textContent='已保存！重启后生效。';$('ap-status').style.color='var(--ok)';$('ap-pass').value='';}
    else{$('ap-status').textContent=d.error||'错误';$('ap-status').style.color='var(--err)';}
  }catch(e){$('ap-status').textContent='错误';$('ap-status').style.color='var(--err)';}
}
async function loadApStatus(){
  return runPoll('ap_status',async()=>{
    if(!dashboardStatusOk)return;
    try{const d=await fetchPollJson('/ap_status',2000);
    if(d.ssid)$('ap-ssid').value=d.ssid;
    $('ap-clients').textContent=d.clients+' 客户端';
    if(typeof d.hidden!=='undefined')$('ap-hidden').checked=!!d.hidden;
    if(d.stored){$('ap-stored').textContent='已保存';$('ap-stored').style.color='var(--ok)';}
    else{$('ap-stored').textContent='固件默认';$('ap-stored').style.color='var(--tx3)';}
    }catch(e){}
  });
}
// ── WiFi管理 ──
function toggleStaticIP(){
  $('static-fields').style.display=$('wifi-static').checked?'block':'none';
}
function rssiIcon(r){
  if(r>=-50) return '\u2587\u2587\u2587\u2587';
  if(r>=-60) return '\u2587\u2587\u2587\u2581';
  if(r>=-70) return '\u2587\u2587\u2581\u2581';
  return '\u2587\u2581\u2581\u2581';
}
async function scanWifi(){
  $('scan-btn').textContent='扫描中...';$('scan-btn').disabled=true;
  try{
    const r=await fetch('/wifi_scan');const d=await r.json();
    const el=$('wifi-nets');
    if(!d.networks.length){el.innerHTML='<div style="padding:8px;font-size:11px;color:var(--tx3);text-align:center">未找到网络</div>';el.style.display='block';}
    else{el.innerHTML=d.networks.map(n=>'<div onclick="pickWifi(\''+n.ssid.replace(/'/g,"\\'")+'\')" style="padding:6px 10px;cursor:pointer;display:flex;justify-content:space-between;align-items:center;border-bottom:1px solid var(--bd);font-size:12px" onmouseover="this.style.background=\'var(--bg)\'" onmouseout="this.style.background=\'\'"><span>'+(n.enc?'\uD83D\uDD12 ':'')+n.ssid+'</span><span style="color:var(--tx3);font-size:10px">'+rssiIcon(n.rssi)+' '+n.rssi+'dBm CH'+n.ch+'</span></div>').join('');el.style.display='block';}
  }catch(e){$('wifi-status').textContent='扫描失败';$('wifi-status').style.color='var(--err)';}
  $('scan-btn').textContent='扫描';$('scan-btn').disabled=false;
}
function pickWifi(ssid){
  $('wifi-ssid').value=ssid;$('wifi-nets').style.display='none';$('wifi-pass').focus();
}
let wifiSlotCache={count:0,max:4,active:-1,networks:[]};
let wifiStatusCache={};
function escapeHtml(s){return String(s||'').replace(/&/g,'&amp;').replace(/</g,'&lt;').replace(/>/g,'&gt;').replace(/"/g,'&quot;').replace(/'/g,'&#39;');}
function renderWifiSlots(){
  const list=$('wifi-saved-list'),wrap=$('wifi-add-wrap'),cnt=$('wifi-slot-count');
  if(!list)return;
  const nets=wifiSlotCache.networks||[];
  const max=wifiSlotCache.max||4;
  const active=wifiSlotCache.active;
  cnt.textContent='('+nets.length+'/'+max+')';
  if(!nets.length){
    list.innerHTML='<div style="font-size:11px;color:var(--tx3);padding:6px 0">未保存网络。</div>';
  }else{
    list.innerHTML=nets.map(n=>{
      const isActive=n.idx===active;
      const dot='<span style="display:inline-block;width:8px;height:8px;border-radius:50%;background:'+(isActive?'var(--ok)':'var(--tx3)')+';margin-right:6px"></span>';
      const tag=n.static?'<span style="font-size:10px;color:var(--tx3);margin-left:6px">[static]</span>':'';
      return '<div style="display:flex;align-items:center;gap:6px;padding:6px 0;border-bottom:1px solid var(--bd);font-size:12px">'+
        '<div style="flex:1;min-width:0;overflow:hidden;text-overflow:ellipsis;white-space:nowrap">'+dot+escapeHtml(n.ssid)+tag+'</div>'+
        '<button class="sniff-btn" onclick="editWifiSlot('+n.idx+')" style="padding:4px 8px;font-size:11px">编辑</button>'+
        '<button class="sniff-btn" onclick="deleteWifiSlot('+n.idx+')" style="padding:4px 8px;font-size:11px;background:var(--errBg);border-color:var(--errBd);color:var(--err)">删除</button>'+
      '</div>';
    }).join('');
  }
  const editIdx=parseInt($('wifi-edit-idx').value,10);
  const canAdd=nets.length<max||editIdx>=0;
  wrap.style.display=canAdd?'':'none';
  $('wifi-save-btn').textContent=editIdx>=0?'保存更改':'保存并连接';
}
async function loadWifiNetworks(){
  return runPoll('wifi_networks',async()=>{
    try{
      const d=await fetchPollJson('/wifi_networks',2000);
      wifiSlotCache=d;
      renderWifiSlots();
    }catch(e){}
  });
}
async function loadWifiStatus(){
  return runPoll('wifi_status',async()=>{
    try{const d=await fetchPollJson('/wifi_status',2000);
    wifiStatusCache=d;
    dashboardStaIp=d.connected&&d.ip?d.ip:'';
    if(typeof d.active==='number')wifiSlotCache.active=d.active;
    renderWifiSlots();
    if(d.connected){
      $('wifi-status').textContent=(d.ip&&d.ip!==location.hostname)?('已连接: '+(d.ssid||'')+' \u2022 '+d.ip+' \u2022 切换到该WiFi并打开此IP'):('已连接: '+(d.ssid||'')+' \u2022 '+d.ip);
      $('wifi-status').style.color='var(--ok)';
    }
    else if(d.connecting&&d.ssid){
      $('wifi-status').textContent='正在连接 '+d.ssid+'...';$('wifi-status').style.color='var(--acc)';
    }
    else if(d.count>0){
      $('wifi-status').textContent=d.count+' 已保存 \u2022 尝试连接...';
      $('wifi-status').style.color='var(--tx3)';
    }
    else{
      $('wifi-status').textContent='未配置';
      $('wifi-status').style.color='var(--tx3)';
    }
    }catch(e){}
  });
}
function editWifiSlot(idx){
  const n=(wifiSlotCache.networks||[]).find(x=>x.idx===idx);
  if(!n)return;
  $('wifi-edit-idx').value=idx;
  $('wifi-ssid').value=n.ssid;
  $('wifi-pass').value='';
  $('wifi-pass').placeholder='留空以保留当前密码';
  $('wifi-static').checked=!!n.static;
  toggleStaticIP();
  if(n.static){
    $('wifi-ip').value=n.ip||'';
    $('wifi-gw').value=n.gw||'';
    $('wifi-mask').value=n.mask||'255.255.255.0';
    $('wifi-dns').value=n.dns||'';
  }
  renderWifiSlots();
  $('wifi-add-wrap').scrollIntoView({behavior:'smooth',block:'nearest'});
}
function clearWifiForm(){
  $('wifi-edit-idx').value=-1;
  $('wifi-ssid').value='';$('wifi-pass').value='';
  $('wifi-pass').placeholder='密码';
  $('wifi-static').checked=false;toggleStaticIP();
  $('wifi-ip').value='';$('wifi-gw').value='';$('wifi-mask').value='255.255.255.0';$('wifi-dns').value='';
}
async function deleteWifiSlot(idx){
  const n=(wifiSlotCache.networks||[]).find(x=>x.idx===idx);
  if(!n)return;
  if(!await dashConfirm('删除网络 "'+n.ssid+'"?','删除WiFi','删除'))return;
  try{
    await fetch('/wifi_delete',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:'idx='+idx});
    if(parseInt($('wifi-edit-idx').value,10)===idx)clearWifiForm();
    loadWifiNetworks();loadWifiStatus();
  }catch(e){$('wifi-status').textContent='删除失败';$('wifi-status').style.color='var(--err)';}
}
async function saveWifi(){
  const ssid=$('wifi-ssid').value,pass=$('wifi-pass').value;
  if(!ssid){$('wifi-status').textContent='请输入SSID';$('wifi-status').style.color='var(--err)';return;}
  const editIdx=parseInt($('wifi-edit-idx').value,10);
  const isEdit=editIdx>=0;
  if(!isEdit&&(wifiSlotCache.count||0)>=(wifiSlotCache.max||4)){
    $('wifi-status').textContent='最多'+(wifiSlotCache.max||4)+'个网络';$('wifi-status').style.color='var(--err)';return;
  }
  let effectivePass=pass;
  if(isEdit&&!pass){
    const orig=(wifiSlotCache.networks||[]).find(x=>x.idx===editIdx);
    if(orig&&!orig.hasPass)effectivePass='';
    else if(!pass){$('wifi-status').textContent='需要密码';$('wifi-status').style.color='var(--err)';return;}
  }
  let body='ssid='+encodeURIComponent(ssid)+'&pass='+encodeURIComponent(effectivePass);
  if(isEdit)body+='&idx='+editIdx;
  if($('wifi-static').checked){
    body+='&static=1&ip='+encodeURIComponent($('wifi-ip').value)+'&gw='+encodeURIComponent($('wifi-gw').value)+'&mask='+encodeURIComponent($('wifi-mask').value)+'&dns='+encodeURIComponent($('wifi-dns').value);
  }
  try{
    const r=await fetch('/wifi_config',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body});
    const d=await r.json();
    if(!d.ok)throw new Error(d.error||'保存失败');
    $('wifi-status').textContent='正在连接到 '+ssid+'...';$('wifi-status').style.color='var(--acc)';
    clearWifiForm();
    loadWifiNetworks();
    setTimeout(loadWifiStatus,500);
    setTimeout(loadWifiStatus,2500);
    setTimeout(loadWifiStatus,5500);
  }catch(e){$('wifi-status').textContent=e.message||'错误';$('wifi-status').style.color='var(--err)';}
}
// ── 插件管理 ──
async function installPlugin(){
  const url=$('plg-url').value;
  if(!url){$('plg-status').textContent='请输入URL';return;}
  const beforeSig=pluginStateSignature(installedPlugins);
  $('plg-status').textContent='下载中...';$('plg-status').style.color='var(--acc)';
  try{
    await fetchJsonWithTimeout('/plugin_install',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:'url='+encodeURIComponent(url)},20000);
    $('plg-url').value='';
    try{await refreshPluginsNow();}catch(e){await refreshPluginsAfterAction(beforeSig);}
    $('plg-status').textContent='已安装';$('plg-status').style.color='var(--ok)';
  }catch(e){
    if(await refreshPluginsAfterAction(beforeSig)){
      $('plg-url').value='';
      $('plg-status').textContent='已安装';$('plg-status').style.color='var(--ok)';
    }else{$('plg-status').textContent=actionErrorMessage(e,'连接错误');$('plg-status').style.color='var(--err)';}
  }
}
async function uploadPlugin(file){
  if(!file)return;
  const beforeSig=pluginStateSignature(installedPlugins);
  $('plg-status').textContent='上传中...';$('plg-status').style.color='var(--acc)';
  try{
    const text=await file.text();
    await fetchJsonWithTimeout('/plugin_upload',{method:'POST',headers:{'Content-Type':'application/json'},body:text},5000);
    try{await refreshPluginsNow();}catch(e){await refreshPluginsAfterAction(beforeSig);}
    $('plg-status').textContent='已安装';$('plg-status').style.color='var(--ok)';
  }catch(e){
    if(await refreshPluginsAfterAction(beforeSig)){
      $('plg-status').textContent='已安装';$('plg-status').style.color='var(--ok)';
    }else{$('plg-status').textContent=actionErrorMessage(e,'错误');$('plg-status').style.color='var(--err)';}
  }
}
async function pastePlugin(){
  const text=$('plg-paste').value.trim();
  if(!text){$('plg-status').textContent='请先粘贴JSON';$('plg-status').style.color='var(--err)';return;}
  try{JSON.parse(text);}catch(e){$('plg-status').textContent='无效JSON: '+e.message;$('plg-status').style.color='var(--err)';return;}
  const beforeSig=pluginStateSignature(installedPlugins);
  $('plg-status').textContent='安装中...';$('plg-status').style.color='var(--acc)';
  try{
    await fetchJsonWithTimeout('/plugin_upload',{method:'POST',headers:{'Content-Type':'application/json'},body:text},5000);
    $('plg-paste').value='';
    try{await refreshPluginsNow();}catch(e){await refreshPluginsAfterAction(beforeSig);}
    $('plg-status').textContent='已安装';$('plg-status').style.color='var(--ok)';
  }catch(e){
    if(await refreshPluginsAfterAction(beforeSig)){
      $('plg-paste').value='';
      $('plg-status').textContent='已安装';$('plg-status').style.color='var(--ok)';
    }else{$('plg-status').textContent=actionErrorMessage(e,'连接错误');$('plg-status').style.color='var(--err)';}
  }
}
async function togglePlugin(idx){
  const beforeSig=pluginStateSignature(installedPlugins);
  try{
    const d=await fetchJsonWithTimeout('/plugin_toggle',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:'idx='+idx},4000);
    if(installedPlugins[idx]&&typeof d.enabled==='boolean'){
      installedPlugins[idx].enabled=d.enabled;
      renderPluginsState({plugins:installedPlugins,maxPlugins:pluginMax});
    }else{
      try{await refreshPluginsNow();}catch(e){await refreshPluginsAfterAction(beforeSig);}
    }
  }catch(e){await refreshPluginsAfterAction(beforeSig);}
}
async function removePlugin(idx){
  if(!await dashConfirm('移除这个插件？','移除插件','移除'))return;
  const beforeSig=pluginStateSignature(installedPlugins);
  try{
    await fetchJsonWithTimeout('/plugin_remove',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:'idx='+idx},4000);
    try{await refreshPluginsNow();}catch(e){await refreshPluginsAfterAction(beforeSig);}
  }catch(e){await refreshPluginsAfterAction(beforeSig);}
}
async function setPluginPriority(idx,value){
  const to=parseInt(value,10)-1;
  if(isNaN(to)||to===idx){renderPluginsState({plugins:installedPlugins,maxPlugins:pluginMax});return;}
  const beforeSig=pluginStateSignature(installedPlugins);
  try{
    await fetchJsonWithTimeout('/plugin_priority',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:'idx='+idx+'&priority='+to},4000);
    try{await refreshPluginsNow();}catch(e){await refreshPluginsAfterAction(beforeSig);}
  }catch(e){await refreshPluginsAfterAction(beforeSig);}
}
function pluginByteBits(byte,mask){
  byte=parseInt(byte,10);mask=(parseInt(mask,10)||0)&255;
  const bits=[];
  if(isNaN(byte)||byte<0||byte>7)return bits;
  for(let b=0;b<8;b++)if(mask&(1<<b))bits.push(byte*8+b);
  return bits;
}
function pluginOpBits(o){
  if(!o)return[];
  if(o.type==='set_bit'){
    const bit=parseInt(o.bit,10);
    return !isNaN(bit)&&bit>=0&&bit<64?[bit]:[];
  }
  if(o.type==='set_byte')return pluginByteBits(o.byte,o.mask===undefined?255:o.mask);
  if(o.type==='or_byte')return pluginByteBits(o.byte,o.val||0);
  if(o.type==='and_byte')return pluginByteBits(o.byte,(~(o.val===undefined?255:o.val))&255);
  if(o.type==='counter')return pluginByteBits(o.byte,o.mask===undefined?15:o.mask);
  if(o.type==='checksum')return pluginByteBits(7,255);
  if(o.type==='emit_periodic')return[];
  return[];
}
function pluginRuleMuxMask(r){return r&&typeof r.mux_mask==='number'?r.mux_mask:peDefaultMuxMask(r&&typeof r.mux==='number'?r.mux:-1);}
function pluginRuleMatchMask(r){return r&&typeof r.match_mask==='number'?r.match_mask:0;}
function pluginMuxesOverlap(a,am,b,bm){if(a<0||b<0)return true;const mask=(am||peDefaultMuxMask(a))&(bm||peDefaultMuxMask(b));return !mask||((a&mask)===(b&mask));}
function pluginBusesOverlap(a,b){return !a||!b||((a&b)!==0);}
function pluginByteMatchesOverlap(a,b){
  const am=pluginRuleMatchMask(a),bm=pluginRuleMatchMask(b);
  if(!am||!bm)return true;
  if(a.match_byte!==b.match_byte)return true;
  const mask=am&bm;
  return !mask||(((a.match_val||0)&mask)===((b.match_val||0)&mask));
}
function pluginFormatBits(bits){
  bits=Array.from(new Set(bits)).sort((a,b)=>a-b);
  if(bits.length>6)return bits.slice(0,6).join(', ')+', +'+(bits.length-6)+' more';
  return bits.join(', ');
}
function pluginConflictGroups(conflicts){
  const grouped={};
  (conflicts||[]).forEach(c=>{
    const key=c.winner+'|'+c.winnerPriority;
    if(!grouped[key])grouped[key]={winner:c.winner,winnerPriority:c.winnerPriority,bits:[]};
    grouped[key].bits.push(c.bit);
  });
  return Object.keys(grouped).map(k=>grouped[k]);
}
function pluginAnalyzePriority(list){
  const owners=[];
  (list||[]).forEach((p,i)=>{
    p.priority=i+1;p.hasConflict=false;
    (p.details||[]).forEach(r=>{r.pluginConflict=false;r.conflicts=[];});
  });
  (list||[]).forEach((p,i)=>{
    if(!p.enabled)return;
    (p.details||[]).forEach(r=>{
      if(r.send===false)return;
      const conflicts=[];
      (r.ops||[]).forEach(o=>{
        pluginOpBits(o).forEach(bit=>{
          const owner=owners.find(x=>x.id===r.id&&pluginBusesOverlap(x.bus,r.bus)&&pluginMuxesOverlap(x.mux,x.mux_mask,r.mux,pluginRuleMuxMask(r))&&pluginByteMatchesOverlap(x,r)&&x.bit===bit);
          if(owner){
            if(owner.plugin!==p.name)conflicts.push({bit:bit,winner:owner.plugin,winnerPriority:owner.priority});
          }else owners.push({id:r.id,bus:r.bus||0,mux:r.mux,mux_mask:pluginRuleMuxMask(r),match_byte:r.match_byte,match_mask:r.match_mask,match_val:r.match_val,bit:bit,plugin:p.name,priority:i+1});
        });
      });
      if(conflicts.length){r.pluginConflict=true;r.conflicts=conflicts;p.hasConflict=true;}
    });
  });
}
function renderPluginConflictPanel(list){
  const el=$('plg-conflicts');
  if(!el)return;
  if(!list.length){el.style.display='none';el.innerHTML='';return;}
  const first=(list||[]).find(p=>p.enabled);
  const rows=[];
  (list||[]).forEach(p=>(p.details||[]).forEach(r=>{
    if(r.pluginConflict)pluginConflictGroups(r.conflicts).forEach(g=>{
      rows.push('<div>CAN '+r.hex+(r.bus?(' '+peBusLabel(r.bus)):'')+(r.mux>=0?' mux '+r.mux+'/0x'+pluginRuleMuxMask(r).toString(16):' 任意mux')+': '+p.name+' 忽略位 '+pluginFormatBits(g.bits)+'; '+g.winner+' (#'+g.winnerPriority+') 胜出</div>');
    });
  }));
  let h='<div style="padding:8px;background:var(--bg2);border:1px solid var(--bd);border-radius:6px;font-size:11px;color:var(--tx3);line-height:1.5">';
  h+='<div style="color:var(--tx2);font-weight:600;margin-bottom:3px">注入优先级</div>';
  h+=first?('#'+first.priority+' '+first.name+' 是第一个启用的插件；合并帧在插件位解析后注入；GTW 2047使用配置的重放次数。'):
    '没有启用的插件正在注入帧。';
  if(rows.length){
    h+='<div style="margin-top:6px;color:var(--warn)">'+rows.slice(0,5).join('')+(rows.length>5?'<div>+'+(rows.length-5)+' 更多冲突</div>':'')+'</div>';
  }
  el.innerHTML=h+'</div>';
  el.style.display='block';
}
function pluginPrioritySelect(idx,total){
  if(total<2)return'';
  let h='<select class="sniff-input" title="设置注入优先级" onchange="setPluginPriority('+idx+',this.value)" style="flex:0 0 58px;width:58px;margin-left:8px;padding:4px;font-size:10px">';
  for(let i=1;i<=total;i++)h+='<option value="'+i+'" '+(i===idx+1?'selected':'')+'>#'+i+'</option>';
  return h+'</select>';
}
function fmtOp(o){
  if(o.type==='set_bit') return 'set_bit('+o.bit+', '+(o.val?'true':'false')+')';
  if(o.type==='checksum') return 'checksum(byte 7)';
  if(o.type==='counter') return 'counter('+o.byte+', mask=0x'+((o.mask===undefined?15:o.mask)&255).toString(16)+', step='+(o.step||1)+')';
  if(o.type==='emit_periodic') return 'emit_periodic('+((o.interval||100)|0)+' ms'+(o.gtw_silent?', GTW silent':'')+')';
  if(o.type==='set_byte') return 'set_byte('+o.byte+', 0x'+o.val.toString(16)+', mask=0x'+o.mask.toString(16)+')';
  if(o.type==='or_byte') return 'or_byte('+o.byte+', 0x'+o.val.toString(16)+')';
  if(o.type==='and_byte') return 'and_byte('+o.byte+', 0x'+o.val.toString(16)+')';
  return o.type;
}
function fmtRuleMatch(r){
  const mask=pluginRuleMatchMask(r);
  if(!mask)return '';
  return 'byte['+(r.match_byte||0)+']&0x'+(mask&255).toString(16)+'=0x'+((r.match_val||0)&255).toString(16);
}
function fmtRuleDiag(r){
  const d=r.diag||{};
  return 'diag match='+(d.match_count||0)+
    ' changed='+(d.changed_count||0)+
    ' tx_ok='+(d.send_ok_count||0)+
    ' tx_fail='+(d.send_fail_count||0)+
    (d.last_original?' original='+d.last_original:'')+
    (d.last_modified?' modified='+d.last_modified:'');
}
function renderPluginDetails(details){
  return '<div style="margin-top:6px;padding:8px;background:var(--bg2);border-radius:6px;font-size:11px;font-family:monospace">'
    +details.map(r=>{
      let hdr='<div style="margin-bottom:4px"><b>CAN '+r.hex+' ('+r.id+')</b>';
      if(r.bus) hdr+=' <span style="color:var(--acc)">'+peBusLabel(r.bus)+'</span>';
      if(r.mux>=0) hdr+=' <span style="color:var(--acc)">mux='+r.mux+'/0x'+pluginRuleMuxMask(r).toString(16)+'</span>';
      if(pluginRuleMatchMask(r)) hdr+=' <span style="color:var(--acc)">'+fmtRuleMatch(r)+'</span>';
      if(r.pluginConflict) hdr+=' <span style="color:var(--warn);font-weight:bold" title="低优先级位将被忽略">&#9888; 优先级重叠</span>';
      hdr+='</div>';
      let ops=r.ops.map(o=>'<div style="padding-left:12px;color:var(--tx2)">'+fmtOp(o)+'</div>').join('');
      ops+='<div style="padding-left:12px;color:var(--tx3)">'+fmtRuleDiag(r)+'</div>';
      let notes='';
      if(r.pluginConflict){
        notes='<div style="margin-top:4px;padding-left:12px;color:var(--warn)">'+pluginConflictGroups(r.conflicts).map(g=>'位 '+pluginFormatBits(g.bits)+' 被忽略; '+g.winner+' (#'+g.winnerPriority+') 胜出').join('<br>')+'</div>';
      }
      return hdr+ops+notes;
    }).join('<div style="border-top:1px solid var(--bd);margin:4px 0"></div>')
    +'</div>';
}
function toggleDetails(idx){
  var p=installedPlugins[idx];
  if(!p||!p.name)return;
  pluginDetailOpen[p.name]=!pluginDetailOpen[p.name];
  var el=$('plg-det-'+idx);
  if(el)el.style.display=pluginDetailOpen[p.name]?'block':'none';
}
function toggleInfo(id){
  var el=$(id);
  if(el)el.style.display=el.style.display==='none'?'block':'none';
}

function openHelpParent(btn){
  var sec=btn.closest('.subsec');
  if(sec&&sec.classList.contains('collapsed')){
    sec.classList.remove('collapsed');
    if(sec.dataset.collapseKey)localStorage.setItem(sec.dataset.collapseKey,'0');
    var secBtn=sec.querySelector('.subsec-btn');
    if(secBtn)secBtn.textContent='收起';
  }
  var card=btn.closest('.card');
  if(card&&card.classList.contains('collapsed')){
    card.classList.remove('collapsed');
    if(card.dataset.collapseKey)localStorage.setItem(card.dataset.collapseKey,'0');
    var cardBtn=card.querySelector('.card-min-btn');
    if(cardBtn)cardBtn.textContent='收起';
  }
}

function closeHelpPanels(scope,exceptId){
  (scope||document).querySelectorAll('.inline-help-panel').forEach(function(panel){
    if(!exceptId||panel.id!==exceptId)panel.classList.remove('show');
  });
}

function toggleHelp(btn,ev){
  if(ev){
    ev.preventDefault();
    ev.stopPropagation();
  }

  openHelpParent(btn);

  var targetId=btn.getAttribute('data-help-target');
  var panelId=btn.getAttribute('data-inline-help-id');
  var panel=panelId?document.getElementById(panelId):null;

  if(!panel){
    panel=document.createElement('div');
    panel.className='inline-help-panel';
    panelId='inline-help-'+Math.random().toString(36).slice(2,10);
    panel.id=panelId;
    btn.setAttribute('data-inline-help-id',panelId);

    var anchor=btn.closest('.subsec-head, .card-hdr, .setting-name, summary');
    if(anchor&&anchor.parentNode){
      anchor.insertAdjacentElement('afterend',panel);
    }else if(btn.parentNode){
      btn.parentNode.insertAdjacentElement('afterend',panel);
    }
  }

  if(targetId){
    var target=document.getElementById(targetId);
    panel.innerHTML=target?target.innerHTML:'暂无更多信息。';
  }else{
    panel.textContent=btn.getAttribute('data-help')||btn.getAttribute('title')||'暂无更多信息。';
  }

  var scope=btn.closest('.subsec, .card')||document;
  var willShow=!panel.classList.contains('show');
  closeHelpPanels(scope,panelId);
  panel.classList.toggle('show',willShow);
  return false;
}

function pluginStateSignature(list){
  return JSON.stringify((list||[]).map(p=>[p&&p.name||'',p&&p.version||'',!!(p&&p.enabled),p&&p.rules||0,p&&p.author||'']));
}

const GTW_UDS_STATE_NAMES=['Idle','Session req','Seed req','Key sent','CommCtrl sent','Active','Failed'];
function renderGtwUdsStatus(d){
  const el=$('plg-gtw-status');if(!el)return;
  const supported=!!d.gtw_silent_supported;
  const uds=d.gtw_uds||{};
  const active=pluginHasAnyGtwSilent(d.plugins||[]);
  if(!active&&!supported){el.style.display='none';return;}
  el.style.display='block';
  const stateIdx=typeof uds.state==='number'?uds.state:0;
  const stateName=GTW_UDS_STATE_NAMES[stateIdx]||('State '+stateIdx);
  const stateColor=stateIdx===5?'var(--ok)':stateIdx===6?'var(--err)':'var(--tx3)';
  let h='<div style="padding:8px 10px;background:var(--bg2);border:1px solid var(--bd);border-radius:6px;font-size:11px">';
  h+='<div style="display:flex;align-items:center;gap:8px;flex-wrap:wrap">';
  if(supported){
    h+='<span style="color:var(--ok);font-weight:bold">&#10003; GTW静默: 已加载自定义密钥</span>';
  }else{
    h+='<span style="color:var(--tx3)">&#10007; GTW静默: 无密钥 &mdash; <code>PLUGIN_GTW_UDS_CUSTOM_KEY</code> 未定义, <code>gtw_silent</code> 已禁用</span>';
  }
  if(active&&supported){
    h+='<span style="color:'+stateColor+';margin-left:auto">UDS: '+stateName+'</span>';
    if(uds.last_seed&&uds.last_seed.length>0){
      h+='</div><div style="margin-top:6px;font-family:monospace;color:var(--tx2)">';
      h+='seed&nbsp;&rarr;&nbsp;<b>'+uds.last_seed+'</b>&nbsp;&nbsp;key&nbsp;&rarr;&nbsp;<b style="color:var(--ok)">'+uds.last_key+'</b>';
      if(uds.last_nrc&&uds.last_nrc!==0)h+='&nbsp;&nbsp;<span style="color:var(--err)">NRC 0x'+uds.last_nrc.toString(16)+'</span>';
    }
  }
  h+='</div></div>';
  el.innerHTML=h;
}
function pluginHasAnyGtwSilent(plugins){
  return (plugins||[]).some(p=>(p.details||[]).some(r=>(r.ops||[]).some(o=>o.type==='emit_periodic'&&o.gtw_silent)));
}
function renderPluginsState(d){
  installedPlugins=d.plugins||[];
  pluginAnalyzePriority(installedPlugins);
  const nextOpen={};
  installedPlugins.forEach(p=>{if(p&&p.name&&pluginDetailOpen[p.name])nextOpen[p.name]=true;});
  pluginDetailOpen=nextOpen;
  pluginMax=d.maxPlugins||pluginMax||0;
  renderGtwUdsStatus(d);
  const max=pluginMax;
  $('plg-count').textContent=max?installedPlugins.length+' / '+max+' 已安装':installedPlugins.length+' 已安装';
  if($('plg-limit')){
    const full=max&&installedPlugins.length>=max;
    $('plg-limit').textContent=max?(full?'已达最大'+max+'个插件。请先移除一个再安装。':'最多'+max+'个插件。请先移除一个再安装。'):'最大插件数: --';
    $('plg-limit').style.color=full?'var(--err)':'var(--tx3)';
  }
  const el=$('plg-list');
  if(!installedPlugins.length){
    renderPluginConflictPanel(installedPlugins);
    el.innerHTML='<div style="font-size:12px;color:var(--tx3);text-align:center;padding:12px">未安装插件</div>';
    return;
  }
  renderPluginConflictPanel(installedPlugins);
  el.innerHTML=installedPlugins.map((p,i)=>{
    let detailsOpen=!!pluginDetailOpen[p.name];
    let row='<div style="margin-bottom:8px;padding-bottom:8px;border-bottom:1px solid var(--bd)">';
    row+='<div class="setting-row"><div class="setting-info" style="cursor:pointer" onclick="toggleDetails('+i+')">';
    row+='<div class="setting-name">'+p.name+' <span style="color:var(--tx3);font-size:11px">v'+p.version+'</span>';
    row+='</div>';
    row+='<div class="setting-desc">优先级 #'+(i+1)+(i===0?' 优先':'')+' &bull; '+p.rules+' 条规则'+(p.author?' &bull; '+p.author:'')+(p.hasConflict?' &bull; <span style="color:var(--warn)">重叠已忽略</span>':'')+' &bull; <span style="color:var(--acc);cursor:pointer">详情</span></div>';
    row+='</div>';
    row+=pluginPrioritySelect(i,installedPlugins.length);
    row+='<label class="tgl"><input type="checkbox" '+(p.enabled?'checked':'')+' onchange="togglePlugin('+i+')"><div class="tgl-track"><div class="tgl-thumb"></div></div></label>';
    row+='<button onclick="peLoadInstalledPlugin('+i+')" style="margin-left:8px;padding:4px 8px;border:1px solid var(--bd);border-radius:5px;background:transparent;color:var(--acc);cursor:pointer;font-size:10px;font-family:inherit">编辑</button>';
    row+='<button onclick="removePlugin('+i+')" style="margin-left:8px;padding:4px 8px;border:1px solid var(--errBd);border-radius:5px;background:transparent;color:var(--err);cursor:pointer;font-size:10px;font-family:inherit">X</button></div>';
    if(p.details){
      row+='<div id="plg-det-'+i+'" style="display:'+(detailsOpen?'block':'none')+'">';
      row+=renderPluginDetails(p.details);
      row+='</div>';
    }
    row+='</div>';
    return row;
  }).join('');
}

async function refreshPluginsNow(){
  const d=await fetchJsonWithTimeout('/plugins',null,2500);
  renderPluginsState(d);
  return d;
}

async function refreshPluginsAfterAction(beforeSig){
  for(let i=0;i<4;i++){
    if(i)await waitMs(250);
    try{
      await refreshPluginsNow();
      if(pluginStateSignature(installedPlugins)!==beforeSig)return true;
    }catch(e){}
  }
  return false;
}

async function pollPlugins(){
  return runPoll('plugins',async()=>{
    if(!dashboardStatusOk)return;
    try{renderPluginsState(await fetchPollJson('/plugins',2000));}catch(e){}
  });
}

// ── 固件更新 ──
var pendingUpdateUrl='';
async function toggleBeta(){
  const beta=$('beta-tgl').checked?'1':'0';
  try{await fetch('/update_beta',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:'beta='+beta});}catch(e){}
  $('upd-info').style.display='none';$('upd-status').textContent='';
}
async function checkUpdate(){
  $('upd-check-btn').disabled=true;$('upd-status').textContent='检查中...';$('upd-status').style.color='var(--acc)';
  $('upd-info').style.display='none';pendingUpdateUrl='';
  try{const r=await fetch('/update_check');const d=await r.json();
    if(!d.ok){$('upd-status').textContent=d.error||'错误';$('upd-status').style.color='var(--err)';$('upd-check-btn').disabled=false;return;}
    $('fw-ver').textContent='v'+d.current;
    if(d.update){
      $('upd-status').textContent='有可用更新！';$('upd-status').style.color='var(--ok)';
      $('upd-ver').textContent='v'+d.latest+(d.prerelease?' (beta)':'');
      $('upd-detail').textContent=d.artifact+' \u2022 '+d.tag;
      pendingUpdateUrl=d.url;
      $('upd-info').style.display='block';
    }else{
      $('upd-status').textContent='已是最新 (v'+d.current+')';$('upd-status').style.color='var(--ok)';
    }
  }catch(e){$('upd-status').textContent='连接错误';$('upd-status').style.color='var(--err)';}
  $('upd-check-btn').disabled=false;
}
async function installUpdate(){
  if(!pendingUpdateUrl){$('upd-status').textContent='无更新URL';return;}
  if(!await dashConfirm('安装固件更新？设备将重启。','安装更新','安装'))return;
  $('upd-install-btn').disabled=true;$('upd-status').textContent='下载并安装中...';$('upd-status').style.color='var(--acc)';
  try{await fetch('/update_install',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:'url='+encodeURIComponent(pendingUpdateUrl)});
    $('upd-status').textContent='更新已安装！重启中...';$('upd-status').style.color='var(--ok)';
    setTimeout(()=>location.reload(),15000);
  }catch(e){$('upd-status').textContent='更新失败';$('upd-status').style.color='var(--err)';$('upd-install-btn').disabled=false;}
}
async function loadUpdateInfo(){
  try{const r=await fetch('/update_beta',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:'noop=1'});
    const d=await r.json();
    if(d.version){$('fw-ver').textContent='v'+d.version;updateFoot(d.version);}
    $('beta-tgl').checked=!!d.beta;
  }catch(e){}
  try{const r=await fetch('/auto_update');const d=await r.json();$('auto-upd-tgl').checked=!!d.enabled;}catch(e){}
}
async function toggleAutoUpdate(){
  const en=$('auto-upd-tgl').checked?'1':'0';
  try{await fetch('/auto_update',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:'enabled='+en});}catch(e){}
}
function updateFoot(ver){
  var ip=location.hostname||'192.168.4.1';
  $('dash-foot').textContent='EV开源CAN工具 \u2022 v'+ver+' \u2022 '+ip;
}

async function loadCanPins(){
  try{const r=await fetch('/can_pins');const d=await r.json();
    if(d.tx>=0)$('can-tx').value=d.tx;
    if(d.rx>=0)$('can-rx').value=d.rx;
    $('can-pins-status').textContent=d.customized?('自定义 TX='+d.tx+' RX='+d.rx):('固件默认 TX='+d.tx+' RX='+d.rx);
  }catch(e){}
}
async function saveCanPins(){
  var tx=parseInt($('can-tx').value,10),rx=parseInt($('can-rx').value,10);
  if(isNaN(tx)||isNaN(rx)){$('can-pins-hint').textContent='请同时输入TX和RX';$('can-pins-hint').style.color='var(--err)';return;}
  if(!await dashConfirm('保存CAN引脚 TX='+tx+' RX='+rx+' 并重启？错误的引脚会禁用CAN。','保存CAN引脚','保存'))return;
  try{const r=await fetch('/can_pins',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:'tx='+tx+'&rx='+rx});
    const d=await r.json();
    if(d.ok){
      $('can-pins-hint').textContent='已保存。重启中...';$('can-pins-hint').style.color='var(--ok)';
      await fetch('/reboot',{method:'POST'});
      setTimeout(()=>location.reload(),8000);
    }else{
      $('can-pins-hint').textContent=d.error||'保存失败';$('can-pins-hint').style.color='var(--err)';
    }
  }catch(e){$('can-pins-hint').textContent='连接错误';$('can-pins-hint').style.color='var(--err)';}
}



// ── 插件编辑器 ────────────────────────────────────────────────
let peState={rules:[]};
function peGetMeta(){return{name:($('pe-name').value||'').trim(),version:($('pe-version').value||'1.0').trim(),author:($('pe-author').value||'').trim()};}
function peParseInt(s,def){if(typeof s==='number')return s;if(s===''||s==null)return def;s=String(s).trim();let n=s.toLowerCase().startsWith('0x')?parseInt(s,16):parseInt(s,10);return isNaN(n)?def:n;}
function peBusLabel(v){if(v===undefined||v===null||v===''||v===0)return'';if(Array.isArray(v))return v.join(',');if(typeof v==='number'){let out=[];if(v&1)out.push('CH');if(v&2)out.push('VEH');if(v&4)out.push('PARTY');return out.join(',');}return String(v);}
function peDefaultMuxMask(mux){return mux<0?0:(mux>7?255:7);}
function peSetStatus(msg,kind){const el=$('pe-status');el.textContent=msg;el.style.color=kind==='ok'?'var(--ok)':kind==='err'?'var(--err)':kind==='acc'?'var(--acc)':'var(--tx3)';}
function peSetTestStatus(msg,kind){const el=$('pe-test-status');el.textContent=msg;el.style.color=kind==='ok'?'var(--ok)':kind==='err'?'var(--err)':kind==='acc'?'var(--acc)':'var(--tx3)';}
function peHasContent(){const meta=peGetMeta();return !!(meta.name||meta.author||meta.version!=='1.0'||peState.rules.length);}
function peRuleLabel(r,i){return 'Rule '+(i+1)+' · CAN 0x'+toHex((r.id||0)&0x7FF,3)+(r.mux>=0?' · mux '+r.mux:'')+(peBusLabel(r.bus)?' · '+peBusLabel(r.bus):'')+(pluginRuleMatchMask(r)?' · '+fmtRuleMatch(r):'');}
function peParseShortcutLine(line){
  const raw=(line||'').trim();
  if(!raw)return{error:'需要快捷行'};
  const m=raw.match(/^\s*(0x[0-9a-fA-F]+|\d+)(?:\s+mux\s*=\s*(-?\d+))?\s+byte\[(\d+)\]\s*=\s*(0x[0-9a-fA-F]+|\d+)(?:\s+mask\s*=\s*(0x[0-9a-fA-F]+|\d+))?(?:\s*\((.*)\))?\s*$/);
  if(!m)return{error:'使用格式如 0x7FF mux=2 byte[5] = 0x4C'};
  const canId=peParseInt(m[1],NaN),mux=m[2]===undefined?-1:peParseInt(m[2],NaN);
  const byte=peParseInt(m[3],NaN),val=peParseInt(m[4],NaN),mask=m[5]===undefined?255:peParseInt(m[5],NaN);
  if(isNaN(canId)||canId<1||canId>0x7FF)return{error:'CAN ID必须为1-0x7FF'};
  if(isNaN(mux)||mux<-1||mux>255)return{error:'mux必须为-1..255'};
  if(isNaN(byte)||byte<0||byte>7)return{error:'byte必须为0-7'};
  if(isNaN(val)||val<0||val>255)return{error:'value必须为0-255'};
  if(isNaN(mask)||mask<0||mask>255)return{error:'mask必须为0-255'};
  return{rule:{id:canId|0,mux:mux|0,mux_mask:peDefaultMuxMask(mux|0),bus:'',send:true,ops:[{type:'set_byte',byte:byte|0,val:val|0,mask:mask|0}]},note:raw};
}
function peUpdateRuleOptions(){
  const sel=$('pe-test-rule');if(!sel)return;
  const prev=parseInt(sel.value,10);
  if(!peState.rules.length){sel.disabled=true;sel.innerHTML='<option value="">无规则</option>';return;}
  sel.disabled=false;
  sel.innerHTML=peState.rules.map((r,i)=>'<option value="'+i+'">'+peRuleLabel(r,i)+'</option>').join('');
  sel.value=String(!isNaN(prev)&&prev>=0&&prev<peState.rules.length?prev:0);
}
function peAddRule(){if(peState.rules.length>=16){peSetStatus('每个插件最多16条规则','err');return;}peState.rules.push({id:0,mux:-1,mux_mask:0,bus:'',match_byte:0,match_mask:0,match_val:0,send:true,ops:[]});peRender();}
function peAddRuleFromShortcut(){
  if(peState.rules.length>=16){peSetStatus('每个插件最多16条规则','err');return;}
  const input=$('pe-shortcut');const parsed=peParseShortcutLine(input.value);
  if(parsed.error){peSetStatus(parsed.error,'err');return;}
  peState.rules.push(parsed.rule);
  input.value='';
  peRender();
  peSetStatus('快捷行已添加','ok');
}
function peRemoveRule(i){peState.rules.splice(i,1);peRender();}
function peAddOp(i,type){const r=peState.rules[i];if(!r)return;if(r.ops.length>=peMaxOps){peSetStatus('每条规则最多'+peMaxOps+'个操作','err');return;}
  const op={type:type};
  if(type==='set_bit'){op.bit=0;op.val=1;}
  else if(type==='set_byte'){op.byte=0;op.val=0;op.mask=255;}
  else if(type==='or_byte'){op.byte=0;op.val=0;}
  else if(type==='and_byte'){op.byte=0;op.val=255;}
  else if(type==='counter'){op.byte=0;op.mask=15;op.step=1;}
  else if(type==='emit_periodic'){op.interval=100;op.gtw_silent=false;}
  r.ops.push(op);peRender();}
function peRemoveOp(i,j){peState.rules[i].ops.splice(j,1);peRender();}
function peUpdateField(i,j,field,value){
  if(j<0){const r=peState.rules[i];if(!r)return;
    if(field==='id')r.id=peParseInt(value,0);
    else if(field==='mux'){const oldDefault=peDefaultMuxMask(r.mux);const oldMask=r.mux_mask||0;r.mux=value===''?-1:peParseInt(value,-1);if(!oldMask||oldMask===oldDefault)r.mux_mask=peDefaultMuxMask(r.mux);}
    else if(field==='mux_mask')r.mux_mask=Math.max(0,Math.min(255,peParseInt(value,peDefaultMuxMask(r.mux))));
    else if(field==='bus')r.bus=String(value||'').trim().toUpperCase();
    else if(field==='match_byte')r.match_byte=Math.max(0,Math.min(7,peParseInt(value,0)));
    else if(field==='match_mask')r.match_mask=Math.max(0,Math.min(255,peParseInt(value,0)));
    else if(field==='match_val')r.match_val=Math.max(0,Math.min(255,peParseInt(value,0)));
    else if(field==='send')r.send=!!value;
    peRender();return;
  }
  const op=peState.rules[i].ops[j];if(!op)return;
  if(field==='type'){const nt=value;Object.keys(op).forEach(k=>{if(k!=='type')delete op[k];});op.type=nt;
    if(nt==='set_bit'){op.bit=0;op.val=1;}
    else if(nt==='set_byte'){op.byte=0;op.val=0;op.mask=255;}
    else if(nt==='or_byte'){op.byte=0;op.val=0;}
    else if(nt==='and_byte'){op.byte=0;op.val=255;}
    else if(nt==='counter'){op.byte=0;op.mask=15;op.step=1;}
    else if(nt==='emit_periodic'){op.interval=100;op.gtw_silent=false;}
    peRender();return;
  }
  if(field==='bit')op.bit=Math.max(0,Math.min(63,peParseInt(value,0)));
  else if(field==='byte')op.byte=Math.max(0,Math.min(7,peParseInt(value,0)));
  else if(field==='val')op.val=Math.max(0,Math.min(op.type==='set_bit'?1:255,peParseInt(value,0)));
  else if(field==='mask')op.mask=Math.max(0,Math.min(255,peParseInt(value,255)));
  else if(field==='step')op.step=Math.max(1,Math.min(255,peParseInt(value,1)));
  else if(field==='interval')op.interval=Math.max(10,Math.min(5000,peParseInt(value,100)));
  else if(field==='gtw_silent')op.gtw_silent=!!value;
  peRenderPreview();peUpdateTestPreview();
}
function peOpRow(i,j,op){
  const sel='<select class="sniff-input" style="width:90px" onchange="peUpdateField('+i+','+j+',\'type\',this.value)">'+
    ['set_bit','set_byte','or_byte','and_byte','counter','emit_periodic','checksum'].map(t=>'<option value="'+t+'"'+(op.type===t?' selected':'')+'>'+t+'</option>').join('')+'</select>';
  let fields='';
  if(op.type==='set_bit'){
    fields='<input class="sniff-input" style="width:55px" type="number" min="0" max="63" value="'+op.bit+'" title="bit (0-63)" onchange="peUpdateField('+i+','+j+',\'bit\',this.value)">'+
      '<select class="sniff-input" style="width:80px" onchange="peUpdateField('+i+','+j+',\'val\',this.value)"><option value="1"'+(op.val?' selected':'')+'>set (1)</option><option value="0"'+(!op.val?' selected':'')+'>clear (0)</option></select>';
  }else if(op.type==='set_byte'){
    fields='<input class="sniff-input" style="width:48px" type="number" min="0" max="7" value="'+op.byte+'" title="byte (0-7)" onchange="peUpdateField('+i+','+j+',\'byte\',this.value)">'+
      '<input class="sniff-input" style="width:70px" value="0x'+((op.val||0)&255).toString(16)+'" title="val (0-255, hex or dec)" onchange="peUpdateField('+i+','+j+',\'val\',this.value)">'+
      '<input class="sniff-input" style="width:70px" value="0x'+(op.mask===undefined?255:op.mask).toString(16)+'" title="mask (0-255)" onchange="peUpdateField('+i+','+j+',\'mask\',this.value)">';
  }else if(op.type==='or_byte'||op.type==='and_byte'){
    fields='<input class="sniff-input" style="width:48px" type="number" min="0" max="7" value="'+op.byte+'" title="byte (0-7)" onchange="peUpdateField('+i+','+j+',\'byte\',this.value)">'+
      '<input class="sniff-input" style="width:70px" value="0x'+((op.val||0)&255).toString(16)+'" title="val (0-255)" onchange="peUpdateField('+i+','+j+',\'val\',this.value)">';
  }else if(op.type==='counter'){
    fields='<input class="sniff-input" style="width:48px" type="number" min="0" max="7" value="'+op.byte+'" title="byte (0-7)" onchange="peUpdateField('+i+','+j+',\'byte\',this.value)">'+
      '<input class="sniff-input" style="width:70px" value="0x'+((op.mask===undefined?15:op.mask)&255).toString(16)+'" title="counter mask (contiguous bits)" onchange="peUpdateField('+i+','+j+',\'mask\',this.value)">'+
      '<input class="sniff-input" style="width:58px" type="number" min="1" max="255" value="'+(op.step||1)+'" title="step" onchange="peUpdateField('+i+','+j+',\'step\',this.value)">';
  }else if(op.type==='emit_periodic'){
    fields='<input class="sniff-input" style="width:72px" type="number" min="10" max="5000" value="'+(op.interval||100)+'" title="interval ms" onchange="peUpdateField('+i+','+j+',\'interval\',this.value)">'+
      '<label style="font-size:11px;color:var(--tx3);display:flex;align-items:center;gap:4px"><input type="checkbox"'+(op.gtw_silent?' checked':'')+' onchange="peUpdateField('+i+','+j+',\'gtw_silent\',this.checked)"> GTW silent</label>';
  }else{
    fields='<span style="font-size:11px;color:var(--tx3);align-self:center;padding:0 4px">重新计算字节7校验和</span>';
  }
  return '<div style="display:flex;gap:4px;align-items:center;margin-bottom:4px;flex-wrap:wrap">'+sel+fields+'<button class="sniff-btn" style="margin-left:auto;padding:2px 8px" onclick="peRemoveOp('+i+','+j+')" title="移除操作">&times;</button></div>';
}
function peRuleBlock(i,r){
  const ops=r.ops.length?r.ops.map((op,j)=>peOpRow(i,j,op)).join(''):'<div style="font-size:11px;color:var(--tx3);padding:4px 0">无操作 &mdash; 在下方添加</div>';
  const hex=r.id?'0x'+r.id.toString(16).toUpperCase():'?';
  const muxMask=r.mux_mask===undefined?peDefaultMuxMask(r.mux):r.mux_mask;
  return '<details open style="margin-bottom:10px;border:1px solid var(--bd);border-radius:6px;padding:8px;background:var(--bg2)">'+
    '<summary style="cursor:pointer;font-size:12px;color:var(--tx);user-select:none">Rule '+(i+1)+' &mdash; CAN '+hex+(r.id?' ('+r.id+')':'')+(r.mux>=0?' mux='+r.mux:'')+(peBusLabel(r.bus)?' '+peBusLabel(r.bus):'')+' &middot; '+r.ops.length+' op'+(r.ops.length===1?'':'s')+'</summary>'+
    '<div style="display:flex;gap:6px;margin:8px 0;flex-wrap:wrap">'+
      '<input class="sniff-input" style="width:110px" value="'+(r.id?('0x'+(r.id|0).toString(16).toUpperCase()):'')+'" placeholder="CAN ID / 0x7FF" onchange="peUpdateField('+i+',-1,\'id\',this.value)">'+
      '<input class="sniff-input" style="width:100px" type="number" min="-1" max="255" value="'+r.mux+'" placeholder="mux (-1=任意)" onchange="peUpdateField('+i+',-1,\'mux\',this.value)">'+
      '<input class="sniff-input" style="width:82px" value="0x'+((muxMask||0)&255).toString(16)+'" placeholder="mux掩码" title="mux掩码，例如0x7, 0xf, 0xff" onchange="peUpdateField('+i+',-1,\'mux_mask\',this.value)">'+
      '<input class="sniff-input" style="width:96px" value="'+peBusLabel(r.bus)+'" placeholder="bus" title="CH, VEH, PARTY, or comma list" onchange="peUpdateField('+i+',-1,\'bus\',this.value)">'+
      '<input class="sniff-input" style="width:58px" type="number" min="0" max="7" value="'+(r.match_byte||0)+'" title="match byte index; mask 0 disables" onchange="peUpdateField('+i+',-1,\'match_byte\',this.value)">'+
      '<input class="sniff-input" style="width:76px" value="0x'+((r.match_mask||0)&255).toString(16)+'" placeholder="匹配掩码" title="匹配掩码；0=禁用" onchange="peUpdateField('+i+',-1,\'match_mask\',this.value)">'+
      '<input class="sniff-input" style="width:76px" value="0x'+((r.match_val||0)&255).toString(16)+'" placeholder="匹配值" title="匹配值" onchange="peUpdateField('+i+',-1,\'match_val\',this.value)">'+
      '<label style="font-size:11px;color:var(--tx3);display:flex;align-items:center;gap:4px"><input type="checkbox"'+(r.send?' checked':'')+' onchange="peUpdateField('+i+',-1,\'send\',this.checked)"> 发送</label>'+
      '<button class="sniff-btn" style="margin-left:auto" onclick="peRemoveRule('+i+')">移除规则</button>'+
    '</div>'+
    ops+
    '<div style="margin-top:6px;display:flex;gap:4px;flex-wrap:wrap">'+
      '<button class="sniff-btn" onclick="peAddOp('+i+',\'set_bit\')">+ set_bit</button>'+
      '<button class="sniff-btn" onclick="peAddOp('+i+',\'set_byte\')">+ set_byte</button>'+
      '<button class="sniff-btn" onclick="peAddOp('+i+',\'or_byte\')">+ or_byte</button>'+
      '<button class="sniff-btn" onclick="peAddOp('+i+',\'and_byte\')">+ and_byte</button>'+
      '<button class="sniff-btn" onclick="peAddOp('+i+',\'counter\')">+ counter</button>'+
      '<button class="sniff-btn" onclick="peAddOp('+i+',\'emit_periodic\')">+ emit_periodic</button>'+
      '<button class="sniff-btn" onclick="peAddOp('+i+',\'checksum\')">+ checksum</button>'+
    '</div>'+
  '</details>';
}
function peRender(){
  const el=$('pe-rules');
  if(!peState.rules.length){    el.innerHTML='<div style="font-size:12px;color:var(--tx3);text-align:center;padding:12px;border:1px dashed var(--bd);border-radius:6px">暂无规则。点击下方&ldquo;+ 添加规则&rdquo;。</div>';}
  else{el.innerHTML=peState.rules.map((r,i)=>peRuleBlock(i,r)).join('');}
  $('pe-count').textContent=peState.rules.length+' rule'+(peState.rules.length===1?'':'s');
  peUpdateRuleOptions();
  peRenderPreview();
  peUpdateTestPreview();
}
function peBuildObj(){
  const meta=peGetMeta();
  const obj={name:meta.name||'Untitled',version:meta.version||'1.0'};
  if(meta.author)obj.author=meta.author;
  obj.rules=peState.rules.map(r=>{
    const out={id:r.id|0};
    if(r.mux>=0)out.mux=r.mux|0;
    const muxMask=r.mux_mask===undefined?peDefaultMuxMask(r.mux):r.mux_mask;
    if(r.mux>=0&&muxMask&&muxMask!==peDefaultMuxMask(r.mux))out.mux_mask=muxMask|0;
    if(peBusLabel(r.bus))out.bus=peBusLabel(r.bus);
    if(pluginRuleMatchMask(r)){out.match_byte=(r.match_byte||0)|0;out.match_mask=(r.match_mask||0)&255;out.match_val=(r.match_val||0)&255;}
    if(r.send===false)out.send=false;
    out.ops=r.ops.map(op=>{
      const o={type:op.type};
      if(op.type==='set_bit'){o.bit=op.bit|0;o.val=op.val?1:0;}
      else if(op.type==='set_byte'){o.byte=op.byte|0;o.val=(op.val|0)&255;if(op.mask!==undefined&&op.mask!==255)o.mask=op.mask|0;}
      else if(op.type==='or_byte'||op.type==='and_byte'){o.byte=op.byte|0;o.val=(op.val|0)&255;}
      else if(op.type==='counter'){o.byte=op.byte|0;o.mask=(op.mask===undefined?15:op.mask)|0;o.step=op.step||1;}
      else if(op.type==='emit_periodic'){o.interval=op.interval||100;if(op.gtw_silent)o.gtw_silent=true;}
      return o;
    });
    return out;
  });
  return obj;
}
function peRenderPreview(){$('pe-preview').textContent=JSON.stringify(peBuildObj(),null,2);}
function peFormatBytes(bytes){return(bytes||[]).slice(0,8).map(b=>toHex((b||0)&255,2)).join(' ');}
function peUpdateTestPreview(){
  const el=$('pe-test-preview');if(!el)return;
  if(!peState.rules.length){el.textContent='添加规则以预览测试帧。';peSetTestStatus('空闲','');return;}
  const idx=parseInt($('pe-test-rule').value,10);
  if(isNaN(idx)||idx<0||idx>=peState.rules.length){el.textContent='选择要测试的规则。';return;}
  const count=peParseInt($('pe-test-count').value,1),interval=peParseInt($('pe-test-interval').value,100);
  if(isNaN(count)||count<1||count>200){el.textContent='次数必须为1-200。';return;}
  if(isNaN(interval)||interval<10||interval>5000){el.textContent='间隔必须为10-5000毫秒。';return;}
  const rule=peState.rules[idx];
  el.textContent='预览 '+peRuleLabel(rule,idx)+'\n等待下一个匹配的总线帧，然后应用此规则。\n注入 '+count+'x 每 '+interval+' ms';
}
function peStopTestPoll(){if(peTestPollTimer){clearInterval(peTestPollTimer);peTestPollTimer=null;}}
async function pePollTestStatus(){
  try{
    const r=await fetch('/plugin_test_status');const d=await r.json();
    if(d.waiting){
      $('pe-test-preview').textContent='等待 CAN 0x'+toHex((d.targetId||d.id||0)&0x7FF,3)+'\n下一个匹配的总线帧将被修改并注入。\n进度: 0/'+(d.total||0);
    }else if(d.id||d.data){
      $('pe-test-preview').textContent='测试 CAN 0x'+toHex((d.id||0)&0x7FF,3)+'\n帧: '+peFormatBytes(d.data||[])+(d.total?'\n进度: '+d.sent+'/'+d.total:'');
    }
    if(d.active){
      peSetTestStatus(d.waiting?('等待 CAN 0x'+toHex((d.targetId||d.id||0)&0x7FF,3)):('运行中 '+d.sent+'/'+d.total+' · 每 '+d.interval+' ms'),'acc');
    }else{
      peSetTestStatus(d.total?(d.sent<d.total?'已停止 '+d.sent+'/'+d.total:'完成 '+d.sent+'/'+d.total):'空闲',d.total&&d.sent>=d.total?'ok':'');
      peStopTestPoll();
    }
  }catch(e){}
}
async function peLoadInstalledPlugin(idx){
  const p=installedPlugins[idx];if(!p)return;
  if(peHasContent()&&!await dashConfirm('将已安装的插件加载到编辑器？当前编辑器内容将被替换。','加载插件','加载'))return;
  $('pe-name').value=p.name||'';$('pe-author').value=p.author||'';$('pe-version').value=p.version||'1.0';
  peState={rules:(p.details||[]).map(r=>({id:r.id|0,mux:typeof r.mux==='number'?r.mux:-1,mux_mask:typeof r.mux_mask==='number'?r.mux_mask:peDefaultMuxMask(typeof r.mux==='number'?r.mux:-1),bus:peBusLabel(r.bus),match_byte:Math.max(0,Math.min(7,(typeof r.match_byte==='number'?r.match_byte:0)|0)),match_mask:((typeof r.match_mask==='number'?r.match_mask:0)|0)&255,match_val:((typeof r.match_val==='number'?r.match_val:0)|0)&255,send:r.send!==false,ops:(r.ops||[]).map(op=>{const out={type:op.type};if(op.type==='set_bit'){out.bit=op.bit|0;out.val=op.val?1:0;}else if(op.type==='set_byte'){out.byte=op.byte|0;out.val=(op.val|0)&255;out.mask=((typeof op.mask==='number'?op.mask:255)|0)&255;}else if(op.type==='or_byte'||op.type==='and_byte'){out.byte=op.byte|0;out.val=(op.val|0)&255;}else if(op.type==='counter'){out.byte=op.byte|0;out.mask=((typeof op.mask==='number'?op.mask:15)|0)&255;out.step=Math.max(1,((typeof op.step==='number'?op.step:1)|0)&255);}else if(op.type==='emit_periodic'){out.interval=Math.max(10,Math.min(5000,(typeof op.interval==='number'?op.interval:100)|0));out.gtw_silent=!!op.gtw_silent;}return out;})}))};
  peLoadedPluginName=p.name||'';peStopTestPoll();peSetTestStatus('空闲','');peRender();peSetStatus('已加载 "'+p.name+'" 到编辑器','ok');
  $('pe-name').scrollIntoView({behavior:'smooth',block:'center'});
}
function peValidate(){
  const meta=peGetMeta();
  if(!meta.name)return '需要插件名称';
  if(meta.name.length>31)return '名称太长（最多31个字符）';
  if(!peState.rules.length)return '至少添加一个规则';
  for(let i=0;i<peState.rules.length;i++){
    const r=peState.rules[i];
    if(!r.id||r.id<1||r.id>2047)return '规则 '+(i+1)+': CAN ID必须为1-2047';
    if(r.mux<-1||r.mux>255)return '规则 '+(i+1)+': mux必须为-1..255';
    if(r.mux>=0&&(r.mux_mask<1||r.mux_mask>255))return '规则 '+(i+1)+': mux掩码必须为1-255';
    if(r.match_byte<0||r.match_byte>7)return '规则 '+(i+1)+': 匹配字节必须为0-7';
    if(r.match_mask<0||r.match_mask>255)return '规则 '+(i+1)+': 匹配掩码必须为0-255';
    if(r.match_val<0||r.match_val>255)return '规则 '+(i+1)+': 匹配值必须为0-255';
    if(!r.ops.length)return '规则 '+(i+1)+': 至少添加一个操作';
    for(let j=0;j<r.ops.length;j++){
      const op=r.ops[j];
      if(op.type==='set_bit'){if(op.bit<0||op.bit>63)return '规则 '+(i+1)+' 操作 '+(j+1)+': bit必须为0-63';}
      else if(op.type==='set_byte'||op.type==='or_byte'||op.type==='and_byte'){
        if(op.byte<0||op.byte>7)return '规则 '+(i+1)+' 操作 '+(j+1)+': byte必须为0-7';
        if(op.val<0||op.val>255)return '规则 '+(i+1)+' 操作 '+(j+1)+': val必须为0-255';
      }else if(op.type==='counter'){
        if(op.byte<0||op.byte>7)return '规则 '+(i+1)+' 操作 '+(j+1)+': byte必须为0-7';
        if(op.mask<1||op.mask>255)return '规则 '+(i+1)+' 操作 '+(j+1)+': mask必须为1-255';
        if(op.step<1||op.step>255)return '规则 '+(i+1)+' 操作 '+(j+1)+': step必须为1-255';
      }else if(op.type==='emit_periodic'){
        if(r.id!==2047||r.mux!==3)return '规则 '+(i+1)+' 操作 '+(j+1)+': emit_periodic需要CAN 0x7FF mux 3';
        if(r.send===false)return '规则 '+(i+1)+' 操作 '+(j+1)+': emit_periodic需要启用发送';
        if(op.interval<10||op.interval>5000)return '规则 '+(i+1)+' 操作 '+(j+1)+': 间隔必须为10-5000毫秒';
      }else if(op.type!=='checksum'){
        return '规则 '+(i+1)+' 操作 '+(j+1)+': 不支持的操作';
      }
    }
  }
  return null;
}
async function peInstall(){
  const err=peValidate();
  if(err){peSetStatus(err,'err');return;}
  const obj=peBuildObj();
  try{const r=await fetch('/plugins');const d=await r.json();
    if(d.plugins&&d.plugins.some(p=>p.name===obj.name)&&obj.name!==peLoadedPluginName){
      if(!await dashConfirm('名为 "'+obj.name+'" 的插件已存在。覆盖？','覆盖插件','覆盖'))return;
    }
  }catch(e){}
  const beforeSig=pluginStateSignature(installedPlugins);
  peSetStatus('安装中...','acc');
  try{
    await fetchJsonWithTimeout('/plugin_upload',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify(obj)},5000);
    peLoadedPluginName=obj.name;
    try{await refreshPluginsNow();}catch(e){await refreshPluginsAfterAction(beforeSig);}
    peSetStatus('已安装','ok');
  }catch(e){
    if(await refreshPluginsAfterAction(beforeSig)){
      peLoadedPluginName=obj.name;
      peSetStatus('已安装','ok');
    }else{peSetStatus(actionErrorMessage(e,'连接错误'),'err');}
  }
}
async function peStartTest(){
  if(!peState.rules.length){peSetTestStatus('请先添加规则','err');return;}
  const idx=parseInt($('pe-test-rule').value,10);
  if(isNaN(idx)||idx<0||idx>=peState.rules.length){peSetTestStatus('选择有效的规则','err');return;}
  const count=peParseInt($('pe-test-count').value,1),interval=peParseInt($('pe-test-interval').value,100);
  if(isNaN(count)||count<1||count>200){peSetTestStatus('次数必须为1-200','err');return;}
  if(isNaN(interval)||interval<10||interval>5000){peSetTestStatus('间隔必须为10-5000毫秒','err');return;}
  peSetTestStatus('启动中...','acc');
  try{
    const r=await fetch('/plugin_test',{method:'POST',headers:{'Content-Type':'application/json'},body:JSON.stringify({plugin:peBuildObj(),rule:idx,count:count,interval:interval})});
    const d=await r.json();
    if(d.ok){
      $('pe-test-preview').textContent='等待 CAN 0x'+toHex((d.targetId||d.id||0)&0x7FF,3)+'\n下一个匹配的总线帧将被修改并注入。\n进度: 0/'+(d.total||0);
      peSetTestStatus(d.active?('等待 CAN 0x'+toHex((d.targetId||d.id||0)&0x7FF,3)):'完成','acc');
      peStopTestPoll();peTestPollTimer=setInterval(pePollTestStatus,500);pePollTestStatus();
    }else{
      peSetTestStatus(d.error||'测试失败','err');
    }
  }catch(e){peSetTestStatus('连接错误','err');}
}
async function peStopTest(){
  try{
    const r=await fetch('/plugin_test_stop',{method:'POST'});const d=await r.json();
    peStopTestPoll();
    peSetTestStatus(d.total?(d.sent<d.total?'已停止 '+d.sent+'/'+d.total:'完成 '+d.sent+'/'+d.total):'空闲',d.total&&d.sent>=d.total?'ok':'');
  }catch(e){peSetTestStatus('Connection error','err');}
}
function peDownload(){
  const err=peValidate();
  if(err){peSetStatus(err,'err');return;}
  const obj=peBuildObj();
  const blob=new Blob([JSON.stringify(obj,null,2)],{type:'application/json'});
  const url=URL.createObjectURL(blob);
  const a=document.createElement('a');a.href=url;a.download=(obj.name||'plugin').replace(/[^A-Za-z0-9_-]/g,'_').toLowerCase()+'.json';document.body.appendChild(a);a.click();document.body.removeChild(a);URL.revokeObjectURL(url);
  peSetStatus('已下载','ok');
}
async function peReset(){
  if(peState.rules.length&&!await dashConfirm('放弃当前编辑器内容？','放弃更改','放弃'))return;
  peState={rules:[]};peLoadedPluginName='';peStopTestPoll();
  $('pe-name').value='';$('pe-author').value='';$('pe-version').value='1.0';
  peRender();peSetStatus('','');peSetTestStatus('空闲','');
}


let espNowState={scanning:false,discovered:[],paired:null};

async function pollEspNow(){
  return runPoll('espnow',async()=>{
    if(!dashboardStatusOk)return;
    try{
      const d=await fetchPollJson('/espnow_status',3000);
      espNowState.scanning=d.scanning;
      espNowState.discovered=d.discovered||[];
      espNowState.paired=d.paired||null;
      renderEspNow();
      updateEspNowMeta();
    }catch(e){}
  });
}

function renderEspNow(){
  const el=$('espnow-devices');
  if(!el)return;
  if(!espNowState.scanning&&!espNowState.discovered.length){
    el.innerHTML='<div style="font-size:12px;color:var(--tx3);text-align:center;padding:12px">点击"开始扫描"查找附近的接收端设备</div>';
    return;
  }
  if(!espNowState.discovered.length){
    el.innerHTML='<div style="font-size:12px;color:var(--acc);text-align:center;padding:12px">扫描中... 等待接收端设备发送暗号</div>';
    return;
  }
  el.innerHTML=espNowState.discovered.map(d=>{
    return '<div style="display:flex;align-items:center;gap:6px;padding:6px 0;border-bottom:1px solid var(--bd);font-size:12px">'+
      '<div style="flex:1;min-width:0">'+
      '<div>'+d.mac+' <span style="color:var(--tx3);font-size:10px">'+(d.rssi||0)+' dBm</span></div>'+
      '</div>'+
      (d.paired?'<span style="color:var(--ok);font-size:11px">已连接</span>':
        '<button class="sniff-btn" onclick="pairEspNow(''+d.mac+'')" style="padding:4px 8px;font-size:11px">连接</button>')+
      '</div>';
  }).join('');
}

function updateEspNowMeta(){
  const paired=espNowState.paired;
  const pairedEl=$('espnow-paired');
  if(paired){
    pairedEl.style.display='block';
    $('espnow-paired-name').textContent='已连接: '+(paired.name||'接收端');
    $('espnow-paired-mac').textContent=paired.mac||'';
    $('espnow-status').textContent='已连接';
    $('espnow-status').style.color='var(--ok)';
  }else{
    pairedEl.style.display='none';
    $('espnow-status').textContent=espNowState.scanning?'扫描中...':'未连接';
    $('espnow-status').style.color=espNowState.scanning?'var(--acc)':'var(--tx3)';
  }
  $('espnow-scan-meta').textContent=espNowState.discovered.length+' 设备';
  const btn=$('espnow-scan-btn');
  if(btn){
    btn.textContent=espNowState.scanning?'停止扫描':'开始扫描';
    btn.classList.toggle('paused',espNowState.scanning);
  }
}

async function toggleEspNowScan(){
  const action=espNowState.scanning?'stop':'start';
  try{
    await fetch('/espnow_scan',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:'action='+action});
    espNowState.scanning=!espNowState.scanning;
    const st=$('espnow-scan-status');
    if(st){st.textContent=espNowState.scanning?'扫描中...':'';st.style.color='var(--acc)';}
    renderEspNow();
    updateEspNowMeta();
  }catch(e){
    const st=$('espnow-scan-status');
    if(st){st.textContent='操作失败';st.style.color='var(--err)';}
  }
}

async function pairEspNow(mac){
  try{
    const r=await fetch('/espnow_pair',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:'mac='+encodeURIComponent(mac)});
    const d=await r.json();
    if(d.ok) {
      $('espnow-scan-status').textContent='已连接!';
      $('espnow-scan-status').style.color='var(--ok)';
      pollEspNow();
    } else {
      $('espnow-scan-status').textContent=d.error||'连接失败';
      $('espnow-scan-status').style.color='var(--err)';
    }
  }catch(e){
    $('espnow-scan-status').textContent='连接错误';
    $('espnow-scan-status').style.color='var(--err)';
  }
}

async function unpairEspNow(){
  try{
    await fetch('/espnow_unpair',{method:'POST'});
    $('espnow-scan-status').textContent='已断开';
    $('espnow-scan-status').style.color='var(--ok)';
    pollEspNow();
  }catch(e){}
}

dashboardPollTimers.push(setInterval(poll,2000));dashboardPollTimers.push(setInterval(pollLog,5000));dashboardPollTimers.push(setInterval(pollSniffer,1000));dashboardPollTimers.push(setInterval(pollPlugins,10000));dashboardPollTimers.push(setInterval(loadWifiStatus,10000));dashboardPollTimers.push(setInterval(loadApStatus,10000));dashboardPollTimers.push(setInterval(loadWifiNetworks,30000));dashboardPollTimers.push(setInterval(pollEspNow,3000));
initCardMinimizers();initSubsectionMinimizers();updateSniffIdToggle();poll();pollRec();peRender();
</script>
</body>
</html>
)HTML";
#endif
