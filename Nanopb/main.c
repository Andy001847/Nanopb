#include "main.h"
#include "pb_data_handle.h"
#include "pb_message.pb.h"

static void device_information_test(void)
{
	pb_byte_t pb_buffer[PB_BUFFER_SIZE] = { 0 };

	device_information_t pack_info = device_information_init_zero;

	pack_info.device_type = 0x04;
	memcpy(pack_info.device_version, DEVICE_VERSION, strlen(DEVICE_VERSION));
	pack_info.device_state = DEVICE_WORK_STATE_RUNNING;
	memcpy(pack_info.algorithm_version, ALG_VERSION, strlen(ALG_VERSION));

	memset(pb_buffer, 0, PB_BUFFER_SIZE);
	int pack_length = pb_pack_data(pb_buffer, device_information_fields, &pack_info);
	if (pack_length < 0)
	{
		Log(1, "Pack data failed.");
	}

	device_information_t unpack_info = device_information_init_zero;

	int unpack_result = pb_unpack_data(pb_buffer, pack_length, device_information_fields, &unpack_info);
	if (unpack_result < 0)
	{
		Log(1, "Unpack data failed.");
	}

	Log(1, "Device type: 0x%02X.", unpack_info.device_type);
	Log(1, "Device version: %s.", unpack_info.device_version);
	Log(1, "Device work state: %d.", unpack_info.device_state);
	Log(1, "Device algorithm version: %s.", unpack_info.algorithm_version);

	Log(1, ">>> pack data length = %d.\n", pack_length);
}

static void mqtt_server_configure_test(void)
{
	pb_byte_t pb_buffer[PB_BUFFER_SIZE] = { 0 };

	mqtt_server_configure_t pack_config = mqtt_server_configure_init_zero;

	pack_config.domain_name_flag = true;
	pack_config.address.arg = MQTT_SERVER;
	pack_config.address.funcs.encode = pb_encode_callback;
	pack_config.port = MQTT_PORT;
	memcpy(pack_config.username, MQTT_USERNAME, strlen(MQTT_USERNAME));
	memcpy(pack_config.password, MQTT_PASSWORD, strlen(MQTT_PASSWORD));

	memset(pb_buffer, 0, PB_BUFFER_SIZE);
	int pack_length = pb_pack_data(pb_buffer, mqtt_server_configure_fields, &pack_config);
	if (pack_length < 0)
	{
		Log(1, "Pack data failed.");
	}

	mqtt_server_configure_t unpack_config = mqtt_server_configure_init_zero;
	char mqtt_server_buffer[64] = { 0 };

	unpack_config.address.arg = mqtt_server_buffer;
	unpack_config.address.funcs.decode = pb_decode_callback;

	int unpack_result = pb_unpack_data(pb_buffer, pack_length, mqtt_server_configure_fields, &unpack_config);
	if (unpack_result < 0)
	{
		Log(1, "Unpack data failed.");
	}

	Log(1, "MQTT server address type: %s.", unpack_config.domain_name_flag ? "domain name" : "IP address");
	Log(1, "MQTT server address: %s.", (char*)unpack_config.address.arg);
	Log(1, "MQTT port: %d.", unpack_config.port);
	Log(1, "MQTT username: %s.", unpack_config.username);
	Log(1, "MQTT password: %s.", unpack_config.password);

	Log(1, ">>> pack data length = %d.\n", pack_length);
}

static void device_upgrade_test(void)
{
	pb_byte_t pb_buffer[PB_BUFFER_SIZE] = { 0 };

	device_upgrade_t pack_upgrade_url = device_upgrade_init_zero;
	pack_upgrade_url.oad_address.arg = UPGRADE_URL;
	pack_upgrade_url.oad_address.funcs.encode = pb_encode_callback;

	memset(pb_buffer, 0, PB_BUFFER_SIZE);
	int pack_length = pb_pack_data(pb_buffer, device_upgrade_fields, &pack_upgrade_url);
	if (pack_length < 0)
	{
		Log(1, "Pack data failed.");
	}

	device_upgrade_t unpack_upgrade_url = device_upgrade_init_zero;
	char unpack_upgrade_address[128] = { 0 };

	unpack_upgrade_url.oad_address.arg = unpack_upgrade_address;
	unpack_upgrade_url.oad_address.funcs.decode = pb_decode_callback;

	int unpack_result = pb_unpack_data(pb_buffer, pack_length, device_upgrade_fields, &unpack_upgrade_url);
	if (unpack_result < 0)
	{
		Log(1, "Unpack data failed.");
	}

	Log(1, "Upgrade address: %s.", (char*)unpack_upgrade_url.oad_address.arg);

	Log(1, ">>> pack data length = %d.\n", pack_length);
}

static void device_data_test(void)
{
	pb_byte_t pb_buffer[PB_BUFFER_SIZE] = { 0 };

	device_data_t pack_data = device_data_init_zero;

	float pack_raw_temperature[3] = {32.15f, 32.46f, 32.69f};
	float pack_alg_temperature[3] = {32.16f, 32.47f, 32.74f};

	memcpy(pack_data.ble_mac, BLE_MAC, strlen(BLE_MAC));
	pack_data.ble_packet_number = 65;
	pack_data.ble_charging_state = false;
	pack_data.ble_battery_level = 80;
	pack_data.raw_temperature_count = 3;
	memcpy(pack_data.raw_temperature, pack_raw_temperature, 3*sizeof(float));
	pack_data.algorithm_temperature_count = 3;
	memcpy(pack_data.algorithm_temperature, pack_alg_temperature, 3 * sizeof(float));
	pack_data.algorithm_state = 2;
	pack_data.algorithm_gesture = 1;
	pack_data.algorithm_predict_state = true;
	pack_data.ble_rssi = -67;
	pack_data.wifi_rssi = -58;
	memcpy(pack_data.current_time, CURRENT_TIME, strlen(CURRENT_TIME));

	memset(pb_buffer, 0, PB_BUFFER_SIZE);
	int pack_length = pb_pack_data(pb_buffer, device_data_fields, &pack_data);
	if (pack_length < 0)
	{
		Log(1, "Pack data failed.");
	}

	device_data_t unpack_data = device_data_init_zero;

	int unpack_result = pb_unpack_data(pb_buffer, pack_length, device_data_fields, &unpack_data);
	if (unpack_result < 0)
	{
		Log(1, "Unpack data failed.");
	}

	Log(1, "BLE mac: %s.", unpack_data.ble_mac);
	Log(1, "Packet number: %d.", unpack_data.ble_packet_number);
	Log(1, "Charging state: %s.", unpack_data.algorithm_state ? "charging" : "discharging");
	Log(1, "Battery level: %d%%.", unpack_data.ble_battery_level);
	Log(1, "Raw temperature: %5.2f¡æ, %5.2f¡æ, %5.2f¡æ.", unpack_data.raw_temperature[0], unpack_data.raw_temperature[1], unpack_data.raw_temperature[2]);
	Log(1, "Algorithm temperature: %5.2f¡æ, %5.2f¡æ, %5.2f¡æ.", unpack_data.algorithm_temperature[0], unpack_data.algorithm_temperature[1], unpack_data.algorithm_temperature[2]);
	Log(1, "Algorithm state: %d.", unpack_data.algorithm_state);
	Log(1, "Algorithm gesture: %d.", unpack_data.algorithm_gesture);
	Log(1, "Algorithm predict state: %s.", unpack_data.algorithm_predict_state ? "success" : "failed");
	Log(1, "BLE rssi: %d.", unpack_data.ble_rssi);
	Log(1, "WiFi rssi: %d.", unpack_data.wifi_rssi);
	Log(1, "Current time: %s.", unpack_data.current_time);

	Log(1, ">>> pack data length = %d.\n", pack_length);
}

static void ble_state_test(void)
{
	pb_byte_t pb_buffer[PB_BUFFER_SIZE] = { 0 };

	ble_state_t pack_state = ble_state_init_zero;

	memcpy(pack_state.ble_mac, BLE_MAC, strlen(BLE_MAC));
	memcpy(pack_state.ble_device_version, BLE_DEVICE_VERSION, strlen(BLE_DEVICE_VERSION));
	pack_state.ble_device_type = 0x0201;
	pack_state.ble_online_state = true;

	memset(pb_buffer, 0, PB_BUFFER_SIZE);
	int pack_length = pb_pack_data(pb_buffer, ble_state_fields, &pack_state);
	if (pack_length < -1)
	{
		Log(1, "Pack data failed.");
	}

	ble_state_t unpack_state = ble_state_init_zero;

	int unpack_result = pb_unpack_data(pb_buffer, pack_length, ble_state_fields, &unpack_state);
	if (unpack_result < 0)
	{
		Log(1, "Unpack data failed.");
	}

	Log(1, "BLE mac: %s.", unpack_state.ble_mac);
	Log(1, "BLE device version: %s.", unpack_state.ble_device_version);
	Log(1, "BLE device type: 0x%04X.", unpack_state.ble_device_type);
	Log(1, "BLE device online state: %s.", unpack_state.ble_online_state ? "online" : "offline");

	Log(1, ">>> pack data length = %d.\n", pack_length);
}

static void device_state_test(void)
{
	pb_byte_t pb_buffer[PB_BUFFER_SIZE] = { 0 };

	device_state_t pack_device_state = device_state_init_zero;

	pack_device_state.online_state = true;

	memset(pb_buffer, 0, PB_BUFFER_SIZE);
	int pack_length = pb_pack_data(pb_buffer, device_state_fields, &pack_device_state);
	if (pack_length < -1)
	{
		Log(1, "Pack data failed.");
	}

	device_state_t unpack_device_state = device_state_init_zero;
	int unpack_result = pb_unpack_data(pb_buffer, pack_length, device_state_fields, &unpack_device_state);
	if (unpack_result < 0)
	{
		Log(1, "Unpack data failed.");
	}

	Log(1, "Device state: %s.", unpack_device_state.online_state ? "online" : "offline");

	Log(1, ">>> pack data length = %d.\n", pack_length);
}

int main(void)
{
	device_information_test();
	mqtt_server_configure_test();
	device_upgrade_test();
	device_data_test();
	ble_state_test();
	device_state_test();

	return 0;
}