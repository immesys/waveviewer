#!/usr/bin/env python
import base64
import os
import os.path
import pyqrcode
import sys

fn = None
if len(sys.argv) == 2:
    fn = sys.argv[1]
elif len(sys.argv) == 1:
    fn = os.environ.get("BW2_DEFAULT_ENTITY", None)

if fn is None:
    print "Usage: {0} <entity file>".format(sys.argv[0])
    print "If BW2_DEFAULT_ENTITY is set, <entity file> may be omitted"
    sys.exit(0)

try:
    with open(fn, "rb") as f:
        contents = f.read()
except IOError as ioe:
    print "Could not read {0}: {1}".format(fn, ioe)
    sys.exit(0)

qrcode = pyqrcode.create(base64.b64encode(contents))
output = os.path.basename(fn) + ".png"
try:
    qrcode.png(output, scale=6)
except IOError as ioe:
    print "Could not write {0}: {1}".format(output, ioe)
