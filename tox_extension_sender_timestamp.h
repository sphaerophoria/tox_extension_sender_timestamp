#pragma once

#include <toxext/toxext.h>

struct ToxExtensionSenderTimestamp;

typedef void(*tox_extension_sender_timestamp_received_cb)(uint32_t friend_number, uint64_t utcUnixTime, void *user_data);
typedef void(*tox_extension_sender_timestamp_negotiate_cb)(uint32_t friend_number, bool negotiated, void *user_data);
struct ToxExtensionSenderTimestamp* tox_extension_sender_timestamp_register(
    struct ToxExt* toxext,
    tox_extension_sender_timestamp_received_cb cb,
    tox_extension_sender_timestamp_negotiate_cb neg_cb,
    void* userdata);
void tox_extension_sender_timestamp_free(struct ToxExtensionSenderTimestamp* extension);
void tox_extension_sender_timestamp_negotiate(struct ToxExtensionSenderTimestamp* extension, uint32_t friend_id);

void tox_extension_sender_timestamp_append(struct ToxExtensionSenderTimestamp* extension, struct ToxExtPacket* packet, uint64_t timestamp);

