#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <DeviceIo/DeviceIo.h>
#include <DeviceIo/RkBtBase.h>
#include <DeviceIo/RkBtSink.h>
#include <DeviceIo/RkBtSource.h>
#include <DeviceIo/RkBle.h>
#include <DeviceIo/RkBtSpp.h>

#include "bt_test.h"

/* Immediate wifi Service UUID */
#define BLE_UUID_SERVICE	"0000180A-0000-1000-8000-00805F9B34FB"
#define BLE_UUID_WIFI_CHAR	"00009999-0000-1000-8000-00805F9B34FB"
#define BLE_UUID_PROXIMITY	"7B931104-1810-4CBC-94DA-875C8067F845"
#define BLE_UUID_SEND		"dfd4416e-1810-47f7-8248-eb8be3dc47f9"
#define BLE_UUID_RECV		"9884d812-1810-4a24-94d3-b2c11a851fac"

static void bt_test_ble_recv_data_callback(const char *uuid, char *data, int len);
static void bt_test_ble_request_data_callback(const char *uuid);

/* Must be initialized before using Bluetooth ble */
static RkBtContent bt_content;

/******************************************/
/*        BT base server init             */
/******************************************/
/*
 * The Bluetooth basic service is turned on and the function
 * must be called before using the Bluetooth function.
 */
void bt_test_init_open(void *data)
{
	printf("---------------BT INIT OPEN----------------\n");
	bt_content.bt_name = "ROCKCHIP_AUDIO";
	bt_content.ble_content.ble_name = "ROCKCHIP_AUDIO BLE";
	bt_content.ble_content.server_uuid.uuid = BLE_UUID_SERVICE;
	bt_content.ble_content.server_uuid.len = UUID_128;
	bt_content.ble_content.chr_uuid[0].uuid = BLE_UUID_WIFI_CHAR;
	bt_content.ble_content.chr_uuid[0].len = UUID_128;
	bt_content.ble_content.chr_uuid[1].uuid = BLE_UUID_SEND;
	bt_content.ble_content.chr_uuid[1].len = UUID_128;
	bt_content.ble_content.chr_uuid[2].uuid = BLE_UUID_RECV;
	bt_content.ble_content.chr_uuid[2].len = UUID_128;
	bt_content.ble_content.chr_cnt = 3;
	bt_content.ble_content.advDataType = BLE_ADVDATA_TYPE_SYSTEM;
	bt_content.ble_content.cb_ble_recv_fun = bt_test_ble_recv_data_callback;
	bt_content.ble_content.cb_ble_request_data = bt_test_ble_request_data_callback;

	rk_bt_init(&bt_content);
}

/******************************************/
/*               A2DP SINK                */
/******************************************/
int bt_sink_callback(RK_BT_SINK_STATE state)
{
	switch(state) {
		case RK_BT_SINK_STATE_IDLE:
			printf("++++++++++++ BT SINK EVENT: idle ++++++++++\n");
			break;
		case RK_BT_SINK_STATE_CONNECT:
			printf("++++++++++++ BT SINK EVENT: connect sucess ++++++++++\n");
			break;
		case RK_BT_SINK_STATE_PLAY:
			printf("++++++++++++ BT SINK EVENT: playing ++++++++++\n");
			break;
		case RK_BT_SINK_STATE_PAUSE:
			printf("++++++++++++ BT SINK EVENT: paused ++++++++++\n");
			break;
		case RK_BT_SINK_STATE_STOP:
			printf("++++++++++++ BT SINK EVENT: stoped ++++++++++\n");
			break;
		case RK_BT_SINK_STATE_DISCONNECT:
			printf("++++++++++++ BT SINK EVENT: disconnected ++++++++++\n");
			break;
	}

    return 0;
}

void bt_test_sink_open(void *data)
{
	rk_bt_sink_register_callback(bt_sink_callback);
	rk_bt_sink_open();
}

void bt_test_sink_visibility00(void *data)
{
	rk_bt_sink_set_visibility(0, 0);
}

void bt_test_sink_visibility01(void *data)
{
	rk_bt_sink_set_visibility(0, 1);
}

void bt_test_sink_visibility10(void *data)
{
	rk_bt_sink_set_visibility(1, 0);
}

void bt_test_sink_visibility11(void *data)
{
	rk_bt_sink_set_visibility(1, 1);
}

void bt_test_sink_status(void *data)
{
	RK_BT_SINK_STATE pState;

	rk_bt_sink_get_state(&pState);
	switch(pState) {
		case RK_BT_SINK_STATE_IDLE:
			printf("++++++++++++ BT MASTER EVENT: idle ++++++++++\n");
			break;
		case RK_BT_SINK_STATE_CONNECT:
			printf("++++++++++++ BT MASTER EVENT: connect sucess ++++++++++\n");
			break;
		case RK_BT_SINK_STATE_PLAY:
			printf("++++++++++++ BT MASTER EVENT: playing ++++++++++\n");
			break;
		case RK_BT_SINK_STATE_PAUSE:
			printf("++++++++++++ BT MASTER EVENT: paused ++++++++++\n");
			break;
		case RK_BT_SINK_STATE_STOP:
			printf("++++++++++++ BT MASTER EVENT: stoped ++++++++++\n");
			break;
		case RK_BT_SINK_STATE_DISCONNECT:
			printf("++++++++++++ BT MASTER EVENT: disconnected ++++++++++\n");
			break;
	}
}

void bt_test_sink_music_play(void *data)
{
	rk_bt_sink_play();
}

void bt_test_sink_music_pause(void *data)
{
	rk_bt_sink_pause();
}

void bt_test_sink_music_next(void *data)
{
	rk_bt_sink_next();
}

void bt_test_sink_music_previous(void *data)
{
	rk_bt_sink_prev();
}

void bt_test_sink_music_stop(void *data)
{
	rk_bt_sink_stop();
}

void bt_test_sink_reconnect_disenable(void *data)
{
	rk_bt_sink_set_auto_reconnect(0);
}

void bt_test_sink_reconnect_enable(void *data)
{
	rk_bt_sink_set_auto_reconnect(1);
}

void bt_test_sink_disconnect(void *data)
{
	rk_bt_sink_disconnect();
}

void bt_test_sink_close(void *data)
{
	rk_bt_sink_close();
}

/******************************************/
/*              A2DP SOURCE               */
/******************************************/
void bt_test_source_status_callback(void *userdata, const RK_BT_SOURCE_EVENT enEvent)
{
	char address[17], name[100];

	switch(enEvent)
	{
		case BT_SOURCE_EVENT_CONNECT_FAILED:
			printf("++++++++++++ BT SOURCE EVENT:connect failed ++++++++++\n");
			break;
		case BT_SOURCE_EVENT_CONNECTED:
			printf("++++++++++++ BT SOURCE EVENT:connect sucess ++++++++++\n");
			rk_bt_source_get_device_name(name, 100);
			rk_bt_source_get_device_addr(address, 17);
			printf("DeviceName:%s. Address:%s\n", name, address);
			break;
		case BT_SOURCE_EVENT_DISCONNECTED:
			printf("++++++++++++ BT SOURCE EVENT:disconnect ++++++++++\n");
			break;
	}
}

void bt_test_source_auto_start(void *data)
{
	rk_bt_source_auto_connect_start(NULL, bt_test_source_status_callback);
}

void bt_test_source_auto_stop(void *data)
{
	rk_bt_source_auto_connect_stop();
}

void bt_test_source_connect_status(void *data)
{
	RK_BT_SOURCE_STATUS status;
	char name[256], address[256];

	rk_bt_source_get_status(&status, name, 256, address, 256);
	if (status == BT_SOURCE_STATUS_CONNECTED) {
		printf("++++++++++++ BT SOURCE STATUS: connected ++++++++++++\n");
		printf("\t name:%s, address:%s\n", name, address);
	} else
		printf("++++++++++++ BT SOURCE STATUS: disconnected ++++++++++++\n");
}

/******************************************/
/*                  BLE                   */
/******************************************/
static void ble_status_callback_test(RK_BLE_STATE state)
{
	printf("%s: status: %d.\n", __func__, state);

	switch (state) {
		case RK_BLE_STATE_IDLE:
			printf("+++++ RK_BLE_STATE_IDLE +++++\n");
			break;
		case RK_BLE_STATE_CONNECT:
			printf("+++++ RK_BLE_STATE_CONNECT +++++\n");
			break;
		case RK_BLE_STATE_DISCONNECT:
			printf("+++++ RK_BLE_STATE_DISCONNECT +++++\n");
			break;
	}
}

static void bt_test_ble_recv_data_callback(const char *uuid, char *data, int len)
{
	char data_t[512];
	char reply_buf[512] = {"My name is rk3308"};

	printf("=== %s uuid: %s===\n", __func__, uuid);
	memcpy(data_t, data, len);
	for (int i = 0 ; i < len; i++) {
		printf("%02x ", data_t[i]);
	}
	printf("\n");

	if (strstr(data_t, "Hello RockChip") || strstr(data_t, "HelloRockChip") ||
		strstr(data_t, "HELLO ROCKCHIP") || strstr(data_t, "HELLOROCKCHIP") ||
		strstr(data_t, "hello rockchip") || strstr(data_t, "hellorockchip")) {
		printf("=== %s Reply:%s ===\n", __func__, reply_buf);
		rk_ble_write(uuid, reply_buf, 17);
	}
}

static void bt_test_ble_request_data_callback(const char *uuid)
{
	printf("=== %s uuid: %s===\n", __func__, uuid);
}

void bt_test_ble_start(void *data) {
	rk_ble_register_status_callback(ble_status_callback_test);
	rk_ble_register_recv_callback(bt_test_ble_recv_data_callback);
	rk_ble_start(&bt_content.ble_content);
}

void bt_test_ble_write(void *data) {
	char write_buf[134];
	int i = 0;

	/* Construct the content of the sent data */
	for (i = 0; i < 134; i++)
		write_buf[i] = '0' + i % 10;

	rk_ble_write(BLE_UUID_SEND, write_buf, 134);
}

void bt_test_ble_get_status(void *data)
{
	RK_BLE_STATE state;

	printf("RK_ble_status_test: ");
	rk_ble_get_state(&state);
	switch (state) {
		case RK_BLE_STATE_IDLE:
			printf("RK_BLE_STATE_IDLE.\n");
			break;
		case RK_BLE_STATE_CONNECT:
			printf("RK_BLE_STATE_CONNECT.\n");
			break;
		case RK_BLE_STATE_DISCONNECT:
			printf("RK_BLE_STATE_DISCONNECT.\n");
			break;
	}
}

void bt_test_ble_stop(void *data) {
	rk_ble_register_status_callback(NULL);
	rk_ble_register_recv_callback(NULL);
	rk_ble_stop();
}

/* SPP */
void _btspp_status_callback(RK_BT_SPP_STATE type)
{
	switch(type) {
		case RK_BT_SPP_STATE_IDLE:
			printf("+++++++ RK_BT_SPP_STATE_IDLE +++++\n");
			break;
		case RK_BT_SPP_STATE_CONNECT:
			printf("+++++++ RK_BT_SPP_EVENT_CONNECT +++++\n");
			break;
		case RK_BT_SPP_STATE_DISCONNECT:
			printf("+++++++ RK_BT_SPP_EVENT_DISCONNECT +++++\n");
			break;
		default:
			printf("+++++++ BT SPP NOT SUPPORT TYPE! +++++\n");
			break;
	}
}

void _btspp_recv_callback(char *data, int len)
{
	if (len) {
		printf("+++++++ RK BT SPP RECV DATA: +++++\n");
		printf("\tRECVED(%d):%s\n", len, data);
	}
}

void bt_test_spp_open(void *data)
{
	rk_bt_spp_open();
	rk_bt_spp_register_status_cb(_btspp_status_callback);
	rk_bt_spp_register_recv_cb(_btspp_recv_callback);
}

void bt_test_spp_write(void *data)
{
	int ret = 0;
	char buff[100] = {"This is a message from rk3308 board!"};

	ret = rk_bt_spp_write(buff, strlen(buff));
	if (ret != strlen(buff)) {
		printf("%s failed, ret<%d> != strlen(buff)<%d>\n",
				__func__, ret, strlen(buff));
	}
}

void bt_test_spp_close(void *data)
{
	rk_bt_sink_close();
}

void bt_test_spp_status(void *data)
{
	RK_BT_SPP_STATE status;

	rk_bt_spp_get_state(&status);
	switch(status) {
		case RK_BT_SPP_STATE_IDLE:
			printf("+++++++ RK_BT_SPP_STATE_IDLE +++++\n");
			break;
		case RK_BT_SPP_STATE_CONNECT:
			printf("+++++++ RK_BT_SPP_STATE_CONNECT +++++\n");
			break;
		case RK_BT_SPP_STATE_DISCONNECT:
			printf("+++++++ RK_BT_SPP_STATE_DISCONNECT +++++\n");
			break;
		default:
			printf("+++++++ BTSPP NO STATUS SUPPORT! +++++\n");
			break;
	}
}
