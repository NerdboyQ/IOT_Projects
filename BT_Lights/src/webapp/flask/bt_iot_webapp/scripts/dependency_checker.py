import argparse
import calendar
import json
import logging
import subprocess
import sys
import datetime as dt
from datetime import datetime as dt2
import time as timer

# Check for necessary dependencies
required = {
    'babel',
    'Flask',
    'Flask-SQLAlchemy',
    'openpyxl',
    'pandas',
    'Path',
    'selenium',
    'tkcalendar',
    'webdriver-manager'
}
process = subprocess.Popen(['pip','list'], stderr=subprocess.PIPE, stdout=subprocess.PIPE)
out, err = process.communicate()
out = out.decode('utf-8').split("\n")
installed = {x.split(" ")[0] for x in out}
missing = required - installed
print("Missing dependencies:", missing)
if missing:
    python = sys.executable
    # run pip install
    subprocess.check_call(
        [python, '-m', 'pip', 'install', '--upgrade', 'pip'],
        stdout=subprocess.DEVNULL
    )
    # install dependencies
    subprocess.check_call(
        [python, '-m', 'pip', 'install', *missing],
        stdout=subprocess.DEVNULL
    )