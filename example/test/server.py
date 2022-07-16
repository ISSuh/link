import asyncio
from asyncio import Transport, DatagramTransport
from ctypes import sizeof

HOST, PORT = '0.0.0.0', 3600


class TCPEchoServerProtocol(asyncio.Protocol):
    transport: Transport

    def connection_made(self, transport: Transport):
        self.transport = transport

    def data_received(self, data: bytes):
        message = data.decode()
        print('Data received: [{!r}] {!r}'.format(len(message), "a"))

        self.transport.write(data)


class UDPEchoServerProtocol(asyncio.DatagramProtocol):
    transport: DatagramTransport

    def connection_made(self, transport: DatagramTransport):
        self.transport = transport

    def datagram_received(self, data: bytes, addr):
        self.transport.sendto(data, addr)


async def main():
    loop = asyncio.get_running_loop()

    udp_transport, _ = await loop.create_datagram_endpoint(
        lambda: UDPEchoServerProtocol(),
        local_addr=(HOST, PORT)
    )
    tcp_server = await loop.create_server(lambda: TCPEchoServerProtocol(), HOST, PORT)

    async with tcp_server:
        await tcp_server.serve_forever()

    udp_transport.close()


if __name__ == '__main__':
    asyncio.run(main())
