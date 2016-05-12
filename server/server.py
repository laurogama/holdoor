import datetime
import json
import logging

from flask import Flask, render_template, request, jsonify
from flask.ext.assets import Environment
from flask_sqlalchemy import SQLAlchemy
from sqlalchemy import Column, String, Integer, DateTime

from ErrorHandler import InvalidUsage
from assets import bundles
from settings import CONFIG

app = Flask(__name__)
assets = Environment(app)
assets.register(bundles)
configuration = CONFIG
app.config['SECRET_KEY'] = configuration['SECRET_KEY']
app.config['SQLALCHEMY_DATABASE_URI'] = configuration['SQLITE_TEST_DB']
db = SQLAlchemy(app)


class Button(db.Model):
    __tablename__ = 'monitor'
    id = Column(Integer, primary_key=True)
    mac = Column(String(18), nullable=False, unique=True)
    counter_access = Column(Integer, default=0)
    date_access = Column(DateTime)

    def __init__(self, mac):
        self.mac = mac
        self.date_access = datetime.datetime.now().utcnow()

    def raise_counter(self):
        self.counter_access += 1

    def to_json(self):
        return json.dumps({"id": self.id, "count": self.counter_access})

    def __repr__(self):
        return "Button ID:{}, MAC ADDRESS: {}, Counts: {}".format(self.id, self.mac, self.counter_access)


@app.route('/')
def index():
    buttons = Button.query.all()
    return render_template("index.html", buttons=buttons)


@app.route('/button/', methods=['POST'])
def button():
    try:
        print request
        data = request.get_json(force=True)
        print data['mac']
        button = Button.query.filter_by(mac=data['mac']).first()
        if button is None:
            monitor = Button(data['mac'])
            db.session.add(monitor)
            db.session.commit()
        else:
            button.raise_counter()
            db.session.commit()
        print button
        return button.to_json(), 200

    except Exception as exception:
        print exception
        raise InvalidUsage('Still coding', status_code=500)


@app.errorhandler(InvalidUsage)
def handle_invalid_usage(error):
    try:
        response = jsonify(error.to_dict())
        response.status_code = error.status_code
        return response
    except Exception as exception:
        print exception


if __name__ == '__main__':
    logging.basicConfig(filename=configuration['LOG_PATH'], level=logging.INFO, debug=True)
    db.create_all()
    app.run(host='0.0.0.0', port=configuration['APP_PORT'])
