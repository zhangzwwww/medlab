import unittest
import datetime

from model import Patient

class TestModel(unittest.TestCase):
    def test_patient_serialze(self):
        patient = Patient(id='id',
                          name='name',
                          gender='Male',
                          birth=datetime.datetime.now(),
                          age='1')
        res = patient.serialize()
        self.assertEqual(res['id'], 'id')
        self.assertEqual(res['name'], 'name')


if __name__ == '__main__':
    unittest.main()
