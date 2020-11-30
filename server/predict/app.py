from flask import Flask
from flask_restful import Api
from redisConfig import Redis

# import os

from ml_model.mlModel import Network
import torch

redisHost = os.environ['REDIS_HOST']
redisPort = os.environ['REDIS_PORT']

redis = Redis(redisHost, redisPort)

model = Network()
model.load_state_dict(torch.load('./ml_model/model_param/model_states.pth'))


def create_app():
    from controller import register_routes

    app = Flask(__name__)

    api = Api(app)
    register_routes(api, app)

    return app


app = create_app()

if __name__ == '__main__':
    app.run(debug=True)
