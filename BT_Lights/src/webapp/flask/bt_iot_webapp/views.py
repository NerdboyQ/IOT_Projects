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