import consul


class Consul(object):
    def __init__(self, host, port):
        self._consul = consul.Consul(host, port)

    def RegisterService(self, name, host, port, tags=None):
        tags = tags or []
        self._consul.agent.service.register(
            name,
            name,
            host,
            port,
            tags,
            check=consul.Check().tcp(host, port, "5s", "30s", "30s")
        )

    def GetService(self, name):
        services = self._consul.agent.services()
        service = services.get(name)
        if not service:
            return None, None
        addr = "{0}:{1}".format(service['Address'], service['Port'])
        return service, addr
