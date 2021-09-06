from sqlalchemy import Column, Integer, String
from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import create_engine
from sqlalchemy.orm import sessionmaker
from passlib.hash import sha256_crypt
from json import dumps
from os import urandom
from collections import OrderedDict
from binascii import hexlify

Base = declarative_base()

class User(Base):
    __tablename__ = "Users"
    idr = Column(Integer,primary_key=True)
    username = Column(String(100),nullable=False)
    password = Column(String(100),nullable=False)
    email = Column(String(100),nullable=False)
    isAdmin = Column(Integer)
    sessionId = Column(String(100),nullable=True)

    def __init__(self,username,password,email,isAdmin=0):
        self.username = username
        self.password = sha256_crypt.encrypt(password)
        self.email = email
        self.isAdmin = 0
        self.sessionId = None

    @staticmethod
    def gen_session_secret():
        return hexlify(urandom(16))

    def generate_cookie(self):
        self.sessionId = self.gen_session_secret()
        return dumps(OrderedDict([("username",self.username),("email",self.email),("isAdmin",self.isAdmin),("sessionId",self.sessionId)]))


engine = create_engine("mysql://dbuser:dbpass@dbhost:3306/db")
Base.metadata.bind = engine
DBSession = sessionmaker(bind=engine)
session = DBSession()
Base.metadata.create_all(engine)
user = User('admin','jyk3W2ZL2GPxPIzsslBNONjcAsnWYl1F','admin@lachupacabra.com')
user.isAdmin = 1
session.add(user)
session.commit()
