#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <memory>
#include <vector>

enum Command {
    DATA = 1,
    CONFIRM = 2,
    RETRANSLATE = 3
};

using Data = std::vector<std::uint8_t>;

struct Packet {
    std::uint8_t id;
    std::uint8_t command;
    Data data;
};

class PacketSender {
public:
    virtual void send(const Packet& p) = 0;
};
using PacketSenderPtr = std::shared_ptr<PacketSender>;

class PacketReceiver {
public:
    virtual void receive(const Packet& p) = 0;
};
using PacketReceiverPtr = std::shared_ptr<PacketReceiver>;

class PacketTransmitter : public PacketReceiver {
public:
    using DataReceiver = std::function<void(const Data&)>;

    PacketTransmitter(PacketSenderPtr sender);

    void receive(const Packet& p);

private:
    void retranslate(uint8_t id);

    PacketSenderPtr sender;
    std::uint8_t nextReceiveId = 0;
    std::uint8_t nextSendId = 0;
    std::map<uint8_t, Packet> sendBuffer;
    DataReceiver dataReceiver;
};

class QRPacketSender : public PacketSender {
    // TODO
};

class KeyboardPacketSender : public PacketSender {
    // TODO
};
