import os

flags = [
'-x', 'c++'
, '-Wall'
, '-Wextra'
, '-std=c++11'
, '-I', '.'
, '-I', './include'
, '-I', '/usr/include/yaml-cpp'
, '-I', '/usr/include/GL'
, '-I', '/usr/include/libdrm'
, '-I', '/usr/include/SDL2'
, '-I', '/usr/include/SDL'
, '-I', '/usr/include/bullet'
, '-I', '/usr/include/libxml2'
, '-I', '/usr/include/opencollada/COLLADABaseUtils/'
, '-I', '/usr/include/opencollada/COLLADAFramework/'
, '-I', '/usr/include/opencollada/COLLADASaxFrameworkLoader/'
, '-I', '/usr/include/opencollada/COLLADAStreamWriter/'
]

def DirectoryOfThisScript():
  return os.path.dirname( os.path.abspath( __file__ ) )


def MakeRelativePathsInFlagsAbsolute( flags, working_directory ):
  if not working_directory:
    return flags
  new_flags = []
  make_next_absolute = False
  path_flags = [ '-isystem', '-I', '-iquote', '--sysroot=' ]
  for flag in flags:
    new_flag = flag

    if make_next_absolute:
      make_next_absolute = False
      if not flag.startswith( '/' ):
        new_flag = os.path.join( working_directory, flag )

    for path_flag in path_flags:
      if flag == path_flag:
        make_next_absolute = True
        break

      if flag.startswith( path_flag ):
        path = flag[ len( path_flag ): ]
        new_flag = path_flag + os.path.join( working_directory, path )
        break

    if new_flag:
      new_flags.append( new_flag )
  return new_flags


def FlagsForFile( filename ):
  relative_to = DirectoryOfThisScript()
  final_flags = MakeRelativePathsInFlagsAbsolute( flags, relative_to )

  return {
    'flags': final_flags,
    'do_cache': True
  }
