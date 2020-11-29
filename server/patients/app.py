from flask import Flask
from flask_sqlalchemy import SQLAlchemy
from flask_restful import Api
from redisConfig import Redis
import pymongo
import os

db = SQLAlchemy()

user = os.environ['MYSQL_USER']
password = os.environ['MYSQL_PASSWORD']
database = os.environ['MYSQL_DB']
host = os.environ['MYSQL_HOST']
port = os.environ['MYSQL_PORT']

redisHost = os.environ['REDIS_HOST']
redisPort = os.environ['REDIS_PORT']

mongoHost = os.environ['MONGODB_HOST']
mongoPort = int(os.environ['MONGODB_PORT'])

redis = Redis(redisHost, redisPort)
mongodb = pymongo.MongoClient(mongoHost, mongoPort).test


def create_app():
    from controller import register_routes
    from imageController import register_image_routes

    app = Flask(__name__)
    app.config['SQLALCHEMY_DATABASE_URI'] = 'mysql://%s:%s@%s:%s/%s'\
        % (user, password, host, port, database)

    api = Api(app)

    register_routes(api, app)
    register_image_routes(api, app)
    db.init_app(app)

    return app


app = create_app()

if __name__ == "__main__":
    app.run(debug=True)
