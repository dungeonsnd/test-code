from twisted.internet import reactor, protocol


class Echo(protocol.Protocol):
    """This is just about the simplest possible protocol"""
    def connectionMade(self):
        print 'Got connection from', self.transport.client
    def connectionLost(self, reason):
        print self.transport.client, 'disconnected!'
    def dataReceived(self, data):
        print 'Recved data:',data
        data1 =data[0:2]
        data2 =data[2:-1]
        self.transport.write(data1)
        self.transport.write(data2)


def main():
    """This runs the protocol on port 8000"""
    factory = protocol.ServerFactory()
    factory.protocol = Echo
    reactor.listenTCP(8000,factory)
    reactor.run()

# this only runs if the module was *not* imported
if __name__ == '__main__':
    main()
