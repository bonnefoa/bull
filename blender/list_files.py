import yaml
import sys

files = []
data = yaml.load(open(sys.argv[1], "r"))

def to_blender(filename):
        filename = filename.rsplit('.', 1)[0]
        return filename + '.blend'

def process_model(model):
        for k, v in model.items():
                if k == 'filename':
                        files.append(to_blender(v))

for _, models in data.items():
        for model in models:
                process_model(model)

print('files = %s' % files)
