/*
    Copyright 2020 (C) Alexey Dynda

    This file is part of Tiny Protocol Library.

    Protocol Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Protocol Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Protocol Library.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 This is Tiny protocol implementation for microcontrollers

 @file
 @brief Tiny protocol Arduino API

*/
#pragma once

#include "TinyPacket.h"
#include "proto/hdlc/tiny_hdlc.h"

#ifdef ARDUINO
#   include <HardwareSerial.h>
#else
#   include <string.h>
#endif

namespace Tiny {

/**
 * @ingroup HDLC_API
 * @{
 */


/**
 *  ProtoHdlc class incapsulates hdlc Protocol functionality.
 *  hdlc version of the Protocol allows to send messages with
 *  confirmation.
 *  Remember that you may use always C-style API functions
 *  instead C++. Please refer to documentation.
 */
class ProtoHdlc
{
public:
    /**
     * Initializes ProtoHdlc object
     * @param buffer - buffer to store the frames being received.
     * @param bufferSize - size of the buffer
     */
    ProtoHdlc(void *buffer, int bufferSize)
        : m_buffer( buffer )
        , m_bufferSize( bufferSize )
    {
    }

    virtual ~ProtoHdlc() = default;

    /**
     * Initializes protocol internal variables.
     * If you need to switch communication with other destination
     * point, you can call this method one again after calling end().
     * @param writecb - write function to some physical channel
     * @param readcb  - read function from some physical channel
     * @return None
     */
    void begin          (write_block_cb_t writecb,
                         read_block_cb_t readcb);

    /**
     * Initializes protocol internal variables.
     * If you need to switch communication with other destination
     * point, you can call this method one again after calling end().
     * @return None
     */
    void begin();

#ifdef ARDUINO
    /**
     * Initializes protocol internal variables and redirects
     * communication through Arduino Serial connection (Serial).
     * @return None
     */
    inline void beginToSerial()
    {
         begin([](void *p, const void *b, int s)->int { return Serial.write((const uint8_t *)b, s); },
               [](void *p, void *b, int s)->int { return Serial.readBytes((uint8_t *)b, s); });
    }

#ifdef HAVE_HWSERIAL1
    /**
     * Initializes protocol internal variables and redirects
     * communication through Arduino Serial1 connection (Serial1).
     * @return None
     */
    inline void beginToSerial1()
    {
         begin([](void *p, const void *b, int s)->int { return Serial1.write((const uint8_t *)b, s); },
               [](void *p, void *b, int s)->int { return Serial1.readBytes((uint8_t *)b, s); });
    }
#endif

#ifdef HAVE_HWSERIAL2
    /**
     * Initializes protocol internal variables and redirects
     * communication through Arduino Serial2 connection (Serial2).
     * @return None
     */
    inline void beginToSerial2()
    {
         begin([](void *p, const void *b, int s)->int { return Serial2.write((const uint8_t *)b, s); },
               [](void *p, void *b, int s)->int { return Serial2.readBytes((uint8_t *)b, s); });
    }
#endif

#ifdef HAVE_HWSERIAL3
    /**
     * Initializes protocol internal variables and redirects
     * communication through Arduino Serial3 connection (Serial3).
     * @return None
     */
    inline void beginToSerial3()
    {
         begin([](void *p, const void *b, int s)->int { return Serial3.write((const uint8_t *)b, s); },
               [](void *p, void *b, int s)->int { return Serial3.readBytes((uint8_t *)b, s); });
    }
#endif


#ifdef HAVE_SERIALUSB
    /**
     * Initializes protocol internal variables and redirects
     * communication through Arduino Serial1 connection (SerialUSB).
     * @return None
     */
    inline void beginToSerialUSB()
    {
         begin([](void *p, const void *b, int s)->int { return SerialUSB.write((const char *)b, s); },
               [](void *p, void *b, int s)->int { return SerialUSB.readBytes((char *)b, s); });
    }
#endif

#endif

    /**
     * Resets protocol state.
     */
    void end();

    /**
     * Sends data block over communication channel.
     * @param buf - data to send
     * @param size - length of the data in bytes
     * @return negative value in case of error
     *         zero if nothing is sent
     *         positive - should be equal to size parameter
     */
    int  write(char* buf, int size);

    /**
     * Sends packet over communication channel.
     * @param pkt - Packet to send
     * @see Packet
     * @return negative value in case of error
     *         zero if nothing is sent
     *         positive - Packet is successfully sent
     */
    int  write(IPacket &pkt);

    /**
     * Processes incoming rx data, specified by a user.
     * @return TINY_SUCCESS
     * @remark if Packet is receive during run execution
     *         callback is called.
     */
    int run_rx(const void *data, int len);

    /**
     * Generates data for tx channel
     * @param data buffer to fill
     * @param max_len length of buffer
     * @return number of bytes generated
     */
    int run_tx(void *data, int max_len);

    /**
     * Disable CRC field in the protocol.
     * If CRC field is OFF, then the frame looks like this:
     * 0x7E databytes 0x7E.
     */
    void disableCrc();

    /**
     * Enables CRC by specified bit-size.
     * 8-bit is supported by Nano version of Tiny library.
     * @param crc crc type
     */
    void enableCrc(hdlc_crc_t crc);

    /**
     * Enables CRC 8-bit field in the protocol. This field
     * contains sum of all data bytes in the packet.
     * 8-bit field is supported by Nano version of Tiny library.
     * @return true if successful
     *         false in case of error.
     */
    bool enableCheckSum();

    /**
     * Enables CRC 16-bit field in the protocol. This field
     * contains FCS 16-bit CCITT like defined in RFC 1662.
     * 16-bit field is not supported by Nano version of Tiny library.
     * @return true if successful
     *         false in case of error.
     */
    bool enableCrc16();

    /**
     * Enables CRC 32-bit field in the protocol. This field
     * contains FCS 32-bit CCITT like defined in RFC 1662.
     * 32-bit field is not supported by Nano version of
     * Tiny library.
     * @return true if successful
     *         false in case of error.
     */
    bool enableCrc32();

    /**
     * Sets receive callback for incoming messages
     * @param on_receive user callback to process incoming messages. The processing must be non-blocking
     */
    void setReceiveCallback(void (*on_receive)(IPacket &pkt) = nullptr) { m_onReceive = on_receive; };

    /**
     * Sets send callback for outgoing messages
     * @param on_send user callback to process outgoing messages. The processing must be non-blocking
     */
    void setSendCallback(void (*on_send)(IPacket &pkt) = nullptr) { m_onSend = on_send; };

protected:
    /**
     * Method called by hdlc protocol upon receiving new frame.
     * Can be redefined in derived classes.
     * @param pdata pointer to received data
     * @param size size of received payload in bytes
     */
    virtual void onReceive(uint8_t *pdata, int size)
    {
        IPacket pkt((char *)pdata, size);
        pkt.m_len = size;
        if ( m_onReceive ) m_onReceive( pkt );
    }

    /**
     * Method called by hdlc protocol upon sending next frame.
     * Can be redefined in derived classes.
     * @param pdata pointer to sent data
     * @param size size of sent payload in bytes
     */
    virtual void onSend(const uint8_t *pdata, int size)
    {
        IPacket pkt((char *)pdata, size);
        pkt.m_len = size;
        if ( m_onSend ) m_onSend( pkt );
    }

private:
    /** The variable contain protocol state */
    hdlc_handle_t       m_handle = nullptr;

    hdlc_struct_t       m_data{};

    void               *m_buffer = nullptr;

    int                 m_bufferSize = 0;

    hdlc_crc_t          m_crc = HDLC_CRC_DEFAULT;

    /** Callback, when new frame is received */
    void              (*m_onReceive)(IPacket &pkt) = nullptr;

    /** Callback, when new frame is sent */
    void              (*m_onSend)(IPacket &pkt) = nullptr;

    /** Internal function */
    static int         onReceiveInternal(void *handle, void *pdata, int size);

    /** Internal function */
    static int         onSendInternal(void *handle, const void *pdata, int size);
};

/**
 * @}
 */

} // Tiny namespace

