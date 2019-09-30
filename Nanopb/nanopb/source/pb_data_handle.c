#include "pb_data_handle.h"

/*
** @Brief: Pack data into protocol buffer
** @Parameters: 1) pb_buffer, protocol buffer that convert pack_info data from pb_buffer
                2) fields, user define fields in xxx.proto file
				3) pack_info, need to transfer data
** @Return: 1) -1, pack data failed.
            2) others, pack data length
**/
int pb_pack_data(pb_byte_t* pb_buffer, const pb_field_t* fields, const void* pack_info)
{
	pb_ostream_t encode_stream = pb_ostream_from_buffer(pb_buffer, PB_BUFFER_SIZE);

	bool pack_result = pb_encode(&encode_stream, fields, pack_info);
	if (!pack_result)
	{
		/* PB encode error */
		return -1;
	}

	return encode_stream.bytes_written;
}

/*
** @Breif: Unpack protocol buffer into data that ready to use
** @Parameters: 1) pb_buffer, protocol buffer that need to convert into use data
                2) pb_buffer_size, need to decode protocol buffer data size
                3) fields, user, define fields in xxx.proto file
				4) unpack_info, user data that ready to use
** @Return: 1) -1, Unpack data from protocol buffer failed
            2) 0, Unpack data from protocol buffer success
**/
int pb_unpack_data(const pb_byte_t* pb_buffer, size_t pb_buffer_size, const pb_field_t* fields, void* unpack_info)
{
	pb_istream_t decode_stream = pb_istream_from_buffer(pb_buffer, pb_buffer_size);

	bool unpack_result = pb_decode(&decode_stream, fields, unpack_info);
	if (!unpack_result)
	{
		/* PB decode error */
		return -1;
	}

	return 0;
}

/*
** Encode data callback function
**/
bool pb_encode_callback(pb_ostream_t* stream, const pb_field_t* fields, void* const* arg)
{
	bool check_result = pb_encode_tag_for_field(stream, fields);
	if (check_result)
	{
		return pb_encode_string(stream, (pb_byte_t*)(*arg), strlen((char*)(*arg)));
	}

	return false;
}

/*
** Decode data callback function
**/
bool pb_decode_callback(pb_istream_t* stream, const pb_field_t* fields, void** arg)
{
	bool read_result = pb_read(stream, (pb_byte_t*)(*arg), stream->bytes_left);
	if (read_result)
	{
		return true;
	}

	return false;
}

/*
** Example:
(1) UserInformation.proto
syntax = "proto3";
option optimize_for = LITE_RUNTIME;

enum UserStatus
{
	UNKNOWN = 0;
	IDLE = 1;
	BUSY = 2;
}

message UserInformation
{
	string name = 1;
	uint32 age = 2;
	string phone = 3;
	UserStatus stat = 4;
	string email = 5;
}

(2) Test code
int main(void)
{
	UserInformation pack_info =
	{
		.name.arg = "Andy",
		.name.funcs.encode = pb_encode_callback,
		.age = 100,
		.phone.arg = "1234567",
		.phone.funcs.encode = pb_encode_callback,
		.stat = UserStatus_IDLE,
		.email.arg = "1234567@hotmail.com",
		.email.funcs.encode = string_encode,
	};

	pb_byte_t pb_buffer[PB_BUFFER_SIZE] = { 0 };

	int lenght = pack_pb_data(pb_buffer, UserInformation_fields, &pack_info);

	char name[32] = {0};
	char phone[32] = {0};
	char email[32] = {0};

	UserInformation unpack_info;

	unpack_info.name.arg = name;
	unpack_info.name.funcs.decode = pb_decode_callback;
	unpack_info.phone.arg = phone;
	unpack_info.phone.funcs.decode = pb_decode_callback;
	unpack_info.email.arg = email;
	unpack_info.email.funcs.decode = pb_decode_callback;

	unpack_pb_data(pb_buffer, UserInformation_fields, lenght, &unpack_info);

	Log(1, "Name: %s.", (char*)unpack_info.name.arg);
	Log(1, "Age: %d.", unpack_info.age);
	Log(1, "Phone: %s.", (char*)unpack_info.phone.arg);
	Log(1, "Stat: %d.", unpack_info.stat);
	Log(1, "Email: %s.", (char*)unpack_info.phone.arg = email);

	return 0;
}
*/