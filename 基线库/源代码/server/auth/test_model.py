import unittest
import datetime

from model import User


class TestModel(unittest.TestCase):
    def test_user_serialize(self):
        user = User(id='id',
                    email='email',
                    ctime=datetime.datetime.now(),
                    role='role',
                    password='pwd')
        res = user.serialize()
        self.assertEqual(res['id'], 'id')
        self.assertEqual(res['email'], 'email')


if __name__ == '__main__':
    unittest.main()
