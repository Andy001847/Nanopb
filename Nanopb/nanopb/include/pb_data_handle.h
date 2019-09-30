#ifndef __PB_DATA_HANDLE_H__
#define __PB_DATA_HANDLE_H__

#include "pb_decode.h"
#include "pb_encode.h"

#define PB_BUFFER_SIZE    1024

/* Functions prototype */
int pb_pack_data(pb_byte_t* pb_buffer, const pb_field_t* fields, const void* pack_info);
int pb_unpack_data(const pb_byte_t* pb_buffer, size_t pb_buffer_size, const pb_field_t* fields, void* unpack_info);
bool pb_encode_callback(pb_ostream_t* stream, const pb_field_t* fields, void* const* arg);
bool pb_decode_callback(pb_istream_t* stream, const pb_field_t* fields, void** arg);

#endif    /* End define macro __PB_DATA_HANDLE_H__ */
