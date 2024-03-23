import os
from flask_sqlalchemy import SQLAlchemy
from bt_iot_webapp.scripts import dependency_checker
from flask import Flask

def create_app(test_config=None):
    app = Flask(__name__, instance_relative_config=True)
    app.config.from_mapping(
        SECRET_KEY='dev',
        TESTING=True, # needed for auto update while developing
        DATABASE=os.path.join(app.instance_path, 'bt_iot_webapp.sqlite'),
    )

    if test_config is None:
        app.config.from_pyfile('config.py', silent=True)
    else:
        app.config.from_mapping(test_config)

    try:
        os.makedirs(app.instance_path)
    except OSError:
        pass
    
    return app

app = create_app()

basedir = os.path.abspath(os.path.dirname(__file__))

app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///' + os.path.join(basedir, 'bt_iot.db') # Creates table
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False # ignores any warning messages
db = SQLAlchemy(app)

from bt_iot_webapp import views, models