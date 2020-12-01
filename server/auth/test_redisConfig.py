import unittest
import os

from redisConfig import Redis

redisHost = os.environ['REDIS_HOST']
redisPort = os.environ['REDIS_PORT']


class TestRedis(unittest.TestCase):
    def test_init(self):
        redis = Redis(redisHost, redisPort)
        self.assertIsNotNone(redis)

    def test_addKey(self):
        redis = Redis(redisHost, redisPort)
        redis.addKey('testKey', 'testValue')
        res = redis.getValue('testKey')
        self.assertEqual(res, 'testValue')

    def test_deleteKey(self):
        redis = Redis(redisHost, redisPort)
        redis.addKey('testKey', 'testValue')
        redis.deleteKey('testKey')
        res = redis.getValue('testKey')
        self.assertIsNone(res)


if __name__ == '__main__':
    unittest.main()
