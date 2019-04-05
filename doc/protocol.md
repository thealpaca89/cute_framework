# Cute Protocol Standard

The Cute protocol implements a client-server based transport layer for securely connecting and authenticating with backend game servers over UDP. The purpose of this document is to aid users in making educated decisions about how to use the Cute Protocol in their own game.

## High Level Overview

The main pieces of the Cute Protocol are:

1. Connect tokens.
2. Web service.
3. Dedicated servers.
4. Clients.

The web service provides an authentication mechanism via [REST](https://en.wikipedia.org/wiki/Representational_state_transfer) call ([HTTPS](https://en.wikipedia.org/wiki/HTTPS) is recommended, but not required). Any authentication technique can be used since authentication with a web service is: A) very well understood with many good pre-built solutions (like OAuth/2 or OpenID). The web service and API are out of scope of the standard, except for how they produce a connect token.

The connect token is the mechanism that allows clients to securely authenticate with a dedicated server.

Dedicated servers are the servers actually running game. Clients are the players who connect to a dedicated server in order to play.

## Connecting as a Client

The steps for a client to connect to a dedicated game server are:

1. Client wants to authenticate with the web service, and issues a REST call to obtain a connect token.
2. Web service generates and returns a connect token to the client.
3. Client sends the token to a dedicated server instance, in order to securely setup a connection and play.
4. The dedicated server processes the connect token, and if valid, starts the connection handshake with the client.
5. If the handshake succeeds, the player is connected and begins to play over a secure UDP channel.

## Connect Token Format

The connect token has three major sections.

1. PUBLIC SECTION
2. SECRET SECTION
3. REST SECTION

Once a client receives a connect token from the web service, the PUBLIC SECTION is read by the client. This section contains a server IP list of dedicated game servers to attempt to connect to, along with some other data. Once read, the client deletes the REST SECTION. The remaining data in the connect token consist of 1024 bytes. The final 1024 bytes are called the *connect token packet*.

The *connect token packet* is not modifiable by the client, and the SECRET SECTION is not readable by anyone but the web service, and the dedicated servers. The entire *connect token packet* is protected by the cryptographically secure AEAD ([Authenticated Encryption with Additional Data](https://en.wikipedia.org/wiki/Authenticated_encryption#Authenticated_encryption_with_associated_data)) primitive XChaCha20-Poly1305, provided by libsodium. They key used for the AEAD primitive is a shared secret known by all dedicated game servers, and the web service. It is recommended to implement a mechanism to rotate this key periodically, though the mechanism to do so is out of scope for this document.

##### Note:
> The AEAD primitive is a function that encrypts a chunk of data, and computes an HMAC ([keyed-hash message authentication code](https://en.wikipedia.org/wiki/HMAC)). The HMAC is a 16 byte value used to authenticate the message, and prevent tampering/modification of the message (i.e. maintain integrity of the message). The encryption ensures only those who know the key can read the message. The Additional Data (the AD in AEAD) is a chunk of data that is not encrypted, but "mixed-in" to the computation of the HMAC.

The PUBLIC SECTION of the *connect token packet* is used as Additional Data for the AEAD, where the SECRET SECTION is encrypted by the AEAD. Once the AEAD is used, the output HMAC is appended to the final 16 bytes of the token, thus completing the full 1024 bytes *connect token packet*.

### The Connect Token Format
```
--  BEGIN PUBLIC SECTION  --
---  BEGIN REST SECTION  ---  
version info                   9          "Cute 1.00" ASCII, including nul byte.
protocol id                    uint64_t   User chosen value to identify the game.
creation timestamp             uint64_t   Unix timestamp of when the connect token was created.
client to server key           32 bytes   Client uses to encrypt packets, server uses to decrypt packets.
server to client key           32 bytes   Server uses to encrypt packets, client uses to decrypt packets.
----  END REST SECTION  ----            
zero byte                      1 byte     Represents packet type of *connect token packet*.
protocol id                    uint64_t   User chosen value to identify the game.
expiration timestamp           uint64_t   Unix timestamp of when the connect token becomes invalid.
handshake timeout              int32_t    Seconds of how long a connection handshake will wait before timing out.
number of server endpoints     uint32_t   The number of servers in the following list in the range of [1, 32].
<for each server endpoint>
    address type               uint8_t    1 = IPv4, 2 = IPv6 address, in the range of [1, 2].
    <if IPv4 address>          6 bytes    Format: a.b.c.d:port
        a                      uint8_t
        b                      uint8_t
        c                      uint8_t
        d                      uint8_t
        port                   uint16_t
    <else if IPv6 address>     18 bytes   Format: [a:b:c:d:e:f:g:h]:port
        a                      uint16_t
        b                      uint16_t
        c                      uint16_t
        d                      uint16_t
        e                      uint16_t
        f                      uint16_t
        g                      uint16_t
        h                      uint16_t
        port                   uint16_t
    <end if>
<end for>
<zeroes padded to 656 bytes>              Counting from the end of the REST SECTION.
connect token nonce            24 bytes
// --  END PUBLIC SECTION  --
// -- BEGIN SECRET SECTION --
client id                      uint64_t   Unique identifier for a particular client.
client to server key           32 bytes   Client uses to encrypt packets, server uses to decrypt packets.
server to client key           32 bytes   Server uses to encrypt packets, client uses to decrypt packets.
user data                      256 bytes  Space for the user to store whatever auxiliary data they need.
// --  END SECRET SECTION  --
HMAC bytes                     16 bytes   Written and used by encryption primitives to verify key signature.
```

The *connect token packet* is sent as-is without modification to a game server in the list. All game servers are valid choices, but it is recommended to start by making an attempt to connect to the first server in the list. If a failure occurs (for example the server is full), the client can move onto the next server in the list. Storing only a single server in the list is acceptable, but it is recommended to have more than one if possible, to improve the chances a client can successfully connect to a game server and play.

Once the client sends the *connect token packet* to a game server, the connection handshake begins.

##### Note:
> The Unix timestamps can, for example, be created with something similar to `(uint64_t)time(NULL)` in the C language.

## Connection Handshake

The connection handshake begins once a client starts sending a *connect token packet* to a game server. The client runs a state machine to fulfill the handshake. Once the state machine completes successfully, the connection is complete and both the client and game server are free to send *payload packet*'s to each other, containing game-specific data.

##### Note:
> There is a description of all packet types and their formats later in this document in the [Packet Formats](#packet-formats) section.

## Client Handshake State Machine

The various client states.

State Name | Value
--- | ---
*connect token expired* | -6
*invalid connect token* | -5
*connection timed out* | -4
*challenge response timed out* | -3
*connection request timed out* | -2
*connection denied* | -1
*disconnected* | 0
*sending connection request* | 1
*sending challenge response* | 2
*connected* | 3

Each state only accepts a subset of packet types, and all extraneous packets received are ignored.

### Disconnected

The default state is *disconnected*. The client transitions to the *sending connection request* state once they receive a valid connect token from the web service and have recorded data from the PUBLIC SECTION of the connect token.

Exactly how the client requests and receives the connect token is out of scope of this document, besides requiring a secure REST API call. Exactly how the web service generates the server list of the connect token is also out of scope of this document, as this is the realm of match-making or other similar mechanisms.

The client reads the PUBLIC SECTION of the connect token and records all of the data. If the data is invalid the client transitions to the *invalid connect token* state. The connect token is deemed invalid by the client if the number of server addresses is outside the range of [1, 32], or if an IP address type is not in the range [1, 2], or if the creation timestamp is more recent than the expiration timestamp.

### Sending Connection Request

The client sends the *connect token packet* to a game server from the list and waits for a response. This packet is **not** encrypted by the client, as it is already processed by the AEAD primitive by the web service. If the server responds with a *challenge request packet*, the client transitions to the *sending challenge response*, after decrypting and recording the contents of the *challenge request packet*.

It is recommended the client moves onto the next server in the server list in the event of failure cases. If no more servers are left in the list, the client transitions to one of the error states.

If the last server in the list responds with a *connection denied packet*, the client transitions to the *connection denied* state. If the last server in the server does not send a *challenge request packet* or a *connection denied packet*, the client transitions to the *connection request timed out* state.

### Sending Challenge Response

During the *sending connection request* the client stored the contents of *challenge request packet*. The contents are used to form the *challenge response packet*. The client sends the server the *challenge response packet*. The purpose is to verify a few things about the client:

1. That the client is not spoofing their IP address, and can successfully respond to the server by reflecting the *challenge request packet* with a *challenge response packet*.
2. That the client is able to decrypt the *challenge request packet* and encrypt the *challenge response packet*. This asserts that the client was the **only** user the web service gave the original connect token to.

If successful, the server will respond with a *connection accepted packet*, and the client transitions to the *connected* state after recording data from the *connection accepted packet*. The client records the *client id* field, the *max clients* field, and the *connection timeout* field from the *connection accepted packet*.

It is recommended the client moves onto the next server in the server list in the event of failure cases. If no more servers are left in the list, the client transitions to one of the error states.

If the last server in the list responds with a *connection denied packet*, the client transitions to the *connection denied* state. If the last server in the server does not send a *connection accepted packet* or a *connection denied packet*, the client transitions to the *challenge response timed out* state.

### Connected

The purpose of the connected state is to allow the user to send and receive *payload packet*'s containing game-specific data. In the absence of any payload packets, the client generates and sends a *keepalive packet* at the KEEPALIVE_FREQUENCY tuning parameter (see [tuning parameters](#tuning-parameters)).

If no *payload packet*'s or *keepalive packet*'s are received from the server within a *connection timeout* timespan, the client transitions to the *connection timed out* state.

If the client receives a *disconnect packet* from the server it transitions to the *disconnected* state after performing the [Disconnect Sequence](#disconnect-sequence).

### Connect Token Expired

If at any time the client's connect token expires, the client transitions to the *connect token expired*. This can happen if the client has to try connecting to multiple servers in the server list, and continually fails to establish a connection.

The time to live for the connect token is calculated as:

    expiration timestamp - creation timestamp

## Packet Formats

* *connect token packet*
* *keepalive packet*
* *connection denied packet*
* *payload packet*
* *connection accepted packet*
* *challenge request packet*
* *challenge response packet*
* *disconnect packet*

## Encrypted Packets

All packets except for the *connect token packet* are encrypted. Each encrypted packet has the following form.

```
packet type      1 byte
sequence nonce   uint64_t
encrypted bytes  <variable length>
HMAC bytes       16 bytes
```

All packets are 1280 bytes or smaller, so the maximum size of the `encrypted bytes`	 is 1255 bytes.

## Unencrypted Packets

Prior to encryption each packet type has a different format, which comprise the data that forms the `encrypted bytes` post-encryption.

### Keepalive Packet

```
<no data>           0 bytes
```

The *keepalive packet* is only sent if no *keepalive packet*'s, *disconnect packet*'s, or *payload packet*'s have been received within KEEPALIVE_FREQUENCY seconds during the client *connected* state. If none of these packets are received by either the client or the server, the connection is terminated after `connection timeout` seconds.

### Connection Denied Packet

```
<no data>           0 bytes
```

The *connection denied packet* can be sent by the server during the connection handshake. It is sent whenever an error occurs (like no room on the server, or received packets fail to decrypt), or if the client fails to comply with the proper handshake process.

### Payload Packet

```
payload data        In the range of [1, 1255] bytes.
```

The *payload packet* can be sent by the client or the server during the *connected* state of the client. They contain game specific user data.

### Connection Accepted Packet

```
client id           uint64_t
max clients         uint32_t
connection timeout  uint32_t
```

The *connection accepted packet* is sent from both the client and the server once a client has successfully connected to the server.

### Challenge Request Packet

```
sequence nonce      uint64_t
secret data         256 bytes
```

The *challenge request packet* is sent from the server as apart of the connection handshake process. This challenge response sequence is used to prevent ip spoofing and *connect token packet* sniffing. For more information about how, see the [Sending Challenge Response](#sending-challenge-response) section.

The `sequence nonce` is an incrementing counter starting at 0, initialized upon server restart.

The `secret data` is simply 256 bytes of data. The data can be anything, including randomized bits. Exactly what bits are the `secret data` is left to the implementation. To fulfill the purpose of this packet, the client merely needs to decrypt the packet with the `server to client` key, encrypt it with the `client to server key`, and send it back to the server. The contents of the `secret data` are ignored by the client.

### Challenge Response Packet

```
sequence nonce      uint64_t
secret data         256 bytes
```

The *challenge response packet* is the reflected version of the *challenge request packet*, and is sent by the client as apart of the connection handshake process. All the client has to do is copy the entire decrypted *challenge request packet*, encrypt it, and send it back to the server for verification. For more information see the [Sending Challenge Response](#sending-challenge-response), or the [Challenge Request Packet](#challenge-request-packet) section.

### Disconnect Packet

```
<no data>           0 bytes
```

The *disconnect packet* can be sent by the client or the server during the client *connected* state. Once sent, the connection is considered terminated. Once received, the connection is also considered terminated. Sending the *disconnect packet* must be done by the [Disconnect Sequence](#disconnect-sequence) in order to statistically assure clean disconnects, without waiting for unnecessary timeouts.

## Server Handshake and Connection Process

## Disconnect Sequence

In order to gracefully disconnect, either the client or the server can perform the Disconnect Sequence, which means to fire off a series of *disconnect packet*'s in quick succession (e.g. in a for loop). The number of packets is defined by the DISCONNECT_SEQUENCE_PACKET_COUNT tuning parameter (see [tuning parameters](#tuning-parameters)). The purpose of the redundancy is to be statistically likely that one of the packets gets through to the endpoint, even in the face of packet loss.

## Tuning Parameters

* KEEPALIVE_FREQUENCY
* DISCONNECT_SEQUENCE_PACKET_COUNT

## Constants

## Protection Against Various Attacks

* Replay
* Reflection
* IP spoofing
* "Going wide"
* Packet sniffing
* DDoS mitigation
* DoS amplification

TODO: Link all packet types and states with anchors.