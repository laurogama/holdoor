import datetime
import json

from sqlalchemy import Column, Integer, String, DateTime, ForeignKey
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
