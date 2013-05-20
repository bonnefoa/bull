BULL_PATH=~/git-repos/bull

python ${BULL_PATH}/blender/list_files.py ${BULL_PATH}/models.yaml > /tmp/export_all.py
cat ${BULL_PATH}/blender/export_all.py >> /tmp/export_all.py

blender -P /tmp/export_all.py
