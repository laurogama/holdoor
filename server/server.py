import logging

from flask import Flask
from flask.ext.assets import Environment
from flask_sqlalchemy import SQLAlchemy

from assets import bundles
from settings import CONFIG

app = Flask(__name__)
assets = Environment(app)
assets.register(bundles)
configuration = CONFIG
app.config['SECRET_KEY'] = configuration['SECRET_KEY']
app.config['SQLALCHEMY_DATABASE_URI'] = configuration['SQLITE_TEST_DB']
db = SQLAlchemy(app)


@app.route('/')
def hello_world():
    return 'Hello World!'


if __name__ == '__main__':
    logging.basicConfig(filename=configuration['LOG_PATH'], level=logging.INFO, debug=True)
    db.create_all()
    app.run(host='0.0.0.0', port=configuration['APP_PORT'])
