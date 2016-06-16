import os
import tempfile
import unittest

import server


class TestApi(unittest.TestCase):
    def setUp(self):
        self.db_fb, server.app.config['SQLALCHEMY_DATABASE_URI'] = tempfile.mkstemp()

        server.app.config['TESTING'] = True
        self.app = server.app.test_client()
        # server.app.db.create_all()

    def tearDown(self):
        os.close(self.db_fb)
        os.unlink(server.app.config['SQLALCHEMY_DATABASE_URI'])

    def test_cracha(self):
        pass

    def test_index(self):
        rv = self.app.get('/')
        assert 'No entries here so far' in rv.data
if __name__ == '__main__':
    unittest.main()
