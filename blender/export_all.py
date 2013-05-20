import os
import bpy
import math
import mathutils

bpy.ops.wm.addon_disable(module="export_shapes")
bpy.ops.wm.addon_enable(module="export_shapes")

def process_file(f):
        bpy.ops.wm.open_mainfile(filepath=f)
        filename_file = bpy.path.display_name_from_filepath(bpy.data.filepath)
        rep = os.path.dirname(bpy.data.filepath)
        filepath_shape = os.path.join(rep, filename_file + '.yaml')
        filepath_dae = os.path.join(rep, filename_file + '.dae')

        rotmat = mathutils.Matrix.Rotation(math.pi / 2, 4, 'X')

        for _, obj in bpy.data.objects.items():
                if not obj.type == 'MESH':
                        continue
                for vert in obj.data.vertices:
                        vert.co = vert.co * rotmat

        bpy.context.scene.collada_export(filepath=filepath_dae, apply_modifiers=True)
        if any([obj.rigid_body for obj in bpy.data.objects]):
                bpy.ops.object.export_bullet_shapes(filepath=filepath_shape, y_up=True)

for f in files:
        process_file(f)

exit(0)
