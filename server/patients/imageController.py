from flask import request, Response
from flask_restful import Resource
import flask
import datetime
import bson.binary
import bson.objectid
import bson.errors
import hashlib
import pymongo
import json
import uuid

from app import mongodb
from app import redis


def register_image_routes(api, app):
    api.add_resource(ImageController, '/v1/image')
    api.add_resource(ImageDateController, '/v1/image/ctime')


class ImageController(Resource):
    def get(self):
        token = request.headers.get('x-auth-token')
        if not existsToken(token):
            resp = Response(status=401)
            return resp

        args = request.args

        patientId = args['patientId']
        time = args['ctime']

        contents = mongodb.files.find({'patientId': patientId, 'ctime': time})
        data = [{'name': d['name'],
                 'content': str(d['content'], encoding='ISO-8859-1'),
                 'ctime': d['ctime'],
                 'patientId': d['patientId']} for d in contents]
        resp = Response(json.dumps(data, ensure_ascii=False),
                        status=200, mimetype='applications/json')

        return resp

    def post(self):
        token = request.headers.get('X-Auth-Token')
        if not existsToken(token):
            resp = Response(status=401)
            return resp

        patientId = request.form['patientId']
        filename = request.form['filename']

        f = flask.request.files['uploaded_file']
        _ = saveFile(f, filename, patientId)
        return 'succeed', 200


class ImageDateController(Resource):
    def get(self):
        token = request.headers.get('X-Auth-Token')
        if not existsToken(token):
            resp = Response(status=401)
            return resp

        args = request.args

        patientId = args['patientId']
        contents = mongodb.files.find({'patientId': patientId})
        d = [d['ctime'] for d in contents]
        data = [{'ctime': d} for d in list(set(d))]
        resp = Response(json.dumps(data),
                        status=200, mimetype='applications/json')
        return resp


def saveFile(content, name, patientId):
    sha1 = hashlib.sha1(content.getvalue()).hexdigest()

    c = dict(
        id=uuid.uuid5(uuid.NAMESPACE_DNS, str(datetime.datetime.now())),
        content=bson.binary.Binary(content.getvalue()),
        ctime=str(datetime.date.today()),
        sha1=sha1,
        name=name,
        patientId=patientId,
    )

    try:
        mongodb.files.save(c)
    except pymongo.errors.DuplicateKeyError:
        pass
    return sha1


def existsToken(token):
    return redis.containsKey(token)
