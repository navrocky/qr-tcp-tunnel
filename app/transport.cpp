#include "transport.h"

PacketTransmitter::PacketTransmitter(PacketSenderPtr sender)
    : sender(sender)
{
}

void PacketTransmitter::receive(const Packet& p)
{
    if (p.id != nextReceiveId) {
        // query retranslate and skip received package
        retranslate(nextReceiveId);
        return;
    }

    // confirm packet
}

void PacketTransmitter::retranslate(std::uint8_t id)
{
    auto packet = Packet {
        .id = nextSendId++,
        .command = Command::RETRANSLATE,
        .data = { id },
    };
    sendBuffer[packet.id] = packet;
    sender->send(packet);
}
