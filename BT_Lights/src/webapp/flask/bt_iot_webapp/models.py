from bt_iot_webapp import app, db

def cleanRecordForJson(d: dict):
    d.pop(list(d.keys())[0])
    for k, v in d.items():
        if type(v) != str:
            d[k] = str(v)

    return str(d).replace("'","\"") 

class ConnectedDevices(db.Model):
    _id = db.Column(db.Integer, nullable=False, primary_key=True)
    name = db.Column(db.String(120), nullable=False, default="")
    addr = db.Column(db.String(120), nullable=False, default="")

with app.app_context():
    db.create_all()