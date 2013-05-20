#!/bin/bash -e

python blender/list_files.py models.yaml > /tmp/export_all.py
cat blender/export_all.py >> /tmp/export_all.py

blender -P /tmp/export_all.py
