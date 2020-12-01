import unittest

from register import Consul

import os

consulHost = os.environ['CONSUL_HOST']
consulPort = os.environ['CONSUL_PORT']


class TestConsul(unittest.TestCase):
    def test_init(self):
        print('Test consul init start.')
        consul_client = Consul(consulHost, consulPort)
        self.assertIsNotNone(consul_client)
        print('Test consul init finish.')

    def test_register_service(self):
        print('Test consul register service start.')
        consul_client = Consul(consulHost, consulPort)
        consul_client.RegisterService('test', consulHost, int(consulPort))
        service, _ = consul_client.GetService('test')
        self.assertIsNotNone(service)
        print('Test consul register service finish.')

        print('Test consul deregister service start.')
        consul_client.DeregisterService('test')
        service, _ = consul_client.GetService('test')
        self.assertIsNone(service)
        print('Test consul deregister service finish.')


if __name__ == '__main__':
    unittest.main()
