import os
import sys
from flask import render_template, jsonify, url_for, Response, stream_with_context
from bt_iot_webapp import app

@app.route("/")
def render_homepage():
    """
    Default homepage
    """
    return render_template("index.html")

@app.route("/scan-for-bt-devices")
def get_bt_devices():
    """
    Scanning for nearby bluetooth devices.
    """ 
    print("Scanning for BT devices...")
    devices = {"BLE":[], "nonBLE": []}


    return jsonify(devices)