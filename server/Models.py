import datetime
import json

from sqlalchemy import Column, Integer, String, DateTime, ForeignKey, Float
from sqlalchemy.orm import relationship

from bootstrap import db


class Button(db.Model):
    __tablename__ = 'button'
    id = Column(Integer, primary_key=True)
    mac = Column(String(18), nullable=False, unique=True)
    counter_access = Column(Integer, default=0)
    product_id = Column(Integer, ForeignKey('product.id'))
    date_access = Column(DateTime)

    def __init__(self, mac):
        self.mac = mac
        self.date_access = datetime.datetime.now().utcnow()

    def raise_counter(self):
        self.counter_access += 1
        self.date_access = datetime.datetime.now().utcnow()

    def reset_counter(self):
        self.counter_access = 0
        self.date_access = datetime.datetime.now().utcnow()

    def to_json(self):
        return json.dumps({"id": self.id, "count": self.counter_access})

    def __repr__(self):
        return "Button ID:{}, MAC ADDRESS: {}, Counts: {}".format(self.id, self.mac, self.counter_access)


class Product(db.Model):
    __tablename__ = 'product'
    id = Column(Integer, primary_key=True)
    name = Column(String(50))
    description = Column(String(120))
    buttons = relationship('Button', backref='product')

    def __init__(self, name, description):
        self.name = name
        self.description = description

    def __repr__(self):
        return "Id:{}, Name:{}, Description:{}".format(self.id, self.name, self.description)


class Cracha(db.Model):
    __tablename__ = 'cracha'
    id = Column(Integer, primary_key=True)
    mac = Column(String(18), nullable=False)
    rfid = Column(String(18), nullable=False)
    date_access = Column(DateTime)

    def __init__(self, mac, rfid):
        self.mac = mac
        self.rfid = rfid
        self.date_access = datetime.datetime.now().utcnow()

    def add(self):
        db.session.add(self)
        db.session.commit()

    def __repr__(self):
        return "Cracha ID:{}, MAC ADDRESS: {}, Rfid: {}".format(self.id, self.mac, self.rfid)


class User(db.Model):
    __tablename__ = 'user'
    id = Column(Integer, primary_key=True)
    username = Column(String(18), nullable=False)
    rfid = Column(String(18), nullable=False)
    defcon = Column(Integer, default=0)

    def __init__(self, username, rfid):
        self.username = username
        self.rfid = rfid

    def __repr__(self):
        return "User ID:{}, Username: {}, Rfid: {}".format(self.id, self.username, self.rfid)


class Location(db.Model):
    __tablename__ = 'location'
    id = Column(Integer, primary_key=True)
    mac = Column(String(18), nullable=False, unique=True)
    name = Column(String(50))
    longitude = Column(Float, nullable=True)
    latitude = Column(Float, nullable=True)
    defcon = Column(Integer, default=0)

    def __init__(self, mac, name, longitude, latitude, defcon):
        self.name = name
        self.mac = mac
        self.longitude = longitude
        self.latitude = latitude
        self.defcon = defcon

    def add(self):
        db.session.add(self)
        db.session.commit()

    def __repr__(self):
        return "ID:{}, name: {}, mac: {}, defcon:{}".format(self.id, self.name, self.mac, self.defcon)
