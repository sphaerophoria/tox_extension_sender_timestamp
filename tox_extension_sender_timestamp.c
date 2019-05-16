#include <toxext/toxext.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "tox_extension_sender_timestamp.h"


static uint8_t const uuid[16] = { 0xe5, 0xd8, 0xac, 0x84, 0x5d, 0xdf, 0x41, 0x6a, 0xb0, 0xe9, 0x6f, 0x3b, 0x29, 0xcf, 0x5f, 0x02};


struct ToxExtensionSenderTimestamp {
	struct ToxExtExtension* extension_handle;
	tox_extension_sender_timestamp_received_cb cb;
	tox_extension_sender_timestamp_negotiate_cb negotiated_cb;
	void* userdata;
};

// Here they confirm that they are indeed sending us sender_timestamp. We don't call the
// negotation callback when we've both negotiated that we have the extension
// because at that point they don't know we can accept the message ids. We wait for
// an enable flag from the other side to indicate that they are now embedding message ids.
static void tox_extension_sender_timestamp_recv(uint32_t friend_id, void const* data, size_t size, void* userdata, struct ToxExtPacket* response_packet) {
	(void)response_packet;
	struct ToxExtensionSenderTimestamp *ext_message_ids = userdata;

	if (size != 8) {
		return;
	}

	uint8_t const* data_b = data;
	uint64_t timestamp = 0;
	timestamp |= (uint64_t)data_b[0] << 56;
	timestamp |= (uint64_t)data_b[1] << 48;
	timestamp |= (uint64_t)data_b[2] << 40;
	timestamp |= (uint64_t)data_b[3] << 32;
	timestamp |= (uint64_t)data_b[4] << 24;
	timestamp |= (uint64_t)data_b[5] << 16;
	timestamp |= (uint64_t)data_b[6] << 8;
	timestamp |= (uint64_t)data_b[7];

	ext_message_ids->cb(friend_id, timestamp, ext_message_ids->userdata);
}

static void tox_extension_sender_timestamp_neg(struct ToxExtExtension* extension, uint32_t friend_id, bool compatible, void* userdata, struct ToxExtPacket* response_packet) {
	(void)extension;
	(void)response_packet;
	struct ToxExtensionSenderTimestamp *ext_message_ids = userdata;

	ext_message_ids->negotiated_cb(friend_id, compatible, ext_message_ids->userdata);
}

struct ToxExtensionSenderTimestamp* tox_extension_sender_timestamp_register(
    struct ToxExt* toxext,
    tox_extension_sender_timestamp_received_cb cb,
    tox_extension_sender_timestamp_negotiate_cb neg_cb,
    void* userdata) {

	assert(cb);

	struct ToxExtensionSenderTimestamp* extension = malloc(sizeof(struct ToxExtensionSenderTimestamp));
	extension->extension_handle = toxext_register(toxext, uuid, extension, tox_extension_sender_timestamp_recv, tox_extension_sender_timestamp_neg);
	extension->cb = cb;
	extension->negotiated_cb = neg_cb;
	extension->userdata = userdata;

	if (!extension->extension_handle) {
		free(extension);
		return NULL;
	}

	return extension;
}

void tox_extension_sender_timestamp_free(struct ToxExtensionSenderTimestamp* extension) {
	toxext_deregister(extension->extension_handle);
	free(extension);
}

void tox_extension_sender_timestamp_negotiate(struct ToxExtensionSenderTimestamp* extension, uint32_t friend_id) {
	toxext_negotiate_connection(extension->extension_handle, friend_id);
}

void tox_extension_sender_timestamp_append(struct ToxExtensionSenderTimestamp* extension, struct ToxExtPacket* packet, uint64_t timestamp) {
	uint8_t data[8];
	data[0] = timestamp >> 56;
	data[1] = timestamp >> 48;
	data[2] = timestamp >> 40;
	data[3] = timestamp >> 32;
	data[4] = timestamp >> 24;
	data[5] = timestamp >> 16;
	data[6] = timestamp >> 8;
	data[7] = timestamp;
	toxext_packet_append(packet, extension->extension_handle, data, 8);
}
