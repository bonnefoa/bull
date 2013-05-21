bl_info = {
                "name": "Export Bullet Shapes",
                "author": "Anthonin Bonnefoy",
                "version": (1, 0),
                "blender": (2, 6, 6),
                "location": "File > Export",
                "description": "Export bullet shapes",
                "warning": "",
                "wiki_url": "",
                "tracker_url": "",
                "category": "Import-Export"}

import bpy
import bpy.types
import bpy.props
import mathutils
import bpy_extras
import os.path
import math

SHAPE_BOX = 'BOX'
SHAPE_SPHERE = 'SPHERE'
SHAPE_CYLINDER = 'CYLINDER'
SHAPE_CONE = 'CONE'
SHAPE_CAPSULE = 'CAPSULE'

DIMENSIONS = set(['half-extents'])

def vector_y_up(v, f):
        return '[%g,%g,%g]' % (f(v.x), f(v.y), f(v.z))

def vector_z_up(v, f):
        return '[%g,%g,%g]' % (f(v.x), f(v.z), f(-v.y))

def quaternion_z_up(q):
        return '[%g,%g,%g,%g]' % (q.w, q.x, q.y, q.z)

def quaternion_y_up(q):
        q = mathutils.Quaternion([1, 0, 0], math.pi) * q
        q = mathutils.Quaternion([0, 1, 0], math.pi) * q
        return '[%g,%g,%g,%g]' % (q.w, q.x, q.y, q.z)

def convert_properties(properties, y_up):
        results = {}
        vect_to_string = vector_y_up
        quat_to_string = quaternion_y_up
        if not y_up:
                vect_to_string = vector_z_up
                quat_to_string = quaternion_z_up
        for k, v in properties.items():
                f = float
                if k in DIMENSIONS:
                        f = abs
                if type(v) == mathutils.Vector:
                        results[k] = vect_to_string(v, f)
                elif type(v) == mathutils.Quaternion:
                        results[k] = quat_to_string(v)
                else:
                        results[k] = v
        return results

def get_shape(obj, y_up):
        shape = obj.rigid_body.collision_shape
        margin = obj.rigid_body.collision_margin
        bound_box = obj.bound_box.data
        mass = obj.rigid_body.mass
        if obj.rigid_body.type == 'PASSIVE':
                mass = 0.0
        properties = {'shape':shape, 'mass':mass, 'margin':margin}
        if shape == SHAPE_BOX:
                properties['half-extents'] = bound_box.dimensions / 2
        elif shape == SHAPE_SPHERE:
                properties['radius'] = max(bound_box.dimensions) / 2
        elif shape == SHAPE_CYLINDER:
                properties['half-extents'] = bound_box.dimensions / 2
        elif shape == SHAPE_CAPSULE:
                properties['radius'] = bound_box.dimensions.x / 2
                properties['height'] = bound_box.dimensions.y
        elif shape == SHAPE_CONE:
                properties['radius'] = bound_box.dimensions.x / 2
                properties['height'] = bound_box.dimensions.z
        properties['origin'] = bound_box.location
        properties['rotation'] = bound_box.matrix_local.to_quaternion()
        return convert_properties(properties, y_up)


class ExportBulletShape(bpy.types.Operator, bpy_extras.io_utils.ExportHelper):
        '''Export bullet shapes from rigid body.'''
        bl_idname = "object.export_bullet_shapes"
        bl_label = "Export Bullet Shapes"
        filename_file = "shape"
        filename_ext = ".yaml"
        filter_glob = bpy.props.StringProperty(default="*.yaml", options={'HIDDEN'})
        y_up = bpy.props.BoolProperty(name="Convert To Y-Up",
                        description="Converts the values to a Y-Axis Up coordinate system",
                        default=False)

        @classmethod
        def poll(cls, context):
                return any([obj.rigid_body for obj in bpy.data.objects])

        def process_obj(self, f, obj):
                if not obj.rigid_body:
                        return
                properties = get_shape(obj, self.y_up )
                for k, v in properties.items():
                        f.write( "%s: %s\n" % (k, v) )

        def execute(self, context):
                with open(self.filepath, "w") as f:
                        for obj in bpy.data.objects:
                                self.process_obj(f, obj)
                self.report({'INFO'}, "Export succeeded!")
                return {'FINISHED'}

        def invoke(self, context, event):
                wm = context.window_manager
                self.filename_file = bpy.path.display_name_from_filepath(bpy.data.filepath) + self.filename_ext
                self.filepath = os.path.join(os.path.dirname(bpy.data.filepath), self.filename_file)
                wm.fileselect_add(self)
                return {'RUNNING_MODAL'}

def menu_export_physics(self, context):
        self.layout.operator(ExportBulletShape.bl_idname, text="Bullet Shapes (.yaml)")

def register():
        bpy.utils.register_module(__name__)
        bpy.types.INFO_MT_file_export.remove(menu_export_physics)
        bpy.types.INFO_MT_file_export.prepend(menu_export_physics)

def unregister():
        bpy.utils.unregister_module(__name__)
        bpy.types.INFO_MT_file_export.remove(menu_export_physics)

if __name__ == '__main__':
        register()
